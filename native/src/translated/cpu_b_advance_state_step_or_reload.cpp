#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] constexpr ByteLocation locate(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U),
        odd ? 0U : 8U};
}

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & 0x00ffffffU, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address & 0x00ffffffU, value, kWordMask);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate(address & 0x00ffffffU);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate(address & 0x00ffffffU);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

void logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void sub_byte(CpuRegisters &registers, std::uint8_t left,
    std::uint8_t right, std::uint8_t result)
{
    std::uint16_t flags{};
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if (left < right) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void sub_word(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (left < right) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void add_word(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void bclr(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address, unsigned bit)
{
    const auto value = read_byte(host, address);
    write_byte(host, address, static_cast<std::uint8_t>(value & ~(1U << bit)));
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
        | (((value & (1U << bit)) == 0U) ? 0x0004U : 0U));
}

void bset(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address, unsigned bit)
{
    const auto value = read_byte(host, address);
    write_byte(host, address, static_cast<std::uint8_t>(value | (1U << bit)));
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
        | (((value & (1U << bit)) == 0U) ? 0x0004U : 0U));
}

[[nodiscard]] bool btst(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address, unsigned bit)
{
    const auto value = read_byte(host, address);
    const bool clear = (value & (1U << bit)) == 0U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
        | (clear ? 0x0004U : 0U));
    return !clear;
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], value);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = read_word(host, registers.address[7]);
    const auto low = read_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_advance_state_step_or_reload(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];

    const auto return_complete = [&]() -> FunctionResult {
        const auto target = pop_return(host, registers);
        registers.program_counter = target;
        return FunctionResult::complete(1U, target);
    };
    const auto finish_flags = [&]() -> FunctionResult {
        bclr(host, registers, base + 0x40U, 0U);
        bset(host, registers, base + 0x40U, 3U);
        bset(host, registers, base + 0x40U, 2U);
        return return_complete();
    };
    const auto finish_collision = [&]() -> FunctionResult {
        bclr(host, registers, base + 0x40U, 1U);
        bclr(host, registers, base + 0x40U, 5U);
        return finish_flags();
    };

    const auto reload = read_byte(host, base + 0x3fU);
    logic_byte(registers, reload);
    if (reload != 0U) {
        registers.program_counter = 0x0001d9daU;
        return host.call_function(550U, 1U, 0x72U, 1U,
            0x0001cefcU, 0x0001d9daU, context);
    }

    auto step = read_byte(host, base + 0x3eU);
    logic_byte(registers, step);
    if (step != 0U) {
        write_byte(host, base + 0x54U, 1U);
        // The 68000 memory-destination SUBQ byte performs the captured
        // read-modify-write bus cycle independently of the preceding TST.
        const auto subtract_operand = read_byte(host, base + 0x3eU);
        const auto decremented = static_cast<std::uint8_t>(subtract_operand - 1U);
        write_byte(host, base + 0x3eU, decremented);
        sub_byte(registers, subtract_operand, 1U, decremented);
        const auto reread = read_byte(host, base + 0x3eU);
        const auto even = static_cast<std::uint8_t>(reread & 0xfeU);
        write_byte(host, base + 0x3eU, even);
        logic_byte(registers, even);
        bset(host, registers, base + 0x40U, 1U);
        bset(host, registers, base + 0x40U, 3U);
        const auto cursor = read_word(host, base + 0x5cU);
        const auto advanced = static_cast<std::uint16_t>(cursor + 0x0080U);
        write_word(host, base + 0x5cU, advanced);
        add_word(registers, cursor, 0x0080U, advanced);
        const auto mask_operand = read_word(host, base + 0x5cU);
        const auto masked = static_cast<std::uint16_t>(mask_operand & 0x0780U);
        write_word(host, base + 0x5cU, masked);
        logic_word(registers, masked);
        write_byte(host, base + 0x5eU, 9U);
        logic_byte(registers, 9U);
        return return_complete();
    }

    auto timer = read_word(host, base + 0x74U);
    logic_word(registers, timer);
    if ((timer & 0x8000U) != 0U) {
        auto difference = read_word(host, base + 0x12U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | difference;
        const auto first_right = read_word(host, base + 0x62U);
        auto result = static_cast<std::uint16_t>(difference - first_right);
        sub_word(registers, difference, first_right, result);
        difference = result;
        result = static_cast<std::uint16_t>(difference + 3U);
        add_word(registers, difference, 3U, result);
        result = static_cast<std::uint16_t>(result & 0xfffcU);
        logic_word(registers, result);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
        bool aligned = result == 0U;
        if (aligned) {
            difference = read_word(host, base + 0x1aU);
            registers.data[0] = (registers.data[0] & 0xffff0000U) | difference;
            const auto second_right = read_word(host, base + 0x64U);
            result = static_cast<std::uint16_t>(difference - second_right);
            sub_word(registers, difference, second_right, result);
            difference = result;
            result = static_cast<std::uint16_t>(difference + 3U);
            add_word(registers, difference, 3U, result);
            result = static_cast<std::uint16_t>(result & 0xfffcU);
            logic_word(registers, result);
            registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
            aligned = result == 0U;
        }
        if (aligned) {
            write_word(host, base + 0x74U, 15U);
            logic_word(registers, 15U);
            bset(host, registers, base + 0x40U, 1U);
            return finish_flags();
        }

        bclr(host, registers, base + 0x40U, 1U);
        push_return(host, registers, 0x0001cf3eU);
        registers.program_counter = 0x0001de30U;
        const auto child = host.call_function(355U, 1U, 0x72U, 2U,
            0x0001cf3aU, 0x0001de30U, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;

        if (btst(host, registers, base + 0x40U, 1U)) {
            const auto value = read_word(host, base + 0x3aU);
            write_word(host, base + 0x5cU, value);
            logic_word(registers, value);
            write_byte(host, base + 0x36U, 0x20U);
            logic_byte(registers, 0x20U);
            return finish_collision();
        }
        if (btst(host, registers, base + 0x40U, 6U)
            || btst(host, registers, base + 0x40U, 7U))
            return finish_collision();

        write_word(host, base + 0x74U, 15U);
        logic_word(registers, 15U);
        bset(host, registers, base + 0x40U, 1U);
        return finish_flags();
    }

    timer = read_word(host, base + 0x74U);
    const auto decremented = static_cast<std::uint16_t>(timer - 1U);
    write_word(host, base + 0x74U, decremented);
    sub_word(registers, timer, 1U, decremented);
    if (decremented == 0U) {
        bclr(host, registers, base + 0x41U, 3U);
        bclr(host, registers, base + 0x40U, 1U);
        write_byte(host, base + 0x0bU, 9U);
        logic_byte(registers, 9U);
    }
    return finish_flags();
}

} // namespace gain_ground::translated
