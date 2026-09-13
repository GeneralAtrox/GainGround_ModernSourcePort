#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kCpuBMainMemoryRegion = 2U;
constexpr std::uint16_t kFullWordMask = 0xffffU;
constexpr std::uint16_t kConditionCodeMask = 0x001fU;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;
constexpr std::uint16_t kOverflow = 0x0002U;
constexpr std::uint16_t kCarry = 0x0001U;

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kCpuBMainMemoryRegion, address, kFullWordMask);
    const auto low = host.read_memory_word(kCpuBMainMemoryRegion, address + 2U, kFullWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_lsr_word_flags(CpuRegisters &registers, std::uint16_t before, std::uint16_t result)
{
    std::uint16_t flags{};
    if (result == 0U)
        flags |= kZero;
    if ((before & 1U) != 0U)
        flags |= kExtend | kCarry;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
}

void set_sub_word_flags(
    CpuRegisters &registers, std::uint16_t destination, std::uint16_t source,
    std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U)
        flags |= kNegative;
    if (result == 0U)
        flags |= kZero;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= kOverflow;
    if (destination < source)
        flags |= kExtend | kCarry;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
}

[[nodiscard]] std::uint32_t prepare_dividend(std::uint16_t value)
{
    std::uint32_t dividend = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(value)));
    const auto doubled = static_cast<std::uint16_t>(value + value);
    dividend = (dividend & 0xffff0000U) | doubled;
    return (dividend << 8U) | (dividend >> 24U);
}

[[nodiscard]] std::uint32_t divide_unsigned(std::uint32_t dividend, std::uint16_t divisor)
{
    const std::uint32_t quotient = dividend / divisor;
    const std::uint32_t remainder = dividend % divisor;
    return (remainder << 16U) | static_cast<std::uint16_t>(quotient);
}

} // namespace

FunctionResult cpu_b_interpolate_wrapped_coordinate(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    const auto input_d0 = static_cast<std::uint16_t>(context.registers.data[0]);
    const auto input_d1 = static_cast<std::uint16_t>(context.registers.data[1]);

    if (input_d1 < input_d0) {
        std::uint32_t value = divide_unsigned(prepare_dividend(input_d1), input_d0);
        value += 1U;
        const auto before_shift = static_cast<std::uint16_t>(value);
        const auto result = static_cast<std::uint16_t>(before_shift >> 1U);
        context.registers.data[1] = (value & 0xffff0000U) | result;
        set_lsr_word_flags(context.registers, before_shift, result);
    } else {
        std::uint32_t value = divide_unsigned(prepare_dividend(input_d0), input_d1);
        value += 1U;
        const auto before_shift = static_cast<std::uint16_t>(value);
        const auto quotient = static_cast<std::uint16_t>(before_shift >> 1U);
        context.registers.data[0] = (value & 0xffff0000U) | quotient;
        set_lsr_word_flags(context.registers, before_shift, quotient);

        const auto result = static_cast<std::uint16_t>(0x0200U - quotient);
        context.registers.data[1] =
            (context.registers.data[1] & 0xffff0000U) | result;
        set_sub_word_flags(context.registers, 0x0200U, quotient, result);
    }

    const std::uint32_t return_address = read_long(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
