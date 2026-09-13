#include "gain_ground/system24_devices.h"
#include <iostream>
#include <stdexcept>

// Reference: segas24_state::irq_timer_sync/start/irq_w. Register writes
// synchronize the counter and schedule count * period from the exact write time.
int main() { try {
    for (unsigned mode : {1U, 3U}) for (unsigned phase : {100U, 1700U, 6900U, 20900U}) {
        gain_ground::System24Devices d;
        const auto require=[](bool ok,const char *why) { if(!ok) throw std::runtime_error(why); };
        const std::uint64_t period=mode==1 ? 41000U : 125U;
        const std::uint64_t start=100000U+phase;
        d.write(0xa00004,4,65535);
        d.write(0xa00000,3901,65535);
        d.advance(start);
        d.write(0xa00002,mode,65535);
        auto expiry=start+4096U*period;
        d.advance(expiry-1);
        require(d.irq_level(0)==0,"timer fired before its exact start-relative deadline");
        require(d.next_event_ns()==expiry,"scheduler did not expose timer deadline");
        d.advance(expiry);
        require(d.irq_level(0)==3,"timer did not assert at expiry");
        d.write(0xa00004,4,65535);
        const auto write_time=expiry+3U*period+37U;
        d.advance(write_time);
        const auto counter=3901U+write_time/period-expiry/period;
        require(*d.read(0xa00000,65535)==counter,"counter sync differs");
        d.write(0xa00000,3901,65535); // Same value must still re-arm.
        expiry=write_time+(4096U-counter)*period;
        d.advance(expiry-1);
        require(d.irq_level(0)==0,"same-value reload did not move expiry");
        d.advance(expiry);
        require(d.irq_level(0)==3,"re-armed timer did not fire");
        d.write(0xa00004,4,65535);
        d.write(0xa00002,0,65535);
        d.advance(expiry+20000000);
        require(d.irq_level(0)==0,"cancelled timer fired");
    }
    std::cout << "PASS: System 24 exact timer deadlines and unchanged reload writes\n";
} catch(const std::exception &e) { std::cerr << e.what() << '\n'; return 1; } }
