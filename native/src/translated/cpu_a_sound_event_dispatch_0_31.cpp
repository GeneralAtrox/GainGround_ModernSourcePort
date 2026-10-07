#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {

namespace {

constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kFullWordMask = 0xffffU;
constexpr std::uint32_t kSharedAddressMask = 0x0003ffffU;

[[maybe_unused]] void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(
        kSharedRegion, address & kSharedAddressMask, kFullWordMask);
}

[[maybe_unused]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(
        kSharedRegion, address & kSharedAddressMask, kFullWordMask);
}

[[maybe_unused]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

[[maybe_unused]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto offset = address & kSharedAddressMask;
    const auto mask = static_cast<std::uint16_t>(
        (offset & 1U) == 0U ? 0xff00U : 0x00ffU);
    const auto shift = (offset & 1U) == 0U ? 8U : 0U;
    return static_cast<std::uint8_t>(host.read_memory_word(
        kSharedRegion, offset & ~1U, mask) >> shift);
}

[[maybe_unused]] void write_word(
    ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(
        kSharedRegion, address & kSharedAddressMask, value, kFullWordMask);
}

[[maybe_unused]] void write_long(
    ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

[[maybe_unused]] void write_byte(
    ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto offset = address & kSharedAddressMask;
    const auto even = (offset & 1U) == 0U;
    host.write_memory_word(kSharedRegion, offset & ~1U,
        static_cast<std::uint16_t>(even ? static_cast<unsigned>(value) << 8U : value),
        static_cast<std::uint16_t>(even ? 0xff00U : 0x00ffU));
}

[[maybe_unused]] void set_logic_flags(
    CpuRegisters &registers, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[maybe_unused]] void set_add_byte_flags(
    CpuRegisters &registers, std::uint8_t left, std::uint8_t right,
    std::uint8_t result)
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

[[maybe_unused]] void set_add_word_flags(
    CpuRegisters &registers, std::uint16_t left, std::uint16_t right,
    std::uint16_t result)
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

[[maybe_unused]] void set_sub_byte_flags(
    CpuRegisters &registers, std::uint8_t left, std::uint8_t right,
    std::uint8_t result)
{
    std::uint16_t flags{};
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if (right > left) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[maybe_unused]] void set_compare_byte_flags(
    CpuRegisters &registers, std::uint8_t left, std::uint8_t right)
{
    const auto result = static_cast<std::uint8_t>(left - right);
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if (right > left) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[maybe_unused]] void left_shift_word_eight(CpuRegisters &registers)
{
    const auto prior = static_cast<std::uint16_t>(registers.data[0]);
    const auto result = static_cast<std::uint16_t>(prior << 8U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if ((prior & 0x0100U) != 0U) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[maybe_unused]] void move_stream_byte_to_data(
    ExecutionHost &host, CpuRegisters &registers, std::size_t data_register)
{
    const auto value = read_byte(host, registers.address[4]++);
    registers.data[data_register] =
        (registers.data[data_register] & 0xffffff00U) | value;
    set_logic_flags(registers, value, 0x80U);
}

[[maybe_unused]] FunctionResult return_from_subroutine(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack = registers.address[7];
    const auto target = (static_cast<std::uint32_t>(read_word(host, stack)) << 16U)
        | read_word(host, stack + 2U);
    registers.address[7] += 4U;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

[[maybe_unused]] void push_return(
    ExecutionHost &host, CpuRegisters &registers, std::uint32_t address)
{
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], address);
}

[[maybe_unused]] FunctionResult call_ym2151_writer(
    FunctionContext &context, std::uint32_t callsite, std::uint32_t return_address)
{
    auto &host = *context.host;
    push_return(host, context.registers, return_address);
    prefetch(host, 0x00084126U);
    prefetch(host, 0x00084128U);
    context.registers.program_counter = 0x00084126U;
    return host.call_function(88U, 0U, 0xffU, 2U,
        callsite, 0x00084126U, context);
}

[[maybe_unused]] FunctionResult call_native(
    FunctionContext &context, std::uint32_t function_id,
    std::uint32_t callsite, std::uint32_t target,
    std::uint32_t return_address)
{
    auto &host = *context.host;
    push_return(host, context.registers, return_address);
    prefetch(host, target);
    prefetch(host, target + 2U);
    context.registers.program_counter = target;
    return host.call_function(function_id, 0U, 0xffU, 2U,
        callsite, target, context);
}

[[maybe_unused]] void move_stream_byte_to_channel(
    ExecutionHost &host, CpuRegisters &registers, std::uint32_t offset)
{
    const auto value = read_byte(host, registers.address[4]++);
    write_byte(host, registers.address[3] + offset, value);
    set_logic_flags(registers, value, 0x80U);
}

[[maybe_unused]] void modify_channel_bit(
    ExecutionHost &host, CpuRegisters &registers, std::uint32_t offset,
    std::uint8_t bit, bool set)
{
    const auto address = registers.address[3] + offset;
    const auto prior = read_byte(host, address);
    const auto mask = static_cast<std::uint8_t>(1U << bit);
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
        | ((prior & mask) == 0U ? 0x0004U : 0U));
    const auto result = static_cast<std::uint8_t>(set ? prior | mask : prior & ~mask);
    write_byte(host, address, result);
}

#include "cpu_a_sound_event_selectors_00_07.inc" // gground-source-split
#include "cpu_a_sound_event_selectors_08_15.inc" // gground-source-split
#include "cpu_a_sound_event_selectors_16_23.inc" // gground-source-split
#include "cpu_a_sound_event_selectors_24_31.inc" // gground-source-split
} // namespace

FunctionResult cpu_a_sound_event_dispatch_0_31(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    auto value = static_cast<std::uint16_t>(registers.data[0]) & 0x001fU;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    set_logic_flags(registers, value, 0x8000U);

    prefetch(host, 0x00083648U);
    auto doubled = static_cast<std::uint16_t>(value + value);
    set_add_word_flags(registers, value, value, doubled);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | doubled;

    prefetch(host, 0x0008364aU);
    value = doubled;
    doubled = static_cast<std::uint16_t>(value + value);
    set_add_word_flags(registers, value, value, doubled);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | doubled;

    prefetch(host, 0x0008364cU);
    prefetch(host, 0x0008364eU);
    const auto selector = static_cast<std::uint8_t>(doubled >> 2U);
    const auto slot = 0x00083650U + doubled;
    prefetch(host, slot);
    prefetch(host, slot + 2U);

    switch (selector) {
    case 0:
        return handle_cpu_a_sound_event_selectors_00_07(
            host, registers, context, selector, value);
    case 1:
        return handle_cpu_a_sound_event_selectors_00_07(
            host, registers, context, selector, value);
    case 2:
        return handle_cpu_a_sound_event_selectors_00_07(
            host, registers, context, selector, value);
    case 3:
        return handle_cpu_a_sound_event_selectors_00_07(
            host, registers, context, selector, value);
    case 4:
        return handle_cpu_a_sound_event_selectors_00_07(
            host, registers, context, selector, value);
    case 5:
        return handle_cpu_a_sound_event_selectors_00_07(
            host, registers, context, selector, value);
    case 6:
        return handle_cpu_a_sound_event_selectors_00_07(
            host, registers, context, selector, value);
    case 7:
        return handle_cpu_a_sound_event_selectors_00_07(
            host, registers, context, selector, value);
    case 8:
        return handle_cpu_a_sound_event_selectors_08_15(
            host, registers, context, selector, value);
    case 9:
        return handle_cpu_a_sound_event_selectors_08_15(
            host, registers, context, selector, value);
    case 10:
        return handle_cpu_a_sound_event_selectors_08_15(
            host, registers, context, selector, value);
    case 11:
        return handle_cpu_a_sound_event_selectors_08_15(
            host, registers, context, selector, value);
    case 12:
        return handle_cpu_a_sound_event_selectors_08_15(
            host, registers, context, selector, value);
    case 13:
        return handle_cpu_a_sound_event_selectors_08_15(
            host, registers, context, selector, value);
    case 14:
        return handle_cpu_a_sound_event_selectors_08_15(
            host, registers, context, selector, value);
    case 15:
        return handle_cpu_a_sound_event_selectors_00_07(
            host, registers, context, selector, value);
    case 16:
        return handle_cpu_a_sound_event_selectors_00_07(
            host, registers, context, selector, value);
    case 17:
        return handle_cpu_a_sound_event_selectors_16_23(
            host, registers, context, selector, value);
    case 18:
        return handle_cpu_a_sound_event_selectors_00_07(
            host, registers, context, selector, value);
    case 19:
        return handle_cpu_a_sound_event_selectors_16_23(
            host, registers, context, selector, value);
    case 20:
        return handle_cpu_a_sound_event_selectors_16_23(
            host, registers, context, selector, value);
    case 21:
        return handle_cpu_a_sound_event_selectors_16_23(
            host, registers, context, selector, value);
    case 22:
        return handle_cpu_a_sound_event_selectors_16_23(
            host, registers, context, selector, value);
    case 23:
        return handle_cpu_a_sound_event_selectors_16_23(
            host, registers, context, selector, value);
    case 24:
        return handle_cpu_a_sound_event_selectors_24_31(
            host, registers, context, selector, value);
    case 25:
        return handle_cpu_a_sound_event_selectors_24_31(
            host, registers, context, selector, value);
    case 26:
        return handle_cpu_a_sound_event_selectors_24_31(
            host, registers, context, selector, value);
    case 27:
        return handle_cpu_a_sound_event_selectors_24_31(
            host, registers, context, selector, value);
    case 28:
        return handle_cpu_a_sound_event_selectors_00_07(
            host, registers, context, selector, value);
    case 29:
        return handle_cpu_a_sound_event_selectors_00_07(
            host, registers, context, selector, value);
    case 30:
        return handle_cpu_a_sound_event_selectors_00_07(
            host, registers, context, selector, value);
    case 31:
        return handle_cpu_a_sound_event_selectors_00_07(
            host, registers, context, selector, value);
    default:
        return FunctionResult::unimplemented();
    }
}

} // namespace gain_ground::translated
