#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] constexpr ByteLocation locate(std::uint32_t address) noexcept
{
    return {address & ~1U,
        static_cast<std::uint16_t>((address & 1U) != 0U ? 0x00ffU : 0xff00U),
        (address & 1U) != 0U ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kPrivateRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate(address);
    host.write_memory_word(kPrivateRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kPrivateRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

void set_logic_long(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(
        kPrivateRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_read_record_byte_6d(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    const auto record_value = read_byte(host, registers.address[5] + 0x6dU);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | record_value;

    auto bits = read_byte(host, 0x0820U);
    bits = static_cast<std::uint8_t>(bits | (1U << (record_value & 7U)));
    write_byte(host, 0x0820U, bits);

    auto count = read_byte(host, 0x0821U);
    write_byte(host, 0x0821U, static_cast<std::uint8_t>(count + 1U));

    const auto record_pointer = host.read_memory_word(
        kPrivateRegion, registers.address[5] + 0x6eU, kWordMask);
    registers.address[0] = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(record_pointer)));
    write_long(host, registers.address[0], 0x00730000U);

    auto phase = host.read_memory_word(kPrivateRegion, 0x7b30U, kWordMask);
    host.write_memory_word(kPrivateRegion, 0x7b30U,
        static_cast<std::uint16_t>(phase + 1U), kWordMask);

    // User-selected unlimited credits preserve the balance and partial coins.
    if (!host.unlimited_credits()) {
        const auto active = read_byte(host, registers.address[6]);
        write_byte(host, registers.address[6], static_cast<std::uint8_t>(active - 1U));
        (void)read_byte(host, registers.address[6] + 1U);
        write_byte(host, registers.address[6] + 1U, 0U);
        (void)read_byte(host, registers.address[6] + 2U);
        write_byte(host, registers.address[6] + 2U, 0U);
    }

    host.write_memory_word(kPrivateRegion, registers.address[4], 0x0303U, kWordMask);
    write_byte(host, registers.address[4] + 2U, 0U);
    write_byte(host, registers.address[4] + 3U, 8U);
    write_byte(host, registers.address[4] + 4U, 0x11U);

    registers.data[1] = 0U;
    host.write_memory_word(kPrivateRegion, registers.address[4] + 0x40U, 0U, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x005fU;
    registers.address[0] = registers.address[4] + 0x80U;
    for (;;) {
        write_long(host, registers.address[0], registers.data[1]);
        set_logic_long(registers, registers.data[1]);
        registers.address[0] += 4U;
        const auto loop_count = static_cast<std::uint16_t>(registers.data[0]);
        registers.data[0] = (registers.data[0] & 0xffff0000U)
            | static_cast<std::uint16_t>(loop_count - 1U);
        if (loop_count == 0U) break;
    }

    write_long(host, registers.address[5] + 2U, 0x0000ee6eU);
    set_logic_long(registers, 0x0000ee6eU);

    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
