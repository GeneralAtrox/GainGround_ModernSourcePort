#include "cpu_b_state72_dispatch_record_phase_detail.h"

namespace gain_ground::translated::cpu_b_state72_dispatch_record_phase_detail {

bool dispatch_record_region_05(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind,
    std::optional<FunctionResult> &outcome)
{
    auto &c = context;
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xf72eU: { // 426d0046 clr.w $46(a5)
            next = 0xf732U;
            const auto destination_address = r.address[5] + 0x46U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf732U: { // 7000 moveq #$0, d0
            next = 0xf734U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xf734U: { // 102d004b move.b $4b(a5), d0
            next = 0xf738U;
            const auto source_address = r.address[5] + 0x4bU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf738U: { // e940 asl.w #$4, d0
            next = 0xf73aU;
            m.asl_word(0U, 4U);
            break;
        }
        case 0xf73aU: { // 41f900026f1c lea.l $26f1c.l, a0
            next = 0xf740U;
            r.address[0] = 0x26f1cU;
            break;
        }
        case 0xf740U: { // d0c0 adda.w d0, a0
            next = 0xf742U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf742U: { // 2b48004c move.l a0, $4c(a5)
            next = 0xf746U;
            const auto value = r.address[0];
            const auto destination_address = r.address[5] + 0x4cU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf746U: { // 3b6800060036 move.w $6(a0), $36(a5)
            next = 0xf74cU;
            const auto source_address = r.address[0] + 0x6U;
            const auto value = c.host->character_profile(r.address[5], 6U, m.word(source_address));
            const auto destination_address = r.address[5] + 0x36U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf74cU: { // 3b6800080038 move.w $8(a0), $38(a5)
            next = 0xf752U;
            const auto source_address = r.address[0] + 0x8U;
            const auto value = c.host->character_profile(r.address[5], 8U, m.word(source_address));
            const auto destination_address = r.address[5] + 0x38U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf752U: { // 3b68000a003a move.w $a(a0), $3a(a5)
            next = 0xf758U;
            const auto source_address = r.address[0] + 0xaU;
            const auto value = c.host->character_profile(r.address[5], 10U, m.word(source_address));
            const auto destination_address = r.address[5] + 0x3aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf758U: { // 42ad003c clr.l $3c(a5)
            next = 0xf75cU;
            const auto destination_address = r.address[5] + 0x3cU;
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0xf75cU: { // 1b7c0000005a move.b #$0, $5a(a5)
            next = 0xf762U;
            m.byte(r.address[5] + 0x5aU, 0x0U);
            m.logic(0x0U, 8U);
            break;
        }
        case 0xf762U: { // 1b7c0000005b move.b #$0, $5b(a5)
            next = 0xf768U;
            m.byte(r.address[5] + 0x5bU, 0x0U);
            m.logic(0x0U, 8U);
            break;
        }
        case 0xf768U: { // 3b7c001e0048 move.w #$1e, $48(a5)
            next = 0xf76eU;
            m.word(r.address[5] + 0x48U, 0x1eU);
            m.logic(0x1eU, 16U);
            break;
        }
        case 0xf76eU: { // 3b6d00700012 move.w $70(a5), $12(a5)
            next = 0xf774U;
            const auto source_address = r.address[5] + 0x70U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf774U: { // 3b7c00080016 move.w #$8, $16(a5)
            next = 0xf77aU;
            m.word(r.address[5] + 0x16U, 0x8U);
            m.logic(0x8U, 16U);
            break;
        }
        case 0xf77aU: { // 3b7c01c5001a move.w #$1c5, $1a(a5)
            next = 0xf780U;
            m.word(r.address[5] + 0x1aU, 0x1c5U);
            m.logic(0x1c5U, 16U);
            break;
        }
        case 0xf780U: { // 3b7c0000001c move.w #$0, $1c(a5)
            next = 0xf786U;
            m.word(r.address[5] + 0x1cU, 0x0U);
            m.logic(0x0U, 16U);
            break;
        }
        case 0xf786U: { // 3b7c00060058 move.w #$6, $58(a5)
            next = 0xf78cU;
            m.word(r.address[5] + 0x58U, 0x6U);
            m.logic(0x6U, 16U);
            break;
        }
        case 0xf78cU: { // 3b7c00180052 move.w #$18, $52(a5)
            next = 0xf792U;
            m.word(r.address[5] + 0x52U, 0x18U);
            m.logic(0x18U, 16U);
            break;
        }
        case 0xf792U: { // 426d005c clr.w $5c(a5)
            next = 0xf796U;
            const auto destination_address = r.address[5] + 0x5cU;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf796U: { // 610004fe bsr.w $fc96
            next = 0xf79aU;
            const auto result = m.call(c, 194U, 0xf796U, 0xfc96U, 0xf79aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xf79aU: { // 4e75 rts 
            next = 0xf79cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf79aU, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xf79cU: { // 3c2d0050 move.w $50(a5), d6
            next = 0xf7a0U;
            m.dw(6U, m.word(r.address[5] + 0x50U));
            m.logic(r.data[6], 16U);
            break;
        }
        case 0xf7a0U: { // dd6c0084 add.w d6, $84(a4)
            next = 0xf7a4U;
            const auto source_value = r.data[6];
            const auto destination_address = r.address[4] + 0x84U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xf7a4U: { // 026c01ff0084 andi.w #$1ff, $84(a4)
            next = 0xf7aaU;
            const auto source_value = 0x1ffU;
            const auto destination_address = r.address[4] + 0x84U;
            const auto destination_value = m.word(destination_address);
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xf7aaU: { // 526d0046 addq.w #$1, $46(a5)
            next = 0xf7aeU;
            const auto address = r.address[5] + 0x46U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xf7aeU: { // 0c6d00100046 cmpi.w #$10, $46(a5)
            next = 0xf7b4U;
            const auto source_value = 0x10U;
            const auto destination_address = r.address[5] + 0x46U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xf7b4U: { // 6606 bne.b $f7bc
            next = 0xf7b6U;
            if ((r.status & 4U) == 0U) { next = 0xf7bcU; transfer_kind = 1U; }
            break;
        }
        case 0xf7b6U: { // 3b7c00020044 move.w #$2, $44(a5)
            next = 0xf7bcU;
            m.word(r.address[5] + 0x44U, 0x2U);
            m.logic(0x2U, 16U);
            break;
        }
        case 0xf7bcU: { // 4e75 rts 
            next = 0xf7beU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf7bcU, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xf7beU: { // 41f90020c000 lea.l $20c000.l, a0
            next = 0xf7c4U;
            r.address[0] = 0x20c000U;
            break;
        }
        case 0xf7c4U: { // d0ed0078 adda.w $78(a5), a0
            next = 0xf7c8U;
            const auto source_address = r.address[5] + 0x78U;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf7c8U: { // 302d0046 move.w $46(a5), d0
            next = 0xf7ccU;
            m.dw(0U, m.word(r.address[5] + 0x46U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xf7ccU: { // 42700000 clr.w (a0, d0.w)
            next = 0xf7d0U;
            const auto destination_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf7d0U: { // 42700008 clr.w $8(a0, d0.w)
            next = 0xf7d4U;
            const auto destination_address = r.address[0] + 0x8U + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf7d4U: { // 42700010 clr.w $10(a0, d0.w)
            next = 0xf7d8U;
            const auto destination_address = r.address[0] + 0x10U + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_state72_dispatch_record_phase_detail
