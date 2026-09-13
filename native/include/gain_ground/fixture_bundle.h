#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "gain_ground/contract_types.h"

namespace gain_ground {

struct FixtureBundleHeader {
    std::uint32_t version{};
    std::uint32_t header_bytes{};
    std::uint32_t record_header_bytes{};
    std::uint32_t compression{};
    std::uint64_t function_count{};
    std::uint64_t fixture_count{};
    std::array<std::uint8_t, 20> base_mame_commit{};
    std::array<std::uint8_t, 20> capture_source_commit{};
    std::array<std::array<std::uint8_t, 32>, 8> identities{};
};

struct FixtureBundleRecordIndex {
    std::uint32_t function_id{};
    std::uint32_t variant_index{};
    std::uint32_t control{};
    std::uint32_t source_index{};
    std::uint64_t source_fixture_ordinal{};
    std::uint64_t raw_bytes{};
    std::uint64_t compressed_bytes{};
    std::array<std::uint8_t, 32> path_digest{};
    std::array<std::uint8_t, 32> semantic_digest{};
    std::array<std::uint8_t, 32> payload_digest{};
    bool native_replay_eligible{};
    std::uint64_t payload_file_offset{};
};

struct FixtureEdge {
    std::uint8_t from_state{};
    std::uint8_t to_state{};
    std::uint8_t kind{};
    std::uint16_t opcode{};
    std::uint32_t source_pc{};
    std::uint32_t target_pc{};
};

struct FixtureMemoryPrestate {
    std::uint16_t region{};
    std::uint16_t flags{};
    std::uint32_t offset{};
    std::uint16_t initial{};
    std::uint16_t read_mask{};
    std::uint32_t access_count{};
    std::uint32_t first_sequence{};
};

struct FixtureMemoryDelta {
    std::uint16_t region{};
    std::uint16_t write_mask{};
    std::uint32_t offset{};
    std::uint16_t initial{};
    std::uint16_t final_value{};
    std::uint32_t write_count{};
    std::uint32_t first_sequence{};
    std::uint32_t last_sequence{};
};

struct FixtureCall {
    std::uint32_t sequence{};
    std::uint32_t function_id{};
    std::uint8_t target_cpu{};
    std::uint8_t target_state{};
    std::uint8_t kind{};
    std::uint32_t callsite{};
    std::uint32_t target{};
};

struct FixtureHardwareEffect {
    std::uint32_t sequence{};
    std::uint8_t kind{};
    std::uint8_t cpu{};
    std::uint8_t state{};
    std::uint32_t pc{};
    std::uint32_t address{};
    std::uint16_t data{};
    std::uint16_t memory_mask{};
};

struct FixtureRecord {
    FixtureBundleRecordIndex bundle;
    std::uint64_t fixture_ordinal{};
    std::uint32_t function_id{};
    std::uint8_t cpu{};
    std::uint8_t state{};
    std::uint8_t entry_kind{};
    std::uint8_t control{};
    std::uint32_t entry_pc{};
    std::uint32_t execution_pc{};
    std::uint32_t exit_pc{};
    std::uint32_t callsite{};
    std::uint32_t expected_return{};
    std::uint64_t start_instruction{};
    std::uint64_t end_instruction{};
    std::uint64_t entry_frame{};
    std::uint64_t end_frame{};
    std::uint32_t instruction_count{};
    CpuRegisters entry_registers{};
    CpuRegisters exit_registers{};
    std::vector<FixtureEdge> edges;
    std::vector<FixtureMemoryPrestate> memory_prestates;
    std::vector<FixtureMemoryDelta> memory_deltas;
    std::vector<FixtureCall> calls;
    std::vector<FixtureHardwareEffect> hardware_effects;
};

class FixtureBundle {
public:
    explicit FixtureBundle(std::filesystem::path path);

    [[nodiscard]] const std::filesystem::path &path() const noexcept { return path_; }
    [[nodiscard]] const FixtureBundleHeader &header() const noexcept { return header_; }
    [[nodiscard]] const std::vector<FixtureBundleRecordIndex> &records() const noexcept { return records_; }
    [[nodiscard]] FixtureRecord load(std::size_t record_index) const;

private:
    std::filesystem::path path_;
    FixtureBundleHeader header_;
    std::vector<FixtureBundleRecordIndex> records_;
};

[[nodiscard]] std::string hexadecimal(const std::uint8_t *bytes, std::size_t count);

} // namespace gain_ground
