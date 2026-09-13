// Implemented but unverified. Original sound commands and state-72 TRAP #5 return.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
namespace {
constexpr std::array<std::uint32_t, 32> command_targets{
    0x17546U, 0x1761aU, 0x1754aU, 0x17620U, 0x1763aU, 0x1754aU, 0x175baU, 0x175c0U,
    0x17738U, 0x17756U, 0x17768U, 0x17772U, 0x1777aU, 0x177a4U, 0x177acU, 0x1754aU,
    0x1754aU, 0x177d4U, 0x1754aU, 0x1754cU, 0x1779cU, 0x175e4U, 0x17602U, 0x17556U,
    0x175d4U, 0x175c6U, 0x175dcU, 0x177baU, 0x1754aU, 0x1754aU, 0x1754aU, 0x1754aU};
}
FunctionResult proven_static_cpu_b_72_000174ba(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        if (pc >= 0x174c6U && pc <= 0x17542U && (pc - 0x174c6U) % 4U == 0U) {
            // Execute the selected original BRA.W, separately from the JMP.
            next = t.branch_word(pc, command_targets[(pc - 0x174c6U) / 4U], true);
        } else switch (pc) {
        case 0x174baU: case 0x1763cU: case 0x1774aU: case 0x1775aU: {
            const auto reg = pc == 0x1774aU ? 1U : 0U;
            const auto mask = pc == 0x174baU ? 0x1fU : pc == 0x1763cU ? 0xfU : 0xffU;
            t.prefetch(pc + 4U);
            if (pc == 0x1775aU) { m.db(reg, r.data[reg] & mask); m.logic(r.data[reg], 8U); }
            else { m.dw(reg, r.data[reg] & mask); m.logic(r.data[reg], 16U); }
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x174beU: case 0x174c0U: case 0x17640U: case 0x17642U: case 0x177c8U:
            m.dw(0U, m.add(r.data[0], r.data[pc == 0x177c8U ? 1U : 0U], 16U));
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x174c2U: case 0x17644U: {
            t.clocks(2U); t.clocks(2U);
            next = (pc + 4U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            t.clocks(2U); t.prefetch(next); t.prefetch(next + 2U);
            const bool known = (next >= 0x174c6U && next <= 0x17542U && (next - 0x174c6U) % 4U == 0U)
                || next == 0x17648U || next == 0x1764cU || next == 0x17650U
                || next == 0x17654U || next == 0x17658U || next == 0x17664U || next == 0x17668U
                || next == 0x17678U; // Selector 12 reaches the listed MOVE.W directly.
            if (!known) {
                r.program_counter = next; t.stop();
                if (const auto event = m.interrupt(c, pc, next)) return *event;
                // Missing table entries remain unresolved through the original catalog lookup.
                return m.dispatch(c, pc, next, 1U, m.state);
            }
            break;
        }
        case 0x17546U: case 0x1759cU: {
            const auto source = r.address[4]++;
            m.logic(t.byte(source), 8U); t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17548U: case 0x1754aU: case 0x175b8U: case 0x175beU: case 0x175c4U:
        case 0x175d2U: case 0x175daU: case 0x175e2U: case 0x17618U: case 0x1761eU:
        case 0x17638U: case 0x17728U: case 0x17736U: case 0x17754U: case 0x17766U:
        case 0x17770U: case 0x17778U: case 0x1779aU: case 0x177a2U: case 0x177aaU:
        case 0x177b8U: case 0x177d2U: case 0x1781aU: return t.rts(pc);
        case 0x1754cU: case 0x17550U: case 0x17556U: case 0x17562U: case 0x1756eU:
        case 0x17578U: case 0x17584U: case 0x1763aU: case 0x1773cU: case 0x17740U:
        case 0x17768U: case 0x1776cU: case 0x17772U: case 0x1777cU: case 0x17782U:
        case 0x177c2U: case 0x177c4U: case 0x177caU: case 0x177d6U: {
            const auto reg = pc == 0x177caU ? 6U : pc == 0x17782U ? 2U :
                pc == 0x17550U || pc == 0x17556U || pc == 0x17562U || pc == 0x1756eU ||
                pc == 0x17578U || pc == 0x17584U || pc == 0x177c4U ? 1U : 0U;
            const auto source = r.address[4]++;
            const auto value = t.byte(source); m.db(reg, value); m.logic(value, 8U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x1754eU: case 0x1758cU: case 0x1759aU:
            m.db(0U, m.add(r.data[0], r.data[7], 8U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17552U: case 0x175feU:
            next = t.branch_word(pc, 0x18010U, true); return t.transfer(pc, 320U, next);
        case 0x17558U: case 0x17564U: case 0x17570U: case 0x1757aU: case 0x17586U: {
            const auto offset = pc == 0x17558U ? 0xf2U : pc == 0x17564U ? 0xf4U :
                pc == 0x17570U ? 0xf5U : pc == 0x1757aU ? 0xf6U : 0xf8U;
            t.prefetch(pc + 4U); m.logic(r.data[1], 8U);
            t.byte(r.address[6] + offset, static_cast<std::uint8_t>(r.data[1]));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x1755cU: case 0x17568U: case 0x1757eU: case 0x1758aU: case 0x17592U:
        case 0x17594U: case 0x175fcU: case 0x17738U: case 0x1777aU: case 0x177beU:
        case 0x177c0U: case 0x177d4U: case 0x17800U: case 0x17802U: {
            const auto reg = pc == 0x17592U ? 2U : pc == 0x17594U ? 3U :
                pc == 0x177c0U || pc == 0x17802U ? 1U : 0U;
            const auto value = pc == 0x1755cU ? 0x18U : pc == 0x17568U ? 0x19U :
                pc == 0x1757eU ? 0x1bU : pc == 0x1758aU ? 0x38U : pc == 0x17594U ? 3U :
                pc == 0x175fcU ? 8U : pc == 0x17800U ? 0x10U : 0U;
            r.data[reg] = value; m.logic(value, 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x1755eU: case 0x1756aU: case 0x17574U: case 0x17580U: case 0x1758eU: case 0x175acU:
        case 0x176eeU: case 0x176f2U: case 0x17700U: case 0x1772eU: case 0x177eaU: case 0x17816U: {
            const auto id = pc == 0x176eeU ? 492U : pc == 0x176f2U ? 486U : pc == 0x17700U ? 321U :
                pc == 0x1772eU || pc == 0x177eaU ? 488U : pc == 0x17816U ? 490U : 320U;
            const auto target = id == 492U ? 0x1814eU : id == 486U ? 0x18040U : id == 321U ? 0x18172U :
                id == 488U ? 0x18050U : id == 490U ? 0x180a8U : 0x18010U;
            const auto child = t.bsr(pc, id, target, pc + 4U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x17596U: case 0x17704U:
            t.prefetch(pc + 4U); m.dw(0U, pc == 0x17596U ? 0xa0U : 0xfce0U); m.logic(r.data[0], 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x1759eU: next = t.branch(pc, 0x175b0U, !(r.status & 8U)); break;
        case 0x175a0U: case 0x1777eU: {
            const auto source = r.address[3] + static_cast<std::int16_t>(r.data[pc == 0x175a0U ? 2U : 0U])
                + (pc == 0x175a0U ? 0x24U : 0x40U);
            t.clocks(2U); t.prefetch(pc + 4U); const auto value = t.byte(source);
            m.db(1U, value); m.logic(value, 8U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x175a4U: {
            t.prefetch(pc + 4U); const auto old = r.data[1]; t.prefetch(pc + 6U);
            r.data[1] = old | 0x80U;
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 0x80U) ? 0U : 4U));
            t.clocks(2U); next = pc + 4U; break;
        }
        case 0x175a8U: case 0x1778cU: {
            const auto destination = r.address[3] + static_cast<std::int16_t>(r.data[pc == 0x175a8U ? 2U : 0U])
                + (pc == 0x175a8U ? 0x24U : 0x40U);
            t.clocks(2U); t.prefetch(pc + 4U); m.logic(r.data[1], 8U);
            t.byte(destination, static_cast<std::uint8_t>(r.data[1])); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x175b0U: case 0x175b2U: case 0x17784U: {
            const auto reg = pc == 0x175b0U ? 2U : pc == 0x175b2U ? 0U : 1U;
            m.db(reg, m.add(r.data[reg], pc == 0x175b2U ? 8U : 1U, 8U));
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x175b4U: next = t.dbf(pc, 0x1759cU, 3U); break;
        case 0x175baU: case 0x175c0U: case 0x175c6U: case 0x175caU: case 0x175ceU:
        case 0x1761aU: case 0x17620U: {
            const auto offset = pc == 0x175baU ? 0xaU : pc == 0x175c0U ? 9U : pc == 0x175c6U ? 0x33U :
                pc == 0x175caU ? 0x30U : pc == 0x175ceU ? 0x32U : pc == 0x1761aU ? 3U : 0xdU;
            const auto source = r.address[4]++;
            const auto value = t.byte(source); m.logic(value, 8U); t.prefetch(pc + 4U);
            t.byte(r.address[3] + offset, value); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x175d4U: case 0x175dcU: case 0x175e4U: case 0x175f4U: case 0x17602U:
        case 0x17612U: case 0x176e2U: case 0x17712U: case 0x17722U: case 0x1779cU:
        case 0x177a4U: case 0x177acU: case 0x177b2U: {
            const bool indexed = pc == 0x17712U || pc == 0x17722U;
            const bool set = pc == 0x175d4U || pc == 0x175e4U || pc == 0x175f4U || pc == 0x1779cU || pc == 0x177a4U;
            const auto mask = pc == 0x175d4U || pc == 0x175dcU ? 0x20U :
                pc == 0x175e4U || pc == 0x17602U ? 8U : pc == 0x176e2U ? 0x80U :
                pc == 0x1779cU || pc == 0x177a4U || pc == 0x177acU || pc == 0x177b2U ? 0x10U : 4U;
            const auto offset = pc == 0x175d4U || pc == 0x175dcU || pc == 0x175e4U ||
                pc == 0x17602U || pc == 0x177a4U || pc == 0x177b2U ? 1U : 0U;
            const auto address = r.address[3] + (indexed ? static_cast<std::int16_t>(r.data[0]) : offset);
            t.prefetch(pc + 4U); if (indexed) t.clocks(2U); t.prefetch(pc + 6U);
            const auto old = t.byte(address); t.prefetch(pc + 8U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & mask) ? 0U : 4U));
            t.byte(address, static_cast<std::uint8_t>(set ? old | mask : old & ~mask));
            next = pc + 6U; break;
        }
        case 0x175eaU: case 0x17608U: case 0x176f6U: case 0x17708U: case 0x177e0U: case 0x177eeU: {
            const bool indexed = pc == 0x17708U;
            const auto mask = pc == 0x17608U || indexed ? 0x80U : pc == 0x176f6U || pc == 0x177eeU ? 8U : 4U;
            const auto offset = pc == 0x17608U ? 0x320U : pc == 0x176f6U || pc == 0x177eeU ? 2U : 0U;
            const auto address = r.address[3] + (indexed ? static_cast<std::int16_t>(r.data[0]) : offset);
            t.prefetch(pc + 4U); if (indexed) t.clocks(2U); t.prefetch(pc + 6U);
            const auto value = t.byte(address);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((value & mask) ? 0U : 4U));
            t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x175f0U: case 0x1760eU: case 0x176fcU: case 0x1770eU:
        case 0x177d8U: case 0x177e6U: case 0x177f4U: case 0x177fcU: {
            const bool taken = pc == 0x177fcU ? !(r.status & 1U) :
                pc == 0x176fcU || pc == 0x1770eU || pc == 0x177d8U ? (r.status & 4U) != 0U : !(r.status & 4U);
            next = t.branch_word(pc, 0x17324U, taken);
            if (taken) return t.transfer(pc, 544U, next);
            break;
        }
        case 0x175faU: case 0x17804U:
            m.db(1U, r.data[7]); m.logic(r.data[1], 8U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17624U: case 0x1762aU: case 0x17752U: case 0x1776eU: case 0x17798U: {
            const auto value = pc == 0x17624U ? r.address[4] - r.address[5] :
                pc == 0x1762aU ? r.address[4] + r.address[5] : pc == 0x17798U ? r.address[4] + 2U :
                r.address[4] + static_cast<std::int16_t>(r.data[0]);
            r.address[4] = (r.address[4] & 0xffff0000U) | (value & 0xffffU);
            t.prefetch(pc + 4U); t.clocks(2U); r.address[4] = value; t.clocks(2U);
            next = pc + 2U; break;
        }
        case 0x17626U: case 0x1774eU: {
            const auto destination = r.address[3] + (pc == 0x17626U ? 4U : static_cast<std::int16_t>(r.data[1]));
            const auto value = r.address[4]; const auto high = static_cast<std::uint16_t>(value >> 16U);
            if (pc == 0x1774eU) t.clocks(2U);
            t.prefetch(pc + 4U);
            r.status = static_cast<std::uint16_t>((r.status & ~8U & (high == 0U ? 0xffffU : ~4U))
                | ((high & 0x8000U) ? 8U : 0U));
            t.word(destination, high); m.logic(value, 16U);
            t.word(destination + 2U, static_cast<std::uint16_t>(value)); m.logic(value, 32U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x1762cU: case 0x17632U: case 0x176e8U: case 0x17792U: {
            const bool indexed = pc == 0x17792U;
            const auto destination = r.address[3] + (indexed ? static_cast<std::int16_t>(r.data[0]) + 0x40U :
                pc == 0x17632U ? 0xeU : 0xfU);
            t.prefetch(pc + 4U); if (indexed) t.clocks(2U); t.prefetch(pc + 6U);
            m.logic(0U, 8U); t.byte(destination, 0U); t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x17648U: next = t.branch_word(pc, 0x17678U, true); break;
        case 0x1764cU: next = t.branch_word(pc, 0x17682U, true); break;
        case 0x17650U: next = t.branch_word(pc, 0x1768cU, true); break;
        case 0x17654U: next = t.branch_word(pc, 0x17696U, true); break;
        case 0x17658U: next = t.branch_word(pc, 0x176a0U, true); break;
        case 0x17664U: next = t.branch_word(pc, 0x176beU, true); break;
        case 0x17668U: next = t.branch_word(pc, 0x176c8U, true); break;
        case 0x17678U:
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); m.logic(0U, 16U);
            t.word(r.address[6] + 0x32U, 0U); t.prefetch(pc + 8U); next = pc + 6U; break;
        case 0x1767eU: case 0x17688U: case 0x17692U: case 0x1769cU: case 0x176a6U: case 0x176c4U: case 0x176ceU:
            next = t.branch_word(pc, 0x176e2U, true); break;
        case 0x17682U: case 0x1768cU: case 0x17696U: case 0x176a0U: case 0x176beU: case 0x176c8U: {
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); const auto address = r.address[6] + 0x32U;
            const auto value = static_cast<std::uint16_t>(t.word(address) & (pc >= 0x176beU ? 0xf800U : 0x7ffU));
            m.logic(value, 16U); t.prefetch(pc + 8U); t.word(address, value); next = pc + 6U; break;
        }
        case 0x17718U: case 0x177f8U:
            t.prefetch(pc + 4U); (void)m.sub(r.data[7], pc == 0x17718U ? 6U : 4U, 8U, true);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x1771cU: next = t.branch(pc, 0x1772aU, (r.status & 1U) != 0U); break;
        case 0x1771eU:
            t.prefetch(pc + 4U); m.dw(0U, m.add(r.data[0], 0xa0U, 16U));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x1772aU: case 0x17732U:
            r.address[3] = pc == 0x1772aU ? r.address[3] - 0x320U : r.address[3] + 0x320U;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x1773aU:
            r.data[1] = r.data[0]; m.logic(r.data[1], 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x1773eU: case 0x1776aU: case 0x177c6U: {
            auto value = r.data[0] & 0xffffU; m.logic(value, 16U); t.prefetch(pc + 4U);
            for (unsigned bit = 0U; bit < 8U; ++bit) {
                const bool carry = (value & 0x8000U) != 0U; value = (value << 1U) & 0xffffU;
                r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | (carry ? 0x11U : 0U)
                    | (value == 0U ? 4U : 0U) | ((value & 0x8000U) ? 8U : 0U));
                t.clocks(2U);
            }
            m.dw(0U, value); t.clocks(2U); next = pc + 2U; break;
        }
        case 0x17742U: case 0x17762U: case 0x17774U: {
            const auto address = r.address[3] + (pc == 0x17774U ? 8U : 0xcU);
            t.prefetch(pc + 4U); const auto old = t.byte(address);
            const auto value = pc == 0x17742U ? m.sub(old, 4U, 8U) : m.add(old, pc == 0x17774U ? r.data[0] : 4U, 8U);
            t.prefetch(pc + 6U); t.byte(address, static_cast<std::uint8_t>(value)); next = pc + 4U; break;
        }
        case 0x17746U: case 0x17756U: case 0x17812U: {
            const auto reg = pc == 0x17746U ? 1U : pc == 0x17756U ? 0U : 2U;
            const auto source = pc == 0x17812U ? r.address[1] + 0x13U : r.address[3] + 0xcU;
            t.prefetch(pc + 4U); const auto value = t.byte(source); m.db(reg, value); m.logic(value, 8U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x1775eU: {
            const auto source = r.address[3] + static_cast<std::int16_t>(r.data[0]);
            t.clocks(2U); t.prefetch(pc + 4U); r.address[4] = t.lng(source);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17786U:
            (void)m.sub(r.data[2], r.data[1], 8U, true); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17788U: next = t.branch_word(pc, 0x17792U, (r.status & 4U) != 0U); break;
        case 0x17790U: next = t.branch(pc, 0x17768U, true); break;
        case 0x177baU: {
            // 48e7 ff00: D7..D0, low word then high word at descending addresses.
            const auto values = r.data; auto sp = r.address[7]; t.prefetch(pc + 4U);
            for (unsigned reg = 8U; reg != 0U; --reg) {
                sp -= 4U; t.word(sp + 2U, static_cast<std::uint16_t>(values[reg - 1U]));
                t.word(sp, static_cast<std::uint16_t>(values[reg - 1U] >> 16U));
            }
            r.address[7] = sp; t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x177ccU: {
            next = 0x177ceU;
            const auto result = t.supervisor_trap(pc, 37U, next);
            if (result.status != TranslationStatus::complete || result.control != 2U) return result;
            break;
        }
        case 0x177ceU: {
            // 4cdf 00ff: commit each high half before the following low read;
            // popm5 also reads the word beyond the last restored register.
            t.prefetch(pc + 4U); auto sp = r.address[7]; auto high = t.word(sp);
            for (unsigned reg = 0U; reg < 8U; ++reg) {
                r.data[reg] = (r.data[reg] & 0xffffU) | (std::uint32_t(high) << 16U);
                const auto low = t.word(sp + 2U); m.dw(reg, low); sp += 4U;
                high = t.word(sp);
            }
            r.address[7] = sp; t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x177dcU:
            t.prefetch(pc + 4U); m.logic(r.data[0], 8U);
            t.byte(r.address[3] + 0x16U, static_cast<std::uint8_t>(r.data[0]));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17806U:
            t.prefetch(pc + 4U); m.db(0U, m.sub(r.data[0], 4U, 8U));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x1780aU: next = t.dbf(pc, 0x17806U, 1U); break;
        case 0x1780eU: {
            const auto value = r.address[6] + static_cast<std::int16_t>(r.data[0]) + 0x30U;
            t.clocks(2U); t.prefetch(pc + 4U); r.address[1] = value;
            t.clocks(2U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
