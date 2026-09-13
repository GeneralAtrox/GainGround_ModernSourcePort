#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kSharedOffsetMask = 0x0003ffffU;

void prefetch_word(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign_mask)
{
    std::uint16_t flags = static_cast<std::uint16_t>(registers.status & 0x0010U);
    if ((value & sign_mask) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_long_flags(CpuRegisters &registers, std::uint32_t left,
    std::uint32_t right, std::uint32_t result)
{
    std::uint16_t flags{};
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x80000000U) != 0U) flags |= 0x0002U;
    if (result < left) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t pop_long(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto offset = registers.address[7] & kSharedOffsetMask;
    const auto value = (static_cast<std::uint32_t>(
        host.read_memory_word(kSharedRegion, offset, kWordMask)) << 16U)
        | host.read_memory_word(kSharedRegion,
            (offset + 2U) & kSharedOffsetMask, kWordMask);
    registers.address[7] += 4U;
    return value;
}
} // namespace

FunctionResult proven_static_cpu_a_plain_000017a4(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x000017a4U)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.data[6] = registers.data[0];

    prefetch_word(host, 0x000017a8U);
    prefetch_word(host, 0x000017aaU);
    prefetch_word(host, 0x000017acU);
    prefetch_word(host, 0x000017aeU);
    host.write_hardware(2U, 0U, 0xffU, 0x000017a6U,
        0x00bc0000U, 0U, kWordMask);
    set_logic_flags(registers, 0U, 0x00008000U);

    registers.data[0] = 0U;
    set_logic_flags(registers, 0U, 0x80000000U);

    prefetch_word(host, 0x000017b0U);
    prefetch_word(host, 0x000017b2U);
    prefetch_word(host, 0x000017b4U);
    prefetch_word(host, 0x000017b6U);
    const auto raw_track = host.read_hardware(1U, 0U, 0xffU,
        0x000017b0U, 0x00b00000U, 0xff00U);
    const auto track = static_cast<std::uint8_t>(raw_track >> 8U);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | track;
    set_logic_flags(registers, track, 0x00000080U);

    prefetch_word(host, 0x000017b8U);
    registers.data[0] = static_cast<std::uint32_t>(
        static_cast<std::uint16_t>(registers.data[0])) * 12U;
    set_logic_flags(registers, registers.data[0], 0x80000000U);

    prefetch_word(host, 0x000017baU);
    prefetch_word(host, 0x000017bcU);
    const auto add_left = static_cast<std::uint16_t>(registers.data[0]);
    const auto add_result = static_cast<std::uint16_t>(add_left + 0x008aU);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | add_result;
    set_add_word_flags(registers, add_left, 0x008aU, add_result);

    prefetch_word(host, 0x000017beU);
    const auto d6_before_add = registers.data[6];
    registers.data[6] += registers.data[0];
    set_add_long_flags(registers, d6_before_add, registers.data[0],
        registers.data[6]);

    prefetch_word(host, 0x000017c0U);
    registers.data[7] = registers.data[6];
    set_logic_flags(registers, registers.data[7], 0x80000000U);

    prefetch_word(host, 0x000017c2U);
    registers.data[7] = (registers.data[7] << 16U)
        | (registers.data[7] >> 16U);
    set_logic_flags(registers, registers.data[7], 0x80000000U);

    prefetch_word(host, 0x000017c4U);
    const auto shift_source = static_cast<std::uint16_t>(registers.data[7]);
    const auto shifted = static_cast<std::uint16_t>(shift_source >> 2U);
    registers.data[7] = (registers.data[7] & 0xffff0000U) | shifted;
    std::uint16_t shift_flags{};
    if (shifted == 0U) shift_flags |= 0x0004U;
    if ((shift_source & 0x0002U) != 0U) shift_flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | shift_flags);

    prefetch_word(host, 0x000017c6U);
    prefetch_word(host, 0x000017c8U);
    prefetch_word(host, 0x000017caU);
    registers.data[6] &= 0xfffc0000U;
    set_logic_flags(registers, registers.data[6], 0x80000000U);

    prefetch_word(host, 0x000017ccU);
    prefetch_word(host, 0x000017ceU);
    const auto return_address = pop_long(host, registers);
    prefetch_word(host, return_address);
    prefetch_word(host, return_address + 2U);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
