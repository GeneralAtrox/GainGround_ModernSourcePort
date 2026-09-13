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

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & 0x0003ffffU, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address & 0x0003ffffU, value, kWordMask);
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto offset = address & 0x0003ffffU;
    const bool odd = (offset & 1U) != 0U;
    const auto word = host.read_memory_word(kRegion, offset & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U));
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto offset = address & 0x0003ffffU;
    const bool odd = (offset & 1U) != 0U;
    host.write_memory_word(kRegion, offset & ~1U,
        static_cast<std::uint16_t>(value) << (odd ? 0U : 8U),
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U));
}

void logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & 0x80U) != 0U) flags |= kNegative;
    if (value == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

void logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & 0x8000U) != 0U) flags |= kNegative;
    if (value == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

void add_byte(CpuRegisters &registers, std::uint8_t destination,
    std::uint8_t source, std::uint8_t result)
{
    std::uint16_t flags = 0U;
    if (static_cast<unsigned>(destination) + source > 0xffU)
        flags |= kExtend | kCarry;
    if (((~(destination ^ source)) & (destination ^ result) & 0x80U) != 0U)
        flags |= kOverflow;
    if ((result & 0x80U) != 0U) flags |= kNegative;
    if (result == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

void add_word(CpuRegisters &registers, std::uint16_t destination,
    std::uint16_t source, std::uint16_t result)
{
    std::uint16_t flags = 0U;
    if (static_cast<std::uint32_t>(destination) + source > 0xffffU)
        flags |= kExtend | kCarry;
    if (((~(destination ^ source)) & (destination ^ result) & 0x8000U) != 0U)
        flags |= kOverflow;
    if ((result & 0x8000U) != 0U) flags |= kNegative;
    if (result == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

void compare_byte(CpuRegisters &registers, std::uint8_t destination,
    std::uint8_t source)
{
    const auto result = static_cast<std::uint8_t>(destination - source);
    std::uint16_t flags = registers.status & kExtend;
    if (source > destination) flags |= kCarry;
    if (((destination ^ source) & (destination ^ result) & 0x80U) != 0U)
        flags |= kOverflow;
    if ((result & 0x80U) != 0U) flags |= kNegative;
    if (result == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

std::uint16_t shift_left_word(CpuRegisters &registers,
    std::uint16_t value, unsigned count)
{
    bool carry = false;
    bool overflow = false;
    for (unsigned index = 0; index < count; ++index) {
        carry = (value & 0x8000U) != 0U;
        const auto shifted = static_cast<std::uint16_t>(value << 1U);
        overflow = overflow || ((value ^ shifted) & 0x8000U) != 0U;
        value = shifted;
    }
    std::uint16_t flags = 0U;
    if (carry) flags |= kExtend | kCarry;
    if (overflow) flags |= kOverflow;
    if ((value & 0x8000U) != 0U) flags |= kNegative;
    if (value == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
    return value;
}

FunctionResult call_child(FunctionContext &context, std::uint32_t id,
    std::uint32_t site, std::uint32_t target, std::uint32_t continuation)
{
    auto &registers = context.registers;
    registers.address[7] -= 4U;
    write_word(*context.host, registers.address[7],
        static_cast<std::uint16_t>(continuation >> 16U));
    write_word(*context.host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(continuation));
    registers.program_counter = target;
    return context.host->call_function(id, 1U, 0x72U, 2U, site, target, context);
}

FunctionResult return_from(FunctionContext &context)
{
    auto &registers = context.registers;
    const auto target = (static_cast<std::uint32_t>(
        read_word(*context.host, registers.address[7])) << 16U)
        | read_word(*context.host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_increment_wrapped_phase(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x0001250aU)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto record = registers.address[5];

    const auto gate = read_byte(host, record + 0x3fU);
    logic_byte(registers, gate);
    if (gate == 0U) {
        const auto result = call_child(context, 253U,
            0x00012510U, 0x0001283cU, 0x00012514U);
        if (result.status != TranslationStatus::complete || result.control != 1U)
            return result;
    }

    const auto old_phase = read_byte(host, record + 0x3dU);
    const auto phase = static_cast<std::uint8_t>(old_phase + 2U);
    write_byte(host, record + 0x3dU, phase);
    add_byte(registers, old_phase, 2U, phase);
    compare_byte(registers, read_byte(host, record + 0x3dU), 0x28U);
    if ((registers.status & kCarry) == 0U) {
        (void)read_byte(host, record);
        write_byte(host, record, 0U);
        logic_byte(registers, 0U);
        return return_from(context);
    }

    registers.data[0] = 0U;
    logic_word(registers, 0U);
    const auto current_phase = read_byte(host, record + 0x3dU);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | current_phase;
    logic_byte(registers, current_phase);
    registers.address[0] = 0x00012f32U;

    auto value = read_byte(host, registers.address[0]
        + static_cast<std::uint16_t>(registers.data[0]));
    write_byte(host, record + 0x10U, value);
    logic_byte(registers, value);
    value = read_byte(host, registers.address[0]
        + static_cast<std::uint16_t>(registers.data[0]));
    write_byte(host, record + 0x11U, value);
    logic_byte(registers, value);
    value = read_byte(host, registers.address[0]
        + static_cast<std::uint16_t>(registers.data[0]) + 1U);
    write_byte(host, record + 0x09U, value);
    logic_byte(registers, value);

    auto d0 = shift_left_word(registers,
        static_cast<std::uint16_t>(registers.data[0]), 2U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    auto d1 = read_word(host, record + 0x58U);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | d1;
    logic_word(registers, d1);
    d1 = shift_left_word(registers, d1, 5U);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | d1;
    registers.data[2] = (registers.data[2] & 0xffff0000U) | d1;
    logic_word(registers, d1);
    const auto d2 = d1;
    d1 = shift_left_word(registers, d1, 2U);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | d1;
    auto sum = static_cast<std::uint16_t>(d1 + d2);
    add_word(registers, d1, d2, sum);
    d1 = sum;
    registers.data[1] = (registers.data[1] & 0xffff0000U) | d1;
    sum = static_cast<std::uint16_t>(d0 + d1);
    add_word(registers, d0, d1, sum);
    d0 = sum;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    registers.address[0] = static_cast<std::uint32_t>(0x00012f5aU
        + static_cast<std::int16_t>(d0));

    auto word = read_word(host, registers.address[0]);
    registers.address[0] += 2U;
    write_word(host, record + 0x06U, word);
    logic_word(registers, word);
    value = read_byte(host, registers.address[0]++);
    write_byte(host, record + 0x01U, value);
    logic_byte(registers, value);
    registers.address[0] += 1U;
    word = read_word(host, registers.address[0]);
    registers.address[0] += 2U;
    write_word(host, record + 0x1eU, word);
    logic_word(registers, word);
    word = read_word(host, registers.address[0]);
    registers.address[0] += 2U;
    write_word(host, record + 0x26U, word);
    logic_word(registers, word);

    auto result = call_child(context, 257U,
        0x00012568U, 0x00012b32U, 0x0001256cU);
    if (result.status == TranslationStatus::complete && result.control == 8U)
        return FunctionResult::complete(1U, result.exit_program_counter);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    if ((registers.status & kCarry) != 0U) {
        const auto state = read_byte(host, record + 0x3fU);
        write_byte(host, record + 0x3fU,
            static_cast<std::uint8_t>(state | 0x80U));
        logic_byte(registers, state);
    } else {
        word = read_word(host, record + 0x1eU);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | word;
        logic_word(registers, word);
        auto destination = read_word(host, record + 0x12U);
        sum = static_cast<std::uint16_t>(destination + word);
        write_word(host, record + 0x12U, sum);
        add_word(registers, destination, word, sum);
        word = read_word(host, record + 0x26U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | word;
        logic_word(registers, word);
        destination = read_word(host, record + 0x1aU);
        sum = static_cast<std::uint16_t>(destination + word);
        write_word(host, record + 0x1aU, sum);
        add_word(registers, destination, word, sum);
    }

    result = call_child(context, 280U,
        0x00012584U, 0x00015d24U, 0x0001258aU);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    result = call_child(context, 281U,
        0x0001258aU, 0x00015d3cU, 0x00012590U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    result = call_child(context, 282U,
        0x00012590U, 0x00015df2U, 0x00012596U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    return return_from(context);
}
} // namespace gain_ground::translated
