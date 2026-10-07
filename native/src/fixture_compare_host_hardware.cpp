#include "fixture_compare_internal.h"
#include <cstdlib>
#include <iostream>
#include <utility>
namespace gain_ground::fixture_compare_detail {
[[nodiscard]] std::uint16_t ComparingHost::read_hardware(
        std::uint8_t kind,
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t pc,
        std::uint32_t address,
        std::uint16_t memory_mask){
        const auto sequence = next_sequence_++;
        const auto *expected = next_hardware(sequence);
        if (!expected)
            return 0;
        compare_hardware(*expected, kind, cpu, state, pc, address, expected->data, memory_mask, sequence);
        return expected->data;
    }

void ComparingHost::write_hardware(
        std::uint8_t kind,
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t pc,
        std::uint32_t address,
        std::uint16_t data,
        std::uint16_t memory_mask){
        const auto sequence = next_sequence_++;
        const auto *expected = next_hardware(sequence);
        if (expected)
            compare_hardware(*expected, kind, cpu, state, pc, address, data, memory_mask, sequence);
    }

[[nodiscard]] std::uint16_t ComparingHost::apply_controlled_status(
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t pc,
        std::uint32_t trigger_address,
        std::uint16_t status){
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

void ComparingHost::compare_hardware(
        const FixtureHardwareEffect &expected,
        std::uint8_t kind,
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t pc,
        std::uint32_t address,
        std::uint16_t data,
        std::uint16_t memory_mask,
        std::uint32_t sequence){
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
}
