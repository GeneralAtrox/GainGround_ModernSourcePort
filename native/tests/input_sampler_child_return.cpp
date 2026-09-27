#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_devices.h"

#include <array>
#include <iostream>
#include <vector>

using namespace gain_ground;

namespace {
struct Probe {
    RuntimeHost host;
    System24Devices devices;
    std::vector<std::uint32_t> samples, returns;
    std::array<unsigned, 4> stores{};
    bool invalid{};

    static void checkpoint(void *argument)
    {
        auto &probe = *static_cast<Probe *>(argument);
        const auto deadline = probe.host.next_cpu_deadline_ns();
        if (deadline != UINT64_MAX) probe.devices.advance(deadline);
    }

    static void observe(void *argument, const TimingTraceEvent &event)
    {
        auto &probe = *static_cast<Probe *>(argument);
        // These existing observations mark instruction entry, not a complete
        // bus/timing trace. They count actual executions of the real children.
        if (event.kind != "unsupported") return;
        const auto *context = probe.host.active_context();
        if (!context) { probe.invalid = true; return; }
        const auto &r = context->registers;
        if (event.pc == 0x85d0U) {
            const auto n = probe.samples.size();
            const std::array<std::uint32_t, 4> addresses{0x800000U, 0x800002U, 0x800004U, 0x800008U};
            probe.invalid |= n >= addresses.size();
            if (n < addresses.size())
                probe.invalid |= r.address[1] != addresses[n] || r.address[0] != 0x800U + 4U * n ||
                    r.address[7] != (n == 3U ? 0x7004U : 0x7000U);
            probe.samples.push_back(r.address[1]);
        }
        if (event.pc == 0x85f0U) {
            const auto n = probe.returns.size();
            const auto bytes = probe.host.region_bytes(2U);
            const auto sp = r.address[7];
            if (sp + 3U >= bytes.size()) { probe.invalid = true; return; }
            const auto target = (std::uint32_t(bytes[sp]) << 24U) |
                (std::uint32_t(bytes[sp + 1U]) << 16U) |
                (std::uint32_t(bytes[sp + 2U]) << 8U) | bytes[sp + 3U];
            const std::array<std::uint32_t, 4> expected{0x85caU, 0x85ccU, 0x85ceU, 0x8580U};
            probe.invalid |= n >= expected.size();
            if (n < expected.size()) probe.invalid |= target != expected[n];
            probe.returns.push_back(target);
        }
        const std::array<std::uint32_t, 4> sites{0x85d6U, 0x85dcU, 0x85e2U, 0x85eaU};
        for (unsigned i = 0; i < sites.size(); ++i)
            if (event.pc == sites[i]) ++probe.stores[i];
    }
};
}

int main()
{
    struct Case { std::array<std::uint8_t, 4> previous, current; };
    const std::array<Case, 4> cases{{
        {{0x00U, 0xffU, 0x55U, 0xaaU}, {0xffU, 0x00U, 0xaaU, 0x55U}},
        {{0x81U, 0x42U, 0x24U, 0x18U}, {0x81U, 0x42U, 0x24U, 0x18U}},
        {{0U, 0U, 0U, 0U}, {0U, 0U, 0U, 0U}},
        {{0U, 0U, 0U, 0U}, {1U, 2U, 4U, 0x80U}},
    }};
    for (const auto &test : cases) {
        Probe probe;
        probe.host.attach_devices(probe.devices);
        probe.host.select_cpu(1U);
        std::vector<std::uint8_t> ram(probe.host.region_bytes(2U).size());
        for (unsigned i = 0; i < 4U; ++i) {
            ram[0x801U + 4U * i] = test.previous[i];
            probe.devices.input(i == 3U ? 4U : i, test.current[i], true);
        }
        // Port 3 must be skipped before the fourth sample at port 4.
        probe.devices.input(3U, 0x5aU, true);
        ram[0x6ffeU] = 0xa5U; ram[0x6fffU] = 0x5aU;
        ram[0x7006U] = 0x85U; ram[0x7007U] = 0x80U;
        ram[0x7008U] = 0x5aU; ram[0x7009U] = 0xa5U;
        ram[0x7ffU] = 0x96U; ram[0x810U] = 0x69U;
        if (!probe.host.load_region(2U, 0U, ram)) return 2;
        probe.host.set_checkpoint(Probe::checkpoint, &probe);
        probe.host.set_timing_observer(Probe::observe, &probe);
        FunctionContext context{};
        context.host = &probe.host; context.cpu = 1U; context.state = 0x72U;
        auto &r = context.registers;
        for (unsigned i = 0; i < 8U; ++i) r.data[i] = 0x12340000U + 0x1100U * i + i;
        for (unsigned i = 0; i < 7U; ++i) r.address[i] = 0x56780000U + i;
        r.address[7] = 0x7004U; r.program_counter = 0x85beU; r.status = 0x271fU;
        const auto before = r;
        const auto result = probe.host.call_function(119U, 1U, 0x72U, 2U, 0x857aU, 0x85beU, context);
        const auto after = probe.host.region_bytes(2U);
        bool good = !probe.host.faulted() && !probe.invalid && !probe.host.timed_execution() &&
            result.status == TranslationStatus::complete && result.control == 1U &&
            result.exit_program_counter == 0x8580U && r.program_counter == 0x8580U &&
            r.address[7] == 0x7008U && r.address[0] == 0x810U && r.address[1] == 0x80000aU &&
            context.cpu == 1U && context.state == 0x72U && probe.samples.size() == 4U &&
            probe.returns.size() == 4U && probe.stores == std::array<unsigned, 4>{4U, 4U, 4U, 4U};
        for (unsigned i = 3U; i < 8U; ++i) good &= r.data[i] == before.data[i];
        for (unsigned i = 2U; i < 7U; ++i) good &= r.address[i] == before.address[i];
        for (unsigned i = 0; i < 4U; ++i) {
            const auto old = test.previous[i], now = test.current[i];
            good &= after[0x800U + 4U * i] == old && after[0x801U + 4U * i] == now &&
                after[0x802U + 4U * i] == static_cast<std::uint8_t>(now & ~old) &&
                after[0x803U + 4U * i] == static_cast<std::uint8_t>(old & ~now);
        }
        const auto pressed = static_cast<std::uint8_t>(test.current[3] & ~test.previous[3]);
        good &= r.data[0] == ((before.data[0] & 0xffff0000U) | pressed) &&
            r.data[1] == ((before.data[1] & 0xffffff00U) | static_cast<std::uint8_t>(~test.previous[3])) &&
            r.data[2] == ((before.data[2] & 0xffffff00U) | static_cast<std::uint8_t>(test.previous[3] & ~test.current[3])) &&
            r.status == (0x2710U | (pressed == 0U ? 4U : 0U) | (pressed & 0x80U ? 8U : 0U));
        for (const auto offset : {0x6ffeU, 0x6fffU, 0x7004U, 0x7005U, 0x7006U, 0x7007U,
                                  0x7008U, 0x7009U, 0x7ffU, 0x810U})
            good &= after[offset] == ram[offset];
        if (!good) {
            std::cerr << "Real input sampler return/state failed: pc=" << std::hex << r.program_counter
                      << " sp=" << r.address[7] << " sr=" << r.status << " samples=" << std::dec
                      << probe.samples.size() << " returns=" << probe.returns.size()
                      << " d0=" << std::hex << r.data[0] << " expected-d0="
                      << ((before.data[0] & 0xffff0000U) | pressed)
                      << " invalid=" << probe.invalid << " fault=" << probe.host.fault().message << '\n';
            return 1;
        }
    }
    // The original 8-bit I/O handler occupies only the low lane, including
    // mirrored addresses and non-input registers. The other lane reads zero.
    Probe mapping;
    mapping.devices.input(0U, 0x81U, true);
    mapping.devices.write(0x80001eU, 1U, 0x00ffU);
    mapping.devices.write(0x800000U, 0x5aU, 0x00ffU);
    for (unsigned reg = 0; reg < 32U; ++reg) {
        const auto low = mapping.devices.read(0x800000U + 2U * reg, 0x00ffU);
        if (!low || mapping.devices.read(0x800000U + 2U * reg, 0xffffU) != low ||
            mapping.devices.read(0x9ffe00U + 2U * reg, 0xffffU) != low ||
            mapping.devices.read(0x800000U + 2U * reg, 0xff00U) != 0U ||
            mapping.devices.read(0x800000U + 2U * reg, 0x000fU) != (*low & 0x000fU)) {
            std::cerr << "I/O read lane or mirror differs at register " << reg << '\n';
            return 1;
        }
    }
    std::cout << "Four real F119/F120 input cases preserve transitions, four child executions, outer RTS, registers and stack guards\n";
}
