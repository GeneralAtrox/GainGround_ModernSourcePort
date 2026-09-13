#include "gain_ground/contract_types.h"
#include "gain_ground/sound_caller_timing.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] ByteLocation locate_byte(std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    return {address & 0x0003fffeU,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(SoundCallerTiming &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_long(SoundCallerTiming &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kRegion, address & kAddressMask,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, (address + 2U) & kAddressMask,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t read_long(SoundCallerTiming &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(
        kRegion, address & kAddressMask, kWordMask);
    const auto low = host.read_memory_word(
        kRegion, (address + 2U) & kAddressMask, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void prefetch(SoundCallerTiming &host, std::uint32_t address)
{
    (void)host.read_memory_word(kRegion, address & kAddressMask, kWordMask);
}

void set_logic(CpuRegisters &registers, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_byte(CpuRegisters &registers,
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

void set_add_word(CpuRegisters &registers,
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

void set_compare_byte(CpuRegisters &registers,
    std::uint8_t destination, std::uint8_t source)
{
    const auto result = static_cast<std::uint8_t>(destination - source);
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (source > destination) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_return(SoundCallerTiming &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], value);
}

[[nodiscard]] FunctionResult call_native(SoundCallerTiming &host, FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t return_address)
{
    // BSR.w: two internal clocks, then the original two stack writes and
    // two target prefetches. Inactive sections retain their existing timing.
    host.clocks(2U);
    push_return(host, context.registers, return_address);
    prefetch(host, target);
    prefetch(host, target + 2U);
    context.registers.program_counter = target;
    return host.call_function(function_id, 0U, 0xffU, 2U,
        callsite, target, context);
}

[[nodiscard]] FunctionResult finish(SoundCallerTiming &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_a_sound_initialize_channel_registers(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    SoundCallerTiming host(context);
    auto &registers = context.registers;

    // MOVEQ / ADD.B / MOVEQ / BSR.w prefix; implemented but unverified.
    host.begin(0x00083ee4U);
    prefetch(host, 0x00083ee8U);
    registers.data[0] = 0x20U;
    set_logic(registers, registers.data[0], 0x80000000U);
    const auto left = static_cast<std::uint8_t>(registers.data[0]);
    const auto right = static_cast<std::uint8_t>(registers.data[7]);
    const auto sum = static_cast<std::uint8_t>(left + right);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | sum;
    set_add_byte(registers, left, right, sum);
    registers.data[1] = 0U;
    set_logic(registers, 0U, 0x80000000U);

    prefetch(host, 0x00083eeaU);
    prefetch(host, 0x00083eecU);
    auto child = call_native(host, context, 87U,
        0x00083eeaU, 0x0008410eU, 0x00083eeeU);
    if (child.status != TranslationStatus::complete) return child;

    host.begin(0x00083eeeU);
    child = call_native(host, context, 96U,
        0x00083eeeU, 0x000842c0U, 0x00083ef2U);
    if (child.status != TranslationStatus::complete) return child;

    host.begin(0x00083ef2U);
    registers.data[1] = (registers.data[1] & 0xffffff00U)
        | static_cast<std::uint8_t>(registers.data[7]);
    set_logic(registers, static_cast<std::uint8_t>(registers.data[1]), 0x80U);
    registers.data[0] = 8U;
    set_logic(registers, registers.data[0], 0x80000000U);
    prefetch(host, 0x00083ef6U);
    prefetch(host, 0x00083ef8U);
    child = call_native(host, context, 87U,
        0x00083ef6U, 0x0008410eU, 0x00083efaU);
    if (child.status != TranslationStatus::complete) return child;

    host.begin(0x00083efaU);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | 0x0fU;
    set_logic(registers, 0x0fU, 0x80U);
    prefetch(host, 0x00083efeU);
    prefetch(host, 0x00083f00U);
    registers.data[1] &= 0xffffff00U;
    set_logic(registers, 0U, 0x80U);
    prefetch(host, 0x00083f02U);
    prefetch(host, 0x00083f04U);
    child = call_native(host, context, 87U,
        0x00083f02U, 0x0008410eU, 0x00083f06U);
    if (child.status != TranslationStatus::complete) return child;

    host.begin(0x00083f06U);
    registers.address[1] = 0x00083f32U;
    prefetch(host, 0x00083f0aU);
    prefetch(host, 0x00083f0cU);
    registers.data[2] = (registers.data[2] & 0xffff0000U) | 2U;
    set_logic(registers, 2U, 0x8000U);
    prefetch(host, 0x00083f0eU);
    for (;;) {
        prefetch(host, 0x00083f10U);
        const auto address_byte = read_byte(host, registers.address[1]++);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | address_byte;
        set_logic(registers, address_byte, 0x80U);
        prefetch(host, 0x00083f12U);
        const auto data_byte = read_byte(host, registers.address[1]++);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | data_byte;
        set_logic(registers, data_byte, 0x80U);
        prefetch(host, 0x00083f14U);
        child = call_native(host, context, 87U,
            0x00083f12U, 0x0008410eU, 0x00083f16U);
        if (child.status != TranslationStatus::complete) return child;
        host.begin(0x00083f16U);
        host.clocks(2U); // DBRA: target probe, then target+2 or two exit fetches.
        const auto count = static_cast<std::uint16_t>(registers.data[2]);
        registers.data[2] = (registers.data[2] & 0xffff0000U)
            | static_cast<std::uint16_t>(count - 1U);
        prefetch(host, 0x00083f0eU);
        if (count == 0U) break;
    }

    prefetch(host, 0x00083f1aU);
    prefetch(host, 0x00083f1cU);
    const auto channel = static_cast<std::uint8_t>(registers.data[7]);
    set_compare_byte(registers, channel, 4U);
    prefetch(host, 0x00083f1eU);
    prefetch(host, 0x00083f20U);
    if (channel >= 4U) {
        host.clocks(2U); // BCC.w taken.
        prefetch(host, 0x00083de6U);
        prefetch(host, 0x00083de8U);
        return finish(host, registers);
    }

    host.clocks(4U); // BCC.w not taken.
    prefetch(host, 0x00083f22U);
    prefetch(host, 0x00083f24U);
    registers.data[0] = 0U;
    set_logic(registers, 0U, 0x80000000U);
    auto d7 = static_cast<std::uint16_t>(registers.data[7]);
    auto doubled = static_cast<std::uint16_t>(d7 + d7);
    registers.data[7] = (registers.data[7] & 0xffff0000U) | doubled;
    set_add_word(registers, d7, d7, doubled);
    prefetch(host, 0x00083f26U);
    d7 = doubled;
    doubled = static_cast<std::uint16_t>(d7 + d7);
    registers.data[7] = (registers.data[7] & 0xffff0000U) | doubled;
    set_add_word(registers, d7, d7, doubled);
    prefetch(host, 0x00083f28U);
    prefetch(host, 0x00083f2aU);
    host.clocks(2U); // MOVE.L indexed destination, before extension prefetch.
    prefetch(host, 0x00083f2cU);
    write_long(host, registers.address[6]
        + static_cast<std::int16_t>(doubled) + 0x30U, 0U);
    set_logic(registers, 0U, 0x80000000U);
    prefetch(host, 0x00083f2eU);
    host.clocks(2U); // Second indexed MOVE.L.
    prefetch(host, 0x00083f30U);
    write_long(host, registers.address[6]
        + static_cast<std::int16_t>(doubled) + 0x40U, 0U);
    set_logic(registers, 0U, 0x80000000U);
    prefetch(host, 0x00083f32U);
    return finish(host, registers);
}

} // namespace gain_ground::translated
