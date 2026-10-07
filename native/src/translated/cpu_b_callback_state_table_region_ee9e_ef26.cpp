#include "cpu_b_callback_state_table_dispatch_detail.h"

namespace gain_ground::translated::cpu_b_callback_state_table_dispatch_detail {

bool dispatch_table_region_01(FunctionContext &,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind)
{
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xee9eU: { // 386d0060 movea.w $60(a5), a4
            next = 0xeea2U;
            const auto source_address = r.address[5] + 0x60U;
            const auto value = m.word(source_address);
            r.address[4] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xeea2U: { // 306d006a movea.w $6a(a5), a0
            next = 0xeea6U;
            const auto source_address = r.address[5] + 0x6aU;
            const auto value = m.word(source_address);
            r.address[0] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xeea6U: { // 2950008a move.l (a0), $8a(a4)
            next = 0xeeaaU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            const auto destination_address = r.address[4] + 0x8aU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xeeaaU: { // 302d0042 move.w $42(a5), d0
            next = 0xeeaeU;
            m.dw(0U, m.word(r.address[5] + 0x42U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xeeaeU: { // e540 asl.w #$2, d0
            next = 0xeeb0U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xeeb0U: { // 4efb0002 jmp $eeb4(pc, d0.w)
            next = 0xeeb4U;
            next = (0xeeb4U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0xeeb4U: { // 6000000a bra.w $eec0
            next = 0xeeb8U;
            if (true) { next = 0xeec0U; transfer_kind = 1U; }
            break;
        }
        case 0xeeb8U: { // 600004f2 bra.w $f3ac
            next = 0xeebcU;
            if (true) { next = 0xf3acU; transfer_kind = 1U; }
            break;
        }
        case 0xeebcU: { // 6000006e bra.w $ef2c
            next = 0xeec0U;
            if (true) { next = 0xef2cU; transfer_kind = 1U; }
            break;
        }
        case 0xeec0U: { // 0c7800060c16 cmpi.w #$6, $c16.w
            next = 0xeec6U;
            const auto source_value = 0x6U;
            const auto destination_address = 0xc16U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xeec6U: { // 6a0a bpl.b $eed2
            next = 0xeec8U;
            if ((r.status & 8U) == 0U) { next = 0xeed2U; transfer_kind = 1U; }
            break;
        }
        case 0xeed0U: { // 6502 bcs.b $eed4
            next = 0xeed2U;
            if ((r.status & 1U) != 0U) { next = 0xeed4U; transfer_kind = 1U; }
            break;
        }
        case 0xeed4U: { // 102d006d move.b $6d(a5), d0
            next = 0xeed8U;
            const auto source_address = r.address[5] + 0x6dU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xeed8U: { // d000 add.b d0, d0
            next = 0xeedaU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0xeedaU: { // 5200 addq.b #$1, d0
            next = 0xeedcU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0xeedcU: { // 13c000d00035 move.b d0, $d00035.l
            next = 0xeee2U;
            const auto value = r.data[0];
            const auto destination_address = 0xd00035U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xeee2U: { // 3b7c00010042 move.w #$1, $42(a5)
            next = 0xeee8U;
            m.word(r.address[5] + 0x42U, 0x1U);
            m.logic(0x1U, 16U);
            break;
        }
        case 0xeee8U: { // 30380c16 move.w $c16.w, d0
            next = 0xeeecU;
            const auto source_address = 0xc16U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xeeecU: { // 0c400001 cmpi.w #$1, d0
            next = 0xeef0U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xeef0U: { // 671e beq.b $ef10
            next = 0xeef2U;
            if ((r.status & 4U) != 0U) { next = 0xef10U; transfer_kind = 1U; }
            break;
        }
        case 0xeef2U: { // 0c400002 cmpi.w #$2, d0
            next = 0xeef6U;
            const auto source_value = 0x2U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xeef6U: { // 6718 beq.b $ef10
            next = 0xeef8U;
            if ((r.status & 4U) != 0U) { next = 0xef10U; transfer_kind = 1U; }
            break;
        }
        case 0xeef8U: { // 426d0044 clr.w $44(a5)
            next = 0xeefcU;
            const auto destination_address = r.address[5] + 0x44U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xeefcU: { // 41f80c0a lea.l $c0a.w, a0
            next = 0xef00U;
            r.address[0] = 0xc0aU;
            break;
        }
        case 0xef00U: { // 0c500064 cmpi.w #$64, (a0)
            next = 0xef04U;
            const auto source_value = 0x64U;
            const auto destination_address = r.address[0];
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xef04U: { // 6a08 bpl.b $ef0e
            next = 0xef06U;
            if ((r.status & 8U) == 0U) { next = 0xef0eU; transfer_kind = 1U; }
            break;
        }
        case 0xef06U: { // 30fc0064 move.w #$64, (a0)+
            next = 0xef0aU;
            const auto value = 0x64U;
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            r.address[0] += 2U;
            m.logic(value, 16U);
            break;
        }
        case 0xef0aU: { // 30fc0100 move.w #$100, (a0)+
            next = 0xef0eU;
            const auto value = 0x100U;
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            r.address[0] += 2U;
            m.logic(value, 16U);
            break;
        }
        case 0xef10U: { // 3b7c000a0044 move.w #$a, $44(a5)
            next = 0xef16U;
            m.word(r.address[5] + 0x44U, 0xaU);
            m.logic(0xaU, 16U);
            break;
        }
        case 0xef16U: { // 296c00000040 move.l $0(a4), $40(a4)
            next = 0xef1cU;
            const auto source_address = r.address[4] + 0x0U;
            const auto value = m.lng(source_address);
            const auto destination_address = r.address[4] + 0x40U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xef1cU: { // 196c00040044 move.b $4(a4), $44(a4)
            next = 0xef22U;
            const auto source_address = r.address[4] + 0x4U;
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[4] + 0x44U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xef22U: { // 426c0000 clr.w $0(a4)
            next = 0xef26U;
            const auto destination_address = r.address[4] + 0x0U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xef26U: { // 56780c12 addq.w #$3, $c12.w
            next = 0xef2aU;
            const auto source_value = 0x3U;
            const auto destination_address = 0xc12U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_callback_state_table_dispatch_detail
