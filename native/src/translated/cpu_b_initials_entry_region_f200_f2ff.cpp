#include "cpu_b_initials_entry_detail.h"

namespace gain_ground::translated::cpu_b_initials_entry_detail {

bool dispatch_initials_region_03(FunctionContext &,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next)
{
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xf202U: {
            next = 0xf204U; r.data[0] = 0U; m.logic(r.data[0], 32U);
            break;
        }
        case 0xf204U: {
            next = 0xf206U; m.db(0U, m.byte(r.address[0])); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf20aU: {
            next = 0xf20cU; m.db(0U, m.byte(r.address[0])); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf210U: {
            next = 0xf212U; m.db(0U, m.byte(r.address[0])); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf206U: {
            next = 0xf20aU; const auto value = m.byte(r.address[1] + static_cast<std::int16_t>(r.data[0])); m.byte(r.address[0], value); ++r.address[0]; m.logic(value, 8U);
            break;
        }
        case 0xf20cU: {
            next = 0xf210U; const auto value = m.byte(r.address[1] + static_cast<std::int16_t>(r.data[0])); m.byte(r.address[0], value); ++r.address[0]; m.logic(value, 8U);
            break;
        }
        case 0xf212U: {
            next = 0xf216U; const auto value = m.byte(r.address[1] + static_cast<std::int16_t>(r.data[0])); m.byte(r.address[0], value); ++r.address[0]; m.logic(value, 8U);
            break;
        }
        case 0xf216U: {
            next = 0xf21aU; const auto value = m.byte(0xc03U); m.byte(r.address[0], value); m.logic(value, 8U);
            break;
        }
        case 0xf21aU: {
            next = 0xf21eU; r.data[0] = m.lng(r.address[4] + 0x80U); m.logic(r.data[0], 32U);
            break;
        }
        case 0xf21eU: {
            next = 0xf222U; r.address[0] = 0x7800U;
            break;
        }
        case 0xf222U: {
            next = 0xf224U; r.data[1] = 0x62U; m.logic(r.data[1], 32U);
            break;
        }
        case 0xf224U: {
            next = 0xf226U; r.data[2] = 0U; m.logic(r.data[2], 32U);
            break;
        }
        case 0xf226U: {
            next = 0xf228U; (void)m.sub(r.data[0], m.lng(r.address[0]), 32U, true);
            break;
        }
        case 0xf228U: {
            next = 0xf22aU; next = ((r.status & 1U) == 0U) ? 0xf238U : 0xf22aU;
            break;
        }
        case 0xf22aU: {
            next = 0xf22cU; r.address[0] += 8U;
            break;
        }
        case 0xf22cU: {
            next = 0xf22eU; m.dw(2U, m.add(r.data[2], 1U, 16U));
            break;
        }
        case 0xf22eU: {
            next = 0xf232U; m.dw(1U, r.data[1] - 1U); if ((r.data[1] & 0xffffU) != 0xffffU) next = 0xf226U;
            break;
        }
        case 0xf232U: {
            next = 0xf236U; const auto value = m.byte(0x836U); m.byte(0x836U, value | 0x80U); m.logic(value, 8U);
            break;
        }
        case 0xf236U: {
            next = 0xf258U; 
            break;
        }
        case 0xf238U: {
            next = 0xf23cU; m.word(0x836U, r.data[2]); m.logic(r.data[2], 16U);
            break;
        }
        case 0xf23cU: {
            next = 0xf23eU; m.dw(1U, m.sub(r.data[1], 1U, 16U));
            break;
        }
        case 0xf2e6U: {
            next = 0xf2eaU; m.dw(1U, m.word(r.address[4] + 0x96U)); m.logic(r.data[1], 16U);
            break;
        }
        case 0xf2eaU: {
            next = 0xf2eeU; r.address[0] = r.address[4] + 0x98U;
            break;
        }
        case 0xf2eeU: {
            next = 0xf2f0U; r.data[0] = 0U; m.logic(r.data[0], 32U);
            break;
        }
        case 0xf2f0U: {
            next = 0xf2f4U; m.db(0U, m.byte(r.address[0] + static_cast<std::int16_t>(r.data[1]))); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf2f4U: {
            next = 0xf2f6U; m.dw(0U, m.add(r.data[0], r.data[0], 16U));
            break;
        }
        case 0xf2f6U: {
            next = 0xf2faU; r.address[0] = 0x10676U;
            break;
        }
        case 0xf2faU: {
            next = 0xf2feU; m.dw(0U, m.word(r.address[0] + static_cast<std::int16_t>(r.data[0]))); m.logic(r.data[0], 16U);
            break;
        }
        case 0xf2feU: {
            next = 0xf302U; m.dw(0U, r.data[0] | m.word(r.address[5] + 0x62U)); m.logic(r.data[0], 16U);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_initials_entry_detail
