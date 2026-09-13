// Implemented but unverified. Original frame dispatch and instruction phases.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_runtime_dispatch_loop(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    enum class CallAddress { pc_relative, absolute_long, indirect };
    auto call = [&](std::uint32_t pc, std::uint32_t target, std::uint32_t next,
                    CallAddress mode, std::uint32_t id) -> FunctionResult {
        // JSR dPC: 18 clocks; absolute long: 20; (An): 16.
        // Preserve each actual child's return PC; task code owns stack effects.
        if (mode == CallAddress::pc_relative) t.clocks(2U);
        else if (mode == CallAddress::absolute_long) t.prefetch(pc + 4U);
        t.prefetch(target);
        r.address[7] -= 4U;
        t.word(r.address[7], static_cast<std::uint16_t>(next >> 16U));
        t.word(r.address[7] + 2U, static_cast<std::uint16_t>(next));
        t.prefetch(target + 2U);
        r.program_counter = target; t.stop();
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        if (const auto event = m.interrupt(c, pc, target)) return *event;
        if (mode == CallAddress::indirect)
            return m.dispatch(c, pc, target, 2U, 0x72U);
        return c.host->call_function(id, 1U, 0x72U, 2U, pc, target, c);
    };
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        bool returned_from_child = false;
        switch (pc) {
        case 0x8572U:
            r.address[7] = 0x7ffeU; t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            next = pc + 4U; break;
        case 0x8576U: case 0x857aU: case 0x8592U: case 0x859eU: {
            const auto target = pc == 0x8576U ? 0x85b0U : pc == 0x857aU ? 0x85beU :
                pc == 0x8592U ? r.address[0] : 0x16d16U;
            const auto return_pc = pc == 0x8576U ? 0x857aU : pc == 0x857aU ? 0x8580U :
                pc == 0x8592U ? 0x8594U : 0x85a4U;
            const auto mode = pc == 0x8576U ? CallAddress::pc_relative :
                pc == 0x8592U ? CallAddress::indirect : CallAddress::absolute_long;
            const auto id = pc == 0x8576U ? 118U : pc == 0x857aU ? 119U : 307U;
            const auto result = call(pc, target, return_pc, mode, id);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            if (result.exit_program_counter != r.program_counter || c.state != 0x72U)
                return {TranslationStatus::contract_violation, result.control, r.program_counter};
            next = r.program_counter;
            returned_from_child = true; break;
        }
        case 0x8580U:
            r.address[5] = 0x1400U; t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            next = pc + 4U; break;
        case 0x8584U: {
            // MOVE.W absolute word -> absolute word: source read precedes
            // the destination prefetch; NZVC is visible before the write.
            t.prefetch(pc + 4U); const auto value = t.word(0x824U);
            m.logic(value, 16U); t.prefetch(pc + 6U);
            t.word(0x822U, value); t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x858aU: {
            const auto value = t.byte(r.address[5]); m.logic(value, 8U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x858cU:
            next = t.branch(pc, 0x8594U, (r.status & 8U) == 0U);
            if (next == 0x8594U) transfer_kind = 1U;
            break;
        case 0x858eU: {
            t.prefetch(pc + 4U); const auto value = t.lng(r.address[5] + 2U);
            r.address[0] = value; t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x8594U:
            r.address[5] += 0x80U; t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            next = pc + 4U; break;
        case 0x8598U: {
            t.prefetch(pc + 4U); const auto old = t.word(0x822U);
            const auto value = m.sub(old, 1U, 16U);
            t.prefetch(pc + 6U); t.word(0x822U, static_cast<std::uint16_t>(value));
            next = pc + 4U; break;
        }
        case 0x859cU:
            next = t.branch(pc, 0x858aU, (r.status & 4U) == 0U);
            if (next == 0x858aU) transfer_kind = 1U;
            break;
        case 0x85a4U:
            next = t.branch(pc, 0x8572U, true); transfer_kind = 1U; break;
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        // JSR already sampled its boundary before invoking the actual child.
        // The child's RTS owns its own boundary; do not resample the old JSR.
        if (!returned_from_child)
            if (const auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x8572U: case 0x8576U: case 0x857aU: case 0x8580U:
        case 0x8584U: case 0x858aU: case 0x858cU: case 0x858eU:
        case 0x8592U: case 0x8594U: case 0x8598U: case 0x859cU:
        case 0x859eU: case 0x85a4U:
            t.begin(next); break;
        default:
            // Preserve the actual transfer and explicit missing mapping.
            return m.dispatch(c, pc, next, transfer_kind, 0x72U);
        }
    }
}
} // namespace gain_ground::translated
