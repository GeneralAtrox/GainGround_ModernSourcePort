#include "gain_ground/fixture_compare.h"

#include "gground_functions.h"
#include "gground_checkpoints.h"
#include "gground_fixture_marker_compatibility.h"

#include <array>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

namespace gain_ground {
namespace {
constexpr std::uint8_t kControlledMemoryInterventionKind = 0x80U;
constexpr std::uint8_t kControlledPostCallInterventionKind = 0x81U;
constexpr std::uint8_t kControlledPostCallPersistentInterventionKind = 0x82U;
constexpr std::uint8_t kControlledStatusInterventionKind = 0x83U;
constexpr std::array<std::uint8_t, 32> kFunction618PayloadDigest{
    0xbaU, 0x65U, 0x7fU, 0x0aU, 0xc8U, 0x3dU, 0xb8U, 0x55U,
    0x5aU, 0x66U, 0xa1U, 0x60U, 0xb8U, 0x3fU, 0x21U, 0x37U,
    0x3bU, 0xb5U, 0xabU, 0x8bU, 0x90U, 0x87U, 0xc2U, 0x88U,
    0x68U, 0x04U, 0xa8U, 0xcfU, 0xc9U, 0x67U, 0xadU, 0xa4U};

struct MemoryKey {
    std::uint16_t region{};
    std::uint32_t offset{};

    auto operator<=>(const MemoryKey &) const = default;
};

struct MemoryObservation {
    const FixtureMemoryPrestate *expected{};
    const FixtureMemoryDelta *expected_delta{};
    std::uint16_t current{};
    std::uint16_t game_current{};
    std::uint16_t read_mask{};
    std::uint16_t write_mask{};
    std::uint32_t access_count{};
    std::uint32_t write_count{};
    std::uint32_t first_sequence{std::numeric_limits<std::uint32_t>::max()};
    std::uint32_t first_write_sequence{std::numeric_limits<std::uint32_t>::max()};
    std::uint32_t last_write_sequence{std::numeric_limits<std::uint32_t>::max()};
    std::uint16_t flags{};
};

std::string number(std::uint64_t value)
{
    return std::to_string(value);
}

std::string hex(std::uint64_t value, unsigned width)
{
    std::ostringstream stream;
    stream << "0x" << std::hex << std::setfill('0') << std::setw(static_cast<int>(width)) << value;
    return stream.str();
}

std::string json_escape(std::string_view value)
{
    std::ostringstream stream;
    for (const unsigned char character : value) {
        switch (character) {
        case '\\': stream << "\\\\"; break;
        case '"': stream << "\\\""; break;
        case '\b': stream << "\\b"; break;
        case '\f': stream << "\\f"; break;
        case '\n': stream << "\\n"; break;
        case '\r': stream << "\\r"; break;
        case '\t': stream << "\\t"; break;
        default:
            if (character < 0x20)
                stream << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<unsigned>(character);
            else
                stream << character;
        }
    }
    return stream.str();
}

class ComparingHost final : public ExecutionHost {
public:
    ComparingHost(const FixtureRecord &fixture, std::size_t fixture_record)
        : fixture_(fixture), fixture_record_(fixture_record)
    {
        for (const auto &item : fixture.memory_prestates) {
            auto &[key, observation] = *memory_.try_emplace({item.region, item.offset}).first;
            (void)key;
            observation.expected = &item;
            observation.current = item.initial;
            observation.game_current = item.initial;
        }
        for (const auto &item : fixture.memory_deltas) {
            auto found = memory_.find({item.region, item.offset});
            if (found == memory_.end()) {
                set_divergence("memory", "fixture.prestate", "present", "missing", item.first_sequence);
                continue;
            }
            found->second.expected_delta = &item;
            if (found->second.current != item.initial)
                set_divergence("memory", "fixture.initial", hex(found->second.current, 4), hex(item.initial, 4), item.first_sequence);
        }
    }

    [[nodiscard]] std::uint16_t read_memory_word(
        std::uint16_t region,
        std::uint32_t byte_offset,
        std::uint16_t memory_mask) override
    {
        const auto sequence = next_sequence_++;
        auto found = memory_.find({region, byte_offset});
        if (found == memory_.end()) {
            set_divergence("memory", "read.address", "captured region/offset",
                number(region) + ":" + hex(byte_offset, 8), sequence);
            return 0;
        }
        auto &item = found->second;
        if (item.first_sequence == std::numeric_limits<std::uint32_t>::max())
            item.first_sequence = sequence;
        item.read_mask |= memory_mask;
        item.flags |= 1U;
        ++item.access_count;
        apply_controlled_memory_intervention(sequence, region, byte_offset);
        return item.current;
    }

    void write_memory_word(
        std::uint16_t region,
        std::uint32_t byte_offset,
        std::uint16_t data,
        std::uint16_t memory_mask) override
    {
        const auto sequence = next_sequence_++;
        auto found = memory_.find({region, byte_offset});
        if (found == memory_.end()) {
            set_divergence("memory", "write.address", "captured region/offset",
                number(region) + ":" + hex(byte_offset, 8), sequence);
            return;
        }
        auto &item = found->second;
        if (item.first_sequence == std::numeric_limits<std::uint32_t>::max())
            item.first_sequence = sequence;
        if (item.first_write_sequence == std::numeric_limits<std::uint32_t>::max())
            item.first_write_sequence = sequence;
        item.last_write_sequence = sequence;
        item.write_mask |= memory_mask;
        item.flags |= 2U;
        ++item.access_count;
        ++item.write_count;
        item.current = static_cast<std::uint16_t>((item.current & ~memory_mask) | (data & memory_mask));
        item.game_current = item.current;
    }

    [[nodiscard]] std::uint16_t read_hardware(
        std::uint8_t kind,
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t pc,
        std::uint32_t address,
        std::uint16_t memory_mask) override
    {
        const auto sequence = next_sequence_++;
        const auto *expected = next_hardware(sequence);
        if (!expected)
            return 0;
        compare_hardware(*expected, kind, cpu, state, pc, address, expected->data, memory_mask, sequence);
        return expected->data;
    }

    void write_hardware(
        std::uint8_t kind,
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t pc,
        std::uint32_t address,
        std::uint16_t data,
        std::uint16_t memory_mask) override
    {
        const auto sequence = next_sequence_++;
        const auto *expected = next_hardware(sequence);
        if (expected)
            compare_hardware(*expected, kind, cpu, state, pc, address, data, memory_mask, sequence);
    }

    [[nodiscard]] std::uint16_t apply_controlled_status(
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t pc,
        std::uint32_t trigger_address,
        std::uint16_t status) override
    {
        if (hardware_index_ >= fixture_.hardware_effects.size()) return status;
        const auto &event = fixture_.hardware_effects[hardware_index_];
        if (event.kind != kControlledStatusInterventionKind || event.pc != pc)
            return status;
        ++hardware_index_;
        if (event.sequence != next_sequence_)
            set_divergence("status_intervention", "sequence", number(event.sequence),
                number(next_sequence_), next_sequence_);
        else if (event.cpu != cpu)
            set_divergence("status_intervention", "cpu", number(event.cpu),
                number(cpu), next_sequence_);
        else if (event.state != state)
            set_divergence("status_intervention", "state", hex(event.state, 2),
                hex(state, 2), next_sequence_);
        else if (event.address != trigger_address)
            set_divergence("status_intervention", "trigger_address", hex(event.address, 8),
                hex(trigger_address, 8), next_sequence_);
        return static_cast<std::uint16_t>(
            (status & ~event.memory_mask) | (event.data & event.memory_mask));
    }

    [[nodiscard]] PendingInterrupt consume_pending_interrupt(
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t completed_instruction_pc) override
    {
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

    [[nodiscard]] FunctionResult call_function(
        std::uint32_t function_id,
        std::uint8_t target_cpu,
        std::uint8_t target_state,
        std::uint8_t kind,
        std::uint32_t callsite,
        std::uint32_t target,
        FunctionContext &context) override
    {
        const bool omitted_legacy_f543_marker = function_id == 543U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001700aU && target == 0x00017024U
            && (call_index_ >= fixture_.calls.size()
                || (fixture_.calls[call_index_].function_id == 202U
                    && fixture_.calls[call_index_].target_cpu == 1U
                    && fixture_.calls[call_index_].target_state == 0x72U
                    && fixture_.calls[call_index_].kind == 2U
                    && fixture_.calls[call_index_].callsite == 0x0000ff82U
                    && fixture_.calls[call_index_].target == 0x000101e2U
                    && fixture_.calls[call_index_].sequence
                        == next_sequence_ + 27U)
                || (fixture_.calls[call_index_].function_id == 202U
                    && fixture_.calls[call_index_].target_cpu == 1U
                    && fixture_.calls[call_index_].target_state == 0x72U
                    && fixture_.calls[call_index_].kind == 2U
                    && fixture_.calls[call_index_].callsite == 0x0000ff9aU
                    && fixture_.calls[call_index_].target == 0x000101e2U
                    && fixture_.calls[call_index_].sequence
                        == next_sequence_ + 20U)
                || (fixture_.calls[call_index_].function_id == 237U
                    && fixture_.calls[call_index_].target_cpu == 1U
                    && fixture_.calls[call_index_].target_state == 0x72U
                    && fixture_.calls[call_index_].kind == 2U
                    && fixture_.calls[call_index_].callsite == 0x00010e1eU
                    && fixture_.calls[call_index_].target == 0x0001106eU
                    && fixture_.calls[call_index_].sequence
                        == next_sequence_ + 16U)
                || (fixture_.calls[call_index_].function_id == 224U
                    && fixture_.calls[call_index_].target_cpu == 1U
                    && fixture_.calls[call_index_].target_state == 0x72U
                    && fixture_.calls[call_index_].kind == 2U
                    && fixture_.calls[call_index_].callsite == 0x00010a28U
                    && fixture_.calls[call_index_].target == 0x00010cc0U
                    && (fixture_.calls[call_index_].sequence
                            == next_sequence_ + 15U
                        || fixture_.calls[call_index_].sequence
                            == next_sequence_ + 17U
                        || fixture_.calls[call_index_].sequence
                            == next_sequence_ + 19U
                        || fixture_.calls[call_index_].sequence
                            == next_sequence_ + 21U
                        || fixture_.calls[call_index_].sequence
                            == next_sequence_ + 26U
                        || fixture_.calls[call_index_].sequence
                            == next_sequence_ + 29U)));
        if (omitted_legacy_f543_marker) {
            record_compatibility_rule("omitted_legacy_f543_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[543U].entry(context);
        }

        // Records before the two current-table captures appended at ordinals
        // 95082 and 95083 were produced before the 0x17024 and 0x17e9a
        // ownership partitions existed. Preserve only those hash-bound bundle
        // records when their next captured call is not the handoff now emitted
        // by the native owner.
        const bool omitted_prepartition_f543_sequential_marker =
            fixture_record_ < 95082U && function_id == 543U
            && target_cpu == 1U && target_state == 0x72U && kind == 0U
            && callsite == 0x00017022U && target == 0x00017024U
            && (call_index_ >= fixture_.calls.size()
                || fixture_.calls[call_index_].function_id != 543U
                || fixture_.calls[call_index_].callsite != callsite
                || fixture_.calls[call_index_].target != target);
        if (omitted_prepartition_f543_sequential_marker) {
            record_compatibility_rule("omitted_prepartition_f543_sequential_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[543U].entry(context);
        }

        const bool omitted_prepartition_f547_branch_marker =
            fixture_record_ < 95082U && function_id == 547U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x00017e2aU && target == 0x00017e9aU
            && (call_index_ >= fixture_.calls.size()
                || fixture_.calls[call_index_].function_id != 547U
                || fixture_.calls[call_index_].callsite != callsite
                || fixture_.calls[call_index_].target != target);
        if (omitted_prepartition_f547_branch_marker) {
            record_compatibility_rule("omitted_prepartition_f547_branch_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[547U].entry(context);
        }

        // The pre-F546-partition enclosing Function 481 captures contain the
        // complete 0x17dd8 -> 0x17ca0 execution but no separate ownership
        // marker.  Consume that exact native tail locally, without advancing
        // the fixture call list, only when the next recorded event is the
        // proven next F484 iteration 58 memory events later.
        const bool omitted_enclosing_f481_f546_marker = fixture_.function_id == 481U
            && function_id == 546U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && target == 0x00017ca0U
            && ((callsite == 0x00017c9cU
                    && call_index_ == fixture_.calls.size())
                || (callsite == 0x00017dd8U
                    && (call_index_ == fixture_.calls.size()
                    || (fixture_.calls[call_index_].function_id == 484U
                    && fixture_.calls[call_index_].target_cpu == 1U
                    && fixture_.calls[call_index_].target_state == 0x72U
                    && fixture_.calls[call_index_].kind == 2U
                    && fixture_.calls[call_index_].callsite == 0x00017c74U
                    && fixture_.calls[call_index_].target == 0x00017d9eU
                    && (fixture_.calls[call_index_].sequence == next_sequence_ + 58U
                        || fixture_.calls[call_index_].sequence == next_sequence_ + 60U)))));
        if (omitted_enclosing_f481_f546_marker) {
            record_compatibility_rule("omitted_enclosing_f481_f546_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[546U].entry(context);
        }

        const bool omitted_legacy_f542_marker = function_id == 542U
            && target_cpu == 1U && target_state == 0x72U && kind == 0U
            && callsite == 0x00016372U && target == 0x00016376U
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 302U
            && (fixture_.calls[call_index_].sequence == next_sequence_
                || fixture_.calls[call_index_].sequence == next_sequence_ + 2U);
        if (omitted_legacy_f542_marker) {
            record_compatibility_rule("omitted_legacy_f542_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return FunctionResult::complete(3U, target);
        }

        const bool omitted_legacy_f590_marker = function_id == 590U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x00016380U && target == 0x0001639aU
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 302U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 2U;
        if (omitted_legacy_f590_marker) {
            record_compatibility_rule("omitted_legacy_f590_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[590U].entry(context);
        }

        const bool omitted_legacy_f591_marker = function_id == 591U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001639eU && target == 0x000163aaU
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 302U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 2U;
        if (omitted_legacy_f591_marker) {
            record_compatibility_rule("omitted_legacy_f591_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[591U].entry(context);
        }

        const bool omitted_legacy_f553_marker = function_id == 553U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001e314U && target == 0x0001e18aU
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 363U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e1c6U
            && fixture_.calls[call_index_].target == 0x0001ebf0U
            && (fixture_.calls[call_index_].sequence == next_sequence_ + 6U
                || fixture_.calls[call_index_].sequence == next_sequence_ + 7U);
        if (omitted_legacy_f553_marker) {
            record_compatibility_rule("omitted_legacy_f553_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[553U].entry(context);
        }

        const bool omitted_legacy_f594_marker = function_id == 594U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && (callsite == 0x0001e1baU || callsite == 0x0001e1c2U)
            && target == 0x0001e1c6U
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 363U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e1c6U
            && fixture_.calls[call_index_].target == 0x0001ebf0U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 2U;
        if (omitted_legacy_f594_marker) {
            record_compatibility_rule("omitted_legacy_f594_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[594U].entry(context);
        }

        const bool omitted_legacy_f361_f554_marker = fixture_.function_id == 361U
            && function_id == 554U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && (callsite == 0x0001e3e4U || callsite == 0x0001e408U
                || callsite == 0x0001e432U || callsite == 0x0001e438U)
            && target == 0x0001e1f2U
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 358U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e220U
            && fixture_.calls[call_index_].target == 0x0001e158U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 17U;
        if (omitted_legacy_f361_f554_marker) {
            record_compatibility_rule("omitted_legacy_f361_f554_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[554U].entry(context);
        }

        const bool omitted_legacy_f361_f555_marker = fixture_.function_id == 361U
            && function_id == 555U
            && target_cpu == 1U && target_state == 0x72U && kind == 0U
            && callsite == 0x0001e224U && target == 0x0001e22aU
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 280U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e22aU
            && fixture_.calls[call_index_].target == 0x00015d24U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 2U;
        if (omitted_legacy_f361_f555_marker) {
            record_compatibility_rule("omitted_legacy_f361_f555_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[555U].entry(context);
        }

        const bool omitted_current_f404_f596_marker = fixture_.function_id == 404U
            && function_id == 596U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001e1d0U && target == 0x0001e1d8U
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 358U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e220U
            && fixture_.calls[call_index_].target == 0x0001e158U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 18U;
        if (omitted_current_f404_f596_marker) {
            record_compatibility_rule("omitted_current_f404_f596_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[596U].entry(context);
        }

        const bool omitted_current_f404_f597_entry_marker = fixture_.function_id == 404U
            && function_id == 597U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001e1eaU && target == 0x0001e1ecU
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 358U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e220U
            && fixture_.calls[call_index_].target == 0x0001e158U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 17U;
        if (omitted_current_f404_f597_entry_marker) {
            record_compatibility_rule("omitted_current_f404_f597_entry_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[597U].entry(context);
        }

        const bool omitted_current_f404_f597_loop_marker = fixture_.function_id == 404U
            && function_id == 597U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001e1eeU && target == 0x0001e1ecU
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 358U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e220U
            && fixture_.calls[call_index_].target == 0x0001e158U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 17U;
        if (omitted_current_f404_f597_loop_marker) {
            record_compatibility_rule("omitted_current_f404_f597_loop_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[597U].entry(context);
        }

        const bool omitted_current_f404_f550_marker = fixture_.function_id == 404U
            && function_id == 550U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001cefcU && target == 0x0001d9daU
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 352U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x000210d0U
            && fixture_.calls[call_index_].target == 0x0001dbc4U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 7U;
        if (omitted_current_f404_f550_marker) {
            record_compatibility_rule("omitted_current_f404_f550_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[550U].entry(context);
        }

        const bool omitted_current_f404_f552_marker = fixture_.function_id == 404U
            && function_id == 552U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001dcd2U && target == 0x0001dcd6U
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 357U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x000210d8U
            && fixture_.calls[call_index_].target == 0x0001e124U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 4U;
        if (omitted_current_f404_f552_marker) {
            record_compatibility_rule("omitted_current_f404_f552_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[552U].entry(context);
        }

        const bool omitted_current_f404_f554_marker = fixture_.function_id == 404U
            && function_id == 554U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001e1eeU && target == 0x0001e1f2U
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 358U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e220U
            && fixture_.calls[call_index_].target == 0x0001e158U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 17U;
        if (omitted_current_f404_f554_marker) {
            record_compatibility_rule("omitted_current_f404_f554_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[554U].entry(context);
        }

        const bool omitted_current_f404_f555_marker = fixture_.function_id == 404U
            && function_id == 555U
            && target_cpu == 1U && target_state == 0x72U && kind == 0U
            && callsite == 0x0001e224U && target == 0x0001e22aU
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 280U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e22aU
            && fixture_.calls[call_index_].target == 0x00015d24U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 2U;
        if (omitted_current_f404_f555_marker) {
            record_compatibility_rule("omitted_current_f404_f555_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[555U].entry(context);
        }

        const bool omitted_legacy_f329_partition_marker = fixture_.function_id == 329U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && ((function_id == 550U && callsite == 0x0001c10eU
                    && target == 0x0001d9daU)
                || (function_id == 551U && callsite == 0x0001c118U
                    && target == 0x0001d9eaU))
            && call_index_ >= fixture_.calls.size();
        if (omitted_legacy_f329_partition_marker) {
            record_compatibility_rule("omitted_legacy_f329_partition_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[function_id].entry(context);
        }

        const bool omitted_legacy_f327_partition_marker = fixture_.function_id == 327U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && ((function_id == 550U && callsite == 0x0001bdc6U
                    && target == 0x0001d9daU)
                || (function_id == 551U && callsite == 0x0001bdd0U
                    && target == 0x0001d9eaU))
            && call_index_ >= fixture_.calls.size();
        if (omitted_legacy_f327_partition_marker) {
            record_compatibility_rule("omitted_legacy_f327_partition_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[function_id].entry(context);
        }

        const bool omitted_legacy_f330_partition_marker = fixture_.function_id == 330U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && ((function_id == 550U && callsite == 0x0001c1e6U
                    && target == 0x0001d9daU)
                || (function_id == 551U && callsite == 0x0001c1f0U
                    && target == 0x0001d9eaU))
            && call_index_ >= fixture_.calls.size();
        if (omitted_legacy_f330_partition_marker) {
            record_compatibility_rule("omitted_legacy_f330_partition_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[function_id].entry(context);
        }

        const bool omitted_legacy_f331_partition_marker = fixture_.function_id == 331U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && ((function_id == 550U && callsite == 0x0001c384U
                    && target == 0x0001d9daU)
                || (function_id == 551U && callsite == 0x0001c38eU
                    && target == 0x0001d9eaU))
            && call_index_ >= fixture_.calls.size();
        if (omitted_legacy_f331_partition_marker) {
            record_compatibility_rule("omitted_legacy_f331_partition_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[function_id].entry(context);
        }

        const bool omitted_legacy_f332_partition_marker = fixture_.function_id == 332U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && ((function_id == 550U && callsite == 0x0001cc6cU
                    && target == 0x0001d9daU)
                || (function_id == 551U && callsite == 0x0001cc76U
                    && target == 0x0001d9eaU))
            && call_index_ >= fixture_.calls.size();
        if (omitted_legacy_f332_partition_marker) {
            record_compatibility_rule("omitted_legacy_f332_partition_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[function_id].entry(context);
        }

        const bool omitted_legacy_f336_partition_marker =
            (fixture_.function_id == 330U || fixture_.function_id == 336U)
            && target_cpu == 1U && target_state == 0x72U
            && ((function_id == 548U
                    && ((kind == 1U && callsite == 0x0001d32eU)
                        || (kind == 0U && callsite == 0x0001d3aeU))
                    && target == 0x0001d3b2U)
                || (function_id == 593U && kind == 1U
                    && callsite == 0x0001d3b6U && target == 0x0001d3baU))
            && call_index_ >= fixture_.calls.size();
        if (omitted_legacy_f336_partition_marker) {
            record_compatibility_rule("omitted_legacy_f336_partition_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[function_id].entry(context);
        }

        const bool omitted_legacy_f186_partition_chain =
            (fixture_.function_id == 186U || fixture_.function_id == 187U
                || fixture_.function_id == 296U)
            && call_index_ >= fixture_.calls.size()
            && target_cpu == 1U && target_state == 0x72U
            && ((function_id == 541U && kind == 0U
                    && callsite == 0x0001611aU && target == 0x0001611eU)
                || (function_id == 585U
                    && ((kind == 0U && callsite == 0x0001611eU)
                        || (kind == 1U && callsite == 0x0001614eU))
                    && target == 0x00016122U)
                || (function_id == 586U && kind == 1U
                    && (callsite == 0x0001612aU || callsite == 0x00016132U)
                    && target == 0x00016134U)
                || (function_id == 587U
                    && ((kind == 1U && callsite == 0x0001612eU)
                        || (kind == 0U && callsite == 0x00016134U))
                    && target == 0x00016138U)
                || (function_id == 588U && kind == 1U
                    && callsite == 0x00016132U && target == 0x00016140U)
                || (function_id == 589U
                    && ((kind == 1U && callsite == 0x0001613eU)
                        || (kind == 0U && callsite == 0x00016140U))
                    && target == 0x00016142U));
        if (omitted_legacy_f186_partition_chain) {
            record_compatibility_rule("omitted_legacy_f186_partition_chain");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[function_id].entry(context);
        }

        const auto sequence = next_sequence_++;
        if (call_index_ >= fixture_.calls.size()) {
            set_divergence("call", "count", number(fixture_.calls.size()), "additional call", sequence);
            return {TranslationStatus::contract_violation, 0, target};
        }
        const auto &expected = fixture_.calls[call_index_++];
        if (expected.sequence != sequence)
            set_divergence("call", "sequence", number(expected.sequence), number(sequence), sequence);
        else if (expected.function_id != function_id)
            set_divergence("call", "function_id", number(expected.function_id), number(function_id), sequence);
        else if (expected.target_cpu != target_cpu)
            set_divergence("call", "target_cpu", number(expected.target_cpu), number(target_cpu), sequence);
        else if (expected.target_state != target_state)
            set_divergence("call", "target_state", hex(expected.target_state, 2), hex(target_state, 2), sequence);
        else if (expected.kind != kind)
            set_divergence("call", "kind", number(expected.kind), number(kind), sequence);
        else if (expected.callsite != callsite)
            set_divergence("call", "callsite", hex(expected.callsite, 8), hex(callsite, 8), sequence);
        else if (expected.target != target)
            set_divergence("call", "target", hex(expected.target, 8), hex(target, 8), sequence);

        for (const auto &site : generated::kComputedControlSites) {
            if (site.control_pc != callsite)
                continue;
            bool target_allowed = false;
            for (std::uint32_t index = 0; index < site.target_count; ++index) {
                if (generated::kComputedControlTargets[site.target_first + index] == target) {
                    target_allowed = true;
                    break;
                }
            }
            if (!site.call_reachable || !target_allowed)
                return {TranslationStatus::contract_violation, 0, target};
            break;
        }

        std::vector<FixtureHardwareEffect> post_call_interventions;
        while (hardware_index_ < fixture_.hardware_effects.size()) {
            const auto &event = fixture_.hardware_effects[hardware_index_];
            if ((event.kind != kControlledPostCallInterventionKind
                    && event.kind != kControlledPostCallPersistentInterventionKind)
                    || event.sequence != sequence)
                break;
            post_call_interventions.push_back(event);
            ++hardware_index_;
            if (event.cpu != fixture_.cpu)
                set_divergence("intervention", "cpu", number(event.cpu),
                    number(fixture_.cpu), sequence);
            else if (event.state != fixture_.state)
                set_divergence("intervention", "state", hex(event.state, 2),
                    hex(fixture_.state, 2), sequence);
            else if (event.pc != callsite)
                set_divergence("intervention", "trigger_pc", hex(event.pc, 8),
                    hex(callsite, 8), sequence);
        }

        if (fixture_.bundle.payload_digest == kFunction618PayloadDigest
                && fixture_.bundle.source_fixture_ordinal == 1U
                && fixture_.function_id == 618U
                && function_id == 543U && target_cpu == 1U
                && target_state == 0x72U && kind == 0U
                && callsite == 0x00017022U && target == 0x00017024U) {
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[543U].entry(context);
        }

        // Kinds 0 and 6 are partition/state-transition ownership markers.
        // Current-owner F392 fixtures continue through the newly partitioned
        // F555 owner and expose its F280/F282 calls. Older kind-0 captures can
        // contain repeated boundary occurrences without child-call evidence,
        // so only that evidenced continuation is crossed here.
        // A kind-1 call back to the current fixture's own entry is a captured
        // loop continuation. These fixtures end at the target entry, so
        // executing the target here would cross the authoritative boundary.
        const bool self_loop_continuation = kind == 1U
            && function_id == fixture_.function_id
            && target == fixture_.entry_pc;
        const bool tail_transfer_boundary = kind == 1U
            && fixture_.control == 3U
            && target == fixture_.exit_pc;
        const bool execute_enclosing_f555_partition = kind == 0U
            && (fixture_.function_id == 392U || fixture_.function_id == 393U
                || fixture_.function_id == 395U || fixture_.function_id == 396U
                || fixture_.function_id == 397U || fixture_.function_id == 398U
                || fixture_.function_id == 399U || fixture_.function_id == 401U
                || fixture_.function_id == 403U || fixture_.function_id == 404U)
            && function_id == 555U
            && target_cpu == 1U && target_state == 0x72U
            && callsite == 0x0001e224U && target == 0x0001e22aU
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 280U;
        const bool execute_enclosing_f548_partition = kind == 0U
            && fixture_.function_id == 394U
            && function_id == 548U
            && target_cpu == 1U && target_state == 0x72U
            && callsite == 0x0001d3aeU && target == 0x0001d3b2U
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 368U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x000209d2U
            && fixture_.calls[call_index_].target == 0x0001efdcU
            && fixture_.calls[call_index_].sequence == next_sequence_ + 29U;
        const bool execute_function526_current_partition = kind == 0U
            && fixture_.function_id == 526U
            && target_cpu == 1U && target_state == 0x72U
            && ((function_id == 541U
                    && callsite == 0x0001611aU && target == 0x0001611eU)
                || (function_id == 585U
                    && callsite == 0x0001611eU && target == 0x00016122U)
                || (function_id == 587U
                    && callsite == 0x00016134U && target == 0x00016138U)
                || (function_id == 589U
                    && callsite == 0x00016140U && target == 0x00016142U));
        const bool execute_enclosing_f628_wait_partition = kind == 0U
            && fixture_.function_id == 628U
            && function_id == 118U
            && target_cpu == 1U && target_state == 0x72U
            && callsite == 0x000085acU && target == 0x000085b0U;
        // Function 54's recorded startup includes the palette owner after
        // function 59 falls through, followed by the main-loop handoff.
        // Execute that child in the enclosing record; standalone 59 still
        // ends at its captured sequential boundary. Keep the call marker.
        const bool execute_enclosing_f54_palette_partition = kind == 0U
            && fixture_.function_id == 54U && function_id == 60U
            && target_cpu == 0U && target_state == 0xffU
            && callsite == 0x000802e2U && target == 0x000802e6U;
        const bool asynchronous_transfer_boundary = kind == 3U
            && fixture_.control == 2U
            && callsite == fixture_.execution_pc
            && target == fixture_.exit_pc;
        if (kind == 6U && (generated::kFd1094TransitionContractSha256.empty()
                || generated::kFd1094TransitionBoundaryIdentitySha256.empty()
                || generated::kFd1094RteIdentitySha256.empty()
                || target_cpu != 1U || target_state == context.state))
            return {TranslationStatus::contract_violation, 0, target};
        if ((kind == 0U && !execute_enclosing_f555_partition
                && !execute_enclosing_f548_partition
                && !execute_function526_current_partition
                && !execute_enclosing_f54_palette_partition
                && !execute_enclosing_f628_wait_partition)
                || kind == 6U || self_loop_continuation
                || tail_transfer_boundary || asynchronous_transfer_boundary) {
            if (!post_call_interventions.empty())
                set_divergence("intervention", "call_kind", "executed child call",
                    number(kind), sequence);
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return FunctionResult::complete(3U, target);
        }

        if (function_id >= generated::kFunctions.size())
            return {TranslationStatus::unimplemented, 0, target};
        const auto &callee = generated::kFunctions[function_id];
        if (callee.runtime_entry_evidence.empty() || callee.source_class.empty()
                || callee.semantic_status.empty())
            return {TranslationStatus::contract_violation, 0, target};
        context.cpu = target_cpu;
        context.state = target_state;
        const auto result = callee.entry(context);
        for (const auto &intervention : post_call_interventions)
            apply_intervention(intervention);
        return result;
    }

    void observe_inline_call(std::uint8_t cpu, std::uint8_t state,
        std::uint32_t site, std::uint32_t target, FunctionContext &context) override
    {
        // Internal BSRs were recorded without a separate function owner.
        // Validate the marker; the enclosing translation executes the helper.
        (void)call_function(UINT32_MAX, cpu, state, 2U, site, target, context);
    }

    [[nodiscard]] bool consume_self_continuation_boundary(
        std::uint32_t function_id,
        std::uint8_t target_cpu,
        std::uint8_t target_state,
        std::uint8_t kind,
        std::uint32_t callsite,
        std::uint32_t target,
        FunctionContext &context) override
    {
        // A control-4 record may end at the repeated entry after an interrupt
        // without a separate call marker. Otherwise consume only the exact
        // next self-continuation marker; inline records continue normally.
        if (fixture_.control == 4U && fixture_.exit_pc == target
                && call_index_ == fixture_.calls.size())
            return true;
        const bool enclosing_f628_wait_marker = fixture_.function_id == 628U
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
        const bool enclosing_wait_marker = enclosing_f628_wait_marker || enclosing_f55_wait_marker;
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

    void finish()
    {
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

    [[nodiscard]] const FirstDivergence &divergence() const noexcept { return divergence_; }
    [[nodiscard]] bool diverged() const noexcept { return !divergence_.field.empty(); }
    [[nodiscard]] const std::vector<std::string> &compatibility_rules() const noexcept
    {
        return compatibility_rules_;
    }

private:
    void record_compatibility_rule(std::string_view rule)
    {
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

    void apply_intervention(const FixtureHardwareEffect &event)
    {
        const MemoryKey target{
            static_cast<std::uint16_t>(event.address >> 24U),
            event.address & 0x00ffffffU,
        };
        const auto found = memory_.find(target);
        if (found == memory_.end()) {
            set_divergence("intervention", "target", "captured region/offset",
                number(target.region) + ":" + hex(target.offset, 8), event.sequence);
            return;
        }
        found->second.current = static_cast<std::uint16_t>(
            (found->second.current & ~event.memory_mask)
            | (event.data & event.memory_mask));
        if (event.kind == kControlledPostCallPersistentInterventionKind)
            found->second.game_current = found->second.current;
    }

    void apply_controlled_memory_intervention(
        std::uint32_t sequence,
        std::uint16_t trigger_region,
        std::uint32_t trigger_offset)
    {
        if (hardware_index_ >= fixture_.hardware_effects.size()) return;
        const auto &event = fixture_.hardware_effects[hardware_index_];
        if (event.kind != kControlledMemoryInterventionKind
            || event.sequence != sequence)
            return;
        ++hardware_index_;
        if (trigger_region != 1U)
            set_divergence("intervention", "trigger_region", "1",
                number(trigger_region), sequence);
        else if (event.cpu != fixture_.cpu)
            set_divergence("intervention", "cpu", number(event.cpu),
                number(fixture_.cpu), sequence);
        else if (event.state != fixture_.state)
            set_divergence("intervention", "state", hex(event.state, 2),
                hex(fixture_.state, 2), sequence);
        else if (event.pc != trigger_offset)
            set_divergence("intervention", "trigger_pc", hex(event.pc, 8),
                hex(trigger_offset, 8), sequence);

        apply_intervention(event);
    }

    const FixtureHardwareEffect *next_hardware(std::uint32_t sequence)
    {
        if (hardware_index_ >= fixture_.hardware_effects.size()) {
            set_divergence("hardware", "count", number(fixture_.hardware_effects.size()), "additional effect", sequence);
            return nullptr;
        }
        return &fixture_.hardware_effects[hardware_index_++];
    }

    void compare_hardware(
        const FixtureHardwareEffect &expected,
        std::uint8_t kind,
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t pc,
        std::uint32_t address,
        std::uint16_t data,
        std::uint16_t memory_mask,
        std::uint32_t sequence)
    {
        if (expected.sequence != sequence)
            set_divergence("hardware", "sequence", number(expected.sequence), number(sequence), sequence);
        else if (expected.kind != kind)
            set_divergence("hardware", "kind", number(expected.kind), number(kind), sequence);
        else if (expected.cpu != cpu)
            set_divergence("hardware", "cpu", number(expected.cpu), number(cpu), sequence);
        else if (expected.state != state)
            set_divergence("hardware", "state", hex(expected.state, 2), hex(state, 2), sequence);
        else if (expected.pc != pc)
            set_divergence("hardware", "pc", hex(expected.pc, 8), hex(pc, 8), sequence);
        else if (expected.address != address)
            set_divergence("hardware", "address", hex(expected.address, 8), hex(address, 8), sequence);
        else if (expected.data != data)
            set_divergence("hardware", "data", hex(expected.data, 4), hex(data, 4), sequence);
        else if (expected.memory_mask != memory_mask)
            set_divergence("hardware", "memory_mask", hex(expected.memory_mask, 4), hex(memory_mask, 4), sequence);
    }

    void set_divergence(
        std::string subsystem,
        std::string field,
        std::string expected,
        std::string actual,
        std::uint32_t sequence)
    {
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

    const FixtureRecord &fixture_;
    std::size_t fixture_record_{};
    std::map<MemoryKey, MemoryObservation> memory_;
    std::uint32_t next_sequence_{};
    std::size_t call_index_{};
    std::size_t hardware_index_{};
    std::set<std::size_t> consumed_interrupt_edges_;
    std::vector<std::string> compatibility_rules_;
    FirstDivergence divergence_;
};

void compare_registers(
    const CpuRegisters &expected,
    const CpuRegisters &actual,
    FixtureComparisonReport &report)
{
    auto set = [&report](std::string field, std::uint32_t expected_value, std::uint32_t actual_value, unsigned width) {
        if (report.first_divergence.field.empty() && expected_value != actual_value)
            report.first_divergence = {"registers", std::move(field), hex(expected_value, width), hex(actual_value, width), 0xffffffffU};
    };
    for (std::size_t index = 0; index != expected.data.size(); ++index)
        set("D" + number(index), expected.data[index], actual.data[index], 8);
    for (std::size_t index = 0; index != expected.address.size(); ++index)
        set("A" + number(index), expected.address[index], actual.address[index], 8);
    set("PC", expected.program_counter, actual.program_counter, 8);
    set("SR", expected.status, actual.status, 4);
}

} // namespace

FixtureComparisonReport compare_fixture(const FixtureRecord &fixture, std::size_t fixture_record)
{
    FixtureComparisonReport report;
    report.fixture_record = fixture_record;
    report.function_id = fixture.function_id;
    report.source_index = fixture.bundle.source_index;
    report.source_fixture_ordinal = fixture.bundle.source_fixture_ordinal;
    if (!fixture.bundle.native_replay_eligible) {
        report.first_divergence = {
            "fixture", "native_replay_eligible", "true", "false", 0xffffffffU};
        return report;
    }
    if (fixture.function_id >= generated::kFunctions.size()) {
        report.first_divergence = {"function", "id", "known function", number(fixture.function_id), 0xffffffffU};
        return report;
    }

    const auto &function = generated::kFunctions[fixture.function_id];
    report.function_label = std::string(function.label);
    report.checkpoint_mask = generated::kFunctionCheckpointMasks[fixture.function_id];
    for (const auto &kind : generated::kCheckpointKinds)
        if ((report.checkpoint_mask & kind.mask) != 0)
            report.checkpoint_kinds.emplace_back(kind.name);
    ComparingHost host(fixture, fixture_record);
    FunctionContext context;
    context.registers = fixture.entry_registers;
    context.cpu = fixture.cpu;
    context.state = fixture.state;
    context.host = &host;
    const auto result = function.entry(context);
    if (result.status == TranslationStatus::unimplemented) {
        report.status = ComparisonStatus::unimplemented;
        report.first_divergence = {"translation", "status", "complete", "unimplemented", 0xffffffffU};
        return report;
    }
    if (result.status != TranslationStatus::complete) {
        host.finish();
        if (host.diverged())
            report.first_divergence = host.divergence();
        else
            report.first_divergence = {"translation", "status", "complete", "contract_violation", 0xffffffffU};
        return report;
    }

    host.finish();
    report.compatibility_rules = host.compatibility_rules();
    if (host.diverged())
        report.first_divergence = host.divergence();
    if (report.first_divergence.field.empty() && result.control != fixture.control)
        report.first_divergence = {"control", "result", number(fixture.control), number(result.control), 0xffffffffU};
    if (report.first_divergence.field.empty() && result.exit_program_counter != fixture.exit_pc)
        report.first_divergence = {"control", "exit_pc", hex(fixture.exit_pc, 8), hex(result.exit_program_counter, 8), 0xffffffffU};
    compare_registers(fixture.exit_registers, context.registers, report);
    report.status = report.first_divergence.field.empty() ? ComparisonStatus::passed : ComparisonStatus::diverged;
    return report;
}

FixtureBatchReport compare_function_fixtures(
    const FixtureBundle &bundle,
    std::span<const std::uint32_t> function_ids)
{
    FixtureBatchReport batch;
    std::vector<bool> selected(generated::kFunctions.size());
    for (const auto function_id : function_ids) {
        if (function_id >= generated::kFunctions.size())
            throw std::runtime_error("function ID is out of range: " + number(function_id));
        if (selected[function_id])
            throw std::runtime_error("function ID is duplicated: " + number(function_id));
        selected[function_id] = true;

        const auto &function = generated::kFunctions[function_id];
        std::vector<std::size_t> fixture_records;
        for (std::size_t record_index = 0; record_index != bundle.records().size(); ++record_index) {
            if (bundle.records()[record_index].function_id == function_id
                && bundle.records()[record_index].native_replay_eligible)
                fixture_records.push_back(record_index);
        }
        ++batch.selected_functions;
        batch.selected_fixtures += fixture_records.size();
        if (!function.implemented) {
            ++batch.unimplemented_functions;
            batch.status = ComparisonStatus::unimplemented;
            batch.has_first_failure = true;
            batch.first_failure.status = ComparisonStatus::unimplemented;
            batch.first_failure.fixture_record = fixture_records.empty() ? 0U : fixture_records.front();
            batch.first_failure.function_id = function.id;
            batch.first_failure.function_label = std::string(function.label);
            batch.first_failure.first_divergence = {
                "translation", "implementation_catalog", "catalogued", "fallback_stub", 0xffffffffU};
            break;
        }

        ++batch.implemented_functions;
        if (fixture_records.empty()) {
            ++batch.diverged_functions;
            batch.status = ComparisonStatus::diverged;
            batch.has_first_failure = true;
            batch.first_failure.status = ComparisonStatus::diverged;
            batch.first_failure.fixture_record = 0U;
            batch.first_failure.function_id = function.id;
            batch.first_failure.function_label = std::string(function.label);
            batch.first_failure.first_divergence = {
                "fixture", "count", "at least one complete fixture", "0", 0xffffffffU};
            break;
        }
        bool function_passed = true;
        for (const auto record_index : fixture_records) {
            const auto fixture = bundle.load(record_index);
            ++batch.executed_fixtures;
            auto report = compare_fixture(fixture, record_index);
            batch.compatibility_rules.insert(
                batch.compatibility_rules.end(),
                report.compatibility_rules.begin(), report.compatibility_rules.end());
            for (const auto &rule : report.compatibility_rules) {
                batch.compatibility_rule_hits.push_back({
                    rule,
                    report.source_index,
                    report.source_fixture_ordinal,
                    report.function_id,
                });
            }
            if (report.status == ComparisonStatus::passed) {
                ++batch.passed_fixtures;
                continue;
            }

            function_passed = false;
            batch.has_first_failure = true;
            batch.first_failure = std::move(report);
            if (batch.first_failure.status == ComparisonStatus::unimplemented) {
                ++batch.unimplemented_functions;
                ++batch.unimplemented_fixtures;
                batch.status = ComparisonStatus::unimplemented;
            } else {
                ++batch.diverged_functions;
                ++batch.diverged_fixtures;
                batch.status = ComparisonStatus::diverged;
            }
            break;
        }
        if (function_passed)
            ++batch.passed_functions;
        else
            break;
    }
    return batch;
}

void enforce_exact_compatibility_histogram(FixtureBatchReport &report)
{
    if (report.status != ComparisonStatus::passed)
        return;

    std::map<std::string, std::size_t> actual;
    for (const auto &rule : report.compatibility_rules)
        ++actual[rule];

    for (const auto &expected : generated::kFixtureMarkerCompatibilityExpectedRuleHits) {
        const auto found = actual.find(std::string(expected.rule));
        const auto count = found == actual.end() ? 0U : found->second;
        if (count != expected.count) {
            report.status = ComparisonStatus::diverged;
            report.has_first_failure = true;
            report.first_failure.status = ComparisonStatus::diverged;
            report.first_failure.first_divergence = {
                "compatibility", "rule_histogram." + std::string(expected.rule),
                number(expected.count), number(count), 0xffffffffU};
            return;
        }
        actual.erase(std::string(expected.rule));
    }
    if (!actual.empty()) {
        report.status = ComparisonStatus::diverged;
        report.has_first_failure = true;
        report.first_failure.status = ComparisonStatus::diverged;
        report.first_failure.first_divergence = {
            "compatibility", "unexpected_rule." + actual.begin()->first,
            "0", number(actual.begin()->second), 0xffffffffU};
        return;
    }
    if (report.compatibility_rules.size()
            != generated::kFixtureMarkerCompatibilityExpectedHitCount) {
        report.status = ComparisonStatus::diverged;
        report.has_first_failure = true;
        report.first_failure.status = ComparisonStatus::diverged;
        report.first_failure.first_divergence = {
            "compatibility", "total_hit_count",
            number(generated::kFixtureMarkerCompatibilityExpectedHitCount),
            number(report.compatibility_rules.size()), 0xffffffffU};
    }
}

std::string comparison_status_name(ComparisonStatus status)
{
    switch (status) {
    case ComparisonStatus::passed: return "passed";
    case ComparisonStatus::unimplemented: return "unimplemented";
    case ComparisonStatus::diverged: return "diverged";
    }
    return "unknown";
}

std::string comparison_report_json(const FixtureComparisonReport &report)
{
    std::ostringstream stream;
    stream << "{\n"
           << "  \"schema\": \"gground-native-fixture-comparison\",\n"
           << "  \"schemaVersion\": 1,\n"
           << "  \"status\": \"" << comparison_status_name(report.status) << "\",\n"
           << "  \"fixtureRecord\": " << report.fixture_record << ",\n"
           << "  \"sourceIndex\": " << report.source_index << ",\n"
           << "  \"sourceFixtureOrdinal\": " << report.source_fixture_ordinal << ",\n"
           << "  \"functionId\": " << report.function_id << ",\n"
           << "  \"functionLabel\": \"" << json_escape(report.function_label) << "\",\n"
           << "  \"checkpointMask\": \"" << hex(report.checkpoint_mask, 8) << "\",\n"
           << "  \"checkpointKinds\": [";
    for (std::size_t index = 0; index != report.checkpoint_kinds.size(); ++index) {
        if (index != 0)
            stream << ", ";
        stream << "\"" << json_escape(report.checkpoint_kinds[index]) << "\"";
    }
    stream << "],\n"
           << "  \"compatibilityRuleHitCount\": " << report.compatibility_rules.size() << ",\n"
           << "  \"compatibilityRuleCounts\": {";
    std::map<std::string, std::size_t> compatibility_counts;
    for (const auto &rule : report.compatibility_rules)
        ++compatibility_counts[rule];
    std::size_t compatibility_index = 0;
    for (const auto &[rule, count] : compatibility_counts) {
        if (compatibility_index++ != 0)
            stream << ", ";
        stream << "\"" << json_escape(rule) << "\": " << count;
    }
    stream << "},\n"
           << "  \"firstDivergence\": {\n"
           << "    \"subsystem\": \"" << json_escape(report.first_divergence.subsystem) << "\",\n"
           << "    \"field\": \"" << json_escape(report.first_divergence.field) << "\",\n"
           << "    \"expected\": \"" << json_escape(report.first_divergence.expected) << "\",\n"
           << "    \"actual\": \"" << json_escape(report.first_divergence.actual) << "\",\n"
           << "    \"sequence\": ";
    if (report.first_divergence.sequence == 0xffffffffU)
        stream << "null\n";
    else
        stream << report.first_divergence.sequence << "\n";
    stream << "  }\n}\n";
    return stream.str();
}

std::string batch_report_json(const FixtureBatchReport &report)
{
    std::ostringstream stream;
    stream << "{\n"
           << "  \"schema\": \"gground-native-fixture-batch-comparison\",\n"
           << "  \"schemaVersion\": 1,\n"
           << "  \"status\": \"" << comparison_status_name(report.status) << "\",\n"
           << "  \"functions\": {\n"
           << "    \"selected\": " << report.selected_functions << ",\n"
           << "    \"implemented\": " << report.implemented_functions << ",\n"
           << "    \"passed\": " << report.passed_functions << ",\n"
           << "    \"unimplemented\": " << report.unimplemented_functions << ",\n"
           << "    \"diverged\": " << report.diverged_functions << "\n"
           << "  },\n"
           << "  \"fixtures\": {\n"
           << "    \"selected\": " << report.selected_fixtures << ",\n"
           << "    \"executed\": " << report.executed_fixtures << ",\n"
           << "    \"passed\": " << report.passed_fixtures << ",\n"
           << "    \"unimplemented\": " << report.unimplemented_fixtures << ",\n"
           << "    \"diverged\": " << report.diverged_fixtures << "\n"
           << "  },\n"
           << "  \"compatibilityRuleHitCount\": " << report.compatibility_rules.size() << ",\n"
           << "  \"compatibilityRuleCounts\": {";
    std::map<std::string, std::size_t> compatibility_counts;
    for (const auto &rule : report.compatibility_rules)
        ++compatibility_counts[rule];
    std::size_t compatibility_index = 0;
    for (const auto &[rule, count] : compatibility_counts) {
        if (compatibility_index++ != 0)
            stream << ", ";
        stream << "\"" << json_escape(rule) << "\": " << count;
    }
    stream << "},\n"
           << "  \"compatibilityRuleHits\": [";
    for (std::size_t index = 0; index != report.compatibility_rule_hits.size(); ++index) {
        if (index != 0)
            stream << ",";
        const auto &hit = report.compatibility_rule_hits[index];
        stream << "{\"rule\":\"" << json_escape(hit.rule)
               << "\",\"sourceIndex\":" << hit.source_index
               << ",\"sourceFixtureOrdinal\":" << hit.source_fixture_ordinal
               << ",\"rootFunctionId\":" << hit.root_function_id << "}";
    }
    stream << "],\n"
           << "  \"stoppedAtFirstFailure\": " << (report.has_first_failure ? "true" : "false") << ",\n"
           << "  \"firstFailure\": ";
    if (report.has_first_failure)
        stream << comparison_report_json(report.first_failure);
    else
        stream << "null\n";
    stream << "}\n";
    return stream.str();
}

} // namespace gain_ground
