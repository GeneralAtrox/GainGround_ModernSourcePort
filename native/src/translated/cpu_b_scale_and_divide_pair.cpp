#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void compare_long(CpuRegisters &registers, std::uint32_t lhs, std::uint32_t rhs)
{
    const auto result = lhs - rhs;
    const bool overflow = (((lhs ^ rhs) & (lhs ^ result)) & 0x80000000U) != 0U;
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    if (lhs < rhs) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void arithmetic_shift_right_two(CpuRegisters &registers, std::uint32_t &value)
{
    const bool carry = (value & 0x00000002U) != 0U;
    value = static_cast<std::uint32_t>(static_cast<std::int32_t>(value) >> 2U);
    std::uint16_t flags{};
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    if (carry) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(kPrivateRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_scale_and_divide_pair(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &registers = context.registers;
    compare_long(registers, registers.data[1], 0x0000ffffU);
    const bool less_or_equal = (registers.status & 0x0004U) != 0U
        || (((registers.status & 0x0008U) != 0U)
            != ((registers.status & 0x0002U) != 0U));
    if (!less_or_equal) {
        arithmetic_shift_right_two(registers, registers.data[0]);
        arithmetic_shift_right_two(registers, registers.data[1]);
        registers.program_counter = 0x0001fd24U;
        const auto loop = context.host->call_function(
            379U, 1U, 0x72U, 1U, 0x0001fd30U, 0x0001fd24U, context);
        if (loop.status == TranslationStatus::complete && loop.control == 3U)
            return FunctionResult::complete(4U, 0x0001fd24U);
        return loop;
    }

    const auto divisor = static_cast<std::uint16_t>(registers.data[1]);
    if (divisor == 0U)
        return {TranslationStatus::contract_violation, 0U, 0x0001fd32U};
    const auto dividend = registers.data[0];
    const auto quotient = dividend / divisor;
    if (quotient > 0xffffU)
        return {TranslationStatus::contract_violation, 0U, 0x0001fd32U};
    const auto remainder = dividend % divisor;
    registers.data[0] = (remainder << 16U) | quotient;
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);

    const auto return_address = pop_return(*context.host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
