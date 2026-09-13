#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {

FunctionResult cpu_b_record_flag5_dispatch_common(
    FunctionContext &context,
    std::uint32_t scan_function_id,
    std::uint32_t scan_target,
    std::uint32_t first_scan_callsite,
    std::uint32_t first_scan_continuation,
    std::uint32_t first_accumulator_callsite,
    std::uint32_t first_accumulator_continuation,
    std::uint32_t first_decode_callsite,
    std::uint32_t first_decode_continuation,
    std::uint32_t second_scan_callsite,
    std::uint32_t second_scan_continuation,
    std::uint32_t second_accumulator_callsite,
    std::uint32_t second_accumulator_continuation,
    std::uint32_t second_decode_callsite,
    std::uint32_t second_decode_continuation) noexcept;

FunctionResult cpu_b_record_flag5_dispatch_variant(FunctionContext &context) noexcept
{
    return cpu_b_record_flag5_dispatch_common(context,
        342U, 0x0001d7d8U,
        0x0001d40aU, 0x0001d40eU,
        0x0001d424U, 0x0001d428U,
        0x0001d43aU, 0x0001d43eU,
        0x0001d444U, 0x0001d448U,
        0x0001d45cU, 0x0001d460U,
        0x0001d472U, 0x0001d476U);
}

} // namespace gain_ground::translated
