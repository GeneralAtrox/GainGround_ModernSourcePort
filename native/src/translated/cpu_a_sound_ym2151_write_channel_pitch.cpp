#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & kAddressMask, kWordMask);
}

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)read_word(host, address);
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto offset = address & kAddressMask & ~1U;
    const auto mask = static_cast<std::uint16_t>((address & 1U) ? 0x00ffU : 0xff00U);
    const auto shift = (address & 1U) ? 0U : 8U;
    return static_cast<std::uint8_t>(host.read_memory_word(kRegion, offset, mask) >> shift);
}

void set_logic_flags(CpuRegisters &registers, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_lsr_word_flags(CpuRegisters &registers, std::uint16_t original,
    std::uint16_t result)
{
    const bool carry = (original & 0x0080U) != 0U;
    std::uint16_t flags = 0U;
    if (carry) flags |= 0x0011U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_add_byte_flags(CpuRegisters &registers, std::uint8_t left,
    std::uint8_t right, std::uint8_t result)
{
    const auto wide = static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(left) + static_cast<std::uint16_t>(right));
    const bool carry = wide > 0xffU;
    const bool overflow = ((~(left ^ right) & (left ^ result)) & 0x80U) != 0U;
    std::uint16_t flags = 0U;
    if (carry) flags |= 0x0011U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_sub_byte_flags(CpuRegisters &registers, std::uint8_t destination,
    std::uint8_t source, std::uint8_t result)
{
    const bool borrow = source > destination;
    const bool overflow = (((destination ^ source) & (destination ^ result)) & 0x80U) != 0U;
    std::uint16_t flags = 0U;
    if (borrow) flags |= 0x0011U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t address)
{
    registers.address[7] -= 4U;
    const auto offset = registers.address[7] & kAddressMask;
    host.write_memory_word(kRegion, offset,
        static_cast<std::uint16_t>(address >> 16U), kWordMask);
    host.write_memory_word(kRegion, offset + 2U,
        static_cast<std::uint16_t>(address), kWordMask);
}
} // namespace

FunctionResult cpu_a_sound_ym2151_write_channel_pitch(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    registers.data[2] = (registers.data[2] & 0xffff0000U)
        | static_cast<std::uint16_t>(registers.data[1]);
    set_logic_flags(registers, static_cast<std::uint16_t>(registers.data[2]), 0x8000U);
    prefetch(host, 0x000841a0U);

    const auto original_pitch = static_cast<std::uint16_t>(registers.data[2]);
    const auto octave = static_cast<std::uint16_t>(original_pitch >> 8U);
    registers.data[2] = (registers.data[2] & 0xffff0000U) | octave;
    set_lsr_word_flags(registers, original_pitch, octave);
    prefetch(host, 0x000841a2U);

    registers.data[0] = 0x30U;
    set_logic_flags(registers, registers.data[0], 0x80000000U);
    prefetch(host, 0x000841a4U);

    const auto register_byte = static_cast<std::uint8_t>(registers.data[0]);
    const auto channel = static_cast<std::uint8_t>(registers.data[7]);
    const auto channel_register = static_cast<std::uint8_t>(register_byte + channel);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | channel_register;
    set_add_byte_flags(registers, register_byte, channel, channel_register);
    prefetch(host, 0x000841a6U);

    push_return(host, registers, 0x000841a6U);
    prefetch(host, 0x00084126U);
    prefetch(host, 0x00084128U);
    registers.program_counter = 0x00084126U;
    (void)host.call_function(88U, 0U, 0xffU, 2U,
        0x000841a4U, 0x00084126U, context);

    const auto pitch_index = static_cast<std::uint16_t>(registers.data[2]) & 0x00ffU;
    registers.data[2] = (registers.data[2] & 0xffff0000U) | pitch_index;
    set_logic_flags(registers, pitch_index, 0x8000U);
    prefetch(host, 0x000841aaU);
    prefetch(host, 0x000841acU);

    registers.address[0] = 0x000841baU;
    prefetch(host, 0x000841aeU);
    prefetch(host, 0x000841b0U);
    prefetch(host, 0x000841b2U);

    const auto table_address = registers.address[0] + static_cast<std::int16_t>(pitch_index);
    const auto pitch_byte = read_byte(host, table_address);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | pitch_byte;
    set_logic_flags(registers, pitch_byte, 0x80U);
    prefetch(host, 0x000841b4U);

    const auto old_register = static_cast<std::uint8_t>(registers.data[0]);
    const auto data_register = static_cast<std::uint8_t>(old_register - 8U);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | data_register;
    set_sub_byte_flags(registers, old_register, 8U, data_register);
    prefetch(host, 0x000841b6U);
    prefetch(host, 0x000841b8U);
    prefetch(host, 0x00084126U);
    prefetch(host, 0x00084128U);
    registers.program_counter = 0x00084126U;
    (void)host.call_function(88U, 0U, 0xffU, 1U,
        0x000841b6U, 0x00084126U, context);
    return FunctionResult::complete(3U, 0x00084126U);
}

} // namespace gain_ground::translated
