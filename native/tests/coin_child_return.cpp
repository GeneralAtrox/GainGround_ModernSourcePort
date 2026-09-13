#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_devices.h"
#include <array>
#include <iostream>
#include <vector>

int main()
{
    using namespace gain_ground;
    for (const unsigned selector : {0U, 0x0cU, 0x14U}) {
        RuntimeHost host;
        System24Devices devices;
        host.attach_devices(devices);
        host.select_cpu(1);
        std::vector<std::uint8_t> ram(host.region_bytes(2).size());
        const auto word = [&](unsigned at, unsigned value) {
            ram[at] = static_cast<std::uint8_t>(value >> 8);
            ram[at + 1] = static_cast<std::uint8_t>(value);
        };
        // Three coin-table records, with one active coin and no service input.
        for (unsigned i = 0; i != 3; ++i) {
            const std::array<unsigned, 6> record{
                0x600U + i * 4, 0x700U + i * 2, 0x800U + i,
                0xffU, 0x710U + i * 2, 0x810U + i};
            for (unsigned j = 0; j != record.size(); ++j)
                word(0x83ea + i * 12 + j * 2, record[j]);
        }
        ram[0x405] = static_cast<std::uint8_t>(selector);
        ram[0x800] = 1;
        ram[0x601] = 1; // Selector 0x0c pays on the second coin.
        word(0x840e, 0x10);
        ram[0x841e] = 2;
        ram[0x841f] = 1; // Selector 0x14's next table award.
        word(0x7ffc, 0x12); word(0x7ffe, 0x3456);
        word(0x8000, 0x8765); word(0x8002, 0x4321);
        if (!host.load_region(2, 0, ram)) return 2;
        FunctionContext c{};
        c.cpu = 1; c.state = 0x72;
        c.registers.status = 0x2700;
        c.registers.program_counter = 0x8264;
        c.registers.address[7] = 0x7ffc;
        const auto result = host.run(c); // Real 111 -> 112 -> 113 execution.
        const auto after = host.region_bytes(2);
        if (host.faulted() || result.status != TranslationStatus::complete ||
            result.control != 1 || result.exit_program_counter != 0x123456 ||
            c.registers.program_counter != 0x123456 || c.registers.address[7] != 0x8000 ||
            after[0x600] != 1 || after[0x7b27] != 1 || after[0x701] != 1 ||
            after[0x604] != 0 || after[0x608] != 0 ||
            after[0x8000] != 0x87 || after[0x8003] != 0x21) {
            std::cerr << "coin child return failed: selector=" << selector
                      << " pc=" << std::hex << c.registers.program_counter
                      << " sp=" << c.registers.address[7]
                      << " credit=" << unsigned(after[0x600]) << '\n';
            return 1;
        }
    }
    std::cout << "Three coin award paths preserve one return and one credit update\n";
}
