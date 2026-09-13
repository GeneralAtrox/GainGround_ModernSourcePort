#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kPaletteRegion = 9U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(
        kSharedRegion, address & kSharedMask, kWordMask);
}

std::uint16_t read_shared(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(
        kSharedRegion, address & kSharedMask, kWordMask);
}

void write_palette_long(ExecutionHost &host, std::uint32_t address,
    std::uint32_t value)
{
    const auto offset = address - 0x00400000U;
    host.write_memory_word(kPaletteRegion, offset,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPaletteRegion, offset + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_move_long_flags(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_00082410(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    prefetch(host, 0x00082414U);
    registers.data[2] = (registers.data[2] & 0xffff0000U) | 0x0007U;
    set_move_word_flags(registers, 0x0007U);

    prefetch(host, 0x00082416U);
    const auto extended = static_cast<std::uint32_t>(static_cast<std::int32_t>(
        static_cast<std::int16_t>(registers.data[0] & 0xffffU)));
    registers.data[0] = extended;
    set_move_long_flags(registers, extended);

    prefetch(host, 0x00082418U);
    registers.data[1] = 0U;
    set_move_long_flags(registers, 0U);
    prefetch(host, 0x0008241aU);

    for (;;) {
        write_palette_long(host, registers.address[0], registers.data[0]);
        registers.address[0] += 4U;
        set_move_long_flags(registers, registers.data[0]);

        for (const auto next_pc : {
                 0x0008241cU, 0x0008241eU, 0x00082420U,
                 0x00082422U, 0x00082424U, 0x00082426U,
                 0x00082428U}) {
            prefetch(host, next_pc);
            write_palette_long(host, registers.address[0], registers.data[1]);
            registers.address[0] += 4U;
            set_move_long_flags(registers, registers.data[1]);
        }

        prefetch(host, 0x0008242aU);
        const auto counter = static_cast<std::uint16_t>(
            registers.data[2] - 1U);
        registers.data[2] =
            (registers.data[2] & 0xffff0000U) | counter;
        prefetch(host, 0x00082418U);
        if (counter != 0xffffU) {
            prefetch(host, 0x0008241aU);
            continue;
        }
        prefetch(host, 0x0008242cU);
        break;
    }

    prefetch(host, 0x0008242eU);
    prefetch(host, 0x00082430U);
    registers.address[0] += 0x00000300U;
    prefetch(host, 0x00082432U);

    const auto high = read_shared(host, registers.address[7]);
    const auto low = read_shared(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
