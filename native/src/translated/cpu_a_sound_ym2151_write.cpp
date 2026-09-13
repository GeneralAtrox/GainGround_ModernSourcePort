// Generated sound timing integration; implemented but unverified.
#include "gain_ground/sound_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_a_sound_ym2151_write(FunctionContext &context) noexcept
{
    return sound_timing::writer(context, 88U, 0x84126U);
}
} // namespace gain_ground::translated
