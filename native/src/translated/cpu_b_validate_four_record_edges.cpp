#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kConditionMask = 0x001fU;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;
constexpr std::uint16_t kOverflow = 0x0002U;
constexpr std::uint16_t kCarry = 0x0001U;

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
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(
        kRegion, address & 0x00fffffeU, mask);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

[[nodiscard]] std::uint8_t read_lookup_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const bool private_memory = address <= 0x0003ffffU;
    const auto region = static_cast<std::uint16_t>(private_memory ? kRegion : 3U);
    const auto offset = private_memory ? address : address & 0x0003ffffU;
    const bool odd = (offset & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(
        region, offset & 0x0003fffeU, mask);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    host.write_memory_word(kRegion, address & 0x00fffffeU,
        static_cast<std::uint16_t>(value) << (odd ? 0U : 8U), mask);
}

void set_data_word(CpuRegisters &registers, std::size_t index, std::uint16_t value)
{
    registers.data[index] = (registers.data[index] & 0xffff0000U) | value;
}

void set_data_byte(CpuRegisters &registers, std::size_t index, std::uint8_t value)
{
    registers.data[index] = (registers.data[index] & 0xffffff00U) | value;
}

void set_logic_flags(CpuRegisters &registers,
    std::uint32_t value, std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & sign) != 0U) flags |= kNegative;
    if ((value & mask) == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

void compare_word(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    std::uint16_t flags = registers.status & kExtend;
    if ((result & 0x8000U) != 0U) flags |= kNegative;
    if (result == 0U) flags |= kZero;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= kOverflow;
    if (destination < source) flags |= kCarry;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

void compare_byte(CpuRegisters &registers,
    std::uint8_t destination, std::uint8_t source)
{
    const auto result = static_cast<std::uint8_t>(destination - source);
    std::uint16_t flags = registers.status & kExtend;
    if ((result & 0x80U) != 0U) flags |= kNegative;
    if (result == 0U) flags |= kZero;
    if (((destination ^ source) & (destination ^ result) & 0x80U) != 0U)
        flags |= kOverflow;
    if (destination < source) flags |= kCarry;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

[[nodiscard]] bool signed_greater(const CpuRegisters &registers)
{
    const bool negative = (registers.status & kNegative) != 0U;
    const bool overflow = (registers.status & kOverflow) != 0U;
    return (registers.status & kZero) == 0U && negative == overflow;
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_word(host, registers.address[7], static_cast<std::uint16_t>(value >> 16U));
    write_word(host, registers.address[7] + 2U, static_cast<std::uint16_t>(value));
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = read_word(host, registers.address[7]);
    const auto low = read_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

[[nodiscard]] FunctionResult finish(ExecutionHost &host, CpuRegisters &registers,
    std::uint8_t control)
{
    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(control, target);
}

[[nodiscard]] FunctionResult call_lookup(FunctionContext &context,
    std::uint32_t callsite, std::uint32_t continuation)
{
    auto &registers = context.registers;
    auto &host = *context.host;
    push_return(host, registers, continuation);
    registers.program_counter = 0x00015edeU;
    return host.call_function(
        287U, 1U, 0x72U, 2U, callsite, 0x00015edeU, context);
}

void load_attribute(CpuRegisters &registers, ExecutionHost &host)
{
    auto raw = read_lookup_byte(host, registers.address[0]);
    set_data_byte(registers, 3U, raw);
    set_logic_flags(registers, raw, 0x80U, 0xffU);
    raw = static_cast<std::uint8_t>(raw & 0xf0U);
    set_data_byte(registers, 3U, raw);
    set_logic_flags(registers, raw, 0x80U, 0xffU);
    const auto extended = static_cast<std::uint16_t>(
        static_cast<std::int16_t>(static_cast<std::int8_t>(raw)));
    set_data_word(registers, 3U, extended);
    set_logic_flags(registers, extended, 0x8000U, 0xffffU);
}
} // namespace

FunctionResult cpu_b_validate_four_record_edges(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];
    const auto entry_pc = registers.program_counter;
    if (entry_pc != 0x00020724U && entry_pc != 0x00020760U
        && entry_pc != 0x0002077eU && entry_pc != 0x00020794U
        && entry_pc != 0x000207aaU)
        return {TranslationStatus::contract_violation, 0U, entry_pc};

    if (entry_pc == 0x00020760U) goto resume_20760;
    if (entry_pc == 0x0002077eU) goto resume_2077e;
    if (entry_pc == 0x00020794U) goto resume_20794;
    if (entry_pc == 0x000207aaU) goto resume_207aa;

    {
        const auto value = read_word(host, base + 0x12U);
        set_data_word(registers, 0U, value);
        compare_word(registers, value, 0xffe0U);
        if ((registers.status & kNegative) != 0U) goto ancestor_reject;
        compare_word(registers, value, 0x01a0U);
        if (signed_greater(registers)) goto ancestor_reject;
    }
    {
        const auto value = read_word(host, base + 0x1aU);
        set_data_word(registers, 0U, value);
        compare_word(registers, value, 0xffe0U);
        if ((registers.status & kNegative) != 0U) goto ancestor_reject;
        compare_word(registers, value, 0x0210U);
        if ((registers.status & kNegative) == 0U) goto ancestor_reject;
    }

    set_data_word(registers, 0U, read_word(host, base + 0x2aU));
    set_data_word(registers, 1U, read_word(host, base + 0x32U));
    {
        const auto value = read_word(host, base + 0x2eU);
        set_data_word(registers, 2U, value);
        const auto doubled = static_cast<std::uint16_t>(value + value);
        set_data_word(registers, 2U, doubled);
        std::uint16_t flags{};
        if (static_cast<std::uint32_t>(value) + value > 0xffffU)
            flags |= 0x0011U;
        if (((~(value ^ value)) & (value ^ doubled) & 0x8000U) != 0U)
            flags |= kOverflow;
        if (doubled == 0U) flags |= kZero;
        if ((doubled & 0x8000U) != 0U) flags |= kNegative;
        registers.status = static_cast<std::uint16_t>(
            (registers.status & ~kConditionMask) | flags);
    }
    {
        const auto child = call_lookup(context, 0x0002075aU, 0x00020760U);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }

resume_20760:
    load_attribute(registers, host);
    compare_word(registers, static_cast<std::uint16_t>(registers.data[2]),
        static_cast<std::uint16_t>(registers.data[3]));
    if ((registers.status & kNegative) != 0U) goto normal_reject;
    {
        const auto mode = read_byte(host, base + 0x0bU);
        compare_byte(registers, mode, 0x02U);
        if ((registers.status & kZero) != 0U) goto normal_success;
    }

    set_data_word(registers, 1U, read_word(host, base + 0x34U));
    {
        const auto child = call_lookup(context, 0x00020778U, 0x0002077eU);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }
resume_2077e:
    load_attribute(registers, host);
    compare_word(registers, static_cast<std::uint16_t>(registers.data[2]),
        static_cast<std::uint16_t>(registers.data[3]));
    if ((registers.status & kNegative) != 0U) goto normal_reject;

    set_data_word(registers, 0U, read_word(host, base + 0x2cU));
    {
        const auto child = call_lookup(context, 0x0002078eU, 0x00020794U);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }
resume_20794:
    load_attribute(registers, host);
    compare_word(registers, static_cast<std::uint16_t>(registers.data[2]),
        static_cast<std::uint16_t>(registers.data[3]));
    if ((registers.status & kNegative) != 0U) goto normal_reject;

    set_data_word(registers, 1U, read_word(host, base + 0x32U));
    {
        const auto child = call_lookup(context, 0x000207a4U, 0x000207aaU);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }
resume_207aa:
    load_attribute(registers, host);
    compare_word(registers, static_cast<std::uint16_t>(registers.data[2]),
        static_cast<std::uint16_t>(registers.data[3]));
    if ((registers.status & kNegative) != 0U) goto normal_reject;

normal_success:
    registers.status = static_cast<std::uint16_t>(registers.status & ~kConditionMask);
    return finish(host, registers, 1U);

normal_reject:
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | kCarry);
    return finish(host, registers, 1U);

ancestor_reject:
    registers.address[7] += 4U;
    {
        const auto value = read_byte(host, base);
        if ((value & 0x80U) == 0U)
            registers.status |= kZero;
        else
            registers.status &= static_cast<std::uint16_t>(~kZero);
        write_byte(host, base, static_cast<std::uint8_t>(value & 0x7fU));
    }
    return finish(host, registers, 8U);
}

} // namespace gain_ground::translated
