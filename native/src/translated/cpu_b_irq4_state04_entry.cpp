// Implemented but unverified. Original state-04 IRQ prologue.
#include "gain_ground/cpu_b_irq_prologue.h"

namespace gain_ground::translated {
FunctionResult cpu_b_irq4_state04_entry(FunctionContext &context) noexcept {
    return cpu_b_irq_prologue(context, 0x806eU, 0x8072U,
                              104U, 0x8078U, true);
}
}
