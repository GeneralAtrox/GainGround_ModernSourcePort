#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kHighByteMask = 0xff00U;
constexpr std::uint16_t kConditionCodeMask = 0x001fU;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;

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

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}
} // namespace

FunctionResult cpu_b_set_constant_zero(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.data[0] &= 0xffff0000U;
    set_move_word_flags(registers, 0U);

    push_return(host, registers, 0x00011326U);
    registers.program_counter = 0x00011332U;
    const auto initialize = host.call_function(245U, 1U, 0x72U, 2U,
        0x00011322U, 0x00011332U, context);
    if (initialize.status != TranslationStatus::complete
        || initialize.control != 1U)
        return initialize;

    for (;;) {
        registers.address[6] += 0x80U;
        const auto word = host.read_memory_word(
            kRegion, registers.address[6], kHighByteMask);
        const auto value = static_cast<std::uint8_t>(word >> 8U);
        set_test_byte_flags(registers, value);
        if ((value & 0x80U) == 0U)
            break;
    }

    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x0010U;
    set_move_word_flags(registers, 0x0010U);
    registers.program_counter = 0x00011332U;
    (void)host.call_function(245U, 1U, 0x72U, 0U,
        0x0001132eU, 0x00011332U, context);
    return FunctionResult::complete(3U, 0x00011332U);
}

} // namespace gain_ground::translated
