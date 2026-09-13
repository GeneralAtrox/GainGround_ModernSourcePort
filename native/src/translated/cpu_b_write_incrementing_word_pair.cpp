#include "gain_ground/contract_types.h"
#include "gain_ground/cpu_b_interrupt.h"

#include <cstdint>
#include <optional>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(
    CpuRegisters &registers, std::uint16_t left, std::uint16_t right,
    std::uint16_t result)
{
    const auto wide = static_cast<std::uint32_t>(left) + right;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (wide > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack = registers.address[7];
    const auto high = host.read_memory_word(kPrivateRegion, stack, kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion, stack + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

[[nodiscard]] std::optional<FunctionResult> check_interrupt(
    FunctionContext &context, std::uint32_t completed_pc, std::uint32_t next_pc)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    if (host.resumes_interrupts_inline())
        return cpu_b_interrupt_boundary(context, completed_pc, next_pc);
    const auto interrupt = host.consume_pending_interrupt(1U, 0x72U, completed_pc);
    const auto mask = static_cast<std::uint8_t>((registers.status >> 8U) & 7U);
    if (!interrupt.asserted || interrupt.level <= mask)
        return std::nullopt;

    const auto saved_status = registers.status;
    registers.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(next_pc), kWordMask);
    registers.address[7] -= 2U;
    host.write_memory_word(
        kPrivateRegion, registers.address[7], saved_status, kWordMask);
    host.write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(next_pc >> 16U), kWordMask);
    registers.status = static_cast<std::uint16_t>(
        (saved_status & 0x38ffU) | 0x2000U
        | (static_cast<std::uint16_t>(interrupt.level) << 8U));
    const auto vector = static_cast<std::uint32_t>(24U + interrupt.level) * 4U;
    const auto target = (static_cast<std::uint32_t>(
        host.read_memory_word(kPrivateRegion, vector, kWordMask)) << 16U)
        | host.read_memory_word(kPrivateRegion, vector + 2U, kWordMask);
    const auto function_id = static_cast<std::uint32_t>(96U + interrupt.level);
    registers.program_counter = target;
    context.state = 0x04U;
    const auto result = host.call_function(function_id, 1U, 0x04U, 6U,
        completed_pc, target, context);
    if (host.resumes_interrupts_inline()) {
        if (result.status != TranslationStatus::complete) return result;
        if (result.control != 2U || registers.program_counter != next_pc ||
            result.exit_program_counter != next_pc || context.state != 0x72U)
            return FunctionResult{TranslationStatus::contract_violation, result.control, registers.program_counter};
        return std::nullopt;
    }
    return FunctionResult::complete(5U, target);
}
} // namespace

FunctionResult cpu_b_write_incrementing_word_pair(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto entry_pc = registers.program_counter;

    auto value = static_cast<std::uint16_t>(registers.data[0]);
    if (entry_pc != 0x00015feaU && entry_pc != 0x00015fecU) {
        value = host.read_memory_word(kPrivateRegion, registers.address[1], kWordMask);
        registers.address[1] += 2U;
        value = static_cast<std::uint16_t>(
            value | static_cast<std::uint16_t>(registers.data[1]));
        registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
        set_logic_word_flags(registers, value);
        if (auto result = check_interrupt(context, 0x00015fe8U, 0x00015feaU))
            return *result;
    }

    if (entry_pc != 0x00015fecU) {
        host.write_memory_word(
            kTileRegion, registers.address[0] - 0x00200000U, value, kWordMask);
        registers.address[0] += 2U;
        set_logic_word_flags(registers, value);
        if (auto result = check_interrupt(context, 0x00015feaU, 0x00015fecU))
            return *result;
    }

    const auto incremented = static_cast<std::uint16_t>(value + 1U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | incremented;
    set_add_word_flags(registers, value, 1U, incremented);
    host.write_memory_word(
        kTileRegion, registers.address[0] - 0x00200000U, incremented, kWordMask);
    set_logic_word_flags(registers, incremented);
    registers.address[0] += 0x7eU;

    const auto counter = static_cast<std::uint16_t>(registers.data[2] - 1U);
    registers.data[2] = (registers.data[2] & 0xffff0000U) | counter;
    if (counter != 0xffffU) {
        constexpr std::uint32_t kCallsite = 0x00015ff4U;
        constexpr std::uint32_t kTarget = 0x00015fe6U;
        registers.program_counter = kTarget;
        (void)host.call_function(291U, 1U, 0x72U, 1U,
            kCallsite, kTarget, context);
        return FunctionResult::complete(4U, kTarget);
    }

    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
