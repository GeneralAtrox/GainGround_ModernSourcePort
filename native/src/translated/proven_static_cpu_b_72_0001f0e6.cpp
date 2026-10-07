#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_word_flags(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_logic_long_flags(CpuRegisters &r, std::uint32_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &r, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    const auto wide = static_cast<std::uint32_t>(left) + right;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (wide > 0xffffU) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion, r.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, r.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] FunctionResult call_child(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t return_pc)
{
    push_return(*context.host, context.registers, return_pc);
    context.registers.program_counter = target;
    return context.host->call_function(function_id, 1U, 0x72U, 2U,
        callsite, target, context);
}

[[nodiscard]] FunctionResult tail_call(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite, std::uint32_t target)
{
    context.registers.program_counter = target;
    return context.host->call_function(function_id, 1U, 0x72U, 1U,
        callsite, target, context);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001f0e6(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &r = context.registers;
    // dbf d6,$1f0e6. d6's word is the table count minus one, 0xffff for a
    // zero count: iterate here instead of re-entering this entry once per
    // iteration, which could exceed the native call depth.
    for (;;) {
        const auto masked = static_cast<std::uint16_t>(r.data[5] & 0x07ffU);
        r.data[5] = (r.data[5] & 0xffff0000U) | masked;
        set_logic_word_flags(r, masked);

        auto child = call_child(context, 382U, 0x0001f0eaU,
            0x000203c2U, 0x0001f0eeU);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;

        const auto tested = static_cast<std::uint16_t>(r.data[0]);
        set_logic_word_flags(r, tested);
        if ((tested & 0x8000U) != 0U)
            return tail_call(context, 611U, 0x0001f0f0U, 0x0001f104U);

        child = call_child(context, 369U, 0x0001f0f2U,
            0x0001f106U, 0x0001f0f6U);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;

        child = call_child(context, 370U, 0x0001f0f6U,
            0x0001f146U, 0x0001f0faU);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;

        r.data[6] = (r.data[6] << 16U) | (r.data[6] >> 16U);
        set_logic_long_flags(r, r.data[6]);

        const auto left = static_cast<std::uint16_t>(r.data[5]);
        const auto right = static_cast<std::uint16_t>(r.data[6]);
        const auto sum = static_cast<std::uint16_t>(left + right);
        r.data[5] = (r.data[5] & 0xffff0000U) | sum;
        set_add_word_flags(r, left, right, sum);

        r.data[6] = (r.data[6] << 16U) | (r.data[6] >> 16U);
        set_logic_long_flags(r, r.data[6]);

        const auto counter = static_cast<std::uint16_t>(r.data[6] - 1U);
        r.data[6] = (r.data[6] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
        r.program_counter = 0x0001f0e6U;
        if (context.host->consume_self_continuation_boundary(610U, 1U, 0x72U,
                1U, 0x0001f100U, 0x0001f0e6U, context))
            return FunctionResult::complete(4U, 0x0001f0e6U);
    }
    return tail_call(context, 611U, 0x0001f100U, 0x0001f104U);
}

} // namespace gain_ground::translated
