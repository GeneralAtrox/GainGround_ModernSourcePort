// Implemented but unverified. Original F175 initials-entry and its reached
// helpers, translated from retained state-72 bytes. No new renderer or clock.
#include "cpu_b_initials_entry.h"
#include "unverified_cpu_b_machine.h"
#include "cpu_b_initials_entry_detail.h"

namespace gain_ground::translated {
FunctionResult cpu_b_initials_entry(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0xf0b4U: {
            next = 0xf0b8U; const auto child = m.call(c, 526U, pc, 0xf2a0U, 0xf0b8U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf100U: {
            next = 0xf104U; const auto child = m.call(c, 175U, pc, 0xf318U, 0xf104U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf104U: {
            next = 0xf108U; const auto child = m.call(c, 527U, pc, 0xf2b4U, 0xf108U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf108U: {
            next = 0xf10cU; const auto child = m.call(c, 175U, pc, 0xf38eU, 0xf10cU); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf18aU: {
            next = 0xf18eU; const auto child = m.call(c, 175U, pc, 0xf318U, 0xf18eU); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf18eU: {
            next = 0xf192U; const auto child = m.call(c, 175U, pc, 0xf2e6U, 0xf192U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf198U: {
            next = 0xf19cU; const auto child = m.call(c, 175U, pc, 0xf38eU, 0xf19cU); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf1b0U: {
            next = 0xf1b4U; const auto child = m.call(c, 175U, pc, 0xf318U, 0xf1b4U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf1b4U: {
            next = 0xf1b8U; const auto child = m.call(c, 175U, pc, 0xf2e6U, 0xf1b8U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf1beU: {
            next = 0xf1c2U; const auto child = m.call(c, 175U, pc, 0xf38eU, 0xf1c2U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf1e2U: {
            next = 0xf1e6U; const auto child = m.call(c, 527U, pc, 0xf2b4U, 0xf1e6U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf1e6U: {
            next = 0xf1eaU; const auto child = m.call(c, 175U, pc, 0xf35cU, 0xf1eaU); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf310U: {
            next = 0xf316U; const auto child = m.call(c, 298U, pc, 0x161eaU, 0xf316U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf3a0U: {
            next = 0xf3a4U; const auto child = m.call(c, 175U, pc, 0xf33eU, 0xf3a4U); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf3a6U: {
            next = 0xf3aaU; const auto child = m.call(c, 175U, pc, 0xf35cU, 0xf3aaU); if (child.status != TranslationStatus::complete || child.control != 1U) return child; next = r.program_counter;
            break;
        }
        case 0xf10cU:
        case 0xf19cU:
        case 0xf1c2U:
        case 0xf316U:
        case 0xf33cU:
        case 0xf35aU:
        case 0xf374U:
        case 0xf3a4U:
        case 0xf3aaU: {
            const auto result = m.ret();
            if (auto event = m.interrupt(c, pc, r.program_counter)) return *event;
            return result;
        }
        default:
            if (cpu_b_initials_entry_detail::dispatch_initials_region_01(
                    c, r, m, pc, next))
                break;
            if (cpu_b_initials_entry_detail::dispatch_initials_region_02(
                    c, r, m, pc, next))
                break;
            if (cpu_b_initials_entry_detail::dispatch_initials_region_03(
                    c, r, m, pc, next))
                break;
            if (cpu_b_initials_entry_detail::dispatch_initials_region_04(
                    c, r, m, pc, next))
                break;
            return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        // Original fallthrough/branch into the existing score-insertion tail.
        if (next == 0xf23eU || next == 0xf258U)
            return cpu_b_callback_state_table_dispatch(c);
    }
}
} // namespace gain_ground::translated
