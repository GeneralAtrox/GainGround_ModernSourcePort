#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
FunctionResult cpu_b_emit_tilemap_a_incrementing_pairs_with_bias_resume_1611e(
    FunctionContext &context) noexcept;

namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_flags(CpuRegisters &registers,
    std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_b_copy_words_to_202000(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.address[0] = 0x00202000U;
    const auto offset = host.read_memory_word(
        kPrivateRegion, registers.address[1], kWordMask);
    registers.address[1] += 2U;
    registers.address[0] = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(registers.address[0])
        + static_cast<std::int16_t>(offset));

    registers.data[3] = 0U;
    set_logic_flags(registers, 0U, 0x80000000U);
    const auto count = host.read_memory_word(
        kPrivateRegion, registers.address[1], kWordMask);
    registers.data[3] = count;
    set_logic_flags(registers, count, 0x8000U);

    const auto bias = host.read_memory_word(
        kPrivateRegion, registers.address[5] + 0x64U, kWordMask);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | bias;
    set_logic_flags(registers, bias, 0x8000U);

    registers.program_counter = 0x0001611eU;
    return cpu_b_emit_tilemap_a_incrementing_pairs_with_bias_resume_1611e(
        context);
}

} // namespace gain_ground::translated
