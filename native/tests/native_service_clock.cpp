#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_devices.h"
#include <iostream>
using namespace gain_ground;
int main() {
    RuntimeHost host;
    System24Devices devices;
    host.attach_devices(devices);
    host.set_slice_end(1'000'000'000U); // Legacy options cannot restore a second clock.
    host.set_native_services([](void *) {}, nullptr);
    devices.use_native_events();
    devices.write(0xa00004, 0x18, 0xffff);
    devices.write(0xa00006, 0x18, 0xffff);
    host.wait_until_time(40U * 41000U); // First sprite event.
    host.select_cpu(1);
    host.wait_until_time(42U * 41000U); // Serial work outlives the pulse.
    if (host.execution_time_ns() != 42U * 41000U || devices.irq_level(0) != 5 || devices.irq_level(1) != 5)
        return 1;
    devices.acknowledge_native_event(0, 5);
    if (devices.irq_level(0) != 0 || devices.irq_level(1) != 5) return 1;
    devices.acknowledge_native_event(1, 5);
    host.select_cpu(0);
    host.wait_until_time(10U); // A service cannot rewind the one shared clock.
    if (host.execution_time_ns() != 42U * 41000U) return 1;
    host.wait_until_time(426U * 41000U);
    if (devices.irq_level(0) != 4 || devices.irq_level(1) != 4) return 1;
    devices.acknowledge_native_event(0, 4); devices.acknowledge_native_event(1, 4);
    if (devices.irq_level(0) || devices.irq_level(1)) return 1;
    for (unsigned lane : {0U, 1U}) {
        host.select_cpu(lane);
        FunctionContext c{}; c.host = &host; c.cpu = static_cast<std::uint8_t>(lane);
        c.state = lane ? 0x72 : 0xff;
        c.registers.program_counter = lane ? 0x8572 : 0x80104;
        c.registers.address[7] = 0x1234;
        const auto result = host.run(c);
        if (host.faulted() || result.status != TranslationStatus::complete || result.control != 10 ||
            result.exit_program_counter != c.registers.program_counter || c.registers.address[7] != 0x1234)
            return 1;
    }
    std::cout << "Single clock, retained/acknowledged events and native loop ownership PASS\n";
}
