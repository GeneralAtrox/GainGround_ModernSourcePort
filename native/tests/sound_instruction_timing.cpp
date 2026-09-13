#include "gain_ground/contract_types.h"
#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_devices.h"
#include "gground_functions.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace gain_ground;
void require(bool value, const char *message) { if (!value) throw std::runtime_error(message); }
struct TimingHost final : ExecutionHost {
    std::uint64_t now{}, busy_until{};
    unsigned timed_depth{};
    std::uint8_t expected_cpu{};
    std::vector<std::uint64_t> reads, writes;
    FunctionResult (*body)(FunctionContext &) noexcept{};
    std::uint64_t execution_time_ns() const noexcept override { return now; }
    void wait_until_time(std::uint64_t deadline) override { now = std::max(now, deadline); }
    void begin_timed_execution() noexcept override { ++timed_depth; }
    void end_timed_execution() noexcept override { --timed_depth; }
    std::uint16_t read_memory_word(std::uint16_t, std::uint32_t, std::uint16_t) override { return 0; }
    void write_memory_word(std::uint16_t, std::uint32_t, std::uint16_t, std::uint16_t) override {}
    std::uint16_t read_hardware(std::uint8_t, std::uint8_t cpu, std::uint8_t, std::uint32_t,
        std::uint32_t, std::uint16_t) override {
        require(cpu == expected_cpu, "status read used the wrong CPU");
        require(timed_depth > 0, "status read outside timed scope");
        reads.push_back(now); return now < busy_until ? 0x80 : 0;
    }
    void write_hardware(std::uint8_t kind, std::uint8_t cpu, std::uint8_t, std::uint32_t,
        std::uint32_t, std::uint16_t, std::uint16_t) override {
        require(cpu == expected_cpu, "sound write used the wrong CPU");
        if (kind == 2) writes.push_back(now);
    }
    PendingInterrupt consume_pending_interrupt(std::uint8_t, std::uint8_t, std::uint32_t) override { return {}; }
    FunctionResult call_function(std::uint32_t, std::uint8_t, std::uint8_t, std::uint8_t kind,
        std::uint32_t, std::uint32_t, FunctionContext &context) override {
        require(kind == 1, "unexpected sound child"); return body(context);
    }
};

int main() {
    for (auto body : {&translated::cpu_a_sound_ym2151_write_wait_ready,
        &translated::cpu_a_sound_ym2151_write, &translated::cpu_b_sound_ym2151_write_init,
        &translated::cpu_b_sound_ym2151_write}) {
        for (const auto busy : {0U, 8000U}) {
            TimingHost host; host.body = body; host.busy_until = busy;
            FunctionContext context{}; context.host = &host;
            context.cpu = host.expected_cpu =
                body == &translated::cpu_b_sound_ym2151_write_init ||
                body == &translated::cpu_b_sound_ym2151_write ? 1U : 0U;
            const auto result = body(context);
            require(result.status == TranslationStatus::complete, "body did not complete");
            require(host.timed_depth == 0, "timed scope leaked across return");
            if (busy) {
                require(host.reads == std::vector<std::uint64_t>{1200,4200,7200,10200}, "busy polling clock differs from BTST/BNE cycles");
                require(host.writes == std::vector<std::uint64_t>{12600,14200} && host.now == 16600, "busy-path writes/RTS timing");
            } else {
                require(host.reads == std::vector<std::uint64_t>{1200}, "BTST bus phase");
                require(host.writes == std::vector<std::uint64_t>{3600,5200} && host.now == 7600, "ready-path writes/RTS timing");
            }
        }
    }
    struct SchedulerProbe {
        RuntimeHost host;
        System24Devices devices;
        bool checked{};
        static void checkpoint(void *arg) {
            auto &probe = *static_cast<SchedulerProbe *>(arg);
            require(probe.host.waiting_for_device(), "deadline did not suspend CPU");
            require(!probe.host.cpu_ready(0) && probe.host.cpu_ready(1), "CPU deadlines not independent");
            require(probe.host.next_cpu_deadline_ns() == 1600, "CPU deadline absent from scheduler");
            probe.host.select_cpu(1);
            require(!probe.host.timed_execution(), "timed scope leaked to other CPU");
            probe.host.select_cpu(0);
            require(probe.host.timed_execution(), "timed scope not restored");
            probe.devices.advance(1600);
            probe.checked = true;
        }
    } probe;
    probe.host.attach_devices(probe.devices);
    probe.host.set_checkpoint(&SchedulerProbe::checkpoint, &probe);
    probe.host.begin_timed_execution();
    probe.host.wait_until_time(1600);
    probe.host.end_timed_execution();
    require(probe.checked && probe.host.cpu_ready(0) && probe.host.next_cpu_deadline_ns() == UINT64_MAX,
        "completed CPU deadline not cleared");
    // Timed polls sample busy immediately, without the legacy device-event shortcut.
    probe.host.set_checkpoint(nullptr, nullptr);
    // Release the board's YM reset line before writing its registers.
    // The power-on device state correctly ignores writes while reset is held.
    probe.devices.write(0x80001c, 4, 0xff);
    probe.devices.write(0x800100, 0x14, 0xff);
    probe.devices.write(0x800102, 0x15, 0xff);
    probe.host.begin_timed_execution();
    require(probe.host.read_hardware(1, 0, 0xff, 0x84126, 0x800102, 0xff) & 0x80, "YM busy sample lost");
    probe.host.end_timed_execution();
    std::cout << "PASS: four sound writers ready/busy bus phases and RTS; CPU deadline isolation and YM busy sample\n";
}
