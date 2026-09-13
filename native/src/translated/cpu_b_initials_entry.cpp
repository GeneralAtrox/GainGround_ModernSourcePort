// Implemented but unverified. Original F175 initials-entry and its reached
// helpers, translated from retained state-72 bytes. No new renderer or clock.
#include "cpu_b_initials_entry.h"
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult cpu_b_initials_entry(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
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
        case 0xf0b4U: {
            next = 0xf0b8U; const auto child = m.call(c, 526U, pc, 0xf2a0U, 0xf0b8U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
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
        case 0xf100U: {
            next = 0xf104U; const auto child = m.call(c, 175U, pc, 0xf318U, 0xf104U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf104U: {
            next = 0xf108U; const auto child = m.call(c, 527U, pc, 0xf2b4U, 0xf108U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf108U: {
            next = 0xf10cU; const auto child = m.call(c, 175U, pc, 0xf38eU, 0xf10cU); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
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
        case 0xf18aU: {
            next = 0xf18eU; const auto child = m.call(c, 175U, pc, 0xf318U, 0xf18eU); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf18eU: {
            next = 0xf192U; const auto child = m.call(c, 175U, pc, 0xf2e6U, 0xf192U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf192U: {
            next = 0xf198U; m.word(r.address[4] + 0xa0U, 3U); m.logic(3U, 16U);
            break;
        }
        case 0xf198U: {
            next = 0xf19cU; const auto child = m.call(c, 175U, pc, 0xf38eU, 0xf19cU); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
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
        case 0xf1b0U: {
            next = 0xf1b4U; const auto child = m.call(c, 175U, pc, 0xf318U, 0xf1b4U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf1b4U: {
            next = 0xf1b8U; const auto child = m.call(c, 175U, pc, 0xf2e6U, 0xf1b8U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf1b8U: {
            next = 0xf1beU; m.word(r.address[4] + 0xa0U, 0x19U); m.logic(0x19U, 16U);
            break;
        }
        case 0xf1beU: {
            next = 0xf1c2U; const auto child = m.call(c, 175U, pc, 0xf38eU, 0xf1c2U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
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
        case 0xf1e2U: {
            next = 0xf1e6U; const auto child = m.call(c, 527U, pc, 0xf2b4U, 0xf1e6U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf1e6U: {
            next = 0xf1eaU; const auto child = m.call(c, 175U, pc, 0xf35cU, 0xf1eaU); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
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
        case 0xf310U: {
            next = 0xf316U; const auto child = m.call(c, 298U, pc, 0x161eaU, 0xf316U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
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
        case 0xf3a0U: {
            next = 0xf3a4U; const auto child = m.call(c, 175U, pc, 0xf33eU, 0xf3a4U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf3a6U: {
            next = 0xf3aaU; const auto child = m.call(c, 175U, pc, 0xf35cU, 0xf3aaU); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf10cU:
        case 0xf19cU:
        case 0xf1c2U:
        case 0xf316U:
        case 0xf33cU:
        case 0xf35aU:
        case 0xf374U:
        case 0xf3a4U:
        case 0xf3aaU: {
            const auto result = m.ret();
            if (auto event = m.interrupt(c, pc, r.program_counter)) return *event;
            return result;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        // Original fallthrough/branch into the existing score-insertion tail.
        if (next == 0xf23eU || next == 0xf258U)
            return cpu_b_callback_state_table_dispatch(c);
    }
}
} // namespace gain_ground::translated
