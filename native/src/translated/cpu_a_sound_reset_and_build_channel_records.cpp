#include "gain_ground/contract_types.h"
#include "gain_ground/sound_caller_timing.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
#include "cpu_a_sound_reset_and_build_channel_records_timing_helpers.inc" // gground-source-split
}
#include "cpu_a_sound_reset_and_build_channel_records_operation.inc" // gground-source-split

} // namespace gain_ground::translated
