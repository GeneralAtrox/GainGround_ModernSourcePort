// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0000a97c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xa97cU: { // 43fa039e lea.l $ad1c(pc), a1
            next = 0xa980U;
            const auto source_address = 0xad1cU;
            r.address[1] = source_address;
            break;
        }
        case 0xa980U: { // 4eb900015fb8 jsr $15fb8.l
            next = 0xa986U;
            const auto result = m.call(c, 638U, 0xa980U, 0x15fb8U, 0xa986U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa986U: { // 43fa03d0 lea.l $ad58(pc), a1
            next = 0xa98aU;
            const auto source_address = 0xad58U;
            r.address[1] = source_address;
            break;
        }
        case 0xa98aU: { // 4eb900015fb8 jsr $15fb8.l
            next = 0xa990U;
            const auto result = m.call(c, 638U, 0xa98aU, 0x15fb8U, 0xa990U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa990U: { // 43fa0414 lea.l $ada6(pc), a1
            next = 0xa994U;
            const auto source_address = 0xada6U;
            r.address[1] = source_address;
            break;
        }
        case 0xa994U: { // 4eb900015fb8 jsr $15fb8.l
            next = 0xa99aU;
            const auto result = m.call(c, 638U, 0xa994U, 0x15fb8U, 0xa99aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa99aU: { // 4eb900015e28 jsr $15e28.l
            next = 0xa9a0U;
            const auto result = m.call(c, 283U, 0xa99aU, 0x15e28U, 0xa9a0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa9a0U: { // 7200 moveq #$0, d1
            next = 0xa9a2U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xa9a2U: { // 04403334 subi.w #$3334, d0
            next = 0xa9a6U;
            const auto source_value = 0x3334U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xa9a6U: { // 6504 bcs.b $a9ac
            next = 0xa9a8U;
            if ((r.status & 1U) != 0U) { next = 0xa9acU; transfer_kind = 1U; }
            break;
        }
        case 0xa9a8U: { // 5841 addq.w #$4, d1
            next = 0xa9aaU;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0xa9aaU: { // 60f6 bra.b $a9a2
            next = 0xa9acU;
            if (true) { next = 0xa9a2U; transfer_kind = 1U; }
            break;
        }
        case 0xa9acU: { // b2780c42 cmp.w $c42.w, d1
            next = 0xa9b0U;
            const auto source_address = 0xc42U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xa9b0U: { // 660a bne.b $a9bc
            next = 0xa9b2U;
            if ((r.status & 4U) == 0U) { next = 0xa9bcU; transfer_kind = 1U; }
            break;
        }
        case 0xa9b2U: { // 5841 addq.w #$4, d1
            next = 0xa9b4U;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0xa9b4U: { // 0c410014 cmpi.w #$14, d1
            next = 0xa9b8U;
            const auto source_value = 0x14U;
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xa9b8U: { // 6502 bcs.b $a9bc
            next = 0xa9baU;
            if ((r.status & 1U) != 0U) { next = 0xa9bcU; transfer_kind = 1U; }
            break;
        }
        case 0xa9baU: { // 7200 moveq #$0, d1
            next = 0xa9bcU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xa9bcU: { // 31c10c42 move.w d1, $c42.w
            next = 0xa9c0U;
            const auto value = r.data[1];
            const auto destination_address = 0xc42U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xa9c0U: { // 41fa0426 lea.l $ade8(pc), a0
            next = 0xa9c4U;
            const auto source_address = 0xade8U;
            r.address[0] = source_address;
            break;
        }
        case 0xa9c4U: { // 22701000 movea.l (a0, d1.w), a1
            next = 0xa9c8U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[1]);
            const auto value = m.lng(source_address);
            r.address[1] = value;
            break;
        }
        case 0xa9c8U: { // 4eb900015fb8 jsr $15fb8.l
            next = 0xa9ceU;
            const auto result = m.call(c, 638U, 0xa9c8U, 0x15fb8U, 0xa9ceU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa9ceU: { // 610000b4 bsr.w $aa84
            next = 0xa9d2U;
            const auto result = m.call(c, 623U, 0xa9ceU, 0xaa84U, 0xa9d2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa9d2U: { // 610000e2 bsr.w $aab6
            next = 0xa9d6U;
            const auto result = m.call(c, 624U, 0xa9d2U, 0xaab6U, 0xa9d6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa9d6U: { // 4e75 rts 
            next = 0xa9d8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xa9d6U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0xa97cU:
        case 0xa980U:
        case 0xa986U:
        case 0xa98aU:
        case 0xa990U:
        case 0xa994U:
        case 0xa99aU:
        case 0xa9a0U:
        case 0xa9a2U:
        case 0xa9a6U:
        case 0xa9a8U:
        case 0xa9aaU:
        case 0xa9acU:
        case 0xa9b0U:
        case 0xa9b2U:
        case 0xa9b4U:
        case 0xa9b8U:
        case 0xa9baU:
        case 0xa9bcU:
        case 0xa9c0U:
        case 0xa9c4U:
        case 0xa9c8U:
        case 0xa9ceU:
        case 0xa9d2U:
        case 0xa9d6U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
