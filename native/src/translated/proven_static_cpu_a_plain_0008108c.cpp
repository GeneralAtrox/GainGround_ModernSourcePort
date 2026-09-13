#include "gain_ground/contract_types.h"
#include "gground_functions.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kSpriteRegion = 11U;
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

std::uint16_t read_shared(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(
        kSharedRegion, address & 0x0003ffffU, kWordMask);
}

void write_shared(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value)
{
    host.write_memory_word(
        kSharedRegion, address & 0x0003ffffU, value, kWordMask);
}

void write_sprite_long(ExecutionHost &host, std::uint32_t address,
    std::uint32_t value)
{
    const auto offset = address - 0x00600000U;
    host.write_memory_word(kSpriteRegion, offset,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kSpriteRegion, offset + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

void write_private_word(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value)
{
    host.write_memory_word(kPrivateRegion,
        address & 0x0003ffffU, value, kWordMask);
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
    std::uint32_t completed_pc, std::uint32_t resume,
    std::uint8_t kind)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto pending = host.consume_pending_interrupt(
        0U, 0xffU, completed_pc);
    if (!pending.asserted
        || pending.level <= ((registers.status >> 8U) & 7U))
        return FunctionResult::complete(0U, resume);
    if (pending.level < 3U || pending.level > 5U)
        return {TranslationStatus::contract_violation, 0U, completed_pc};

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

    const bool irq3 = pending.level == 3U;
    const bool irq4 = pending.level == 4U;
    const auto vector = irq3 ? 0x6cU : (irq4 ? 0x70U : 0x74U);
    const auto stub = irq3 ? 0x42U : (irq4 ? 0x48U : 0x4eU);
    const auto target = irq3 ? 0x00080042U
        : (irq4 ? 0x00080048U : 0x0008004eU);
    const auto function_id = irq3 ? 46U : (irq4 ? 47U : 48U);
    prefetch_low(host, vector);
    prefetch_low(host, vector + 2U);
    prefetch(host, stub);
    prefetch(host, stub + 2U);
    registers.program_counter = target;
    const auto child = host.call_function(function_id, 0U, 0xffU, kind,
        completed_pc, target, context);
    if (child.status != TranslationStatus::complete)
        return child;
    if (kind == 0U) {
        const auto trampoline = irq3
            ? cpu_a_irq3_vector_trampoline(context)
            : (irq4 ? cpu_a_irq4_vector_trampoline(context)
                    : cpu_a_irq5_vector_trampoline(context));
        if (trampoline.status != TranslationStatus::complete)
            return trampoline;
    }
    registers.program_counter = resume;
    return FunctionResult::complete(1U, resume);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_0008108c(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    prefetch(host, 0x00081090U);
    registers.address[0] = 0x00600000U;
    prefetch(host, 0x00081092U);
    prefetch(host, 0x00081094U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x1fffU;
    set_move_word_flags(registers, 0x1fffU);
    prefetch(host, 0x00081096U);
    registers.data[1] = 0xffffffffU;
    set_move_long_flags(registers, registers.data[1]);
    prefetch(host, 0x00081098U);
    prefetch(host, 0x0008109aU);

    for (;;) {
        write_sprite_long(host, registers.address[0], registers.data[1]);
        registers.address[0] += 4U;
        set_move_long_flags(registers, registers.data[1]);
        prefetch(host, 0x0008109cU);
        const auto move_interrupt = service_interrupt(
            context, 0x00081098U, 0x0008109aU, 0U);
        if (move_interrupt.status != TranslationStatus::complete)
            return move_interrupt;

        const auto counter = static_cast<std::uint16_t>(
            registers.data[0] - 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU)
            break;
        prefetch(host, 0x00081098U);
        prefetch(host, 0x0008109aU);
        const auto loop_interrupt = service_interrupt(
            context, 0x0008109aU, 0x00081098U, 1U);
        if (loop_interrupt.status != TranslationStatus::complete)
            return loop_interrupt;
    }

    prefetch(host, 0x00081098U);
    prefetch(host, 0x0008109eU);
    prefetch(host, 0x000810a0U);
    prefetch(host, 0x000810a2U);
    registers.address[0] = 0xfff00504U;
    prefetch(host, 0x000810a4U);
    prefetch(host, 0x000810a6U);
    registers.address[1] = 0x0008125aU;

    prefetch(host, 0x000810a8U);
    prefetch(host, 0x000810aaU);
    const auto first = read_shared(host, registers.address[1]);
    registers.address[1] += 2U;
    write_private_word(host, registers.address[0], first);
    registers.address[0] += 2U;
    set_move_word_flags(registers, first);

    prefetch(host, 0x000810acU);
    const auto second = (static_cast<std::uint32_t>(
        read_shared(host, registers.address[1])) << 16U)
        | read_shared(host, registers.address[1] + 2U);
    registers.address[1] += 4U;
    write_private_word(host, registers.address[0],
        static_cast<std::uint16_t>(second >> 16U));
    write_private_word(host, registers.address[0] + 2U,
        static_cast<std::uint16_t>(second));
    registers.address[0] += 4U;
    set_move_long_flags(registers, second);

    prefetch(host, 0x000810aeU);
    const auto third = (static_cast<std::uint32_t>(
        read_shared(host, registers.address[1])) << 16U)
        | read_shared(host, registers.address[1] + 2U);
    registers.address[1] += 4U;
    write_private_word(host, registers.address[0],
        static_cast<std::uint16_t>(third >> 16U));
    write_private_word(host, registers.address[0] + 2U,
        static_cast<std::uint16_t>(third));
    registers.address[0] += 4U;
    set_move_long_flags(registers, third);

    prefetch(host, 0x000810b0U);
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
