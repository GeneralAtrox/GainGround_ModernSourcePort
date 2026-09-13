#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kConditionCodeMask = 0x001fU;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;

struct ByteLocation {
    std::uint32_t address;
    std::uint16_t mask;
    unsigned shift;
};

constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U),
        odd ? 0U : 8U};
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.address, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.address,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address, value, kWordMask);
}

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

void set_logic_flags(CpuRegisters &registers,
    std::uint32_t value, std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & sign) != 0U) flags |= kNegative;
    if ((value & mask) == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
}

void set_bit_test_flags(CpuRegisters &registers, bool bit_set)
{
    if (bit_set)
        registers.status = static_cast<std::uint16_t>(registers.status & ~kZero);
    else
        registers.status = static_cast<std::uint16_t>(registers.status | kZero);
}

void set_sub_byte_flags(CpuRegisters &registers,
    std::uint8_t left, std::uint8_t right, std::uint8_t result)
{
    std::uint16_t flags{};
    if ((result & 0x80U) != 0U) flags |= kNegative;
    if (result == 0U) flags |= kZero;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if (right > left) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], value);
}

void push_long_with_flags(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value));
    write_word(host, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U));
    set_logic_flags(registers, value, 0x80000000U, 0xffffffffU);
}

void push_word_with_flags(ExecutionHost &host, CpuRegisters &registers,
    std::uint16_t value)
{
    registers.address[7] -= 2U;
    write_word(host, registers.address[7], value);
    set_logic_flags(registers, value, 0x8000U, 0xffffU);
}

std::uint32_t pop_long(ExecutionHost &host, CpuRegisters &registers)
{
    const auto value = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return value;
}

std::uint16_t pop_word(ExecutionHost &host, CpuRegisters &registers)
{
    const auto value = read_word(host, registers.address[7]);
    registers.address[7] += 2U;
    return value;
}

FunctionResult invoke(ExecutionHost &host, FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t continuation)
{
    push_return(host, context.registers, continuation);
    context.registers.program_counter = target;
    return host.call_function(function_id, 1U, 0x72U, 2U,
        callsite, target, context);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    return pop_long(host, registers);
}
} // namespace

FunctionResult cpu_b_test_record_flag_41(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    const auto record_flag = read_byte(host, registers.address[5] + 0x41U);
    set_logic_flags(registers, record_flag, 0x80U, 0xffU);

    bool skip_timer_test = false;
    if (record_flag == 0U) {
        const auto entry_flags = read_byte(host, registers.address[4] + 0x8cU);
        const bool enabled = (entry_flags & 0x04U) != 0U;
        set_bit_test_flags(registers, enabled);
        if (enabled) {
            write_byte(host, registers.address[4] + 0x90U, 4U);
            set_logic_flags(registers, 4U, 0x80U, 0xffU);

            (void)read_byte(host, registers.address[4] + 0x91U);
            write_byte(host, registers.address[4] + 0x91U, 0U);
            set_logic_flags(registers, 0U, 0x80U, 0xffU);

            registers.address[0] = 0x0001158eU;
            const auto phase = read_byte(host, static_cast<std::uint32_t>(
                registers.address[0] + static_cast<std::int32_t>(
                    static_cast<std::int16_t>(registers.data[7]))));
            write_byte(host, registers.address[5] + 0x41U, phase);
            set_logic_flags(registers, phase, 0x80U, 0xffU);

            const auto command = read_word(host, registers.address[3] + 0x24U);
            registers.data[0] = (registers.data[0] & 0xffff0000U) | command;
            set_logic_flags(registers, command, 0x8000U, 0xffffU);

            push_long_with_flags(host, registers, registers.address[3]);
            push_word_with_flags(host, registers,
                static_cast<std::uint16_t>(registers.data[7]));
            push_long_with_flags(host, registers, registers.address[4]);

            auto child = invoke(host, context, 308U,
                0x00010df6U, 0x00016ff8U, 0x00010dfcU);
            if (child.status != TranslationStatus::complete || child.control != 1U)
                return child;

            registers.address[4] = pop_long(host, registers);
            const auto restored_d7 = pop_word(host, registers);
            registers.data[7] = (registers.data[7] & 0xffff0000U) | restored_d7;
            set_logic_flags(registers, restored_d7, 0x8000U, 0xffffU);
            registers.address[3] = pop_long(host, registers);
            skip_timer_test = true;
        }
    }

    if (!skip_timer_test) {
        const auto timer = read_byte(host, registers.address[4] + 0x90U);
        set_logic_flags(registers, timer, 0x80U, 0xffU);
        if (timer == 0U) {
            const auto target = pop_return(host, registers);
            registers.program_counter = target;
            return FunctionResult::complete(1U, target);
        }
    }

    const auto phase_address = registers.address[4] + 0x91U;
    const auto old_phase = read_byte(host, phase_address);
    const auto new_phase = static_cast<std::uint8_t>(old_phase - 1U);
    write_byte(host, phase_address, new_phase);
    set_sub_byte_flags(registers, old_phase, 1U, new_phase);
    const bool greater_than_zero = new_phase != 0U
        && ((new_phase & 0x80U) != 0U) == ((registers.status & 0x0002U) != 0U);
    if (greater_than_zero) {
        const auto target = pop_return(host, registers);
        registers.program_counter = target;
        return FunctionResult::complete(1U, target);
    }

    write_byte(host, phase_address, 4U);
    set_logic_flags(registers, 4U, 0x80U, 0xffU);

    const auto timer_address = registers.address[4] + 0x90U;
    const auto old_timer = read_byte(host, timer_address);
    const auto new_timer = static_cast<std::uint8_t>(old_timer - 1U);
    write_byte(host, timer_address, new_timer);
    set_sub_byte_flags(registers, old_timer, 1U, new_timer);

    struct Callback {
        std::uint32_t displacement;
        std::uint32_t function_id;
        std::uint32_t callsite;
        std::uint32_t continuation;
    };
    constexpr Callback callbacks[] = {
        {0x0cU, 237U, 0x00010e1eU, 0x00010e20U},
        {0x14U, 221U, 0x00010e24U, 0x00010e26U},
        {0x1cU, 243U, 0x00010e2aU, 0x00010e2cU},
    };

    for (const auto &callback : callbacks) {
        registers.address[0] = read_long(
            host, registers.address[3] + callback.displacement);
        auto child = invoke(host, context, callback.function_id,
            callback.callsite, registers.address[0], callback.continuation);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
