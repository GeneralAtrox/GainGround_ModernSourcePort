#include "gain_ground/contract_types.h"
#include "gground_functions.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kSharedRegion, address & kSharedMask, kWordMask);
}

void prefetch_program(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kSharedRegion, address & kSharedMask, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kSharedRegion, address & kSharedMask, value, kWordMask);
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
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
                                 std::uint32_t completed_pc,
                                 std::uint32_t resume_pc,
                                 std::uint8_t call_kind)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto pending = host.consume_pending_interrupt(0U, 0xffU, completed_pc);
    const auto mask = static_cast<std::uint8_t>((registers.status >> 8U) & 7U);
    if (!pending.asserted || pending.level <= mask)
        return FunctionResult::complete(0U, resume_pc);

    if (completed_pc == 0x000805b2U)
        prefetch(host, 0x000805b2U);
    else if (completed_pc == 0x000805b0U)
        prefetch(host, 0x000805b4U);

    const auto saved_status = registers.status;
    registers.address[7] -= 4U;
    write_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(resume_pc));
    registers.address[7] -= 2U;
    write_word(host, registers.address[7], saved_status);
    write_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(resume_pc >> 16U));
    registers.status = static_cast<std::uint16_t>(
        (saved_status & 0x38ffU) | 0x2000U
        | (static_cast<std::uint16_t>(pending.level) << 8U));

    const bool irq4 = pending.level == 4U;
    const auto vector = irq4 ? 0x00000070U : 0x00000074U;
    const auto trampoline = irq4 ? 0x00080048U : 0x0008004eU;
    const auto vector_stub = irq4 ? 0x00000048U : 0x0000004eU;
    const auto function_id = irq4 ? 47U : 48U;
    prefetch_program(host, vector);
    prefetch_program(host, vector + 2U);
    (void)read_word(host, vector_stub);
    (void)read_word(host, vector_stub + 2U);
    registers.program_counter = trampoline;
    const auto result = host.call_function(function_id, 0U, 0xffU, call_kind,
        completed_pc, trampoline, context);
    if (result.status != TranslationStatus::complete)
        return result;
    if (call_kind == 0U) {
        const auto trampoline_result = irq4
            ? cpu_a_irq4_vector_trampoline(context)
            : cpu_a_irq5_vector_trampoline(context);
        if (trampoline_result.status != TranslationStatus::complete)
            return trampoline_result;
    }
    registers.program_counter = resume_pc;
    return FunctionResult::complete(1U, resume_pc);
}
} // namespace

FunctionResult cpu_a_asset_metatile_workspace_clear(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    prefetch(host, 0x000805a8U);
    prefetch(host, 0x000805aaU);
    registers.address[0] = 0xffff97e2U;
    prefetch(host, 0x000805acU);
    prefetch(host, 0x000805aeU);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x05ffU;
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);
    registers.data[1] = 0U;
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);
    prefetch(host, 0x000805b0U);

    bool loop_word_prefetched = false;
    for (;;) {
        if (!loop_word_prefetched)
            prefetch(host, 0x000805b2U);
        loop_word_prefetched = false;
        write_long(host, registers.address[0], registers.data[1]);
        registers.address[0] += 4U;
        set_move_long_flags(registers, registers.data[1]);
        const auto write_interrupt = service_interrupt(
            context, 0x000805b0U, 0x000805b2U, 0U);
        if (write_interrupt.status != TranslationStatus::complete)
            return write_interrupt;

        if (write_interrupt.control == 0U)
            prefetch(host, 0x000805b4U);
        const auto counter = static_cast<std::uint16_t>(registers.data[0] - 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
        if (counter != 0xffffU) {
            prefetch(host, 0x000805b0U);
            const auto loop_interrupt = service_interrupt(
                context, 0x000805b2U, 0x000805b0U, 1U);
            if (loop_interrupt.status != TranslationStatus::complete)
                return loop_interrupt;
            loop_word_prefetched = loop_interrupt.control != 0U;
            continue;
        }
        prefetch(host, 0x000805b0U);
        prefetch(host, 0x000805b6U);
        prefetch(host, 0x000805b8U);
        const auto target = (static_cast<std::uint32_t>(read_word(host, registers.address[7])) << 16U)
            | read_word(host, registers.address[7] + 2U);
        registers.address[7] += 4U;
        prefetch(host, target);
        prefetch(host, target + 2U);
        registers.program_counter = target;
        return FunctionResult::complete(1U, target);
    }
}

} // namespace gain_ground::translated
