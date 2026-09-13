// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult cpu_a_dynamic_fdc_service(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 0U, 255U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x80356U: { // 1039fff00820 move.b $fff00820.l, d0
            next = 0x8035cU;
            const auto source_address = 0xfff00820U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x8035cU: { // 02400008 andi.w #$8, d0
            next = 0x80360U;
            const auto source_value = 0x8U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x80360U: { // 6638 bne.b $8039a
            next = 0x80362U;
            if ((r.status & 4U) == 0U) { next = 0x8039aU; transfer_kind = 1U; }
            break;
        }
        case 0x80362U: { // 4a39fff00418 tst.b $fff00418.l
            next = 0x80368U;
            const auto destination_address = 0xfff00418U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x80368U: { // 6730 beq.b $8039a
            next = 0x8036aU;
            if ((r.status & 4U) != 0U) { next = 0x8039aU; transfer_kind = 1U; }
            break;
        }
        case 0x8036aU: { // 46fc2700 move.w #$2700, sr
            next = 0x8036eU;
            r.status = static_cast<std::uint16_t>(0x2700U);
            break;
        }
        case 0x8036eU: { // 4239fff00418 clr.b $fff00418.l
            next = 0x80374U;
            const auto destination_address = 0xfff00418U;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x80374U: { // 50f88001 st.b $8001.w
            next = 0x80378U;
            const auto destination_address = 0xffff8001U;
            m.byte(destination_address, 0xffU);
            break;
        }
        case 0x80378U: { // 70ff moveq #$ff, d0
            next = 0x8037aU;
            r.data[0] = 0xffffffffU;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x8037aU: { // 4e48 trap #$8
            next = 0x8037cU;
            const auto result = m.exception(c, 40U, 0x8037aU, 0x8037cU);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x8037cU) return result;
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x8037cU: { // 41f8d000 lea.l $d000.w, a0
            next = 0x80380U;
            r.address[0] = 0xffffd000U;
            break;
        }
        case 0x80380U: { // 43f9fff07b20 lea.l $fff07b20.l, a1
            next = 0x80386U;
            r.address[1] = 0xfff07b20U;
            break;
        }
        case 0x80386U: { // 223c0000004c move.l #$4c, d1
            next = 0x8038cU;
            const auto value = 0x4cU;
            r.data[1] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x8038cU: { // 4e47 trap #$7
            next = 0x8038eU;
            const auto result = m.exception(c, 39U, 0x8038cU, 0x8038eU);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x8038eU) return result;
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x8038eU: { // 7000 moveq #$0, d0
            next = 0x80390U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x80390U: { // 4e48 trap #$8
            next = 0x80392U;
            const auto result = m.exception(c, 40U, 0x80390U, 0x80392U);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x80392U) return result;
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x80392U: { // 42388001 clr.b $8001.w
            next = 0x80396U;
            const auto destination_address = 0xffff8001U;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x80396U: { // 46fc2000 move.w #$2000, sr
            next = 0x8039aU;
            r.status = static_cast<std::uint16_t>(0x2000U);
            break;
        }
        case 0x8039aU: { // 30388002 move.w $8002.w, d0
            next = 0x8039eU;
            const auto source_address = 0xffff8002U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x8039eU: { // 6602 bne.b $803a2
            next = 0x803a0U;
            if ((r.status & 4U) == 0U) { next = 0x803a2U; transfer_kind = 1U; }
            break;
        }
        case 0x803a0U: { // 4e75 rts 
            next = 0x803a2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x803a0U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x803a2U: { // 0c400001 cmpi.w #$1, d0
            next = 0x803a6U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x803a6U: { // 6604 bne.b $803ac
            next = 0x803a8U;
            if ((r.status & 4U) == 0U) { next = 0x803acU; transfer_kind = 1U; }
            break;
        }
        case 0x803a8U: { // 6100ff3c bsr.w $802e6
            next = 0x803acU;
            const auto result = m.call(c, 60U, 0x803a8U, 0x802e6U, 0x803acU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x803acU: { // 30388002 move.w $8002.w, d0
            next = 0x803b0U;
            const auto source_address = 0xffff8002U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x803b0U: { // 0c400007 cmpi.w #$7, d0
            next = 0x803b4U;
            const auto source_value = 0x7U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x803b4U: { // 6c00fd0a bge.w $800c0
            next = 0x803b8U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x800c0U; transfer_kind = 1U; }
            break;
        }
        case 0x803b8U: { // b0788004 cmp.w $8004.w, d0
            next = 0x803bcU;
            const auto source_address = 0xffff8004U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x803bcU: { // 6750 beq.b $8040e
            next = 0x803beU;
            if ((r.status & 4U) != 0U) { next = 0x8040eU; transfer_kind = 1U; }
            break;
        }
        case 0x803beU: { // 31c08004 move.w d0, $8004.w
            next = 0x803c2U;
            const auto value = r.data[0];
            const auto destination_address = 0xffff8004U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x803c2U: { // 46fc2700 move.w #$2700, sr
            next = 0x803c6U;
            r.status = static_cast<std::uint16_t>(0x2700U);
            break;
        }
        case 0x803c6U: { // 50f88001 st.b $8001.w
            next = 0x803caU;
            const auto destination_address = 0xffff8001U;
            m.byte(destination_address, 0xffU);
            break;
        }
        case 0x803caU: { // 70ff moveq #$ff, d0
            next = 0x803ccU;
            r.data[0] = 0xffffffffU;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x803ccU: { // 4e48 trap #$8
            next = 0x803ceU;
            const auto result = m.exception(c, 40U, 0x803ccU, 0x803ceU);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x803ceU) return result;
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x803ceU: { // 30388002 move.w $8002.w, d0
            next = 0x803d2U;
            const auto source_address = 0xffff8002U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x803d2U: { // d040 add.w d0, d0
            next = 0x803d4U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x803d4U: { // 4dfa02c2 lea.l $80698(pc), a6
            next = 0x803d8U;
            const auto source_address = 0x80698U;
            r.address[6] = source_address;
            break;
        }
        case 0x803d8U: { // dcc0 adda.w d0, a6
            next = 0x803daU;
            const auto source_value = r.data[0];
            r.address[6] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x803daU: { // dcd6 adda.w (a6), a6
            next = 0x803dcU;
            const auto source_address = r.address[6];
            const auto source_value = m.word(source_address);
            r.address[6] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x803dcU: { // 3e1e move.w (a6)+, d7
            next = 0x803deU;
            const auto source_address = r.address[6];
            const auto value = m.word(source_address);
            r.address[6] += 2U;
            m.dw(7U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x803deU: { // 41f8d000 lea.l $d000.w, a0
            next = 0x803e2U;
            r.address[0] = 0xffffd000U;
            break;
        }
        case 0x803e2U: { // 225e movea.l (a6)+, a1
            next = 0x803e4U;
            const auto source_address = r.address[6];
            const auto value = m.lng(source_address);
            r.address[6] += 4U;
            r.address[1] = value;
            break;
        }
        case 0x803e4U: { // 201e move.l (a6)+, d0
            next = 0x803e6U;
            const auto source_address = r.address[6];
            const auto value = m.lng(source_address);
            r.address[6] += 4U;
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x803e6U: { // 221e move.l (a6)+, d1
            next = 0x803e8U;
            const auto source_address = r.address[6];
            const auto value = m.lng(source_address);
            r.address[6] += 4U;
            r.data[1] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x803e8U: { // 3f07 move.w d7, -(a7)
            next = 0x803eaU;
            const auto value = r.data[7];
            r.address[7] -= 2U;
            const auto destination_address = r.address[7];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x803eaU: { // 4e4a trap #$a
            next = 0x803ecU;
            const auto result = m.exception(c, 42U, 0x803eaU, 0x803ecU);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x803ecU) return result;
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x803ecU: { // 3e1f move.w (a7)+, d7
            next = 0x803eeU;
            const auto source_address = r.address[7];
            const auto value = m.word(source_address);
            r.address[7] += 2U;
            m.dw(7U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x803eeU: { // 51cfffee dbra d7, $803de
            next = 0x803f2U;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x803deU;
            break;
        }
        case 0x803f2U: { // 7000 moveq #$0, d0
            next = 0x803f4U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x803f4U: { // 4e48 trap #$8
            next = 0x803f6U;
            const auto result = m.exception(c, 40U, 0x803f4U, 0x803f6U);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x803f6U) return result;
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x803f6U: { // 42388001 clr.b $8001.w
            next = 0x803faU;
            const auto destination_address = 0xffff8001U;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x803faU: { // 46fc2000 move.w #$2000, sr
            next = 0x803feU;
            r.status = static_cast<std::uint16_t>(0x2000U);
            break;
        }
        case 0x803feU: { // 30388002 move.w $8002.w, d0
            next = 0x80402U;
            const auto source_address = 0xffff8002U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x80402U: { // 6700000a beq.w $8040e
            next = 0x80406U;
            if ((r.status & 4U) != 0U) { next = 0x8040eU; transfer_kind = 1U; }
            break;
        }
        case 0x80406U: { // b0788004 cmp.w $8004.w, d0
            next = 0x8040aU;
            const auto source_address = 0xffff8004U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x8040aU: { // 6600ff4a bne.w $80356
            next = 0x8040eU;
            if ((r.status & 4U) == 0U) { next = 0x80356U; transfer_kind = 1U; }
            break;
        }
        case 0x8040eU: { // 42788002 clr.w $8002.w
            next = 0x80412U;
            const auto destination_address = 0xffff8002U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x80412U: { // 4e75 rts 
            next = 0x80414U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x80412U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0x800c0U) return c.host->call_function(54U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0x80356U:
        case 0x8035cU:
        case 0x80360U:
        case 0x80362U:
        case 0x80368U:
        case 0x8036aU:
        case 0x8036eU:
        case 0x80374U:
        case 0x80378U:
        case 0x8037aU:
        case 0x8037cU:
        case 0x80380U:
        case 0x80386U:
        case 0x8038cU:
        case 0x8038eU:
        case 0x80390U:
        case 0x80392U:
        case 0x80396U:
        case 0x8039aU:
        case 0x8039eU:
        case 0x803a0U:
        case 0x803a2U:
        case 0x803a6U:
        case 0x803a8U:
        case 0x803acU:
        case 0x803b0U:
        case 0x803b4U:
        case 0x803b8U:
        case 0x803bcU:
        case 0x803beU:
        case 0x803c2U:
        case 0x803c6U:
        case 0x803caU:
        case 0x803ccU:
        case 0x803ceU:
        case 0x803d2U:
        case 0x803d4U:
        case 0x803d8U:
        case 0x803daU:
        case 0x803dcU:
        case 0x803deU:
        case 0x803e2U:
        case 0x803e4U:
        case 0x803e6U:
        case 0x803e8U:
        case 0x803eaU:
        case 0x803ecU:
        case 0x803eeU:
        case 0x803f2U:
        case 0x803f4U:
        case 0x803f6U:
        case 0x803faU:
        case 0x803feU:
        case 0x80402U:
        case 0x80406U:
        case 0x8040aU:
        case 0x8040eU:
        case 0x80412U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
