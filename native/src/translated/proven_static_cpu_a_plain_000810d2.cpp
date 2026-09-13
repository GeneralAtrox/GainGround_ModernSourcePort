#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWindowRegion = 7U;
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

void write_window_long(ExecutionHost &host, std::uint32_t address,
    std::uint32_t value)
{
    const auto offset = address - 0x0020c000U;
    host.write_memory_word(kWindowRegion, offset,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kWindowRegion, offset + 2U,
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

FunctionResult service_interrupt(FunctionContext &context,
    std::uint32_t resume)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto pending = host.consume_pending_interrupt(
        0U, 0xffU, 0x000810e0U);
    if (!pending.asserted
        || pending.level <= ((registers.status >> 8U) & 7U))
        return FunctionResult::complete(0U, resume);
    if (pending.level != 4U && pending.level != 5U)
        return {TranslationStatus::contract_violation, 0U, 0x000810e0U};

    prefetch(host, 0x000810e0U);
    const auto saved = registers.status;
    registers.address[7] -= 4U;
    write_shared(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(resume));
    registers.address[7] -= 2U;
    write_shared(host, registers.address[7], saved);
    write_shared(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(resume >> 16U));
    registers.status = static_cast<std::uint16_t>((saved & 0x38ffU)
        | 0x2000U | (static_cast<std::uint16_t>(pending.level) << 8U));

    const bool irq4 = pending.level == 4U;
    const auto vector = irq4 ? 0x70U : 0x74U;
    const auto stub = irq4 ? 0x48U : 0x4eU;
    const auto target = irq4 ? 0x00080048U : 0x0008004eU;
    const auto function_id = irq4 ? 47U : 48U;
    prefetch_low(host, vector);
    prefetch_low(host, vector + 2U);
    prefetch(host, stub);
    prefetch(host, stub + 2U);
    registers.program_counter = target;
    const auto child = host.call_function(function_id, 0U, 0xffU, 1U,
        0x000810e0U, target, context);
    if (child.status != TranslationStatus::complete)
        return child;
    registers.program_counter = resume;
    return FunctionResult::complete(1U, resume);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_000810d2(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    prefetch(host, 0x000810d6U);
    registers.address[0] = 0x0020c000U;
    prefetch(host, 0x000810d8U);
    prefetch(host, 0x000810daU);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x07ffU;
    set_move_word_flags(registers, 0x07ffU);
    prefetch(host, 0x000810dcU);
    registers.data[1] = 0U;
    set_move_long_flags(registers, 0U);
    prefetch(host, 0x000810deU);

    bool dbra_prefetched = false;
    for (;;) {
        if (!dbra_prefetched)
            prefetch(host, 0x000810e0U);
        dbra_prefetched = false;
        write_window_long(host, registers.address[0], registers.data[1]);
        registers.address[0] += 4U;
        set_move_long_flags(registers, registers.data[1]);
        prefetch(host, 0x000810e2U);

        const auto counter = static_cast<std::uint16_t>(
            registers.data[0] - 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
        prefetch(host, 0x000810deU);
        if (counter == 0xffffU)
            break;

        const auto interrupt = service_interrupt(context, 0x000810deU);
        if (interrupt.status != TranslationStatus::complete)
            return interrupt;
        dbra_prefetched = interrupt.control != 0U;
    }

    prefetch(host, 0x000810e4U);
    prefetch(host, 0x000810e6U);
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
