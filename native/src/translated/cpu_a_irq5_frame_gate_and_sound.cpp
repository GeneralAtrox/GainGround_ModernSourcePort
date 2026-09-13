#include "gain_ground/contract_types.h"
#include "gain_ground/sound_caller_timing.h"
#include "gain_ground/m68000_interrupt_entry.h"

#include <array>
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

template <class Host>
void prefetch(Host &host, std::uint32_t address)
{
    (void)host.read_memory_word(kSharedRegion, address & kSharedMask, kWordMask);
}

void prefetch_program(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

template <class Host>
std::uint16_t read_word(Host &host, std::uint32_t address,
                        std::uint16_t mask = kWordMask)
{
    return host.read_memory_word(kSharedRegion, address & kSharedMask, mask);
}

template <class Host>
std::uint32_t read_long(Host &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

template <class Host>
void write_word(Host &host, std::uint32_t address,
                std::uint16_t value, std::uint16_t mask = kWordMask)
{
    host.write_memory_word(kSharedRegion, address & kSharedMask, value, mask);
}

template <class Host>
void push_long_predecrement(Host &host, CpuRegisters &registers,
                            std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_word(host, registers.address[7] + 2U, static_cast<std::uint16_t>(value));
    write_word(host, registers.address[7], static_cast<std::uint16_t>(value >> 16U));
}

template <class Host>
void push_return(Host &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_word(host, registers.address[7], static_cast<std::uint16_t>(value >> 16U));
    write_word(host, registers.address[7] + 2U, static_cast<std::uint16_t>(value));
}

template <class Host>
void save_all(Host &host, CpuRegisters &registers)
{
    const std::array<std::uint32_t, 15> values{
        registers.address[6], registers.address[5], registers.address[4],
        registers.address[3], registers.address[2], registers.address[1],
        registers.address[0], registers.data[7], registers.data[6],
        registers.data[5], registers.data[4], registers.data[3],
        registers.data[2], registers.data[1], registers.data[0]};
    for (const auto value : values)
        push_long_predecrement(host, registers, value);
}

template <class Host>
void restore_all(Host &host, CpuRegisters &registers)
{
    for (unsigned index = 0; index != 8U; ++index) {
        registers.data[index] = read_long(host, registers.address[7]);
        registers.address[7] += 4U;
    }
    for (unsigned index = 0; index != 7U; ++index) {
        registers.address[index] = read_long(host, registers.address[7]);
        registers.address[7] += 4U;
    }
}

template <class Host>
FunctionResult rte(Host &host, CpuRegisters &registers)
{
    const auto status = read_word(host, registers.address[7]);
    registers.address[7] += 2U;
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.status = status;
    registers.program_counter = target;
    return FunctionResult::complete(2U, target);
}

void set_byte_logic_flags(CpuRegisters &registers, std::uint8_t value);

FunctionResult service_post_rte_irq3(FunctionContext &context,
                                     std::uint32_t resume_pc)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto pending = host.consume_pending_interrupt(0U, 0xffU, 0x00081028U);
    const auto mask = static_cast<std::uint8_t>((registers.status >> 8U) & 7U);
    if (!pending.asserted || pending.level <= mask)
        return FunctionResult::complete(2U, resume_pc);

    if (host.resumes_interrupts_inline())
        return service_cpu_a_autovector(context, pending.level, 0x00081028U, resume_pc);

    const auto continuation_pc = resume_pc;
    const auto saved_status = registers.status;
    registers.address[7] -= 4U;
    write_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(continuation_pc));
    registers.address[7] -= 2U;
    write_word(host, registers.address[7], saved_status);
    write_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(continuation_pc >> 16U));
    registers.status = static_cast<std::uint16_t>(
        (saved_status & 0x38ffU) | 0x2000U
        | (static_cast<std::uint16_t>(pending.level) << 8U));
    // Historical fixture boundary; live entry is handled above.
    const auto level = 3U;
    const auto vector = (24U + level) * 4U;
    prefetch_program(host, vector);
    prefetch_program(host, vector + 2U);
    const auto target = 0x00080042U;
    (void)read_long(host, target);
    registers.program_counter = target;
    const auto child = host.call_function(43U + level, 0U, 0xffU, 3U,
        0x00081028U, target, context);
    if (child.status != TranslationStatus::complete)
        return child;
    if (resume_pc == 0x00080332U) {
        registers.program_counter = resume_pc;
        return FunctionResult::complete(1U, resume_pc);
    }
    if (resume_pc != 0x0008034aU && resume_pc != 0x00080332U) {
        prefetch(host, resume_pc - 4U);
        prefetch(host, resume_pc - 2U);
    }
    const auto frame_gate = static_cast<std::uint8_t>(
        read_word(host, 0xffff8400U, 0xff00U) >> 8U);
    set_byte_logic_flags(registers, frame_gate);
    if (resume_pc == 0x0008034aU) {
        prefetch(host, 0x0008034eU);
        prefetch(host, 0x00080350U);
    } else if (resume_pc != 0x00080332U) {
        prefetch(host, resume_pc);
        prefetch(host, resume_pc + 2U);
    }
    registers.program_counter = resume_pc;
    return FunctionResult::complete(1U, resume_pc);
}

void set_byte_logic_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_a_irq5_frame_gate_and_sound(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};
    SoundCallerTiming host(context);
    auto &registers = context.registers;
    host.begin(0x00080fdaU); // Handler instructions; exception entry is separate.

    prefetch(host, 0x00080fdeU);
    (void)read_word(host, 0xffff8400U, 0xff00U);
    prefetch(host, 0x00080fe0U);
    write_word(host, 0xffff8400U, 0xff00U, 0xff00U);
    prefetch(host, 0x00080fe2U);
    const auto frame_gate = static_cast<std::uint8_t>(
        read_word(host, 0xffff8000U, 0x00ffU));
    set_byte_logic_flags(registers, frame_gate);
    prefetch(host, 0x00080fe4U);
    bool delay_entry_prefetched = false;

    if (frame_gate == 0U) {
        host.clocks(4U); // BNE.b not taken.
        prefetch(host, 0x00080fe6U);
        prefetch(host, 0x00080fe8U);
        const auto busy = static_cast<std::uint8_t>(
            read_word(host, 0xffff800aU, 0xff00U) >> 8U);
        host.clocks(2U); // TAS internal interval between read and write.
        write_word(host, 0xffff800aU,
            static_cast<std::uint16_t>(busy | 0x80U) << 8U, 0xff00U);
        set_byte_logic_flags(registers, busy);
        prefetch(host, 0x00080feaU);

        if (busy == 0U) {
            host.clocks(4U); // BNE.w not taken.
            prefetch(host, 0x00080fecU);
            prefetch(host, 0x00080feeU);
            prefetch(host, 0x00080ff0U);
            save_all(host, registers);
            prefetch(host, 0x00080ff2U);
            prefetch(host, 0x00080ff4U);
            registers.address[5] = 0x00fb0000U;
            prefetch(host, 0x00080ff6U);
            prefetch(host, 0x00080ff8U);
            prefetch(host, 0x00080ffaU);
            registers.address[6] = 0xffffc000U;
            prefetch(host, 0x00080ffcU);
            prefetch(host, 0x00080ffeU);
            prefetch(host, 0x00081000U);
            prefetch(host, 0x00083208U);
            push_return(host, registers, 0x00081002U);
            prefetch(host, 0x0008320aU);
            registers.program_counter = 0x00083208U;
            const auto child = host.call_function(76U, 0U, 0xffU, 2U,
                0x00080ffcU, 0x00083208U, context);
            if (child.status != TranslationStatus::complete
                    || registers.program_counter != 0x00081002U)
                return child;

            host.begin(0x00081002U);
            prefetch(host, 0x00081006U);
            restore_all(host, registers);
            (void)read_word(host, registers.address[7]);
            prefetch(host, 0x00081008U);
            prefetch(host, 0x0008100aU);
            (void)read_word(host, 0xffff800aU, 0xff00U);
            prefetch(host, 0x0008100cU);
            write_word(host, 0xffff800aU, 0U, 0xff00U);
            registers.status = static_cast<std::uint16_t>(
                (registers.status & ~0x000fU) | 0x0004U);
        } else {
            host.clocks(2U); // BNE.w taken.
            prefetch(host, 0x0008100aU);
            prefetch(host, 0x0008100cU);
        }

        prefetch(host, 0x0008100eU);
        prefetch(host, 0x00081010U);
        prefetch(host, 0x00081012U);
        context.host->write_hardware(2U, 0U, 0xffU, 0x0008100aU,
            0x00a00000U, 0x0f3dU, kWordMask);
        host.clocks(4U);
        prefetch(host, 0x00081014U);
        prefetch(host, 0x00081016U);
        prefetch(host, 0x00081018U);
        prefetch(host, 0x0008101aU);
        context.host->write_hardware(2U, 0U, 0xffU, 0x00081012U,
            0x00a00004U, 0x001cU, kWordMask);
        host.clocks(4U);
        delay_entry_prefetched = true;
    } else {
        host.clocks(2U); // BNE.b taken to the delay entry.
    }

    if (!delay_entry_prefetched)
        prefetch(host, 0x0008101aU);
    prefetch(host, 0x0008101cU);
    prefetch(host, 0x0008101eU);
    push_long_predecrement(host, registers, registers.data[0]);
    registers.data[0] = 0x0000007fU;
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);
    // DBRA charges two internal clocks before every target probe. The
    // existing target/exit prefetches supply the remaining 8/12 clocks.
    prefetch(host, 0x00081020U);
    for (;;) {
        prefetch(host, 0x00081022U);
        const auto counter = static_cast<std::uint16_t>(registers.data[0] - 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
        host.clocks(2U);
        prefetch(host, 0x00081020U);
        if (counter == 0xffffU)
            break;
    }
    prefetch(host, 0x00081024U);
    prefetch(host, 0x00081026U);
    prefetch(host, 0x00081028U);
    registers.data[0] = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    (void)read_word(host, registers.address[7]);
    prefetch(host, 0x0008102aU);
    const auto returned = rte(host, registers);
    if (returned.status != TranslationStatus::complete)
        return returned;
    // Entry and its handler own their timing after this RTE completes.
    host.stop();
    return service_post_rte_irq3(context, returned.exit_program_counter);
}

} // namespace gain_ground::translated
