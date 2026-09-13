// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00020dee(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20deeU: { // 6100be76 bsr.w $1cc66
            next = 0x20df2U;
            const auto result = m.call(c, 332U, 0x20deeU, 0x1cc66U, 0x20df2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20df2U: { // 6100e408 bsr.w $1f1fc
            next = 0x20df6U;
            const auto result = m.call(c, 371U, 0x20df2U, 0x1f1fcU, 0x20df6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20df6U: { // 6100cdcc bsr.w $1dbc4
            next = 0x20dfaU;
            const auto result = m.call(c, 352U, 0x20df6U, 0x1dbc4U, 0x20dfaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20dfaU: { // 6100cdf4 bsr.w $1dbf0
            next = 0x20dfeU;
            const auto result = m.call(c, 353U, 0x20dfaU, 0x1dbf0U, 0x20dfeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20dfeU: { // 6100d030 bsr.w $1de30
            next = 0x20e02U;
            const auto result = m.call(c, 355U, 0x20dfeU, 0x1de30U, 0x20e02U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e02U: { // 6100d2ea bsr.w $1e0ee
            next = 0x20e06U;
            const auto result = m.call(c, 356U, 0x20e02U, 0x1e0eeU, 0x20e06U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e06U: { // 6100d31c bsr.w $1e124
            next = 0x20e0aU;
            const auto result = m.call(c, 357U, 0x20e06U, 0x1e124U, 0x20e0aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e0aU: { // 6100d4f8 bsr.w $1e304
            next = 0x20e0eU;
            const auto result = m.call(c, 360U, 0x20e0aU, 0x1e304U, 0x20e0eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e0eU: { // 4e75 rts 
            next = 0x20e10U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20e0eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20deeU:
        case 0x20df2U:
        case 0x20df6U:
        case 0x20dfaU:
        case 0x20dfeU:
        case 0x20e02U:
        case 0x20e06U:
        case 0x20e0aU:
        case 0x20e0eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
