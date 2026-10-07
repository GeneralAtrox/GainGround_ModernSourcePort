#include "cpu_b_callback_state_table_dispatch_detail.h"

namespace gain_ground::translated::cpu_b_callback_state_table_dispatch_detail {

bool dispatch_table_region_04(FunctionContext &,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind)
{
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xf05aU: { // 536d0046 subq.w #$1, $46(a5)
            next = 0xf05eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x46U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xf05eU: { // 6f12 ble.b $f072
            next = 0xf060U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0xf072U; transfer_kind = 1U; }
            break;
        }
        case 0xf060U: { // 0c6d005a0046 cmpi.w #$5a, $46(a5)
            next = 0xf066U;
            const auto source_value = 0x5aU;
            const auto destination_address = r.address[5] + 0x46U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xf066U: { // 642a bcc.b $f092
            next = 0xf068U;
            if ((r.status & 1U) == 0U) { next = 0xf092U; transfer_kind = 1U; }
            break;
        }
        case 0xf068U: { // 102c008d move.b $8d(a4), d0
            next = 0xf06cU;
            const auto source_address = r.address[4] + 0x8dU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf06cU: { // 02000006 andi.b #$6, d0
            next = 0xf070U;
            const auto source_value = 0x6U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0xf070U: { // 6720 beq.b $f092
            next = 0xf072U;
            if ((r.status & 4U) != 0U) { next = 0xf092U; transfer_kind = 1U; }
            break;
        }
        case 0xf072U: { // 102d006d move.b $6d(a5), d0
            next = 0xf076U;
            const auto source_address = r.address[5] + 0x6dU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf076U: { // 01b80c04 bclr.b d0, $c04.w
            next = 0xf07aU;
            const auto destination_address = 0xc04U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (r.data[0] & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0xf07aU: { // 426d0042 clr.w $42(a5)
            next = 0xf07eU;
            const auto destination_address = r.address[5] + 0x42U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf086U: { // 102d006d move.b $6d(a5), d0
            next = 0xf08aU;
            const auto source_address = r.address[5] + 0x6dU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf08aU: { // d000 add.b d0, d0
            next = 0xf08cU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0xf08cU: { // 13c000d00035 move.b d0, $d00035.l
            next = 0xf092U;
            const auto value = r.data[0];
            const auto destination_address = 0xd00035U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_callback_state_table_dispatch_detail
