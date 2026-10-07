#include "cpu_b_state72_dispatch_record_phase_detail.h"

namespace gain_ground::translated::cpu_b_state72_dispatch_record_phase_detail {

bool dispatch_record_region_01(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind,
    std::optional<FunctionResult> &outcome)
{
    auto &c = context;
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xf3c0U: { // 41f80d06 lea.l $d06.w, a0
            next = 0xf3c4U;
            r.address[0] = 0xd06U;
            break;
        }
        case 0xf3c4U: { // 7200 moveq #$0, d1
            next = 0xf3c6U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xf3c6U: { // 122d006d move.b $6d(a5), d1
            next = 0xf3caU;
            const auto source_address = r.address[5] + 0x6dU;
            const auto value = m.byte(source_address);
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf3caU: { // d241 add.w d1, d1
            next = 0xf3ccU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0xf3ccU: { // 302d0044 move.w $44(a5), d0
            next = 0xf3d0U;
            m.dw(0U, m.word(r.address[5] + 0x44U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xf3d0U: { // 31801000 move.w d0, (a0, d1.w)
            next = 0xf3d4U;
            const auto value = r.data[0];
            const auto destination_address = r.address[0] + static_cast<std::int16_t>(r.data[1]);
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf3d4U: { // e540 asl.w #$2, d0
            next = 0xf3d6U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xf3d6U: { // 4efb0002 jmp $f3da(pc, d0.w)
            next = 0xf3daU;
            next = (0xf3daU + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0xf3daU: { // 6000013e bra.w $f51a
            next = 0xf3deU;
            if (true) { next = 0xf51aU; transfer_kind = 1U; }
            break;
        }
        case 0xf3deU: { // 60000216 bra.w $f5f6
            next = 0xf3e2U;
            if (true) { next = 0xf5f6U; transfer_kind = 1U; }
            break;
        }
        case 0xf3e2U: { // 60000280 bra.w $f664
            next = 0xf3e6U;
            if (true) { next = 0xf664U; transfer_kind = 1U; }
            break;
        }
        case 0xf3e6U: { // 600003b4 bra.w $f79c
            next = 0xf3eaU;
            if (true) { next = 0xf79cU; transfer_kind = 1U; }
            break;
        }
        case 0xf3eaU: { // 600003d2 bra.w $f7be
            next = 0xf3eeU;
            if (true) { next = 0xf7beU; transfer_kind = 1U; }
            break;
        }
        case 0xf3eeU: { // 60000a28 bra.w $fe18
            next = 0xf3f2U;
            if (true) { next = 0xfe18U; transfer_kind = 1U; }
            break;
        }
        case 0xf3f2U: { // 60000428 bra.w $f81c
            next = 0xf3f6U;
            if (true) { next = 0xf81cU; transfer_kind = 1U; }
            break;
        }
        case 0xf3f6U: { // 600004fe bra.w $f8f6
            next = 0xf3faU;
            if (true) { next = 0xf8f6U; transfer_kind = 1U; }
            break;
        }
        case 0xf3faU: next = 0xf900U; transfer_kind = 1U; break; // 60000504
        case 0xf3feU: next = 0xf902U; transfer_kind = 1U; break; // 60000502
        case 0xf402U: next = 0xf9b6U; transfer_kind = 1U; break; // 600005b2
        case 0xf51aU: { // 102d006d move.b $6d(a5), d0
            next = 0xf51eU;
            const auto source_address = r.address[5] + 0x6dU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf51eU: { // 01b80c06 bclr.b d0, $c06.w
            next = 0xf522U;
            const auto destination_address = 0xc06U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (r.data[0] & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0xf522U: { // 4a2c0000 tst.b $0(a4)
            next = 0xf526U;
            m.logic(m.byte(r.address[4] + 0x0U), 8U);
            break;
        }
        case 0xf526U: { // 6730 beq.b $f558
            next = 0xf528U;
            if ((r.status & 4U) != 0U) { next = 0xf558U; transfer_kind = 1U; }
            break;
        }
        case 0xf528U: { // 142c0001 move.b $1(a4), d2
            next = 0xf52cU;
            const auto source_address = r.address[4] + 0x1U;
            const auto value = m.byte(source_address);
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf52cU: { // e09a ror.l #$8, d2
            next = 0xf52eU;
            m.rotate_right(2U, 8U, 32U);
            break;
        }
        case 0xf52eU: { // 43fa111a lea.l $1064a(pc), a1
            next = 0xf532U;
            const auto source_address = 0x1064aU;
            r.address[1] = source_address;
            break;
        }
        case 0xf532U: { // 4eb9000160fc jsr $160fc.l
            next = 0xf538U;
            const auto result = m.call(c, 295U, 0xf532U, 0x160fcU, 0xf538U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xf538U: { // 142c0041 move.b $41(a4), d2
            next = 0xf53cU;
            const auto source_address = r.address[4] + 0x41U;
            const auto value = m.byte(source_address);
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf53cU: { // e09a ror.l #$8, d2
            next = 0xf53eU;
            m.rotate_right(2U, 8U, 32U);
            break;
        }
        case 0xf53eU: { // 43fa110e lea.l $1064e(pc), a1
            next = 0xf542U;
            const auto source_address = 0x1064eU;
            r.address[1] = source_address;
            break;
        }
        case 0xf542U: { // 4eb9000160fc jsr $160fc.l
            next = 0xf548U;
            const auto result = m.call(c, 295U, 0xf542U, 0x160fcU, 0xf548U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xf548U: { // 610006a2 bsr.w $fbec
            next = 0xf54cU;
            const auto result = m.call(c, 190U, 0xf548U, 0xfbecU, 0xf54cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xf54cU: { // 3b7c00010044 move.w #$1, $44(a5)
            next = 0xf552U;
            m.word(r.address[5] + 0x44U, 0x1U);
            m.logic(0x1U, 16U);
            break;
        }
        case 0xf552U: { // 426d0046 clr.w $46(a5)
            next = 0xf556U;
            const auto destination_address = r.address[5] + 0x46U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf556U: { // 4e75 rts 
            next = 0xf558U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf556U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xf558U: { // 4a2c0040 tst.b $40(a4)
            next = 0xf55cU;
            m.logic(m.byte(r.address[4] + 0x40U), 8U);
            break;
        }
        case 0xf55cU: { // 6714 beq.b $f572
            next = 0xf55eU;
            if ((r.status & 4U) != 0U) { next = 0xf572U; transfer_kind = 1U; }
            break;
        }
        case 0xf55eU: // 3b7c00080044 move.w #8,$44(a5)
            next = 0xf564U; m.word(r.address[5] + 0x44U, 8U); m.logic(8U, 16U); break;
        case 0xf564U: // 397c800000a2 move.w #$8000,$a2(a4)
            next = 0xf56aU; m.word(r.address[4] + 0xa2U, 0x8000U); m.logic(0x8000U, 16U); break;
        case 0xf56aU: { // 29780d020086 move.l $d02.w,$86(a4)
            next = 0xf570U;
            const auto value = m.lng(0xd02U);
            m.lng(r.address[4] + 0x86U, value); m.logic(value, 32U); break;
        }
        case 0xf570U: {
            const auto result = m.ret();
            if (auto event = m.interrupt(c, pc, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
        }
        case 0xf572U: { // 3b7c00020042 move.w #$2, $42(a5)
            next = 0xf578U;
            m.word(r.address[5] + 0x42U, 0x2U);
            m.logic(0x2U, 16U);
            break;
        }
        case 0xf578U: { // 3b7c00000044 move.w #$0, $44(a5)
            next = 0xf57eU;
            m.word(r.address[5] + 0x44U, 0x0U);
            m.logic(0x0U, 16U);
            break;
        }
        case 0xf57eU: { // 3b7c00780046 move.w #$78, $46(a5)
            next = 0xf584U;
            m.word(r.address[5] + 0x46U, 0x78U);
            m.logic(0x78U, 16U);
            break;
        }
        case 0xf584U: { // 6100fd02 bsr.w $f288
            next = 0xf588U;
            const auto result = m.call(c, 177U, 0xf584U, 0xf288U, 0xf588U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xf588U: { // 610005ba bsr.w $fb44
            next = 0xf58cU;
            const auto result = m.call(c, 187U, 0xf588U, 0xfb44U, 0xf58cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xf58cU: { // 102d006d move.b $6d(a5), d0
            next = 0xf590U;
            const auto source_address = r.address[5] + 0x6dU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_state72_dispatch_record_phase_detail
