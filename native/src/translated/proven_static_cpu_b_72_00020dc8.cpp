// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00020dc8(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20dc8U: { // 6100b5b4 bsr.w $1c37e
            next = 0x20dccU;
            const auto result = m.call(c, 331U, 0x20dc8U, 0x1c37eU, 0x20dccU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20dccU: { // 6100d320 bsr.w $1e0ee
            next = 0x20dd0U;
            const auto result = m.call(c, 356U, 0x20dccU, 0x1e0eeU, 0x20dd0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20dd0U: { // 6100e42a bsr.w $1f1fc
            next = 0x20dd4U;
            const auto result = m.call(c, 371U, 0x20dd0U, 0x1f1fcU, 0x20dd4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20dd4U: { // 6100cdee bsr.w $1dbc4
            next = 0x20dd8U;
            const auto result = m.call(c, 352U, 0x20dd4U, 0x1dbc4U, 0x20dd8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20dd8U: { // 6100ce16 bsr.w $1dbf0
            next = 0x20ddcU;
            const auto result = m.call(c, 353U, 0x20dd8U, 0x1dbf0U, 0x20ddcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20ddcU: { // 6100d052 bsr.w $1de30
            next = 0x20de0U;
            const auto result = m.call(c, 355U, 0x20ddcU, 0x1de30U, 0x20de0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20de0U: { // 6100d342 bsr.w $1e124
            next = 0x20de4U;
            const auto result = m.call(c, 357U, 0x20de0U, 0x1e124U, 0x20de4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20de4U: { // 6100d51e bsr.w $1e304
            next = 0x20de8U;
            const auto result = m.call(c, 360U, 0x20de4U, 0x1e304U, 0x20de8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20de8U: { // 4e75 rts 
            next = 0x20deaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20de8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20dc8U:
        case 0x20dccU:
        case 0x20dd0U:
        case 0x20dd4U:
        case 0x20dd8U:
        case 0x20ddcU:
        case 0x20de0U:
        case 0x20de4U:
        case 0x20de8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
