#include "gain_ground/system24_devices.h"
#include "gain_ground/runtime_host.h"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace gain_ground;
void require(bool ok, const char *message) { if (!ok) throw std::runtime_error(message); }
struct Probe {
    RuntimeHost host;
    System24Devices devices;
    static void checkpoint(void *arg) {
        auto &p = *static_cast<Probe *>(arg);
        const auto next = p.host.next_cpu_deadline_ns();
        if (next != UINT64_MAX) p.devices.advance(next);
    }
};
int main() { try {
    // MAME ymfm_mame.h: busy_end = exact machine time + 64 / 4 MHz.
    // Cover every sub-chip-clock phase, including the observed 200 ns phase.
    for (std::uint64_t phase = 0; phase < 250; ++phase) {
        System24Devices d;
        const auto start = 100000 + phase;
        const auto end = start + 16000;
        d.write(0x80001c, 4, 0xff); // Release the board's YM reset before testing writes.
        d.advance(start);
        require(d.write(0x800100, 0x64, 0xff), "address write failed");
        require(d.write(0x800102, 0x7f, 0xff), "data write failed");
        require((*d.read(0x800102, 0xff) & 0x80) != 0, "not busy after write");
        require(d.next_event_ns() == end, "scheduler rounded busy deadline");
        d.advance(end - 1);
        require((*d.read(0x800102, 0xff) & 0x80) != 0, "busy cleared before exact deadline");
        d.advance(end);
        require((*d.read(0x800102, 0xff) & 0x80) == 0, "busy not cleared at deadline");
        require(d.next_event_ns() > end, "expired busy event stalls scheduler");
    }
    // Replay the observed caller phase through the real 319 body and YM chip.
    Probe p; p.host.attach_devices(p.devices); p.host.select_cpu(1);
    p.host.set_checkpoint(Probe::checkpoint, &p);
    std::vector<std::uint8_t> ram(p.host.region_bytes(2).size());
    ram[0x7001] = 1; ram[0x7002] = 0x23; ram[0x7003] = 0x40;
    ram[0x7004] = 0xa5; ram[0x7005] = 0x5a;
    require(p.host.load_region(2, 0, ram), "RAM setup failed");
    constexpr std::uint64_t start = 100200;
    p.devices.write(0x80001c, 4, 0xff);
    p.devices.advance(start);
    p.devices.write(0x800100, 0x64, 0xff); p.devices.write(0x800102, 0x7f, 0xff);
    p.devices.advance(start + 5600);
    FunctionContext c{}; c.host = &p.host; c.cpu = 1; c.state = 0x72;
    c.registers.status = 0x2700; c.registers.program_counter = 0x17ec2;
    c.registers.address[7] = 0x7000; c.registers.data[0] = 0x65; c.registers.data[1] = 0x7f;
    const auto result = p.host.call_function(319, 1, 0x72, 2, 0x9000, 0x17ec2, c);
    require(!p.host.faulted() && result.status == TranslationStatus::complete && result.control == 1,
            "real writer did not return");
    require(c.registers.program_counter == 0x12340 && c.registers.address[7] == 0x7004 &&
            p.host.region_bytes(2)[0x7004] == 0xa5 && p.host.region_bytes(2)[0x7005] == 0x5a,
            "real writer return/stack changed");
    require(p.devices.time_ns() == start + 25200, "real writer skipped the reference busy poll");
    // Its data write occurs 2400 ns before return; that restarts the busy period.
    require(p.devices.next_event_ns() == start + 22800 + 16000, "real writer data-write time differs");
    std::cout << "PASS: 250 exact busy deadlines and real function 319 polling/return\n";
} catch (const std::exception &e) { std::cerr << e.what() << '\n'; return 1; } }
