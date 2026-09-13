// Implemented but unverified. Source-backed IRQ4 timing; no new validation.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_sample_input_word_irq_buffer(FunctionContext &context) noexcept;
namespace {
FunctionResult complete_input_sample(FunctionContext &c, FunctionResult result) {
    // The live dispatcher already follows this boundary through RTS. A
    // retained fixture may still leave that final instruction body pending.
    if (result.control == 3U && result.exit_program_counter == 0x81c8U &&
        c.registers.program_counter == 0x81c8U)
        return cpu_b_sample_input_word_irq_buffer(c);
    return result;
}
}
FunctionResult cpu_b_irq4_state72_io_tick(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x8078U: {
            // MOVE.B $040f.w,$800007.l: abwl1, source, malw1,
            // destination, malw3, mmrw3. Preserve the byte lane and flags.
            t.prefetch(pc + 4U);
            const auto output = t.byte(0x40fU);
            m.logic(output, 8U);
            t.prefetch(pc + 6U);
            t.byte(0x800007U, output);
            t.prefetch(pc + 8U); t.prefetch(pc + 10U);
            next = 0x8080U; break;
        }
        case 0x8080U: case 0x8084U: case 0x8090U: case 0x8094U: case 0x8098U: {
            const auto id = pc == 0x8080U ? 107U : pc == 0x8084U ? 106U :
                pc == 0x8090U ? 110U : pc == 0x8094U ? 109U : 111U;
            const auto target = pc == 0x8080U ? 0x81b6U : pc == 0x8084U ? 0x814aU :
                pc == 0x8090U ? 0x824cU : pc == 0x8094U ? 0x81eaU : 0x8264U;
            const auto child = t.jsr_pc_relative(pc, id, target, pc + 4U,
                pc == 0x8080U ? complete_input_sample : nullptr);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x8088U: {
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            const auto flags = t.byte(0x820U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((flags & 0x80U) ? 0U : 4U));
            t.prefetch(pc + 8U); next = 0x808eU; break;
        }
        case 0x808eU: next = t.branch(pc, 0x809cU, (r.status & 4U) == 0U); break;
        case 0x809cU: t.restore(pc, true); next = 0x80a0U; break;
        case 0x80a0U: return t.rte();
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
}
