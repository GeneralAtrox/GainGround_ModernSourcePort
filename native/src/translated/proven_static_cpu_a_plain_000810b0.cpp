#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kScrollRegion = 6U;
constexpr std::uint16_t kWordMask = 0xffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(
        kSharedRegion, address & 0x0003ffffU, kWordMask);
}

void prefetch_low(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

void write_shared(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value)
{
    host.write_memory_word(
        kSharedRegion, address & 0x0003ffffU, value, kWordMask);
}

void write_scroll_long(ExecutionHost &host, std::uint32_t address,
    std::uint32_t value)
{
    const auto offset = address - 0x00208000U;
    host.write_memory_word(kScrollRegion, offset,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kScrollRegion, offset + 2U,
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

FunctionResult service_irq3(FunctionContext &context,
    std::uint32_t resume, std::uint32_t lookahead)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto pending = host.consume_pending_interrupt(
        0U, 0xffU, 0x000810beU);
    if (!pending.asserted || pending.level <= ((registers.status >> 8U) & 7U))
        return FunctionResult::complete(0U, resume);
    if (pending.level != 3U)
        return {TranslationStatus::contract_violation, 0U, 0x000810beU};

    prefetch(host, lookahead);
    const auto saved = registers.status;
    registers.address[7] -= 4U;
    write_shared(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(resume));
    registers.address[7] -= 2U;
    write_shared(host, registers.address[7], saved);
    write_shared(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(resume >> 16U));
    registers.status = static_cast<std::uint16_t>((saved & 0x38ffU) | 0x2300U);
    prefetch_low(host, 0x0000006cU);
    prefetch_low(host, 0x0000006eU);
    prefetch(host, 0x00000042U);
    prefetch(host, 0x00000044U);
    registers.program_counter = 0x00080042U;
    const auto child = host.call_function(46U, 0U, 0xffU, 1U,
        0x000810beU, 0x00080042U, context);
    if (child.status != TranslationStatus::complete)
        return child;
    registers.program_counter = resume;
    return FunctionResult::complete(1U, resume);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_000810b0(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    prefetch(host, 0x000810b4U);
    registers.address[0] = 0x00208000U;
    prefetch(host, 0x000810b6U);
    prefetch(host, 0x000810b8U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x03ffU;
    set_move_word_flags(registers, 0x03ffU);
    prefetch(host, 0x000810baU);
    registers.data[1] = 0U;
    set_move_long_flags(registers, 0U);
    prefetch(host, 0x000810bcU);

    bool dbra_prefetched = false;
    for (;;) {
        if (!dbra_prefetched)
            prefetch(host, 0x000810beU);
        dbra_prefetched = false;
        write_scroll_long(host, registers.address[0], registers.data[1]);
        registers.address[0] += 4U;
        set_move_long_flags(registers, registers.data[1]);
        prefetch(host, 0x000810c0U);

        const auto counter = static_cast<std::uint16_t>(registers.data[0] - 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU)
            break;

        prefetch(host, 0x000810bcU);
        const auto interrupt = service_irq3(
            context, 0x000810bcU, 0x000810beU);
        if (interrupt.status != TranslationStatus::complete)
            return interrupt;
        dbra_prefetched = interrupt.control != 0U;
    }

    prefetch(host, 0x000810bcU);
    prefetch(host, 0x000810c2U);
    prefetch(host, 0x000810c4U);
    prefetch(host, 0x000810c6U);
    registers.address[0] = 0x0020a000U;

    prefetch(host, 0x000810c8U);
    for (const auto next_pc : {
             0x000810caU, 0x000810ccU, 0x000810ceU}) {
        prefetch(host, next_pc);
        write_scroll_long(host, registers.address[0], registers.data[1]);
        registers.address[0] += 4U;
        set_move_long_flags(registers, registers.data[1]);
    }
    prefetch(host, 0x000810d0U);
    write_scroll_long(host, registers.address[0], registers.data[1]);
    set_move_long_flags(registers, registers.data[1]);

    prefetch(host, 0x000810d2U);
    const auto high = host.read_memory_word(kSharedRegion,
        registers.address[7] & 0x0003ffffU, kWordMask);
    const auto low = host.read_memory_word(kSharedRegion,
        (registers.address[7] + 2U) & 0x0003ffffU, kWordMask);
    registers.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
