#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kConditionCodeMask = 0x001fU;
constexpr std::uint16_t kExtendFlag = 0x0010U;
constexpr std::uint16_t kZeroFlag = 0x0004U;
} // namespace

FunctionResult proven_static_cpu_b_72_0001d9da(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];

    host.write_memory_word(kPrivateRegion, base + 2U, 0x0001U, kWordMask);
    host.write_memory_word(kPrivateRegion, base + 4U, 0xed98U, kWordMask);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) |
        (registers.status & kExtendFlag));

    host.write_memory_word(kPrivateRegion, base + 0x54U, 0U, kWordMask);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) |
        (registers.status & kExtendFlag) | kZeroFlag);

    const auto high = host.read_memory_word(
        kPrivateRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
