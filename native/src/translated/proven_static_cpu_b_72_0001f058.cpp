#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_word(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub_word(CpuRegisters &r, std::uint16_t left,
    std::uint16_t right, std::uint16_t result, bool compare)
{
    std::uint16_t flags = compare
        ? static_cast<std::uint16_t>(r.status & 0x0010U) : 0U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (left < right) flags |= compare ? 0x0001U : 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] FunctionResult dispatch(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite, std::uint32_t target)
{
    context.registers.program_counter = target;
    return context.host->call_function(function_id, 1U, 0x72U, 1U,
        callsite, target, context);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &r)
{
    const auto high = host.read_memory_word(kPrivateRegion,
        r.address[7] & 0x0003ffffU, kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion,
        (r.address[7] + 2U) & 0x0003ffffU, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001f058(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto address = (r.address[5] + 0x74U) & 0x0003ffffU;
    const auto loaded = host.read_memory_word(
        kPrivateRegion, address, kWordMask);
    r.data[0] = (r.data[0] & 0xffff0000U) | loaded;
    set_logic_word(r, loaded);

    const auto decremented = static_cast<std::uint16_t>(loaded - 1U);
    r.data[0] = (r.data[0] & 0xffff0000U) | decremented;
    set_sub_word(r, loaded, 1U, decremented, false);
    host.write_memory_word(kPrivateRegion, address, decremented, kWordMask);
    set_logic_word(r, decremented);

    const auto compared = static_cast<std::uint16_t>(decremented - 2U);
    set_sub_word(r, decremented, 2U, compared, true);
    if (decremented == 2U)
        return dispatch(context, 608U, 0x0001f066U, 0x0001f0b8U);

    set_logic_word(r, decremented);
    if (decremented == 0U)
        return dispatch(context, 605U, 0x0001f06aU, 0x0001f06eU);

    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
