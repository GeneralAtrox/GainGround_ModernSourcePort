// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

#include <optional>

namespace gain_ground::translated {
namespace {
struct CpuAFdcStep {
    bool handled = false;
    std::optional<FunctionResult> result;
};
#include "cpu_a_dynamic_fdc_request_sample_and_idle_return.inc" // gground-source-split
#include "cpu_a_dynamic_fdc_request_execute_and_complete.inc" // gground-source-split
}

FunctionResult cpu_a_dynamic_fdc_service(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 0U, 255U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        auto step = handle_cpu_a_dynamic_fdc_request_sample_and_idle_return(
            c, r, m, pc, next, transfer_kind);
        if (!step.handled)
            step = handle_cpu_a_dynamic_fdc_request_execute_and_complete(
                c, r, m, pc, next, transfer_kind);
        if (!step.handled)
            return {TranslationStatus::contract_violation, 0U, pc};
        if (step.result) return *step.result;
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0x800c0U) return c.host->call_function(54U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0x80356U:
        case 0x8035cU:
        case 0x80360U:
        case 0x80362U:
        case 0x80368U:
        case 0x8036aU:
        case 0x8036eU:
        case 0x80374U:
        case 0x80378U:
        case 0x8037aU:
        case 0x8037cU:
        case 0x80380U:
        case 0x80386U:
        case 0x8038cU:
        case 0x8038eU:
        case 0x80390U:
        case 0x80392U:
        case 0x80396U:
        case 0x8039aU:
        case 0x8039eU:
        case 0x803a0U:
        case 0x803a2U:
        case 0x803a6U:
        case 0x803a8U:
        case 0x803acU:
        case 0x803b0U:
        case 0x803b4U:
        case 0x803b8U:
        case 0x803bcU:
        case 0x803beU:
        case 0x803c2U:
        case 0x803c6U:
        case 0x803caU:
        case 0x803ccU:
        case 0x803ceU:
        case 0x803d2U:
        case 0x803d4U:
        case 0x803d8U:
        case 0x803daU:
        case 0x803dcU:
        case 0x803deU:
        case 0x803e2U:
        case 0x803e4U:
        case 0x803e6U:
        case 0x803e8U:
        case 0x803eaU:
        case 0x803ecU:
        case 0x803eeU:
        case 0x803f2U:
        case 0x803f4U:
        case 0x803f6U:
        case 0x803faU:
        case 0x803feU:
        case 0x80402U:
        case 0x80406U:
        case 0x8040aU:
        case 0x8040eU:
        case 0x80412U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
} // namespace gain_ground::translated
