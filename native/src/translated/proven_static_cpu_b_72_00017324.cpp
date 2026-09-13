// Implemented but unverified. Original single-instruction return owner.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00017324(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U || r.program_counter != 0x17324U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    return t.rts(0x17324U);
}
} // namespace gain_ground::translated
