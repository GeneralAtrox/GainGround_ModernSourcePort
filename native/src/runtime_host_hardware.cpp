#include "gain_ground/runtime_host.h"
#include <cstdio>
#include <cstdlib>
#include <optional>
#include "gain_ground/direct_asset_loader.h"
#include "gain_ground/system24_devices.h"
#include "gain_ground/native_function_registry.h"
#include "gground_fixture_contract.h"
#include "gground_functions.h"
#include "gground_memory_map.h"
#include "translated/cpu_b_irq_timing.h"
#include "gameplay/legacy_gameplay_bridge.h"
#include "gameplay/legacy_enemy_bridge.h"
#include <algorithm>


namespace gain_ground {
void RuntimeHost::set_irq_line(std::uint8_t cpu, std::uint8_t level, bool asserted) noexcept
{
    if (cpu >= 2U || level == 0U || level > 7U) return;
    const auto bit = static_cast<std::uint8_t>(1U << level);
    if (asserted) irq_lines_[cpu] |= bit;
    else irq_lines_[cpu] &= static_cast<std::uint8_t>(~bit);
}

PendingInterrupt RuntimeHost::consume_pending_interrupt(std::uint8_t cpu, std::uint8_t,
                                                       std::uint32_t completed_pc)
{
    checkpoint();
    if (cpu >= 2U) { fail("Invalid interrupt CPU"); return {}; }
    if (native_loop()) {
        native_services_(native_services_argument_);
        if (faulted()) return {};
        const auto level = devices_ ? devices_->irq_level(cpu) : 0U;
        const auto mask = active_ ? (active_->registers.status >> 8U) & 7U : 7U;
        if (level > mask) {
            devices_->acknowledge_native_event(cpu, level);
            return {true, static_cast<std::uint8_t>(level)};
        }
        return {};
    }
    if (devices_) {
        // Suspend original idle/backedge loops without changing registers.
        // The scheduler resumes here after advancing to a device event, so
        // the pending interrupt is sampled after the suspension returns.
        const auto status = active_ ? active_->registers.status : 0x2700U;
        const auto pending = devices_->irq_level(cpu);
        if (pending <= ((status >> 8U) & 7U) &&
            ((cpu == 0U && completed_pc == 0x80118U) ||
             (cpu == 1U && completed_pc == 0x85b4U && !(status & 8U)))) {
            waiting_ = true;
            if (checkpoint_) checkpoint_(checkpoint_argument_);
            waiting_ = false;
        }
        const auto level = devices_->irq_level(cpu);
        if (level) return {true, level};
    }
    for (std::uint8_t level = 7U; level != 0U; --level)
        if (irq_lines_[cpu] & (1U << level)) return {true, level};
    return {};
}

}
