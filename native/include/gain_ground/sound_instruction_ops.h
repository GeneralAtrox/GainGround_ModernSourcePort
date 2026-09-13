#pragma once
#include "gain_ground/instruction_timing.h"

namespace gain_ground {
// Ordinary BTST #7,abs.l / BNE.b / MOVE.B Dn,abs.l / RTS operations, from
// existing original sound-writer bytes. Ordinary interrupt sampling is mapped
// below; pipeline/exception parity remains unverified.
struct SoundInstructionOps {
    enum class Operation { btst, bne, move, rts };
    ExecutionHost &host;
    CpuRegisters &registers;
    std::uint8_t cpu, state;
    std::uint32_t pc;
    Operation operation;
    std::uint32_t port{}, stack{};
    unsigned data_register{};
    std::uint16_t prefetched{}, status_value{}, return_high{}, return_low{};
    PendingInterrupt sampled_interrupt{};

    void sample_interrupt() {
        // The reference copies m_int_next_state before the final prefetch.
        // Latch here, but run the ISR only after this instruction completes.
        // Fixture hosts retain ownership of their captured IRQ boundaries.
        if (!host.resumes_interrupts_inline()) return;
        const auto pending = host.consume_pending_interrupt(cpu, state, pc);
        sampled_interrupt = pending.asserted && pending.level > ((registers.status >> 8U) & 7U)
            ? pending : PendingInterrupt{};
    }

    std::uint16_t data_word(std::uint32_t address) {
        return host.read_memory_word(cpu == 0U ? 3U : 2U, address & 0x3ffffU, 0xffffU);
    }
    std::uint16_t program(TimingInstruction &timing, std::uint32_t address) {
        std::uint16_t value{};
        if (!host.read_timing_program_word(cpu, state, address, value))
            timing.unobserved("FD1094 opcode view is unavailable");
        return value;
    }
    std::uint16_t status_read(std::uint32_t address, std::uint16_t mask) {
        return host.read_hardware(1U, cpu, state, pc, address & ~1U, mask);
    }
    std::uint16_t write_value() const {
        return static_cast<std::uint16_t>((registers.data[data_register] & 0xffU) * 0x0101U);
    }
    void port_write(std::uint32_t address, std::uint16_t mask, std::uint16_t value) {
        host.write_hardware(2U, cpu, state, pc, address & ~1U, value, mask);
        // Kind 3 is the existing YM observation of this write, not another bus access.
        host.write_hardware(3U, cpu, state, pc, (address & 2U) ? 0x800101U : 0x800100U,
                            value & 0xffU, mask);
    }
    void test_flags() {
        registers.status = static_cast<std::uint16_t>((registers.status & ~4U) |
                                                      ((status_value & 0x80U) ? 0U : 4U));
    }
    void move_flags() {
        const auto value = registers.data[data_register] & 0xffU;
        registers.status = static_cast<std::uint16_t>((registers.status & ~0xfU) |
                                                     (value == 0 ? 4U : 0U) | (value & 0x80U ? 8U : 0U));
    }
    std::uint32_t return_pc() const { return (std::uint32_t(return_high) << 16U) | return_low; }
    std::uint32_t branch_pc() const { return (registers.status & 4U) ? pc + 2U : pc - 8U; }
    std::uint32_t next_pc() const {
        switch (operation) {
        case Operation::btst: return pc + 8U;
        case Operation::bne: return branch_pc();
        case Operation::move: return pc + 6U;
        case Operation::rts: return return_pc();
        }
        return pc;
    }
};
} // namespace gain_ground
