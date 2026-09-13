#include "gain_ground/contract_types.h"

#include <cstddef>
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kConditionCodeMask = 0x001fU;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(kPrivateRegion, address & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? word : (word >> 8U));
}

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & 0x8000U) != 0U) flags |= kNegative;
    if (value == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
}

void set_test_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & 0x80U) != 0U) flags |= kNegative;
    if (value == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
}

void decrement_low_word(CpuRegisters &registers, std::size_t index)
{
    const auto value = static_cast<std::uint16_t>(registers.data[index]);
    registers.data[index] = (registers.data[index] & 0xffff0000U)
        | static_cast<std::uint16_t>(value - 1U);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(
        kPrivateRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_load_record_table_72_alt2(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    const auto record_base = host.read_memory_word(
        kPrivateRegion, registers.address[5] + 0x72U, kWordMask);
    registers.address[6] = static_cast<std::uint32_t>(static_cast<std::int32_t>(
        static_cast<std::int16_t>(record_base)));
    registers.address[6] += 0x500U;

    const auto outer_count = host.read_memory_word(
        kPrivateRegion, registers.address[3] + 4U, kWordMask);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | outer_count;
    set_move_word_flags(registers, outer_count);

    const auto inner_count = host.read_memory_word(
        kPrivateRegion, registers.address[3] + 6U, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | inner_count;
    set_move_word_flags(registers, inner_count);

    for (;;) {
        const auto state = read_byte(host, registers.address[6]);
        set_test_byte_flags(registers, state);

        if ((state & 0x80U) == 0U) {
            const auto inner_before = static_cast<std::uint16_t>(registers.data[0]);
            decrement_low_word(registers, 0U);
            if (inner_before == 0U) {
                const auto target = pop_return(host, registers);
                registers.program_counter = target;
                return FunctionResult::complete(1U, target);
            }
        }

        registers.address[6] -= 0x80U;
        const auto outer_before = static_cast<std::uint16_t>(registers.data[1]);
        decrement_low_word(registers, 1U);
        if (outer_before == 0U) {
            const auto target = pop_return(host, registers);
            registers.program_counter = target;
            return FunctionResult::complete(1U, target);
        }
    }
}

} // namespace gain_ground::translated
