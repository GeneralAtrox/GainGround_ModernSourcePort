// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00020d6a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20d6aU: { // 6100ad20 bsr.w $1ba8c
            next = 0x20d6eU;
            const auto result = m.call(c, 562U, 0x20d6aU, 0x1ba8cU, 0x20d6eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20d6eU: { // 6100ce54 bsr.w $1dbc4
            next = 0x20d72U;
            const auto result = m.call(c, 352U, 0x20d6eU, 0x1dbc4U, 0x20d72U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20d72U: { // 6100ce7c bsr.w $1dbf0
            next = 0x20d76U;
            const auto result = m.call(c, 353U, 0x20d72U, 0x1dbf0U, 0x20d76U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20d76U: { // 6100d0b8 bsr.w $1de30
            next = 0x20d7aU;
            const auto result = m.call(c, 355U, 0x20d76U, 0x1de30U, 0x20d7aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20d7aU: { // 6100d372 bsr.w $1e0ee
            next = 0x20d7eU;
            const auto result = m.call(c, 356U, 0x20d7aU, 0x1e0eeU, 0x20d7eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20d7eU: { // 6100d3a4 bsr.w $1e124
            next = 0x20d82U;
            const auto result = m.call(c, 357U, 0x20d7eU, 0x1e124U, 0x20d82U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20d82U: { // 6100d3fc bsr.w $1e180
            next = 0x20d86U;
            const auto result = m.call(c, 359U, 0x20d82U, 0x1e180U, 0x20d86U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20d86U: { // 4e75 rts 
            next = 0x20d88U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20d86U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20d6aU:
        case 0x20d6eU:
        case 0x20d72U:
        case 0x20d76U:
        case 0x20d7aU:
        case 0x20d7eU:
        case 0x20d82U:
        case 0x20d86U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
