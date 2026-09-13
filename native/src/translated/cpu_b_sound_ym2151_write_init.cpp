// Generated sound timing integration; implemented but unverified.
#include "gain_ground/sound_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_sound_ym2151_write_init(FunctionContext &context) noexcept
{
    return sound_timing::writer(context, 319U, 0x17ec2U);
}
} // namespace gain_ground::translated
