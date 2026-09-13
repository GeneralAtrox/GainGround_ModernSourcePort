#include "gain_ground/runtime_host.h"
#include <iostream>
#include <vector>

int main()
{
    using namespace gain_ground;
    for (const bool clears_step : {true, false}) {
        RuntimeHost host;
        host.select_cpu(1);
        std::vector<std::uint8_t> ram(host.region_bytes(2).size());
        const auto word = [&](unsigned a, unsigned v) {
            ram[a] = static_cast<std::uint8_t>(v >> 8);
            ram[a+1] = static_cast<std::uint8_t>(v);
        };
        constexpr unsigned record = 0x3400;
        ram[record+0x41] = clears_step ? 0x60 : 0x20;
        ram[record+0x36] = 77;
        ram[record+0x37] = 1;
        word(record+0x66,0); word(record+0x68,0x3000);
        word(0x3000,0); word(0x3002,0x3100);
        ram[0x3102] = 255; // Outside helper's first threshold: retain step bit.
        word(record+0x6e,0); word(record+0x70,0x3200);
        ram[0x3203] = 2;
        word(record+0x1e,0x1234); word(record+0x26,0x5678);
        word(0x7ffc,0x12); word(0x7ffe,0x3456);
        word(0x8000,0x8765); word(0x8002,0x4321);
        if (!host.load_region(2,0,ram)) return 2;
        FunctionContext c{};
        c.cpu=1; c.state=0x72;
        c.registers.status=0x2700;
        c.registers.program_counter=0x1d5b4;
        c.registers.address[5]=record;
        c.registers.address[7]=0x7ffc;
        const auto result=host.run(c); // Real 340 -> 549, and optionally -> 364.
        const auto after=host.region_bytes(2);
        if (host.faulted() || result.status!=TranslationStatus::complete ||
            result.control!=1 || result.exit_program_counter!=0x123456 ||
            c.registers.program_counter!=0x123456 || c.registers.address[7]!=0x8000 ||
            after[record+0x41]!=(clears_step ? 0 : 0x20) ||
            after[record+0x36]!=(clears_step ? 77 : 3) ||
            after[record+0x40]!=(clears_step ? 0 : 2) ||
            after[record+0x1e]!=(clears_step ? 0x12 : 0) ||
            after[0x8000]!=0x87 || after[0x8003]!=0x21) {
            std::cerr << "state-step child return failed: clear=" << clears_step
                      << " pc=" << std::hex << c.registers.program_counter
                      << " sp=" << c.registers.address[7] << '\n';
            return 1;
        }
    }
    std::cout << "Both registered helper paths return through the caller with balanced stack\n";
}
