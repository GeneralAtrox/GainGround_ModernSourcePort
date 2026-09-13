#include "gain_ground/contract_types.h"
#include "gain_ground/cpu_b_interrupt.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::int32_t signed_word(std::uint16_t value)
{
    return static_cast<std::int16_t>(value);
}

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_b_copy_words_to_200000(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &registers = context.registers;
    auto &host = *context.host;
    const auto first_offset = host.read_memory_word(kRegion, registers.address[1], kWordMask);
    registers.address[1] += 2U;
    const auto count = host.read_memory_word(kRegion, registers.address[1], kWordMask);
    registers.address[1] += 2U;
    registers.data[2] = (registers.data[2] & 0xffff0000U) | count;
    set_move_word_flags(registers, count);
    const auto source_word = host.read_memory_word(
        kRegion, registers.address[5] + 0x62U, kWordMask);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | source_word;
    set_move_word_flags(registers, source_word);
    const auto second_offset = host.read_memory_word(
        kRegion, registers.address[5] + 0x7aU, kWordMask);
    registers.address[0] = static_cast<std::uint32_t>(
        0x00200000LL + signed_word(first_offset) + signed_word(second_offset));

    constexpr std::uint32_t kCompletedPc = 0x00015fe2U;
    constexpr std::uint32_t kNextPc = 0x00015fe6U;
    if (host.resumes_interrupts_inline()) {
        if (const auto result = cpu_b_interrupt_boundary(context, kCompletedPc, kNextPc))
            return *result;
    }
    const auto interrupt = host.resumes_interrupts_inline() ? PendingInterrupt{}
        : host.consume_pending_interrupt(1U, 0x72U, kCompletedPc);
    const auto current_mask = static_cast<std::uint8_t>((registers.status >> 8U) & 7U);
    if (interrupt.asserted && interrupt.level > current_mask) {
        const auto saved_status = registers.status;
        registers.address[7] -= 4U;
        host.write_memory_word(kRegion, registers.address[7] + 2U,
            static_cast<std::uint16_t>(kNextPc), kWordMask);
        registers.address[7] -= 2U;
        host.write_memory_word(kRegion, registers.address[7], saved_status, kWordMask);
        host.write_memory_word(kRegion, registers.address[7] + 2U,
            static_cast<std::uint16_t>(kNextPc >> 16U), kWordMask);
        registers.status = static_cast<std::uint16_t>(
            (saved_status & 0x38ffU) | 0x2000U
            | (static_cast<std::uint16_t>(interrupt.level) << 8U));
        const auto vector_address = static_cast<std::uint32_t>(24U + interrupt.level) * 4U;
        const auto target = (static_cast<std::uint32_t>(
            host.read_memory_word(kRegion, vector_address, kWordMask)) << 16U)
            | host.read_memory_word(kRegion, vector_address + 2U, kWordMask);
        registers.program_counter = target;
        context.state = 0x04U;
        const auto function_id = host.resumes_interrupts_inline() ? 96U + interrupt.level : 101U;
        const auto result = host.call_function(function_id, 1U, 0x04U, 6U,
            kCompletedPc, target, context);
        if (!host.resumes_interrupts_inline()) return FunctionResult::complete(5U, target);
        if (result.status != TranslationStatus::complete) return result;
        if (result.control != 2U || result.exit_program_counter != kNextPc ||
            registers.program_counter != kNextPc || context.state != 0x72U)
            return {TranslationStatus::contract_violation, result.control, registers.program_counter};
        // Setup has already completed; continue once into the word-pair loop.
    }

    registers.program_counter = kNextPc;
    (void)host.call_function(291U, 1U, 0x72U, 0U,
        kCompletedPc, kNextPc, context);
    return FunctionResult::complete(3U, kNextPc);
}

} // namespace gain_ground::translated
