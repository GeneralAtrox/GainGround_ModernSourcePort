#include "gain_ground/contract_types.h"

#include <cstddef>
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

void set_add_word_flags(
    CpuRegisters &r, std::uint16_t left, std::uint16_t right,
    std::uint16_t result)
{
    const auto wide = static_cast<std::uint32_t>(left) + right;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (wide > 0xffffU) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub_word_flags(
    CpuRegisters &r, std::uint16_t left, std::uint16_t right,
    std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (left < right) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void negate_word(CpuRegisters &r, std::size_t index)
{
    const auto value = static_cast<std::uint16_t>(r.data[index]);
    const auto result = static_cast<std::uint16_t>(0U - value);
    r.data[index] = (r.data[index] & 0xffff0000U) | result;
    set_sub_word_flags(r, 0U, value, result);
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion, r.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, r.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto high = host.read_memory_word(kPrivateRegion, r.address[7], kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion, r.address[7] + 2U, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

[[nodiscard]] FunctionResult call_interpolator(
    FunctionContext &context, std::uint32_t callsite, std::uint32_t continuation)
{
    auto &host = *context.host;
    auto &r = context.registers;
    push_return(host, r, continuation);
    r.program_counter = 0x000162ccU;
    return host.call_function(
        301U, 1U, 0x72U, 2U, callsite, 0x000162ccU, context);
}

[[nodiscard]] FunctionResult finish_return(FunctionContext &context)
{
    const auto target = pop_return(*context.host, context.registers);
    context.registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_apply_signed_lookup_result(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    auto d0 = static_cast<std::uint16_t>(r.data[0]);
    set_logic_word_flags(r, d0);

    if (d0 == 0U) {
        const auto d1 = static_cast<std::uint16_t>(r.data[1]);
        set_logic_word_flags(r, d1);
        const auto result = static_cast<std::uint16_t>(
            (d1 & 0x8000U) != 0U ? 0x0600U : 0x0200U);
        r.data[1] = (r.data[1] & 0xffff0000U) | result;
        set_logic_word_flags(r, result);
        return finish_return(context);
    }

    if ((d0 & 0x8000U) == 0U) {
        const auto d1 = static_cast<std::uint16_t>(r.data[1]);
        set_logic_word_flags(r, d1);
        if ((d1 & 0x8000U) == 0U) {
            r.program_counter = 0x000162ccU;
            (void)host.call_function(301U, 1U, 0x72U, 1U,
                0x00016358U, 0x000162ccU, context);
            return FunctionResult::complete(3U, 0x000162ccU);
        }

        negate_word(r, 1U);
        const auto child = call_interpolator(context, 0x00016348U, 0x0001634cU);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
        const auto before = static_cast<std::uint16_t>(r.data[1]);
        const auto subtracted = static_cast<std::uint16_t>(before - 0x0800U);
        r.data[1] = (r.data[1] & 0xffff0000U) | subtracted;
        set_sub_word_flags(r, before, 0x0800U, subtracted);
        negate_word(r, 1U);
        const auto masked = static_cast<std::uint16_t>(r.data[1] & 0x07ffU);
        r.data[1] = (r.data[1] & 0xffff0000U) | masked;
        set_logic_word_flags(r, masked);
        return finish_return(context);
    }

    negate_word(r, 0U);
    const auto d1 = static_cast<std::uint16_t>(r.data[1]);
    set_logic_word_flags(r, d1);
    if ((d1 & 0x8000U) != 0U) {
        negate_word(r, 1U);
        const auto child = call_interpolator(context, 0x00016328U, 0x0001632cU);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
        const auto before = static_cast<std::uint16_t>(r.data[1]);
        const auto added = static_cast<std::uint16_t>(before + 0x0400U);
        r.data[1] = (r.data[1] & 0xffff0000U) | added;
        set_add_word_flags(r, before, 0x0400U, added);
        return finish_return(context);
    }

    const auto child = call_interpolator(context, 0x00016332U, 0x00016336U);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;
    const auto before = static_cast<std::uint16_t>(r.data[1]);
    const auto subtracted = static_cast<std::uint16_t>(before - 0x0400U);
    r.data[1] = (r.data[1] & 0xffff0000U) | subtracted;
    set_sub_word_flags(r, before, 0x0400U, subtracted);
    negate_word(r, 1U);
    const auto masked = static_cast<std::uint16_t>(r.data[1] & 0x07ffU);
    r.data[1] = (r.data[1] & 0xffff0000U) | masked;
    set_logic_word_flags(r, masked);
    return finish_return(context);
}

} // namespace gain_ground::translated
