#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    return {address & ~1U,
        static_cast<std::uint16_t>((address & 1U) != 0U ? 0x00ffU : 0xff00U),
        (address & 1U) != 0U ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kPrivateRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kPrivateRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = static_cast<std::uint16_t>(registers.status & 0x0010U);
    value &= mask;
    if ((value & sign) != 0U)
        flags |= 0x0008U;
    if (value == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t add_flags(CpuRegisters &registers,
    std::uint32_t destination, std::uint32_t source,
    std::uint32_t sign, std::uint32_t mask)
{
    destination &= mask;
    source &= mask;
    const std::uint32_t wide = destination + source;
    const std::uint32_t result = wide & mask;
    std::uint16_t flags = 0U;
    if ((result & sign) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(destination ^ source) & (destination ^ result)) & sign) != 0U)
        flags |= 0x0002U;
    if (wide > mask) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    return result;
}

[[nodiscard]] std::uint32_t subtract_flags(CpuRegisters &registers,
    std::uint32_t destination, std::uint32_t source,
    std::uint32_t sign, std::uint32_t mask, bool update_extend)
{
    destination &= mask;
    source &= mask;
    const std::uint32_t result = (destination - source) & mask;
    std::uint16_t flags = static_cast<std::uint16_t>(
        update_extend ? 0U : (registers.status & 0x0010U));
    if ((result & sign) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if ((((destination ^ source) & (destination ^ result)) & sign) != 0U)
        flags |= 0x0002U;
    if (source > destination) flags |= static_cast<std::uint16_t>(
        update_extend ? 0x0011U : 0x0001U);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    return result;
}

[[nodiscard]] std::uint16_t arithmetic_shift_left_three(
    CpuRegisters &registers, std::uint16_t value)
{
    bool carry = false;
    bool overflow = false;
    for (unsigned index = 0; index < 3U; ++index) {
        const bool old_sign = (value & 0x8000U) != 0U;
        carry = old_sign;
        value = static_cast<std::uint16_t>(value << 1U);
        overflow = overflow || (old_sign != ((value & 0x8000U) != 0U));
    }
    std::uint16_t flags = 0U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    if (carry) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    return value;
}

void push_long(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] FunctionResult finish(FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto high = host.read_memory_word(
        kPrivateRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_step_descriptor_callback_phase_a(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;
    FunctionResult character_result;
    if (host.run_character_attack_phase(context, character_result)) return character_result;
    if (registers.program_counter != 0x00010a2eU)
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};

    const auto source = host.read_memory_word(
        kPrivateRegion, registers.address[5] + 0x38U, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | source;
    set_logic_flags(registers, source, 0x8000U, 0xffffU);
    registers.data[7] = (registers.data[7] & 0xffff0000U) | source;
    set_logic_flags(registers, source, 0x8000U, 0xffffU);

    auto d0 = static_cast<std::uint16_t>(add_flags(
        registers, source, source, 0x8000U, 0xffffU));
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    registers.data[1] = (registers.data[1] & 0xffff0000U) | d0;
    set_logic_flags(registers, d0, 0x8000U, 0xffffU);
    d0 = static_cast<std::uint16_t>(add_flags(
        registers, d0, d0, 0x8000U, 0xffffU));
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    auto d1 = static_cast<std::uint16_t>(registers.data[1]);
    d1 = static_cast<std::uint16_t>(add_flags(
        registers, d1, d0, 0x8000U, 0xffffU));
    registers.data[1] = (registers.data[1] & 0xffff0000U) | d1;
    d0 = arithmetic_shift_left_three(registers, d0);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    d0 = static_cast<std::uint16_t>(add_flags(
        registers, d0, d1, 0x8000U, 0xffffU));
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;

    registers.address[3] = 0x000115a2U;
    registers.address[3] = static_cast<std::uint32_t>(
        registers.address[3] + static_cast<std::int16_t>(d0));
    push_long(host, registers, 0x00010a4aU);
    registers.program_counter = 0x00010a92U;
    const auto child = host.call_function(210U, 1U, 0x72U, 2U,
        0x00010a46U, 0x00010a92U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    registers.program_counter = 0x00010a4aU;
    const auto timer_address = registers.address[5] + 0x40U;
    auto timer = read_byte(host, timer_address);
    set_logic_flags(registers, timer, 0x80U, 0xffU);
    if (timer != 0U) {
        timer = read_byte(host, timer_address);
        timer = static_cast<std::uint8_t>(subtract_flags(
            registers, timer, 1U, 0x80U, 0xffU, true));
        write_byte(host, timer_address, timer);
    }

    const auto mode_address = registers.address[5] + 0x3eU;
    const auto mode = read_byte(host, mode_address);
    (void)subtract_flags(registers, mode, 1U, 0x80U, 0xffU, false);
    if (mode != 1U)
        return finish(context);

    const auto phase_address = registers.address[5] + 0x3cU;
    auto phase = read_byte(host, phase_address);
    phase = static_cast<std::uint8_t>(add_flags(
        registers, phase, 1U, 0x80U, 0xffU));
    write_byte(host, phase_address, phase);

    registers.address[0] = 0x00011542U;
    const auto table_index = static_cast<std::int16_t>(
        static_cast<std::uint16_t>(registers.data[7]));
    const auto phase_limit = read_byte(host, static_cast<std::uint32_t>(
        registers.address[0] + table_index));
    registers.data[0] = (registers.data[0] & 0xffffff00U) | phase_limit;
    set_logic_flags(registers, phase_limit, 0x80U, 0xffU);
    const auto compared_phase = read_byte(host, phase_address);
    (void)subtract_flags(registers, phase_limit, compared_phase,
        0x80U, 0xffU, false);
    const bool greater = (registers.status & 0x0004U) == 0U
        && (((registers.status & 0x0008U) != 0U)
            == ((registers.status & 0x0002U) != 0U));
    if (greater)
        return finish(context);

    (void)read_byte(host, phase_address);
    write_byte(host, phase_address, 0U);
    set_logic_flags(registers, 0U, 0x80U, 0xffU);

    const auto subphase_address = registers.address[5] + 0x3dU;
    auto subphase = read_byte(host, subphase_address);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | subphase;
    set_logic_flags(registers, subphase, 0x80U, 0xffU);
    subphase = static_cast<std::uint8_t>(add_flags(
        registers, subphase, 1U, 0x80U, 0xffU));
    registers.data[0] = (registers.data[0] & 0xffffff00U) | subphase;
    write_byte(host, subphase_address, subphase);
    set_logic_flags(registers, subphase, 0x80U, 0xffU);

    registers.address[0] = 0x00011562U;
    const auto subphase_limit = read_byte(host, static_cast<std::uint32_t>(
        registers.address[0] + table_index));
    (void)subtract_flags(registers, subphase, subphase_limit,
        0x80U, 0xffU, false);
    if ((registers.status & 0x0001U) != 0U)
        return finish(context);

    (void)read_byte(host, mode_address);
    write_byte(host, mode_address, 0U);
    set_logic_flags(registers, 0U, 0x80U, 0xffU);
    write_byte(host, subphase_address, 1U);
    set_logic_flags(registers, 1U, 0x80U, 0xffU);
    return finish(context);
}

} // namespace gain_ground::translated
