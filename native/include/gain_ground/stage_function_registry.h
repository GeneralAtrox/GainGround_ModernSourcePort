#pragma once
#include "gain_ground/contract_types.h"

namespace gain_ground::stage_registry {
// Source-owned stage entries. Registration is not fixture/parity evidence.
const FunctionContract *find(std::uint8_t cpu, std::uint8_t state,
                             std::uint32_t pc) noexcept;
}
