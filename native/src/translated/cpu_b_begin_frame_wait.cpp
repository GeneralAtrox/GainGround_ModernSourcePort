// Implemented but unverified. CLR retains its discarded read before the write.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_begin_frame_wait(FunctionContext &c) noexcept {
    auto &r = c.registers;
    constexpr auto pc = 0x85acU;
    if (!c.host || c.cpu != 1U || c.state != 0x72U || r.program_counter != pc)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    t.prefetch(pc + 4U); (void)t.byte(0x502U);
    m.logic(0U, 8U); t.prefetch(pc + 6U); t.byte(0x502U, 0U);
    if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
    r.program_counter = 0x85b0U; t.stop();
    if (const auto event = m.interrupt(c, pc, r.program_counter)) return *event;
    return c.host->call_function(118U, 1U, 0x72U, 0U, pc, 0x85b0U, c);
}
} // namespace gain_ground::translated
