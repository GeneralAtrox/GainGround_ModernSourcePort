#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & kAddressMask, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address & kAddressMask, value, kWordMask);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto offset = address & kAddressMask & ~1U;
    const bool odd = (address & 1U) != 0U;
    const auto word = host.read_memory_word(
        kRegion, offset, odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)read_word(host, address);
}

void set_logic_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(left) + right > 0xffffU)
        flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_bit_test_flags(CpuRegisters &registers, bool bit_was_set)
{
    if (bit_was_set)
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);
    else
        registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host,
    CpuRegisters &registers)
{
    const auto offset = registers.address[7] & kAddressMask;
    const auto high = host.read_memory_word(kRegion, offset, kWordMask);
    const auto low = host.read_memory_word(kRegion, offset + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_a_sound_channel_accumulate_phase(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto channel = registers.address[3];

    prefetch(host, 0x00083490U);
    auto d1 = read_word(host, channel + 0x12U);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | d1;
    set_logic_word_flags(registers, d1);

    prefetch(host, 0x00083492U);
    prefetch(host, 0x00083494U);
    auto d0 = read_word(host, channel + 0x10U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    set_logic_word_flags(registers, d0);

    prefetch(host, 0x00083496U);
    prefetch(host, 0x00083498U);
    auto result = static_cast<std::uint16_t>(d1 + d0);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | result;
    set_add_word_flags(registers, d1, d0, result);

    prefetch(host, 0x0008349aU);
    prefetch(host, 0x0008349cU);
    result = static_cast<std::uint16_t>(result & 0x7fffU);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | result;
    set_logic_word_flags(registers, result);

    prefetch(host, 0x0008349eU);
    d0 = static_cast<std::uint16_t>(d0 & 0x8000U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    set_logic_word_flags(registers, d0);

    prefetch(host, 0x000834a0U);
    prefetch(host, 0x000834a2U);
    d1 = result;
    result = static_cast<std::uint16_t>(d1 + d0);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | result;
    set_add_word_flags(registers, d1, d0, result);

    prefetch(host, 0x000834a4U);
    write_word(host, channel + 0x10U, result);
    set_logic_word_flags(registers, result);
    prefetch(host, 0x000834a6U);
    prefetch(host, 0x000834a8U);
    prefetch(host, 0x000834aaU);

    const auto flags = read_byte(host, channel);
    set_bit_test_flags(registers, (flags & 0x04U) != 0U);
    prefetch(host, 0x000834acU);

    if ((flags & 0x04U) == 0U) {
        prefetch(host, 0x0008419cU);
        prefetch(host, 0x0008419eU);
        registers.program_counter = 0x0008419cU;
        return host.call_function(93U, 0U, 0xffU, 1U,
            0x000834aaU, 0x0008419cU, context);
    }

    prefetch(host, 0x000834aeU);
    prefetch(host, 0x000834b0U);
    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
