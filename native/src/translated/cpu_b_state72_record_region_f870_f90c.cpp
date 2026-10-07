#include "cpu_b_state72_dispatch_record_phase_detail.h"

namespace gain_ground::translated::cpu_b_state72_dispatch_record_phase_detail {

bool dispatch_record_region_07(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind,
    std::optional<FunctionResult> &outcome)
{
    auto &c = context;
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xf870U: { // 5340 subq.w #$1, d0
            next = 0xf872U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xf872U: { // d040 add.w d0, d0
            next = 0xf874U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xf874U: { // 3200 move.w d0, d1
            next = 0xf876U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf876U: { // d040 add.w d0, d0
            next = 0xf878U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xf878U: { // d041 add.w d1, d0
            next = 0xf87aU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xf87aU: { // 43fa0eb4 lea.l $10730(pc), a1
            next = 0xf87eU;
            const auto source_address = 0x10730U;
            r.address[1] = source_address;
            break;
        }
        case 0xf87eU: { // d2c0 adda.w d0, a1
            next = 0xf880U;
            const auto source_value = r.data[0];
            r.address[1] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf880U: { // 1b59005b move.b (a1)+, $5b(a5)
            next = 0xf884U;
            const auto source_address = r.address[1];
            const auto value = m.byte(source_address);
            r.address[1] += 1U;
            const auto destination_address = r.address[5] + 0x5bU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf884U: { // 1b59003c move.b (a1)+, $3c(a5)
            next = 0xf888U;
            const auto source_address = r.address[1];
            const auto value = m.byte(source_address);
            r.address[1] += 1U;
            const auto destination_address = r.address[5] + 0x3cU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf888U: { // 206c0092 movea.l $92(a4), a0
            next = 0xf88cU;
            const auto source_address = r.address[4] + 0x92U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0xf88cU: { // 4a91 tst.l (a1)
            next = 0xf88eU;
            const auto destination_address = r.address[1];
            const auto value = m.lng(destination_address);
            m.logic(value, 32U);
            break;
        }
        case 0xf88eU: { // 6702 beq.b $f892
            next = 0xf890U;
            if ((r.status & 4U) != 0U) { next = 0xf892U; transfer_kind = 1U; }
            break;
        }
        case 0xf890U: { // 2051 movea.l (a1), a0
            next = 0xf892U;
            const auto source_address = r.address[1];
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0xf892U: { // 6100043e bsr.w $fcd2
            next = 0xf896U;
            const auto result = m.call(c, 196U, 0xf892U, 0xfcd2U, 0xf896U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xf896U: { // 4e75 rts 
            next = 0xf898U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf896U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xf898U: { // 3b7cffff0052 move.w #$ffff, $52(a5)
            next = 0xf89eU;
            m.word(r.address[5] + 0x52U, 0xffffU);
            m.logic(0xffffU, 16U);
            break;
        }
        case 0xf89eU: { // 426d0044 clr.w $44(a5)
            next = 0xf8a2U;
            const auto destination_address = r.address[5] + 0x44U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf8a2U: { // 4df83200 lea.l $3200.w, a6
            next = 0xf8a6U;
            r.address[6] = 0x3200U;
            break;
        }
        case 0xf8a6U: { // 4aae0042 tst.l $42(a6)
            next = 0xf8aaU;
            const auto destination_address = r.address[6] + 0x42U;
            const auto value = m.lng(destination_address);
            m.logic(value, 32U);
            break;
        }
        case 0xf8aaU: { // 6648 bne.b $f8f4
            next = 0xf8acU;
            if ((r.status & 4U) == 0U) { next = 0xf8f4U; transfer_kind = 1U; }
            break;
        }
        case 0xf8acU: { // 2d7c0001375a0002 move.l #$1375a, $2(a6)
            next = 0xf8b4U;
            m.lng(r.address[6] + 0x2U, 0x1375aU);
            m.logic(0x1375aU, 32U);
            break;
        }
        case 0xf8b4U: { // 3d6d00120012 move.w $12(a5), $12(a6)
            next = 0xf8baU;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[6] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf8baU: { // 3d7c00080016 move.w #$8, $16(a6)
            next = 0xf8c0U;
            m.word(r.address[6] + 0x16U, 0x8U);
            m.logic(0x8U, 16U);
            break;
        }
        case 0xf8c0U: { // 3d6d001a001a move.w $1a(a5), $1a(a6)
            next = 0xf8c6U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[6] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf8c6U: { // 1d6d004b004b move.b $4b(a5), $4b(a6)
            next = 0xf8ccU;
            const auto source_address = r.address[5] + 0x4bU;
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[6] + 0x4bU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf8ccU: { // 1d7c0001003d move.b #$1, $3d(a6)
            next = 0xf8d2U;
            m.byte(r.address[6] + 0x3dU, 0x1U);
            m.logic(0x1U, 8U);
            break;
        }
        case 0xf8d2U: { // 3d7c00020058 move.w #$2, $58(a6)
            next = 0xf8d8U;
            m.word(r.address[6] + 0x58U, 0x2U);
            m.logic(0x2U, 16U);
            break;
        }
        case 0xf8d8U: { // 3d7c00090052 move.w #$9, $52(a6)
            next = 0xf8deU;
            m.word(r.address[6] + 0x52U, 0x9U);
            m.logic(0x9U, 16U);
            break;
        }
        case 0xf8deU: { // 1cbc0080 move.b #$80, (a6)
            next = 0xf8e2U;
            const auto value = 0x80U;
            const auto destination_address = r.address[6];
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf8e2U: { // 1d7c00800080 move.b #$80, $80(a6)
            next = 0xf8e8U;
            m.byte(r.address[6] + 0x80U, 0x80U);
            m.logic(0x80U, 8U);
            break;
        }
        case 0xf8e8U: { // 1d7c00800100 move.b #$80, $100(a6)
            next = 0xf8eeU;
            m.byte(r.address[6] + 0x100U, 0x80U);
            m.logic(0x80U, 8U);
            break;
        }
        case 0xf8eeU: { // 1d7c00800180 move.b #$80, $180(a6)
            next = 0xf8f4U;
            m.byte(r.address[6] + 0x180U, 0x80U);
            m.logic(0x80U, 8U);
            break;
        }
        case 0xf8f4U: { // 4e75 rts 
            next = 0xf8f6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf8f4U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xf8f6U: { // 4af80d1a tas.b $d1a.w
            next = 0xf8faU;
            const auto destination_address = 0xd1aU;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0xf8faU: { // 426d0044 clr.w $44(a5)
            next = 0xf8feU;
            const auto destination_address = r.address[5] + 0x44U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf8feU: { // 4e75 rts 
            next = 0xf900U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf8feU, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        // Retained F178 disassembly: lifecycle modes 8, 9 and 10.
        case 0xf900U: case 0xf93cU: case 0xf9a4U: case 0xf9b4U: case 0xf9c2U: {
            const auto result = m.ret();
            if (auto event = m.interrupt(c, pc, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
        }
        case 0xf902U: // 0c7800020c16 cmpi.w #2,$c16.w
            next = 0xf908U; (void)m.sub(m.word(0xc16U), 2U, 16U, true); break;
        case 0xf908U: // 660000aa bne.w $f9b4
            next = (r.status & 4U) ? 0xf90cU : 0xf9b4U; transfer_kind = 1U; break;
        case 0xf90cU: // 4a380d0c tst.b $d0c.w
            next = 0xf910U; m.logic(m.byte(0xd0cU), 8U); break;
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_state72_dispatch_record_phase_detail
