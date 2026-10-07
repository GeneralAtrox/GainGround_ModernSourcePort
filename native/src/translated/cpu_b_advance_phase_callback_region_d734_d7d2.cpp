#include "cpu_b_advance_phase_callback_dispatch_detail.h"

namespace gain_ground::translated::cpu_b_advance_phase_callback_dispatch_detail {

bool dispatch_instruction_region_01(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind,
    std::optional<FunctionResult> &outcome)
{
    auto &c = context;
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xd734U: { // 6100046a bsr.w $dba0
            next = 0xd738U;
            const auto result = m.call(c, 154U, 0xd734U, 0xdba0U, 0xd738U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xd738U: { // 61000620 bsr.w $dd5a
            next = 0xd73cU;
            const auto result = m.call(c, 162U, 0xd738U, 0xdd5aU, 0xd73cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xd73cU: { // 6100062e bsr.w $dd6c
            next = 0xd740U;
            const auto result = m.call(c, 163U, 0xd73cU, 0xdd6cU, 0xd740U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
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
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xd76eU: { // 6100068a bsr.w $ddfa
            next = 0xd772U;
            const auto result = m.call(c, 165U, 0xd76eU, 0xddfaU, 0xd772U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xd772U: { // 610006d8 bsr.w $de4c
            next = 0xd776U;
            const auto result = m.call(c, 166U, 0xd772U, 0xde4cU, 0xd776U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
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
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xd7a4U: { // 4e75 rts 
            next = 0xd7a6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd7a4U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
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
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_advance_phase_callback_dispatch_detail
