#include "gain_ground/contract_types.h"
#include "gground_functions.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgram = 1U;
constexpr std::uint16_t kShared = 3U;
constexpr std::uint16_t kMixer = 10U;
constexpr std::uint16_t kWord = 0xffffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void fetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgram, address, kWord);
}

std::uint16_t read_shared(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kShared, address & kSharedMask, kWord);
}

void write_shared(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kShared, address & kSharedMask, value, kWord);
}

std::uint32_t read_shared_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_shared(host, address)) << 16U)
        | read_shared(host, address + 2U);
}

void write_shared_long(ExecutionHost &host, std::uint32_t address,
    std::uint32_t value)
{
    write_shared(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_shared(host, address + 2U, static_cast<std::uint16_t>(value));
}

void logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void compare_address(CpuRegisters &registers, std::uint32_t left,
    std::uint32_t right)
{
    const auto result = left - right;
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x80000000U) != 0U) flags |= 0x0002U;
    if (left < right) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t return_address)
{
    registers.address[7] -= 4U;
    write_shared_long(host, registers.address[7], return_address);
}

bool service_irq4(FunctionContext &context, std::uint32_t completed_pc,
    std::uint32_t next_pc, std::uint8_t call_kind)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto pending = host.consume_pending_interrupt(0U, 0xffU, completed_pc);
    const auto mask = static_cast<std::uint8_t>((registers.status >> 8U) & 7U);
    if (!pending.asserted || pending.level <= mask) return false;

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
    fetch(host, 0x70U);
    fetch(host, 0x72U);
    (void)read_shared(host, 0x48U);
    (void)read_shared(host, 0x4aU);
    registers.program_counter = 0x00080048U;
    const auto child = host.call_function(
        47U, 0U, 0xffU, call_kind, completed_pc, 0x00080048U, context);
    if (child.status != TranslationStatus::complete) return true;
    if (call_kind == 0U) {
        const auto nested = cpu_a_irq4_vector_trampoline(context);
        if (nested.status != TranslationStatus::complete) return true;
    }
    registers.program_counter = next_pc;
    return true;
}

FunctionResult call_child(FunctionContext &context, std::uint32_t id,
    std::uint32_t callsite, std::uint32_t target, std::uint32_t return_address)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    push_return(host, registers, return_address);
    fetch(host, target);
    fetch(host, target + 2U);
    registers.program_counter = target;
    return host.call_function(id, 0U, 0xffU, 2U, callsite, target, context);
}
} // namespace

FunctionResult cpu_a_post_load_video_refresh(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    fetch(host, 0x000006faU);
    auto gate = read_shared(host, 0xfffffc80U);
    logic_word(registers, gate);
    if (gate == 0U) {
        fetch(host, 0x000006fcU);
        fetch(host, 0x00000756U);
        fetch(host, 0x00000758U);
        const auto target = read_shared_long(host, registers.address[7]);
        registers.address[7] += 4U;
        fetch(host, target);
        fetch(host, target + 2U);
        registers.program_counter = target;
        return FunctionResult::complete(1U, target);
    }

    fetch(host, 0x000006fcU);
    fetch(host, 0x000006feU);
    fetch(host, 0x00000700U);
    compare_address(registers, registers.address[1], 0x00200000U);
    fetch(host, 0x00000702U);
    if ((registers.status & 0x0001U) != 0U) {
        fetch(host, 0x00000704U);
        fetch(host, 0x00000756U);
        fetch(host, 0x00000758U);
        const auto target = read_shared_long(host, registers.address[7]);
        registers.address[7] += 4U;
        fetch(host, target);
        fetch(host, target + 2U);
        registers.program_counter = target;
        return FunctionResult::complete(1U, target);
    }
    fetch(host, 0x00000704U);
    fetch(host, 0x00000706U);
    fetch(host, 0x00000708U);
    compare_address(registers, registers.address[1], 0x00600000U);
    fetch(host, 0x0000070aU);
    if ((registers.status & 0x0001U) == 0U) {
        fetch(host, 0x0000070cU);
        fetch(host, 0x00000756U);
        fetch(host, 0x00000758U);
        const auto target = read_shared_long(host, registers.address[7]);
        registers.address[7] += 4U;
        fetch(host, target);
        fetch(host, target + 2U);
        registers.program_counter = target;
        return FunctionResult::complete(1U, target);
    }

    fetch(host, 0x0000070cU);
    fetch(host, 0x0000070eU);
    fetch(host, 0x00000710U);
    (void)read_shared(host, 0xfffffc80U);
    fetch(host, 0x00000712U);
    write_shared(host, 0xfffffc80U, 0U);
    logic_word(registers, 0U);
    fetch(host, 0x00000714U);

    registers.address[7] -= 12U;
    write_shared(host, registers.address[7] + 10U,
        static_cast<std::uint16_t>(registers.address[0]));
    write_shared(host, registers.address[7] + 8U,
        static_cast<std::uint16_t>(registers.address[0] >> 16U));
    write_shared(host, registers.address[7] + 6U,
        static_cast<std::uint16_t>(registers.data[1]));
    write_shared(host, registers.address[7] + 4U,
        static_cast<std::uint16_t>(registers.data[1] >> 16U));
    write_shared(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(registers.data[0]));
    write_shared(host, registers.address[7],
        static_cast<std::uint16_t>(registers.data[0] >> 16U));

    fetch(host, 0x00000716U);
    fetch(host, 0x00000718U);
    fetch(host, 0x0000071aU);
    fetch(host, 0x0000071cU);
    host.write_memory_word(kMixer, 0x1aU, 0x0001U, 0x00ffU);
    fetch(host, 0x0000071eU);
    registers.status = 0x2300U;
    (void)service_irq4(context, 0x00000720U, 0x00000720U, 0U);

    auto child = call_child(context, 6U, 0x00000720U, 0x00000bacU, 0x00000724U);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;
    child = call_child(context, 5U, 0x00000724U, 0x00000b98U, 0x00000728U);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    fetch(host, 0x0000072cU);
    fetch(host, 0x0000072eU);
    fetch(host, 0x00000730U);
    host.write_memory_word(kMixer, 0x1aU, 0x0000U, 0x00ffU);
    fetch(host, 0x00000732U);
    registers.status = 0x2300U;
    (void)service_irq4(context, 0x00000734U, 0x00000734U, 0U);

    fetch(host, 0x00000738U);
    (void)read_shared(host, 0xfffffc84U);
    fetch(host, 0x0000073aU);
    write_shared(host, 0xfffffc84U, 0U);
    logic_word(registers, 0U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x4400U;
    logic_word(registers, 0x4400U);
    fetch(host, 0x0000073cU);
    fetch(host, 0x0000073eU);
    for (;;) {
        fetch(host, 0x0000073cU);
        if (static_cast<std::uint16_t>(registers.data[0]) != 0U)
            fetch(host, 0x0000073eU);
        const auto counter = static_cast<std::uint16_t>(registers.data[0] - 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
        const auto next = counter == 0xffffU ? 0x00000740U : 0x0000073cU;
        (void)service_irq4(context, 0x0000073cU, next, 1U);
        if (counter == 0xffffU) break;
    }

    fetch(host, 0x00000740U);
    fetch(host, 0x00000742U);
    fetch(host, 0x00000744U);
    const auto marker = read_shared(host, 0xfffffc84U);
    logic_word(registers, marker);
    fetch(host, 0x00000746U);
    const bool write_sync = marker == 0U;
    if (write_sync) {
        fetch(host, 0x00000748U);
        fetch(host, 0x0000074aU);
        fetch(host, 0x0000074cU);
    }
    fetch(host, 0x0000074eU);
    if (write_sync) {
        host.write_hardware(2U, 0U, 0xffU, 0x00000746U,
            0x00270000U, 0x0101U, 0x00ffU);
    }
    fetch(host, 0x00000750U);
    registers.status = 0x2600U;
    fetch(host, 0x00000752U);
    fetch(host, 0x00000752U);
    fetch(host, 0x00000754U);
    fetch(host, 0x00000756U);
    registers.data[0] = read_shared_long(host, registers.address[7]);
    registers.data[1] = read_shared_long(host, registers.address[7] + 4U);
    registers.address[0] = read_shared_long(host, registers.address[7] + 8U);
    registers.address[7] += 12U;
    const auto target_high = read_shared(host, registers.address[7]);
    fetch(host, 0x00000758U);
    (void)read_shared(host, registers.address[7]);
    const auto target = (static_cast<std::uint32_t>(target_high) << 16U)
        | read_shared(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    fetch(host, target);
    fetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
