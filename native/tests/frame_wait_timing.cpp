#include "gain_ground/contract_types.h"
#include "gground_functions.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

using namespace gain_ground;
void require(bool ok, const char *message) { if (!ok) throw std::runtime_error(message); }
struct Probe final : ExecutionHost {
    std::uint64_t now{36406000}, edge{}, first_stack{}, child_end{};
    std::uint32_t resume{};
    unsigned depth{}, calls{};
    std::map<std::uint64_t, std::uint16_t> memory;
    static std::uint64_t key(unsigned region, unsigned offset) {
        return (std::uint64_t(region) << 32U) | offset;
    }
    std::uint64_t execution_time_ns() const noexcept override { return now; }
    void wait_until_time(std::uint64_t deadline) override { now = std::max(now, deadline); }
    void begin_timed_execution() noexcept override { ++depth; }
    void end_timed_execution() noexcept override { require(depth != 0, "scope underflow"); --depth; }
    bool resumes_interrupts_inline() const noexcept override { return true; }
    std::uint16_t read_memory_word(std::uint16_t region, std::uint32_t offset, std::uint16_t) override {
        return memory[key(region, offset)];
    }
    void write_memory_word(std::uint16_t region, std::uint32_t offset,
                           std::uint16_t value, std::uint16_t mask) override {
        if (offset >= 0x3fff0U && !first_stack) first_stack = now;
        auto &word = memory[key(region, offset)];
        word = static_cast<std::uint16_t>((word & ~mask) | (value & mask));
    }
    std::uint16_t read_hardware(std::uint8_t, std::uint8_t, std::uint8_t,
        std::uint32_t, std::uint32_t, std::uint16_t) override { throw std::runtime_error("hardware read"); }
    void write_hardware(std::uint8_t, std::uint8_t, std::uint8_t,
        std::uint32_t, std::uint32_t, std::uint16_t, std::uint16_t) override { throw std::runtime_error("hardware write"); }
    PendingInterrupt consume_pending_interrupt(std::uint8_t, std::uint8_t, std::uint32_t) override {
        return {!calls && now >= edge, 5U};
    }
    FunctionResult call_function(std::uint32_t id, std::uint8_t cpu, std::uint8_t state,
        std::uint8_t kind, std::uint32_t, std::uint32_t target, FunctionContext &c) override {
        require(id == 48 && cpu == 0 && state == 0xff && kind == 3 && target == 0x8004e,
                "wrong interrupt child");
        require(depth == 0 && ++calls == 1, "parent timing leaked into ISR");
        // Controlled child duration checks that the parent restarts its deadline.
        now += 123400; child_end = now;
        auto &r = c.registers; const auto sp = r.address[7] & 0x3ffffU;
        r.status = memory[key(3, sp)];
        resume = (std::uint32_t(memory[key(3, sp + 2)]) << 16U) | memory[key(3, sp + 4)];
        r.address[7] += 6; r.program_counter = resume;
        memory[key(3, 0x38400)] = 0xff00; // Frame handler releases the wait gate.
        return FunctionResult::complete(2U, resume);
    }
};

int main() {
    // Captured TST starts at 36,406,000 ns. Taken BEQ spans 36,407,200..
    // 36,408,200 and samples before its final bus cycle at 36,407,800.
    struct Case { std::uint64_t edge, stack; std::uint32_t resume; };
    for (auto test : {Case{36406700,36407800,0x8034e},
                      Case{36406900,36408800,0x8034a},
                      Case{36407600,36408800,0x8034a},
                      Case{36408000,36410000,0x8034e}}) {
        Probe h; h.edge = test.edge;
        h.memory[Probe::key(1,0x74)] = 8; h.memory[Probe::key(1,0x76)] = 0x4e;
        h.memory[Probe::key(3,0x4e)] = 0x4ef9; h.memory[Probe::key(3,0x50)] = 8;
        h.memory[Probe::key(3,0x3fffc)] = 8; h.memory[Probe::key(3,0x3fffe)] = 0x10c;
        FunctionContext c{}; c.host = &h; c.cpu = 0; c.state = 0xff;
        c.registers.program_counter = 0x8034a; c.registers.status = 0x2004;
        c.registers.address[7] = 0xfffffffc;
        const auto result = translated::cpu_a_sprite_frame_gate_wait(c);
        require(result.status == TranslationStatus::complete && result.control == 1 &&
            result.exit_program_counter == 0x8010c && c.registers.address[7] == 0,
            "wait failed to return through original stack");
        require(h.first_stack == test.stack && h.resume == test.resume, "wrong IPL acceptance boundary");
        require(h.now == h.child_end + (test.resume == 0x8034e ? 6200U : 5200U),
            "post-ISR instruction time was swallowed or double charged");
        require(h.depth == 0 && h.calls == 1 && h.memory[Probe::key(3,0x38400)] == 0,
            "scope, interrupt count or gate clear");
    }
    std::cout << "PASS: four IPL edge positions, captured first-IRQ boundary, post-ISR clocks and RTS\n";
}
