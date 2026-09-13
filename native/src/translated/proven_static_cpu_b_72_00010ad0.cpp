#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void push_return_address(FunctionContext &context,
                         std::uint32_t return_address) noexcept
{
    auto &registers = context.registers;
    registers.address[7] -= 4U;
    context.host->write_memory_word(kPrivateRegion, registers.address[7],
        static_cast<std::uint16_t>(return_address >> 16U), kWordMask);
    context.host->write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(return_address), kWordMask);
}
} // namespace

FunctionResult proven_static_cpu_b_72_00010ad0(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    push_return_address(context, 0x00010ad4U);
    context.registers.program_counter = 0x00010b28U;
    const auto child = context.host->call_function(
        215U, 1U, 0x72U, 2U, 0x00010ad0U, 0x00010b28U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    context.registers.program_counter = 0x00010b02U;
    return context.host->call_function(
        533U, 1U, 0x72U, 1U, 0x00010ad4U, 0x00010b02U, context);
}

} // namespace gain_ground::translated
