#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void push_long(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] FunctionResult call_child(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t return_address)
{
    push_long(*context.host, context.registers, return_address);
    context.registers.program_counter = target;
    return context.host->call_function(function_id, 1U, 0x72U, 2U,
        callsite, target, context);
}

[[nodiscard]] FunctionResult finish(FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto high = host.read_memory_word(
        kPrivateRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_run_descriptor_update_pair(FunctionContext &context) noexcept
{
    if (!context.host) return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    FunctionResult character_result;
    if (context.host->run_character_attacks(context, character_result)) return character_result;
    auto &r = context.registers;
    for (;;) {
        switch (r.program_counter) {
        case 0x10a24U: case 0x10a28U: {
            const auto site = r.program_counter;
            const bool primary = site == 0x10a24U;
            const auto continuation = primary ? 0x10a28U : 0x10a2cU;
            const auto child = call_child(context, primary ? 209U : 224U, site,
                primary ? 0x10a2eU : 0x10cc0U, continuation);
            if (child.status != TranslationStatus::complete || child.control != 1U ||
                r.program_counter != continuation) return child;
            break;
        }
        case 0x10a2cU: return finish(context);
        default: return {TranslationStatus::contract_violation, 0U, r.program_counter};
        }
    }
}

} // namespace gain_ground::translated
