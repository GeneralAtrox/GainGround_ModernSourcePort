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

FunctionResult cpu_b_state_pipeline_tail_b(FunctionContext &context) noexcept
{
    if (context.host == nullptr
        || context.registers.program_counter != 0x00020918U)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto result = call_child(context, 331U,
        0x00020918U, 0x0001c37eU, 0x0002091cU);
    if (!complete(result)) return result;

    result = call_child(context, 368U,
        0x0002091cU, 0x0001efdcU, 0x00020920U);
    if (!complete(result)) return result;

    result = call_child(context, 356U,
        0x00020920U, 0x0001e0eeU, 0x00020924U);
    if (!complete(result)) return result;

    result = call_child(context, 352U,
        0x00020924U, 0x0001dbc4U, 0x00020928U);
    if (!complete(result)) return result;

    result = call_child(context, 353U,
        0x00020928U, 0x0001dbf0U, 0x0002092cU);
    if (!complete(result)) return result;

    result = call_child(context, 355U,
        0x0002092cU, 0x0001de30U, 0x00020930U);
    if (!complete(result)) return result;

    result = call_child(context, 357U,
        0x00020930U, 0x0001e124U, 0x00020934U);
    if (!complete(result)) return result;

    result = call_child(context, 360U,
        0x00020934U, 0x0001e304U, 0x00020938U);
    if (!complete(result)) return result;

    const auto target = pop_return(*context.host, context.registers);
    context.registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
