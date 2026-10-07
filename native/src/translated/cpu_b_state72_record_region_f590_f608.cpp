#include "cpu_b_state72_dispatch_record_phase_detail.h"

namespace gain_ground::translated::cpu_b_state72_dispatch_record_phase_detail {

bool dispatch_record_region_02(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind,
    std::optional<FunctionResult> &outcome)
{
    auto &c = context;
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xf590U: { // 01f80c04 bset.b d0, $c04.w
            next = 0xf594U;
            const auto destination_address = 0xc04U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (r.data[0] & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0xf594U: { // 01b80820 bclr.b d0, $820.w
            next = 0xf598U;
            const auto destination_address = 0x820U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (r.data[0] & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0xf598U: { // 53380821 subq.b #$1, $821.w
            next = 0xf59cU;
            const auto source_value = 0x1U;
            const auto destination_address = 0x821U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0xf59cU: { // 306d006e movea.w $6e(a5), a0
            next = 0xf5a0U;
            const auto source_address = r.address[5] + 0x6eU;
            const auto value = m.word(source_address);
            r.address[0] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xf5a0U: { // 30280002 move.w $2(a0), d0
            next = 0xf5a4U;
            m.dw(0U, m.word(r.address[0] + 0x2U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xf5a4U: { // 41f87b44 lea.l $7b44.w, a0
            next = 0xf5a8U;
            r.address[0] = 0x7b44U;
            break;
        }
        case 0xf5a8U: { // b050 cmp.w (a0), d0
            next = 0xf5aaU;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xf5aaU: { // 6502 bcs.b $f5ae
            next = 0xf5acU;
            if ((r.status & 1U) != 0U) { next = 0xf5aeU; transfer_kind = 1U; }
            break;
        }
        case 0xf5acU: { // 3080 move.w d0, (a0)
            next = 0xf5aeU;
            const auto value = r.data[0];
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf5aeU: { // 5448 addq.w #$2, a0
            next = 0xf5b0U;
            const auto source_value = 0x2U;
            r.address[0] += source_value;
            break;
        }
        case 0xf5b0U: { // b050 cmp.w (a0), d0
            next = 0xf5b2U;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xf5b2U: { // 6402 bcc.b $f5b6
            next = 0xf5b4U;
            if ((r.status & 1U) == 0U) { next = 0xf5b6U; transfer_kind = 1U; }
            break;
        }
        case 0xf5b4U: // 3080 move.w d0,(a0)
            next = 0xf5b6U; m.word(r.address[0], r.data[0]); m.logic(r.data[0], 16U); break;
        case 0xf5b6U: { // 41fa1164 lea.l $1071c(pc), a0
            next = 0xf5baU;
            const auto source_address = 0x1071cU;
            r.address[0] = source_address;
            break;
        }
        case 0xf5baU: { // 43f87b58 lea.l $7b58.w, a1
            next = 0xf5beU;
            r.address[1] = 0x7b58U;
            break;
        }
        case 0xf5beU: { // b058 cmp.w (a0)+, d0
            next = 0xf5c0U;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xf5c0U: { // 6504 bcs.b $f5c6
            next = 0xf5c2U;
            if ((r.status & 1U) != 0U) { next = 0xf5c6U; transfer_kind = 1U; }
            break;
        }
        case 0xf5c2U: { // 5449 addq.w #$2, a1
            next = 0xf5c4U;
            const auto source_value = 0x2U;
            r.address[1] += source_value;
            break;
        }
        case 0xf5c4U: { // 60f8 bra.b $f5be
            next = 0xf5c6U;
            if (true) { next = 0xf5beU; transfer_kind = 1U; }
            break;
        }
        case 0xf5c6U: { // 5251 addq.w #$1, (a1)
            next = 0xf5c8U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[1];
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xf5c8U: { // 202c0080 move.l $80(a4), d0
            next = 0xf5ccU;
            r.data[0] = m.lng(r.address[4] + 0x80U);
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xf5ccU: { // 41f87b48 lea.l $7b48.w, a0
            next = 0xf5d0U;
            r.address[0] = 0x7b48U;
            break;
        }
        case 0xf5d0U: { // b090 cmp.l (a0), d0
            next = 0xf5d2U;
            const auto source_address = r.address[0];
            const auto source_value = m.lng(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 32U, true);
            break;
        }
        case 0xf5d2U: { // 6402 bcc.b $f5d6
            next = 0xf5d4U;
            if ((r.status & 1U) == 0U) { next = 0xf5d6U; transfer_kind = 1U; }
            break;
        }
        case 0xf5d4U: // 2080 move.l d0,(a0)
            next = 0xf5d6U; m.lng(r.address[0], r.data[0]); m.logic(r.data[0], 32U); break;
        case 0xf5d6U: { // 5848 addq.w #$4, a0
            next = 0xf5d8U;
            const auto source_value = 0x4U;
            r.address[0] += source_value;
            break;
        }
        case 0xf5d8U: { // b090 cmp.l (a0), d0
            next = 0xf5daU;
            const auto source_address = r.address[0];
            const auto source_value = m.lng(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 32U, true);
            break;
        }
        case 0xf5daU: { // 6502 bcs.b $f5de
            next = 0xf5dcU;
            if ((r.status & 1U) != 0U) { next = 0xf5deU; transfer_kind = 1U; }
            break;
        }
        case 0xf5dcU: { // 2080 move.l d0, (a0)
            next = 0xf5deU;
            const auto value = r.data[0];
            const auto destination_address = r.address[0];
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf5deU: { // 4eb900016252 jsr $16252.l
            next = 0xf5e4U;
            const auto result = m.call(c, 300U, 0xf5deU, 0x16252U, 0xf5e4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xf5e4U: { // 41f87b54 lea.l $7b54.w, a0
            next = 0xf5e8U;
            r.address[0] = 0x7b54U;
            break;
        }
        case 0xf5e8U: { // d390 add.l d1, (a0)
            next = 0xf5eaU;
            const auto source_value = r.data[1];
            const auto destination_address = r.address[0];
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0xf5eaU: { // 6404 bcc.b $f5f0
            next = 0xf5ecU;
            if ((r.status & 1U) == 0U) { next = 0xf5f0U; transfer_kind = 1U; }
            break;
        }
        case 0xf5ecU: { // 52a8fffc addq.l #1,-4(a0)
            next = 0xf5f0U;
            const auto address = r.address[0] - 4U;
            const auto value = m.add(m.lng(address), 1U, 32U);
            m.word(address + 2U, value); m.word(address, value >> 16U); break;
        }
        case 0xf5f0U: { // 4af80410 tas.b $410.w
            next = 0xf5f4U;
            const auto destination_address = 0x410U;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0xf5f4U: { // 4e75 rts 
            next = 0xf5f6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf5f4U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xf5f6U: { // 41f90020c000 lea.l $20c000.l, a0
            next = 0xf5fcU;
            r.address[0] = 0x20c000U;
            break;
        }
        case 0xf5fcU: { // d0ed0078 adda.w $78(a5), a0
            next = 0xf600U;
            const auto source_address = r.address[5] + 0x78U;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf600U: { // 302d0046 move.w $46(a5), d0
            next = 0xf604U;
            m.dw(0U, m.word(r.address[5] + 0x46U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xf604U: { // 323cff80 move.w #$ff80, d1
            next = 0xf608U;
            m.dw(1U, 0xff80U);
            m.logic(r.data[1], 16U);
            break;
        }
        case 0xf608U: { // 31810000 move.w d1, (a0, d0.w)
            next = 0xf60cU;
            const auto value = r.data[1];
            const auto destination_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_state72_dispatch_record_phase_detail
