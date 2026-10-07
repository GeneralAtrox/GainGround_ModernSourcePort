#include "cpu_b_initials_entry_detail.h"

namespace gain_ground::translated::cpu_b_initials_entry_detail {

bool dispatch_initials_region_01(FunctionContext &,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next)
{
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xf094U: {
            next = 0xf098U; const auto value = m.sub(m.word(r.address[4] + 0x9cU), 1U, 16U); m.word(r.address[4] + 0x9cU, value);
            break;
        }
        case 0xf098U: {
            next = 0xf09aU; next = ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) ? 0xf0b8U : 0xf09aU;
            break;
        }
        case 0xf09aU: {
            next = 0xf09eU; m.dw(0U, m.word(r.address[4] + 0x9eU)); m.logic(r.data[0], 16U);
            break;
        }
        case 0xf09eU: {
            next = 0xf0a0U; r.data[1] = 1U; m.logic(r.data[1], 32U);
            break;
        }
        case 0xf0a0U: {
            next = 0xf0a4U; r.status = static_cast<std::uint16_t>(r.status & ~0x1fU);
            break;
        }
        case 0xf0a4U: {
            next = 0xf0a6U; m.db(0U, m.sbcd(r.data[0], r.data[1]));
            break;
        }
        case 0xf0a6U: {
            next = 0xf0aaU; next = ((r.status & 8U) != 0U) ? 0xf1c4U : 0xf0aaU;
            break;
        }
        case 0xf0aaU: {
            next = 0xf0aeU; m.word(r.address[4] + 0x9eU, r.data[0]); m.logic(r.data[0], 16U);
            break;
        }
        case 0xf0aeU: {
            next = 0xf0b4U; m.word(r.address[4] + 0x9cU, 0x20U); m.logic(0x20U, 16U);
            break;
        }
        case 0xf0b8U: {
            next = 0xf0bcU; m.db(0U, m.byte(r.address[4] + 0x8cU)); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf0bcU: {
            next = 0xf0c0U; m.db(0U, r.data[0] & 6U); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf0c0U: {
            next = 0xf0c2U; next = ((r.status & 4U) != 0U) ? 0xf10eU : 0xf0c2U;
            break;
        }
        case 0xf0c2U: {
            next = 0xf0c6U; m.dw(1U, m.word(r.address[4] + 0x96U)); m.logic(r.data[1], 16U);
            break;
        }
        case 0xf0c6U: {
            next = 0xf0caU; r.address[0] = r.address[4] + 0x98U;
            break;
        }
        case 0xf0caU: {
            next = 0xf0ceU; m.db(0U, m.byte(r.address[0] + static_cast<std::int16_t>(r.data[1]))); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf0ceU: {
            next = 0xf0d2U; (void)m.sub(r.data[0], 0x1cU, 8U, true);
            break;
        }
        case 0xf0d2U: {
            next = 0xf0d6U; next = ((r.status & 4U) != 0U) ? 0xf1c4U : 0xf0d6U;
            break;
        }
        case 0xf0d6U: {
            next = 0xf0daU; (void)m.sub(r.data[0], 0x1dU, 8U, true);
            break;
        }
        case 0xf0daU: {
            next = 0xf0dcU; next = ((r.status & 4U) == 0U) ? 0xf0e6U : 0xf0dcU;
            break;
        }
        case 0xf0dcU: {
            next = 0xf0e2U; m.byte(r.address[0] + static_cast<std::int16_t>(r.data[1]), 0x1aU); m.logic(0x1aU, 8U);
            break;
        }
        case 0xf0e2U: {
            next = 0xf0e4U; m.dw(1U, m.sub(r.data[1], 1U, 16U));
            break;
        }
        case 0xf0e4U: {
            next = 0xf0f2U; 
            break;
        }
        case 0xf0e6U: {
            next = 0xf0e8U; m.dw(1U, m.add(r.data[1], 1U, 16U));
            break;
        }
        case 0xf0e8U: {
            next = 0xf0ecU; (void)m.sub(r.data[1], 3U, 16U, true);
            break;
        }
        case 0xf0ecU: {
            next = 0xf0eeU; next = ((r.status & 4U) == 0U) ? 0xf0f2U : 0xf0eeU;
            break;
        }
        case 0xf0eeU: {
            next = 0xf0f2U; m.db(0U, 0x1cU); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf0f2U: {
            next = 0xf0f6U; m.word(r.address[4] + 0x96U, r.data[1]); m.logic(r.data[1], 16U);
            break;
        }
        case 0xf0f6U: {
            next = 0xf0faU; m.byte(r.address[0] + static_cast<std::int16_t>(r.data[1]), r.data[0]); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf0faU: {
            next = 0xf100U; m.word(r.address[4] + 0xa0U, 0x19U); m.logic(0x19U, 16U);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_initials_entry_detail
