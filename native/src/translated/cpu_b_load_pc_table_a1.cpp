#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
FunctionResult cpu_b_load_pc_table_a1_alt(FunctionContext &context) noexcept;

namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kPrivateMask = 0x0003ffffU;

void push_return(
    ExecutionHost &host, CpuRegisters &registers, std::uint32_t target)
{
    registers.address[7] -= 4U;
    const auto stack = registers.address[7] & kPrivateMask;
    host.write_memory_word(kPrivateRegion, stack,
        static_cast<std::uint16_t>(target >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, (stack + 2U) & kPrivateMask,
        static_cast<std::uint16_t>(target), kWordMask);
}
} // namespace

FunctionResult cpu_b_load_pc_table_a1(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    if (registers.program_counter != 0x0000fb3aU)
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};

    registers.address[1] = 0x00010638U;
    push_return(host, registers, 0x0000fb44U);
    registers.program_counter = 0x00015f96U;
    const auto child = host.call_function(289U, 1U, 0x72U, 2U,
        0x0000fb3eU, 0x00015f96U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    registers.program_counter = 0x0000fb44U;
    return cpu_b_load_pc_table_a1_alt(context);
}

} // namespace gain_ground::translated
