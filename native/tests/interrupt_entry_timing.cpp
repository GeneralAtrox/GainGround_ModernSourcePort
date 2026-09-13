#include "gain_ground/m68000_interrupt_entry.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

using namespace gain_ground;
void require(bool ok, const char *message) { if (!ok) throw std::runtime_error(message); }
struct Access {
    bool write;
    std::uint16_t region;
    std::uint32_t offset;
    std::uint16_t value;
    std::uint64_t time;
    bool operator==(const Access &) const = default;
};
struct Probe final : ExecutionHost {
    std::uint64_t now{};
    unsigned depth{}, calls{}, unsupported{};
    bool bad_return{};
    TimingCpuPosition position;
    std::map<std::uint64_t, std::uint16_t> memory;
    std::vector<Access> accesses;
    static std::uint64_t key(std::uint16_t region, std::uint32_t offset) {
        return (std::uint64_t(region) << 32U) | offset;
    }
    std::uint64_t execution_time_ns() const noexcept override { return now; }
    void wait_until_time(std::uint64_t deadline) override { now = std::max(now, deadline); }
    void begin_timed_execution() noexcept override { ++depth; }
    void end_timed_execution() noexcept override { --depth; }
    bool resumes_interrupts_inline() const noexcept override { return true; }
    TimingCpuPosition *instruction_timing_position(std::uint8_t) noexcept override { return &position; }
    void observe_instruction_timing(const TimingTraceEvent &e) override {
        if (e.kind == "unsupported") ++unsupported;
    }
    std::uint16_t read_memory_word(std::uint16_t region, std::uint32_t offset, std::uint16_t mask) override {
        require(mask == 0xffffU, "IRQ word mask");
        const auto value = memory.at(key(region, offset));
        accesses.push_back({false, region, offset, value, now});
        return value;
    }
    void write_memory_word(std::uint16_t region, std::uint32_t offset, std::uint16_t value, std::uint16_t mask) override {
        require(mask == 0xffffU, "IRQ stack word mask");
        memory[key(region, offset)] = value;
        accesses.push_back({true, region, offset, value, now});
    }
    std::uint16_t read_hardware(std::uint8_t, std::uint8_t, std::uint8_t,
        std::uint32_t, std::uint32_t, std::uint16_t) override { throw std::runtime_error("invented hardware read"); }
    void write_hardware(std::uint8_t, std::uint8_t, std::uint8_t,
        std::uint32_t, std::uint32_t, std::uint16_t, std::uint16_t) override { throw std::runtime_error("invented hardware write"); }
    PendingInterrupt consume_pending_interrupt(std::uint8_t, std::uint8_t, std::uint32_t) override { return {}; }
    FunctionResult call_function(std::uint32_t id, std::uint8_t cpu, std::uint8_t state,
        std::uint8_t kind, std::uint32_t site, std::uint32_t target, FunctionContext &c) override {
        ++calls;
        require(depth == 0U, "entry timer leaked into child");
        require(cpu == 0U && state == 0xffU && kind == 3U && site == 0x81028U, "ISR dispatch identity");
        require(id >= 46U && id <= 48U && target == 0x80042U + (id - 46U) * 6U, "vector/native target mapping");
        require(c.registers.address[7] == 0xffffc0faU, "six-byte exception stack");
        require(c.registers.status == std::uint16_t(0x2015U | ((id - 43U) << 8U)), "live interrupt SR");
        // Emulate the child's RTE boundary; entry stack bytes are the source.
        c.registers.status = memory.at(key(3, 0x3c0faU));
        c.registers.program_counter = (std::uint32_t(memory.at(key(3, 0x3c0fcU))) << 16U)
            | memory.at(key(3, 0x3c0feU));
        c.registers.address[7] += 6U;
        if (bad_return) ++c.registers.program_counter;
        return FunctionResult::complete(2U, c.registers.program_counter);
    }
};

int main() {
    // Pinned state_interrupt + default_autovectors_map/vpa_sync. This table
    // exercises both sides of the phase-7 boundary, not just total duration.
    constexpr std::array<unsigned, 10> waits{10,9,8,7,6,5,4,13,12,11};
    for (unsigned phase = 0; phase < 10; ++phase) for (unsigned level = 3; level <= 5; ++level) {
        Probe h; const auto origin = std::uint64_t(phase) * 100U; h.now = origin;
        const auto vector = (24U + level) * 4U;
        const auto offset = 0x42U + (level - 3U) * 6U;
        h.memory[Probe::key(1, vector)] = 8U;
        h.memory[Probe::key(1, vector + 2U)] = static_cast<std::uint16_t>(offset);
        h.memory[Probe::key(3, offset)] = 0x4ef9U;
        h.memory[Probe::key(3, offset + 2U)] = 8U;
        FunctionContext c{}; c.host = &h; c.registers.status = 0xa015U;
        c.registers.address[7] = 0xffffc100U;
        c.registers.program_counter = 0x81028U;
        const auto result = service_cpu_a_autovector(c, static_cast<std::uint8_t>(level), 0x81028U, 0x8034eU);
        require(result.status == TranslationStatus::complete && result.control == 2U, "ISR did not return");
        require(c.registers.status == 0xa015U && c.registers.program_counter == 0x8034eU &&
            c.registers.address[7] == 0xffffc100U, "RTE failed to restore SR/PC/SP");
        const auto d = waits[phase] * 100U;
        const std::vector<Access> expected{
            {true,3,0x3c0feU,0x034eU,origin+600U},
            {true,3,0x3c0faU,0xa015U,origin+1900U+d},
            {true,3,0x3c0fcU,8U,origin+2300U+d},
            {false,1,vector,8U,origin+2700U+d},
            {false,1,vector+2U,static_cast<std::uint16_t>(offset),origin+3100U+d},
            {false,3,offset,0x4ef9U,origin+3500U+d},
            {false,3,offset+2U,8U,origin+4100U+d}};
        require(h.accesses == expected, "entry access order/value/phase or duplicate vector read");
        require(h.now == origin+4500U+d && h.position.clocks == 45U+waits[phase], "entry clocks");
        require(h.calls == 1U && h.depth == 0U && h.unsupported == 1U, "dispatch count, scope or trace honesty");
    }
    // Unsupported user stack must fail before any memory mutation.
    Probe h; FunctionContext c{}; c.host = &h; c.registers.status = 0x0015U;
    c.registers.address[7] = 0xffffc100U;
    auto result = service_cpu_a_autovector(c, 5U, 0x81028U, 0x8034eU);
    require(result.status == TranslationStatus::contract_violation && h.accesses.empty() && h.calls == 0,
        "unsupported user stack was guessed");
    for (const bool wrong_return : {false, true}) {
        Probe failure; failure.bad_return = wrong_return;
        failure.memory[Probe::key(1, 0x74U)] = wrong_return ? 8U : 0x40U;
        failure.memory[Probe::key(1, 0x76U)] = 0x4eU;
        failure.memory[Probe::key(3, 0x4eU)] = 0x4ef9U;
        failure.memory[Probe::key(3, 0x50U)] = 8U;
        FunctionContext f{}; f.host = &failure; f.registers.status = 0xa015U;
        f.registers.address[7] = 0xffffc100U;
        result = service_cpu_a_autovector(f, 5U, 0x81028U, 0x8034eU);
        require(result.status == TranslationStatus::contract_violation,
            "unmapped vector or wrong RTE was accepted");
        require(failure.calls == (wrong_return ? 1U : 0U) && failure.depth == 0U,
            "invalid vector dispatched or failure leaked timed scope");
        require(result.exit_program_counter == (wrong_return ? 0x8034fU : 0x40004eU),
            "unresolved vector/return address lost");
    }
    std::cout << "PASS: 30 supervisor entries, all E-clock phases, IRQ3/4/5 vectors, stack order, single dispatch, RTE; user-stack, unmapped-vector and wrong-RTE rejection\n";
}
