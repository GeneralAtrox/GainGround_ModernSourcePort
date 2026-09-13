// Implemented but unverified. Original indexed operator-volume dispatcher.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
namespace {
FunctionResult complete_operator_write(FunctionContext &c, FunctionResult result) {
    // Retain the old caller's completed-tail convention only at its original
    // BSR continuations. invoke_pushed still checks the exact PC, SP and state.
    if (result.status == TranslationStatus::complete && result.control == 3U) {
        switch (c.registers.program_counter) {
        case 0x180f2U: case 0x18106U: case 0x18110U:
        case 0x18124U: case 0x1812eU: case 0x18138U:
            return FunctionResult::complete(1U, c.registers.program_counter);
        default: break;
        }
    }
    return result;
}
}
FunctionResult proven_static_cpu_b_72_000180a8(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x180a8U:
            t.prefetch(pc + 4U); m.dw(0U, 0x60U); m.logic(0x60U, 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x180acU:
            m.db(0U, m.add(r.data[0], r.data[7], 8U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x180aeU: {
            t.prefetch(pc + 4U); const auto value = t.byte(r.address[3] + 0x17U);
            m.db(1U, value); m.logic(value, 8U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x180b2U:
            t.prefetch(pc + 4U); m.dw(1U, r.data[1] & 7U); m.logic(r.data[1], 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x180b6U: case 0x180b8U:
            m.dw(1U, m.add(r.data[1], r.data[1], 16U));
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x180baU:
            // 4efb 1002: retain the signed brief-extension calculation and
            // execute the selected original BRA, not a collapsed selector.
            t.clocks(2U); t.clocks(2U);
            next = pc + 4U + static_cast<std::int16_t>(r.data[1]);
            t.clocks(2U); t.prefetch(next); t.prefetch(next + 2U); break;
        case 0x180beU: case 0x180c2U: case 0x180c6U: case 0x180caU:
            next = t.branch_word(pc, 0x180deU, true); break;
        case 0x180ceU: next = t.branch_word(pc, 0x180e8U, true); break;
        case 0x180d2U: case 0x180d6U: next = t.branch_word(pc, 0x180fcU, true); break;
        case 0x180daU: next = t.branch_word(pc, 0x1811aU, true); break;
        case 0x180deU: case 0x180e8U: case 0x180f2U: case 0x180fcU:
        case 0x18106U: case 0x18110U: case 0x1811aU: case 0x18124U:
        case 0x1812eU: case 0x18138U: {
            const auto immediate = pc == 0x180deU ? 0x18U : pc == 0x180e8U ? 0x10U :
                pc == 0x1811aU ? 0U : 8U;
            t.prefetch(pc + 4U); m.db(0U, m.add(r.data[0], immediate, 8U));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x180e2U: case 0x180ecU: case 0x180f6U: case 0x18100U:
        case 0x1810aU: case 0x18114U: case 0x1811eU: case 0x18128U:
        case 0x18132U: case 0x1813cU:
            r.data[3] = pc == 0x1811eU ? 0x1cU : pc == 0x18100U || pc == 0x18128U ? 0x1dU :
                pc == 0x180ecU || pc == 0x1810aU || pc == 0x18132U ? 0x1eU : 0x1fU;
            m.logic(r.data[3], 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x180eeU: case 0x18102U: case 0x1810cU:
        case 0x18120U: case 0x1812aU: case 0x18134U: {
            const auto child = t.bsr(pc, 491U, 0x1813eU, pc + 4U, complete_operator_write);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x180e4U: case 0x180f8U: case 0x18116U:
            next = t.branch_word(pc, 0x1813eU, true);
            return t.transfer(pc, 491U, next);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        // Preserve the captured sequential ownership marker at 1813c. It has
        // no branch clocks or return push; RuntimeHost executes the next owner.
        if (next == 0x1813eU)
            return c.host->call_function(491U, 1U, 0x72U, 0U, pc, next, c);
        t.begin(next);
    }
}
} // namespace gain_ground::translated
