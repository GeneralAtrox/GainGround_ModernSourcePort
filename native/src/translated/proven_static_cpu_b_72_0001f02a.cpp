#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint8_t read_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto value = host.read_memory_word(
        kPrivateRegion, address & 0x0003fffeU, mask);
    return static_cast<std::uint8_t>(value >> (odd ? 0U : 8U));
}

void set_logic_word(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub_word(CpuRegisters &r, std::uint16_t left,
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

FunctionResult proven_static_cpu_b_72_0001f02a(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    const bool bit2 = (read_byte(host, base + 0x41U) & 0x04U) != 0U;
    r.status = static_cast<std::uint16_t>(
        bit2 ? (r.status & ~0x0004U) : (r.status | 0x0004U));
    if (bit2)
        return dispatch(context, 604U, 0x0001f030U, 0x0001f058U);

    const auto address = (base + 0x74U) & 0x0003ffffU;
    const auto tested = host.read_memory_word(
        kPrivateRegion, address, kWordMask);
    set_logic_word(r, tested);
    if (tested == 0U)
        return dispatch(context, 603U, 0x0001f036U, 0x0001f03eU);

    const auto before = host.read_memory_word(
        kPrivateRegion, address, kWordMask);
    const auto after = static_cast<std::uint16_t>(before - 1U);
    host.write_memory_word(kPrivateRegion, address, after, kWordMask);
    set_sub_word(r, before, 1U, after);

    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
