// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00020da6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20da6U: { // 6100c062 bsr.w $1ce0a
            next = 0x20daaU;
            const auto result = m.call(c, 333U, 0x20da6U, 0x1ce0aU, 0x20daaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20daaU: { // 6100d342 bsr.w $1e0ee
            next = 0x20daeU;
            const auto result = m.call(c, 356U, 0x20daaU, 0x1e0eeU, 0x20daeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20daeU: { // 6100e44c bsr.w $1f1fc
            next = 0x20db2U;
            const auto result = m.call(c, 371U, 0x20daeU, 0x1f1fcU, 0x20db2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20db2U: { // 6100ce10 bsr.w $1dbc4
            next = 0x20db6U;
            const auto result = m.call(c, 352U, 0x20db2U, 0x1dbc4U, 0x20db6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20db6U: { // 6100ce38 bsr.w $1dbf0
            next = 0x20dbaU;
            const auto result = m.call(c, 353U, 0x20db6U, 0x1dbf0U, 0x20dbaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20dbaU: { // 6100d074 bsr.w $1de30
            next = 0x20dbeU;
            const auto result = m.call(c, 355U, 0x20dbaU, 0x1de30U, 0x20dbeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20dbeU: { // 6100d364 bsr.w $1e124
            next = 0x20dc2U;
            const auto result = m.call(c, 357U, 0x20dbeU, 0x1e124U, 0x20dc2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20dc2U: { // 6100d678 bsr.w $1e43c
            next = 0x20dc6U;
            const auto result = m.call(c, 362U, 0x20dc2U, 0x1e43cU, 0x20dc6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20dc6U: { // 4e75 rts 
            next = 0x20dc8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20dc6U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20da6U:
        case 0x20daaU:
        case 0x20daeU:
        case 0x20db2U:
        case 0x20db6U:
        case 0x20dbaU:
        case 0x20dbeU:
        case 0x20dc2U:
        case 0x20dc6U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
