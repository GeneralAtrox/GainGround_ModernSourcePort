#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] ByteLocation locate_byte(std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    return {address & 0x0003fffeU,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(
    ExecutionHost &host, std::uint16_t region, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        region, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kPrivateRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void set_logic_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t read_long(
    ExecutionHost &host, std::uint16_t region, std::uint32_t address)
{
    const auto high = host.read_memory_word(region, address & 0x0003ffffU, kWordMask);
    const auto low = host.read_memory_word(region, (address + 2U) & 0x0003ffffU, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_expand_record_pointer_triplet(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto counter_address = (registers.address[5] + 0x26U) & 0x0003ffffU;
    const auto table_offset = host.read_memory_word(
        kPrivateRegion, counter_address, kWordMask);
    registers.address[1] = static_cast<std::uint32_t>(
        0x00fadac0U + static_cast<std::int16_t>(table_offset));

    const auto counter = host.read_memory_word(kPrivateRegion, counter_address, kWordMask);
    host.write_memory_word(kPrivateRegion, counter_address,
        static_cast<std::uint16_t>(counter + 3U), kWordMask);

    registers.address[0] = 0x00000e8aU;
    registers.data[3] = 2U;
    for (;;) {
        const auto source = read_byte(host, kSharedRegion, registers.address[1]++);
        const auto prior = read_byte(host, kPrivateRegion, registers.address[0] + 1U);

        registers.data[1] = (registers.data[1] & 0xffffff00U) | prior;
        write_byte(host, registers.address[0], prior);

        registers.data[2] = (registers.data[2] & 0xffffff00U) | source;
        const auto inverted_source = static_cast<std::uint8_t>(~source);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | inverted_source;
        write_byte(host, registers.address[0] + 1U, inverted_source);

        const auto overlap = static_cast<std::uint8_t>(source & prior);
        registers.data[2] = (registers.data[2] & 0xffffff00U) | overlap;
        set_logic_byte_flags(registers, overlap);
        write_byte(host, registers.address[0] + 3U, overlap);

        const auto inverted_prior = static_cast<std::uint8_t>(~prior);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | inverted_prior;
        const auto exclusive = static_cast<std::uint8_t>(inverted_source & inverted_prior);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | exclusive;
        set_logic_byte_flags(registers, exclusive);
        write_byte(host, registers.address[0] + 2U, exclusive);

        registers.address[0] += 0x200U;
        const auto count = static_cast<std::uint16_t>(registers.data[3]);
        registers.data[3] = (registers.data[3] & 0xffff0000U)
            | static_cast<std::uint16_t>(count - 1U);
        if (count == 0U) break;
    }

    const auto return_address = read_long(host, kPrivateRegion, registers.address[7]);
    registers.address[7] += 4U;
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
