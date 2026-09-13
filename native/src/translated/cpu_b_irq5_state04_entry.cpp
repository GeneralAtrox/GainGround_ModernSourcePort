// Implemented but unverified. Original state-04 IRQ prologue.
#include "gain_ground/cpu_b_irq_prologue.h"

namespace gain_ground::translated {
FunctionResult cpu_b_irq5_state04_entry(FunctionContext &context) noexcept {
    return cpu_b_irq_prologue(context, 0x80a2U, 0x80a6U,
                              105U, 0x80acU, true);
}
}
