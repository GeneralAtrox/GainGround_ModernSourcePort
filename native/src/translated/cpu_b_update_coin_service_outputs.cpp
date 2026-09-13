// Implemented but unverified. Original coin/service selector paths and output updates.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_update_record_from_table_8368(FunctionContext &) noexcept;
namespace {
FunctionResult finish_add_chain(FunctionContext &c, FunctionResult child) {
    if (child.status == TranslationStatus::complete && child.control == 3U &&
        child.exit_program_counter == 0x8368U && c.registers.program_counter == 0x8368U)
        return cpu_b_update_record_from_table_8368(c);
    return child;
}
// Actual BRA.B targets from retained state-72 opcodes at 82aa..82c9.
constexpr std::array<std::uint32_t, 16> coin_targets{
    0x82d4U, 0x82d2U, 0x82d0U, 0x82ceU, 0x82ccU, 0x82caU, 0x82dcU, 0x82e4U,
    0x82eeU, 0x82f8U, 0x8316U, 0x8316U, 0x8316U, 0x8316U, 0x8316U, 0x82d4U};
}
FunctionResult cpu_b_update_coin_service_outputs(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U}; CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter; auto next = pc;
        if (pc >= 0x82aaU && pc <= 0x82c8U && !(pc & 1U)) {
            next = t.branch(pc, coin_targets[(pc - 0x82aaU) / 2U], true);
        } else switch (pc) {
        case 0x8264U: case 0x826eU: case 0x8274U: case 0x8286U: case 0x82a2U:
        case 0x8308U: case 0x8312U: case 0x8326U: case 0x8336U: case 0x8348U: case 0x8350U: {
            const auto reg = pc == 0x8264U || pc == 0x826eU ? 5U : pc == 0x8274U ? 6U :
                pc == 0x8286U ? 7U : 1U;
            const auto value = pc == 0x826eU || pc == 0x8308U || pc == 0x8350U ? 1U :
                pc == 0x8286U ? 2U : pc == 0x8312U ? 3U : 0U;
            r.data[reg] = value; m.logic(value, 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x8266U:
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((t.byte(0x81eU) & 8U) ? 0U : 4U));
            t.prefetch(pc + 8U); next = pc + 6U; break;
        case 0x826cU: next = t.branch(pc, 0x8274U, (r.status & 4U) != 0U); break;
        case 0x8270U: {
            t.prefetch(pc + 4U); const auto value = m.add(t.word(0x7b28U), r.data[5], 16U);
            t.prefetch(pc + 6U); t.word(0x7b28U, static_cast<std::uint16_t>(value)); next = pc + 4U; break;
        }
        case 0x8276U: {
            t.prefetch(pc + 4U); const auto value = t.byte(0x405U); m.db(6U, value); m.logic(value, 8U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x827aU: case 0x827eU: case 0x8282U: case 0x831aU: {
            const auto reg = pc == 0x827aU ? 6U : pc == 0x827eU ? 5U : pc == 0x8282U ? 1U : 2U;
            r.address[reg] = pc == 0x827aU ? 0x83eaU : pc == 0x827eU ? 0x410U : pc == 0x8282U ? 0x412U : 0x840eU;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x8288U: case 0x828aU: case 0x828cU: case 0x8344U: case 0x8346U: {
            const auto reg = pc == 0x8288U ? 0U : pc == 0x828aU || pc == 0x8344U ? 4U : 2U;
            const auto source = r.address[6]; r.address[6] += 2U;
            r.address[reg] = static_cast<std::uint32_t>(static_cast<std::int16_t>(t.word(source)));
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x828eU: case 0x833cU: {
            const auto value = t.byte(r.address[pc == 0x828eU ? 2U : 3U]);
            m.db(pc == 0x828eU ? 0U : 1U, value); m.logic(value, 8U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x8290U: {
            const auto source = r.address[6]; r.address[6] += 2U;
            const auto value = t.word(source); m.dw(0U, r.data[0] & value); m.logic(r.data[0], 16U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x8292U: next = t.branch_word(pc, 0x8344U, (r.status & 4U) != 0U); break;
        case 0x8296U:
            (void)t.byte(r.address[5]); t.prefetch(pc + 4U); t.byte(r.address[5], 0xffU);
            next = pc + 2U; break; // ST leaves CCR untouched.
        case 0x8298U: case 0x8352U: {
            const auto address = r.address[4]; const auto old = t.word(address);
            const auto value = m.add(old, pc == 0x8298U ? 1U : r.data[1], 16U);
            t.prefetch(pc + 4U); t.word(address, static_cast<std::uint16_t>(value)); next = pc + 2U; break;
        }
        case 0x829aU:
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); m.logic(0x11U, 8U);
            t.prefetch(pc + 8U); t.byte(0xd00035U, 0x11U); t.prefetch(pc + 10U); next = pc + 8U; break;
        case 0x82a4U:
            m.dw(2U, r.data[6]); m.logic(r.data[2], 16U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x82a6U:
            t.clocks(2U); t.clocks(2U);
            next = (0x82aaU + static_cast<std::int16_t>(r.data[2])) & 0xffffffU;
            if (next & 1U) return {TranslationStatus::contract_violation, 0U, next};
            t.clocks(2U); t.prefetch(next); t.prefetch(next + 2U);
            // Preserve the calculation even beyond the nominal selector table.
            // A listed local instruction can be entered directly; unknown
            // local instruction boundaries fail in the PC switch below.
            if (next < 0x8264U || next > 0x8362U) {
                r.program_counter = next; t.stop();
                if (const auto event = m.interrupt(c, pc, next)) return *event;
                return m.dispatch(c, pc, next, 1U, m.state);
            }
            break;
        case 0x82caU: case 0x82ccU: case 0x82ceU: case 0x82d0U: case 0x82d2U: case 0x82d4U: case 0x832cU:
            m.db(1U, m.add(r.data[1], 1U, 8U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x82d6U: case 0x830aU: case 0x8340U: {
            // 830a is 6158 (BSR.B), so its pushed return is 830c, not 830e.
            const auto child = t.bsr(pc, 112U, 0x8364U, pc + (pc == 0x82d6U ? 4U : 2U), finish_add_chain);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x82daU: case 0x8302U: case 0x830cU: next = t.branch(pc, 0x8342U, true); break;
        case 0x82dcU: case 0x82f8U:
            t.prefetch(pc + 4U); m.logic(t.byte(r.address[0] + 1U), 8U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x82e0U: next = t.branch(pc, 0x8304U, !(r.status & 4U)); break;
        case 0x82e2U: case 0x82ecU: case 0x82f6U: next = t.branch(pc, 0x82feU, true); break;
        case 0x82e4U: case 0x82eeU:
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            (void)m.sub(t.byte(r.address[0] + 1U), pc == 0x82e4U ? 2U : 3U, 8U, true);
            t.prefetch(pc + 8U); next = pc + 6U; break;
        case 0x82eaU: case 0x82f4U:
            next = t.branch(pc, 0x8304U, (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U); break;
        case 0x82fcU: next = t.branch(pc, 0x830eU, !(r.status & 4U)); break;
        case 0x82feU: {
            t.prefetch(pc + 4U); const auto address = r.address[0] + 1U;
            const auto value = m.add(t.byte(address), 1U, 8U);
            t.prefetch(pc + 6U); t.byte(address, static_cast<std::uint8_t>(value)); next = pc + 4U; break;
        }
        case 0x8304U: case 0x830eU: {
            t.prefetch(pc + 4U); const auto address = r.address[0] + 1U;
            (void)t.byte(address); m.logic(0U, 8U); t.prefetch(pc + 6U); t.byte(address, 0U);
            next = pc + 4U; break;
        }
        case 0x8314U: next = t.branch(pc, 0x830aU, true); break;
        case 0x8316U:
            t.prefetch(pc + 4U); m.db(2U, m.sub(r.data[2], 0x14U, 8U));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x831eU: {
            const auto source = r.address[2] + static_cast<std::int16_t>(r.data[2]);
            t.clocks(2U); t.prefetch(pc + 4U); const auto value = t.word(source);
            m.dw(2U, value); m.logic(value, 16U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x8322U: case 0x8330U: {
            const auto reg = pc == 0x8322U ? 2U : 3U;
            // 45f2 2000 / 47f2 1000 use signed word indexes, including 0x80..ff.
            const auto value = r.address[2] + static_cast<std::int16_t>(r.data[pc == 0x8322U ? 2U : 1U]);
            t.clocks(2U); t.prefetch(pc + 4U); r.address[reg] = value;
            t.clocks(2U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x8328U: {
            t.prefetch(pc + 4U); const auto value = t.byte(r.address[0] + 2U);
            m.db(1U, value); m.logic(value, 8U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x832eU:
            (void)m.sub(r.data[1], t.byte(r.address[2]), 8U, true);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x8334U:
            next = t.branch(pc, 0x8338U, (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U); break;
        case 0x8338U:
            t.prefetch(pc + 4U); m.logic(r.data[1], 8U);
            t.byte(r.address[0] + 2U, static_cast<std::uint8_t>(r.data[1]));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x833eU: next = t.branch(pc, 0x8342U, (r.status & 4U) != 0U); break;
        case 0x8342U: {
            const auto address = r.address[1]; const auto value = m.add(t.byte(address), 1U, 8U);
            t.prefetch(pc + 4U); t.byte(address, static_cast<std::uint8_t>(value)); next = pc + 2U; break;
        }
        case 0x834aU:
            t.prefetch(pc + 4U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((t.byte(r.address[2]) & 8U) ? 0U : 4U));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x834eU: next = t.branch(pc, 0x8354U, (r.status & 4U) != 0U); break;
        case 0x8354U:
            m.dw(1U, m.add(r.data[1], r.data[5], 16U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x8356U: next = t.branch(pc, 0x835aU, (r.status & 4U) != 0U); break;
        case 0x8358U: {
            const auto child = t.bsr(pc, 113U, 0x8368U, pc + 2U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x835aU: next = t.dbf(pc, 0x8288U, 7U); break;
        case 0x835eU: {
            const auto child = t.jsr_pc_relative(pc, 114U, 0x8382U, pc + 4U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x8362U: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
