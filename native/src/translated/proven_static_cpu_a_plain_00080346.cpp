#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;
constexpr std::uint16_t kHighByteMask = 0xff00U;
} // namespace

FunctionResult proven_static_cpu_a_plain_00080346(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    constexpr std::uint32_t address = 0xffff8400U;
    (void)host.read_memory_word(kSharedRegion, 0x0000034aU, 0xffffU);
    (void)host.read_memory_word(kSharedRegion, address & kSharedMask, kHighByteMask);
    (void)host.read_memory_word(kSharedRegion, 0x0000034cU, 0xffffU);
    host.write_memory_word(kSharedRegion, address & kSharedMask, 0U, kHighByteMask);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x000fU) | 0x0004U);

    registers.program_counter = 0x0008034aU;
    return host.call_function(61U, 0U, 0xffU, 0U,
        0x00080346U, 0x0008034aU, context);
}

} // namespace gain_ground::translated
