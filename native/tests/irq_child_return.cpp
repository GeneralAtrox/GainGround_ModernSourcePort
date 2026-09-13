#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_devices.h"
#include <iostream>
#include <vector>

int main()
{
    using namespace gain_ground;
    RuntimeHost host;
    System24Devices devices;
    host.attach_devices(devices);
    host.select_cpu(1);
    std::vector<std::uint8_t> ram(host.region_bytes(2).size());
    ram[0x820] = 0x80; // I/O-only IRQ path; coin award chain has its own test.
    if (!host.load_region(2, 0, ram)) return 2;
    for (const bool pressed : {true, false}) {
        devices.input(0, 2, pressed);
        FunctionContext c{};
        c.cpu = 1; c.state = 0x72; c.host = &host;
        auto &r = c.registers;
        for (unsigned i = 0; i != 8; ++i) r.data[i] = 0x12340000U + i;
        for (unsigned i = 0; i != 7; ++i) r.address[i] = 0x56780000U + i;
        r.address[7] = 0x7ff2;
        r.status = 0x2400;
        r.program_counter = 0x806e;
        const auto saved = r;
        // Architectural exception frame presented to the actual IRQ4 entry.
        host.write_memory_word(2, 0x7ff2, 0x2015, 0xffff);
        host.write_memory_word(2, 0x7ff4, 0, 0xffff);
        host.write_memory_word(2, 0x7ff6, 0x857a, 0xffff);
        host.write_memory_word(2, 0x7ff8, 0xa55a, 0xffff);
        const auto result = host.call_function(100, 1, 0x04, 6, 0x85b4, 0x806e, c);
        // Real 100 -> 104 -> 107 -> 108, including the final fall-through and RTE.
        const auto after = host.region_bytes(2);
        bool addresses_preserved = true;
        for (unsigned i = 0; i != 7; ++i)
            addresses_preserved &= r.address[i] == saved.address[i];
        if (host.faulted() || result.status != TranslationStatus::complete ||
            result.control != 2 || result.exit_program_counter != 0x857a ||
            r.program_counter != 0x857a || r.status != 0x2015 || c.state != 0x72 ||
            r.address[7] != 0x7ff8 || r.data != saved.data || !addresses_preserved ||
            after[0x810] != (pressed ? 0 : 2) ||
            after[0x811] != (pressed ? 2 : 0) ||
            after[0x812] != (pressed ? 2 : 0) ||
            after[0x813] != (pressed ? 0 : 2) ||
            after[0x7ff8] != 0xa5 || after[0x7ff9] != 0x5a) {
            std::cerr << "IRQ child return failed: pressed=" << pressed
                      << " pc=" << std::hex << r.program_counter
                      << " sp=" << r.address[7] << '\n';
            return 1;
        }
    }
    std::cout << "Real IRQ children preserve registers, FD1094 state and one input edge per sample\n";
}
