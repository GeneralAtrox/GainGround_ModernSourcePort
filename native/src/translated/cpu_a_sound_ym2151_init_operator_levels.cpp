#include "gain_ground/contract_types.h"
#include "gain_ground/sound_caller_timing.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

void prefetch(SoundCallerTiming &host, std::uint32_t address)
{
    (void)host.read_memory_word(kRegion, address & kAddressMask, kWordMask);
}

void set_logic_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_logic_long_flags(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
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

void push_return(SoundCallerTiming &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    const auto offset = registers.address[7] & kAddressMask;
    host.write_memory_word(kRegion, offset,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, offset + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(
    SoundCallerTiming &host, CpuRegisters &registers)
{
    const auto offset = registers.address[7] & kAddressMask;
    const auto result = (static_cast<std::uint32_t>(
        host.read_memory_word(kRegion, offset, kWordMask)) << 16U)
        | host.read_memory_word(kRegion, offset + 2U, kWordMask);
    registers.address[7] += 4U;
    return result;
}

void call_writer(SoundCallerTiming &host, FunctionContext &context,
    std::uint32_t callsite, std::uint32_t return_address)
{
    auto &registers = context.registers;
    host.clocks(2U); // BSR.w internal prefix, before the stack writes.
    push_return(host, registers, return_address);
    prefetch(host, 0x00084126U);
    prefetch(host, 0x00084128U);
    registers.program_counter = 0x00084126U;
    (void)host.call_function(88U, 0U, 0xffU, 2U,
        callsite, 0x00084126U, context);
}
} // namespace

FunctionResult cpu_a_sound_ym2151_init_operator_levels(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    SoundCallerTiming host(context);
    auto &registers = context.registers;
    host.begin(0x000842c0U);

    prefetch(host, 0x000842c4U);
    prefetch(host, 0x000842c6U);
    registers.data[2] = 3U;
    set_logic_long_flags(registers, registers.data[2]);
    registers.data[0] = 0x60U;
    set_logic_long_flags(registers, registers.data[0]);
    const auto channel = static_cast<std::uint16_t>(registers.data[7]);
    const auto base = static_cast<std::uint16_t>(registers.data[0]);
    const auto initial_register = static_cast<std::uint16_t>(base + channel);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | initial_register;
    set_add_word_flags(registers, base, channel, initial_register);
    prefetch(host, 0x000842c8U);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | 0xffU;
    set_logic_byte_flags(registers, 0xffU);

    for (;;) {
        prefetch(host, 0x000842caU);
        prefetch(host, 0x000842ccU);
        call_writer(host, context, 0x000842caU, 0x000842ceU);
        host.begin(0x000842ceU);

        const auto register_byte = static_cast<std::uint8_t>(registers.data[0]);
        const auto selected = static_cast<std::uint8_t>(register_byte | 0x80U);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | selected;
        set_logic_byte_flags(registers, selected);
        prefetch(host, 0x000842d2U);
        prefetch(host, 0x000842d4U);
        call_writer(host, context, 0x000842d2U, 0x000842d6U);
        host.begin(0x000842d6U);

        const auto restored = static_cast<std::uint8_t>(
            static_cast<std::uint8_t>(registers.data[0]) & 0x7fU);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | restored;
        set_logic_byte_flags(registers, restored);
        prefetch(host, 0x000842daU);
        prefetch(host, 0x000842dcU);
        const auto advanced = static_cast<std::uint8_t>(restored + 8U);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | advanced;
        set_add_byte_flags(registers, restored, 8U, advanced);
        prefetch(host, 0x000842deU);
        prefetch(host, 0x000842e0U);

        host.clocks(2U); // DBRA: target probe plus taken/expired prefetches.
        const auto count = static_cast<std::uint16_t>(registers.data[2]);
        registers.data[2] = (registers.data[2] & 0xffff0000U)
            | static_cast<std::uint16_t>(count - 1U);
        if (count == 0U) break;
    }

    prefetch(host, 0x000842caU);
    prefetch(host, 0x000842e2U);
    prefetch(host, 0x000842e4U);
    const auto return_address = pop_return(host, registers);
    prefetch(host, return_address);
    prefetch(host, return_address + 2U);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
