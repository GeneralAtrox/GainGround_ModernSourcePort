#include "gain_ground/runtime_host.h"
#include "gain_ground/runtime_input.h"
#include <iostream>
#include <vector>

int main()
{
    using namespace gain_ground;
    for (unsigned player : {0U, 1U, 2U}) for (bool pressed : {false, true}) {
        RuntimeHost host;
        System24Devices devices;
        host.attach_devices(devices); host.select_cpu(1);
        std::vector<std::uint8_t> ram(host.region_bytes(2).size());
        if (!host.load_region(2, 0, ram)) return 2;
        const auto word = [&](unsigned at, unsigned value) {
            host.write_memory_word(2, at, static_cast<std::uint16_t>(value), 0xffff);
        };
        // The generic Start input alone must not activate a player.
        devices.input(4, static_cast<std::uint8_t>(16U << player), true);
        set_start_input(devices, player, true);
        if (!pressed) set_start_input(devices, player, false);
        FunctionContext c{}; c.cpu = 1; c.state = 0x72;
        c.registers.status = 0x2700;
        c.registers.address[7] = 0x7ffc;
        word(0x7ffc, 0x12); word(0x7ffe, 0x3456);
        c.registers.program_counter = 0x81b6;
        auto result = host.run(c); // Actual IRQ input sampler and its children.
        if (host.faulted() || result.control != 1 || c.registers.address[7] != 0x8000)
            return 3;
        word(0x146a, 0x810 + player * 4); // Selected player's sampled controls.
        word(0x146c, player); // Low byte at +6d is the player index.
        word(0x146e, 0x1600);
        word(0x600, 0x0200); // Two credits.
        word(0x7ffc, 0x12); word(0x7ffe, 0x3456);
        c.registers.program_counter = 0xfa92;
        c.registers.address[7] = 0x7ffc;
        c.registers.address[4] = 0x1000;
        c.registers.address[5] = 0x1400;
        c.registers.address[6] = 0x600;
        c.registers.data[7] = 1; // Original eligibility input to activation.
        result = host.run(c); // Real 183 -> 184, including credit consumption.
        const auto after = host.region_bytes(2);
        const auto callback = (unsigned(after[0x1402]) << 24) |
            (unsigned(after[0x1403]) << 16) | (unsigned(after[0x1404]) << 8) | after[0x1405];
        if (host.faulted() || result.status != TranslationStatus::complete ||
            result.control != 1 || result.exit_program_counter != 0x123456 ||
            c.registers.address[7] != 0x8000 || c.state != 0x72 ||
            after[0x600] != (pressed ? 1 : 2) ||
            after[0x820] != (pressed ? (1U << player) : 0) ||
            after[0x821] != (pressed ? 1 : 0) ||
            callback != (pressed ? 0xee6eU : 0U)) {
            std::cerr << "Player start failed: player=" << player << " pressed=" << pressed << '\n';
            return 1;
        }
    }
    std::cout << "All three player-start bindings activate once; release/generic Start do not\n";
}
