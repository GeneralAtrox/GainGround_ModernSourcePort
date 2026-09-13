// Implemented but unverified. Validation is recorded in the current function work packet.
#include "unverified_cpu_a_diagnostic_irq.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_a_plain_000811a0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 0U, 255U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        std::uint8_t instruction_kind = 0U;
        switch (pc) {
        case 0x811a0U: { // e859 ror.w #$4, d1
            next = 0x811a2U;
            m.rotate_right(1U, 4U, 16U);
            (void)m.word(0x811a4U);
            break;
        }
        case 0x811a2U: { // 7000 moveq #$0, d0
            next = 0x811a4U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            (void)m.word(0x811a6U);
            break;
        }
        case 0x811a4U: { // 1018 move.b (a0)+, d0
            next = 0x811a6U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            (void)m.word(0x811a8U);
            break;
        }
        case 0x811a6U: { // e148 lsl.w #$8, d0
            next = 0x811a8U;
            m.shift_word(0U, 8U, true, false);
            (void)m.word(0x811aaU);
            break;
        }
        case 0x811a8U: { // 1018 move.b (a0)+, d0
            next = 0x811aaU;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            (void)m.word(0x811acU);
            break;
        }
        case 0x811aaU: { // 43f900200000 lea.l $200000.l, a1
            next = 0x811b0U;
            (void)m.word(0x811aeU);
            (void)m.word(0x811b0U);
            r.address[1] = 0x200000U;
            (void)m.word(0x811b2U);
            break;
        }
        case 0x811b0U: { // d2c0 adda.w d0, a1
            next = 0x811b2U;
            const auto source_value = r.data[0];
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x811b4U);
            break;
        }
        case 0x811b2U: { // 7000 moveq #$0, d0
            next = 0x811b4U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            (void)m.word(0x811b6U);
            break;
        }
        case 0x811b4U: { // 1018 move.b (a0)+, d0
            next = 0x811b6U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            (void)m.word(0x811b8U);
            break;
        }
        case 0x811b6U: { // 6728 beq.b $811e0
            next = 0x811b8U;
            instruction_kind = 1U;
            if ((r.status & 4U) != 0U) { next = 0x811e0U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811b8U: { // 45fa0006 lea.l $811c0(pc), a2
            next = 0x811bcU;
            (void)m.word(0x811bcU);
            const auto source_address = 0x811c0U;
            r.address[2] = source_address;
            (void)m.word(0x811beU);
            break;
        }
        case 0x811bcU: { // 4ef22000 jmp (a2, d2.w)
            next = 0x811c0U;
            instruction_kind = 1U;
            const auto target_address = r.address[2] + static_cast<std::int16_t>(r.data[2]);
            next = target_address & 0xffffffU;
            (void)m.word(next);
            (void)m.word(next + 2U);
            transfer_kind = 1U;
            break;
        }
        case 0x811c0U: { // 6004 bra.b $811c6
            next = 0x811c2U;
            instruction_kind = 1U;
            if (true) { next = 0x811c6U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811c2U: { // 601e bra.b $811e2
            next = 0x811c4U;
            instruction_kind = 1U;
            if (true) { next = 0x811e2U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811c4U: { // 6042 bra.b $81208
            next = 0x811c6U;
            instruction_kind = 1U;
            if (true) { next = 0x81208U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811c6U: { // b07c0020 cmp.w #$20, d0
            next = 0x811caU;
            (void)m.word(0x811caU);
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            (void)m.word(0x811ccU);
            break;
        }
        case 0x811caU: { // 670a beq.b $811d6
            next = 0x811ccU;
            instruction_kind = 1U;
            if ((r.status & 4U) != 0U) { next = 0x811d6U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811ccU: { // 8041 or.w d1, d0
            next = 0x811ceU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            (void)m.word(0x811d0U);
            break;
        }
        case 0x811ceU: { // 3280 move.w d0, (a1)
            next = 0x811d0U;
            const auto value = r.data[0];
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x811d2U);
            break;
        }
        case 0x811d0U: { // d2fc0080 adda.w #$80, a1
            next = 0x811d4U;
            (void)m.word(0x811d4U);
            const auto source_value = 0x80U;
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x811d6U);
            break;
        }
        case 0x811d4U: { // 60dc bra.b $811b2
            next = 0x811d6U;
            instruction_kind = 1U;
            if (true) { next = 0x811b2U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811d6U: { // 32bc0000 move.w #$0, (a1)
            next = 0x811daU;
            (void)m.word(0x811daU);
            const auto value = 0x0U;
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x811dcU);
            break;
        }
        case 0x811daU: { // d2fc0080 adda.w #$80, a1
            next = 0x811deU;
            (void)m.word(0x811deU);
            const auto source_value = 0x80U;
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x811e0U);
            break;
        }
        case 0x811deU: { // 60d2 bra.b $811b2
            next = 0x811e0U;
            instruction_kind = 1U;
            if (true) { next = 0x811b2U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811e0U: { // 4e75 rts 
            next = 0x811e2U;
            const auto result = m.ret();
            (void)m.word(r.program_counter); (void)m.word(r.program_counter + 2U);
            if (auto event = unverified::diagnostic_irq3(c, m, 0x811e0U, r.program_counter, 3U)) return *event;
            return result;
            break;
        }
        case 0x811e2U: { // b07c0020 cmp.w #$20, d0
            next = 0x811e6U;
            (void)m.word(0x811e6U);
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            (void)m.word(0x811e8U);
            break;
        }
        case 0x811e6U: { // 6710 beq.b $811f8
            next = 0x811e8U;
            instruction_kind = 1U;
            if ((r.status & 4U) != 0U) { next = 0x811f8U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811e8U: { // d040 add.w d0, d0
            next = 0x811eaU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x811ecU);
            break;
        }
        case 0x811eaU: { // 8041 or.w d1, d0
            next = 0x811ecU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            (void)m.word(0x811eeU);
            break;
        }
        case 0x811ecU: { // 3280 move.w d0, (a1)
            next = 0x811eeU;
            const auto value = r.data[0];
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x811f0U);
            break;
        }
        case 0x811eeU: { // 5240 addq.w #$1, d0
            next = 0x811f0U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x811f2U);
            break;
        }
        case 0x811f0U: { // 3300 move.w d0, -(a1)
            next = 0x811f2U;
            (void)m.word(0x811f4U);
            const auto value = r.data[0];
            r.address[1] -= 2U;
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x811f2U: { // d2fc0082 adda.w #$82, a1
            next = 0x811f6U;
            (void)m.word(0x811f6U);
            const auto source_value = 0x82U;
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x811f8U);
            break;
        }
        case 0x811f6U: { // 60ba bra.b $811b2
            next = 0x811f8U;
            instruction_kind = 1U;
            if (true) { next = 0x811b2U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811f8U: { // 92fc0002 suba.w #$2, a1
            next = 0x811fcU;
            (void)m.word(0x811fcU);
            const auto source_value = 0x2U;
            r.address[1] -= static_cast<std::int16_t>(source_value);
            (void)m.word(0x811feU);
            break;
        }
        case 0x811fcU: { // 22fc00000000 move.l #$0, (a1)+
            next = 0x81202U;
            (void)m.word(0x81200U);
            (void)m.word(0x81202U);
            const auto value = 0x0U;
            const auto destination_address = r.address[1];
            m.lng(destination_address, value);
            r.address[1] += 4U;
            m.logic(value, 32U);
            (void)m.word(0x81204U);
            break;
        }
        case 0x81202U: { // d2fc007e adda.w #$7e, a1
            next = 0x81206U;
            (void)m.word(0x81206U);
            const auto source_value = 0x7eU;
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x81208U);
            break;
        }
        case 0x81206U: { // 60aa bra.b $811b2
            next = 0x81208U;
            instruction_kind = 1U;
            if (true) { next = 0x811b2U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x81208U: { // b07c0020 cmp.w #$20, d0
            next = 0x8120cU;
            (void)m.word(0x8120cU);
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            (void)m.word(0x8120eU);
            break;
        }
        case 0x8120cU: { // 6722 beq.b $81230
            next = 0x8120eU;
            instruction_kind = 1U;
            if ((r.status & 4U) != 0U) { next = 0x81230U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x8120eU: { // d040 add.w d0, d0
            next = 0x81210U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x81212U);
            break;
        }
        case 0x81210U: { // d040 add.w d0, d0
            next = 0x81212U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x81214U);
            break;
        }
        case 0x81212U: { // d07c0080 add.w #$80, d0
            next = 0x81216U;
            (void)m.word(0x81216U);
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x81218U);
            break;
        }
        case 0x81216U: { // 8041 or.w d1, d0
            next = 0x81218U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            (void)m.word(0x8121aU);
            break;
        }
        case 0x81218U: { // 3280 move.w d0, (a1)
            next = 0x8121aU;
            const auto value = r.data[0];
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x8121cU);
            break;
        }
        case 0x8121aU: { // 5440 addq.w #$2, d0
            next = 0x8121cU;
            const auto source_value = 0x2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x8121eU);
            break;
        }
        case 0x8121cU: { // 3300 move.w d0, -(a1)
            next = 0x8121eU;
            (void)m.word(0x81220U);
            const auto value = r.data[0];
            r.address[1] -= 2U;
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x8121eU: { // d2fc0082 adda.w #$82, a1
            next = 0x81222U;
            (void)m.word(0x81222U);
            const auto source_value = 0x82U;
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x81224U);
            break;
        }
        case 0x81222U: { // 5340 subq.w #$1, d0
            next = 0x81224U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x81226U);
            break;
        }
        case 0x81224U: { // 3280 move.w d0, (a1)
            next = 0x81226U;
            const auto value = r.data[0];
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x81228U);
            break;
        }
        case 0x81226U: { // 5440 addq.w #$2, d0
            next = 0x81228U;
            const auto source_value = 0x2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x8122aU);
            break;
        }
        case 0x81228U: { // 3300 move.w d0, -(a1)
            next = 0x8122aU;
            (void)m.word(0x8122cU);
            const auto value = r.data[0];
            r.address[1] -= 2U;
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x8122aU: { // d2fc0082 adda.w #$82, a1
            next = 0x8122eU;
            (void)m.word(0x8122eU);
            const auto source_value = 0x82U;
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x81230U);
            break;
        }
        case 0x8122eU: { // 6082 bra.b $811b2
            next = 0x81230U;
            instruction_kind = 1U;
            if (true) { next = 0x811b2U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x81230U: { // 92fc0002 suba.w #$2, a1
            next = 0x81234U;
            (void)m.word(0x81234U);
            const auto source_value = 0x2U;
            r.address[1] -= static_cast<std::int16_t>(source_value);
            (void)m.word(0x81236U);
            break;
        }
        case 0x81234U: { // 22fc00000000 move.l #$0, (a1)+
            next = 0x8123aU;
            (void)m.word(0x81238U);
            (void)m.word(0x8123aU);
            const auto value = 0x0U;
            const auto destination_address = r.address[1];
            m.lng(destination_address, value);
            r.address[1] += 4U;
            m.logic(value, 32U);
            (void)m.word(0x8123cU);
            break;
        }
        case 0x8123aU: { // d2fc007e adda.w #$7e, a1
            next = 0x8123eU;
            (void)m.word(0x8123eU);
            const auto source_value = 0x7eU;
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x81240U);
            break;
        }
        case 0x8123eU: { // 92fc0002 suba.w #$2, a1
            next = 0x81242U;
            (void)m.word(0x81242U);
            const auto source_value = 0x2U;
            r.address[1] -= static_cast<std::int16_t>(source_value);
            (void)m.word(0x81244U);
            break;
        }
        case 0x81242U: { // 22fc00000000 move.l #$0, (a1)+
            next = 0x81248U;
            (void)m.word(0x81246U);
            (void)m.word(0x81248U);
            const auto value = 0x0U;
            const auto destination_address = r.address[1];
            m.lng(destination_address, value);
            r.address[1] += 4U;
            m.logic(value, 32U);
            (void)m.word(0x8124aU);
            break;
        }
        case 0x81248U: { // d2fc007e adda.w #$7e, a1
            next = 0x8124cU;
            (void)m.word(0x8124cU);
            const auto source_value = 0x7eU;
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x8124eU);
            break;
        }
        case 0x8124cU: { // 6000ff64 bra.w $811b2
            next = 0x81250U;
            instruction_kind = 1U;
            if (true) { next = 0x811b2U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = unverified::diagnostic_irq3(c, m, pc, next, instruction_kind)) return *event;
        switch (next) {
        case 0x811a0U:
        case 0x811a2U:
        case 0x811a4U:
        case 0x811a6U:
        case 0x811a8U:
        case 0x811aaU:
        case 0x811b0U:
        case 0x811b2U:
        case 0x811b4U:
        case 0x811b6U:
        case 0x811b8U:
        case 0x811bcU:
        case 0x811c0U:
        case 0x811c2U:
        case 0x811c4U:
        case 0x811c6U:
        case 0x811caU:
        case 0x811ccU:
        case 0x811ceU:
        case 0x811d0U:
        case 0x811d4U:
        case 0x811d6U:
        case 0x811daU:
        case 0x811deU:
        case 0x811e0U:
        case 0x811e2U:
        case 0x811e6U:
        case 0x811e8U:
        case 0x811eaU:
        case 0x811ecU:
        case 0x811eeU:
        case 0x811f0U:
        case 0x811f2U:
        case 0x811f6U:
        case 0x811f8U:
        case 0x811fcU:
        case 0x81202U:
        case 0x81206U:
        case 0x81208U:
        case 0x8120cU:
        case 0x8120eU:
        case 0x81210U:
        case 0x81212U:
        case 0x81216U:
        case 0x81218U:
        case 0x8121aU:
        case 0x8121cU:
        case 0x8121eU:
        case 0x81222U:
        case 0x81224U:
        case 0x81226U:
        case 0x81228U:
        case 0x8122aU:
        case 0x8122eU:
        case 0x81230U:
        case 0x81234U:
        case 0x8123aU:
        case 0x8123eU:
        case 0x81242U:
        case 0x81248U:
        case 0x8124cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
