#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
FunctionResult cpu_a_sound_write_modulation_value(FunctionContext &context) noexcept;

namespace {
constexpr std::uint16_t kRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kRegion, address & kAddressMask, kWordMask);
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto offset = address & kAddressMask;
    const auto mask = static_cast<std::uint16_t>(
        (offset & 1U) == 0U ? 0xff00U : 0x00ffU);
    const auto shift = (offset & 1U) == 0U ? 8U : 0U;
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, offset & ~1U, mask) >> shift);
}

void set_logic_flags(CpuRegisters &registers,
    std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_byte_flags(CpuRegisters &registers,
    std::uint8_t left, std::uint8_t right, std::uint8_t result)
{
    const auto sum = static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(left) + right);
    std::uint16_t flags{};
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if (sum > 0xffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    const auto sum = static_cast<std::uint32_t>(left) + right;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (sum > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t return_address)
{
    registers.address[7] -= 4U;
    const auto offset = registers.address[7] & kAddressMask;
    host.write_memory_word(kRegion, offset,
        static_cast<std::uint16_t>(return_address >> 16U), kWordMask);
    host.write_memory_word(kRegion, offset + 2U,
        static_cast<std::uint16_t>(return_address), kWordMask);
}

FunctionResult call_modulation(FunctionContext &context,
    std::uint32_t callsite, std::uint32_t return_address)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    prefetch(host, callsite);
    prefetch(host, callsite + 2U);
    push_return(host, registers, return_address);
    prefetch(host, 0x000842b0U);
    prefetch(host, 0x000842b2U);
    registers.program_counter = 0x000842b0U;
    return host.call_function(95U, 0U, 0xffU, 2U,
        callsite, 0x000842b0U, context);
}

FunctionResult tail_modulation(FunctionContext &context, std::uint32_t callsite)
{
    auto &host = *context.host;
    prefetch(host, callsite);
    prefetch(host, callsite + 2U);
    prefetch(host, 0x000842b0U);
    prefetch(host, 0x000842b2U);
    context.registers.program_counter = 0x000842b0U;
    const auto child = host.call_function(95U, 0U, 0xffU, 1U,
        callsite, 0x000842b0U, context);
    if (child.status != TranslationStatus::complete
            || (child.control == 3U
                && child.exit_program_counter == 0x000842b0U))
        return child;
    return FunctionResult::complete(1U, context.registers.program_counter);
}

void add_register_byte(CpuRegisters &registers, std::uint8_t immediate)
{
    const auto before = static_cast<std::uint8_t>(registers.data[0]);
    const auto after = static_cast<std::uint8_t>(before + immediate);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | after;
    set_add_byte_flags(registers, before, immediate, after);
}

void set_operator(CpuRegisters &registers, std::uint8_t value)
{
    registers.data[3] = value;
    set_logic_flags(registers, registers.data[3], 0x80000000U);
}
} // namespace

FunctionResult cpu_a_sound_write_channel_modulation(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x0060U;
    set_logic_flags(registers, 0x0060U, 0x8000U);

    prefetch(host, 0x0008421eU);
    add_register_byte(registers, static_cast<std::uint8_t>(registers.data[7]));

    prefetch(host, 0x00084220U);
    prefetch(host, 0x00084222U);
    prefetch(host, 0x00084224U);
    const auto raw_selector = read_byte(host, registers.address[3] + 0x17U);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | raw_selector;
    set_logic_flags(registers, raw_selector, 0x80U);

    prefetch(host, 0x00084226U);
    auto selector = static_cast<std::uint16_t>(registers.data[1]) & 7U;
    registers.data[1] = (registers.data[1] & 0xffff0000U) | selector;
    set_logic_flags(registers, selector, 0x8000U);

    prefetch(host, 0x00084228U);
    auto doubled = static_cast<std::uint16_t>(selector + selector);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | doubled;
    set_add_word_flags(registers, selector, selector, doubled);
    prefetch(host, 0x0008422aU);
    selector = doubled;
    doubled = static_cast<std::uint16_t>(selector + selector);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | doubled;
    set_add_word_flags(registers, selector, selector, doubled);

    prefetch(host, 0x0008422cU);
    prefetch(host, 0x0008422eU);
    const auto mode = static_cast<std::uint8_t>(doubled >> 2U);
    const auto slot = 0x00084230U + doubled;
    prefetch(host, slot);
    prefetch(host, slot + 2U);

    if (mode <= 3U) {
        prefetch(host, 0x00084250U);
        prefetch(host, 0x00084252U);
        add_register_byte(registers, 0x18U);
        prefetch(host, 0x00084254U);
        set_operator(registers, 0x1fU);
        return tail_modulation(context, 0x00084256U);
    }

    if (mode == 4U) {
        prefetch(host, 0x0008425aU);
        prefetch(host, 0x0008425cU);
        add_register_byte(registers, 0x10U);
        prefetch(host, 0x0008425eU);
        set_operator(registers, 0x1eU);
        const auto first = call_modulation(context, 0x00084260U, 0x00084264U);
        if (first.status != TranslationStatus::complete) return first;
        add_register_byte(registers, 8U);
        prefetch(host, 0x00084268U);
        set_operator(registers, 0x1fU);
        return tail_modulation(context, 0x0008426aU);
    }

    if (mode <= 6U) {
        prefetch(host, 0x0008426eU);
        prefetch(host, 0x00084270U);
        add_register_byte(registers, 8U);
        prefetch(host, 0x00084272U);
        set_operator(registers, 0x1dU);
        const auto first = call_modulation(context, 0x00084274U, 0x00084278U);
        if (first.status != TranslationStatus::complete) return first;
        add_register_byte(registers, 8U);
        prefetch(host, 0x0008427cU);
        set_operator(registers, 0x1eU);
        const auto second = call_modulation(context, 0x0008427eU, 0x00084282U);
        if (second.status != TranslationStatus::complete) return second;
        add_register_byte(registers, 8U);
        prefetch(host, 0x00084286U);
        set_operator(registers, 0x1fU);
        return tail_modulation(context, 0x00084288U);
    }

    prefetch(host, 0x0008428cU);
    prefetch(host, 0x0008428eU);
    add_register_byte(registers, 0U);
    prefetch(host, 0x00084290U);
    set_operator(registers, 0x1cU);
    const auto first = call_modulation(context, 0x00084292U, 0x00084296U);
    if (first.status != TranslationStatus::complete) return first;
    add_register_byte(registers, 8U);
    prefetch(host, 0x0008429aU);
    set_operator(registers, 0x1dU);
    const auto second = call_modulation(context, 0x0008429cU, 0x000842a0U);
    if (second.status != TranslationStatus::complete) return second;
    add_register_byte(registers, 8U);
    prefetch(host, 0x000842a4U);
    set_operator(registers, 0x1eU);
    const auto third = call_modulation(context, 0x000842a6U, 0x000842aaU);
    if (third.status != TranslationStatus::complete) return third;
    add_register_byte(registers, 8U);
    prefetch(host, 0x000842aeU);
    set_operator(registers, 0x1fU);
    prefetch(host, 0x000842b0U);
    prefetch(host, 0x000842b2U);
    registers.program_counter = 0x000842b0U;
    const auto final = cpu_a_sound_write_modulation_value(context);
    if (final.status != TranslationStatus::complete) return final;
    return FunctionResult::complete(1U, context.registers.program_counter);
}

} // namespace gain_ground::translated
