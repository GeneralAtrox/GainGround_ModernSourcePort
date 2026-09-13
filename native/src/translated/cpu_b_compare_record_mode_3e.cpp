#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] constexpr ByteLocation locate(std::uint32_t address) noexcept
{
    return {address & ~1U, static_cast<std::uint16_t>((address & 1U) ? 0x00ffU : 0xff00U),
        (address & 1U) ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, ByteLocation location)
{
    return static_cast<std::uint8_t>(
        host.read_memory_word(kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, ByteLocation location, std::uint8_t value)
{
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void set_logic_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    const std::uint16_t flags = static_cast<std::uint16_t>(
        (registers.status & 0x0010U) | ((value & 0x80U) ? 0x0008U : 0U) | (value == 0U ? 0x0004U : 0U));
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_compare_byte_flags(CpuRegisters &registers, std::uint8_t destination, std::uint8_t source)
{
    const auto result = static_cast<std::uint8_t>(destination - source);
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if (destination < source) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void clear_byte(ExecutionHost &host, CpuRegisters &registers, ByteLocation location)
{
    (void)read_byte(host, location);
    write_byte(host, location, 0U);
    set_logic_byte_flags(registers, 0U);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

} // namespace

FunctionResult cpu_b_compare_record_mode_3e(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    const std::uint32_t base = context.registers.address[5];
    const auto mode = locate(base + 0x3eU);
    const auto value = read_byte(host, mode);
    set_compare_byte_flags(context.registers, value, 2U);
    if (value < 2U) {
        write_byte(host, mode, 2U);
        set_logic_byte_flags(context.registers, 2U);
        clear_byte(host, context.registers, locate(base + 0x3cU));
        clear_byte(host, context.registers, locate(base + 0x3dU));
    }
    const auto return_address = read_long(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
