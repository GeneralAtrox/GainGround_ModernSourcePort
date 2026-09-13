#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult proven_static_cpu_b_72_00015fde(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto source_word = host.read_memory_word(
        kPrivateRegion, registers.address[5] + 0x62U, kWordMask);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | source_word;
    set_move_word_flags(registers, source_word);

    const auto displacement = static_cast<std::int16_t>(host.read_memory_word(
        kPrivateRegion, registers.address[5] + 0x7aU, kWordMask));
    registers.address[0] = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(registers.address[0]) + displacement);

    constexpr std::uint32_t kCallsite = 0x00015fe2U;
    constexpr std::uint32_t kTarget = 0x00015fe6U;
    registers.program_counter = kTarget;
    (void)host.call_function(291U, 1U, 0x72U, 0U,
        kCallsite, kTarget, context);
    return FunctionResult::complete(3U, kTarget);
}

} // namespace gain_ground::translated
