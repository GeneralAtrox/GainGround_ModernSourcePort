#include "fixture_compare_internal.h"
#include <cstdlib>
#include <iostream>
#include <utility>
namespace gain_ground::fixture_compare_detail {
ComparingHost::ComparingHost(const FixtureRecord &fixture, std::size_t fixture_record)
        : fixture_(fixture), fixture_record_(fixture_record){
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

[[nodiscard]] std::uint16_t ComparingHost::read_memory_word(
        std::uint16_t region,
        std::uint32_t byte_offset,
        std::uint16_t memory_mask){
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

void ComparingHost::write_memory_word(
        std::uint16_t region,
        std::uint32_t byte_offset,
        std::uint16_t data,
        std::uint16_t memory_mask){
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

void ComparingHost::apply_intervention(const FixtureHardwareEffect &event){
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

void ComparingHost::apply_controlled_memory_intervention(
        std::uint32_t sequence,
        std::uint16_t trigger_region,
        std::uint32_t trigger_offset){
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
}
