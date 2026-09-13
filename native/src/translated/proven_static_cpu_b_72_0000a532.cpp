// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0000a532(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xa532U: { // 4eb900017054 jsr $17054.l
            next = 0xa538U;
            const auto result = m.call(c, 311U, 0xa532U, 0x17054U, 0xa538U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa538U: { // 4eb9000085ac jsr $85ac.l
            next = 0xa53eU;
            const auto result = m.call(c, 117U, 0xa538U, 0x85acU, 0xa53eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa53eU: { // 4eb9000085f2 jsr $85f2.l
            next = 0xa544U;
            const auto result = m.call(c, 121U, 0xa53eU, 0x85f2U, 0xa544U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa544U: { // 4eb900008622 jsr $8622.l
            next = 0xa54aU;
            const auto result = m.call(c, 122U, 0xa544U, 0x8622U, 0xa54aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa54aU: { // 4eb900008654 jsr $8654.l
            next = 0xa550U;
            const auto result = m.call(c, 123U, 0xa54aU, 0x8654U, 0xa550U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa550U: { // 61000016 bsr.w $a568
            next = 0xa554U;
            const auto result = m.call(c, 138U, 0xa550U, 0xa568U, 0xa554U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa554U: { // 6100005a bsr.w $a5b0
            next = 0xa558U;
            const auto result = m.call(c, 139U, 0xa554U, 0xa5b0U, 0xa558U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa558U: { // 610000be bsr.w $a618
            next = 0xa55cU;
            const auto result = m.call(c, 140U, 0xa558U, 0xa618U, 0xa55cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa55cU: { // 427900404018 clr.w $404018.l
            next = 0xa562U;
            const auto destination_address = 0x404018U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xa562U: { // 4ef900008572 jmp $8572.l
            next = 0xa568U;
            next = 0x8572U;
            transfer_kind = 1U;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0x8572U) return c.host->call_function(116U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0xa532U:
        case 0xa538U:
        case 0xa53eU:
        case 0xa544U:
        case 0xa54aU:
        case 0xa550U:
        case 0xa554U:
        case 0xa558U:
        case 0xa55cU:
        case 0xa562U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
