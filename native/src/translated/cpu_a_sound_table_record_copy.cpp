#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kOffsetMask = 0x0003ffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] ByteLocation locate_byte(std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    return {address & 0x0003fffeU,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kSharedRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kSharedRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kSharedRegion, address & kOffsetMask, kWordMask);
    const auto low = host.read_memory_word(kSharedRegion, (address + 2U) & kOffsetMask, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kSharedRegion, address & kOffsetMask,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kSharedRegion, (address + 2U) & kOffsetMask,
        static_cast<std::uint16_t>(value), kWordMask);
}

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kSharedRegion, address & kOffsetMask, kWordMask);
}

void set_logic(CpuRegisters &registers, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_sub_byte(CpuRegisters &registers, std::uint8_t left, std::uint8_t right, std::uint8_t result)
{
    std::uint16_t flags{};
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if (right > left) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_a_sound_table_record_copy(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[1] = registers.address[6];

    prefetch(host, 0x00083600U);
    const auto skipped = host.read_memory_word(
        kSharedRegion, registers.address[0] & kOffsetMask, kWordMask);
    registers.address[0] += 2U;
    set_logic(registers, skipped, 0x8000U);

    prefetch(host, 0x00083602U);
    registers.data[1] = 0U;
    prefetch(host, 0x00083604U);
    prefetch(host, 0x00083606U);
    registers.data[1] = (registers.data[1] & 0xffffff00U)
        | static_cast<std::uint8_t>(registers.data[7]);
    set_logic(registers, static_cast<std::uint8_t>(registers.data[1]), 0x80U);
    prefetch(host, 0x00083608U);
    auto d1w = static_cast<std::uint16_t>(registers.data[1] | 0x0010U);
    registers.data[1] = d1w;
    set_logic(registers, d1w, 0x8000U);
    prefetch(host, 0x0008360aU);
    const auto doubled = static_cast<std::uint16_t>(d1w + d1w);
    registers.data[1] = doubled;

    prefetch(host, 0x0008360cU);
    registers.address[2] = 0x000839a6U;
    prefetch(host, 0x0008360eU);
    prefetch(host, 0x00083610U);
    prefetch(host, 0x00083612U);
    const auto displacement = host.read_memory_word(kSharedRegion,
        (registers.address[2] + static_cast<std::int16_t>(doubled)) & kOffsetMask, kWordMask);
    registers.address[1] = static_cast<std::uint32_t>(
        registers.address[1] + static_cast<std::int16_t>(displacement));

    prefetch(host, 0x00083614U);
    const auto first = host.read_memory_word(kSharedRegion,
        registers.address[0] & kOffsetMask, kWordMask);
    registers.address[0] += 2U;
    host.write_memory_word(kSharedRegion, registers.address[1] & kOffsetMask, first, kWordMask);
    registers.address[1] += 2U;
    set_logic(registers, first, 0x8000U);

    prefetch(host, 0x00083616U);
    const auto ignored_byte = read_byte(host, registers.address[0]++);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | ignored_byte;
    set_logic(registers, ignored_byte, 0x80U);
    prefetch(host, 0x00083618U);
    prefetch(host, 0x0008361aU);
    const auto tagged = static_cast<std::uint8_t>(registers.data[7] | 0x90U);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | tagged;
    set_logic(registers, tagged, 0x80U);
    prefetch(host, 0x0008361cU);
    prefetch(host, 0x0008361eU);
    write_byte(host, registers.address[1]++, tagged);

    prefetch(host, 0x00083620U);
    const auto copied_byte = read_byte(host, registers.address[0]++);
    write_byte(host, registers.address[1]++, copied_byte);
    set_logic(registers, copied_byte, 0x80U);

    registers.data[1] = registers.address[0];
    prefetch(host, 0x00083622U);
    prefetch(host, 0x00083624U);
    const auto relative = read_long(host, registers.address[0]);
    registers.address[0] += 4U;
    registers.data[1] += relative;
    registers.data[1] -= registers.address[5];
    prefetch(host, 0x00083626U);
    prefetch(host, 0x00083628U);
    write_long(host, registers.address[1], registers.data[1]);
    registers.address[1] += 4U;
    set_logic(registers, registers.data[1], 0x80000000U);

    prefetch(host, 0x0008362aU);
    const auto copied_long = read_long(host, registers.address[0]);
    registers.address[0] += 4U;
    write_long(host, registers.address[1], copied_long);
    registers.address[1] += 4U;
    set_logic(registers, copied_long, 0x80000000U);

    prefetch(host, 0x0008362cU);
    prefetch(host, 0x0008362eU);
    host.write_memory_word(kSharedRegion, registers.address[1] & kOffsetMask, 0x5001U, kWordMask);
    registers.address[1] += 2U;
    set_logic(registers, 0x5001U, 0x8000U);

    prefetch(host, 0x00083630U);
    prefetch(host, 0x00083632U);
    const auto count = read_byte(host, registers.address[0] - 9U);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | count;
    const auto decremented = static_cast<std::uint8_t>(count - 1U);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | decremented;
    set_sub_byte(registers, count, 1U, decremented);
    prefetch(host, 0x00083634U);
    prefetch(host, 0x00083636U);
    write_byte(host, registers.address[1]++, decremented);

    prefetch(host, 0x00083638U);
    registers.data[1] = 0U;
    prefetch(host, 0x0008363aU);
    write_byte(host, registers.address[1]++, 0U);
    set_logic(registers, 0U, 0x80U);
    registers.data[2] = 15U;
    do {
        prefetch(host, 0x0008363cU);
        prefetch(host, 0x0008363eU);
        write_long(host, registers.address[1], 0U);
        registers.address[1] += 4U;
        prefetch(host, 0x00083640U);
        const auto before = static_cast<std::uint16_t>(registers.data[2]);
        registers.data[2] = (registers.data[2] & 0xffff0000U)
            | static_cast<std::uint16_t>(before - 1U);
        if (before == 0U) break;
    } while (true);

    prefetch(host, 0x0008363cU);
    prefetch(host, 0x00083642U);
    prefetch(host, 0x00083644U);
    const auto return_address = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    (void)host.read_memory_word(kSharedRegion, return_address & kOffsetMask, kWordMask);
    (void)host.read_memory_word(kSharedRegion, (return_address + 2U) & kOffsetMask, kWordMask);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
