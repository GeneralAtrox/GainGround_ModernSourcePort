// Implemented but unverified. capture/cpu_b_state72_opcodes.bin[0x400:0x402] = 4e73.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_state72_default_trap_return(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U || r.program_counter != 0x400U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    return t.rte();
}
} // namespace gain_ground::translated
