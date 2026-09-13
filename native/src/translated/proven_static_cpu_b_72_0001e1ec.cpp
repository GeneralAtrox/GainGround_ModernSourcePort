#include "gain_ground/contract_types.h"
#include "gground_functions.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
void set_add_word_flags(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(left) + right > 0xffffU)
        flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if ((result & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (result == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001e1ec(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &registers = context.registers;
    const auto left = static_cast<std::uint16_t>(registers.data[1]);
    const auto right = static_cast<std::uint16_t>(registers.data[7]);
    const auto sum = static_cast<std::uint16_t>(left + right);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | sum;
    set_add_word_flags(registers, left, right, sum);

    const auto counter = static_cast<std::uint16_t>(registers.data[0] - 1U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
    if (counter != 0xffffU) {
        registers.program_counter = 0x0001e1ecU;
        const auto boundary = context.host->call_function(597U, 1U, 0x72U, 1U,
            0x0001e1eeU, 0x0001e1ecU, context);
        if (boundary.status != TranslationStatus::complete)
            return boundary;
        if (boundary.control == 3U
            && boundary.exit_program_counter == 0x0001e1ecU)
            return FunctionResult::complete(4U, 0x0001e1ecU);
        return boundary;
    }

    registers.program_counter = 0x0001e1f2U;
    const auto child = context.host->call_function(554U, 1U, 0x72U, 1U,
        0x0001e1eeU, 0x0001e1f2U, context);
    if (child.status != TranslationStatus::complete)
        return child;
    if (child.control == 3U
        && child.exit_program_counter != 0x0001e1f2U)
        return proven_static_cpu_b_72_0001e22a(context);
    return child;
}

} // namespace gain_ground::translated
