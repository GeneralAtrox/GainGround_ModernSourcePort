#include "cpu_b_initials_entry_detail.h"

namespace gain_ground::translated::cpu_b_initials_entry_detail {

bool dispatch_initials_region_02(FunctionContext &,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next)
{
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xf10eU: {
            next = 0xf112U; m.db(0U, m.byte(r.address[4] + 0x8bU)); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf112U: {
            next = 0xf114U; m.db(1U, r.data[0]); m.logic(r.data[1], 8U);
            break;
        }
        case 0xf114U: {
            next = 0xf118U; m.db(0U, r.data[0] & 0x60U); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf118U: {
            next = 0xf11aU; next = ((r.status & 4U) != 0U) ? 0xf122U : 0xf11aU;
            break;
        }
        case 0xf11aU: {
            next = 0xf11eU; m.db(1U, r.data[1] & 0x90U); m.logic(r.data[1], 8U);
            break;
        }
        case 0xf11eU: {
            next = 0xf122U; next = ((r.status & 4U) == 0U) ? 0xf1b8U : 0xf122U;
            break;
        }
        case 0xf122U: {
            next = 0xf126U; m.db(0U, m.byte(r.address[4] + 0x8cU)); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf126U: {
            next = 0xf12aU; m.db(1U, m.byte(r.address[4] + 0x8aU)); m.logic(r.data[1], 8U);
            break;
        }
        case 0xf12aU: {
            next = 0xf12eU; r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[1] & (1U << 5U)) ? 0U : 4U));
            break;
        }
        case 0xf130U: {
            next = 0xf134U; r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[0] & (1U << 6U)) ? 0U : 4U));
            break;
        }
        case 0xf136U: {
            next = 0xf13aU; r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[1] & (1U << 6U)) ? 0U : 4U));
            break;
        }
        case 0xf13cU: {
            next = 0xf140U; r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[0] & (1U << 5U)) ? 0U : 4U));
            break;
        }
        case 0xf142U: {
            next = 0xf146U; r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[1] & (1U << 4U)) ? 0U : 4U));
            break;
        }
        case 0xf148U: {
            next = 0xf14cU; r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[0] & (1U << 7U)) ? 0U : 4U));
            break;
        }
        case 0xf14eU: {
            next = 0xf152U; r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[1] & (1U << 7U)) ? 0U : 4U));
            break;
        }
        case 0xf154U: {
            next = 0xf158U; r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[0] & (1U << 4U)) ? 0U : 4U));
            break;
        }
        case 0xf12eU: {
            next = 0xf130U; next = ((r.status & 4U) == 0U) ? 0xf136U : 0xf130U;
            break;
        }
        case 0xf134U: {
            next = 0xf136U; next = ((r.status & 4U) == 0U) ? 0xf1a2U : 0xf136U;
            break;
        }
        case 0xf13aU: {
            next = 0xf13cU; next = ((r.status & 4U) == 0U) ? 0xf142U : 0xf13cU;
            break;
        }
        case 0xf140U: {
            next = 0xf142U; next = ((r.status & 4U) == 0U) ? 0xf1a2U : 0xf142U;
            break;
        }
        case 0xf146U: {
            next = 0xf148U; next = ((r.status & 4U) == 0U) ? 0xf14eU : 0xf148U;
            break;
        }
        case 0xf14cU: {
            next = 0xf14eU; next = ((r.status & 4U) == 0U) ? 0xf19eU : 0xf14eU;
            break;
        }
        case 0xf152U: {
            next = 0xf154U; next = ((r.status & 4U) == 0U) ? 0xf15aU : 0xf154U;
            break;
        }
        case 0xf158U: {
            next = 0xf15aU; next = ((r.status & 4U) == 0U) ? 0xf19eU : 0xf15aU;
            break;
        }
        case 0xf15aU: {
            next = 0xf15eU; m.db(0U, m.byte(r.address[4] + 0x8bU)); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf15eU: {
            next = 0xf160U; m.db(1U, r.data[0]); m.logic(r.data[1], 8U);
            break;
        }
        case 0xf160U: {
            next = 0xf164U; m.dw(0U, r.data[0] & 0x60U); m.logic(r.data[0], 16U);
            break;
        }
        case 0xf164U: {
            next = 0xf166U; next = ((r.status & 4U) == 0U) ? 0xf176U : 0xf166U;
            break;
        }
        case 0xf166U: {
            next = 0xf16aU; m.dw(1U, r.data[1] & 0x90U); m.logic(r.data[1], 16U);
            break;
        }
        case 0xf16aU: {
            next = 0xf16cU; next = ((r.status & 4U) != 0U) ? 0xf198U : 0xf16cU;
            break;
        }
        case 0xf16cU: {
            next = 0xf170U; const auto value = m.sub(m.word(r.address[4] + 0xa0U), 1U, 16U); m.word(r.address[4] + 0xa0U, value);
            break;
        }
        case 0xf170U: {
            next = 0xf172U; next = ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) ? 0xf198U : 0xf172U;
            break;
        }
        case 0xf172U: {
            next = 0xf174U; r.data[0] = 0xffffffffU; m.logic(r.data[0], 32U);
            break;
        }
        case 0xf174U: {
            next = 0xf17eU; 
            break;
        }
        case 0xf176U: {
            next = 0xf17aU; const auto value = m.sub(m.word(r.address[4] + 0xa0U), 1U, 16U); m.word(r.address[4] + 0xa0U, value);
            break;
        }
        case 0xf17aU: {
            next = 0xf17cU; next = ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) ? 0xf198U : 0xf17cU;
            break;
        }
        case 0xf17cU: {
            next = 0xf17eU; r.data[0] = 1U; m.logic(r.data[0], 32U);
            break;
        }
        case 0xf17eU: {
            next = 0xf182U; m.dw(1U, m.word(r.address[4] + 0x96U)); m.logic(r.data[1], 16U);
            break;
        }
        case 0xf182U: {
            next = 0xf186U; r.address[0] = r.address[4] + 0x98U;
            break;
        }
        case 0xf186U: {
            next = 0xf18aU; const auto address = r.address[0] + static_cast<std::int16_t>(r.data[1]); const auto value = m.add(m.byte(address), r.data[0], 8U); m.byte(address, value);
            break;
        }
        case 0xf192U: {
            next = 0xf198U; m.word(r.address[4] + 0xa0U, 3U); m.logic(3U, 16U);
            break;
        }
        case 0xf19eU: {
            next = 0xf1a0U; r.data[0] = 0xffffffffU; m.logic(r.data[0], 32U);
            break;
        }
        case 0xf1a0U: {
            next = 0xf1a4U; 
            break;
        }
        case 0xf1a2U: {
            next = 0xf1a4U; r.data[0] = 1U; m.logic(r.data[0], 32U);
            break;
        }
        case 0xf1a4U: {
            next = 0xf1a8U; m.dw(1U, m.word(r.address[4] + 0x96U)); m.logic(r.data[1], 16U);
            break;
        }
        case 0xf1a8U: {
            next = 0xf1acU; r.address[0] = r.address[4] + 0x98U;
            break;
        }
        case 0xf1acU: {
            next = 0xf1b0U; const auto address = r.address[0] + static_cast<std::int16_t>(r.data[1]); const auto value = m.add(m.byte(address), r.data[0], 8U); m.byte(address, value);
            break;
        }
        case 0xf1b8U: {
            next = 0xf1beU; m.word(r.address[4] + 0xa0U, 0x19U); m.logic(0x19U, 16U);
            break;
        }
        case 0xf1c4U: {
            next = 0xf1c8U; r.address[0] = r.address[4] + 0x98U;
            break;
        }
        case 0xf1c8U: {
            next = 0xf1caU; r.data[1] = 3U; m.logic(r.data[1], 32U);
            break;
        }
        case 0xf1caU: {
            next = 0xf1ccU; const auto value = m.byte(r.address[0]); ++r.address[0]; m.db(0U, value); m.logic(r.data[0], 8U);
            break;
        }
        case 0xf1ccU: {
            next = 0xf1d0U; (void)m.sub(r.data[0], 0x1cU, 8U, true);
            break;
        }
        case 0xf1d0U: {
            next = 0xf1d2U; next = ((r.status & 4U) != 0U) ? 0xf1d8U : 0xf1d2U;
            break;
        }
        case 0xf1d2U: {
            next = 0xf1d6U; (void)m.sub(r.data[0], 0x1dU, 8U, true);
            break;
        }
        case 0xf1d6U: {
            next = 0xf1d8U; next = ((r.status & 4U) == 0U) ? 0xf1deU : 0xf1d8U;
            break;
        }
        case 0xf1d8U: {
            next = 0xf1deU; m.byte(r.address[0] - 1U, 0x1aU); m.logic(0x1aU, 8U);
            break;
        }
        case 0xf1deU: {
            next = 0xf1e2U; m.dw(1U, r.data[1] - 1U); if ((r.data[1] & 0xffffU) != 0xffffU) next = 0xf1caU;
            break;
        }
        case 0xf1eaU: {
            next = 0xf1f0U; r.address[0] = 0x200682U;
            break;
        }
        case 0xf1f0U: {
            next = 0xf1f4U; r.address[0] += static_cast<std::int16_t>(m.word(r.address[5] + 0x7aU));
            break;
        }
        case 0xf1f4U: {
            next = 0xf1f6U; const auto address = r.address[0]; (void)m.lng(address); m.word(address + 2U, 0U); m.word(address, 0U); m.logic(0U, 32U);
            break;
        }
        case 0xf1f6U: {
            next = 0xf1faU; const auto address = r.address[0] + 0x80U; (void)m.lng(address); m.word(address + 2U, 0U); m.word(address, 0U); m.logic(0U, 32U);
            break;
        }
        case 0xf1faU: {
            next = 0xf1feU; r.address[0] = r.address[4] + 0x98U;
            break;
        }
        case 0xf1feU: {
            next = 0xf202U; r.address[1] = 0x106b2U;
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_initials_entry_detail
