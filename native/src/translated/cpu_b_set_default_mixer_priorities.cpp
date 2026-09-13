// Implemented but unverified. Original default mixer table selection.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_set_default_mixer_priorities(FunctionContext &c) noexcept {
    auto &r = c.registers;
    constexpr auto pc = 0x891aU;
    if (!c.host || c.cpu != 1U || c.state != 0x72U || r.program_counter != pc)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    r.address[1] = 0x8954U; t.prefetch(pc + 4U); t.prefetch(pc + 6U);
    r.program_counter = 0x891eU; t.stop();
    if (const auto event = m.interrupt(c, pc, r.program_counter)) return *event;
    return c.host->call_function(129U, 1U, 0x72U, 0U, pc, 0x891eU, c);
}
} // namespace gain_ground::translated
