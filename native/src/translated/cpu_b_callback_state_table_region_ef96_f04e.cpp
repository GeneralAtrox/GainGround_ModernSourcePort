#include "cpu_b_callback_state_table_dispatch_detail.h"

namespace gain_ground::translated::cpu_b_callback_state_table_dispatch_detail {

bool dispatch_table_region_03(FunctionContext &,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind)
{
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xef96U: { // 0c40000a cmpi.w #$a, d0
            next = 0xef9aU;
            const auto source_value = 0xaU;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xef9aU: { // 653e bcs.b $efda
            next = 0xef9cU;
            if ((r.status & 1U) != 0U) { next = 0xefdaU; transfer_kind = 1U; }
            break;
        }
        case 0xef9cU: { // 7261 moveq #$61, d1
            next = 0xef9eU;
            r.data[1] = 0x61U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xef9eU: { // 9240 sub.w d0, d1
            next = 0xefa0U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0xefa0U: { // 3f01 move.w d1, -(a7)
            next = 0xefa2U;
            const auto value = r.data[1];
            r.address[7] -= 2U;
            const auto destination_address = r.address[7];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xefa2U: { // 31c00836 move.w d0, $836.w
            next = 0xefa6U;
            const auto value = r.data[0];
            const auto destination_address = 0x836U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xefa6U: { // 5240 addq.w #$1, d0
            next = 0xefa8U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xefaeU: { // 2400 move.l d0, d2
            next = 0xefb0U;
            const auto value = r.data[0];
            r.data[2] = value;
            m.logic(value, 32U);
            break;
        }
        case 0xefb0U: { // e09a ror.l #$8, d2
            next = 0xefb2U;
            m.rotate_right(2U, 8U, 32U);
            break;
        }
        case 0xefb2U: { // 43fa167c lea.l $10630(pc), a1
            next = 0xefb6U;
            const auto source_address = 0x10630U;
            r.address[1] = source_address;
            break;
        }
        case 0xefbcU: { // 43fa165a lea.l $10618(pc), a1
            next = 0xefc0U;
            const auto source_address = 0x10618U;
            r.address[1] = source_address;
            break;
        }
        case 0xefc6U: { // 321f move.w (a7)+, d1
            next = 0xefc8U;
            const auto source_address = r.address[7];
            const auto value = m.word(source_address);
            r.address[7] += 2U;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xefc8U: { // 202c0080 move.l $80(a4), d0
            next = 0xefccU;
            r.data[0] = m.lng(r.address[4] + 0x80U);
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xefccU: { // 42ac0098 clr.l $98(a4)
            next = 0xefd0U;
            const auto destination_address = r.address[4] + 0x98U;
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0xefd0U: { // 19780c03009b move.b $c03.w, $9b(a4)
            next = 0xefd6U;
            const auto source_address = 0xc03U;
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[4] + 0x9bU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xefd6U: { // 60000266 bra.w $f23e
            next = 0xefdaU;
            if (true) { next = 0xf23eU; transfer_kind = 1U; }
            break;
        }
        case 0xefdaU: { // 41f900200082 lea.l $200082.l, a0
            next = 0xefe0U;
            r.address[0] = 0x200082U;
            break;
        }
        case 0xefe0U: { // 0c400009 cmpi.w #$9, d0
            next = 0xefe4U;
            const auto source_value = 0x9U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xefe4U: { // 6716 beq.b $effc
            next = 0xefe6U;
            if ((r.status & 4U) != 0U) { next = 0xeffcU; transfer_kind = 1U; }
            break;
        }
        case 0xefe6U: { // e540 asl.w #$2, d0
            next = 0xefe8U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xefe8U: { // d07c80c4 add.w #$80c4, d0
            next = 0xefecU;
            const auto source_value = 0x80c4U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xefecU: { // 806d0062 or.w $62(a5), d0
            next = 0xeff0U;
            const auto source_address = r.address[5] + 0x62U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xeff0U: { // d0ed007a adda.w $7a(a5), a0
            next = 0xeff4U;
            const auto source_address = r.address[5] + 0x7aU;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xeffaU: { // 600c bra.b $f008
            next = 0xeffcU;
            if (true) { next = 0xf008U; transfer_kind = 1U; }
            break;
        }
        case 0xeffcU: { // 43fa1636 lea.l $10634(pc), a1
            next = 0xf000U;
            const auto source_address = 0x10634U;
            r.address[1] = source_address;
            break;
        }
        case 0xf000U: { // 7401 moveq #$1, d2
            next = 0xf002U;
            r.data[2] = 0x1U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0xf008U: { // 41f900200182 lea.l $200182.l, a0
            next = 0xf00eU;
            r.address[0] = 0x200182U;
            break;
        }
        case 0xf00eU: { // d0ed007a adda.w $7a(a5), a0
            next = 0xf012U;
            const auto source_address = r.address[5] + 0x7aU;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf012U: { // 4290 clr.l (a0)
            next = 0xf014U;
            const auto destination_address = r.address[0];
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0xf014U: { // 3b7c00010044 move.w #$1, $44(a5)
            next = 0xf01aU;
            m.word(r.address[5] + 0x44U, 0x1U);
            m.logic(0x1U, 16U);
            break;
        }
        case 0xf01aU: { // 397c00000096 move.w #$0, $96(a4)
            next = 0xf020U;
            m.word(r.address[4] + 0x96U, 0x0U);
            m.logic(0x0U, 16U);
            break;
        }
        case 0xf020U: { // 397c001a0098 move.w #$1a, $98(a4)
            next = 0xf026U;
            m.word(r.address[4] + 0x98U, 0x1aU);
            m.logic(0x1aU, 16U);
            break;
        }
        case 0xf026U: { // 397c1a1a009a move.w #$1a1a, $9a(a4)
            next = 0xf02cU;
            m.word(r.address[4] + 0x9aU, 0x1a1aU);
            m.logic(0x1a1aU, 16U);
            break;
        }
        case 0xf02cU: { // 397c001e009c move.w #$1e, $9c(a4)
            next = 0xf032U;
            m.word(r.address[4] + 0x9cU, 0x1eU);
            m.logic(0x1eU, 16U);
            break;
        }
        case 0xf032U: { // 397c0020009e move.w #$20, $9e(a4)
            next = 0xf038U;
            m.word(r.address[4] + 0x9eU, 0x20U);
            m.logic(0x20U, 16U);
            break;
        }
        case 0xf03cU: { // 397c001900a0 move.w #$19, $a0(a4)
            next = 0xf042U;
            m.word(r.address[4] + 0xa0U, 0x19U);
            m.logic(0x19U, 16U);
            break;
        }
        case 0xf042U: { // 306d0068 movea.w $68(a5), a0
            next = 0xf046U;
            const auto source_address = r.address[5] + 0x68U;
            const auto value = m.word(source_address);
            r.address[0] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xf046U: { // 1010 move.b (a0), d0
            next = 0xf048U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf048U: { // 80380404 or.b $404.w, d0
            next = 0xf04cU;
            const auto source_address = 0x404U;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0xf04cU: { // 6706 beq.b $f054
            next = 0xf04eU;
            if ((r.status & 4U) != 0U) { next = 0xf054U; transfer_kind = 1U; }
            break;
        }
        case 0xf04eU: { // 197c001c0098 move.b #$1c, $98(a4)
            next = 0xf054U;
            m.byte(r.address[4] + 0x98U, 0x1cU);
            m.logic(0x1cU, 8U);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_callback_state_table_dispatch_detail
