#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_devices.h"

#include <iostream>
#include <vector>

using namespace gain_ground;

struct WaitProbe {
    RuntimeHost host;
    System24Devices devices;
    unsigned waits{};
    unsigned interrupt_entries{};
    bool asserted{};

    static void checkpoint(void *argument)
    {
        auto &probe = *static_cast<WaitProbe *>(argument);
        const auto deadline = probe.host.next_cpu_deadline_ns();
        if (deadline != UINT64_MAX)
            probe.devices.advance(deadline);
        const auto *context = probe.host.active_context();
        // Drive an interrupt input after three real BPL iterations. The actual
        // 101 -> 105 handler releases the countdown; no child result is supplied.
        if (deadline == UINT64_MAX && probe.host.waiting_for_device() && context &&
            context->state == 0x72U && context->registers.program_counter == 0x85b0U &&
            ++probe.waits == 3U) {
            probe.host.set_irq_line(1U, 5U, true);
            probe.asserted = true;
        }
        if (probe.asserted && context && context->state == 0x04U) {
            ++probe.interrupt_entries;
            probe.host.set_irq_line(1U, 5U, false);
            probe.asserted = false;
        }
    }
};

int main()
{
    // F115's BSR at 8552 and F116's BSR at 8576 share this actual child path.
    for (const auto return_pc : {0x8556U, 0x857aU}) {
        WaitProbe probe;
        probe.host.attach_devices(probe.devices);
        probe.host.select_cpu(1U);
        std::vector<std::uint8_t> ram(probe.host.region_bytes(2U).size());
        ram[0x76] = 0x80U;
        ram[0x77] = 0xa2U; // Original IRQ5 vector -> state-04 entry 101.
        ram[0x7002] = static_cast<std::uint8_t>(return_pc >> 8U);
        ram[0x7003] = static_cast<std::uint8_t>(return_pc);
        ram[0x7004] = 0xa5U;
        ram[0x7005] = 0x5aU;
        if (!probe.host.load_region(2U, 0U, ram)) return 2;
        // IRQ5 reads the shared audio-work flag; keep that independent path idle.
        const std::vector<std::uint8_t> shared(probe.host.region_bytes(3U).size());
        if (!probe.host.load_region(3U, 0U, shared)) return 2;
        probe.host.set_checkpoint(WaitProbe::checkpoint, &probe);
        FunctionContext context{};
        context.host = &probe.host;
        context.cpu = 1U;
        context.state = 0x72U;
        auto &registers = context.registers;
        for (unsigned i = 0; i < 8U; ++i) registers.data[i] = 0x12340000U + i;
        for (unsigned i = 0; i < 7U; ++i) registers.address[i] = 0x56780000U + i;
        registers.address[7] = 0x7000U;
        registers.program_counter = 0x85b0U;
        registers.status = 0x2015U;
        const auto before = registers;
        const auto result = probe.host.call_function(118U, 1U, 0x72U, 2U,
            return_pc - 4U, 0x85b0U, context);
        const auto after = probe.host.region_bytes(2U);
        bool retained_addresses = true;
        for (unsigned i = 0; i < 7U; ++i)
            retained_addresses &= registers.address[i] == before.address[i];
        if (probe.host.faulted() || result.status != TranslationStatus::complete ||
            result.control != 1U || result.exit_program_counter != return_pc ||
            registers.program_counter != return_pc || registers.address[7] != 0x7004U ||
            registers.status != 0x2010U || context.state != 0x72U ||
            registers.data != before.data || !retained_addresses ||
            probe.waits != 3U || probe.interrupt_entries != 1U || probe.asserted ||
            after[0x502] != 1U || after[0x6c00] != 0x80U || after[0x6c01] != 0U ||
            after[0x7004] != 0xa5U || after[0x7005] != 0x5aU ||
            probe.host.timed_execution()) {
            std::cerr << "Real frame-wait child failed: pc=" << std::hex
                      << registers.program_counter << " sp=" << registers.address[7]
                      << " sr=" << registers.status << " waits=" << std::dec << probe.waits
                      << " irqs=" << probe.interrupt_entries << " fault="
                      << probe.host.fault().message << '\n';
            return 1;
        }
    }
    std::cout << "Real F118 -> IRQ5 children -> RTE -> RTS preserve both caller returns, registers, FD1094 state and one frame commit\n";
}
