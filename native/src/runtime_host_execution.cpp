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
const FunctionContract *RuntimeHost::entry_at(std::uint8_t cpu, std::uint8_t state,
                                              std::uint32_t pc) const noexcept
{
    return native_registry::find(cpu, state, pc);
}

FunctionContext RuntimeHost::cpu_a_reset_context()
{
    FunctionContext c{};
    c.host = this;
    c.cpu = 0U;
    c.state = 0xffU;
    c.registers.status = 0x2700U;
    // Original big-endian initial SSP and PC; no hard-coded boot entry override.
    c.registers.address[7] = (static_cast<std::uint32_t>(read_memory_word(1U, 0U, 0xffffU)) << 16U)
        | read_memory_word(1U, 2U, 0xffffU);
    c.registers.program_counter = (static_cast<std::uint32_t>(read_memory_word(1U, 4U, 0xffffU)) << 16U)
        | read_memory_word(1U, 6U, 0xffffU);
    return c;
}

FunctionResult RuntimeHost::execute(const FunctionContract &first, FunctionContext &c, std::uint32_t callsite)
{
    if (depth_ >= 512U) {
        // Opt-in diagnostics: the innermost frames of the runaway chain.
        static std::FILE *log = [] { const char *p = std::getenv("GAIN_GROUND_NAV_LOG"); return p && *p ? std::fopen(p, "a") : nullptr; }();
        if (log) {
            std::fprintf(log, "call depth exceeded entering %06x (function %u); innermost frames:", unsigned(first.address), unsigned(first.id));
            unsigned shown = 0U;
            for (const auto *frame = invocation_; frame && shown < 24U; frame = frame->parent, ++shown)
                std::fprintf(log, " %u@%06x", unsigned(frame->function_id), unsigned(frame->callsite));
            std::fputc('\n', log);
            std::fflush(log);
        }
        fail("Native call depth exceeds runtime capacity");
        return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    }
    auto *previous_context = active_;
    Invocation frame{invocation_, {}, 0U, false};
    frame.callsite = callsite;
    invocation_ = &frame;
    active_ = &c;
    ++depth_;
    max_depth_[selected_cpu_] = std::max(max_depth_[selected_cpu_], depth_);
    const FunctionContract *f = &first;
    FunctionResult result{};
    // Class selection is host bookkeeping and must not add guest bus reads.
    // Retain the decision across IRQ resumption even if a callback changes the
    // actor's next-frame callback while completing this invocation.
    const auto actor = c.registers.address[5];
    const auto actor_bytes = region_bytes(2U);
    bool enemy_invocation = false;
    if (c.cpu == 1U && c.state == 0x72U && actor >= 0x3400U && actor < 0x7400U &&
        (actor & 0x7fU) == 0U && actor_bytes.size() > actor + 5U) {
        const auto callback = (std::uint32_t(actor_bytes[actor+2U]) << 24U) |
            (std::uint32_t(actor_bytes[actor+3U]) << 16U) |
            (std::uint32_t(actor_bytes[actor+4U]) << 8U) | actor_bytes[actor+5U];
        enemy_invocation = callback == first.address;
    }
    for (;;) {
        if (native_loop() && (f->id == 55U || f->id == 116U)) {
            // The native scheduler owns these two endless outer loops.
            // Unwind startup/reset callers back to it at the explicit boundary.
            result = FunctionResult::complete(10U, f->address);
            break;
        }
        checkpoint();
        if (faulted()) { result = {TranslationStatus::contract_violation, 0U, c.registers.program_counter}; break; }
        if (!f->implemented || !f->entry) {
            fail("Native function has no body");
            result = {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
            break;
        }
        frame.executed_child = false;
        frame.executing_state = f->state;
        frame.cpu = c.cpu;
        if (!reset_anchor_[c.cpu] && stack_reset_entry(c.cpu, c.registers.program_counter))
            reset_anchor_[c.cpu] = &frame;
        frame.function_id = f->id;
        frame.actor = c.registers.address[5];
        if (start_stage_force_ && f->id == 153U && c.cpu == 1U && c.state == 0x72U &&
            c.registers.program_counter == 0xd734U)
            (void)stage_select_frame(c.registers.address[5]);
        if (f->id == 140U && c.cpu == 1U && c.state == 0x72U && c.registers.program_counter == 0xa618U)
            enemy_navigation_.reset();
        if (f->id == 140U && c.cpu == 1U && c.state == 0x72U &&
            c.registers.program_counter == 0xa618U && !apply_level_definition()) {
            result = {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
            break;
        }
        if (devices_ && f->id == 353U && c.cpu == 1U && c.state == 0x72U && c.registers.program_counter == 0x1dbf0U)
            enemy_navigation_.prepare(regions_[2].bytes,regions_[3].bytes,c.registers.address[5],devices_->frame());
        const auto moving_actor=c.registers.address[5];
        const auto contacted_actor=c.registers.address[6];
        const bool new_contact_test=devices_ && f->id==354U && c.cpu==1U && c.state==0x72U &&
            moving_actor<regions_[2].bytes.size()-0x40U && (regions_[2].bytes[moving_actor+0x40]&2U)==0;
        const bool separating_contact=devices_ && f->id==354U && c.cpu==1U && c.state==0x72U &&
            enemy_navigation_.separating(regions_[2].bytes,moving_actor,c.registers.address[6],devices_->frame()) &&
            (regions_[2].bytes[moving_actor+0x40]&2U)==0;
        if (!enemy_invocation || !gameplay::run_enemy(*f, c, result)) result = f->entry(c);
        // Only waive the NPC/NPC contact just tested, when our validated step
        // reduces an existing overlap. Terrain and other contacts still run.
        if(separating_contact && result.status==TranslationStatus::complete && result.control==1U)
            regions_[2].bytes[moving_actor+0x40]&=static_cast<std::uint8_t>(~2U);
        if(new_contact_test && result.status==TranslationStatus::complete && result.control==1U)
            enemy_navigation_.record_contact_block(regions_[2].bytes,moving_actor,contacted_actor,devices_->frame());
        if(devices_ && f->id==357U && c.cpu==1U && c.state==0x72U &&
           result.status==TranslationStatus::complete && result.control==1U)
            enemy_navigation_.finish_move(regions_[2].bytes,moving_actor,devices_->frame());
        if (result.status != TranslationStatus::complete) {
            // Stop at the first rejected native operation, before an older
            // wrapper can discard its result and report a misleading transfer.
            if (!faulted()) {
                fault_ = {"Native function rejected execution", c.registers.program_counter,
                    result.exit_program_counter, 0U, 0U, c.cpu,
                    f->id, f->address, c.state, static_cast<std::uint8_t>(result.status), result.control};
            }
            checkpoint();
            break;
        }
        if (result.control == 9U) {
            // Guest stack reset: every frame above the anchor is abandoned
            // with the guest frames it mirrored; the anchor continues there.
            if (reset_anchor_[c.cpu] != &frame) break;
            if (c.registers.program_counter != result.exit_program_counter) {
                fail("Stack reset result and architectural PC disagree");
                result = {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
                break;
            }
            f = entry_at(c.cpu, c.state, c.registers.program_counter);
            if (!f) {
                fail("No exact native entry for stack reset target", c.registers.program_counter);
                result = {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
                break;
            }
            continue;
        }
        if (result.control == 5U && frame.executed_child && frame.child.control == 2U &&
            c.registers.program_counter >= f->body_min && c.registers.program_counter <= f->body_max) {
            // A translated instruction suspended for an ISR. Resume its owner
            // at the architectural return PC after the native ISR completed.
            continue;
        }
        if (result.control == 5U && frame.executed_child) { result = frame.child; break; }
        // Retained trampolines and self-loop bodies can report a transfer or
        // loop boundary after the live child has already run through RTS/RTE.
        // Preserve that actual return for both control-3 and control-4 wrappers.
        if ((result.control == 3U || result.control == 4U) && frame.executed_child &&
            result.exit_program_counter == frame.child_target &&
            frame.child.status == TranslationStatus::complete &&
            frame.child.exit_program_counter == c.registers.program_counter &&
            (frame.child.control == 1U || frame.child.control == 2U)) {
            result = frame.child;
            break;
        }
        if (result.control != 3U) break;
        if (c.registers.program_counter != result.exit_program_counter) {
            fail("Transfer result and architectural PC disagree");
            result = {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
            break;
        }
        f = entry_at(c.cpu, c.state, c.registers.program_counter);
        if (!f) {
            fail("No exact native entry for transfer target", c.registers.program_counter);
            result = {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
            break;
        }
    }
    if (reset_anchor_[c.cpu] == &frame) reset_anchor_[c.cpu] = nullptr;
    --depth_;
    active_ = previous_context;
    invocation_ = frame.parent;
    return result;
}

FunctionResult RuntimeHost::run(FunctionContext &c)
{
    c.host = this;
    const auto *f = entry_at(c.cpu, c.state, c.registers.program_counter);
    if (!f) {
        active_ = &c;
        fail("No exact native entry for CPU start", c.registers.program_counter);
        active_ = nullptr;
        return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    }
    return execute(*f, c);
}

FunctionResult RuntimeHost::call_function(std::uint32_t id, std::uint8_t cpu,
    std::uint8_t state, std::uint8_t kind, std::uint32_t site, std::uint32_t target, FunctionContext &c)
{
    checkpoint();
    const auto *f = entry_at(cpu, state, target);
    if (faulted() || !f || f->id != id || c.cpu != cpu) {
        fail("Native call target/CPU/state mapping is unresolved", target);
        return {TranslationStatus::contract_violation, 0U, target};
    }
    if (devices_ && cpu == 1U && state == 0x72U && (id == 309U || id == 310U)) {
        // Presentation-only override for the original title command. Never
        // suppress or replace the translated call, queue writes or YM timing.
        const auto command = c.registers.data[0] & 0xffffU;
        // DBCC checks the credit-change flag; DBD2/DBD6 submit cue 0x36.
        // It overlays the title, so preserve both replacement playback and
        // the original title-voice mask, including before key-on and after EOF.
        const bool coin_credit_cue = id == 309U && site == 0xdbd6U && command == 0x36U;
        if (!coin_credit_cue)
            devices_->audio.notify_sound_command(id == 309U && site == 0xd430U && command == 0x52U);
    }
    if (assets_ && cpu == 0U && state == 0xffU && kind == 4U &&
        (id == 50U || id == 51U || id == 52U || id == 53U)) {
        if (c.state != state || c.registers.program_counter != target) {
            fail("Direct load trap architectural state disagrees with target", target);
            return {TranslationStatus::contract_violation, 0U, target};
        }
        return direct_load_trap(id, c);
    }
    if (kind == 0U) {
        // A sequential partition marker: let its enclosing body finish, then
        // the outer dispatcher executes the returned control-3 continuation.
        return FunctionResult::complete(3U, target);
    }
    if (kind != 1U && kind != 2U && kind != 3U && kind != 4U && kind != 6U) {
        fail("Exception/FD1094 transition requires a live device model", target);
        return {TranslationStatus::contract_violation, 0U, target};
    }
    // Older bodies assign the IRQ state before calling the host. The active
    // owner's state still identifies the state to restore after its ISR.
    const auto source_state = invocation_ ? invocation_->executing_state : c.state;
    if (kind == 6U) c.state = state; // explicit native FD1094/IRQ target mapping
    if (c.state != state || c.registers.program_counter != target) {
        fail("Native call architectural state disagrees with target", target);
        return {TranslationStatus::contract_violation, 0U, target};
    }
    // IRQ5's prologue and frame-commit body now advance their own bus and
    // internal clocks. No aggregate minimum-duration delay owns that time.
    // D2 still contains the request ID on entry to the priority selector.
    // An enqueued coin is insufficient: rejected requests must not unmute
    // the original title voices. The accepted cue's own key-ons release them.
    const bool coin_request = devices_ && cpu == 0U && state == 0xffU &&
        id == 85U && site == 0x83d1aU && (c.registers.data[2] & 0xffffU) == 0x36U;
    const auto stack_before = c.registers.address[7];
    const auto caller_id = invocation_ ? invocation_->function_id : 0U;
    // CPU B stack words as the caller left them: [a7] is the caller's own
    // continuation (just pushed), [a7+4] is the caller's return address.
    const auto stack_long = [&](std::uint32_t a) -> std::optional<std::uint32_t> {
        const auto &b = regions_[2].bytes;
        if (cpu != 1U || a + 8U > b.size()) return std::nullopt;
        return (std::uint32_t(b[a]) << 24U) | (std::uint32_t(b[a + 1U]) << 16U) | (std::uint32_t(b[a + 2U]) << 8U) | b[a + 3U];
    };
    const auto caller_next = stack_long(stack_before);
    const auto caller_return = stack_long(stack_before + 4U);
    if (kind == 1U && stack_reset_entry(cpu, target) && reset_anchor_[cpu]) {
        // A jump into the stack-reset entry discards every guest frame. Unwind
        // the native frames that mirrored them instead of nesting beneath them.
        if (invocation_) {
            invocation_->child = FunctionResult::complete(9U, target);
            invocation_->child_target = target;
            invocation_->executed_child = true;
        }
        return FunctionResult::complete(9U, target);
    }
    auto child = execute(*f, c, site);
    // Nonlocal returns. Some originals drop their caller's frame and return
    // to the caller's caller. Hand translations report that as control 8;
    // a literal translation reports control 1 with the stack popped twice.
    // Normalise both here so every caller, generated or hand-written, sees a
    // control-8 result pass through it, and the frame whose continuation the
    // return lands on sees an ordinary return.
    if (cpu == 1U && child.status == TranslationStatus::complete) {
        const auto pc = c.registers.program_counter;
        if (child.control == 1U && caller_return && c.registers.address[7] == stack_before + 8U && pc == *caller_return)
            child = FunctionResult::complete(8U, pc);
        else if (child.control == 8U && caller_next && pc == *caller_next)
            child = FunctionResult::complete(1U, pc);
    }
    // Opt-in diagnostics (GAIN_GROUND_NAV_LOG): every child result that is not a
    // plain return to its caller, with the stack movement it left behind.
    if (devices_ && (child.status != TranslationStatus::complete || child.control != 1U ||
                     c.registers.address[7] != stack_before + 4U)) {
        static std::FILE *log = [] { const char *p = std::getenv("GAIN_GROUND_NAV_LOG"); return p && *p ? std::fopen(p, "a") : nullptr; }();
        if (log) {
            std::fprintf(log, "call frame %llu caller %u site %06x callee %u target %06x -> status %u control %u exit %06x a7 %05x -> %05x\n",
                static_cast<unsigned long long>(devices_->frame()), unsigned(caller_id), unsigned(site), unsigned(id), unsigned(target),
                unsigned(child.status), unsigned(child.control), unsigned(child.exit_program_counter),
                unsigned(stack_before), unsigned(c.registers.address[7]));
            std::fflush(log);
        }
    }
    if (coin_request && child.status == TranslationStatus::complete && child.control == 1U)
        devices_->audio.notify_coin_credit_accepted();
    if (child.status == TranslationStatus::complete && child.control == 2U &&
        (kind == 3U || kind == 4U || kind == 6U)) c.state = source_state;
    if (invocation_) {
        invocation_->child = child;
        invocation_->child_target = target;
        invocation_->executed_child = true;
    }
    return child;
}

}
