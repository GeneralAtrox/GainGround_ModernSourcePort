#include "cpu_b_state72_dispatch_record_phase_detail.h"

namespace gain_ground::translated::cpu_b_state72_dispatch_record_phase_detail {

bool dispatch_record_region_03(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind,
    std::optional<FunctionResult> &outcome)
{
    auto &c = context;
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xf60cU: { // 31810008 move.w d1, $8(a0, d0.w)
            next = 0xf610U;
            const auto value = r.data[1];
            const auto destination_address = r.address[0] + 0x8U + static_cast<std::int16_t>(r.data[0]);
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf610U: { // 31810010 move.w d1, $10(a0, d0.w)
            next = 0xf614U;
            const auto value = r.data[1];
            const auto destination_address = r.address[0] + 0x10U + static_cast<std::int16_t>(r.data[0]);
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf614U: { // 31810018 move.w d1, $18(a0, d0.w)
            next = 0xf618U;
            const auto value = r.data[1];
            const auto destination_address = r.address[0] + 0x18U + static_cast<std::int16_t>(r.data[0]);
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf618U: { // 06400020 addi.w #$20, d0
            next = 0xf61cU;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xf61cU: { // 3b400046 move.w d0, $46(a5)
            next = 0xf620U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x46U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf620U: { // 0c400400 cmpi.w #$400, d0
            next = 0xf624U;
            const auto source_value = 0x400U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xf624U: { // 653c bcs.b $f662
            next = 0xf626U;
            if ((r.status & 1U) != 0U) { next = 0xf662U; transfer_kind = 1U; }
            break;
        }
        case 0xf626U: { // 41f900200090 lea.l $200090.l, a0
            next = 0xf62cU;
            r.address[0] = 0x200090U;
            break;
        }
        case 0xf62cU: { // d0ed007a adda.w $7a(a5), a0
            next = 0xf630U;
            const auto source_address = r.address[5] + 0x7aU;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf630U: { // 43fa0f12 lea.l $10544(pc), a1
            next = 0xf634U;
            const auto source_address = 0x10544U;
            r.address[1] = source_address;
            break;
        }
        case 0xf634U: { // 7006 moveq #$6, d0
            next = 0xf636U;
            r.data[0] = 0x6U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xf636U: { // 3219 move.w (a1)+, d1
            next = 0xf638U;
            const auto source_address = r.address[1];
            const auto value = m.word(source_address);
            r.address[1] += 2U;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf638U: { // 826d0064 or.w $64(a5), d1
            next = 0xf63cU;
            const auto source_address = r.address[5] + 0x64U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = destination_value | source_value;
            m.logic(value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0xf63cU: { // 3081 move.w d1, (a0)
            next = 0xf63eU;
            const auto value = r.data[1];
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf63eU: { // d0fc0080 adda.w #$80, a0
            next = 0xf642U;
            const auto source_value = 0x80U;
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf642U: { // 51c8fff2 dbra d0, $f636
            next = 0xf646U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0xf636U;
            break;
        }
        case 0xf646U: { // 7037 moveq #$37, d0
            next = 0xf648U;
            r.data[0] = 0x37U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xf648U: { // 41f90020c040 lea.l $20c040.l, a0
            next = 0xf64eU;
            r.address[0] = 0x20c040U;
            break;
        }
        case 0xf64eU: { // d0ed0078 adda.w $78(a5), a0
            next = 0xf652U;
            const auto source_address = r.address[5] + 0x78U;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf652U: { // 30bcff00 move.w #$ff00, (a0)
            next = 0xf656U;
            const auto value = 0xff00U;
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf656U: { // 5048 addq.w #$8, a0
            next = 0xf658U;
            const auto source_value = 0x8U;
            r.address[0] += source_value;
            break;
        }
        case 0xf658U: { // 51c8fff8 dbra d0, $f652
            next = 0xf65cU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0xf652U;
            break;
        }
        case 0xf65cU: { // 3b7c00020044 move.w #$2, $44(a5)
            next = 0xf662U;
            m.word(r.address[5] + 0x44U, 0x2U);
            m.logic(0x2U, 16U);
            break;
        }
        case 0xf662U: { // 4e75 rts 
            next = 0xf664U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf662U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xf664U: { // 102c008b move.b $8b(a4), d0
            next = 0xf668U;
            const auto source_address = r.address[4] + 0x8bU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf668U: { // 123c0006 move.b #$6, d1
            next = 0xf66cU;
            const auto value = 0x6U;
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf66cU: { // c200 and.b d0, d1
            next = 0xf66eU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = destination_value & source_value;
            m.logic(value, 8U);
            m.db(1U, value);
            break;
        }
        case 0xf66eU: { // 6648 bne.b $f6b8
            next = 0xf670U;
            if ((r.status & 4U) == 0U) { next = 0xf6b8U; transfer_kind = 1U; }
            break;
        }
        case 0xf670U: { // 0c2c00010000 cmpi.b #$1, $0(a4)
            next = 0xf676U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[4] + 0x0U;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0xf676U: { // 673e beq.b $f6b6
            next = 0xf678U;
            if ((r.status & 4U) != 0U) { next = 0xf6b6U; transfer_kind = 1U; }
            break;
        }
        case 0xf678U: { // 08000005 btst.b #$5, d0
            next = 0xf67cU;
            const auto old = r.data[0];
            const auto bit_mask = 1U << (0x5U & 31U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0xf67cU: { // 6612 bne.b $f690
            next = 0xf67eU;
            if ((r.status & 4U) == 0U) { next = 0xf690U; transfer_kind = 1U; }
            break;
        }
        case 0xf67eU: { // 08000004 btst.b #$4, d0
            next = 0xf682U;
            const auto old = r.data[0];
            const auto bit_mask = 1U << (0x4U & 31U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0xf682U: { // 6732 beq.b $f6b6
            next = 0xf684U;
            if ((r.status & 4U) != 0U) { next = 0xf6b6U; transfer_kind = 1U; }
            break;
        }
        case 0xf684U: // 3b7cfffc0050 move.w #$fffc,$50(a5)
            next = 0xf68aU; m.word(r.address[5] + 0x50U, 0xfffcU); m.logic(0xfffcU, 16U); break;
        case 0xf68aU: { // 61000568 bsr.w $fbf4
            const auto result = m.call(c, 469U, pc, 0xfbf4U, 0xf68eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter; break;
        }
        case 0xf68eU: next = 0xf69aU; transfer_kind = 1U; break; // 600a
        case 0xf690U: { // 3b7c00040050 move.w #$4, $50(a5)
            next = 0xf696U;
            m.word(r.address[5] + 0x50U, 0x4U);
            m.logic(0x4U, 16U);
            break;
        }
        case 0xf696U: { // 61000574 bsr.w $fc0c
            next = 0xf69aU;
            const auto result = m.call(c, 191U, 0xf696U, 0xfc0cU, 0xf69aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xf69aU: { // 426d0046 clr.w $46(a5)
            next = 0xf69eU;
            const auto destination_address = r.address[5] + 0x46U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf69eU: { // 3b7c00030044 move.w #$3, $44(a5)
            next = 0xf6a4U;
            m.word(r.address[5] + 0x44U, 0x3U);
            m.logic(0x3U, 16U);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_state72_dispatch_record_phase_detail
