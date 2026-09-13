#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_flags(CpuRegisters &registers, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    const auto wide = static_cast<std::uint32_t>(left) + right;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (wide > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void rotate_left_four(CpuRegisters &registers)
{
    const auto value = registers.data[2];
    const auto result = static_cast<std::uint32_t>((value << 4U) | (value >> 28U));
    registers.data[2] = result;
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if ((value & 0x10000000U) != 0U) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x000fU) | flags);
}

void bset_sign_marker(CpuRegisters &registers)
{
    const bool was_set = (registers.data[3] & 0x80000000U) != 0U;
    registers.data[3] |= 0x80000000U;
    if (was_set) registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);
    else registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
}

void increment_d0_word(CpuRegisters &registers)
{
    const auto left = static_cast<std::uint16_t>(registers.data[0]);
    const auto result = static_cast<std::uint16_t>(left + 1U);
    set_add_word_flags(registers, left, 1U, result);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
}

void write_d0(ExecutionHost &host, CpuRegisters &registers, bool postincrement)
{
    const auto value = static_cast<std::uint16_t>(registers.data[0]);
    host.write_memory_word(kTileRegion, registers.address[0] - 0x00200000U, value, kWordMask);
    if (postincrement) registers.address[0] += 2U;
    set_logic_flags(registers, value, 0x8000U);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack = registers.address[7];
    const auto high = host.read_memory_word(kPrivateRegion, stack, kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion, stack + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_emit_tilemap_a_incrementing_pairs_with_priority(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    for (;;) {
        rotate_left_four(registers);
        const auto rotated_word = static_cast<std::uint16_t>(registers.data[2]);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | rotated_word;
        set_logic_flags(registers, rotated_word, 0x8000U);
        const auto nibble = static_cast<std::uint16_t>(rotated_word & 0x000fU);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | nibble;
        set_logic_flags(registers, nibble, 0x8000U);
        bool emit_zero{};
        if (nibble != 0U) {
            bset_sign_marker(registers);
        } else {
            const auto d3_word = static_cast<std::uint16_t>(registers.data[3]);
            set_logic_flags(registers, d3_word, 0x8000U);
            if (d3_word != 0U) {
                set_logic_flags(registers, registers.data[3], 0x80000000U);
                if ((registers.data[3] & 0x80000000U) == 0U) emit_zero = true;
                else bset_sign_marker(registers);
            }
        }
        if (emit_zero) {
            registers.data[0] = 0U;
            set_logic_flags(registers, 0U, 0x80000000U);
        } else {
            const auto before_add = static_cast<std::uint16_t>(registers.data[0]);
            const auto added = static_cast<std::uint16_t>(before_add + 0x0030U);
            registers.data[0] = (registers.data[0] & 0xffff0000U) | added;
            set_add_word_flags(registers, before_add, 0x0030U, added);
            const auto doubled = static_cast<std::uint16_t>(added << 1U);
            set_add_word_flags(registers, added, added, doubled);
            registers.data[0] = (registers.data[0] & 0xffff0000U) | doubled;
        }
        const auto prioritized = static_cast<std::uint16_t>(registers.data[0] | 0x8000U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | prioritized;
        set_logic_flags(registers, prioritized, 0x8000U);
        write_d0(host, registers, true);
        increment_d0_word(registers);
        write_d0(host, registers, false);
        registers.address[0] += 0x7eU;
        const auto counter = static_cast<std::uint16_t>(registers.data[3] - 1U);
        registers.data[3] = (registers.data[3] & 0xffff0000U) | counter;
        if (counter != 0xffffU) {
            registers.program_counter = 0x000160c8U;
            (void)host.call_function(294U, 1U, 0x72U, 1U,
                0x000160f6U, 0x000160c8U, context);
            return FunctionResult::complete(4U, 0x000160c8U);
        }
        break;
    }

    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
