#include "gain_ground/contract_types.h"
#include "cpu_a_fdc_read_span_detail.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
#include "cpu_a_fdc_read_span_memory_access_helpers.inc" // gground-source-split
}
#include "cpu_a_fdc_read_span_operation.inc" // gground-source-split
#include "cpu_a_fdc_read_span_continuation.inc" // gground-source-split

} // namespace gain_ground::translated
