#include "fixture_compare_internal.h"
#include <cstdlib>
#include <iostream>
#include <utility>
namespace gain_ground::fixture_compare_detail {
void ComparingHost::finish(){
        if (call_index_ != fixture_.calls.size())
            set_divergence("call", "count", number(fixture_.calls.size()), number(call_index_), next_sequence_);
        if (hardware_index_ != fixture_.hardware_effects.size())
            set_divergence("hardware", "count", number(fixture_.hardware_effects.size()), number(hardware_index_), next_sequence_);

        for (std::size_t index = 0; index != fixture_.edges.size(); ++index) {
            const auto &edge = fixture_.edges[index];
            if (edge.kind != 6U || consumed_interrupt_edges_.contains(index)
                || (fixture_.cpu == 1U && edge.to_state != 0x04U))
                continue;
            for (const auto &function : generated::kFunctions) {
                if (function.cpu != fixture_.cpu || function.state != edge.to_state
                    || function.address != edge.target_pc)
                    continue;
                const auto marker = function.label.find("_irq");
                if (marker != std::string_view::npos && marker + 4U < function.label.size()
                    && function.label[marker + 4U] >= '1'
                    && function.label[marker + 4U] <= '7')
                    set_divergence("interrupt", "consumed", "true", "false", next_sequence_);
                break;
            }
        }

        for (const auto &[key, actual] : memory_) {
            const auto &expected = *actual.expected;
            const std::string address = number(key.region) + ":" + hex(key.offset, 8);
            if (actual.flags != (expected.flags & 3U))
                set_divergence("memory", address + ".flags", number(expected.flags & 3U), number(actual.flags), expected.first_sequence);
            else if (actual.read_mask != expected.read_mask)
                set_divergence("memory", address + ".read_mask", hex(expected.read_mask, 4), hex(actual.read_mask, 4), expected.first_sequence);
            else if (actual.access_count != expected.access_count)
                set_divergence("memory", address + ".access_count", number(expected.access_count), number(actual.access_count), expected.first_sequence);
            else if (actual.first_sequence != expected.first_sequence)
                set_divergence("memory", address + ".first_sequence", number(expected.first_sequence), number(actual.first_sequence), expected.first_sequence);

            if (actual.expected_delta) {
                const auto &delta = *actual.expected_delta;
                if (actual.game_current != delta.final_value)
                    set_divergence("memory", address + ".final", hex(delta.final_value, 4), hex(actual.game_current, 4), delta.last_sequence);
                else if (actual.write_mask != delta.write_mask)
                    set_divergence("memory", address + ".write_mask", hex(delta.write_mask, 4), hex(actual.write_mask, 4), delta.first_sequence);
                else if (actual.write_count != delta.write_count)
                    set_divergence("memory", address + ".write_count", number(delta.write_count), number(actual.write_count), delta.first_sequence);
                else if (actual.first_write_sequence != delta.first_sequence)
                    set_divergence("memory", address + ".first_write_sequence", number(delta.first_sequence), number(actual.first_write_sequence), delta.first_sequence);
                else if (actual.last_write_sequence != delta.last_sequence)
                    set_divergence("memory", address + ".last_write_sequence", number(delta.last_sequence), number(actual.last_write_sequence), delta.last_sequence);
            } else if (actual.write_count != 0) {
                set_divergence("memory", address + ".write_count", "0", number(actual.write_count), actual.first_write_sequence);
            }
        }
    }

[[nodiscard]] bool ComparingHost::diverged() const noexcept{ return !divergence_.field.empty(); }

void ComparingHost::record_compatibility_rule(std::string_view rule){
        const bool allowed = generated::fixture_marker_compatibility_allowed(
            rule, fixture_.bundle.source_index, fixture_.function_id,
            fixture_.bundle.source_fixture_ordinal);
        if (!allowed) {
            set_divergence(
                "compatibility",
                "fixture_marker_provenance",
                "hash-bound source/index/root/ordinal allowlist entry",
                std::string(rule),
                0U);
            return;
        }
        compatibility_rules_.emplace_back(rule);
    }

void ComparingHost::set_divergence(
        std::string subsystem,
        std::string field,
        std::string expected,
        std::string actual,
        std::uint32_t sequence){
        if (!diverged()) {
            divergence_ = {std::move(subsystem), std::move(field), std::move(expected), std::move(actual), sequence};
            // A translated wait loop may never return after a mismatch.
            // Optional diagnostics expose the first failure without changing replay.
            if (std::getenv("GG_REPLAY_TRACE_FIRST_DIVERGENCE") != nullptr)
                std::cerr << "first divergence: function=" << fixture_.function_id
                          << " record=" << fixture_record_ << " sequence=" << sequence
                          << " " << divergence_.subsystem << "." << divergence_.field
                          << " expected=" << divergence_.expected
                          << " actual=" << divergence_.actual << std::endl;
        }
    }
}
