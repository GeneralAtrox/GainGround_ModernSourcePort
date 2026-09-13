#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_word_flags(CpuRegisters &registers, std::uint16_t result)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_sub_word_flags(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (right > left) flags |= 0x0011U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001e25a(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    auto left = static_cast<std::uint16_t>(registers.data[7]);
    auto result = static_cast<std::uint16_t>(left + left);
    registers.data[7] = (registers.data[7] & 0xffff0000U) | result;
    set_add_word_flags(registers, left, left, result);

    result = host.read_memory_word(kRegion, registers.address[5] + 0x5cU, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
    set_logic_word_flags(registers, result);

    left = result;
    result = static_cast<std::uint16_t>(left + 0x0080U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
    set_add_word_flags(registers, left, 0x0080U, result);

    const bool shifted_out = (result & 0x0080U) != 0U;
    result = static_cast<std::uint16_t>(result >> 8U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
    std::uint16_t shift_flags{};
    if (result == 0U) shift_flags |= 0x0004U;
    if (shifted_out) shift_flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | shift_flags);

    result = static_cast<std::uint16_t>(result & 0x0007U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
    set_logic_word_flags(registers, result);

    left = result;
    result = static_cast<std::uint16_t>(left - 1U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
    set_sub_word_flags(registers, left, 1U, result);

    if ((registers.status & 0x0008U) != 0U) {
        registers.program_counter = 0x0001e274U;
        return host.call_function(602U, 1U, 0x72U, 1U,
            0x0001e26cU, 0x0001e274U, context);
    }

    registers.program_counter = 0x0001e26eU;
    return host.call_function(601U, 1U, 0x72U, 1U,
        0x0001e26cU, 0x0001e26eU, context);
}

} // namespace gain_ground::translated
