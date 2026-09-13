// Implemented but unverified. Original state-04 IRQ prologue.
#include "gain_ground/cpu_b_irq_prologue.h"

namespace gain_ground::translated {
FunctionResult cpu_b_irq3_state04_entry(FunctionContext &context) noexcept {
    return cpu_b_irq_prologue(context, 0x8034U, 0x803cU,
                              103U, 0x8042U, false);
}
}
