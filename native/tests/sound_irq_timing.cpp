#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_devices.h"
#include <iostream>
#include <vector>

using namespace gain_ground;
struct Probe {
    RuntimeHost host;
    System24Devices devices;
    static void checkpoint(void *arg) {
        auto &p = *static_cast<Probe *>(arg);
        const auto deadline = p.host.next_cpu_deadline_ns();
        if (deadline != UINT64_MAX) p.devices.advance(deadline);
    }
};
int main() {
    struct Case { bool busy; unsigned priority; };
    for (const bool native_clock : {false, true})
    for (const auto test : {Case{true, 0}, Case{false, 0},
                            Case{false, 0x8000}, Case{false, 0x0400}}) {
        const bool busy = test.busy;
        Probe p; p.host.attach_devices(p.devices);
        if (native_clock) p.host.set_native_services([](void *) {}, nullptr);
        std::vector<std::uint8_t> ram(p.host.region_bytes(3).size());
        if (test.priority) {
            const auto word = [&](unsigned offset, unsigned value) {
                ram[offset] = static_cast<std::uint8_t>(value >> 8U);
                ram[offset + 1U] = static_cast<std::uint8_t>(value);
            };
            // Real 72 -> 76 -> 82 -> 85 priority rejection. F85's LEA
            // 4(SP),SP / RTS skips F82; the enclosing IRQ must still RTE
            // through its original saved registers and exception frame.
            word(0x30004, 0x1000); // A5=FB0000, request table offset.
            word(0x31002, 0x0040); // Nonzero request resource offset.
            word(0x31102, 0x0020); // Request cooldown.
            word(0x31202, test.priority == 0x8000 ? 0x0800 : 0x0080);
            word(0x3c060, 2); // Queue entry, cursor zero.
            word(0x3c080, 1); // One pending request.
            word(0x3c032, test.priority); // Existing higher-priority sound.
        }
        ram[0x3800a] = busy ? 0x80 : 0;
        ram[0x37ff0] = 0x20; ram[0x37ff1] = 0x15;
        ram[0x37ff2] = 0; ram[0x37ff3] = 8;
        ram[0x37ff4] = 1; ram[0x37ff5] = 0x18;
        ram[0x37ff6] = 0xa5; ram[0x37ff7] = 0x5a;
        if (!p.host.load_region(3, 0, ram)) return 2;
        p.host.set_checkpoint(&Probe::checkpoint, &p);
        FunctionContext c{}; c.cpu = 0; c.state = 0xff; c.host = &p.host;
        auto &r = c.registers;
        for (unsigned i=0; i<8; ++i) r.data[i] = 0x12340000 + i;
        for (unsigned i=0; i<7; ++i) r.address[i] = 0x56780000 + i;
        r.address[7] = 0xffff7ff0; r.status = 0x2300; r.program_counter = 0x80f96;
        const auto saved = r;
        const auto result = p.host.call_function(72, 0, 0xff, 3, 0x80042, 0x80f96, c);
        bool preserved = r.data == saved.data;
        for (unsigned i=0; i<7; ++i) preserved &= r.address[i] == saved.address[i];
        const auto after = p.host.region_bytes(3);
        std::cout << "native_clock=" << native_clock << " busy=" << busy << " priority=" << test.priority << " elapsed=" << p.devices.time_ns()
                  << " pc=" << std::hex << r.program_counter << std::dec << '\n';
        if (p.host.faulted() || result.status != TranslationStatus::complete || result.control != 2 ||
            r.program_counter != 0x80118 || r.status != 0x2015 || r.address[7] != 0xffff7ff6 ||
            !preserved || after[0x37ff6] != 0xa5 || after[0x37ff7] != 0x5a ||
            after[0x3800a] != (busy ? 0x80 : 0) || p.host.timed_execution()) return 1;
        // Original 20+18+10+16+4+127*10+14+20+20 clocks, at 10 MHz.
        if (busy && p.devices.time_ns() != 139200) return 1;
        if (!busy) {
            // Compare the real child alone with 72 -> 76 -> 88 -> RTS -> RTE.
            // The parent must charge its 222 entry + 168 exit clocks exactly
            // once, independently of the child's elapsed time.
            Probe child; child.host.attach_devices(child.devices);
            if (native_clock) child.host.set_native_services([](void *) {}, nullptr);
            ram[0x37000] = 0; ram[0x37001] = 8;
            ram[0x37002] = 0x0f; ram[0x37003] = 0xbc;
            if (!child.host.load_region(3, 0, ram)) return 2;
            child.host.set_checkpoint(&Probe::checkpoint, &child);
            FunctionContext cc{}; cc.cpu = 0; cc.state = 0xff; cc.host = &child.host;
            cc.registers.address[5] = 0xfb0000;
            cc.registers.address[6] = 0xffffc000;
            cc.registers.address[7] = 0xffff7000;
            cc.registers.status = 0x2300; cc.registers.program_counter = 0x83208;
            const auto cr = child.host.call_function(76, 0, 0xff, 2, 0x80fb6, 0x83208, cc);
            std::cout << "child elapsed=" << child.devices.time_ns() << " parent overhead="
                      << p.devices.time_ns() - child.devices.time_ns() << " pc=" << std::hex
                      << cc.registers.program_counter << " sp=" << cc.registers.address[7]
                      << std::dec << " fault=" << child.host.faulted() << '\n';
            if (child.host.faulted() || cr.status != TranslationStatus::complete || cr.control != 1 ||
                cc.registers.program_counter != 0x80fbc || cc.registers.address[7] != 0xffff7004 ||
                p.devices.time_ns() != child.devices.time_ns() + 39000) return 1;
        }
    }
}
