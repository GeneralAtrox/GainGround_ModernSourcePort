// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult cpu_b_advance_phase_and_dispatch(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xd54eU: { // 61000650 bsr.w $dba0
            next = 0xd552U;
            const auto result = m.call(c, 154U, 0xd54eU, 0xdba0U, 0xd552U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd552U: { // 610006dc bsr.w $dc30
            next = 0xd556U;
            const auto result = m.call(c, 156U, 0xd552U, 0xdc30U, 0xd556U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd556U: { // 302d000c move.w $c(a5), d0
            next = 0xd55aU;
            m.dw(0U, m.word(r.address[5] + 0xcU));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xd55aU: { // e540 asl.w #$2, d0
            next = 0xd55cU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xd55cU: { // 4efb0002 jmp $d560(pc, d0.w)
            next = 0xd560U;
            next = (0xd560U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0xd560U: { // 60000006 bra.w $d568
            next = 0xd564U;
            if (true) { next = 0xd568U; transfer_kind = 1U; }
            break;
        }
        case 0xd564U: { // 60000064 bra.w $d5ca
            next = 0xd568U;
            if (true) { next = 0xd5caU; transfer_kind = 1U; }
            break;
        }
        case 0xd568U: { // 536d0018 subq.w #$1, $18(a5)
            next = 0xd56cU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x18U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xd56cU: { // 6e22 bgt.b $d590
            next = 0xd56eU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0xd590U; transfer_kind = 1U; }
            break;
        }
        case 0xd56eU: { // 3b7c00400018 move.w #$40, $18(a5)
            next = 0xd574U;
            m.word(r.address[5] + 0x18U, 0x40U);
            m.logic(0x40U, 16U);
            break;
        }
        case 0xd574U: { // 0c6d00040010 cmpi.w #$4, $10(a5)
            next = 0xd57aU;
            const auto source_value = 0x4U;
            const auto destination_address = r.address[5] + 0x10U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xd57aU: { // 6746 beq.b $d5c2
            next = 0xd57cU;
            if ((r.status & 4U) != 0U) { next = 0xd5c2U; transfer_kind = 1U; }
            break;
        }
        case 0xd57cU: { // 302d0010 move.w $10(a5), d0
            next = 0xd580U;
            m.dw(0U, m.word(r.address[5] + 0x10U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xd580U: { // 526d0010 addq.w #$1, $10(a5)
            next = 0xd584U;
            const auto address = r.address[5] + 0x10U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xd584U: { // e740 asl.w #$3, d0
            next = 0xd586U;
            m.asl_word(0U, 3U);
            break;
        }
        case 0xd586U: { // 41f80c1a lea.l $c1a.w, a0
            next = 0xd58aU;
            r.address[0] = 0xc1aU;
            break;
        }
        case 0xd58aU: { // 31bc00020000 move.w #$2, (a0, d0.w)
            next = 0xd590U;
            const auto value = 0x2U;
            const auto destination_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd590U: { // 49f80c1a lea.l $c1a.w, a4
            next = 0xd594U;
            r.address[4] = 0xc1aU;
            break;
        }
        case 0xd594U: { // 43fa0ac8 lea.l $e05e(pc), a1
            next = 0xd598U;
            const auto source_address = 0xe05eU;
            r.address[1] = source_address;
            break;
        }
        case 0xd598U: { // 7403 moveq #$3, d2
            next = 0xd59aU;
            r.data[2] = 0x3U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0xd59aU: { // 61000764 bsr.w $dd00
            next = 0xd59eU;
            const auto result = m.call(c, 161U, 0xd59aU, 0xdd00U, 0xd59eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd59eU: { // 4a54 tst.w (a4)
            next = 0xd5a0U;
            const auto destination_address = r.address[4];
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd5a0U: { // 6716 beq.b $d5b8
            next = 0xd5a2U;
            if ((r.status & 4U) != 0U) { next = 0xd5b8U; transfer_kind = 1U; }
            break;
        }
        case 0xd5a2U: { // 41f900208800 lea.l $208800.l, a0
            next = 0xd5a8U;
            r.address[0] = 0x208800U;
            break;
        }
        case 0xd5a8U: { // d0d1 adda.w (a1), a0
            next = 0xd5aaU;
            const auto source_address = r.address[1];
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xd5aaU: { // 32290002 move.w $2(a1), d1
            next = 0xd5aeU;
            m.dw(1U, m.word(r.address[1] + 0x2U));
            m.logic(r.data[1], 16U);
            break;
        }
        case 0xd5aeU: { // 302c0002 move.w $2(a4), d0
            next = 0xd5b2U;
            m.dw(0U, m.word(r.address[4] + 0x2U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xd5b2U: { // 30c0 move.w d0, (a0)+
            next = 0xd5b4U;
            const auto value = r.data[0];
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            r.address[0] += 2U;
            m.logic(value, 16U);
            break;
        }
        case 0xd5b4U: { // 51c9fffc dbra d1, $d5b2
            next = 0xd5b8U;
            m.dw(1U, r.data[1] - 1U);
            if ((r.data[1] & 0xffffU) != 0xffffU) next = 0xd5b2U;
            break;
        }
        case 0xd5b8U: { // 5849 addq.w #$4, a1
            next = 0xd5baU;
            const auto source_value = 0x4U;
            r.address[1] += source_value;
            break;
        }
        case 0xd5baU: { // 504c addq.w #$8, a4
            next = 0xd5bcU;
            const auto source_value = 0x8U;
            r.address[4] += source_value;
            break;
        }
        case 0xd5bcU: { // 51caffdc dbra d2, $d59a
            next = 0xd5c0U;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0xd59aU;
            break;
        }
        case 0xd5c0U: { // 4e75 rts 
            next = 0xd5c2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd5c0U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xd5c2U: { // 3b7c0001000c move.w #$1, $c(a5)
            next = 0xd5c8U;
            m.word(r.address[5] + 0xcU, 0x1U);
            m.logic(0x1U, 16U);
            break;
        }
        case 0xd5c8U: { // 4e75 rts 
            next = 0xd5caU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd5c8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xd5caU: { // 526d0018 addq.w #$1, $18(a5)
            next = 0xd5ceU;
            const auto address = r.address[5] + 0x18U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xd5ceU: { // 0c6d00960018 cmpi.w #$96, $18(a5)
            next = 0xd5d4U;
            const auto source_value = 0x96U;
            const auto destination_address = r.address[5] + 0x18U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xd5d4U: { // 6b0c bmi.b $d5e2
            next = 0xd5d6U;
            if ((r.status & 8U) != 0U) { next = 0xd5e2U; transfer_kind = 1U; }
            break;
        }
        case 0xd5d6U: { // 4a788002 tst.w $8002.w
            next = 0xd5daU;
            const auto destination_address = 0xffff8002U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd5daU: { // 6606 bne.b $d5e2
            next = 0xd5dcU;
            if ((r.status & 4U) == 0U) { next = 0xd5e2U; transfer_kind = 1U; }
            break;
        }
        case 0xd5dcU: { // 4ef900008806 jmp $8806.l
            next = 0xd5e2U;
            next = 0x8806U;
            transfer_kind = 1U;
            break;
        }
        case 0xd5e2U: { // 4e75 rts 
            next = 0xd5e4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd5e2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0x8806U) return c.host->call_function(124U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0xd54eU:
        case 0xd552U:
        case 0xd556U:
        case 0xd55aU:
        case 0xd55cU:
        case 0xd560U:
        case 0xd564U:
        case 0xd568U:
        case 0xd56cU:
        case 0xd56eU:
        case 0xd574U:
        case 0xd57aU:
        case 0xd57cU:
        case 0xd580U:
        case 0xd584U:
        case 0xd586U:
        case 0xd58aU:
        case 0xd590U:
        case 0xd594U:
        case 0xd598U:
        case 0xd59aU:
        case 0xd59eU:
        case 0xd5a0U:
        case 0xd5a2U:
        case 0xd5a8U:
        case 0xd5aaU:
        case 0xd5aeU:
        case 0xd5b2U:
        case 0xd5b4U:
        case 0xd5b8U:
        case 0xd5baU:
        case 0xd5bcU:
        case 0xd5c0U:
        case 0xd5c2U:
        case 0xd5c8U:
        case 0xd5caU:
        case 0xd5ceU:
        case 0xd5d4U:
        case 0xd5d6U:
        case 0xd5daU:
        case 0xd5dcU:
        case 0xd5e2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
