#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kByteMask = 0x00ffU;

void set_btst_zero(CpuRegisters &registers, bool bit_set)
{
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x0004U) | (bit_set ? 0U : 0x0004U));
}
} // namespace

FunctionResult cpu_b_dispatch_secondary_tilemap_update(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[1] = 0x000105e8U;

    const auto selector = host.read_memory_word(kPrivateRegion, 0x00000832U, kByteMask);
    const bool bit1 = (selector & 0x0002U) != 0U;
    set_btst_zero(registers, bit1);

    if (bit1) {
        constexpr std::uint32_t kCallsite = 0x0000f294U;
        constexpr std::uint32_t kTarget = 0x00015fd4U;
        registers.program_counter = kTarget;
        (void)host.call_function(290U, 1U, 0x72U, 1U,
            kCallsite, kTarget, context);
        return FunctionResult::complete(3U, kTarget);
    }

    constexpr std::uint32_t kCallsite = 0x0000f29aU;
    constexpr std::uint32_t kTarget = 0x00015ffaU;
    registers.program_counter = kTarget;
    (void)host.call_function(292U, 1U, 0x72U, 1U,
        kCallsite, kTarget, context);
    return FunctionResult::complete(3U, kTarget);
}

} // namespace gain_ground::translated
