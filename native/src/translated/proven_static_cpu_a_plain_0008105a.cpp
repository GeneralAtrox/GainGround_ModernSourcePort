#include "gain_ground/contract_types.h"
#include "gground_functions.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kScrollRegion = 6U;
constexpr std::uint16_t kWindowRegion = 7U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kSharedRegion, address & kSharedMask, kWordMask);
}

void prefetch_low(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

std::uint16_t read_shared(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kSharedRegion, address & kSharedMask, kWordMask);
}

void write_shared(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kSharedRegion, address & kSharedMask, value, kWordMask);
}

void write_video_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    const bool scroll = address >= 0x00208000U;
    const auto region = scroll ? kScrollRegion : kTileRegion;
    const auto base = scroll ? 0x00208000U : 0x00200000U;
    const auto offset = address - base;
    host.write_memory_word(region, offset,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(region, offset + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

void write_private_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    const auto offset = address & kSharedMask;
    host.write_memory_word(kPrivateRegion, offset,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, offset + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

void write_window_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
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
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_move_long_flags(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

FunctionResult service_interrupt(FunctionContext &context,
    std::uint32_t completed_pc, std::uint32_t resume, std::uint8_t kind)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto pending = host.consume_pending_interrupt(0U, 0xffU, completed_pc);
    if (!pending.asserted || pending.level <= ((registers.status >> 8U) & 7U))
        return FunctionResult::complete(0U, resume);
    if (pending.level < 3U || pending.level > 5U)
        return {TranslationStatus::contract_violation, 0U, completed_pc};
    const auto saved = registers.status;
    registers.address[7] -= 4U;
    write_shared(host, registers.address[7] + 2U, static_cast<std::uint16_t>(resume));
    registers.address[7] -= 2U;
    write_shared(host, registers.address[7], saved);
    write_shared(host, registers.address[7] + 2U, static_cast<std::uint16_t>(resume >> 16U));
    registers.status = static_cast<std::uint16_t>((saved & 0x38ffU) | 0x2000U
        | (static_cast<std::uint16_t>(pending.level) << 8U));

    const bool irq3 = pending.level == 3U;
    const bool irq4 = pending.level == 4U;
    const auto vector = irq3 ? 0x6cU : (irq4 ? 0x70U : 0x74U);
    const auto stub = irq3 ? 0x42U : (irq4 ? 0x48U : 0x4eU);
    const auto target = irq3 ? 0x00080042U : (irq4 ? 0x00080048U : 0x0008004eU);
    const auto function_id = irq3 ? 46U : (irq4 ? 47U : 48U);
    prefetch_low(host, vector);
    prefetch_low(host, vector + 2U);
    prefetch(host, stub);
    prefetch(host, stub + 2U);
    registers.program_counter = target;
    const auto child = host.call_function(function_id, 0U, 0xffU, kind,
        completed_pc, target, context);
    if (child.status != TranslationStatus::complete) return child;
    if (kind == 0U) {
        const auto trampoline = irq3 ? cpu_a_irq3_vector_trampoline(context)
            : (irq4 ? cpu_a_irq4_vector_trampoline(context)
                    : cpu_a_irq5_vector_trampoline(context));
        if (trampoline.status != TranslationStatus::complete) return trampoline;
    }
    registers.program_counter = resume;
    return FunctionResult::complete(1U, resume);
}

template <typename WriteLong>
FunctionResult clear_loop(FunctionContext &context, WriteLong write_long,
    std::uint32_t move_pc, std::uint32_t dbra_pc, std::uint32_t fallthrough_pc)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    for (;;) {
        write_long(host, registers.address[0], registers.data[1]);
        registers.address[0] += 4U;
        set_move_long_flags(registers, registers.data[1]);
        prefetch(host, dbra_pc + 2U);
        const auto move_interrupt = service_interrupt(context, move_pc, dbra_pc, 0U);
        if (move_interrupt.status != TranslationStatus::complete) return move_interrupt;

        const auto counter = static_cast<std::uint16_t>(registers.data[0] - 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
        prefetch(host, move_pc);
        prefetch(host, dbra_pc);
        const auto loop_interrupt = service_interrupt(context, dbra_pc, move_pc, 1U);
        if (loop_interrupt.status != TranslationStatus::complete) return loop_interrupt;
    }
    prefetch(host, move_pc);
    prefetch(host, fallthrough_pc);
    return FunctionResult::complete(0U, fallthrough_pc);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_0008105a(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    prefetch(host, 0x0008105eU);
    registers.address[0] = 0x00200000U;
    prefetch(host, 0x00081060U);
    prefetch(host, 0x00081062U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x23ffU;
    set_move_word_flags(registers, 0x23ffU);
    prefetch(host, 0x00081064U);
    registers.data[1] = 0U;
    set_move_long_flags(registers, 0U);
    prefetch(host, 0x00081066U);
    prefetch(host, 0x00081068U);

    const auto first = clear_loop(context, write_video_long,
        0x00081066U, 0x00081068U, 0x0008106cU);
    if (first.status != TranslationStatus::complete) return first;

    prefetch(host, 0x0008106eU);
    prefetch(host, 0x00081070U);
    registers.address[0] = 0xfff0050eU;
    prefetch(host, 0x00081072U);
    prefetch(host, 0x00081074U);
    for (const auto next_pc : {0x00081076U, 0x00081078U, 0x0008107aU, 0x0008107cU}) {
        write_private_long(host, registers.address[0], registers.data[1]);
        registers.address[0] += 4U;
        set_move_long_flags(registers, registers.data[1]);
        prefetch(host, next_pc);
    }

    prefetch(host, 0x0008107eU);
    registers.address[0] = 0x0020c000U;
    prefetch(host, 0x00081080U);
    prefetch(host, 0x00081082U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x07ffU;
    set_move_word_flags(registers, 0x07ffU);
    prefetch(host, 0x00081084U);
    prefetch(host, 0x00081086U);

    const auto second = clear_loop(context, write_window_long,
        0x00081084U, 0x00081086U, 0x0008108aU);
    if (second.status != TranslationStatus::complete) return second;

    prefetch(host, 0x0008108cU);
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
