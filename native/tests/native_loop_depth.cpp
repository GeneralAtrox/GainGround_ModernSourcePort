#include "gain_ground/runtime_host.h"
#include <iostream>
#include <vector>
using namespace gain_ground;
int main() {
    for (unsigned count : {0U, 1U, 512U, 511U, 0xff00U, 0xffffU}) {
        RuntimeHost host;
        std::vector<std::uint8_t> ram(host.region_bytes(2).size());
        for (unsigned actor = 0x5400; actor < 0x6400; actor += 0x80) ram[actor] = 0x80;
        ram[0x7ff0] = 0; ram[0x7ff1] = 0; ram[0x7ff2] = 0x90; ram[0x7ff3] = 0;
        ram[0x7ff4] = 0xa5; ram[0x7ff5] = 0x5a;
        if (!host.load_region(2, 0, ram)) return 2;
        host.select_cpu(1);
        FunctionContext c{}; c.host = &host; c.cpu = 1; c.state = 0x72;
        auto &r = c.registers;
        r.program_counter = 0x1f0da; r.status = 0x2000; r.address[7] = 0x7ff0;
        r.data[4] = 0x12340000U | count; r.data[5] = 0xabcd1234; r.data[6] = 0x42;
        r.data[7] = 0x56780021;
        const auto result = host.run(c);
        const auto expected = (0x1234U - 0x21U * (count + 1U)) & 0x7ffU;
        const bool pass = !host.faulted() && result.status == TranslationStatus::complete && result.control == 1 &&
            r.program_counter == 0x9000 && r.address[7] == 0x7ff4 && r.data[4] == 0x1234ffff &&
            r.data[5] == (0xabcd0000U | expected) && r.data[6] == 0x00210042 &&
            r.data[7] == 0x56780021 && host.max_call_depth(1) < 8 &&
            host.region_bytes(2)[0x7ff4] == 0xa5 && host.region_bytes(2)[0x7ff5] == 0x5a;
        std::cout << "loop count=" << count << " depth=" << host.max_call_depth(1) << " pass=" << pass
            << " fault=" << host.fault().message << '\n';
        if (!pass) return 1;
    }
}
