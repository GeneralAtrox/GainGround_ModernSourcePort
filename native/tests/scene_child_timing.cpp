#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_devices.h"
#include <iostream>
#include <vector>

using namespace gain_ground;
struct SceneProbe {
    RuntimeHost host;
    System24Devices devices;
    static void checkpoint(void *argument) {
        auto &p = *static_cast<SceneProbe *>(argument);
        const auto deadline = p.host.next_cpu_deadline_ns();
        if (deadline != UINT64_MAX) p.devices.advance(deadline);
    }
};
int main() {
    // Run the installed 309 -> 312 -> 316 sound command chain and the
    // actual restore/RTS continuation. No child fixture responses are supplied.
    struct Case { const char *name; unsigned command, descriptor, pending, count; bool enqueue; std::uint64_t ns; };
    // Source phase sums: wrapper 116 clocks; 316 contributes 18/88/112/
    // 178/126 clocks; the real 546 RTS adds 16, and 547 normal adds 114.
    // Queue-full adds 592's MOVE to CCR (16 clocks) and RTS (16 clocks).
    for (const auto test : {
        Case{"limit", 255, 0, 0, 0, false, 15000},
        Case{"same-disabled", 0, 0, 0, 0, false, 22000},
        Case{"same-pending", 0, 0x8005, 1, 0, false, 24400},
        Case{"same-enabled", 0, 0x8005, 0, 0, true, 40800},
        Case{"new-command", 2, 5, 0, 0, true, 35600},
        Case{"queue-full", 2, 5, 0, 15, true, 31600}}) {
    SceneProbe p; p.host.attach_devices(p.devices); p.host.select_cpu(1);
    p.host.set_checkpoint(SceneProbe::checkpoint, &p);
    std::vector<std::uint8_t> ram(p.host.region_bytes(2).size());
    ram[0x7000] = 0; ram[0x7001] = 1; ram[0x7002] = 0x23; ram[0x7003] = 0x40;
    ram[0x7004] = 0xa5; ram[0x7005] = 0x5a;
    const auto table = 0x17ef8 + (test.command & 0xfe);
    ram[table] = static_cast<std::uint8_t>(test.descriptor >> 8);
    ram[table + 1] = static_cast<std::uint8_t>(test.descriptor);
    if (!p.host.load_region(2, 0, ram)) return 2;
    std::vector<std::uint8_t> shared(p.host.region_bytes(3).size());
    shared[0x3c08b] = static_cast<std::uint8_t>(test.pending);
    shared[0x3c081] = static_cast<std::uint8_t>(test.count);
    if (!p.host.load_region(3, 0, shared)) return 2;
    FunctionContext c{}; c.host = &p.host; c.cpu = 1; c.state = 0x72;
    auto &r = c.registers;
    r.program_counter = 0x1700c; r.status = 0x2700; r.address[7] = 0x7000;
    r.address[5] = 0x12345678; r.address[6] = 0x23456789;
    r.data[0] = test.command;
    const auto result = p.host.call_function(309, 1, 0x72, 2, 0x9000, 0x1700c, c);
    const auto after = p.host.region_bytes(2);
    std::cout << test.name << " elapsed=" << std::dec << p.devices.time_ns() << " pc=" << std::hex << r.program_counter
              << " sp=" << r.address[7] << " fault=" << p.host.fault().message << '\n';
    if (p.host.faulted() || result.status != TranslationStatus::complete || result.control != 1 ||
        r.program_counter != 0x12340 || result.exit_program_counter != r.program_counter ||
        r.address[7] != 0x7004 || r.address[5] != 0x12345678 || r.address[6] != 0x23456789 ||
        (r.status & 0xff00) != 0x2700 || c.state != 0x72 || p.host.timed_execution() ||
        after[0x7004] != 0xa5 || after[0x7005] != 0x5a || p.devices.time_ns() != test.ns) return 1;
    const auto queue = p.host.region_bytes(3);
    // The real 547 continuation increments the command counter once.
    if (queue[0x3c081] != test.count + (test.enqueue ? 1 : 0) ||
        queue[0x3c089] != (test.enqueue ? test.command : 0)) return 1;
    if (test.enqueue && test.count != 15 &&
        (queue[0x3c060] != 0 || queue[0x3c061] != test.command || queue[0x3c083] != 2)) return 1;
    if (test.count == 15 && (queue[0x3c083] != 0 || !(r.status & 1))) return 1;
    }
}
