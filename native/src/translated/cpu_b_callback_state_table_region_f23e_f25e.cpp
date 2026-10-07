#include "cpu_b_callback_state_table_dispatch_detail.h"

namespace gain_ground::translated::cpu_b_callback_state_table_dispatch_detail {

bool dispatch_table_region_05(FunctionContext &,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind)
{
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xf23eU: { // 41f87b18 lea.l $7b18.w, a0
            next = 0xf242U;
            r.address[0] = 0x7b18U;
            break;
        }
        case 0xf242U: { // 43e8fff8 lea.l -$8(a0), a1
            next = 0xf246U;
            const auto source_address = r.address[0] - 0x8U;
            r.address[1] = source_address;
            break;
        }
        case 0xf246U: { // 4a41 tst.w d1
            next = 0xf248U;
            const auto value = r.data[1];
            m.logic(value, 16U);
            break;
        }
        case 0xf248U: { // 6b08 bmi.b $f252
            next = 0xf24aU;
            if ((r.status & 8U) != 0U) { next = 0xf252U; transfer_kind = 1U; }
            break;
        }
        case 0xf24aU: { // 2121 move.l -(a1), -(a0)
            next = 0xf24cU;
            r.address[1] -= 4U;
            const auto source_address = r.address[1];
            const auto value = m.lng(source_address);
            r.address[0] -= 4U;
            const auto destination_address = r.address[0];
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf24cU: { // 2121 move.l -(a1), -(a0)
            next = 0xf24eU;
            r.address[1] -= 4U;
            const auto source_address = r.address[1];
            const auto value = m.lng(source_address);
            r.address[0] -= 4U;
            const auto destination_address = r.address[0];
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf24eU: { // 51c9fffa dbra d1, $f24a
            next = 0xf252U;
            m.dw(1U, r.data[1] - 1U);
            if ((r.data[1] & 0xffffU) != 0xffffU) next = 0xf24aU;
            break;
        }
        case 0xf252U: { // 22c0 move.l d0, (a1)+
            next = 0xf254U;
            const auto value = r.data[0];
            const auto destination_address = r.address[1];
            m.lng(destination_address, value);
            r.address[1] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0xf254U: { // 22ac0098 move.l $98(a4), (a1)
            next = 0xf258U;
            const auto source_address = r.address[4] + 0x98U;
            const auto value = m.lng(source_address);
            const auto destination_address = r.address[1];
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf258U: { // 3b7c00020044 move.w #$2, $44(a5)
            next = 0xf25eU;
            m.word(r.address[5] + 0x44U, 0x2U);
            m.logic(0x2U, 16U);
            break;
        }
        case 0xf25eU: { // 3b7c00780046 move.w #$78, $46(a5)
            next = 0xf264U;
            m.word(r.address[5] + 0x46U, 0x78U);
            m.logic(0x78U, 16U);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_callback_state_table_dispatch_detail
