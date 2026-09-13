#pragma once
#include "gain_ground/contract_types.h"

namespace gain_ground::translated {
FunctionResult cpu_b_initials_entry(FunctionContext &) noexcept;
inline bool is_initials_entry_pc(std::uint32_t pc) noexcept {
    return (pc >= 0xf094U && pc <= 0xf23cU) ||
           (pc >= 0xf2e6U && pc <= 0xf374U) ||
           (pc >= 0xf38eU && pc <= 0xf3aaU);
}
} // namespace gain_ground::translated
