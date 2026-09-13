#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kByteMask = 0x00ffU;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_btst_zero(CpuRegisters &r, bool bit_set)
{
    r.status = static_cast<std::uint16_t>(
        (r.status & ~0x0004U) | (bit_set ? 0U : 0x0004U));
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto high = host.read_memory_word(kPrivateRegion, r.address[7], kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion, r.address[7] + 2U, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_dispatch_flagged_tilemap_update(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    r.address[1] = 0x000105e8U;

    auto selector = host.read_memory_word(kPrivateRegion, 0x00000832U, kByteMask);
    const bool bit0 = (selector & 0x0001U) != 0U;
    set_btst_zero(r, bit0);
    if (bit0) {
        constexpr std::uint32_t kCallsite = 0x0000f272U;
        constexpr std::uint32_t kTarget = 0x00015fd4U;
        r.program_counter = kTarget;
        (void)host.call_function(290U, 1U, 0x72U, 1U,
            kCallsite, kTarget, context);
        return FunctionResult::complete(3U, kTarget);
    }

    selector = host.read_memory_word(kPrivateRegion, 0x00000832U, kByteMask);
    const bool bit2 = (selector & 0x0004U) != 0U;
    set_btst_zero(r, bit2);
    if (bit2) {
        constexpr std::uint32_t kCallsite = 0x0000f280U;
        constexpr std::uint32_t kTarget = 0x00015ffaU;
        r.program_counter = kTarget;
        (void)host.call_function(292U, 1U, 0x72U, 1U,
            kCallsite, kTarget, context);
        return FunctionResult::complete(3U, kTarget);
    }

    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
