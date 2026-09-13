// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult cpu_b_step_triplet_phase_and_restart(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xd4aeU: { // 61000042 bsr.w $d4f2
            next = 0xd4b2U;
            const auto result = m.call(c, 150U, 0xd4aeU, 0xd4f2U, 0xd4b2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd4b2U: { // 610006ec bsr.w $dba0
            next = 0xd4b6U;
            const auto result = m.call(c, 154U, 0xd4b2U, 0xdba0U, 0xd4b6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd4b6U: { // 61000778 bsr.w $dc30
            next = 0xd4baU;
            const auto result = m.call(c, 156U, 0xd4b6U, 0xdc30U, 0xd4baU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd4baU: { // 536d0018 subq.w #$1, $18(a5)
            next = 0xd4beU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x18U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xd4beU: { // 6630 bne.b $d4f0
            next = 0xd4c0U;
            if ((r.status & 4U) == 0U) { next = 0xd4f0U; transfer_kind = 1U; }
            break;
        }
        case 0xd4c0U: { // 42780c00 clr.w $c00.w
            next = 0xd4c4U;
            const auto destination_address = 0xc00U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd4c4U: { // 41f80c02 lea.l $c02.w, a0
            next = 0xd4c8U;
            r.address[0] = 0xc02U;
            break;
        }
        case 0xd4c8U: { // 5250 addq.w #$1, (a0)
            next = 0xd4caU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[0];
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xd4caU: { // 0c500005 cmpi.w #$5, (a0)
            next = 0xd4ceU;
            const auto source_value = 0x5U;
            const auto destination_address = r.address[0];
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xd4ceU: { // 6502 bcs.b $d4d2
            next = 0xd4d0U;
            if ((r.status & 1U) != 0U) { next = 0xd4d2U; transfer_kind = 1U; }
            break;
        }
        case 0xd4d0U: { // 4250 clr.w (a0)
            next = 0xd4d2U;
            const auto destination_address = r.address[0];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd4d2U: { // 4af80836 tas.b $836.w
            next = 0xd4d6U;
            const auto destination_address = 0x836U;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0xd4d6U: { // 30380838 move.w $838.w, d0
            next = 0xd4daU;
            const auto source_address = 0x838U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd4daU: { // d07c000a add.w #$a, d0
            next = 0xd4deU;
            const auto source_value = 0xaU;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd4deU: { // 0c400029 cmpi.w #$29, d0
            next = 0xd4e2U;
            const auto source_value = 0x29U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xd4e2U: { // 6502 bcs.b $d4e6
            next = 0xd4e4U;
            if ((r.status & 1U) != 0U) { next = 0xd4e6U; transfer_kind = 1U; }
            break;
        }
        case 0xd4e4U: { // 7000 moveq #$0, d0
            next = 0xd4e6U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xd4e6U: { // 31c00838 move.w d0, $838.w
            next = 0xd4eaU;
            const auto value = r.data[0];
            const auto destination_address = 0x838U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd4eaU: { // 4ef900009c8e jmp $9c8e.l
            next = 0xd4f0U;
            next = 0x9c8eU;
            transfer_kind = 1U;
            break;
        }
        case 0xd4f0U: { // 4e75 rts 
            next = 0xd4f2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd4f0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0x9c8eU) return c.host->call_function(519U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0xd4aeU:
        case 0xd4b2U:
        case 0xd4b6U:
        case 0xd4baU:
        case 0xd4beU:
        case 0xd4c0U:
        case 0xd4c4U:
        case 0xd4c8U:
        case 0xd4caU:
        case 0xd4ceU:
        case 0xd4d0U:
        case 0xd4d2U:
        case 0xd4d6U:
        case 0xd4daU:
        case 0xd4deU:
        case 0xd4e2U:
        case 0xd4e4U:
        case 0xd4e6U:
        case 0xd4eaU:
        case 0xd4f0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
