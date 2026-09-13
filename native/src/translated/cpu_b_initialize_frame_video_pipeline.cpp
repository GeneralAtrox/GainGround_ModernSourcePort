// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult cpu_b_initialize_frame_video_pipeline(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xa618U: { // 4eb900017054 jsr $17054.l
            next = 0xa61eU;
            const auto result = m.call(c, 311U, 0xa618U, 0x17054U, 0xa61eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa61eU: { // 33fc00010040401a move.w #$1, $40401a.l
            next = 0xa626U;
            const auto value = 0x1U;
            const auto destination_address = 0x40401aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xa626U: { // 4eb9000085ac jsr $85ac.l
            next = 0xa62cU;
            const auto result = m.call(c, 117U, 0xa626U, 0x85acU, 0xa62cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa62cU: { // 4eb900008622 jsr $8622.l
            next = 0xa632U;
            const auto result = m.call(c, 122U, 0xa62cU, 0x8622U, 0xa632U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa632U: { // 4eb900008654 jsr $8654.l
            next = 0xa638U;
            const auto result = m.call(c, 123U, 0xa632U, 0x8654U, 0xa638U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa638U: { // 61000022 bsr.w $a65c
            next = 0xa63cU;
            const auto result = m.call(c, 141U, 0xa638U, 0xa65cU, 0xa63cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa63cU: { // 4eb90001826e jsr $1826e.l
            next = 0xa642U;
            const auto result = m.call(c, 322U, 0xa63cU, 0x1826eU, 0xa642U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa642U: { // 61000130 bsr.w $a774
            next = 0xa646U;
            const auto result = m.call(c, 142U, 0xa642U, 0xa774U, 0xa646U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa646U: { // 61000182 bsr.w $a7ca
            next = 0xa64aU;
            const auto result = m.call(c, 143U, 0xa646U, 0xa7caU, 0xa64aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa64aU: { // 33fc000200404018 move.w #$2, $404018.l
            next = 0xa652U;
            const auto value = 0x2U;
            const auto destination_address = 0x404018U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xa652U: { // 13f804190040401b move.b $419.w, $40401b.l
            next = 0xa65aU;
            const auto source_address = 0x419U;
            const auto value = m.byte(source_address);
            const auto destination_address = 0x40401bU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xa65aU: { // 4e75 rts 
            next = 0xa65cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xa65aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0xa618U:
        case 0xa61eU:
        case 0xa626U:
        case 0xa62cU:
        case 0xa632U:
        case 0xa638U:
        case 0xa63cU:
        case 0xa642U:
        case 0xa646U:
        case 0xa64aU:
        case 0xa652U:
        case 0xa65aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
