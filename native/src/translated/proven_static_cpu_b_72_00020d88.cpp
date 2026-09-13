// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00020d88(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20d88U: { // 6100ada2 bsr.w $1bb2c
            next = 0x20d8cU;
            const auto result = m.call(c, 564U, 0x20d88U, 0x1bb2cU, 0x20d8cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20d8cU: { // 6100ce36 bsr.w $1dbc4
            next = 0x20d90U;
            const auto result = m.call(c, 352U, 0x20d8cU, 0x1dbc4U, 0x20d90U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20d90U: { // 6100ce5e bsr.w $1dbf0
            next = 0x20d94U;
            const auto result = m.call(c, 353U, 0x20d90U, 0x1dbf0U, 0x20d94U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20d94U: { // 6100d09a bsr.w $1de30
            next = 0x20d98U;
            const auto result = m.call(c, 355U, 0x20d94U, 0x1de30U, 0x20d98U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20d98U: { // 6100d354 bsr.w $1e0ee
            next = 0x20d9cU;
            const auto result = m.call(c, 356U, 0x20d98U, 0x1e0eeU, 0x20d9cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20d9cU: { // 6100d386 bsr.w $1e124
            next = 0x20da0U;
            const auto result = m.call(c, 357U, 0x20d9cU, 0x1e124U, 0x20da0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20da0U: { // 6100d3de bsr.w $1e180
            next = 0x20da4U;
            const auto result = m.call(c, 359U, 0x20da0U, 0x1e180U, 0x20da4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20da4U: { // 4e75 rts 
            next = 0x20da6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20da4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20d88U:
        case 0x20d8cU:
        case 0x20d90U:
        case 0x20d94U:
        case 0x20d98U:
        case 0x20d9cU:
        case 0x20da0U:
        case 0x20da4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
