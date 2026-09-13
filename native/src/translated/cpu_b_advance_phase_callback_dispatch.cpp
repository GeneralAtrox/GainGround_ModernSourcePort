// Implemented but unverified. Validation is recorded in the current function work packet.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult cpu_b_advance_phase_callback_dispatch(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xd734U: { // 6100046a bsr.w $dba0
            next = 0xd738U;
            const auto result = m.call(c, 154U, 0xd734U, 0xdba0U, 0xd738U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd738U: { // 61000620 bsr.w $dd5a
            next = 0xd73cU;
            const auto result = m.call(c, 162U, 0xd738U, 0xdd5aU, 0xd73cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd73cU: { // 6100062e bsr.w $dd6c
            next = 0xd740U;
            const auto result = m.call(c, 163U, 0xd73cU, 0xdd6cU, 0xd740U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd740U: { // 30380c16 move.w $c16.w, d0
            next = 0xd744U;
            const auto source_address = 0xc16U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd744U: { // e540 asl.w #$2, d0
            next = 0xd746U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xd746U: { // 4efb0002 jmp $d74a(pc, d0.w)
            next = 0xd74aU;
            next = (0xd74aU + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0xd74aU: { // 6000001e bra.w $d76a
            next = 0xd74eU;
            if (true) { next = 0xd76aU; transfer_kind = 1U; }
            break;
        }
        case 0xd74eU: { // 600000f6 bra.w $d846
            next = 0xd752U;
            if (true) { next = 0xd846U; transfer_kind = 1U; }
            break;
        }
        case 0xd752U: { // 6000014e bra.w $d8a2
            next = 0xd756U;
            if (true) { next = 0xd8a2U; transfer_kind = 1U; }
            break;
        }
        case 0xd756U: { // 600001b6 bra.w $d90e
            next = 0xd75aU;
            if (true) { next = 0xd90eU; transfer_kind = 1U; }
            break;
        }
        case 0xd75aU: { // 600001b8 bra.w $d914
            next = 0xd75eU;
            if (true) { next = 0xd914U; transfer_kind = 1U; }
            break;
        }
        case 0xd75eU: { // 60000216 bra.w $d976
            next = 0xd762U;
            if (true) { next = 0xd976U; transfer_kind = 1U; }
            break;
        }
        case 0xd762U: { // 600002ac bra.w $da10
            next = 0xd766U;
            if (true) { next = 0xda10U; transfer_kind = 1U; }
            break;
        }
        case 0xd766U: { // 600003f0 bra.w $db58
            next = 0xd76aU;
            if (true) { next = 0xdb58U; transfer_kind = 1U; }
            break;
        }
        case 0xd76aU: { // 6100061e bsr.w $dd8a
            next = 0xd76eU;
            const auto result = m.call(c, 164U, 0xd76aU, 0xdd8aU, 0xd76eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd76eU: { // 6100068a bsr.w $ddfa
            next = 0xd772U;
            const auto result = m.call(c, 165U, 0xd76eU, 0xddfaU, 0xd772U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd772U: { // 610006d8 bsr.w $de4c
            next = 0xd776U;
            const auto result = m.call(c, 166U, 0xd772U, 0xde4cU, 0xd776U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd776U: { // 4a780c10 tst.w $c10.w
            next = 0xd77aU;
            const auto destination_address = 0xc10U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd77aU: { // 6e62 bgt.b $d7de
            next = 0xd77cU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0xd7deU; transfer_kind = 1U; }
            break;
        }
        case 0xd77cU: { // 42780c10 clr.w $c10.w
            next = 0xd780U;
            const auto destination_address = 0xc10U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd780U: { // 4a780c12 tst.w $c12.w
            next = 0xd784U;
            const auto destination_address = 0xc12U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd784U: { // 6666 bne.b $d7ec
            next = 0xd786U;
            if ((r.status & 4U) == 0U) { next = 0xd7ecU; transfer_kind = 1U; }
            break;
        }
        case 0xd786U: { // 0c7800030c00 cmpi.w #$3, $c00.w
            next = 0xd78cU;
            const auto source_value = 0x3U;
            const auto destination_address = 0xc00U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xd78cU: { // 6418 bcc.b $d7a6
            next = 0xd78eU;
            if ((r.status & 1U) == 0U) { next = 0xd7a6U; transfer_kind = 1U; }
            break;
        }
        case 0xd78eU: { // 31fc00040c16 move.w #$4, $c16.w
            next = 0xd794U;
            const auto value = 0x4U;
            const auto destination_address = 0xc16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd794U: { // 3b7c00000018 move.w #$0, $18(a5)
            next = 0xd79aU;
            m.word(r.address[5] + 0x18U, 0x0U);
            m.logic(0x0U, 16U);
            break;
        }
        case 0xd79aU: { // 3b7c000b001a move.w #$b, $1a(a5)
            next = 0xd7a0U;
            m.word(r.address[5] + 0x1aU, 0xbU);
            m.logic(0xbU, 16U);
            break;
        }
        case 0xd7a0U: { // 610006c6 bsr.w $de68
            next = 0xd7a4U;
            const auto result = m.call(c, 167U, 0xd7a0U, 0xde68U, 0xd7a4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd7a4U: { // 4e75 rts 
            next = 0xd7a6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd7a4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xd7a6U: { // 4a380c06 tst.b $c06.w
            next = 0xd7aaU;
            const auto destination_address = 0xc06U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0xd7aaU: { // 66000098 bne.w $d844
            next = 0xd7aeU;
            if ((r.status & 4U) == 0U) { next = 0xd844U; transfer_kind = 1U; }
            break;
        }
        case 0xd7aeU: { // 4a380c04 tst.b $c04.w
            next = 0xd7b2U;
            const auto destination_address = 0xc04U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0xd7b2U: { // 66000090 bne.w $d844
            next = 0xd7b6U;
            if ((r.status & 4U) == 0U) { next = 0xd844U; transfer_kind = 1U; }
            break;
        }
        case 0xd7b6U: { // 4a780c00 tst.w $c00.w
            next = 0xd7baU;
            const auto destination_address = 0xc00U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd7baU: { // 670a beq.b $d7c6
            next = 0xd7bcU;
            if ((r.status & 4U) != 0U) { next = 0xd7c6U; transfer_kind = 1U; }
            break;
        }
        case 0xd7bcU: { // 31fc00018002 move.w #$1, $8002.w
            next = 0xd7c2U;
            const auto value = 0x1U;
            const auto destination_address = 0xffff8002U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd7c2U: { // 42780c00 clr.w $c00.w
            next = 0xd7c6U;
            const auto destination_address = 0xc00U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd7c6U: { // 42780c02 clr.w $c02.w
            next = 0xd7caU;
            const auto destination_address = 0xc02U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd7caU: { // 42780820 clr.w $820.w
            next = 0xd7ceU;
            const auto destination_address = 0x820U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd7ceU: { // 30380836 move.w $836.w, d0
            next = 0xd7d2U;
            const auto source_address = 0x836U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd7d2U: { // 5940 subq.w #$4, d0
            next = 0xd7d4U;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd7d4U: { // 61000508 bsr.w $dcde
            next = 0xd7d8U;
            const auto result = m.call(c, 160U, 0xd7d4U, 0xdcdeU, 0xd7d8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd7d8U: { // 4ef900009c8e jmp $9c8e.l
            next = 0xd7deU;
            next = 0x9c8eU;
            transfer_kind = 1U;
            break;
        }
        case 0xd7deU: { // 0c3800020d2d cmpi.b #$2, $d2d.w
            next = 0xd7e4U;
            const auto source_value = 0x2U;
            const auto destination_address = 0xd2dU;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0xd7e4U: { // 6a06 bpl.b $d7ec
            next = 0xd7e6U;
            if ((r.status & 8U) == 0U) { next = 0xd7ecU; transfer_kind = 1U; }
            break;
        }
        case 0xd7e6U: { // 4a780c14 tst.w $c14.w
            next = 0xd7eaU;
            const auto destination_address = 0xc14U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd7eaU: { // 6e30 bgt.b $d81c
            next = 0xd7ecU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0xd81cU; transfer_kind = 1U; }
            break;
        }
        case 0xd7ecU: { // 4a380c06 tst.b $c06.w
            next = 0xd7f0U;
            const auto destination_address = 0xc06U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0xd7f0U: { // 6652 bne.b $d844
            next = 0xd7f2U;
            if ((r.status & 4U) == 0U) { next = 0xd844U; transfer_kind = 1U; }
            break;
        }
        case 0xd7f2U: { // 31fc00010c16 move.w #$1, $c16.w
            next = 0xd7f8U;
            const auto value = 0x1U;
            const auto destination_address = 0xc16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd7f8U: { // 426d0018 clr.w $18(a5)
            next = 0xd7fcU;
            const auto destination_address = r.address[5] + 0x18U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd7fcU: { // 41f80ea2 lea.l $ea2.w, a0
            next = 0xd800U;
            r.address[0] = 0xea2U;
            break;
        }
        case 0xd800U: { // 5250 addq.w #$1, (a0)
            next = 0xd802U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[0];
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xd802U: { // 52680200 addq.w #$1, $200(a0)
            next = 0xd806U;
            const auto address = r.address[0] + 0x200U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xd806U: { // 52680400 addq.w #$1, $400(a0)
            next = 0xd80aU;
            const auto address = r.address[0] + 0x400U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xd80aU: { // 4a780c14 tst.w $c14.w
            next = 0xd80eU;
            const auto destination_address = 0xc14U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd80eU: { // 670a beq.b $d81a
            next = 0xd810U;
            if ((r.status & 4U) != 0U) { next = 0xd81aU; transfer_kind = 1U; }
            break;
        }
        case 0xd810U: { // 4250 clr.w (a0)
            next = 0xd812U;
            const auto destination_address = r.address[0];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd812U: { // 42680200 clr.w $200(a0)
            next = 0xd816U;
            const auto destination_address = r.address[0] + 0x200U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd816U: { // 42680400 clr.w $400(a0)
            next = 0xd81aU;
            const auto destination_address = r.address[0] + 0x400U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd81aU: { // 4e75 rts 
            next = 0xd81cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd81aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xd81cU: { // 4a780c0a tst.w $c0a.w
            next = 0xd820U;
            const auto destination_address = 0xc0aU;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd820U: { // 6622 bne.b $d844
            next = 0xd822U;
            if ((r.status & 4U) == 0U) { next = 0xd844U; transfer_kind = 1U; }
            break;
        }
        case 0xd822U: { // 31fc00030c16 move.w #$3, $c16.w
            next = 0xd828U;
            const auto value = 0x3U;
            const auto destination_address = 0xc16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd828U: { // 4df81780 lea.l $1780.w, a6
            next = 0xd82cU;
            r.address[6] = 0x1780U;
            break;
        }
        case 0xd82cU: { // 7402 moveq #$2, d2
            next = 0xd82eU;
            r.data[2] = 0x2U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0xd82eU: { // 7200 moveq #$0, d1
            next = 0xd830U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xd830U: { // 7007 moveq #$7, d0
            next = 0xd832U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xd832U: { // 3c81 move.w d1, (a6)
            next = 0xd834U;
            const auto value = r.data[1];
            const auto destination_address = r.address[6];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd834U: { // 4dee0080 lea.l $80(a6), a6
            next = 0xd838U;
            r.address[6] = r.address[6] + 0x80U;
            break;
        }
        case 0xd838U: { // 51c8fff8 dbra d0, $d832
            next = 0xd83cU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0xd832U;
            break;
        }
        case 0xd83cU: { // 4dee0180 lea.l $180(a6), a6
            next = 0xd840U;
            r.address[6] = r.address[6] + 0x180U;
            break;
        }
        case 0xd840U: { // 51caffee dbra d2, $d830
            next = 0xd844U;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0xd830U;
            break;
        }
        case 0xd844U: { // 4e75 rts 
            next = 0xd846U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd844U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xd846U: { // 526d0018 addq.w #$1, $18(a5)
            next = 0xd84aU;
            const auto address = r.address[5] + 0x18U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xd84aU: { // 0c6d00010018 cmpi.w #$1, $18(a5)
            next = 0xd850U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x18U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xd850U: { // 654e bcs.b $d8a0
            next = 0xd852U;
            if ((r.status & 1U) != 0U) { next = 0xd8a0U; transfer_kind = 1U; }
            break;
        }
        case 0xd852U: { // 31fc00020c16 move.w #$2, $c16.w
            next = 0xd858U;
            const auto value = 0x2U;
            const auto destination_address = 0xc16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd858U: { // 3b7c005a0018 move.w #$5a, $18(a5)
            next = 0xd85eU;
            m.word(r.address[5] + 0x18U, 0x5aU);
            m.logic(0x5aU, 16U);
            break;
        }
        case 0xd85eU: { // 50f80d0c st.b $d0c.w
            next = 0xd862U;
            const auto destination_address = 0xd0cU;
            (void)m.byte(destination_address);
            m.byte(destination_address, 0xffU);
            break;
        }
        case 0xd862U: { // 52780c02 addq.w #$1, $c02.w
            next = 0xd866U;
            const auto source_value = 0x1U;
            const auto destination_address = 0xc02U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xd866U: { // 30380c02 move.w $c02.w, d0
            next = 0xd86aU;
            const auto source_address = 0xc02U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd86aU: { // 7200 moveq #$0, d1
            next = 0xd86cU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xd86cU: { // 3200 move.w d0, d1
            next = 0xd86eU;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd86eU: { // 83fc000a divs.w #$a, d1
            next = 0xd872U;
            const auto divisor = 0xaU;
            if ((divisor & 0xffffU) == 0U) {
                const auto result = m.exception(c, 5U, 0xd86eU, 0xd872U);
                if (result.status != TranslationStatus::complete || result.control != 2U) return result;
                next = r.program_counter;
            } else {
                m.divide(1U, divisor, true);
            }
            break;
        }
        case 0xd872U: { // 4841 swap d1
            next = 0xd874U;
            const auto value = (r.data[1] << 16U) | (r.data[1] >> 16U);
            r.data[1] = value;
            m.logic(value, 32U);
            break;
        }
        case 0xd874U: { // 4a41 tst.w d1
            next = 0xd876U;
            const auto value = r.data[1];
            m.logic(value, 16U);
            break;
        }
        case 0xd876U: { // 661c bne.b $d894
            next = 0xd878U;
            if ((r.status & 4U) == 0U) { next = 0xd894U; transfer_kind = 1U; }
            break;
        }
        case 0xd878U: { // 52780c00 addq.w #$1, $c00.w
            next = 0xd87cU;
            const auto source_value = 0x1U;
            const auto destination_address = 0xc00U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xd87cU: { // 32380c00 move.w $c00.w, d1
            next = 0xd880U;
            const auto source_address = 0xc00U;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd880U: { // 5241 addq.w #$1, d1
            next = 0xd882U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0xd882U: { // 31c18002 move.w d1, $8002.w
            next = 0xd886U;
            const auto value = r.data[1];
            const auto destination_address = 0xffff8002U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd886U: { // 5240 addq.w #$1, d0
            next = 0xd888U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd888U: { // 31c08006 move.w d0, $8006.w
            next = 0xd88cU;
            const auto value = r.data[0];
            const auto destination_address = 0xffff8006U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd88cU: { // 4eb90000a8b6 jsr $a8b6.l
            next = 0xd892U;
            const auto result = m.call(c, 615U, 0xd88cU, 0xa8b6U, 0xd892U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd892U: { // 4e75 rts 
            next = 0xd894U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd892U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xd894U: { // 5240 addq.w #$1, d0
            next = 0xd896U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd896U: { // 31c08006 move.w d0, $8006.w
            next = 0xd89aU;
            const auto value = r.data[0];
            const auto destination_address = 0xffff8006U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd89aU: { // 4eb90000a87c jsr $a87c.l
            next = 0xd8a0U;
            const auto result = m.call(c, 614U, 0xd89aU, 0xa87cU, 0xd8a0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd8a0U: { // 4e75 rts 
            next = 0xd8a2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd8a0U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xd8a2U: { // 302d0018 move.w $18(a5), d0
            next = 0xd8a6U;
            m.dw(0U, m.word(r.address[5] + 0x18U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xd8a6U: { // 6712 beq.b $d8ba
            next = 0xd8a8U;
            if ((r.status & 4U) != 0U) { next = 0xd8baU; transfer_kind = 1U; }
            break;
        }
        case 0xd8a8U: { // 5340 subq.w #$1, d0
            next = 0xd8aaU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd8aaU: { // 3b400018 move.w d0, $18(a5)
            next = 0xd8aeU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x18U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd8aeU: { // 0c40003c cmpi.w #$3c, d0
            next = 0xd8b2U;
            const auto source_value = 0x3cU;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xd8b2U: { // 6a04 bpl.b $d8b8
            next = 0xd8b4U;
            if ((r.status & 8U) == 0U) { next = 0xd8b8U; transfer_kind = 1U; }
            break;
        }
        case 0xd8b4U: { // 42380d0c clr.b $d0c.w
            next = 0xd8b8U;
            const auto destination_address = 0xd0cU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0xd8b8U: { // 4e75 rts 
            next = 0xd8baU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd8b8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xd8baU: { // 4a380c05 tst.b $c05.w
            next = 0xd8beU;
            const auto destination_address = 0xc05U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0xd8beU: { // 664c bne.b $d90c
            next = 0xd8c0U;
            if ((r.status & 4U) == 0U) { next = 0xd90cU; transfer_kind = 1U; }
            break;
        }
        case 0xd8c0U: { // 4a788002 tst.w $8002.w
            next = 0xd8c4U;
            const auto destination_address = 0xffff8002U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd8c4U: { // 6646 bne.b $d90c
            next = 0xd8c6U;
            if ((r.status & 4U) == 0U) { next = 0xd90cU; transfer_kind = 1U; }
            break;
        }
        case 0xd8c6U: { // 4a788006 tst.w $8006.w
            next = 0xd8caU;
            const auto destination_address = 0xffff8006U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd8caU: { // 6640 bne.b $d90c
            next = 0xd8ccU;
            if ((r.status & 4U) == 0U) { next = 0xd90cU; transfer_kind = 1U; }
            break;
        }
        case 0xd8ccU: { // 0c7800040c00 cmpi.w #$4, $c00.w
            next = 0xd8d2U;
            const auto source_value = 0x4U;
            const auto destination_address = 0xc00U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xd8d2U: { // 6b22 bmi.b $d8f6
            next = 0xd8d4U;
            if ((r.status & 8U) != 0U) { next = 0xd8f6U; transfer_kind = 1U; }
            break;
        }
        case 0xd8d4U: { // 31fc00050c16 move.w #$5, $c16.w
            next = 0xd8daU;
            const auto value = 0x5U;
            const auto destination_address = 0xc16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd8daU: { // 426d000c clr.w $c(a5)
            next = 0xd8deU;
            const auto destination_address = r.address[5] + 0xcU;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd8deU: { // 426d0010 clr.w $10(a5)
            next = 0xd8e2U;
            const auto destination_address = r.address[5] + 0x10U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd8e2U: { // 426d0016 clr.w $16(a5)
            next = 0xd8e6U;
            const auto destination_address = r.address[5] + 0x16U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd8e6U: { // 48e70004 movem.l a5, -(a7)
            next = 0xd8eaU;
            const auto saved_13 = r.address[5];
            auto address = r.address[7];
            address -= 4U;
            m.word(address + 2U, saved_13);
            m.word(address, saved_13 >> 16U);
            r.address[7] = address;
            break;
        }
        case 0xd8eaU: { // 4eb90000a8f0 jsr $a8f0.l
            next = 0xd8f0U;
            const auto result = m.call(c, 616U, 0xd8eaU, 0xa8f0U, 0xd8f0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd8f0U: { // 4cdf2000 movem.l (a7)+, a5
            next = 0xd8f4U;
            const auto memory_address = r.address[7];
            auto address = memory_address;
            r.address[5] = m.lng(address);
            address += 4U;
            (void)m.word(address);
            r.address[7] = address;
            break;
        }
        case 0xd8f4U: { // 4e75 rts 
            next = 0xd8f6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd8f4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xd8f6U: { // 48e70004 movem.l a5, -(a7)
            next = 0xd8faU;
            const auto saved_13 = r.address[5];
            auto address = r.address[7];
            address -= 4U;
            m.word(address + 2U, saved_13);
            m.word(address, saved_13 >> 16U);
            r.address[7] = address;
            break;
        }
        case 0xd8faU: { // 4eb90000a618 jsr $a618.l
            next = 0xd900U;
            const auto result = m.call(c, 140U, 0xd8faU, 0xa618U, 0xd900U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd900U: { // 4cdf2000 movem.l (a7)+, a5
            next = 0xd904U;
            const auto memory_address = r.address[7];
            auto address = memory_address;
            r.address[5] = m.lng(address);
            address += 4U;
            (void)m.word(address);
            r.address[7] = address;
            break;
        }
        case 0xd904U: { // 42780c16 clr.w $c16.w
            next = 0xd908U;
            const auto destination_address = 0xc16U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd908U: { // 42780c10 clr.w $c10.w
            next = 0xd90cU;
            const auto destination_address = 0xc10U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd90cU: { // 4e75 rts 
            next = 0xd90eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd90cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xd90eU: { // 42780c16 clr.w $c16.w
            next = 0xd912U;
            const auto destination_address = 0xc16U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd912U: { // 4e75 rts 
            next = 0xd914U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd912U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xd914U: { // 4a780c10 tst.w $c10.w
            next = 0xd918U;
            const auto destination_address = 0xc10U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd918U: { // 670e beq.b $d928
            next = 0xd91aU;
            if ((r.status & 4U) != 0U) { next = 0xd928U; transfer_kind = 1U; }
            break;
        }
        case 0xd91aU: { // 42780c16 clr.w $c16.w
            next = 0xd91eU;
            const auto destination_address = 0xc16U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd91eU: { // 42787452 clr.w $7452.w
            next = 0xd922U;
            const auto destination_address = 0x7452U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd922U: { // 4278745c clr.w $745c.w
            next = 0xd926U;
            const auto destination_address = 0x745cU;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd926U: { // 4e75 rts 
            next = 0xd928U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd926U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xd928U: { // 4a380c06 tst.b $c06.w
            next = 0xd92cU;
            const auto destination_address = 0xc06U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0xd92cU: { // 6646 bne.b $d974
            next = 0xd92eU;
            if ((r.status & 4U) == 0U) { next = 0xd974U; transfer_kind = 1U; }
            break;
        }
        case 0xd92eU: { // 4a380c04 tst.b $c04.w
            next = 0xd932U;
            const auto destination_address = 0xc04U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0xd932U: { // 6640 bne.b $d974
            next = 0xd934U;
            if ((r.status & 4U) == 0U) { next = 0xd974U; transfer_kind = 1U; }
            break;
        }
        case 0xd934U: { // 536d0018 subq.w #$1, $18(a5)
            next = 0xd938U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x18U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xd938U: { // 6e3a bgt.b $d974
            next = 0xd93aU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0xd974U; transfer_kind = 1U; }
            break;
        }
        case 0xd93aU: { // 536d001a subq.w #$1, $1a(a5)
            next = 0xd93eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xd93eU: { // 6b0c bmi.b $d94c
            next = 0xd940U;
            if ((r.status & 8U) != 0U) { next = 0xd94cU; transfer_kind = 1U; }
            break;
        }
        case 0xd940U: { // 3b7c001e0018 move.w #$1e, $18(a5)
            next = 0xd946U;
            m.word(r.address[5] + 0x18U, 0x1eU);
            m.logic(0x1eU, 16U);
            break;
        }
        case 0xd946U: { // 61000538 bsr.w $de80
            next = 0xd94aU;
            const auto result = m.call(c, 168U, 0xd946U, 0xde80U, 0xd94aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd94aU: { // 6028 bra.b $d974
            next = 0xd94cU;
            if (true) { next = 0xd974U; transfer_kind = 1U; }
            break;
        }
        case 0xd94cU: { // 4a780c00 tst.w $c00.w
            next = 0xd950U;
            const auto destination_address = 0xc00U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd950U: { // 670a beq.b $d95c
            next = 0xd952U;
            if ((r.status & 4U) != 0U) { next = 0xd95cU; transfer_kind = 1U; }
            break;
        }
        case 0xd952U: { // 31fc00018002 move.w #$1, $8002.w
            next = 0xd958U;
            const auto value = 0x1U;
            const auto destination_address = 0xffff8002U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd958U: { // 42780c00 clr.w $c00.w
            next = 0xd95cU;
            const auto destination_address = 0xc00U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd95cU: { // 42780c02 clr.w $c02.w
            next = 0xd960U;
            const auto destination_address = 0xc02U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd960U: { // 42780820 clr.w $820.w
            next = 0xd964U;
            const auto destination_address = 0x820U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd964U: { // 30380836 move.w $836.w, d0
            next = 0xd968U;
            const auto source_address = 0x836U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd968U: { // 5940 subq.w #$4, d0
            next = 0xd96aU;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd96aU: { // 61000372 bsr.w $dcde
            next = 0xd96eU;
            const auto result = m.call(c, 160U, 0xd96aU, 0xdcdeU, 0xd96eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd96eU: { // 4ef900009c8e jmp $9c8e.l
            next = 0xd974U;
            next = 0x9c8eU;
            transfer_kind = 1U;
            break;
        }
        case 0xd974U: { // 4e75 rts 
            next = 0xd976U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd974U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xd976U: { // 526d0016 addq.w #$1, $16(a5)
            next = 0xd97aU;
            const auto address = r.address[5] + 0x16U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xd97aU: { // 302d0010 move.w $10(a5), d0
            next = 0xd97eU;
            m.dw(0U, m.word(r.address[5] + 0x10U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xd97eU: { // 02400007 andi.w #$7, d0
            next = 0xd982U;
            const auto source_value = 0x7U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd982U: { // e540 asl.w #$2, d0
            next = 0xd984U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xd984U: { // 41f900025a14 lea.l $25a14.l, a0
            next = 0xd98aU;
            r.address[0] = 0x25a14U;
            break;
        }
        case 0xd98aU: { // 20300000 move.l (a0, d0.w), d0
            next = 0xd98eU;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0xd98eU: { // 4eb900015e4e jsr $15e4e.l
            next = 0xd994U;
            const auto result = m.call(c, 284U, 0xd98eU, 0x15e4eU, 0xd994U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd994U: { // 526d0010 addq.w #$1, $10(a5)
            next = 0xd998U;
            const auto address = r.address[5] + 0x10U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xd998U: { // 0c6d02940010 cmpi.w #$294, $10(a5)
            next = 0xd99eU;
            const auto source_value = 0x294U;
            const auto destination_address = r.address[5] + 0x10U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xd99eU: { // 6a4a bpl.b $d9ea
            next = 0xd9a0U;
            if ((r.status & 8U) == 0U) { next = 0xd9eaU; transfer_kind = 1U; }
            break;
        }
        case 0xd9a0U: { // 302d0010 move.w $10(a5), d0
            next = 0xd9a4U;
            m.dw(0U, m.word(r.address[5] + 0x10U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xd9a4U: { // 0c400240 cmpi.w #$240, d0
            next = 0xd9a8U;
            const auto source_value = 0x240U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xd9a8U: { // 6a14 bpl.b $d9be
            next = 0xd9aaU;
            if ((r.status & 8U) == 0U) { next = 0xd9beU; transfer_kind = 1U; }
            break;
        }
        case 0xd9aaU: { // 43f900025a34 lea.l $25a34.l, a1
            next = 0xd9b0U;
            r.address[1] = 0x25a34U;
            break;
        }
        case 0xd9b0U: { // 02400001 andi.w #$1, d0
            next = 0xd9b4U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd9b4U: { // e540 asl.w #$2, d0
            next = 0xd9b6U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xd9b6U: { // 22710000 movea.l (a1, d0.w), a1
            next = 0xd9baU;
            const auto source_address = r.address[1] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.lng(source_address);
            r.address[1] = value;
            break;
        }
        case 0xd9baU: { // 7407 moveq #$7, d2
            next = 0xd9bcU;
            r.data[2] = 0x7U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0xd9bcU: { // 601a bra.b $d9d8
            next = 0xd9beU;
            if (true) { next = 0xd9d8U; transfer_kind = 1U; }
            break;
        }
        case 0xd9beU: { // 04400240 subi.w #$240, d0
            next = 0xd9c2U;
            const auto source_value = 0x240U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd9c2U: { // 0c400020 cmpi.w #$20, d0
            next = 0xd9c6U;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xd9c6U: { // 6a20 bpl.b $d9e8
            next = 0xd9c8U;
            if ((r.status & 8U) == 0U) { next = 0xd9e8U; transfer_kind = 1U; }
            break;
        }
        case 0xd9c8U: { // 43f900025a3c lea.l $25a3c.l, a1
            next = 0xd9ceU;
            r.address[1] = 0x25a3cU;
            break;
        }
        case 0xd9ceU: { // 0240001c andi.w #$1c, d0
            next = 0xd9d2U;
            const auto source_value = 0x1cU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd9d2U: { // 22710000 movea.l (a1, d0.w), a1
            next = 0xd9d6U;
            const auto source_address = r.address[1] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.lng(source_address);
            r.address[1] = value;
            break;
        }
        case 0xd9d6U: { // 740f moveq #$f, d2
            next = 0xd9d8U;
            r.data[2] = 0xfU;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0xd9d8U: { // 2019 move.l (a1)+, d0
            next = 0xd9daU;
            const auto source_address = r.address[1];
            const auto value = m.lng(source_address);
            r.address[1] += 4U;
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0xd9daU: { // 4eb900015e4e jsr $15e4e.l
            next = 0xd9e0U;
            const auto result = m.call(c, 284U, 0xd9daU, 0x15e4eU, 0xd9e0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd9e0U: { // 51cafff6 dbra d2, $d9d8
            next = 0xd9e4U;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0xd9d8U;
            break;
        }
        case 0xd9e4U: { // 6100052a bsr.w $df10
            next = 0xd9e8U;
            const auto result = m.call(c, 636U, 0xd9e4U, 0xdf10U, 0xd9e8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xd9e8U: { // 4e75 rts 
            next = 0xd9eaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd9e8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xd9eaU: { // 4a380c04 tst.b $c04.w
            next = 0xd9eeU;
            const auto destination_address = 0xc04U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0xd9eeU: { // 661e bne.b $da0e
            next = 0xd9f0U;
            if ((r.status & 4U) == 0U) { next = 0xda0eU; transfer_kind = 1U; }
            break;
        }
        case 0xd9f0U: { // 31fc00060c16 move.w #$6, $c16.w
            next = 0xd9f6U;
            const auto value = 0x6U;
            const auto destination_address = 0xc16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd9f6U: { // 3b7c0001000c move.w #$1, $c(a5)
            next = 0xd9fcU;
            m.word(r.address[5] + 0xcU, 0x1U);
            m.logic(0x1U, 16U);
            break;
        }
        case 0xd9fcU: { // 426d0010 clr.w $10(a5)
            next = 0xda00U;
            const auto destination_address = r.address[5] + 0x10U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xda00U: { // 426d0012 clr.w $12(a5)
            next = 0xda04U;
            const auto destination_address = r.address[5] + 0x12U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xda04U: { // 426d0014 clr.w $14(a5)
            next = 0xda08U;
            const auto destination_address = r.address[5] + 0x14U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xda08U: { // 4eb90000b684 jsr $b684.l
            next = 0xda0eU;
            const auto result = m.call(c, 628U, 0xda08U, 0xb684U, 0xda0eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xda0eU: { // 4e75 rts 
            next = 0xda10U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xda0eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xda10U: { // 526d0016 addq.w #$1, $16(a5)
            next = 0xda14U;
            const auto address = r.address[5] + 0x16U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xda14U: { // 0c6d08680016 cmpi.w #$868, $16(a5)
            next = 0xda1aU;
            const auto source_value = 0x868U;
            const auto destination_address = r.address[5] + 0x16U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xda1aU: { // 6606 bne.b $da22
            next = 0xda1cU;
            if ((r.status & 4U) == 0U) { next = 0xda22U; transfer_kind = 1U; }
            break;
        }
        case 0xda1cU: { // 4eb900017054 jsr $17054.l
            next = 0xda22U;
            const auto result = m.call(c, 311U, 0xda1cU, 0x17054U, 0xda22U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xda22U: { // 6100053e bsr.w $df62
            next = 0xda26U;
            const auto result = m.call(c, 637U, 0xda22U, 0xdf62U, 0xda26U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xda26U: { // 526d0010 addq.w #$1, $10(a5)
            next = 0xda2aU;
            const auto address = r.address[5] + 0x10U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xda2aU: { // 302d000c move.w $c(a5), d0
            next = 0xda2eU;
            m.dw(0U, m.word(r.address[5] + 0xcU));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xda2eU: { // d040 add.w d0, d0
            next = 0xda30U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xda30U: { // 41fa065e lea.l $e090(pc), a0
            next = 0xda34U;
            const auto source_address = 0xe090U;
            r.address[0] = source_address;
            break;
        }
        case 0xda34U: { // 30300000 move.w (a0, d0.w), d0
            next = 0xda38U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xda38U: { // b06d0010 cmp.w $10(a5), d0
            next = 0xda3cU;
            const auto source_address = r.address[5] + 0x10U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xda3cU: { // 6e18 bgt.b $da56
            next = 0xda3eU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0xda56U; transfer_kind = 1U; }
            break;
        }
        case 0xda3eU: { // 526d000c addq.w #$1, $c(a5)
            next = 0xda42U;
            const auto address = r.address[5] + 0xcU;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xda42U: { // 0c6d000f000c cmpi.w #$f, $c(a5)
            next = 0xda48U;
            const auto source_value = 0xfU;
            const auto destination_address = r.address[5] + 0xcU;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xda48U: { // 6a000104 bpl.w $db4e
            next = 0xda4cU;
            if ((r.status & 8U) == 0U) { next = 0xdb4eU; transfer_kind = 1U; }
            break;
        }
        case 0xda4cU: { // 426d0010 clr.w $10(a5)
            next = 0xda50U;
            const auto destination_address = r.address[5] + 0x10U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xda50U: { // 4eb90000b6c8 jsr $b6c8.l
            next = 0xda56U;
            const auto result = m.call(c, 629U, 0xda50U, 0xb6c8U, 0xda56U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xda56U: { // 302d000c move.w $c(a5), d0
            next = 0xda5aU;
            m.dw(0U, m.word(r.address[5] + 0xcU));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xda5aU: { // 5340 subq.w #$1, d0
            next = 0xda5cU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xda5cU: { // e540 asl.w #$2, d0
            next = 0xda5eU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xda5eU: { // 4efb0002 jmp $da62(pc, d0.w)
            next = 0xda62U;
            next = (0xda62U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0xda62U: { // 60000036 bra.w $da9a
            next = 0xda66U;
            if (true) { next = 0xda9aU; transfer_kind = 1U; }
            break;
        }
        case 0xda66U: { // 60000072 bra.w $dada
            next = 0xda6aU;
            if (true) { next = 0xdadaU; transfer_kind = 1U; }
            break;
        }
        case 0xda6aU: { // 60000070 bra.w $dadc
            next = 0xda6eU;
            if (true) { next = 0xdadcU; transfer_kind = 1U; }
            break;
        }
        case 0xda6eU: { // 60000078 bra.w $dae8
            next = 0xda72U;
            if (true) { next = 0xdae8U; transfer_kind = 1U; }
            break;
        }
        case 0xda72U: { // 6000009e bra.w $db12
            next = 0xda76U;
            if (true) { next = 0xdb12U; transfer_kind = 1U; }
            break;
        }
        case 0xda76U: { // 600000a4 bra.w $db1c
            next = 0xda7aU;
            if (true) { next = 0xdb1cU; transfer_kind = 1U; }
            break;
        }
        case 0xda7aU: { // 600000ac bra.w $db28
            next = 0xda7eU;
            if (true) { next = 0xdb28U; transfer_kind = 1U; }
            break;
        }
        case 0xda7eU: { // 600000b4 bra.w $db34
            next = 0xda82U;
            if (true) { next = 0xdb34U; transfer_kind = 1U; }
            break;
        }
        case 0xda82U: { // 600000ba bra.w $db3e
            next = 0xda86U;
            if (true) { next = 0xdb3eU; transfer_kind = 1U; }
            break;
        }
        case 0xda86U: { // 600000b6 bra.w $db3e
            next = 0xda8aU;
            if (true) { next = 0xdb3eU; transfer_kind = 1U; }
            break;
        }
        case 0xda8aU: { // 600000b4 bra.w $db40
            next = 0xda8eU;
            if (true) { next = 0xdb40U; transfer_kind = 1U; }
            break;
        }
        case 0xda8eU: { // 600000bc bra.w $db4c
            next = 0xda92U;
            if (true) { next = 0xdb4cU; transfer_kind = 1U; }
            break;
        }
        case 0xda92U: { // 600000b8 bra.w $db4c
            next = 0xda96U;
            if (true) { next = 0xdb4cU; transfer_kind = 1U; }
            break;
        }
        case 0xda96U: { // 600000b4 bra.w $db4c
            next = 0xda9aU;
            if (true) { next = 0xdb4cU; transfer_kind = 1U; }
            break;
        }
        case 0xda9aU: { // 302d0010 move.w $10(a5), d0
            next = 0xda9eU;
            m.dw(0U, m.word(r.address[5] + 0x10U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xda9eU: { // 3200 move.w d0, d1
            next = 0xdaa0U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xdaa0U: { // 0241000e andi.w #$e, d1
            next = 0xdaa4U;
            const auto source_value = 0xeU;
            const auto destination_value = r.data[1];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0xdaa4U: { // d241 add.w d1, d1
            next = 0xdaa6U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0xdaa6U: { // 45fa0706 lea.l $e1ae(pc), a2
            next = 0xdaaaU;
            const auto source_address = 0xe1aeU;
            r.address[2] = source_address;
            break;
        }
        case 0xdaaaU: { // d4c1 adda.w d1, a2
            next = 0xdaacU;
            const auto source_value = r.data[1];
            r.address[2] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xdaacU: { // 02400001 andi.w #$1, d0
            next = 0xdab0U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xdab0U: { // 6704 beq.b $dab6
            next = 0xdab2U;
            if ((r.status & 4U) != 0U) { next = 0xdab6U; transfer_kind = 1U; }
            break;
        }
        case 0xdab2U: { // 303c0d97 move.w #$d97, d0
            next = 0xdab6U;
            m.dw(0U, 0xd97U);
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xdab6U: { // 41f900204000 lea.l $204000.l, a0
            next = 0xdabcU;
            r.address[0] = 0x204000U;
            break;
        }
        case 0xdabcU: { // d0da adda.w (a2)+, a0
            next = 0xdabeU;
            const auto source_address = r.address[2];
            const auto source_value = m.word(source_address);
            r.address[2] += 2U;
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xdabeU: { // 7600 moveq #$0, d3
            next = 0xdac0U;
            r.data[3] = 0x0U;
            m.logic(r.data[3], 32U);
            break;
        }
        case 0xdac0U: { // 7400 moveq #$0, d2
            next = 0xdac2U;
            r.data[2] = 0x0U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0xdac2U: { // 161a move.b (a2)+, d3
            next = 0xdac4U;
            const auto source_address = r.address[2];
            const auto value = m.byte(source_address);
            r.address[2] += 1U;
            m.db(3U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xdac4U: { // 141a move.b (a2)+, d2
            next = 0xdac6U;
            const auto source_address = r.address[2];
            const auto value = m.byte(source_address);
            r.address[2] += 1U;
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xdac6U: { // 43d0 lea.l (a0), a1
            next = 0xdac8U;
            const auto source_address = r.address[0];
            r.address[1] = source_address;
            break;
        }
        case 0xdac8U: { // 3203 move.w d3, d1
            next = 0xdacaU;
            const auto value = r.data[3];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xdacaU: { // 32c0 move.w d0, (a1)+
            next = 0xdaccU;
            const auto value = r.data[0];
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            r.address[1] += 2U;
            m.logic(value, 16U);
            break;
        }
        case 0xdaccU: { // 51c9fffc dbra d1, $daca
            next = 0xdad0U;
            m.dw(1U, r.data[1] - 1U);
            if ((r.data[1] & 0xffffU) != 0xffffU) next = 0xdacaU;
            break;
        }
        case 0xdad0U: { // 41e80080 lea.l $80(a0), a0
            next = 0xdad4U;
            r.address[0] = r.address[0] + 0x80U;
            break;
        }
        case 0xdad4U: { // 51cafff0 dbra d2, $dac6
            next = 0xdad8U;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0xdac6U;
            break;
        }
        case 0xdad8U: { // 4e75 rts 
            next = 0xdadaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xdad8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xdadaU: { // 4e75 rts 
            next = 0xdadcU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xdadaU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xdadcU: { // 43f900025c9c lea.l $25c9c.l, a1
            next = 0xdae2U;
            r.address[1] = 0x25c9cU;
            break;
        }
        case 0xdae2U: { // 7202 moveq #$2, d1
            next = 0xdae4U;
            r.data[1] = 0x2U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xdae4U: { // 6000000a bra.w $daf0
            next = 0xdae8U;
            if (true) { next = 0xdaf0U; transfer_kind = 1U; }
            break;
        }
        case 0xdae8U: { // 43f900025ca4 lea.l $25ca4.l, a1
            next = 0xdaeeU;
            r.address[1] = 0x25ca4U;
            break;
        }
        case 0xdaeeU: { // 7203 moveq #$3, d1
            next = 0xdaf0U;
            r.data[1] = 0x3U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xdaf0U: { // 7000 moveq #$0, d0
            next = 0xdaf2U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xdaf2U: { // 302d0010 move.w $10(a5), d0
            next = 0xdaf6U;
            m.dw(0U, m.word(r.address[5] + 0x10U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xdaf6U: { // 80c1 divu.w d1, d0
            next = 0xdaf8U;
            const auto divisor = r.data[1];
            if ((divisor & 0xffffU) == 0U) {
                const auto result = m.exception(c, 5U, 0xdaf6U, 0xdaf8U);
                if (result.status != TranslationStatus::complete || result.control != 2U) return result;
                next = r.program_counter;
            } else {
                m.divide(0U, divisor, false);
            }
            break;
        }
        case 0xdaf8U: { // 4840 swap d0
            next = 0xdafaU;
            const auto value = (r.data[0] << 16U) | (r.data[0] >> 16U);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0xdafaU: { // e540 asl.w #$2, d0
            next = 0xdafcU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xdafcU: { // 22710000 movea.l (a1, d0.w), a1
            next = 0xdb00U;
            const auto source_address = r.address[1] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.lng(source_address);
            r.address[1] = value;
            break;
        }
        case 0xdb00U: { // 2019 move.l (a1)+, d0
            next = 0xdb02U;
            const auto source_address = r.address[1];
            const auto value = m.lng(source_address);
            r.address[1] += 4U;
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0xdb02U: { // 4eb900015e4e jsr $15e4e.l
            next = 0xdb08U;
            const auto result = m.call(c, 284U, 0xdb02U, 0x15e4eU, 0xdb08U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xdb08U: { // 2019 move.l (a1)+, d0
            next = 0xdb0aU;
            const auto source_address = r.address[1];
            const auto value = m.lng(source_address);
            r.address[1] += 4U;
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0xdb0aU: { // 4eb900015e4e jsr $15e4e.l
            next = 0xdb10U;
            const auto result = m.call(c, 284U, 0xdb0aU, 0x15e4eU, 0xdb10U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xdb10U: { // 4e75 rts 
            next = 0xdb12U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xdb10U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xdb12U: { // 43f900025cb0 lea.l $25cb0.l, a1
            next = 0xdb18U;
            r.address[1] = 0x25cb0U;
            break;
        }
        case 0xdb18U: { // 6000ffd4 bra.w $daee
            next = 0xdb1cU;
            if (true) { next = 0xdaeeU; transfer_kind = 1U; }
            break;
        }
        case 0xdb1cU: { // 43f900025cbc lea.l $25cbc.l, a1
            next = 0xdb22U;
            r.address[1] = 0x25cbcU;
            break;
        }
        case 0xdb22U: { // 7206 moveq #$6, d1
            next = 0xdb24U;
            r.data[1] = 0x6U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xdb24U: { // 6000ffca bra.w $daf0
            next = 0xdb28U;
            if (true) { next = 0xdaf0U; transfer_kind = 1U; }
            break;
        }
        case 0xdb28U: { // 43f900025cd4 lea.l $25cd4.l, a1
            next = 0xdb2eU;
            r.address[1] = 0x25cd4U;
            break;
        }
        case 0xdb2eU: { // 7206 moveq #$6, d1
            next = 0xdb30U;
            r.data[1] = 0x6U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xdb30U: { // 6000ffbe bra.w $daf0
            next = 0xdb34U;
            if (true) { next = 0xdaf0U; transfer_kind = 1U; }
            break;
        }
        case 0xdb34U: { // 43f900025cb0 lea.l $25cb0.l, a1
            next = 0xdb3aU;
            r.address[1] = 0x25cb0U;
            break;
        }
        case 0xdb3aU: { // 6000ffb2 bra.w $daee
            next = 0xdb3eU;
            if (true) { next = 0xdaeeU; transfer_kind = 1U; }
            break;
        }
        case 0xdb3eU: { // 4e75 rts 
            next = 0xdb40U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xdb3eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xdb40U: { // 43f900025cbc lea.l $25cbc.l, a1
            next = 0xdb46U;
            r.address[1] = 0x25cbcU;
            break;
        }
        case 0xdb46U: { // 7206 moveq #$6, d1
            next = 0xdb48U;
            r.data[1] = 0x6U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xdb48U: { // 6000ffa6 bra.w $daf0
            next = 0xdb4cU;
            if (true) { next = 0xdaf0U; transfer_kind = 1U; }
            break;
        }
        case 0xdb4cU: { // 4e75 rts 
            next = 0xdb4eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xdb4cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xdb4eU: { // 31fc00070c16 move.w #$7, $c16.w
            next = 0xdb54U;
            const auto value = 0x7U;
            const auto destination_address = 0xc16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xdb54U: { // 426d0010 clr.w $10(a5)
            next = 0xdb58U;
            const auto destination_address = r.address[5] + 0x10U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xdb58U: { // 526d0016 addq.w #$1, $16(a5)
            next = 0xdb5cU;
            const auto address = r.address[5] + 0x16U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xdb5cU: { // 0c6d08680016 cmpi.w #$868, $16(a5)
            next = 0xdb62U;
            const auto source_value = 0x868U;
            const auto destination_address = r.address[5] + 0x16U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xdb62U: { // 6606 bne.b $db6a
            next = 0xdb64U;
            if ((r.status & 4U) == 0U) { next = 0xdb6aU; transfer_kind = 1U; }
            break;
        }
        case 0xdb64U: { // 4eb900017054 jsr $17054.l
            next = 0xdb6aU;
            const auto result = m.call(c, 311U, 0xdb64U, 0x17054U, 0xdb6aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xdb6aU: { // 526d0010 addq.w #$1, $10(a5)
            next = 0xdb6eU;
            const auto address = r.address[5] + 0x10U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xdb6eU: { // 0c6d00960010 cmpi.w #$96, $10(a5)
            next = 0xdb74U;
            const auto source_value = 0x96U;
            const auto destination_address = r.address[5] + 0x10U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xdb74U: { // 6b28 bmi.b $db9e
            next = 0xdb76U;
            if ((r.status & 8U) != 0U) { next = 0xdb9eU; transfer_kind = 1U; }
            break;
        }
        case 0xdb76U: { // 31fc00018002 move.w #$1, $8002.w
            next = 0xdb7cU;
            const auto value = 0x1U;
            const auto destination_address = 0xffff8002U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xdb7cU: { // 42780c00 clr.w $c00.w
            next = 0xdb80U;
            const auto destination_address = 0xc00U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xdb80U: { // 42780c02 clr.w $c02.w
            next = 0xdb84U;
            const auto destination_address = 0xc02U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xdb84U: { // 4eb90001826e jsr $1826e.l
            next = 0xdb8aU;
            const auto result = m.call(c, 322U, 0xdb84U, 0x1826eU, 0xdb8aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xdb8aU: { // 42780820 clr.w $820.w
            next = 0xdb8eU;
            const auto destination_address = 0x820U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xdb8eU: { // 30380836 move.w $836.w, d0
            next = 0xdb92U;
            const auto source_address = 0x836U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xdb92U: { // 5940 subq.w #$4, d0
            next = 0xdb94U;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xdb94U: { // 61000148 bsr.w $dcde
            next = 0xdb98U;
            const auto result = m.call(c, 160U, 0xdb94U, 0xdcdeU, 0xdb98U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xdb98U: { // 4ef900009c8e jmp $9c8e.l
            next = 0xdb9eU;
            next = 0x9c8eU;
            transfer_kind = 1U;
            break;
        }
        case 0xdb9eU: { // 4e75 rts 
            next = 0xdba0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xdb9eU, r.program_counter)) return *event;
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
        case 0xd734U:
        case 0xd738U:
        case 0xd73cU:
        case 0xd740U:
        case 0xd744U:
        case 0xd746U:
        case 0xd74aU:
        case 0xd74eU:
        case 0xd752U:
        case 0xd756U:
        case 0xd75aU:
        case 0xd75eU:
        case 0xd762U:
        case 0xd766U:
        case 0xd76aU:
        case 0xd76eU:
        case 0xd772U:
        case 0xd776U:
        case 0xd77aU:
        case 0xd77cU:
        case 0xd780U:
        case 0xd784U:
        case 0xd786U:
        case 0xd78cU:
        case 0xd78eU:
        case 0xd794U:
        case 0xd79aU:
        case 0xd7a0U:
        case 0xd7a4U:
        case 0xd7a6U:
        case 0xd7aaU:
        case 0xd7aeU:
        case 0xd7b2U:
        case 0xd7b6U:
        case 0xd7baU:
        case 0xd7bcU:
        case 0xd7c2U:
        case 0xd7c6U:
        case 0xd7caU:
        case 0xd7ceU:
        case 0xd7d2U:
        case 0xd7d4U:
        case 0xd7d8U:
        case 0xd7deU:
        case 0xd7e4U:
        case 0xd7e6U:
        case 0xd7eaU:
        case 0xd7ecU:
        case 0xd7f0U:
        case 0xd7f2U:
        case 0xd7f8U:
        case 0xd7fcU:
        case 0xd800U:
        case 0xd802U:
        case 0xd806U:
        case 0xd80aU:
        case 0xd80eU:
        case 0xd810U:
        case 0xd812U:
        case 0xd816U:
        case 0xd81aU:
        case 0xd81cU:
        case 0xd820U:
        case 0xd822U:
        case 0xd828U:
        case 0xd82cU:
        case 0xd82eU:
        case 0xd830U:
        case 0xd832U:
        case 0xd834U:
        case 0xd838U:
        case 0xd83cU:
        case 0xd840U:
        case 0xd844U:
        case 0xd846U:
        case 0xd84aU:
        case 0xd850U:
        case 0xd852U:
        case 0xd858U:
        case 0xd85eU:
        case 0xd862U:
        case 0xd866U:
        case 0xd86aU:
        case 0xd86cU:
        case 0xd86eU:
        case 0xd872U:
        case 0xd874U:
        case 0xd876U:
        case 0xd878U:
        case 0xd87cU:
        case 0xd880U:
        case 0xd882U:
        case 0xd886U:
        case 0xd888U:
        case 0xd88cU:
        case 0xd892U:
        case 0xd894U:
        case 0xd896U:
        case 0xd89aU:
        case 0xd8a0U:
        case 0xd8a2U:
        case 0xd8a6U:
        case 0xd8a8U:
        case 0xd8aaU:
        case 0xd8aeU:
        case 0xd8b2U:
        case 0xd8b4U:
        case 0xd8b8U:
        case 0xd8baU:
        case 0xd8beU:
        case 0xd8c0U:
        case 0xd8c4U:
        case 0xd8c6U:
        case 0xd8caU:
        case 0xd8ccU:
        case 0xd8d2U:
        case 0xd8d4U:
        case 0xd8daU:
        case 0xd8deU:
        case 0xd8e2U:
        case 0xd8e6U:
        case 0xd8eaU:
        case 0xd8f0U:
        case 0xd8f4U:
        case 0xd8f6U:
        case 0xd8faU:
        case 0xd900U:
        case 0xd904U:
        case 0xd908U:
        case 0xd90cU:
        case 0xd90eU:
        case 0xd912U:
        case 0xd914U:
        case 0xd918U:
        case 0xd91aU:
        case 0xd91eU:
        case 0xd922U:
        case 0xd926U:
        case 0xd928U:
        case 0xd92cU:
        case 0xd92eU:
        case 0xd932U:
        case 0xd934U:
        case 0xd938U:
        case 0xd93aU:
        case 0xd93eU:
        case 0xd940U:
        case 0xd946U:
        case 0xd94aU:
        case 0xd94cU:
        case 0xd950U:
        case 0xd952U:
        case 0xd958U:
        case 0xd95cU:
        case 0xd960U:
        case 0xd964U:
        case 0xd968U:
        case 0xd96aU:
        case 0xd96eU:
        case 0xd974U:
        case 0xd976U:
        case 0xd97aU:
        case 0xd97eU:
        case 0xd982U:
        case 0xd984U:
        case 0xd98aU:
        case 0xd98eU:
        case 0xd994U:
        case 0xd998U:
        case 0xd99eU:
        case 0xd9a0U:
        case 0xd9a4U:
        case 0xd9a8U:
        case 0xd9aaU:
        case 0xd9b0U:
        case 0xd9b4U:
        case 0xd9b6U:
        case 0xd9baU:
        case 0xd9bcU:
        case 0xd9beU:
        case 0xd9c2U:
        case 0xd9c6U:
        case 0xd9c8U:
        case 0xd9ceU:
        case 0xd9d2U:
        case 0xd9d6U:
        case 0xd9d8U:
        case 0xd9daU:
        case 0xd9e0U:
        case 0xd9e4U:
        case 0xd9e8U:
        case 0xd9eaU:
        case 0xd9eeU:
        case 0xd9f0U:
        case 0xd9f6U:
        case 0xd9fcU:
        case 0xda00U:
        case 0xda04U:
        case 0xda08U:
        case 0xda0eU:
        case 0xda10U:
        case 0xda14U:
        case 0xda1aU:
        case 0xda1cU:
        case 0xda22U:
        case 0xda26U:
        case 0xda2aU:
        case 0xda2eU:
        case 0xda30U:
        case 0xda34U:
        case 0xda38U:
        case 0xda3cU:
        case 0xda3eU:
        case 0xda42U:
        case 0xda48U:
        case 0xda4cU:
        case 0xda50U:
        case 0xda56U:
        case 0xda5aU:
        case 0xda5cU:
        case 0xda5eU:
        case 0xda62U:
        case 0xda66U:
        case 0xda6aU:
        case 0xda6eU:
        case 0xda72U:
        case 0xda76U:
        case 0xda7aU:
        case 0xda7eU:
        case 0xda82U:
        case 0xda86U:
        case 0xda8aU:
        case 0xda8eU:
        case 0xda92U:
        case 0xda96U:
        case 0xda9aU:
        case 0xda9eU:
        case 0xdaa0U:
        case 0xdaa4U:
        case 0xdaa6U:
        case 0xdaaaU:
        case 0xdaacU:
        case 0xdab0U:
        case 0xdab2U:
        case 0xdab6U:
        case 0xdabcU:
        case 0xdabeU:
        case 0xdac0U:
        case 0xdac2U:
        case 0xdac4U:
        case 0xdac6U:
        case 0xdac8U:
        case 0xdacaU:
        case 0xdaccU:
        case 0xdad0U:
        case 0xdad4U:
        case 0xdad8U:
        case 0xdadaU:
        case 0xdadcU:
        case 0xdae2U:
        case 0xdae4U:
        case 0xdae8U:
        case 0xdaeeU:
        case 0xdaf0U:
        case 0xdaf2U:
        case 0xdaf6U:
        case 0xdaf8U:
        case 0xdafaU:
        case 0xdafcU:
        case 0xdb00U:
        case 0xdb02U:
        case 0xdb08U:
        case 0xdb0aU:
        case 0xdb10U:
        case 0xdb12U:
        case 0xdb18U:
        case 0xdb1cU:
        case 0xdb22U:
        case 0xdb24U:
        case 0xdb28U:
        case 0xdb2eU:
        case 0xdb30U:
        case 0xdb34U:
        case 0xdb3aU:
        case 0xdb3eU:
        case 0xdb40U:
        case 0xdb46U:
        case 0xdb48U:
        case 0xdb4cU:
        case 0xdb4eU:
        case 0xdb54U:
        case 0xdb58U:
        case 0xdb5cU:
        case 0xdb62U:
        case 0xdb64U:
        case 0xdb6aU:
        case 0xdb6eU:
        case 0xdb74U:
        case 0xdb76U:
        case 0xdb7cU:
        case 0xdb80U:
        case 0xdb84U:
        case 0xdb8aU:
        case 0xdb8eU:
        case 0xdb92U:
        case 0xdb94U:
        case 0xdb98U:
        case 0xdb9eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
