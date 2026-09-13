#include "gain_ground/contract_types.h"
#include "gground_functions.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kScrollRegion = 6U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

void write_video_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    const auto region = address >= 0x00208000U ? kScrollRegion : kTileRegion;
    const auto base = address >= 0x00208000U ? 0x00208000U : 0x00200000U;
    host.write_memory_word(region, address - base,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(region, address - base + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint16_t read_shared(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kSharedRegion, address & kSharedMask, kWordMask);
}

void write_shared(ExecutionHost &host,
    std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kSharedRegion, address & kSharedMask, value, kWordMask);
}

void set_move_long_flags(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] bool service_irq4(FunctionContext &context,
    std::uint32_t completed_pc, std::uint32_t next_pc, std::uint8_t call_kind)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto pending = host.consume_pending_interrupt(
        0U, 0xffU, completed_pc);
    const auto mask = static_cast<std::uint8_t>((registers.status >> 8U) & 7U);
    if (!pending.asserted || pending.level <= mask)
        return false;

    const auto saved_status = registers.status;
    registers.address[7] -= 4U;
    write_shared(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(next_pc));
    registers.address[7] -= 2U;
    write_shared(host, registers.address[7], saved_status);
    write_shared(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(next_pc >> 16U));
    registers.status = static_cast<std::uint16_t>(
        (saved_status & 0x38ffU) | 0x2000U
        | (static_cast<std::uint16_t>(pending.level) << 8U));

    prefetch(host, 0x00000070U);
    prefetch(host, 0x00000072U);
    (void)read_shared(host, 0x00000048U);
    (void)read_shared(host, 0x0000004aU);
    registers.program_counter = 0x00080048U;
    const auto child = host.call_function(
        47U, 0U, 0xffU, call_kind, completed_pc, 0x00080048U, context);
    if (child.status != TranslationStatus::complete)
        return true;
    if (call_kind == 0U) {
        const auto nested = cpu_a_irq4_vector_trampoline(context);
        if (nested.status != TranslationStatus::complete)
            return true;
    }
    registers.program_counter = next_pc;
    return true;
}
} // namespace

FunctionResult cpu_a_clear_tile_ram(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.data[1] = 0U;
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);
    prefetch(host, 0x00000b9cU);
    prefetch(host, 0x00000b9eU);
    registers.address[0] = 0x00200000U;
    prefetch(host, 0x00000ba0U);
    prefetch(host, 0x00000ba2U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x2803U;
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);
    prefetch(host, 0x00000ba4U);
    prefetch(host, 0x00000ba6U);

    for (;;) {
        write_video_long(host, registers.address[0], registers.data[1]);
        registers.address[0] += 4U;
        set_move_long_flags(registers, registers.data[1]);
        prefetch(host, 0x00000ba8U);
        (void)service_irq4(context, 0x00000ba4U, 0x00000ba6U, 0U);

        const auto counter = static_cast<std::uint16_t>(registers.data[0] - 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU)
            break;
        prefetch(host, 0x00000ba4U);
        prefetch(host, 0x00000ba6U);
        (void)service_irq4(context, 0x00000ba6U, 0x00000ba4U, 1U);
    }

    prefetch(host, 0x00000ba4U);
    prefetch(host, 0x00000baaU);
    prefetch(host, 0x00000bacU);
    const auto target = (static_cast<std::uint32_t>(
        read_shared(host, registers.address[7])) << 16U)
        | read_shared(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
