#include "fixture_compare_internal.h"
#include <cstdlib>
#include <iostream>
#include <utility>
namespace gain_ground::fixture_compare_detail {
[[nodiscard]] PendingInterrupt ComparingHost::consume_pending_interrupt(
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t completed_instruction_pc){
        if (cpu == 0U && call_index_ < fixture_.calls.size()) {
            const auto &call = fixture_.calls[call_index_];
            // Some translated instructions ask before emitting the captured
            // interrupt-only look-ahead prefetch; older owners ask after it.
            if (call.target_cpu == cpu && call.target_state == state
                    && call.callsite == completed_instruction_pc
                    && call.sequence >= 7U
                    && (next_sequence_ == call.sequence - 7U
                        || (call.sequence >= 8U && next_sequence_ == call.sequence - 8U))
                    && call.function_id < generated::kFunctions.size()) {
                const auto label = generated::kFunctions[call.function_id].label;
                const auto marker = label.find("_irq");
                if (marker != std::string_view::npos && marker + 4U < label.size()) {
                    const auto digit = label[marker + 4U];
                    if (digit >= '1' && digit <= '7') {
                        return {true, static_cast<std::uint8_t>(digit - '0')};
                    }
                }
            }
            // Older tail-continuation fixtures omit the Function 46 vector
            // ownership marker but retain the exact IRQ3 trampoline/service
            // call. The translated loop asks immediately before the captured
            // interrupt-only look-ahead prefetch. That prefetch plus three
            // frame writes, four vector/stub reads, and three trampoline reads
            // place the Function 72 call eleven bus events later.
            if (call.function_id == 72U && call.target_cpu == cpu
                    && call.target_state == state
                    && call.callsite == 0x00080042U
                    && call.target == 0x00080f96U
                    && call.sequence >= 11U
                    && next_sequence_ == call.sequence - 11U)
                return {true, 3U};
        }
        for (std::size_t index = 0; index != fixture_.edges.size(); ++index) {
            if (consumed_interrupt_edges_.contains(index))
                continue;
            const auto &edge = fixture_.edges[index];
            if (edge.kind != 6U || edge.from_state != state
                || edge.source_pc != completed_instruction_pc
                || (cpu == 1U && edge.to_state != 0x04U))
                continue;
            bool at_captured_occurrence = false;
            for (const auto &call : fixture_.calls) {
                if (call.kind != 6U || call.callsite != completed_instruction_pc
                    || call.target != edge.target_pc || call.sequence < 5U)
                    continue;
                // A 68000 interrupt entry emits three stack writes and two
                // vector reads before the captured kind-6 ownership call.
                // Matching that sequence distinguishes repeated loop PCs.
                at_captured_occurrence = next_sequence_ == call.sequence - 5U;
                break;
            }
            if (!at_captured_occurrence)
                continue;
            for (const auto &function : generated::kFunctions) {
                if (function.cpu != cpu || function.state != edge.to_state
                    || function.address != edge.target_pc)
                    continue;
                const auto marker = function.label.find("_irq");
                if (marker == std::string_view::npos || marker + 4U >= function.label.size())
                    return {};
                const auto digit = function.label[marker + 4U];
                if (digit < '1' || digit > '7')
                    return {};
                consumed_interrupt_edges_.insert(index);
                return {true, static_cast<std::uint8_t>(digit - '0')};
            }
        }
        return {};
    }

void ComparingHost::observe_inline_call(std::uint8_t cpu, std::uint8_t state,
        std::uint32_t site, std::uint32_t target, FunctionContext &context){
        // Internal BSRs were recorded without a separate function owner.
        // Validate the marker; the enclosing translation executes the helper.
        (void)call_function(UINT32_MAX, cpu, state, 2U, site, target, context);
    }

[[nodiscard]] bool ComparingHost::consume_self_continuation_boundary(
        std::uint32_t function_id,
        std::uint8_t target_cpu,
        std::uint8_t target_state,
        std::uint8_t kind,
        std::uint32_t callsite,
        std::uint32_t target,
        FunctionContext &context){
        // A control-4 record may end at the repeated entry after an interrupt
        // without a separate call marker. Otherwise consume only the exact
        // next self-continuation marker; inline records continue normally.
        if (fixture_.control == 4U && fixture_.exit_pc == target
                && call_index_ == fixture_.calls.size())
            return true;
        // F115/F116 captures retain thousands of BPL backedges inside F118,
        // then end at an interrupt. Consume each exact marker in this loop:
        // recursively calling F118 exhausts the stack and turns the eventual
        // interrupt result into a self-continuation boundary on unwind.
        const bool enclosing_f118_wait_marker = (fixture_.function_id == 115U
                || fixture_.function_id == 116U || fixture_.function_id == 628U)
            && function_id == 118U
            && target_cpu == 1U && target_state == 0x72U
            && kind == 1U
            && callsite == 0x000085b4U && target == 0x000085b0U;
        // F55 record 247 contains repeated F61 backedges inside the caller
        // capture (sequences 12, 18, 24, ...). They continue the existing loop;
        // recursively invoking F61 for each marker exhausts the host stack.
        const bool enclosing_f55_wait_marker = fixture_.function_id == 55U
            && function_id == 61U && target_cpu == 0U && target_state == 0xffU
            && kind == 1U && callsite == 0x0008034eU && target == 0x0008034aU;
        const bool enclosing_wait_marker = enclosing_f118_wait_marker || enclosing_f55_wait_marker;
        if (call_index_ >= fixture_.calls.size()) {
            if (enclosing_wait_marker)
                set_divergence("call", "count", "captured continuation marker",
                    "missing", next_sequence_);
            return enclosing_wait_marker;
        }
        const auto &expected = fixture_.calls[call_index_];
        if (expected.sequence != next_sequence_
                || expected.function_id != function_id
                || expected.target_cpu != target_cpu
                || expected.target_state != target_state
                || expected.kind != kind
                || expected.callsite != callsite
                || expected.target != target) {
            if (enclosing_wait_marker)
                set_divergence("call", "continuation_marker",
                    "exact sequence/cpu/state/kind/callsite/target",
                    "mismatched", next_sequence_);
            return enclosing_wait_marker;
        }
        if (enclosing_wait_marker) {
            if (hardware_index_ < fixture_.hardware_effects.size()
                    && fixture_.hardware_effects[hardware_index_].sequence
                        <= expected.sequence) {
                set_divergence("hardware", "continuation_marker_order",
                    "no pending event through marker sequence", "pending event",
                    expected.sequence);
                return true;
            }
            ++next_sequence_;
            ++call_index_;
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return false;
        }
        const auto boundary = call_function(function_id, target_cpu, target_state,
            kind, callsite, target, context);
        return boundary.status == TranslationStatus::complete;
    }
}
