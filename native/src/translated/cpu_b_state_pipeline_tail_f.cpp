#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(
        kRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

[[nodiscard]] FunctionResult call_child(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t continuation)
{
    push_return(*context.host, context.registers, continuation);
    context.registers.program_counter = target;
    return context.host->call_function(function_id, 1U, 0x72U, 2U,
        callsite, target, context);
}

[[nodiscard]] bool complete(const FunctionResult &result) noexcept
{
    return result.status == TranslationStatus::complete
        && result.control == 1U;
}
} // namespace

FunctionResult cpu_b_state_pipeline_tail_f(FunctionContext &context) noexcept
{
    if (context.host == nullptr
        || context.registers.program_counter != 0x00020d4eU)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto result = call_child(context, 327U,
        0x00020d4eU, 0x0001bdc0U, 0x00020d52U);
    if (!complete(result)) return result;

    result = call_child(context, 373U,
        0x00020d52U, 0x0001f3a8U, 0x00020d56U);
    if (!complete(result)) return result;

    result = call_child(context, 361U,
        0x00020d56U, 0x0001e3a6U, 0x00020d5aU);
    if (!complete(result)) return result;

    const auto target = pop_return(*context.host, context.registers);
    context.registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
