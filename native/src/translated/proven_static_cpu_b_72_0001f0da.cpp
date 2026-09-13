#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
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

void set_sub_word_flags(CpuRegisters &r, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (left < right) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] FunctionResult dispatch(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t kind,
    std::uint32_t callsite, std::uint32_t target)
{
    context.registers.program_counter = target;
    return context.host->call_function(function_id, 1U, 0x72U, kind,
        callsite, target, context);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001f0da(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &r = context.registers;
    const auto left = static_cast<std::uint16_t>(r.data[5]);
    const auto right = static_cast<std::uint16_t>(r.data[7]);
    const auto difference = static_cast<std::uint16_t>(left - right);
    r.data[5] = (r.data[5] & 0xffff0000U) | difference;
    set_sub_word_flags(r, left, right, difference);

    const auto counter = static_cast<std::uint16_t>(r.data[4] - 1U);
    r.data[4] = (r.data[4] & 0xffff0000U) | counter;
    if (counter != 0xffffU) {
        const auto continuation = dispatch(
            context, 609U, 1U, 0x0001f0dcU, 0x0001f0daU);
        if (continuation.status == TranslationStatus::complete
            && continuation.control == 3U)
            return FunctionResult::complete(4U, 0x0001f0daU);
        return continuation;
    }

    r.data[6] = (r.data[6] << 16U) | (r.data[6] >> 16U);
    set_logic_long_flags(r, r.data[6]);
    const auto moved = static_cast<std::uint16_t>(r.data[7]);
    r.data[6] = (r.data[6] & 0xffff0000U) | moved;
    set_logic_word_flags(r, moved);
    r.data[6] = (r.data[6] << 16U) | (r.data[6] >> 16U);
    set_logic_long_flags(r, r.data[6]);

    return dispatch(context, 610U, 0U, 0x0001f0e4U, 0x0001f0e6U);
}

} // namespace gain_ground::translated
