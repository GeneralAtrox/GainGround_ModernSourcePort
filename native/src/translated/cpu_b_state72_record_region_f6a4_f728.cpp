#include "cpu_b_state72_dispatch_record_phase_detail.h"

namespace gain_ground::translated::cpu_b_state72_dispatch_record_phase_detail {

bool dispatch_record_region_04(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind,
    std::optional<FunctionResult> &outcome)
{
    auto &c = context;
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xf6a4U: { // 2f0c move.l a4, -(a7)
            next = 0xf6a6U;
            const auto value = r.address[4];
            r.address[7] -= 4U;
            const auto destination_address = r.address[7];
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf6a6U: { // 303c0042 move.w #$42, d0
            next = 0xf6aaU;
            m.dw(0U, 0x42U);
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xf6aaU: { // 4eb900016ff8 jsr $16ff8.l
            next = 0xf6b0U;
            const auto result = m.call(c, 308U, 0xf6aaU, 0x16ff8U, 0xf6b0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xf6b0U: { // 285f movea.l (a7)+, a4
            next = 0xf6b2U;
            const auto source_address = r.address[7];
            const auto value = m.lng(source_address);
            r.address[7] += 4U;
            r.address[4] = value;
            break;
        }
        case 0xf6b2U: { // 600000e8 bra.w $f79c
            next = 0xf6b6U;
            if (true) { next = 0xf79cU; transfer_kind = 1U; }
            break;
        }
        case 0xf6b6U: { // 4e75 rts 
            next = 0xf6b8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf6b6U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xf6b8U: { // 142c0000 move.b $0(a4), d2
            next = 0xf6bcU;
            const auto source_address = r.address[4] + 0x0U;
            const auto value = m.byte(source_address);
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf6bcU: { // 5302 subq.b #$1, d2
            next = 0xf6beU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[2];
            const auto value = m.sub(destination_value, source_value, 8U);
            m.db(2U, value);
            break;
        }
        case 0xf6beU: { // 19420000 move.b d2, $0(a4)
            next = 0xf6c2U;
            const auto value = r.data[2];
            const auto destination_address = r.address[4] + 0x0U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf6c2U: { // 7201 moveq #$1, d1
            next = 0xf6c4U;
            r.data[1] = 0x1U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xf6c4U: { // 102c0001 move.b $1(a4), d0
            next = 0xf6c8U;
            const auto source_address = r.address[4] + 0x1U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf6c8U: { // 44fc0000 move.w #$0, ccr
            next = 0xf6ccU;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x0U & 0x1fU));
            break;
        }
        case 0xf6ccU: { // 8101 sbcd.b d1, d0
            next = 0xf6ceU;
            const auto source_value = r.data[1];
            const auto value = m.sbcd(r.data[0], source_value);
            m.db(0U, value);
            break;
        }
        case 0xf6ceU: { // 19400001 move.b d0, $1(a4)
            next = 0xf6d2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[4] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf6d2U: { // 122d004a move.b $4a(a5), d1
            next = 0xf6d6U;
            const auto source_address = r.address[5] + 0x4aU;
            const auto value = m.byte(source_address);
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf6d6U: { // 41f41002 lea.l $2(a4, d1.w), a0
            next = 0xf6daU;
            const auto source_address = r.address[4] + 0x2U + static_cast<std::int16_t>(r.data[1]);
            r.address[0] = source_address;
            break;
        }
        case 0xf6daU: { // 1b50004b move.b (a0), $4b(a5)
            next = 0xf6deU;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x4bU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf6deU: { // 9401 sub.b d1, d2
            next = 0xf6e0U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[2];
            const auto value = m.sub(destination_value, source_value, 8U);
            m.db(2U, value);
            break;
        }
        case 0xf6e0U: { // 6708 beq.b $f6ea
            next = 0xf6e2U;
            if ((r.status & 4U) != 0U) { next = 0xf6eaU; transfer_kind = 1U; }
            break;
        }
        case 0xf6e2U: { // 10e80001 move.b $1(a0), (a0)+
            next = 0xf6e6U;
            const auto value = m.byte(r.address[0] + 1U);
            const auto destination = r.address[0];
            r.address[0] += 1U;
            m.byte(destination, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf6e6U: { // 5302 subq.b #$1, d2
            next = 0xf6e8U;
            m.db(2U, m.sub(r.data[2], 1U, 8U));
            break;
        }
        case 0xf6e8U: { // 66f8 bne.b $f6e2
            next = 0xf6eaU;
            if ((r.status & 4U) == 0U) { next = 0xf6e2U; transfer_kind = 1U; }
            break;
        }
        case 0xf6eaU: { // 142c0001 move.b $1(a4), d2
            next = 0xf6eeU;
            const auto source_address = r.address[4] + 0x1U;
            const auto value = m.byte(source_address);
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf6eeU: { // e09a ror.l #$8, d2
            next = 0xf6f0U;
            m.rotate_right(2U, 8U, 32U);
            break;
        }
        case 0xf6f0U: { // 43fa0f58 lea.l $1064a(pc), a1
            next = 0xf6f4U;
            const auto source_address = 0x1064aU;
            r.address[1] = source_address;
            break;
        }
        case 0xf6f4U: { // 4eb9000160fc jsr $160fc.l
            next = 0xf6faU;
            const auto result = m.call(c, 295U, 0xf6f4U, 0x160fcU, 0xf6faU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xf6faU: { // 7037 moveq #$37, d0
            next = 0xf6fcU;
            r.data[0] = 0x37U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xf6fcU: { // 41f90020c040 lea.l $20c040.l, a0
            next = 0xf702U;
            r.address[0] = 0x20c040U;
            break;
        }
        case 0xf702U: { // d0ed0078 adda.w $78(a5), a0
            next = 0xf706U;
            const auto source_address = r.address[5] + 0x78U;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf706U: { // 30bcff80 move.w #$ff80, (a0)
            next = 0xf70aU;
            const auto value = 0xff80U;
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf70aU: { // 5048 addq.w #$8, a0
            next = 0xf70cU;
            const auto source_value = 0x8U;
            r.address[0] += source_value;
            break;
        }
        case 0xf70cU: { // 51c8fff8 dbra d0, $f706
            next = 0xf710U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0xf706U;
            break;
        }
        case 0xf710U: { // 7200 moveq #$0, d1
            next = 0xf712U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xf712U: { // 41f900200090 lea.l $200090.l, a0
            next = 0xf718U;
            r.address[0] = 0x200090U;
            break;
        }
        case 0xf718U: { // d0ed007a adda.w $7a(a5), a0
            next = 0xf71cU;
            const auto source_address = r.address[5] + 0x7aU;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf71cU: { // 7006 moveq #$6, d0
            next = 0xf71eU;
            r.data[0] = 0x6U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xf71eU: { // 3081 move.w d1, (a0)
            next = 0xf720U;
            const auto value = r.data[1];
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf720U: { // d0fc0080 adda.w #$80, a0
            next = 0xf724U;
            const auto source_value = 0x80U;
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf724U: { // 51c8fff8 dbra d0, $f71e
            next = 0xf728U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0xf71eU;
            break;
        }
        case 0xf728U: { // 3b7c00040044 move.w #$4, $44(a5)
            next = 0xf72eU;
            m.word(r.address[5] + 0x44U, 0x4U);
            m.logic(0x4U, 16U);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_state72_dispatch_record_phase_detail
