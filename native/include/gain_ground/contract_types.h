#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace gain_ground {

class ExecutionHost;
struct TimingCpuPosition;
struct TimingTraceEvent;

struct CpuRegisters {
    std::array<std::uint32_t, 8> data{};
    std::array<std::uint32_t, 8> address{};
    std::uint32_t program_counter{};
    std::uint16_t status{};
};

struct FunctionContext {
    CpuRegisters registers{};
    std::uint8_t cpu{};
    std::uint8_t state{0xffU};
    ExecutionHost *host{};
};

enum class TranslationStatus : std::uint8_t {
    unimplemented = 0,
    complete = 1,
    contract_violation = 2,
};

struct FunctionResult {
    TranslationStatus status{TranslationStatus::unimplemented};
    std::uint8_t control{};
    std::uint32_t exit_program_counter{0xffffffffU};

    [[nodiscard]] static constexpr FunctionResult unimplemented() noexcept
    {
        return {};
    }

    [[nodiscard]] static constexpr FunctionResult complete(
        std::uint8_t control_result,
        std::uint32_t exit_pc) noexcept
    {
        return {TranslationStatus::complete, control_result, exit_pc};
    }
};

struct PendingInterrupt {
    bool asserted{};
    std::uint8_t level{};
};

class ExecutionHost {
public:
    virtual ~ExecutionHost() = default;
    // Standalone runtime follows the board's unmapped bus behavior. Fixture
    // hosts retain their strict captured-region boundary.
    virtual bool allows_unmapped_program_access() const noexcept { return false; }
    // A live host can finish an ISR before returning to its native caller.
    // Fixture hosts retain interrupt boundaries as observable results.
    virtual bool resumes_interrupts_inline() const noexcept { return false; }
    // Approved direct-loading ABI skips hardware-speed probes in native boot.
    virtual bool replaces_boot_calibration() const noexcept { return false; }
    // Optional live gameplay migration. Other hosts keep the original owners.
    virtual bool run_character_update(FunctionContext &, FunctionResult &) { return false; }
    virtual bool run_character_attacks(FunctionContext &, FunctionResult &) { return false; }
    virtual bool run_character_movement(FunctionContext &, FunctionResult &) { return false; }
    virtual bool run_character_attack_phase(FunctionContext &, FunctionResult &) { return false; }
    virtual bool run_character_collision(FunctionContext &, FunctionResult &) { return false; }
    virtual bool run_character_exit(FunctionContext &, FunctionResult &) { return false; }
    virtual bool run_projectile_update(FunctionContext &, FunctionResult &) { return false; }
    virtual bool run_character_damage(FunctionContext &, FunctionResult &) { return false; }
    // Optional presentation inputs for the enemy walking sprite path only.
    virtual std::uint16_t enemy_walking_heading(std::uint32_t, std::uint16_t original) const { return original; }
    virtual std::uint16_t enemy_walking_advance(std::uint32_t, std::uint16_t original) const { return original; }
    // Live attack contexts may select the authored mode without changing movement RAM.
    virtual std::uint8_t enemy_direction_mode(std::uint32_t, std::uint8_t original) const { return original; }
    // Definition word offset is +6 (movement), +8 (primary), or +A (secondary).
    virtual std::uint16_t character_profile(std::uint32_t, unsigned,
                                           std::uint16_t original) const noexcept {
        return original;
    }
    // Live scheduling for original CPU delay loops. Fixture hosts compare
    // architectural effects and retain their existing untimed execution.
    virtual std::uint64_t execution_time_ns() const noexcept { return 0U; }
    virtual void wait_until_time(std::uint64_t) {}
    virtual void begin_timed_execution() noexcept {}
    virtual void end_timed_execution() noexcept {}
    virtual TimingCpuPosition *instruction_timing_position(std::uint8_t) noexcept { return nullptr; }
    virtual void observe_instruction_timing(const TimingTraceEvent &) {}
    virtual bool read_timing_program_word(std::uint8_t cpu, std::uint8_t state,
                                         std::uint32_t address, std::uint16_t &value) {
        (void)state;
        if (cpu != 0U) return false; // FD1094 opcodes are not AS_PROGRAM data RAM.
        value = read_memory_word(address < 0x80000U ? 1U : 3U, address & 0x3ffffU, 0xffffU);
        return true;
    }

    [[nodiscard]] virtual std::uint16_t read_memory_word(
        std::uint16_t region,
        std::uint32_t byte_offset,
        std::uint16_t memory_mask) = 0;
    virtual void write_memory_word(
        std::uint16_t region,
        std::uint32_t byte_offset,
        std::uint16_t data,
        std::uint16_t memory_mask) = 0;
    [[nodiscard]] virtual std::uint16_t read_hardware(
        std::uint8_t kind,
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t pc,
        std::uint32_t address,
        std::uint16_t memory_mask) = 0;
    virtual void write_hardware(
        std::uint8_t kind,
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t pc,
        std::uint32_t address,
        std::uint16_t data,
        std::uint16_t memory_mask) = 0;
    [[nodiscard]] virtual std::uint16_t apply_controlled_status(
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t pc,
        std::uint32_t trigger_address,
        std::uint16_t status)
    {
        (void)cpu;
        (void)state;
        (void)pc;
        (void)trigger_address;
        return status;
    }
    [[nodiscard]] virtual PendingInterrupt consume_pending_interrupt(
        std::uint8_t cpu,
        std::uint8_t state,
        std::uint32_t completed_instruction_pc) = 0;
    [[nodiscard]] virtual FunctionResult call_function(
        std::uint32_t function_id,
        std::uint8_t target_cpu,
        std::uint8_t target_state,
        std::uint8_t kind,
        std::uint32_t callsite,
        std::uint32_t target,
        FunctionContext &context) = 0;
    // The caller executes an inline helper. Fixture hosts may consume its
    // recorded BSR marker; production must not dispatch the helper twice.
    virtual void observe_inline_call(std::uint8_t, std::uint8_t,
        std::uint32_t, std::uint32_t, FunctionContext &) {}
    // Production hosts use the default and execute the real backedge. Fixture
    // hosts may stop at an authoritative capture-slice boundary without
    // changing guest-visible CPU or memory state.
    [[nodiscard]] virtual bool consume_self_continuation_boundary(
        std::uint32_t function_id,
        std::uint8_t target_cpu,
        std::uint8_t target_state,
        std::uint8_t kind,
        std::uint32_t callsite,
        std::uint32_t target,
        FunctionContext &context)
    {
        (void)function_id;
        (void)target_cpu;
        (void)target_state;
        (void)kind;
        (void)callsite;
        (void)target;
        (void)context;
        return false;
    }
};

using FunctionEntry = FunctionResult (*)(FunctionContext &) noexcept;

struct ComputedControlSiteContract {
    std::uint32_t owner_function_id;
    std::uint32_t control_pc;
    std::uint32_t target_first;
    std::uint32_t target_count;
    bool call_reachable;
};

struct FunctionContract {
    std::uint32_t id;
    std::uint8_t cpu;
    std::uint8_t state;
    bool runtime_seed;
    bool implemented;
    std::uint32_t address;
    std::uint32_t body_min;
    std::uint32_t body_max;
    std::uint32_t body_bytes;
    std::uint32_t first_fixture;
    std::uint32_t fixture_count;
    std::string_view owner;
    std::string_view state_name;
    std::string_view confidence;
    std::string_view runtime_entry_evidence;
    std::string_view source_class;
    std::string_view semantic_status;
    std::string_view label;
    std::string_view implementation_source;
    FunctionEntry entry;
};

struct BackingStoreContract {
    std::string_view id;
    std::uint32_t logical_bytes;
    std::uint32_t allocation_bytes;
    std::uint32_t physical_entries;
    std::uint32_t active_tile_count;
    std::uint32_t bytes_per_tile;
    std::uint8_t cpu_mask;
    std::string_view kind;
    std::string_view role;
    std::string_view note;
    std::string_view evidence;
};

struct MemoryWindowContract {
    std::string_view id;
    std::string_view source_function;
    std::string_view address_space;
    std::uint8_t cpu_mask;
    std::uint32_t start;
    std::uint32_t end;
    std::uint32_t mirror;
    bool has_mirror;
    std::string_view binding;
    std::string_view access;
    std::string_view owner;
    std::string_view backing_store;
    std::string_view register_name;
};

struct RegisterContract {
    std::uint32_t address;
    std::uint16_t width_bits;
    std::string_view name;
    std::string_view owner;
    std::string_view lane;
    std::string_view evidence;
};

struct FixtureRegionContract {
    std::uint16_t id;
    std::uint32_t bytes;
    bool immutable;
    std::string_view name;
};

} // namespace gain_ground
