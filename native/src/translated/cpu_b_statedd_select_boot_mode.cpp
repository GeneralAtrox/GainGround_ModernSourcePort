#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] ByteLocation locate_byte(std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void set_logic_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void compare_long(CpuRegisters &registers, std::uint32_t source)
{
    const auto destination = registers.data[0];
    const auto result = destination - source;
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result) & 0x80000000U) != 0U)
        flags |= 0x0002U;
    if (destination < source) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_b_statedd_select_boot_mode(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    (void)read_byte(host, 0x00000419U);
    write_byte(host, 0x00000419U, 0U);
    set_logic_byte_flags(registers, 0U);

    const auto inputs = read_byte(host, 0x00000403U);
    if ((inputs & 0x01U) == 0U)
        registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
    else
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);

    if ((inputs & 0x01U) != 0U) {
        write_byte(host, 0x00000419U, 2U);
        set_logic_byte_flags(registers, 2U);
    }

    compare_long(registers, 0x0072ffffU);
    registers.program_counter = 0x000084f2U;
    (void)host.call_function(115U, 1U, 0x72U, 6U,
        0x000084ecU, 0x000084f2U, context);
    return FunctionResult::complete(5U, 0x000084f2U);
}

} // namespace gain_ground::translated
