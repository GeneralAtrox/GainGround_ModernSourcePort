#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
} // namespace

FunctionResult proven_static_cpu_b_72_00015fb8(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.address[0] = 0x00200000U;
    const auto displacement = host.read_memory_word(
        kPrivateRegion, registers.address[1] & 0x0003ffffU, kWordMask);
    registers.address[1] += 2U;
    registers.address[0] = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(registers.address[0])
        + static_cast<std::int16_t>(displacement));

    constexpr std::uint32_t kNextPc = 0x00015fc0U;
    registers.program_counter = kNextPc;
    (void)host.call_function(
        473U, 1U, 0x72U, 0U, 0x00015fbeU, kNextPc, context);
    return FunctionResult::complete(3U, kNextPc);
}

} // namespace gain_ground::translated
