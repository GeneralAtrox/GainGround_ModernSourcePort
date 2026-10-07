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
RuntimeHost::RuntimeHost()
{
    // Region IDs describe the native bus ABI; only their geometry is reused.
    // Unknown power-on RAM is not silently promoted to authoritative zeroes.
    for (const auto &spec : generated::kFixtureRegions) {
        if (spec.id == 4U) continue; // BIOS mirrors the same physical ROM as 1.
        auto &r = regions_[spec.id];
        r.bytes.resize(spec.bytes);
        r.known_bits.resize(spec.bytes);
        r.read_only = spec.immutable;
        // Pages both CPUs use are ordered in time under quantum slices. A
        // page's first touch by the second CPU would come too late to order,
        // so the areas CPU B is known to use start out shared: the video
        // regions and the communication window at 0x38000 in shared RAM.
        if (spec.id >= 5U) page_users_[spec.id].assign((spec.bytes + 255U) / 256U, 3U);
        if (spec.id == 3U) {
            page_users_[3].assign((spec.bytes + 255U) / 256U, 0U);
            std::fill(page_users_[3].begin() + (0x38000U >> 8U), page_users_[3].end(), std::uint8_t{3});
        }
    }
}

void RuntimeHost::set_checkpoint(Checkpoint callback, void *argument) noexcept
{
    checkpoint_ = callback;
    checkpoint_argument_ = argument;
}

std::uint64_t RuntimeHost::execution_time_ns() const noexcept
{
    if (native_loop()) return devices_ ? devices_->time_ns() : 0U;
    return devices_ ? std::max(devices_->time_ns(), local_ns_[selected_cpu_]) : 0U;
}

void RuntimeHost::sync_devices()
{
    if (native_loop()) return;
    if (devices_ && local_ns_[selected_cpu_] > devices_->time_ns()) devices_->advance(local_ns_[selected_cpu_]);
}

bool RuntimeHost::shared_page(std::uint16_t id, std::uint32_t offset) noexcept
{
    auto &users = page_users_[id];
    if (users.empty()) return false;
    auto &page = users[offset >> 8U];
    page = static_cast<std::uint8_t>(page | (1U << selected_cpu_));
    return page == 3U;
}

// The time up to which a suspended CPU has executed: the deadline it waits
// for, or where it stopped running. A disabled CPU B never accesses anything.
std::uint64_t RuntimeHost::committed_ns(unsigned cpu) const noexcept
{
    if (cpu == 1U && !devices_->cpu_b_enabled()) return UINT64_MAX;
    return std::max(devices_->time_ns(), cpu_deadlines_[cpu] != UINT64_MAX ? cpu_deadlines_[cpu] : local_ns_[cpu]);
}

void RuntimeHost::sync_shared()
{
    if (native_loop()) return;
    if (!slice_end_ || !devices_ || !checkpoint_) return;
    const auto cpu = selected_cpu_;
    const auto other = 1U - cpu;
    const auto now = execution_time_ns();
    if (committed_ns(other) >= now) return;
    // Wait, as for a deadline at this time, until the other CPU gets here.
    const auto previous_deadline = cpu_deadlines_[cpu];
    const auto previous_waiting = waiting_;
    cpu_deadlines_[cpu] = now;
    waiting_ = true;
    while (!faulted() && committed_ns(other) < now) checkpoint_(checkpoint_argument_);
    waiting_ = previous_waiting;
    cpu_deadlines_[cpu] = previous_deadline;
    local_ns_[cpu] = std::max(local_ns_[cpu], now);
}

void RuntimeHost::wait_until_time(std::uint64_t deadline_ns)
{
    if (native_loop() && devices_) {
        // One monotonic timeline. No peer CPU deadline or shared-page rendezvous.
        while (!faulted() && devices_->time_ns() < deadline_ns)
            devices_->advance(std::min(deadline_ns, devices_->next_event_ns()));
        return;
    }
    if (!devices_ || !checkpoint_) return;
    const auto cpu = selected_cpu_;
    if (deadline_ns <= std::max(devices_->time_ns(), local_ns_[cpu])) return;
    if (deadline_ns < slice_end_) { local_ns_[cpu] = deadline_ns; return; }
    const auto previous_deadline = cpu_deadlines_[cpu];
    cpu_deadlines_[cpu] = deadline_ns;
    const auto previous_waiting = waiting_;
    waiting_ = true;
    const auto other = 1U - cpu;
    while (!faulted() && devices_->time_ns() < deadline_ns) {
        // When the other CPU cannot run before this deadline, the scheduler
        // would only step the device clock to the next device event or this
        // deadline and switch straight back. Take exactly that step here and
        // save the fiber round trip; the device sees the same sequence of
        // advances. Otherwise yield so the other CPU runs in its turn.
        // A CPU without a deadline is runnable, not idle: the scheduler would
        // switch to it before stepping time. Only a later deadline, or CPU B
        // still disabled, leaves this CPU alone until its own deadline.
        const auto other_deadline = cpu_deadlines_[other];
        const bool other_idle = (other == 1U && !devices_->cpu_b_enabled()) ||
                                (other_deadline != UINT64_MAX && other_deadline > deadline_ns);
        if (other_idle && direct_wait_) {
            const auto step = std::min(devices_->next_event_ns(), deadline_ns);
            if (step > devices_->time_ns()) { devices_->advance(step); operations_since_switch_ = 0U; continue; }
        }
        checkpoint_(checkpoint_argument_);
    }
    waiting_ = previous_waiting;
    cpu_deadlines_[cpu] = previous_deadline;
}

bool RuntimeHost::cpu_ready(unsigned cpu) const noexcept
{
    return cpu_deadlines_[cpu] == UINT64_MAX || (devices_ && cpu_deadlines_[cpu] <= devices_->time_ns());
}

std::uint64_t RuntimeHost::next_cpu_deadline_ns() const noexcept
{
    auto next = UINT64_MAX;
    for (const auto deadline : cpu_deadlines_)
        if (deadline > (devices_ ? devices_->time_ns() : 0U)) next = std::min(next, deadline);
    return next;
}

void RuntimeHost::checkpoint()
{
    ++operations_;
    ++operations_since_switch_;
    if (checkpoint_) checkpoint_(checkpoint_argument_);
}

void RuntimeHost::fail(std::string_view message, std::uint32_t address,
                        std::uint16_t region, std::uint16_t mask)
{
    if (!faulted()) fault_ = {message, active_ ? active_->registers.program_counter : 0U,
        address, region, mask, active_ ? active_->cpu : std::uint8_t{0}};
    checkpoint();
}


} // namespace gain_ground
