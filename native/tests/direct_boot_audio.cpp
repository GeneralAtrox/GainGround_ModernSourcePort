#include "gain_ground/direct_boot_audio.h"
#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_devices.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

using namespace gain_ground;
struct BootProbe {
    RuntimeHost host;
    System24Devices devices;
    static void checkpoint(void *argument) {
        auto &p = *static_cast<BootProbe *>(argument);
        const auto deadline = p.host.next_cpu_deadline_ns();
        if (deadline != UINT64_MAX) p.devices.advance(deadline);
    }
};
int main(int argc, char **argv) {
    if (argc != 2) return 2;
    std::ifstream input(argv[1], std::ios::binary);
    const std::vector<std::uint8_t> bios{std::istreambuf_iterator<char>(input), {}};
    if (bios.size() != 0x40000U) return 2;
    BootProbe p;
    if (!p.host.load_region(1, 0, bios) ||
        !p.host.load_region(3, 0, std::vector<std::uint8_t>(0x40000))) return 2;
    p.host.attach_devices(p.devices);
    prepare_direct_boot_devices(p.devices);
    p.host.set_checkpoint(BootProbe::checkpoint, &p);
    FunctionContext c{}; c.host = &p.host; c.cpu = 0; c.state = 0xff;
    c.registers.status = 0x2700;
    const auto result = run_direct_boot_audio(c);
    std::cout << "boot ns=" << p.devices.time_ns() << " pc=" << std::hex
              << c.registers.program_counter << " sp=" << c.registers.address[7]
              << " fault=" << p.host.fault().message << '\n';
    // Original instruction phase sum, including both BIOS BSRs and returns.
    if (p.host.faulted() || result.status != TranslationStatus::complete || result.control != 1 ||
        c.registers.program_counter != 0x4f8 || c.registers.address[7] != 0 ||
        c.state != 0xff || (c.registers.status & 0xff00) != 0x2700 ||
        p.host.timed_execution() || p.devices.time_ns() != 832600) return 1;
    p.devices.write(0x80001e, 0x88, 0xff); // Original game enables port-H output.
    if (p.devices.read(0x80000e, 0xff) != 0x80) return 1;
    // BIOS reg 0x10=125, 0x11=0: (1024 - 500) * 64 / 4 MHz = 8.384 ms.
    // This distinguishes restored BIOS configuration from a reset chip's
    // 16.384 ms default, and checks the exact off-chip-clock bus origin.
    const auto start = p.devices.time_ns();
    p.devices.write(0x800100, 0x14, 0xff);
    p.devices.write(0x800102, 0x15, 0xff);
    p.devices.advance(start + 8384000 - 1);
    if (p.devices.audio.irq()) { std::cerr << "YM Timer A asserted early\n"; return 1; }
    p.devices.advance(start + 8384000);
    if (!p.devices.audio.irq()) { std::cerr << "YM Timer A missed deadline\n"; return 1; }
    std::cout << "BIOS DAC and exact YM Timer-A period PASS\n";
    for (unsigned phase = 0; phase < 250; ++phase) {
        System24Audio audio;
        const auto write = [&](unsigned reg, unsigned value) {
            audio.write(0, static_cast<std::uint8_t>(reg));
            audio.write(1, static_cast<std::uint8_t>(value));
        };
        write(0x10, 125); write(0x11, 0);
        const std::uint64_t origin = 100000 + phase;
        audio.advance_to_ns(origin); write(0x14, 0x15);
        auto deadline = origin + 8384000;
        audio.advance_to_ns(deadline - 1);
        if (audio.irq() || audio.next_event_ns() != deadline) return 1;
        audio.advance_to_ns(deadline);
        if (!audio.irq()) return 1;
        write(0x14, 0x15); // Clear status, retain the running periodic timer.
        deadline += 8384000;
        audio.advance_to_ns(deadline - 1);
        if (audio.irq()) return 1;
        audio.advance_to_ns(deadline);
        if (!audio.irq()) return 1;
        write(0x14, 0x10); // Acknowledge and cancel.
        audio.advance_to_ns(deadline + 8384000);
        if (audio.irq() || audio.next_event_ns() != UINT64_MAX) return 1;
        if (audio.take_samples().size() != ((deadline + 8384000) / 16000) * 2) return 1;
    }
    std::cout << "250 bus phases: YM timer start/reload/cancel and sample count PASS\n";
}
