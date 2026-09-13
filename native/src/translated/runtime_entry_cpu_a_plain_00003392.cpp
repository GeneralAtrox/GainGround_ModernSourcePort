#include "gain_ground/contract_types.h"

#include <array>
#include <cstdint>

namespace gain_ground::translated {
FunctionResult cpu_a_ym2151_write_register(FunctionContext &context) noexcept;

namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kMainRamRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kMainRamMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign_mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign_mask) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_shift_flags(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign_mask, bool carry)
{
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if ((value & sign_mask) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_flags(CpuRegisters &registers, std::uint32_t left,
    std::uint32_t right, std::uint32_t result, std::uint32_t mask,
    std::uint32_t sign_mask)
{
    const auto wide = static_cast<std::uint64_t>(left & mask)
        + static_cast<std::uint64_t>(right & mask);
    std::uint16_t flags{};
    if ((result & sign_mask) != 0U) flags |= 0x0008U;
    if ((result & mask) == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & sign_mask) != 0U)
        flags |= 0x0002U;
    if (wide > mask) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    const auto offset = registers.address[7] & kMainRamMask;
    host.write_memory_word(kMainRamRegion, offset,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kMainRamRegion, offset + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

void call_subroutine(FunctionContext &context, std::uint32_t function_id,
    std::uint32_t callsite, std::uint32_t target, std::uint32_t return_address)
{
    auto &host = *context.host;
    push_return(host, context.registers, return_address);
    prefetch(host, target);
    prefetch(host, target + 2U);
    context.registers.program_counter = target;
    auto result = host.call_function(function_id, 0U, 0xffU, 2U,
        callsite, target, context);
    while (function_id == 40U
        && result.status == TranslationStatus::complete
        && result.control == 4U) {
        result = cpu_a_ym2151_write_register(context);
    }
}

void call_indexed_subroutine(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t return_address)
{
    auto &host = *context.host;
    prefetch(host, target);
    push_return(host, context.registers, return_address);
    prefetch(host, target + 2U);
    context.registers.program_counter = target;
    (void)host.call_function(function_id, 0U, 0xffU, 2U,
        callsite, target, context);
}

[[nodiscard]] std::uint8_t read_program_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const auto offset = address & kMainRamMask;
    const auto mask = static_cast<std::uint16_t>(
        (offset & 1U) == 0U ? 0xff00U : 0x00ffU);
    const auto shift = (offset & 1U) == 0U ? 8U : 0U;
    return static_cast<std::uint8_t>(host.read_memory_word(
        kProgramRegion, offset & ~1U, mask) >> shift);
}
} // namespace

FunctionResult runtime_entry_cpu_a_plain_00003392(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    auto word0 = static_cast<std::uint16_t>(registers.data[0]);
    const bool shift_word_carry = (word0 & 0x0800U) != 0U;
    word0 = static_cast<std::uint16_t>(word0 << 5U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | word0;
    set_shift_flags(registers, word0, 0x8000U, shift_word_carry);
    prefetch(host, 0x00003396U);

    registers.data[1] = 0U;
    set_logic_flags(registers, 0U, 0x80000000U);
    prefetch(host, 0x00003398U);
    registers.data[1] = word0;
    set_logic_flags(registers, word0, 0x8000U);
    prefetch(host, 0x0000339aU);

    word0 &= 0x03ffU;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | word0;
    set_logic_flags(registers, word0, 0x8000U);
    prefetch(host, 0x0000339cU);
    prefetch(host, 0x0000339eU);

    auto word1 = static_cast<std::uint16_t>(registers.data[1]) & 0xfc00U;
    registers.data[1] = (registers.data[1] & 0xffff0000U) | word1;
    set_logic_flags(registers, word1, 0x8000U);
    prefetch(host, 0x000033a0U);

    const auto long1_before_shift = registers.data[1];
    const bool shift_long_carry = (long1_before_shift & 0x40000000U) != 0U;
    registers.data[1] = long1_before_shift << 2U;
    set_shift_flags(registers, registers.data[1], 0x80000000U,
        shift_long_carry);
    prefetch(host, 0x000033a2U);

    word1 = static_cast<std::uint16_t>(registers.data[1]);
    const auto word_sum = static_cast<std::uint16_t>(word1 + word0);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | word_sum;
    set_add_flags(registers, word1, word0, word_sum,
        0xffffU, 0x8000U);
    prefetch(host, 0x000033a4U);

    const auto long1 = registers.data[1];
    registers.data[1] = long1 + 0x00020000U;
    set_add_flags(registers, long1, 0x00020000U, registers.data[1],
        0xffffffffU, 0x80000000U);
    prefetch(host, 0x000033a6U);
    prefetch(host, 0x000033a8U);
    prefetch(host, 0x000033aaU);
    registers.address[0] = registers.data[1];
    prefetch(host, 0x000033acU);

    registers.data[0] = (registers.data[0] & 0xffffff00U) | 0x20U;
    set_logic_flags(registers, 0x20U, 0x80U);
    prefetch(host, 0x000033aeU);
    prefetch(host, 0x000033b0U);
    const auto d7_byte = static_cast<std::uint8_t>(registers.data[7]);
    auto d0_byte = static_cast<std::uint8_t>(0x20U + d7_byte);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | d0_byte;
    set_add_flags(registers, 0x20U, d7_byte, d0_byte, 0xffU, 0x80U);
    prefetch(host, 0x000033b2U);
    registers.data[1] = 0U;
    set_logic_flags(registers, 0U, 0x80000000U);
    prefetch(host, 0x000033b4U);
    prefetch(host, 0x000033b6U);
    call_subroutine(context, 40U, 0x000033b4U, 0x000034e6U,
        0x000033b8U);

    const auto first_register = static_cast<std::uint8_t>(registers.data[0]);
    d0_byte = static_cast<std::uint8_t>(first_register + 0x18U);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | d0_byte;
    set_add_flags(registers, first_register, 0x18U, d0_byte, 0xffU, 0x80U);
    prefetch(host, 0x000033bcU);
    prefetch(host, 0x000033beU);
    call_subroutine(context, 40U, 0x000033bcU, 0x000034e6U,
        0x000033c0U);

    const auto table_byte = read_program_byte(host, registers.address[0]++);
    registers.data[3] = (registers.data[3] & 0xffffff00U) | table_byte;
    set_logic_flags(registers, table_byte, 0x80U);
    prefetch(host, 0x000033c4U);
    registers.data[0] = 0x40U;
    set_logic_flags(registers, 0x40U, 0x80000000U);
    prefetch(host, 0x000033c6U);
    d0_byte = static_cast<std::uint8_t>(0x40U + d7_byte);
    registers.data[0] = d0_byte;
    set_add_flags(registers, 0x40U, d7_byte, d0_byte, 0xffU, 0x80U);
    prefetch(host, 0x000033c8U);
    registers.data[2] = 3U;
    set_logic_flags(registers, 3U, 0x80000000U);
    prefetch(host, 0x000033caU);
    call_subroutine(context, 513U, 0x000033c8U, 0x000034bcU,
        0x000033ccU);

    registers.data[1] = (registers.data[1] & 0xffffff00U) | table_byte;
    set_logic_flags(registers, table_byte, 0x80U);
    prefetch(host, 0x000033d0U);
    prefetch(host, 0x000033d2U);
    word1 = static_cast<std::uint16_t>(registers.data[1]) & 7U;
    registers.data[1] = (registers.data[1] & 0xffff0000U) | word1;
    set_logic_flags(registers, word1, 0x8000U);
    prefetch(host, 0x000033d4U);
    auto doubled = static_cast<std::uint16_t>(word1 + word1);
    set_add_flags(registers, word1, word1, doubled, 0xffffU, 0x8000U);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | doubled;
    prefetch(host, 0x000033d6U);
    word1 = doubled;
    doubled = static_cast<std::uint16_t>(word1 + word1);
    set_add_flags(registers, word1, word1, doubled, 0xffffU, 0x8000U);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | doubled;
    prefetch(host, 0x000033d8U);

    constexpr std::array<std::uint32_t, 8> kSelectorFunctions{
        501U, 502U, 503U, 504U, 505U, 506U, 507U, 508U};
    const auto selector = static_cast<std::uint32_t>(doubled >> 2U);
    const auto selector_target = 0x0000341aU + doubled;
    call_indexed_subroutine(context, kSelectorFunctions[selector], 0x000033d6U,
        selector_target, 0x000033daU);

    registers.data[2] = 15U;
    set_logic_flags(registers, 15U, 0x80000000U);
    prefetch(host, 0x000033deU);
    call_subroutine(context, 513U, 0x000033dcU, 0x000034bcU,
        0x000033e0U);

    registers.data[0] = (registers.data[0] & 0xffffff00U) | 0x20U;
    set_logic_flags(registers, 0x20U, 0x80U);
    prefetch(host, 0x000033e4U);
    d0_byte = static_cast<std::uint8_t>(0x20U + d7_byte);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | d0_byte;
    set_add_flags(registers, 0x20U, d7_byte, d0_byte, 0xffU, 0x80U);
    prefetch(host, 0x000033e6U);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | table_byte;
    set_logic_flags(registers, table_byte, 0x80U);
    prefetch(host, 0x000033e8U);
    prefetch(host, 0x000033eaU);
    prefetch(host, 0x000034e6U);
    prefetch(host, 0x000034e8U);
    registers.program_counter = 0x000034e6U;
    return host.call_function(40U, 0U, 0xffU, 1U,
        0x000033e8U, 0x000034e6U, context);
}

} // namespace gain_ground::translated
