// Implemented but unverified. Original 60000dc8 BRA.W $17e44.
#include "gain_ground/cpu_b_init_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_sound_driver_init_jump(FunctionContext &c) noexcept {
    if (!c.host || c.registers.program_counter != 0x1707aU)
        return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    CpuBInitTiming timing(c);
    return timing.branch(318U, 0x1707aU, 0x17e44U);
}
} // namespace gain_ground::translated
