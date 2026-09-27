#include "gain_ground/runtime_host.h"
#include "gain_ground/runtime_input.h"
#include <iostream>
#include <vector>

int main()
{
    using namespace gain_ground;
    for (bool unlimited : {false, true}) for (unsigned credits : {0U, 2U})
    for (unsigned player : {0U, 1U, 2U}) for (bool pressed : {false, true}) {
        RuntimeHost host;
        System24Devices devices;
        host.attach_devices(devices); host.select_cpu(1);
        std::vector<std::uint8_t> ram(host.region_bytes(2).size());
        if (!host.load_region(2, 0, ram)) return 2;
        if (host.unlimited_credits()) return 4;
        host.set_unlimited_credits(unlimited);
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
        word(0x600, (credits << 8U) | 1U);
        word(0x602, 0x0200); // Partial coin counters must survive unlimited play.
        word(0x404, 0x0035); // Adjacent coinage setting must be preserved.
        word(0x7ffc, 0x12); word(0x7ffe, 0x3456);
        c.registers.program_counter = 0xfa92;
        c.registers.address[7] = 0x7ffc;
        c.registers.address[4] = 0x1000;
        c.registers.address[5] = 0x1400;
        c.registers.address[6] = 0x600;
        // The same credit/override eligibility used by the start/continue callers.
        c.registers.data[7] = (host.read_memory_word(2,0x600,0xff00) |
            host.read_memory_word(2,0x404,0xff00)) >> 8U;
        result = host.run(c); // Real 183 -> 184, including credit consumption.
        const auto after = host.region_bytes(2);
        const auto callback = (unsigned(after[0x1402]) << 24) |
            (unsigned(after[0x1403]) << 16) | (unsigned(after[0x1404]) << 8) | after[0x1405];
        const bool activated = pressed && (credits != 0U || unlimited);
        if (host.faulted() || result.status != TranslationStatus::complete ||
            result.control != 1 || result.exit_program_counter != 0x123456 ||
            c.registers.address[7] != 0x8000 || c.state != 0x72 ||
            after[0x600] != (activated && !unlimited ? credits - 1U : credits) ||
            after[0x601] != (activated && !unlimited ? 0U : 1U) ||
            after[0x602] != (activated && !unlimited ? 0U : 2U) ||
            after[0x820] != (activated ? (1U << player) : 0) ||
            after[0x821] != (activated ? 1 : 0) ||
            callback != (activated ? 0xee6eU : 0U)) {
            std::cerr << "Player start failed: player=" << player << " pressed=" << pressed << '\n';
            return 1;
        }
        if (host.read_memory_word(2,0x404,0x00ff) != 0x35 || after[0x404] != 0) return 5;
        host.set_unlimited_credits(false);
        if (host.read_memory_word(2,0x404,0xffff) != 0x0035) return 6;
    }
    std::cout << "PASS: 24 start cases; normal spending, zero-credit unlimited play, release and toggle restoration\n";
}
