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

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (value == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_add_word(CpuRegisters &registers, std::uint16_t left,
                  std::uint16_t right, std::uint16_t result)
{
    const auto wide = static_cast<std::uint32_t>(left) + right;
    std::uint16_t flags = 0U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right) & (left ^ result)) & 0x8000U) != 0U) flags |= 0x0002U;
    if (wide > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

FunctionResult service_interrupt(FunctionContext &context, std::uint32_t completed_pc,
                                 std::uint32_t resume_pc,
                                 std::uint32_t interrupt_lookahead = 0U,
                                 std::uint8_t call_kind = 1U)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto pending = host.consume_pending_interrupt(0U, 0xffU, completed_pc);
    const auto mask = static_cast<std::uint8_t>((registers.status >> 8U) & 7U);
    if (!pending.asserted || pending.level <= mask)
        return FunctionResult::complete(0U, resume_pc);

    if (interrupt_lookahead != 0U)
        prefetch(host, interrupt_lookahead);

    const auto saved_status = registers.status;
    registers.address[7] -= 4U;
    write_word(host, registers.address[7] + 2U, static_cast<std::uint16_t>(resume_pc));
    registers.address[7] -= 2U;
    write_word(host, registers.address[7], saved_status);
    write_word(host, registers.address[7] + 2U, static_cast<std::uint16_t>(resume_pc >> 16U));
    registers.status = static_cast<std::uint16_t>((saved_status & 0x38ffU) | 0x2000U
        | (static_cast<std::uint16_t>(pending.level) << 8U));

    const auto vector = static_cast<std::uint32_t>(pending.level + 24U) * 4U;
    prefetch_program(host, vector);
    prefetch_program(host, vector + 2U);
    (void)read_long(host, vector - 0x2aU);
    const auto vector_target = pending.level == 3U ? 0x00080042U : 0x00080048U;
    const std::uint32_t function_id = pending.level == 3U ? 46U : 47U;
    registers.program_counter = vector_target;
    const auto result = host.call_function(function_id, 0U, 0xffU, call_kind,
        completed_pc, vector_target, context);
    if (result.status != TranslationStatus::complete)
        return result;
    if (call_kind == 0U) {
        const auto trampoline = pending.level == 3U
            ? cpu_a_irq3_vector_trampoline(context)
            : cpu_a_irq4_vector_trampoline(context);
        if (trampoline.status != TranslationStatus::complete)
            return trampoline;
    }
    registers.program_counter = resume_pc;
    return FunctionResult::complete(1U, resume_pc);
}

FunctionResult return_from_subroutine(ExecutionHost &host, CpuRegisters &registers)
{
    prefetch(host, 0x00080556U);
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_a_asset_copy_source_rectangle_to_primary_workspace(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    prefetch(host, 0x00080526U);
    prefetch(host, 0x00080528U);
    registers.address[0] = 0xffff8402U;
    prefetch(host, 0x0008052aU);
    const auto destination_offset = read_word(host, registers.address[4]);
    registers.address[4] += 2U;
    registers.address[0] += static_cast<std::int16_t>(destination_offset);
    prefetch(host, 0x0008052cU);
    registers.address[1] = read_long(host, registers.address[4]);
    registers.address[4] += 4U;
    prefetch(host, 0x0008052eU);
    const auto width = read_word(host, registers.address[1]);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | width;
    set_logic_word(registers, width);
    registers.data[1] = (registers.data[1] << 16U) | (registers.data[1] >> 16U);
    prefetch(host, 0x00080530U);
    prefetch(host, 0x00080532U);
    const auto width_again = read_word(host, registers.address[1]);
    registers.address[1] += 2U;
    registers.data[1] = (registers.data[1] & 0xffff0000U) | width_again;
    set_logic_word(registers, width_again);
    prefetch(host, 0x00080534U);
    const auto height = read_word(host, registers.address[1]);
    registers.address[1] += 2U;
    registers.data[2] = (registers.data[2] & 0xffff0000U) | height;
    set_logic_word(registers, height);

    bool target_prefetched = false;
    for (;;) {
        if (!target_prefetched)
            prefetch(host, 0x00080536U);
        target_prefetched = false;
        const auto value = read_word(host, registers.address[1]);
        registers.address[1] += 2U;
        registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
        set_logic_word(registers, value);
        const auto bias = static_cast<std::uint16_t>(registers.data[3]);
        const auto adjusted = static_cast<std::uint16_t>(value + bias);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | adjusted;
        set_add_word(registers, value, bias, adjusted);
        prefetch(host, 0x00080538U);
        prefetch(host, 0x0008053aU);
        const auto add_interrupt = service_interrupt(
            context, 0x00080536U, 0x00080538U, 0U, 0U);
        if (add_interrupt.status != TranslationStatus::complete)
            return add_interrupt;
        write_word(host, registers.address[0], adjusted);
        registers.address[0] += 2U;
        set_logic_word(registers, adjusted);
        prefetch(host, 0x0008053cU);
        auto inner = static_cast<std::uint16_t>(registers.data[1] - 1U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | inner;
        if (inner != 0xffffU) {
            prefetch(host, 0x00080534U);
            prefetch(host, 0x00080536U);
            const auto loop_interrupt = service_interrupt(
                context, 0x0008053aU, 0x00080534U);
            if (loop_interrupt.status != TranslationStatus::complete)
                return loop_interrupt;
            target_prefetched = true;
            continue;
        }

        prefetch(host, 0x00080534U);
        prefetch(host, 0x0008053eU);
        const auto loop_exit_interrupt = service_interrupt(
            context, 0x0008053aU, 0x0008053eU, 0x00080540U);
        if (loop_exit_interrupt.status != TranslationStatus::complete)
            return loop_exit_interrupt;
        registers.data[1] = (registers.data[1] << 16U) | (registers.data[1] >> 16U);
        if (loop_exit_interrupt.control == 0U)
            prefetch(host, 0x00080540U);
        registers.data[0] = (registers.data[0] & 0xffff0000U)
            | static_cast<std::uint16_t>(registers.data[1]);
        set_logic_word(registers, static_cast<std::uint16_t>(registers.data[0]));
        prefetch(host, 0x00080542U);
        registers.data[1] = (registers.data[1] << 16U) | (registers.data[1] >> 16U);
        prefetch(host, 0x00080544U);
        registers.data[1] = (registers.data[1] & 0xffff0000U)
            | static_cast<std::uint16_t>(registers.data[0]);
        set_logic_word(registers, static_cast<std::uint16_t>(registers.data[1]));
        prefetch(host, 0x00080546U);
        prefetch(host, 0x00080548U);
        registers.address[0] += 0x80U;
        prefetch(host, 0x0008054aU);
        const auto prior = static_cast<std::uint16_t>(registers.data[0]);
        const auto doubled = static_cast<std::uint16_t>(prior + prior);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | doubled;
        set_add_word(registers, prior, prior, doubled);
        prefetch(host, 0x0008054cU);
        const auto plus_two = static_cast<std::uint16_t>(doubled + 2U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | plus_two;
        set_add_word(registers, doubled, 2U, plus_two);
        prefetch(host, 0x0008054eU);
        registers.address[0] -= static_cast<std::int16_t>(plus_two);
        prefetch(host, 0x00080550U);
        prefetch(host, 0x00080552U);
        const auto outer = static_cast<std::uint16_t>(registers.data[2] - 1U);
        registers.data[2] = (registers.data[2] & 0xffff0000U) | outer;
        if (outer != 0xffffU) {
            prefetch(host, 0x00080534U);
            prefetch(host, 0x00080536U);
            const auto interrupt = service_interrupt(
                context, 0x00080550U, 0x00080534U);
            if (interrupt.status != TranslationStatus::complete)
                return interrupt;
            target_prefetched = true;
            continue;
        }
        prefetch(host, 0x00080534U);
        prefetch(host, 0x00080554U);
        return return_from_subroutine(host, registers);
    }
}

} // namespace gain_ground::translated
