// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0000a9d8(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xa9d8U: { // 43fa0342 lea.l $ad1c(pc), a1
            next = 0xa9dcU;
            const auto source_address = 0xad1cU;
            r.address[1] = source_address;
            break;
        }
        case 0xa9dcU: { // 4eb900015fb8 jsr $15fb8.l
            next = 0xa9e2U;
            const auto result = m.call(c, 638U, 0xa9dcU, 0x15fb8U, 0xa9e2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa9e2U: { // 43fa0374 lea.l $ad58(pc), a1
            next = 0xa9e6U;
            const auto source_address = 0xad58U;
            r.address[1] = source_address;
            break;
        }
        case 0xa9e6U: { // 4eb900015fb8 jsr $15fb8.l
            next = 0xa9ecU;
            const auto result = m.call(c, 638U, 0xa9e6U, 0x15fb8U, 0xa9ecU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa9ecU: { // 43fa03b8 lea.l $ada6(pc), a1
            next = 0xa9f0U;
            const auto source_address = 0xada6U;
            r.address[1] = source_address;
            break;
        }
        case 0xa9f0U: { // 4eb900015fb8 jsr $15fb8.l
            next = 0xa9f6U;
            const auto result = m.call(c, 638U, 0xa9f0U, 0x15fb8U, 0xa9f6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa9f6U: { // 0c7800040c00 cmpi.w #$4, $c00.w
            next = 0xa9fcU;
            const auto source_value = 0x4U;
            const auto destination_address = 0xc00U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xa9fcU: { // 6a14 bpl.b $aa12
            next = 0xa9feU;
            if ((r.status & 8U) == 0U) { next = 0xaa12U; transfer_kind = 1U; }
            break;
        }
        case 0xa9feU: { // 45fa0578 lea.l $af78(pc), a2
            next = 0xaa02U;
            const auto source_address = 0xaf78U;
            r.address[2] = source_address;
            break;
        }
        case 0xaa02U: { // 7602 moveq #$2, d3
            next = 0xaa04U;
            r.data[3] = 0x2U;
            m.logic(r.data[3], 32U);
            break;
        }
        case 0xaa04U: { // 225a movea.l (a2)+, a1
            next = 0xaa06U;
            const auto source_address = r.address[2];
            const auto value = m.lng(source_address);
            r.address[2] += 4U;
            r.address[1] = value;
            break;
        }
        case 0xaa06U: { // 4eb900015fb8 jsr $15fb8.l
            next = 0xaa0cU;
            const auto result = m.call(c, 638U, 0xaa06U, 0x15fb8U, 0xaa0cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xaa0cU: { // 51cbfff6 dbra d3, $aa04
            next = 0xaa10U;
            m.dw(3U, r.data[3] - 1U);
            if ((r.data[3] & 0xffffU) != 0xffffU) next = 0xaa04U;
            break;
        }
        case 0xaa10U: { // 6012 bra.b $aa24
            next = 0xaa12U;
            if (true) { next = 0xaa24U; transfer_kind = 1U; }
            break;
        }
        case 0xaa12U: { // 45fa0570 lea.l $af84(pc), a2
            next = 0xaa16U;
            const auto source_address = 0xaf84U;
            r.address[2] = source_address;
            break;
        }
        case 0xaa16U: { // 7606 moveq #$6, d3
            next = 0xaa18U;
            r.data[3] = 0x6U;
            m.logic(r.data[3], 32U);
            break;
        }
        case 0xaa18U: { // 225a movea.l (a2)+, a1
            next = 0xaa1aU;
            const auto source_address = r.address[2];
            const auto value = m.lng(source_address);
            r.address[2] += 4U;
            r.address[1] = value;
            break;
        }
        case 0xaa1aU: { // 4eb900015fb8 jsr $15fb8.l
            next = 0xaa20U;
            const auto result = m.call(c, 638U, 0xaa1aU, 0x15fb8U, 0xaa20U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xaa20U: { // 51cbfff6 dbra d3, $aa18
            next = 0xaa24U;
            m.dw(3U, r.data[3] - 1U);
            if ((r.data[3] & 0xffffU) != 0xffffU) next = 0xaa18U;
            break;
        }
        case 0xaa24U: { // 6100005e bsr.w $aa84
            next = 0xaa28U;
            const auto result = m.call(c, 623U, 0xaa24U, 0xaa84U, 0xaa28U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xaa28U: { // 610000d0 bsr.w $aafa
            next = 0xaa2cU;
            const auto result = m.call(c, 625U, 0xaa28U, 0xaafaU, 0xaa2cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xaa2cU: { // 4e75 rts 
            next = 0xaa2eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xaa2cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0xa9d8U:
        case 0xa9dcU:
        case 0xa9e2U:
        case 0xa9e6U:
        case 0xa9ecU:
        case 0xa9f0U:
        case 0xa9f6U:
        case 0xa9fcU:
        case 0xa9feU:
        case 0xaa02U:
        case 0xaa04U:
        case 0xaa06U:
        case 0xaa0cU:
        case 0xaa10U:
        case 0xaa12U:
        case 0xaa16U:
        case 0xaa18U:
        case 0xaa1aU:
        case 0xaa20U:
        case 0xaa24U:
        case 0xaa28U:
        case 0xaa2cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
