#pragma once

#include "gain_ground/contract_types.h"
#include "gain_ground/timing_trace.h"
#include "gain_ground/gameplay/character_definition.h"
#include "gain_ground/gameplay/enemy_navigation.h"
#include <array>
#include <cstddef>
#include <span>
#include <string_view>
#include <vector>

namespace gain_ground {
class DirectAssetLoader;
class System24Devices;

// Implementation-first runtime. No fixture records or expected results are used.
// Device models and emulated clock advancement are provided by System24Devices.
struct RuntimeFault {
    std::string_view message;
    std::uint32_t pc{};
    std::uint32_t address{};
    std::uint16_t region{};
    std::uint16_t mask{};
    std::uint8_t cpu{};
    std::uint32_t function_id{UINT32_MAX};
    std::uint32_t function_entry{};
    std::uint8_t state{};
    std::uint8_t result_status{};
    std::uint8_t result_control{};
};

class RuntimeHost final : public ExecutionHost {
public:
    // A checkpoint may suspend the calling fiber. It must not throw across a
    // translated noexcept function. A faulted fiber must never be resumed.
    using Checkpoint = void (*)(void *);
    RuntimeHost();
    void set_checkpoint(Checkpoint callback, void *argument) noexcept;
    bool load_region(std::uint16_t region, std::uint32_t offset,
                     std::span<const std::uint8_t> bytes);
    bool can_load_bus(std::uint32_t address, std::uint32_t bytes) const noexcept;
    bool load_bus(std::uint32_t address, std::span<const std::uint8_t> bytes);
    [[nodiscard]] std::span<const std::uint8_t> region_bytes(std::uint16_t region) const noexcept { return regions_[region == 4U ? 1U : region].bytes; }
    // Approved fast-loading mode: deterministic cleared RAM, extracted BIOS
    // loads, and original CPU-A post-loader entry. Call before scheduling CPUs.
    bool prepare_direct_boot(DirectAssetLoader &assets, FunctionContext &context);
    void prepare_sega_logo();
    void attach_devices(System24Devices &devices) noexcept { devices_ = &devices; }
    void select_cpu(unsigned cpu) noexcept;
    bool waiting_for_device() const noexcept { return waiting_; }
    bool allows_unmapped_program_access() const noexcept override { return true; }
    bool resumes_interrupts_inline() const noexcept override { return true; }
    bool replaces_boot_calibration() const noexcept override { return assets_ != nullptr; }
    bool run_character_update(FunctionContext &, FunctionResult &) override;
    bool run_character_attacks(FunctionContext &, FunctionResult &) override;
    bool run_character_movement(FunctionContext &, FunctionResult &) override;
    bool run_character_attack_phase(FunctionContext &, FunctionResult &) override;
    bool run_character_collision(FunctionContext &, FunctionResult &) override;
    bool run_character_exit(FunctionContext &, FunctionResult &) override;
    bool run_projectile_update(FunctionContext &, FunctionResult &) override;
    bool run_character_damage(FunctionContext &, FunctionResult &) override;
    std::uint16_t enemy_walking_heading(std::uint32_t record, std::uint16_t original) const override;
    std::uint16_t enemy_walking_advance(std::uint32_t record, std::uint16_t original) const override;
    std::uint8_t enemy_direction_mode(std::uint32_t record, std::uint8_t original) const override;
    std::uint16_t character_profile(std::uint32_t record, unsigned definition_offset,
                                    std::uint16_t original) const noexcept override;
    bool load_character_definitions(const std::filesystem::path &directory, std::string &error) {
        return character_definitions_.load(directory, error);
    }
    // Stage select. During play the current stage is cleared at once through
    // the original all-enemies-defeated clear, which then advances to original
    // stage index 0..39 (round*10 + stage) instead of the following stage; -1
    // clears the request. Outside play the request waits for the next game.
    void set_start_stage(int stage_index) noexcept {
        start_stage_ = stage_index; start_stage_publish_ = false; start_stage_force_ = stage_index >= 0;
    }
    [[nodiscard]] int start_stage() const noexcept { return start_stage_; }
    [[nodiscard]] bool take_start_stage_applied() noexcept {
        const auto applied = start_stage_applied_; start_stage_applied_ = false; return applied;
    }
    // Test aids. Player records are learned from the character update entry;
    // teleport writes a player's 16.16 position, leaving every other field to
    // the original movement, contact and terrain code.
    void set_player_invulnerable(bool on) noexcept { invulnerable_ = on; }
    bool player_invulnerable() const noexcept override { return invulnerable_; }
    [[nodiscard]] std::uint32_t player_record(unsigned player) const noexcept {
        return player < player_records_.size() ? player_records_[player] : 0U;
    }
    bool teleport_player(unsigned player, int x, int y);
    // The two stage-select decisions, taken on CPU B's own writes and frames.
    // Public so tests can exercise them without running the stage teardown.
    std::uint16_t stage_select_write(std::uint16_t region, std::uint32_t offset, std::uint16_t value, std::uint32_t pc);
    bool stage_select_frame(std::uint32_t record);
    std::uint64_t execution_time_ns() const noexcept override;
    void wait_until_time(std::uint64_t deadline_ns) override;
    void begin_timed_execution() noexcept override { ++timed_depth_[selected_cpu_]; }
    void end_timed_execution() noexcept override { --timed_depth_[selected_cpu_]; }
    bool timed_execution() const noexcept { return timed_depth_[selected_cpu_] != 0U; }
    using TimingObserver = void (*)(void *, const TimingTraceEvent &);
    void set_timing_observer(TimingObserver callback, void *argument) noexcept {
        timing_observer_ = callback; timing_argument_ = argument;
    }
    TimingCpuPosition *instruction_timing_position(std::uint8_t cpu) noexcept override {
        return cpu < 2U ? &timing_positions_[cpu] : nullptr;
    }
    void observe_instruction_timing(const TimingTraceEvent &event) override {
        if (timing_observer_) timing_observer_(timing_argument_, event);
    }
    bool cpu_ready(unsigned cpu) const noexcept;
    std::uint64_t next_cpu_deadline_ns() const noexcept;
    [[nodiscard]] FunctionContext cpu_a_reset_context();
    [[nodiscard]] FunctionResult run(FunctionContext &context);
    [[nodiscard]] const RuntimeFault &fault() const noexcept { return fault_; }
    [[nodiscard]] bool faulted() const noexcept { return !fault_.message.empty(); }
    [[nodiscard]] std::uint64_t execution_checkpoints() const noexcept { return operations_; }
    // Operations since the scheduler last (re)entered a CPU. The scheduler
    // yields a CPU after 4096; an in-place wait step counts as a re-entry so
    // the yield points stay where a real switch would have put them.
    [[nodiscard]] std::uint64_t operations_since_switch() const noexcept { return operations_since_switch_; }
    void reset_yield_window() noexcept { operations_since_switch_ = 0U; }
    // The window scheduler steps the device clock to the next event or CPU
    // deadline and switches back; a waiting CPU may take that step in place
    // when the other CPU cannot run first. Only that scheduler enables it.
    void set_direct_wait(bool on) noexcept { direct_wait_ = on; }
    [[nodiscard]] const FunctionContext *active_context() const noexcept { return active_; }
    // IRQ lines are driven explicitly by device models, never by host wall time.
    void set_irq_line(std::uint8_t cpu, std::uint8_t level, bool asserted) noexcept;

    std::uint16_t read_memory_word(std::uint16_t, std::uint32_t, std::uint16_t) override;
    void write_memory_word(std::uint16_t, std::uint32_t, std::uint16_t, std::uint16_t) override;
    std::uint16_t read_hardware(std::uint8_t, std::uint8_t, std::uint8_t,
                                std::uint32_t, std::uint32_t, std::uint16_t) override;
    void write_hardware(std::uint8_t, std::uint8_t, std::uint8_t, std::uint32_t,
                        std::uint32_t, std::uint16_t, std::uint16_t) override;
    PendingInterrupt consume_pending_interrupt(std::uint8_t, std::uint8_t, std::uint32_t) override;
    FunctionResult call_function(std::uint32_t, std::uint8_t, std::uint8_t,
                                 std::uint8_t, std::uint32_t, std::uint32_t,
                                 FunctionContext &) override;

private:
    struct Region {
        std::vector<std::uint8_t> bytes;
        std::vector<std::uint8_t> known_bits;
        bool read_only{};
    };
    struct Invocation {
        Invocation *parent{};
        FunctionResult child{};
        std::uint32_t child_target{};
        bool executed_child{};
        std::uint8_t executing_state{};
        std::uint8_t cpu{};
        std::uint32_t function_id{}, callsite{}, actor{};
    };
    std::array<Region, 12> regions_;
    DirectAssetLoader *assets_{};
    gameplay::CharacterDefinitions character_definitions_;
    gameplay::LegacyEnemyNavigation enemy_navigation_;
    // Direct-loading replacement for the original 76-byte disk save record.
    // Session lifetime only; a fresh host lets the original code select defaults.
    std::array<std::uint8_t, 0x4c> saved_settings_{};
    bool has_saved_settings_{};
    int start_stage_{-1};
    bool start_stage_publish_{};
    bool start_stage_force_{};
    bool start_stage_applied_{};
    bool invulnerable_{};
    std::array<std::uint32_t, 4> player_records_{};
    System24Devices *devices_{};
    bool waiting_{};
    struct SuspendedExecution { FunctionContext *context{}; Invocation *invocation{}; std::size_t depth{}; bool waiting{}; };
    std::array<SuspendedExecution,2> suspended_{};
    unsigned selected_cpu_{};
    std::array<std::uint8_t, 2> irq_lines_{};
    FunctionContext *active_{};
    Invocation *invocation_{};
    RuntimeFault fault_{};
    std::uint64_t operations_{};
    std::uint64_t operations_since_switch_{};
    bool direct_wait_{};
    std::size_t depth_{};
    Checkpoint checkpoint_{};
    void *checkpoint_argument_{};
    std::array<std::uint64_t,2> cpu_deadlines_{UINT64_MAX, UINT64_MAX};
    std::array<unsigned,2> timed_depth_{};
    std::array<TimingCpuPosition,2> timing_positions_{};
    TimingObserver timing_observer_{};
    void *timing_argument_{};
    void checkpoint();
    void fail(std::string_view, std::uint32_t address = 0,
              std::uint16_t region = 0, std::uint16_t mask = 0);
    Region *region_at(std::uint16_t region, std::uint32_t offset);
    const FunctionContract *entry_at(std::uint8_t cpu, std::uint8_t state,
                                     std::uint32_t pc) const noexcept;
    FunctionResult execute(const FunctionContract &, FunctionContext &, std::uint32_t callsite = 0U);
    FunctionResult direct_load_trap(std::uint32_t id, FunctionContext &);
    bool apply_level_definition();
    std::uint16_t definition_read_word(std::uint16_t region, std::uint32_t offset,
                                       std::uint16_t original) const noexcept;
};
} // namespace gain_ground
