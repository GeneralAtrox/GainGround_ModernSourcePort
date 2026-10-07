#include "cpu_b_callback_state_table_dispatch_detail.h"

namespace gain_ground::translated::cpu_b_callback_state_table_dispatch_detail {

bool dispatch_table_region_02(FunctionContext &,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind)
{
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xef30U: { // 302d0044 move.w $44(a5), d0
            next = 0xef34U;
            m.dw(0U, m.word(r.address[5] + 0x44U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xef34U: { // e540 asl.w #$2, d0
            next = 0xef36U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xef36U: { // 4efb0002 jmp $ef3a(pc, d0.w)
            next = 0xef3aU;
            next = (0xef3aU + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0xef3aU: { // 6000000a bra.w $ef46
            next = 0xef3eU;
            if (true) { next = 0xef46U; transfer_kind = 1U; }
            break;
        }
        case 0xef3eU: { // 60000154 bra.w $f094
            next = 0xf094U; transfer_kind = 1U; break;
        }
        case 0xef42U: { // 60000116 bra.w $f05a
            next = 0xef46U;
            if (true) { next = 0xf05aU; transfer_kind = 1U; }
            break;
        }
        case 0xef46U: { // 536d0046 subq.w #$1, $46(a5)
            next = 0xef4aU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x46U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xef4aU: { // 6f16 ble.b $ef62
            next = 0xef4cU;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0xef62U; transfer_kind = 1U; }
            break;
        }
        case 0xef50U: { // 0c6d005a0046 cmpi.w #$5a, $46(a5)
            next = 0xef56U;
            const auto source_value = 0x5aU;
            const auto destination_address = r.address[5] + 0x46U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xef56U: { // 643c bcc.b $ef94
            next = 0xef58U;
            if ((r.status & 1U) == 0U) { next = 0xef94U; transfer_kind = 1U; }
            break;
        }
        case 0xef58U: { // 102c008c move.b $8c(a4), d0
            next = 0xef5cU;
            const auto source_address = r.address[4] + 0x8cU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xef5cU: { // 02000006 andi.b #$6, d0
            next = 0xef60U;
            const auto source_value = 0x6U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0xef60U: { // 6732 beq.b $ef94
            next = 0xef62U;
            if ((r.status & 4U) != 0U) { next = 0xef94U; transfer_kind = 1U; }
            break;
        }
        case 0xef62U: { // 222c0080 move.l $80(a4), d1
            next = 0xef66U;
            r.data[1] = m.lng(r.address[4] + 0x80U);
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xef66U: { // 41f87800 lea.l $7800.w, a0
            next = 0xef6aU;
            r.address[0] = 0x7800U;
            break;
        }
        case 0xef6aU: { // 7000 moveq #$0, d0
            next = 0xef6cU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xef6cU: { // 7462 moveq #$62, d2
            next = 0xef6eU;
            r.data[2] = 0x62U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0xef6eU: { // b290 cmp.l (a0), d1
            next = 0xef70U;
            const auto source_address = r.address[0];
            const auto source_value = m.lng(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 32U, true);
            break;
        }
        case 0xef70U: { // 6424 bcc.b $ef96
            next = 0xef72U;
            if ((r.status & 1U) == 0U) { next = 0xef96U; transfer_kind = 1U; }
            break;
        }
        case 0xef72U: { // 5240 addq.w #$1, d0
            next = 0xef74U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xef74U: { // 5048 addq.w #$8, a0
            next = 0xef76U;
            const auto source_value = 0x8U;
            r.address[0] += source_value;
            break;
        }
        case 0xef76U: { // 51cafff6 dbra d2, $ef6e
            next = 0xef7aU;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0xef6eU;
            break;
        }
        case 0xef7aU: { // 43fa1682 lea.l $105fe(pc), a1
            next = 0xef7eU;
            const auto source_address = 0x105feU;
            r.address[1] = source_address;
            break;
        }
        case 0xef84U: { // 3b7c00020044 move.w #$2, $44(a5)
            next = 0xef8aU;
            m.word(r.address[5] + 0x44U, 0x2U);
            m.logic(0x2U, 16U);
            break;
        }
        case 0xef8aU: { // 3b7c00780046 move.w #$78, $46(a5)
            next = 0xef90U;
            m.word(r.address[5] + 0x46U, 0x78U);
            m.logic(0x78U, 16U);
            break;
        }
        case 0xef90U: { // 4af80836 tas.b $836.w
            next = 0xef94U;
            const auto destination_address = 0x836U;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_callback_state_table_dispatch_detail
