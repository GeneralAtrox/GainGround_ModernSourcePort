// Generated sound timing integration; implemented but unverified.
#include "gain_ground/sound_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_a_sound_ym2151_write_wait_ready(FunctionContext &context) noexcept
{
    return sound_timing::writer(context, 87U, 0x8410eU);
}
} // namespace gain_ground::translated
