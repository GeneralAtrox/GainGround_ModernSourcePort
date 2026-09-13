// Implemented but unverified. Retained F179 disassembly and state-72 opcode
// bytes supply the full selector table, including the two return-only modes.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult cpu_b_dispatch_control_counter(FunctionContext &c) noexcept
{
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xf406U: // 30380c16 MOVE.W $c16.w,D0
            m.dw(0U, m.word(0xc16U)); m.logic(r.data[0], 16U);
            next = 0xf40aU; break;
        case 0xf40aU: // e540 ASL.W #2,D0
            m.asl_word(0U, 2U); next = 0xf40cU; break;
        case 0xf40cU: // 4efb0002 JMP $f410(PC,D0.W)
            next = (0xf410U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U; break;
        case 0xf410U: next = 0xf428U; transfer_kind = 1U; break; // 60000016
        case 0xf414U: next = 0xf43aU; transfer_kind = 1U; break; // 60000024
        case 0xf418U: next = 0xf4d8U; transfer_kind = 1U; break; // 600000be
        case 0xf41cU: next = 0xf4daU; transfer_kind = 1U; break; // 600000bc
        case 0xf420U: next = 0xf516U; transfer_kind = 1U; break; // 600000f4
        case 0xf424U: next = 0xf518U; transfer_kind = 1U; break; // 600000f2
        case 0xf428U: // 0c6d000a0044 CMPI.W #10,$44(A5)
            m.sub(m.word(r.address[5] + 0x44U), 10U, 16U, true);
            next = 0xf42eU; break;
        case 0xf42eU: // 6608 BNE $f438
            next = (r.status & 4U) ? 0xf430U : 0xf438U; break;
        case 0xf430U: // 426d0044 CLR.W $44(A5)
            (void)m.word(r.address[5] + 0x44U); m.word(r.address[5] + 0x44U, 0U);
            m.logic(0U, 16U); next = 0xf434U; break;
        case 0xf434U: { // 6100096a BSR $fda0
            const auto child = m.call(c, 470U, pc, 0xfda0U, 0xf438U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            next = r.program_counter; break;
        }
        case 0xf43aU: // 302d0044 MOVE.W $44(A5),D0
            m.dw(0U, m.word(r.address[5] + 0x44U)); m.logic(r.data[0], 16U);
            next = 0xf43eU; break;
        case 0xf43eU: // 0c400009 CMPI.W #9,D0
            m.sub(r.data[0], 9U, 16U, true); next = 0xf442U; break;
        case 0xf442U: // 64000092 BCC $f4d6
            next = (r.status & 1U) ? 0xf446U : 0xf4d6U; break;
        case 0xf446U: // 0c400004 CMPI.W #4,D0
            m.sub(r.data[0], 4U, 16U, true); next = 0xf44aU; break;
        case 0xf44aU: // 6706 BEQ $f452
            next = (r.status & 4U) ? 0xf452U : 0xf44cU; break;
        case 0xf44cU: // 0c400005 CMPI.W #5,D0
            m.sub(r.data[0], 5U, 16U, true); next = 0xf450U; break;
        case 0xf450U: // 6606 BNE $f458
            next = (r.status & 4U) ? 0xf452U : 0xf458U; break;
        case 0xf452U: { // 610008bc BSR $fd10
            const auto child = m.call(c, 197U, pc, 0xfd10U, 0xf456U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            next = r.program_counter; break;
        }
        case 0xf456U: next = 0xf46cU; transfer_kind = 1U; break; // 6014
        case 0xf458U: // 102c0000 MOVE.B (A4),D0
            m.db(0U, m.byte(r.address[4])); m.logic(r.data[0], 8U);
            next = 0xf45cU; break;
        case 0xf45cU: // 802c0040 OR.B $40(A4),D0
            m.db(0U, r.data[0] | m.byte(r.address[4] + 0x40U));
            m.logic(r.data[0], 8U); next = 0xf460U; break;
        case 0xf460U: // 67000074 BEQ $f4d6
            next = (r.status & 4U) ? 0xf4d6U : 0xf464U; break;
        case 0xf464U: // 0c6d00080044 CMPI.W #8,$44(A5)
            m.sub(m.word(r.address[5] + 0x44U), 8U, 16U, true);
            next = 0xf46aU; break;
        case 0xf46aU: // 6706 BEQ $f472
            next = (r.status & 4U) ? 0xf472U : 0xf46cU; break;
        case 0xf46cU: { // 29780d020086 MOVE.L $d02.w,$86(A4)
            const auto value = m.lng(0xd02U); m.lng(r.address[4] + 0x86U, value);
            m.logic(value, 32U); next = 0xf472U; break;
        }
        case 0xf472U: { // 610008cc BSR $fd40
            const auto child = m.call(c, 639U, pc, 0xfd40U, 0xf476U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            next = r.program_counter; break;
        }
        case 0xf476U: // 3b7cffff0052 MOVE.W #$ffff,$52(A5)
            m.word(r.address[5] + 0x52U, 0xffffU); m.logic(0xffffU, 16U);
            next = 0xf47cU; break;
        case 0xf47cU: // 4a6c00a2 TST.W $a2(A4)
            m.logic(m.word(r.address[4] + 0xa2U), 16U); next = 0xf480U; break;
        case 0xf480U: // 6a04 BPL $f486
            next = (r.status & 8U) ? 0xf482U : 0xf486U; break;
        case 0xf482U: // 426c00a2 CLR.W $a2(A4)
            (void)m.word(r.address[4] + 0xa2U); m.word(r.address[4] + 0xa2U, 0U);
            m.logic(0U, 16U); next = 0xf486U; break;
        case 0xf486U: // 0c7800270c02 CMPI.W #$27,$c02.w
            m.sub(m.word(0xc02U), 0x27U, 16U, true); next = 0xf48cU; break;
        case 0xf48cU: // 6716 BEQ $f4a4
            next = (r.status & 4U) ? 0xf4a4U : 0xf48eU; break;
        case 0xf48eU: // 322c00a2 MOVE.W $a2(A4),D1
            m.dw(1U, m.word(r.address[4] + 0xa2U)); m.logic(r.data[1], 16U);
            next = 0xf492U; break;
        case 0xf492U: // 0c410013 CMPI.W #$13,D1
            m.sub(r.data[1], 0x13U, 16U, true); next = 0xf496U; break;
        case 0xf496U: // 6a14 BPL $f4ac (tests N, not N xor V)
            next = (r.status & 8U) ? 0xf498U : 0xf4acU; break;
        case 0xf498U: // 41fa1236 LEA $106d0(PC),A0
            r.address[0] = 0x106d0U; next = 0xf49cU; break;
        case 0xf49cU: // e541 ASL.W #2,D1
            m.asl_word(1U, 2U); next = 0xf49eU; break;
        case 0xf49eU: // 22301000 MOVE.L (A0,D1.W),D1
            r.data[1] = m.lng(r.address[0] + static_cast<std::int16_t>(r.data[1]));
            m.logic(r.data[1], 32U); next = 0xf4a2U; break;
        case 0xf4a2U: next = 0xf4b2U; transfer_kind = 1U; break; // 600e
        case 0xf4a4U: // 223c05000000 MOVE.L #$05000000,D1
            r.data[1] = 0x05000000U; m.logic(r.data[1], 32U); next = 0xf4aaU; break;
        case 0xf4aaU: next = 0xf4b2U; transfer_kind = 1U; break; // 6006
        case 0xf4acU: // 223c01000000 MOVE.L #$01000000,D1
            r.data[1] = 0x01000000U; m.logic(r.data[1], 32U); next = 0xf4b2U; break;
        case 0xf4b2U: // 294100a4 MOVE.L D1,$a4(A4)
            m.lng(r.address[4] + 0xa4U, r.data[1]); m.logic(r.data[1], 32U);
            next = 0xf4b6U; break;
        case 0xf4b6U: // 202c0086 MOVE.L $86(A4),D0
            r.data[0] = m.lng(r.address[4] + 0x86U); m.logic(r.data[0], 32U);
            next = 0xf4baU; break;
        case 0xf4baU: { // 4eb900015e7e JSR $15e7e.l
            const auto child = m.call(c, 640U, pc, 0x15e7eU, 0xf4c0U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            next = r.program_counter; break;
        }
        case 0xf4c0U: // 294000a8 MOVE.L D0,$a8(A4)
            m.lng(r.address[4] + 0xa8U, r.data[0]); m.logic(r.data[0], 32U);
            next = 0xf4c4U; break;
        case 0xf4c4U: // 3b7c00090044 MOVE.W #9,$44(A5)
            m.word(r.address[5] + 0x44U, 9U); m.logic(9U, 16U);
            next = 0xf4caU; break;
        case 0xf4caU: case 0xf508U: // 102d006d MOVE.B $6d(A5),D0
            m.db(0U, m.byte(r.address[5] + 0x6dU)); m.logic(r.data[0], 8U);
            next = pc == 0xf4caU ? 0xf4ceU : 0xf50cU; break;
        case 0xf4ceU: case 0xf50cU: { // 01f80c05 / 01f80c06 BSET.B D0,$c05/6.w
            const auto address = pc == 0xf4ceU ? 0xc05U : 0xc06U;
            const auto old = m.byte(address);
            const auto bit = 1U << (r.data[0] & 7U);
            m.byte(address, old | bit);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit) ? 0U : 4U));
            next = pc == 0xf4ceU ? 0xf4d2U : 0xf510U; break;
        }
        case 0xf4d2U: case 0xf4daU: { // 61000912 / 6100090a BSR $fde6
            const auto continuation = pc == 0xf4d2U ? 0xf4d6U : 0xf4deU;
            const auto child = m.call(c, 198U, pc, 0xfde6U, continuation);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            next = r.program_counter; break;
        }
        case 0xf4deU: // 7000 MOVEQ #0,D0
            r.data[0] = 0U; m.logic(0U, 32U); next = 0xf4e0U; break;
        case 0xf4e0U: // 102c0000 MOVE.B (A4),D0
            m.db(0U, m.byte(r.address[4])); m.logic(r.data[0], 8U);
            next = 0xf4e4U; break;
        case 0xf4e4U: // 426c0000 CLR.W (A4)
            (void)m.word(r.address[4]); m.word(r.address[4], 0U);
            m.logic(0U, 16U); next = 0xf4e8U; break;
        case 0xf4e8U: { // 91780c10 SUB.W D0,$c10.w
            const auto value = m.sub(m.word(0xc10U), r.data[0], 16U);
            m.word(0xc10U, value); next = 0xf4ecU; break;
        }
        case 0xf4ecU: // 0c6d00040044 CMPI.W #4,$44(A5)
            m.sub(m.word(r.address[5] + 0x44U), 4U, 16U, true);
            next = 0xf4f2U; break;
        case 0xf4f2U: // 6406 BCC $f4fa
            next = (r.status & 1U) ? 0xf4f4U : 0xf4faU; break;
        case 0xf4f4U: // 426d0044 CLR.W $44(A5)
            (void)m.word(r.address[5] + 0x44U); m.word(r.address[5] + 0x44U, 0U);
            m.logic(0U, 16U); next = 0xf4f8U; break;
        case 0xf4faU: // 0c6d00060044 CMPI.W #6,$44(A5)
            m.sub(m.word(r.address[5] + 0x44U), 6U, 16U, true);
            next = 0xf500U; break;
        case 0xf500U: // 6412 BCC $f514
            next = (r.status & 1U) ? 0xf502U : 0xf514U; break;
        case 0xf502U: // 3b7c00060044 MOVE.W #6,$44(A5)
            m.word(r.address[5] + 0x44U, 6U); m.logic(6U, 16U);
            next = 0xf508U; break;
        case 0xf510U: { // 53780c10 SUBQ.W #1,$c10.w
            const auto value = m.sub(m.word(0xc10U), 1U, 16U);
            m.word(0xc10U, value); next = 0xf514U; break;
        }
        case 0xf438U: case 0xf4d6U: case 0xf4d8U: case 0xf4f8U:
        case 0xf514U: case 0xf516U: case 0xf518U: { // 4e75 RTS
            const auto result = m.ret();
            if (auto event = m.interrupt(c, pc, r.program_counter)) return *event;
            return result;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        // Known local entries are handled by the instruction switch. Preserve
        // an external transfer's actual calculated target; never substitute a mode.
        if (next < 0xf406U || next > 0xf518U)
            return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}
} // namespace gain_ground::translated
