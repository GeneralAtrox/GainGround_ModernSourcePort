#pragma once
#include "gain_ground/contract_types.h"

namespace gain_ground {
// Explicitly timed instruction slices only. Untimed fixture hosts retain the
// existing architectural operations; no host-checkpoint-to-cycle conversion.
class M68000Timing {
public:
    explicit M68000Timing(ExecutionHost &host) noexcept
        : host_(host), deadline_(host.execution_time_ns()) { host_.begin_timed_execution(); }
    ~M68000Timing() { host_.end_timed_execution(); }
    M68000Timing(const M68000Timing &) = delete;
    M68000Timing &operator=(const M68000Timing &) = delete;
    void clocks(std::uint32_t count) {
        deadline_ += std::uint64_t(count) * 100U; // System 24: 10 MHz.
        host_.wait_until_time(deadline_);
    }
private:
    ExecutionHost &host_;
    std::uint64_t deadline_;
};
} // namespace gain_ground
