#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
FunctionResult cpu_b_dispatch_descriptor_state(FunctionContext &context) noexcept;

namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t target)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kRegion, registers.address[7],
        static_cast<std::uint16_t>(target >> 16U), kWordMask);
    host.write_memory_word(kRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(target), kWordMask);
}
} // namespace

FunctionResult cpu_b_dispatch_descriptor_update(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &registers = context.registers;
    if (registers.program_counter != 0x00013756U)
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};

    push_return(*context.host, registers, 0x0001375aU);
    registers.program_counter = 0x0001386aU;
    const auto child = context.host->call_function(270U, 1U, 0x72U, 2U,
        0x00013756U, 0x0001386aU, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    registers.program_counter = 0x0001375aU;
    return cpu_b_dispatch_descriptor_state(context);
}

} // namespace gain_ground::translated
