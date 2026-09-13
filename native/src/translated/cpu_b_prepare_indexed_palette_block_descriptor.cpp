#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

void set_compare_word(CpuRegisters &registers,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (right > left) flags |= 0x0001U;
    flags |= registers.status & 0x0010U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_asl_two_word(CpuRegisters &registers, std::uint16_t original, std::uint16_t result)
{
    auto shifted = original;
    bool carry{};
    bool overflow{};
    for (unsigned count = 0; count != 2U; ++count) {
        const bool old_sign = (shifted & 0x8000U) != 0U;
        carry = old_sign;
        shifted = static_cast<std::uint16_t>(shifted << 1U);
        overflow = overflow || (old_sign != ((shifted & 0x8000U) != 0U));
    }
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if (overflow) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_logic_long(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_b_prepare_indexed_palette_block_descriptor(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[0] = 0x0000745cU;

    host.write_memory_word(kRegion, registers.address[0], 0x0050U, kWordMask);
    registers.address[0] += 2U;
    host.write_memory_word(kRegion, registers.address[0], 0x00b0U, kWordMask);
    registers.address[0] += 2U;

    const auto palette_index = host.read_memory_word(
        kRegion, registers.address[5] + 0x1aU, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | palette_index;
    const auto comparison = static_cast<std::uint16_t>(palette_index - 10U);
    set_compare_word(registers, palette_index, 10U, comparison);
    if (palette_index == 10U)
        host.write_memory_word(kRegion, registers.address[0] - 2U, 0x00a0U, kWordMask);

    host.write_memory_word(kRegion, registers.address[0], 0x0803U, kWordMask);
    registers.address[0] += 2U;

    const auto scaled = static_cast<std::uint16_t>(palette_index << 2U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | scaled;
    set_asl_two_word(registers, palette_index, scaled);
    registers.address[1] = 0x00024e6eU;

    const auto table_value = read_long(host,
        registers.address[1] + static_cast<std::int16_t>(scaled));
    write_long(host, registers.address[0], table_value);
    registers.address[0] += 4U;
    set_logic_long(registers, table_value);

    const auto return_address = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
