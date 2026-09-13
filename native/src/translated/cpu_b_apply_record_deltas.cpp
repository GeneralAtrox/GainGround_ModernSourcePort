#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t data)
{
    host.write_memory_word(kRegion, address, data, kWordMask);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto word = host.read_memory_word(kRegion, address & ~1U,
        odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? (word & 0x00ffU) : (word >> 8U));
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = read_word(host, address);
    const auto low = read_word(host, address + 2U);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    // 68000 read-modify-write longword operations commit the low word first.
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return target;
}

void set_zero_only(CpuRegisters &registers, bool zero)
{
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x0004U) | (zero ? 0x0004U : 0U));
}

void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_sub_byte_flags(CpuRegisters &registers,
    std::uint8_t left, std::uint8_t right, std::uint8_t result)
{
    const auto old_x = static_cast<std::uint16_t>(registers.status & 0x0010U);
    std::uint16_t flags{};
    if (left < right) flags |= 0x0001U;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags | old_x);
}

void set_add_long_flags(CpuRegisters &registers,
    std::uint32_t left, std::uint32_t right, std::uint32_t result)
{
    std::uint16_t flags{};
    if (result < left) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x80000000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

} // namespace

FunctionResult cpu_b_apply_record_deltas(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const std::uint32_t base = registers.address[5];
    // 0x1e124 btst.b #1,(0x40,a5): modifies Z only (Z=bit==0); bit set -> rts 0x1e12c.
    {
        const auto flags = read_byte(host, base + 0x40U);
        const bool bit_clear = (flags & 0x02U) == 0U;
        set_zero_only(registers, bit_clear);
        if (!bit_clear) {
            registers.program_counter = pop_return(host, registers);
            return FunctionResult::complete(1U, registers.program_counter);
        }
    }

    // 0x1e12e tst.b (0x3e,a5): N/Z, V=C=0; nonzero -> rts 0x1e134.
    {
        const auto phase = read_byte(host, base + 0x3eU);
        set_logic_flags(registers, phase, 0x80U, 0xffU);
        if (phase != 0U) {
            registers.program_counter = pop_return(host, registers);
            return FunctionResult::complete(1U, registers.program_counter);
        }
    }

    // 0x1e136 cmpi.b #0x1e,(0x59,a5): full flags, X preserved; equal -> apply.
    const auto marker = read_byte(host, base + 0x59U);
    const auto difference = static_cast<std::uint8_t>(marker - 0x1eU);
    set_sub_byte_flags(registers, marker, 0x1eU, difference);

    if (marker != 0x1eU) {
        // 0x1e13e tst.b (0x59,a5): the original performs a second bus read.
        const auto tested_marker = read_byte(host, base + 0x59U);
        set_logic_flags(registers, tested_marker, 0x80U, 0xffU);
        if (tested_marker != 0U) {
            registers.program_counter = pop_return(host, registers);
            return FunctionResult::complete(1U, registers.program_counter);
        }
    }

    // 0x1e146 move.l (0x1e,a5),d0: N/Z per long, V=C=0, X preserved.
    const std::uint32_t delta_x = read_long(host, base + 0x1eU);
    registers.data[0] = delta_x;
    set_logic_flags(registers, delta_x, 0x80000000U, 0xffffffffU);

    // 0x1e14a add.l d0,(0x12,a5): X=C=carry unconditionally, V/N/Z full; 32-bit wrap.
    {
        const auto destination = read_long(host, base + 0x12U);
        const auto result = destination + delta_x;
        write_long(host, base + 0x12U, result);
        set_add_long_flags(registers, destination, delta_x, result);
    }

    // 0x1e14e move.l (0x26,a5),d0.
    const std::uint32_t delta_y = read_long(host, base + 0x26U);
    registers.data[0] = delta_y;
    set_logic_flags(registers, delta_y, 0x80000000U, 0xffffffffU);

    // 0x1e152 add.l d0,(0x1a,a5).
    {
        const auto destination = read_long(host, base + 0x1aU);
        const auto result = destination + delta_y;
        write_long(host, base + 0x1aU, result);
        set_add_long_flags(registers, destination, delta_y, result);
    }

    // 0x1e156 rts.
    registers.program_counter = pop_return(host, registers);
    return FunctionResult::complete(1U, registers.program_counter);
}

} // namespace gain_ground::translated
