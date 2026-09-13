#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kRegion, address & kAddressMask, kWordMask);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto offset = address & kAddressMask;
    const auto mask = static_cast<std::uint16_t>(
        (offset & 1U) == 0U ? 0xff00U : 0x00ffU);
    const auto shift = (offset & 1U) == 0U ? 8U : 0U;
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, offset & ~1U, mask) >> shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto offset = address & kAddressMask;
    const auto mask = static_cast<std::uint16_t>(
        (offset & 1U) == 0U ? 0xff00U : 0x00ffU);
    const auto shift = (offset & 1U) == 0U ? 8U : 0U;
    host.write_memory_word(kRegion, offset & ~1U,
        static_cast<std::uint16_t>(value) << shift, mask);
}

void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign_mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign_mask) != 0U) flags |= 0x0008U;
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

void set_sub_word_flags(CpuRegisters &registers,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (right > left) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    const auto offset = registers.address[7] & kAddressMask;
    host.write_memory_word(kRegion, offset,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, offset + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto offset = registers.address[7] & kAddressMask;
    const auto result = (static_cast<std::uint32_t>(
        host.read_memory_word(kRegion, offset, kWordMask)) << 16U)
        | host.read_memory_word(kRegion, offset + 2U, kWordMask);
    registers.address[7] += 4U;
    return result;
}

void call_child(FunctionContext &context, std::uint32_t function_id,
    std::uint32_t callsite, std::uint32_t target, std::uint32_t return_address)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    push_return(host, registers, return_address);
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    (void)host.call_function(function_id, 0U, 0xffU, 2U,
        callsite, target, context);
}
} // namespace

FunctionResult cpu_a_sound_ym2151_write_operator_channel_block(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    prefetch(host, 0x00084168U);
    prefetch(host, 0x0008416aU);
    registers.data[0] = 0U;
    set_logic_flags(registers, 0U, 0x80000000U);
    const auto block_index = read_byte(host, registers.address[3] + 0x16U);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | block_index;
    set_logic_flags(registers, block_index, 0x80U);
    prefetch(host, 0x0008416cU);
    prefetch(host, 0x0008416eU);
    const auto old_index = static_cast<std::uint16_t>(registers.data[0]);
    const auto decremented = static_cast<std::uint16_t>(old_index - 1U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | decremented;
    set_sub_word_flags(registers, old_index, 1U, decremented);
    const auto doubled_once = static_cast<std::uint16_t>(decremented + decremented);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | doubled_once;
    set_add_word_flags(registers, decremented, decremented, doubled_once);
    prefetch(host, 0x00084170U);
    prefetch(host, 0x00084172U);
    registers.data[1] = 6U;
    set_logic_flags(registers, registers.data[1], 0x80000000U);
    call_child(context, 89U, 0x00084170U, 0x0008413eU, 0x00084172U);

    registers.data[0] = 0x20U;
    set_logic_flags(registers, registers.data[0], 0x80000000U);
    const auto channel = static_cast<std::uint8_t>(registers.data[7]);
    const auto first_register = static_cast<std::uint8_t>(0x20U + channel);
    registers.data[0] = first_register;
    set_add_byte_flags(registers, 0x20U, channel, first_register);
    prefetch(host, 0x00084176U);
    prefetch(host, 0x00084178U);
    const auto channel_value = read_byte(host, registers.address[0]++);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | channel_value;
    set_logic_flags(registers, channel_value, 0x80U);
    prefetch(host, 0x0008417aU);
    prefetch(host, 0x0008417cU);
    write_byte(host, registers.address[3] + 0x17U, channel_value);
    set_logic_flags(registers, channel_value, 0x80U);
    prefetch(host, 0x0008417eU);
    call_child(context, 88U, 0x0008417cU, 0x00084126U, 0x0008417eU);

    prefetch(host, 0x00084182U);
    registers.data[3] = 0U;
    set_logic_flags(registers, 0U, 0x80000000U);
    const auto old_register = static_cast<std::uint8_t>(registers.data[0]);
    const auto operator_register = static_cast<std::uint8_t>(old_register + 0x20U);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | operator_register;
    set_add_byte_flags(registers, old_register, 0x20U, operator_register);
    prefetch(host, 0x00084184U);
    registers.data[2] = 0x17U;
    set_logic_flags(registers, registers.data[2], 0x80000000U);

    prefetch(host, 0x00084186U);
    for (;;) {
        prefetch(host, 0x00084188U);
        const auto value = read_byte(host, registers.address[0]++);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | value;
        set_logic_flags(registers, value, 0x80U);
        prefetch(host, 0x0008418aU);
        prefetch(host, 0x0008418cU);
        write_byte(host, registers.address[3] + 0x18U
            + static_cast<std::int16_t>(registers.data[3]), value);
        set_logic_flags(registers, value, 0x80U);
        prefetch(host, 0x0008418eU);
        call_child(context, 88U, 0x0008418cU, 0x00084126U, 0x0008418eU);

        const auto old_offset = static_cast<std::uint16_t>(registers.data[3]);
        const auto next_offset = static_cast<std::uint16_t>(old_offset + 1U);
        registers.data[3] = (registers.data[3] & 0xffff0000U) | next_offset;
        set_add_word_flags(registers, old_offset, 1U, next_offset);
        prefetch(host, 0x00084192U);
        const auto old_address = static_cast<std::uint8_t>(registers.data[0]);
        const auto next_address = static_cast<std::uint8_t>(old_address + 8U);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | next_address;
        set_add_byte_flags(registers, old_address, 8U, next_address);
        prefetch(host, 0x00084194U);
        prefetch(host, 0x00084196U);
        prefetch(host, 0x00084198U);
        const auto count = static_cast<std::uint16_t>(registers.data[2]);
        registers.data[2] = (registers.data[2] & 0xffff0000U)
            | static_cast<std::uint16_t>(count - 1U);
        prefetch(host, 0x00084186U);
        if (count == 0U) break;
    }

    prefetch(host, 0x0008419aU);
    prefetch(host, 0x0008419cU);
    const auto return_address = pop_return(host, registers);
    prefetch(host, return_address);
    prefetch(host, return_address + 2U);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
