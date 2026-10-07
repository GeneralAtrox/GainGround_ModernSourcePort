#include "cpu_b_initials_entry_detail.h"

namespace gain_ground::translated::cpu_b_initials_entry_detail {

bool dispatch_initials_region_04(FunctionContext &,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next)
{
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xf302U: {
            next = 0xf308U; r.address[0] = 0x200202U;
            break;
        }
        case 0xf308U: {
            next = 0xf30aU; m.asl_word(1U, 8U);
            break;
        }
        case 0xf30aU: {
            next = 0xf30cU; r.address[0] += static_cast<std::int16_t>(r.data[1]);
            break;
        }
        case 0xf30cU: {
            next = 0xf310U; r.address[0] += static_cast<std::int16_t>(m.word(r.address[5] + 0x7aU));
            break;
        }
        case 0xf318U: {
            next = 0xf31cU; m.dw(1U, m.word(r.address[4] + 0x96U)); m.logic(r.data[1], 16U);
            break;
        }
        case 0xf31cU: {
            next = 0xf320U; r.address[0] = r.address[4] + 0x98U;
            break;
        }
        case 0xf320U: {
            next = 0xf322U; r.address[0] += static_cast<std::int16_t>(r.data[1]);
            break;
        }
        case 0xf322U: {
            next = 0xf324U; m.db(0U, m.byte(r.address[0])); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf324U: {
            next = 0xf328U; r.address[1] = 0x1066eU;
            break;
        }
        case 0xf328U: {
            next = 0xf32aU; m.dw(1U, m.add(r.data[1], r.data[1], 16U));
            break;
        }
        case 0xf32aU: {
            next = 0xf32cU; r.address[1] += static_cast<std::int16_t>(r.data[1]);
            break;
        }
        case 0xf32cU: {
            next = 0xf32eU; const auto value = m.byte(r.address[1]); ++r.address[1]; (void)m.sub(r.data[0], value, 8U, true);
            break;
        }
        case 0xf32eU: {
            next = 0xf330U; next = ((r.status & 8U) == 0U) ? 0xf334U : 0xf330U;
            break;
        }
        case 0xf330U: {
            next = 0xf332U; m.db(0U, m.byte(r.address[1])); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf332U: {
            next = 0xf33aU; 
            break;
        }
        case 0xf334U: {
            next = 0xf336U; (void)m.sub(r.data[0], m.byte(r.address[1]), 8U, true);
            break;
        }
        case 0xf336U: {
            next = 0xf338U; next = ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) ? 0xf33aU : 0xf338U;
            break;
        }
        case 0xf338U: {
            next = 0xf33aU; --r.address[1]; m.db(0U, m.byte(r.address[1])); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf33aU: {
            next = 0xf33cU; m.byte(r.address[0], r.data[0]); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf33eU: {
            next = 0xf344U; r.address[0] = 0x200200U;
            break;
        }
        case 0xf344U: {
            next = 0xf348U; r.address[0] += static_cast<std::int16_t>(m.word(r.address[5] + 0x7aU));
            break;
        }
        case 0xf348U: {
            next = 0xf34cU; m.dw(0U, m.word(r.address[4] + 0x96U)); m.logic(r.data[0], 16U);
            break;
        }
        case 0xf34cU: {
            next = 0xf34eU; m.asl_word(0U, 8U);
            break;
        }
        case 0xf34eU: {
            next = 0xf350U; r.address[0] += static_cast<std::int16_t>(r.data[0]);
            break;
        }
        case 0xf350U: {
            next = 0xf354U; m.word(r.address[0], 0xb0U); m.logic(0xb0U, 16U);
            break;
        }
        case 0xf354U: {
            next = 0xf35aU; m.word(r.address[0] + 0x80U, 0xb1U); m.logic(0xb1U, 16U);
            break;
        }
        case 0xf35cU: {
            next = 0xf362U; r.address[0] = 0x200200U;
            break;
        }
        case 0xf362U: {
            next = 0xf366U; r.address[0] += static_cast<std::int16_t>(m.word(r.address[5] + 0x7aU));
            break;
        }
        case 0xf366U: {
            next = 0xf368U; r.data[0] = 7U; m.logic(r.data[0], 32U);
            break;
        }
        case 0xf368U: {
            next = 0xf36aU; r.data[1] = 0U; m.logic(r.data[1], 32U);
            break;
        }
        case 0xf36aU: {
            next = 0xf36cU; m.word(r.address[0], r.data[1]); m.logic(r.data[1], 16U);
            break;
        }
        case 0xf36cU: {
            next = 0xf370U; r.address[0] += 0x80U;
            break;
        }
        case 0xf370U: {
            next = 0xf374U; m.dw(0U, r.data[0] - 1U); if ((r.data[0] & 0xffffU) != 0xffffU) next = 0xf36aU;
            break;
        }
        case 0xf38eU: {
            next = 0xf392U; m.dw(0U, m.word(r.address[4] + 0x9cU)); m.logic(r.data[0], 16U);
            break;
        }
        case 0xf392U: {
            next = 0xf396U; m.dw(0U, r.data[0] & 3U); m.logic(r.data[0], 16U);
            break;
        }
        case 0xf396U: {
            next = 0xf398U; next = ((r.status & 4U) == 0U) ? 0xf3aaU : 0xf398U;
            break;
        }
        case 0xf398U: {
            next = 0xf39eU; const auto value = m.byte(r.address[4] + 0x9dU); r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((value & 4U) ? 0U : 4U));
            break;
        }
        case 0xf39eU: {
            next = 0xf3a0U; next = ((r.status & 4U) != 0U) ? 0xf3a6U : 0xf3a0U;
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_initials_entry_detail
