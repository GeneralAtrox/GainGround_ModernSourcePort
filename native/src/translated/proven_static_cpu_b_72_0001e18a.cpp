#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto shift = odd ? 0U : 8U;
    return static_cast<std::uint8_t>(
        host.read_memory_word(kRegion, address & ~1U, mask) >> shift);
}

void set_logic(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word(CpuRegisters &registers, std::uint16_t left,
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

void set_compare_word(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left - right);
    std::uint16_t flags = registers.status & 0x0010U;
    if (right > left) flags |= 0x0001U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

std::uint16_t arithmetic_shift_left(CpuRegisters &registers,
    std::uint16_t value, unsigned count)
{
    bool carry{};
    bool overflow{};
    for (unsigned index = 0; index != count; ++index) {
        const bool old_sign = (value & 0x8000U) != 0U;
        carry = old_sign;
        value = static_cast<std::uint16_t>(value << 1U);
        overflow = overflow || (old_sign != ((value & 0x8000U) != 0U));
    }
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if (overflow) flags |= 0x0002U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    return value;
}

void set_bit_zero(CpuRegisters &registers, bool bit_was_set)
{
    if (bit_was_set)
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);
    else
        registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001e18a(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];

    registers.data[0] = 0U;
    set_logic(registers, 0U, 0x80000000U, 0xffffffffU);
    const auto phase = read_byte(host, base + 0x36U);
    registers.data[0] = phase;
    set_logic(registers, phase, 0x80U, 0xffU);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | phase;
    set_logic(registers, phase, 0x8000U, 0xffffU);

    auto d0 = arithmetic_shift_left(registers, phase, 2U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    auto d1 = arithmetic_shift_left(registers, phase, 1U);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | d1;
    auto sum = static_cast<std::uint16_t>(d0 + d1);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | sum;
    set_add_word(registers, d0, d1, sum);

    d1 = host.read_memory_word(kRegion, base + 0x56U, kWordMask);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | d1;
    set_logic(registers, d1, 0x8000U, 0xffffU);
    auto d2 = d1;
    registers.data[2] = (registers.data[2] & 0xffff0000U) | d2;
    set_logic(registers, d2, 0x8000U, 0xffffU);
    d1 = static_cast<std::uint16_t>(d1 & 0x0600U);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | d1;
    set_logic(registers, d1, 0x8000U, 0xffffU);
    const auto advance = host.enemy_walking_advance(base, sum);
    d2 = static_cast<std::uint16_t>(d2 + advance);
    registers.data[2] = (registers.data[2] & 0xffff0000U) | d2;
    set_add_word(registers, static_cast<std::uint16_t>(d2 - advance), advance, d2);
    d2 = static_cast<std::uint16_t>(d2 & 0x07ffU);
    registers.data[2] = (registers.data[2] & 0xffff0000U) | d2;
    set_logic(registers, d2, 0x8000U, 0xffffU);
    host.write_memory_word(kRegion, base + 0x56U, d2, kWordMask);
    set_logic(registers, d2, 0x8000U, 0xffffU);
    d2 = static_cast<std::uint16_t>(d2 & 0x0600U);
    registers.data[2] = (registers.data[2] & 0xffff0000U) | d2;
    set_logic(registers, d2, 0x8000U, 0xffffU);

    const auto timer = read_byte(host, base + 0x59U);
    set_logic(registers, timer, 0x80U, 0xffU);
    if (timer != 0U) {
        registers.program_counter = 0x0001e238U;
        return host.call_function(598U, 1U, 0x72U, 1U,
            0x0001e1b4U, 0x0001e238U, context);
    }

    set_compare_word(registers, d1, d2);
    if ((registers.status & 0x0004U) == 0U) {
        registers.program_counter = 0x0001e1c6U;
        return host.call_function(594U, 1U, 0x72U, 1U,
            0x0001e1baU, 0x0001e1c6U, context);
    }

    const auto flags = read_byte(host, base + 0x40U);
    set_bit_zero(registers, (flags & 0x08U) != 0U);
    if ((registers.status & 0x0004U) != 0U) {
        registers.program_counter = 0x0001e22aU;
        return host.call_function(555U, 1U, 0x72U, 1U,
            0x0001e1c2U, 0x0001e22aU, context);
    }

    registers.program_counter = 0x0001e1c6U;
    return host.call_function(594U, 1U, 0x72U, 1U,
        0x0001e1c2U, 0x0001e1c6U, context);
}

} // namespace gain_ground::translated
