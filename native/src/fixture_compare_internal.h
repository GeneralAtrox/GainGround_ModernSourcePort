#pragma once
#include "gain_ground/fixture_bundle.h"
#include "gain_ground/contract_types.h"
#include "gain_ground/fixture_compare.h"
#include "gground_functions.h"
#include "gground_checkpoints.h"
#include "gground_fixture_marker_compatibility.h"
#include <array>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
namespace gain_ground::fixture_compare_detail {
inline constexpr std::uint8_t kControlledMemoryInterventionKind = 0x80U;
inline constexpr std::uint8_t kControlledPostCallInterventionKind = 0x81U;
inline constexpr std::uint8_t kControlledPostCallPersistentInterventionKind = 0x82U;
inline constexpr std::uint8_t kControlledStatusInterventionKind = 0x83U;
inline constexpr std::array<std::uint8_t, 32> kFunction618PayloadDigest{
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

inline std::string number(std::uint64_t v){return std::to_string(v);}
inline std::string hex(std::uint64_t v,unsigned w){std::ostringstream s;s<<"0x"<<std::hex<<std::setfill('0')<<std::setw(static_cast<int>(w))<<v;return s.str();}
class ComparingHost final : public ExecutionHost {
public:
ComparingHost(const FixtureRecord &fixture, std::size_t fixture_record);


[[nodiscard]] std::uint16_t read_memory_word(
        std::uint16_t region,
        std::uint32_t byte_offset,
        std::uint16_t memory_mask);


void write_memory_word(
        std::uint16_t region,
        std::uint32_t byte_offset,
        std::uint16_t data,
        std::uint16_t memory_mask);


[[nodiscard]] std::uint16_t read_hardware(
        std::uint8_t kind,
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t pc,
        std::uint32_t address,
        std::uint16_t memory_mask);


void write_hardware(
        std::uint8_t kind,
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t pc,
        std::uint32_t address,
        std::uint16_t data,
        std::uint16_t memory_mask);


[[nodiscard]] std::uint16_t apply_controlled_status(
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t pc,
        std::uint32_t trigger_address,
        std::uint16_t status);


[[nodiscard]] PendingInterrupt consume_pending_interrupt(
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t completed_instruction_pc);


[[nodiscard]] FunctionResult call_function(
        std::uint32_t function_id,
        std::uint8_t target_cpu,
        std::uint8_t target_state,
        std::uint8_t kind,
        std::uint32_t callsite,
        std::uint32_t target,
        FunctionContext &context);
    [[nodiscard]] std::optional<FunctionResult> try_early_compatibility(std::uint32_t function_id, std::uint8_t target_cpu, std::uint8_t target_state, std::uint8_t kind, std::uint32_t callsite, std::uint32_t target, FunctionContext &context);
    [[nodiscard]] std::optional<FunctionResult> try_current_compatibility(std::uint32_t function_id, std::uint8_t target_cpu, std::uint8_t target_state, std::uint8_t kind, std::uint32_t callsite, std::uint32_t target, FunctionContext &context);
    [[nodiscard]] std::optional<FunctionResult> try_partition_compatibility(std::uint32_t function_id, std::uint8_t target_cpu, std::uint8_t target_state, std::uint8_t kind, std::uint32_t callsite, std::uint32_t target, FunctionContext &context);


void observe_inline_call(std::uint8_t cpu, std::uint8_t state,
        std::uint32_t site, std::uint32_t target, FunctionContext &context);


[[nodiscard]] bool consume_self_continuation_boundary(
        std::uint32_t function_id,
        std::uint8_t target_cpu,
        std::uint8_t target_state,
        std::uint8_t kind,
        std::uint32_t callsite,
        std::uint32_t target,
        FunctionContext &context);


void finish();


    [[nodiscard]] const FirstDivergence &divergence() const noexcept { return divergence_; }
[[nodiscard]] bool diverged() const noexcept;

    [[nodiscard]] const std::vector<std::string> &compatibility_rules() const noexcept
    {
        return compatibility_rules_;
    }

private:
void record_compatibility_rule(std::string_view rule);


void apply_intervention(const FixtureHardwareEffect &event);


void apply_controlled_memory_intervention(
        std::uint32_t sequence,
        std::uint16_t trigger_region,
        std::uint32_t trigger_offset);


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
        std::uint32_t sequence);


void set_divergence(
        std::string subsystem,
        std::string field,
        std::string expected,
        std::string actual,
        std::uint32_t sequence);


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
}
