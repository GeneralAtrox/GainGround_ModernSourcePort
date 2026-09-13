#pragma once

#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated::record_deadline_detail {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kConditionMask = 0x001fU;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;
constexpr std::uint16_t kOverflow = 0x0002U;
constexpr std::uint16_t kCarry = 0x0001U;

inline std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & 0x0003ffffU, kWordMask);
}

inline void write_word(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value)
{
    host.write_memory_word(kRegion, address & 0x0003ffffU, value, kWordMask);
}

inline std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

inline void write_long(ExecutionHost &host, std::uint32_t address,
    std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

inline void write_long_read_modify_write(ExecutionHost &host,
    std::uint32_t address, std::uint32_t value)
{
    // A Motorola 68000 longword read-modify-write commits its low word first.
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
}

inline std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto offset = address & 0x0003ffffU;
    const bool odd = (offset & 1U) != 0U;
    const auto value = host.read_memory_word(kRegion, offset & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U));
    return static_cast<std::uint8_t>(odd ? value : value >> 8U);
}

inline void write_byte(ExecutionHost &host, std::uint32_t address,
    std::uint8_t value)
{
    const auto offset = address & 0x0003ffffU;
    const bool odd = (offset & 1U) != 0U;
    host.write_memory_word(kRegion, offset & ~1U,
        static_cast<std::uint16_t>(value) << (odd ? 0U : 8U),
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U));
}

inline void logic(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & sign) != 0U) flags |= kNegative;
    if ((value & mask) == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

inline void add(CpuRegisters &registers, std::uint32_t destination,
    std::uint32_t source, std::uint32_t result,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = 0U;
    if (static_cast<std::uint64_t>(destination & mask)
        + (source & mask) > mask)
        flags |= kExtend | kCarry;
    if (((~(destination ^ source)) & (destination ^ result) & sign) != 0U)
        flags |= kOverflow;
    if ((result & sign) != 0U) flags |= kNegative;
    if ((result & mask) == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

inline void subtract_word(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source, std::uint16_t result)
{
    std::uint16_t flags = 0U;
    if (source > destination) flags |= kExtend | kCarry;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= kOverflow;
    if ((result & 0x8000U) != 0U) flags |= kNegative;
    if (result == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

inline void subtract_long(CpuRegisters &registers,
    std::uint32_t destination, std::uint32_t source, std::uint32_t result)
{
    std::uint16_t flags = 0U;
    if (source > destination) flags |= kExtend | kCarry;
    if (((destination ^ source) & (destination ^ result) & 0x80000000U) != 0U)
        flags |= kOverflow;
    if ((result & 0x80000000U) != 0U) flags |= kNegative;
    if (result == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

inline void subtract_byte(CpuRegisters &registers,
    std::uint8_t destination, std::uint8_t source, std::uint8_t result)
{
    std::uint16_t flags = 0U;
    if (source > destination) flags |= kExtend | kCarry;
    if (((destination ^ source) & (destination ^ result) & 0x80U) != 0U)
        flags |= kOverflow;
    if ((result & 0x80U) != 0U) flags |= kNegative;
    if (result == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

inline std::uint16_t arithmetic_shift_right_word(CpuRegisters &registers,
    std::uint16_t value)
{
    const auto result = static_cast<std::uint16_t>(
        static_cast<std::int16_t>(value) >> 1);
    std::uint16_t flags = (value & 1U) != 0U ? kExtend | kCarry : 0U;
    if ((result & 0x8000U) != 0U) flags |= kNegative;
    if (result == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
    return result;
}

inline std::uint16_t arithmetic_shift_left_word(CpuRegisters &registers,
    std::uint16_t value, unsigned count)
{
    bool carry = false;
    bool overflow = false;
    for (unsigned index = 0; index < count; ++index) {
        carry = (value & 0x8000U) != 0U;
        const auto result = static_cast<std::uint16_t>(value << 1U);
        overflow = overflow || ((value ^ result) & 0x8000U) != 0U;
        value = result;
    }
    std::uint16_t flags = carry ? kExtend | kCarry : 0U;
    if (overflow) flags |= kOverflow;
    if ((value & 0x8000U) != 0U) flags |= kNegative;
    if (value == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
    return value;
}

inline FunctionResult call_child(FunctionContext &context, std::uint32_t id,
    std::uint32_t site, std::uint32_t target, std::uint32_t continuation)
{
    auto &registers = context.registers;
    registers.address[7] -= 4U;
    write_long(*context.host, registers.address[7], continuation);
    registers.program_counter = target;
    return context.host->call_function(id, 1U, 0x72U, 2U, site, target, context);
}

inline FunctionResult return_from(FunctionContext &context)
{
    auto &registers = context.registers;
    const auto target = read_long(*context.host, registers.address[7]);
    registers.address[7] += 4U;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

inline FunctionResult expire(FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto record = registers.address[5];

    auto value = read_byte(host, record + 1U);
    write_byte(host, record + 1U, static_cast<std::uint8_t>(value | 0x04U));
    if ((value & 0x04U) == 0U)
        registers.status |= kZero;
    else
        registers.status &= static_cast<std::uint16_t>(~kZero);

    write_long(host, record + 2U, 0x00012ceaU);
    logic(registers, 0x00012ceaU, 0x80000000U, 0xffffffffU);
    value = read_byte(host, record + 0x3fU);
    write_byte(host, record + 0x3fU, static_cast<std::uint8_t>(value | 0x80U));
    logic(registers, value, 0x80U, 0xffU);
    write_word(host, record + 0x10U, 0x2f2fU);
    logic(registers, 0x2f2fU, 0x8000U, 0xffffU);

    value = read_byte(host, record);
    write_byte(host, record, static_cast<std::uint8_t>(value & ~0x01U));
    if ((value & 0x01U) == 0U)
        registers.status |= kZero;
    else
        registers.status &= static_cast<std::uint16_t>(~kZero);

    auto result = call_child(context, 280U,
        0x00012cfaU, 0x00015d24U, 0x00012d00U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    result = call_child(context, 282U,
        0x00012d00U, 0x00015df2U, 0x00012d06U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    return return_from(context);
}

template<std::uint32_t Entry, bool AdvancePhase>
FunctionResult execute(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != Entry)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto record = registers.address[5];
    auto result = call_child(context, 253U,
        Entry, 0x0001283cU, Entry + 4U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    if ((registers.status & kCarry) != 0U)
        return expire(context);

    const auto timer = read_word(host, record + 0x46U);
    const auto remaining = static_cast<std::uint16_t>(timer - 1U);
    write_word(host, record + 0x46U, remaining);
    subtract_word(registers, timer, 1U, remaining);
    if ((registers.status & kZero) != 0U
        || ((registers.status & kNegative) != 0U)
            != ((registers.status & kOverflow) != 0U))
        return expire(context);

    result = call_child(context, 257U,
        Entry + 0x10U, 0x00012b32U, Entry + 0x14U);
    if (result.status != TranslationStatus::complete)
        return result;
    if (result.control == 8U)
        return FunctionResult::complete(1U, result.exit_program_counter);
    if (result.control != 1U)
        return result;
    if ((registers.status & kCarry) != 0U)
        return expire(context);

    auto source = read_long(host, record + 0x1eU);
    registers.data[0] = source;
    logic(registers, source, 0x80000000U, 0xffffffffU);
    auto destination = read_long(host, record + 0x12U);
    auto sum = destination + source;
    write_long_read_modify_write(host, record + 0x12U, sum);
    add(registers, destination, source, sum, 0x80000000U, 0xffffffffU);
    source = read_long(host, record + 0x26U);
    registers.data[0] = source;
    logic(registers, source, 0x80000000U, 0xffffffffU);
    destination = read_long(host, record + 0x1aU);
    sum = destination + source;
    write_long_read_modify_write(host, record + 0x1aU, sum);
    add(registers, destination, source, sum, 0x80000000U, 0xffffffffU);

    if constexpr (AdvancePhase) {
        const auto previous = read_byte(host, record + 0x3dU);
        const auto phase = static_cast<std::uint8_t>(previous + 1U);
        write_byte(host, record + 0x3dU, phase);
        add(registers, previous, 1U, phase, 0x80U, 0xffU);
        const auto loaded = read_byte(host, record + 0x3dU);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | loaded;
        logic(registers, loaded, 0x80U, 0xffU);
        const auto masked = static_cast<std::uint16_t>(registers.data[0] & 3U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | masked;
        logic(registers, masked, 0x8000U, 0xffffU);
        const auto sprite = static_cast<std::uint16_t>(masked + 0x9dU);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | sprite;
        add(registers, masked, 0x9dU, sprite, 0x8000U, 0xffffU);
        write_byte(host, record + 9U, static_cast<std::uint8_t>(sprite));
        logic(registers, static_cast<std::uint8_t>(sprite), 0x80U, 0xffU);
    }

    constexpr auto callsite = Entry + (AdvancePhase ? 0x3cU : 0x28U);
    result = call_child(context, 280U,
        callsite, 0x00015d24U, callsite + 6U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    result = call_child(context, 281U,
        callsite + 6U, 0x00015d3cU, callsite + 12U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    result = call_child(context, 282U,
        callsite + 12U, 0x00015df2U, callsite + 18U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    return return_from(context);
}
} // namespace gain_ground::translated::record_deadline_detail
