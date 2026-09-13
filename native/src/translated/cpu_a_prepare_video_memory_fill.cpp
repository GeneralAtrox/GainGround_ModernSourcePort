#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kMainRamRegion = 3U;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, 0xffffU);
}

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t address)
{
    registers.address[7] -= 4U;
    const auto stack = registers.address[7] & 0x0003ffffU;
    host.write_memory_word(kMainRamRegion, stack,
                           static_cast<std::uint16_t>(address >> 16U), 0xffffU);
    host.write_memory_word(kMainRamRegion, (stack + 2U) & 0x0003ffffU,
                           static_cast<std::uint16_t>(address), 0xffffU);
}
} // namespace

FunctionResult cpu_a_prepare_video_memory_fill(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.data[6] = (registers.data[6] & 0xffff0000U) | 0x0080U;
    set_move_word_flags(registers, 0x0080U);
    prefetch(host, 0x00003862U);
    prefetch(host, 0x00003864U);

    const auto source = host.read_memory_word(
        kProgramRegion, registers.address[4], 0xffffU);
    registers.address[4] += 2U;
    registers.address[3] = static_cast<std::uint32_t>(static_cast<std::int32_t>(
        static_cast<std::int16_t>(source)));
    prefetch(host, 0x00003866U);
    prefetch(host, 0x00003868U);
    registers.address[3] += 0x00204000U;
    prefetch(host, 0x0000386aU);
    prefetch(host, 0x0000386cU);

    push_return(host, registers, 0x0000386cU);
    prefetch(host, 0x0000386eU);
    prefetch(host, 0x00003870U);
    registers.program_counter = 0x0000386eU;
    const auto child = host.call_function(
        44U, 0U, 0xffU, 2U, 0x0000386aU, 0x0000386eU, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    prefetch(host, 0x00003870U);
    registers.program_counter = 0x0000386eU;
    (void)host.call_function(
        44U, 0U, 0xffU, 0U, 0x0000386cU, 0x0000386eU, context);
    return FunctionResult::complete(3U, 0x0000386eU);
}

} // namespace gain_ground::translated
