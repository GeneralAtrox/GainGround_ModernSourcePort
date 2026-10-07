// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

#include <optional>

namespace gain_ground::translated {
namespace {
struct CpuAProcessAssetLoadRequestStep {
    bool handled = false;
    std::optional<FunctionResult> result;
};
#include "cpu_a_process_asset_load_request_pc_080420_080460.inc" // gground-source-split
#include "cpu_a_process_asset_load_request_pc_080460_0804a0.inc" // gground-source-split
#include "cpu_a_process_asset_load_request_pc_0804a0_0804e0.inc" // gground-source-split
#include "cpu_a_process_asset_load_request_pc_0804e0_080520.inc" // gground-source-split
#include "cpu_a_process_asset_load_request_pc_080520_end.inc" // gground-source-split
}

FunctionResult cpu_a_process_asset_load_request(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 0U, 255U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        auto step = handle_cpu_a_process_asset_load_request_pc_080420_080460(c, r, m, pc, next, transfer_kind);
        if (!step.handled)
            step = handle_cpu_a_process_asset_load_request_pc_080460_0804a0(c, r, m, pc, next, transfer_kind);
        if (!step.handled)
            step = handle_cpu_a_process_asset_load_request_pc_0804a0_0804e0(c, r, m, pc, next, transfer_kind);
        if (!step.handled)
            step = handle_cpu_a_process_asset_load_request_pc_0804e0_080520(c, r, m, pc, next, transfer_kind);
        if (!step.handled)
            step = handle_cpu_a_process_asset_load_request_pc_080520_end(c, r, m, pc, next, transfer_kind);
        if (!step.handled)
            return {TranslationStatus::contract_violation, 0U, pc};
        if (step.result) return *step.result;
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x80414U:
        case 0x80418U:
        case 0x8041cU:
        case 0x80420U:
        case 0x80424U:
        case 0x80426U:
        case 0x8042aU:
        case 0x8042cU:
        case 0x8042eU:
        case 0x80430U:
        case 0x80436U:
        case 0x8043aU:
        case 0x8043cU:
        case 0x8043eU:
        case 0x80440U:
        case 0x80444U:
        case 0x80446U:
        case 0x80448U:
        case 0x8044cU:
        case 0x8044eU:
        case 0x80450U:
        case 0x80452U:
        case 0x80458U:
        case 0x8045cU:
        case 0x8045eU:
        case 0x80460U:
        case 0x80464U:
        case 0x80466U:
        case 0x8046aU:
        case 0x8046eU:
        case 0x80472U:
        case 0x80476U:
        case 0x80478U:
        case 0x8047aU:
        case 0x8047eU:
        case 0x80482U:
        case 0x80484U:
        case 0x80486U:
        case 0x80488U:
        case 0x8048cU:
        case 0x8048eU:
        case 0x80490U:
        case 0x80496U:
        case 0x80498U:
        case 0x8049cU:
        case 0x8049eU:
        case 0x804a0U:
        case 0x804a6U:
        case 0x804a8U:
        case 0x804acU:
        case 0x804b0U:
        case 0x804b4U:
        case 0x804b8U:
        case 0x804baU:
        case 0x804beU:
        case 0x804c0U:
        case 0x804c2U:
        case 0x804c4U:
        case 0x804caU:
        case 0x804ceU:
        case 0x804d2U:
        case 0x804d6U:
        case 0x804daU:
        case 0x804dcU:
        case 0x804deU:
        case 0x804e0U:
        case 0x804e2U:
        case 0x804e4U:
        case 0x804e6U:
        case 0x804e8U:
        case 0x804eaU:
        case 0x804ecU:
        case 0x804eeU:
        case 0x804f0U:
        case 0x804f2U:
        case 0x804f6U:
        case 0x804f8U:
        case 0x804faU:
        case 0x804fcU:
        case 0x804feU:
        case 0x80502U:
        case 0x80504U:
        case 0x80506U:
        case 0x80508U:
        case 0x8050cU:
        case 0x8050eU:
        case 0x80512U:
        case 0x80514U:
        case 0x80518U:
        case 0x8051cU:
        case 0x80520U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

} // namespace gain_ground::translated
