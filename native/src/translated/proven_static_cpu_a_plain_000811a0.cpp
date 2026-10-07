// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"
#include "unverified_cpu_a_diagnostic_irq.h"

#include <optional>

namespace gain_ground::translated {
namespace {
struct CpuAPlain000811A0Step {
    bool handled = false;
    std::optional<FunctionResult> result;
};
#include "cpu_a_plain_000811a0_pc_0811a0_0811d0.inc" // gground-source-split
#include "cpu_a_plain_000811a0_pc_0811d0_081200.inc" // gground-source-split
#include "cpu_a_plain_000811a0_pc_081200_081300.inc" // gground-source-split
}

FunctionResult proven_static_cpu_a_plain_000811a0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 0U, 255U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        std::uint8_t instruction_kind = 0U;
        auto step = handle_cpu_a_plain_000811a0_pc_0811a0_0811d0(c, r, m, pc, next, transfer_kind, instruction_kind);
        if (!step.handled)
            step = handle_cpu_a_plain_000811a0_pc_0811d0_081200(c, r, m, pc, next, transfer_kind, instruction_kind);
        if (!step.handled)
            step = handle_cpu_a_plain_000811a0_pc_081200_081300(c, r, m, pc, next, transfer_kind, instruction_kind);
        if (!step.handled)
            return {TranslationStatus::contract_violation, 0U, pc};
        if (step.result) return *step.result;
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = unverified::diagnostic_irq3(c, m, pc, next, instruction_kind)) return *event;
        switch (next) {
        case 0x811a0U:
        case 0x811a2U:
        case 0x811a4U:
        case 0x811a6U:
        case 0x811a8U:
        case 0x811aaU:
        case 0x811b0U:
        case 0x811b2U:
        case 0x811b4U:
        case 0x811b6U:
        case 0x811b8U:
        case 0x811bcU:
        case 0x811c0U:
        case 0x811c2U:
        case 0x811c4U:
        case 0x811c6U:
        case 0x811caU:
        case 0x811ccU:
        case 0x811ceU:
        case 0x811d0U:
        case 0x811d4U:
        case 0x811d6U:
        case 0x811daU:
        case 0x811deU:
        case 0x811e0U:
        case 0x811e2U:
        case 0x811e6U:
        case 0x811e8U:
        case 0x811eaU:
        case 0x811ecU:
        case 0x811eeU:
        case 0x811f0U:
        case 0x811f2U:
        case 0x811f6U:
        case 0x811f8U:
        case 0x811fcU:
        case 0x81202U:
        case 0x81206U:
        case 0x81208U:
        case 0x8120cU:
        case 0x8120eU:
        case 0x81210U:
        case 0x81212U:
        case 0x81216U:
        case 0x81218U:
        case 0x8121aU:
        case 0x8121cU:
        case 0x8121eU:
        case 0x81222U:
        case 0x81224U:
        case 0x81226U:
        case 0x81228U:
        case 0x8122aU:
        case 0x8122eU:
        case 0x81230U:
        case 0x81234U:
        case 0x8123aU:
        case 0x8123eU:
        case 0x81242U:
        case 0x81248U:
        case 0x8124cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

} // namespace gain_ground::translated
