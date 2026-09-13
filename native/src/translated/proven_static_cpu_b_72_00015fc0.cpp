#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_word(CpuRegisters &registers, std::uint16_t value) noexcept
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (value == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult proven_static_cpu_b_72_00015fc0(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    const auto count = host.read_memory_word(
        kPrivateRegion, registers.address[1] & 0x0003ffffU, kWordMask);
    registers.address[1] += 2U;
    registers.data[2] = (registers.data[2] & 0xffff0000U) | count;
    set_logic_word(registers, count);

    constexpr std::uint32_t kNextPc = 0x00015fc2U;
    registers.program_counter = kNextPc;
    (void)host.call_function(
        474U, 1U, 0x72U, 0U, 0x00015fc0U, kNextPc, context);
    return FunctionResult::complete(3U, kNextPc);
}

} // namespace gain_ground::translated
