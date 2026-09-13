#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] constexpr ByteLocation locate(std::uint32_t address) noexcept
{
    return {address & ~1U,
        static_cast<std::uint16_t>((address & 1U) != 0U ? 0x00ffU : 0xff00U),
        (address & 1U) != 0U ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

void clear_long(ExecutionHost &host, CpuRegisters &registers, std::uint32_t address)
{
    (void)host.read_memory_word(kRegion, address, kWordMask);
    (void)host.read_memory_word(kRegion, address + 2U, kWordMask);
    host.write_memory_word(kRegion, address + 2U, 0U, kWordMask);
    host.write_memory_word(kRegion, address, 0U, kWordMask);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x000fU) | 0x0004U);
}

void set_logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_logic_long(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint16_t asl_word(
    CpuRegisters &registers, std::uint16_t value, unsigned count)
{
    bool carry = false;
    bool overflow = false;
    for (unsigned i = 0; i < count; ++i) {
        const bool old_sign = (value & 0x8000U) != 0U;
        carry = old_sign;
        value = static_cast<std::uint16_t>(value << 1U);
        overflow = overflow || (old_sign != ((value & 0x8000U) != 0U));
    }
    std::uint16_t flags = 0U;
    if (carry) flags |= 0x0011U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
    return value;
}

void set_sub_byte(CpuRegisters &registers, std::uint8_t destination,
    std::uint8_t source, std::uint8_t result, bool update_extend)
{
    const bool carry = source > destination;
    const bool overflow = ((destination ^ source) & (destination ^ result) & 0x80U) != 0U;
    std::uint16_t flags = update_extend ? 0U : (registers.status & 0x0010U);
    if (update_extend && carry) flags |= 0x0010U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    if (carry) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_add_byte(CpuRegisters &registers, std::uint8_t destination,
    std::uint8_t source, std::uint8_t result)
{
    const bool carry = static_cast<unsigned>(destination) + source > 0xffU;
    const bool overflow = (~(destination ^ source) & (destination ^ result) & 0x80U) != 0U;
    std::uint16_t flags = 0U;
    if (carry) flags |= 0x0011U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto result = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return result;
}
} // namespace

FunctionResult cpu_b_load_phase_table_pair(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    FunctionResult character_result;
    if (context.host->run_character_movement(context, character_result)) return character_result;
    auto &host = *context.host;
    auto &registers = context.registers;

    clear_long(host, registers, registers.address[5] + 0x1eU);
    clear_long(host, registers, registers.address[5] + 0x26U);

    const auto mode_byte = read_byte(host, registers.address[4] + 0x8bU);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | mode_byte;
    auto index = static_cast<std::uint16_t>(registers.data[0]) & 0x00f0U;
    set_logic_word(registers, index);
    const bool shifted_carry = (index & 0x0004U) != 0U;
    index = static_cast<std::uint16_t>(index >> 3U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | index;
    set_logic_word(registers, index);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x0011U) | (shifted_carry ? 0x0011U : 0U));

    registers.address[0] = 0x000107e4U + static_cast<std::int16_t>(index);
    auto table_index = host.read_memory_word(kRegion, registers.address[0], kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | table_index;
    set_logic_word(registers, table_index);

    if ((table_index & 0x8000U) == 0U) {
        host.write_memory_word(kRegion, registers.address[5] + 0x58U,
            table_index, kWordMask);
        const auto scaled = asl_word(registers, table_index, 2U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | scaled;
        registers.address[0] = 0x00010804U + static_cast<std::int16_t>(scaled);

        const auto phase = host.read_memory_word(
            kRegion, registers.address[5] + 0x36U, kWordMask);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | phase;
        set_logic_word(registers, phase);
        const auto phase_offset = asl_word(registers, phase, 4U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | phase_offset;
        registers.address[1] = 0x00010824U + static_cast<std::int16_t>(phase_offset);

        auto pair_index = host.read_memory_word(kRegion, registers.address[0], kWordMask);
        registers.address[0] += 2U;
        registers.data[0] = (registers.data[0] & 0xffff0000U) | pair_index;
        set_logic_word(registers, pair_index);
        if ((pair_index & 0x8000U) == 0U) {
            const auto value = read_long(host,
                registers.address[1] + static_cast<std::int16_t>(pair_index));
            write_long(host, registers.address[5] + 0x1eU, value);
            set_logic_long(registers, value);
        }

        pair_index = host.read_memory_word(kRegion, registers.address[0], kWordMask);
        registers.address[0] += 2U;
        registers.data[0] = (registers.data[0] & 0xffff0000U) | pair_index;
        set_logic_word(registers, pair_index);
        if ((pair_index & 0x8000U) == 0U) {
            const auto value = read_long(host,
                registers.address[1] + static_cast<std::int16_t>(pair_index));
            write_long(host, registers.address[5] + 0x26U, value);
            set_logic_long(registers, value);
        }
    } else {
        const auto phase = read_byte(host, registers.address[5] + 0x3dU);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | phase;
        const auto decremented = static_cast<std::uint8_t>(phase - 1U);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | decremented;
        set_sub_byte(registers, phase, 1U, decremented, true);
        if (decremented == 0U) {
            const auto return_address = pop_return(host, registers);
            registers.program_counter = return_address;
            return FunctionResult::complete(1U, return_address);
        }
    }

    const auto busy = read_byte(host, registers.address[5] + 0x3eU);
    set_logic_byte(registers, busy);
    if (busy == 0U) {
        const auto old_counter = read_byte(host, registers.address[5] + 0x3cU);
        const auto counter = static_cast<std::uint8_t>(old_counter + 1U);
        write_byte(host, registers.address[5] + 0x3cU, counter);
        set_add_byte(registers, old_counter, 1U, counter);

        const auto phase = host.read_memory_word(
            kRegion, registers.address[5] + 0x36U, kWordMask);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | phase;
        set_logic_word(registers, phase);
        registers.address[0] = 0x000108d4U;
        const auto limit = read_byte(host,
            registers.address[0] + static_cast<std::int16_t>(phase));
        registers.data[0] = (registers.data[0] & 0xffffff00U) | limit;
        set_logic_byte(registers, limit);
        const auto current = read_byte(host, registers.address[5] + 0x3cU);
        const auto comparison = static_cast<std::uint8_t>(limit - current);
        set_sub_byte(registers, limit, current, comparison, false);
        if (static_cast<std::int8_t>(limit) <= static_cast<std::int8_t>(current)) {
            (void)read_byte(host, registers.address[5] + 0x3cU);
            write_byte(host, registers.address[5] + 0x3cU, 0U);
            set_logic_byte(registers, 0U);
            const auto old_phase = read_byte(host, registers.address[5] + 0x3dU);
            registers.data[0] = (registers.data[0] & 0xffffff00U) | old_phase;
            const auto incremented_phase = static_cast<std::uint8_t>(old_phase + 1U);
            registers.data[0] = (registers.data[0] & 0xffffff00U) | incremented_phase;
            set_add_byte(registers, old_phase, 1U, incremented_phase);
            const auto next_phase = static_cast<std::uint8_t>(incremented_phase & 3U);
            registers.data[0] = (registers.data[0] & 0xffffff00U) | next_phase;
            set_logic_byte(registers, next_phase);
            write_byte(host, registers.address[5] + 0x3dU, next_phase);
            set_logic_byte(registers, next_phase);
        }
    }

    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
