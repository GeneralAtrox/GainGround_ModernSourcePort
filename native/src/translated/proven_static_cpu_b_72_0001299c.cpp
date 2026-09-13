// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0001299c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1299cU: { // 47f86c00 lea.l $6c00.w, a3
            next = 0x129a0U;
            r.address[3] = 0x6c00U;
            break;
        }
        case 0x129a0U: { // 4a5b tst.w (a3)+
            next = 0x129a2U;
            const auto destination_address = r.address[3];
            const auto value = m.word(destination_address);
            r.address[3] += 2U;
            m.logic(value, 16U);
            break;
        }
        case 0x129a2U: { // 6b04 bmi.b $129a8
            next = 0x129a4U;
            if ((r.status & 8U) != 0U) { next = 0x129a8U; transfer_kind = 1U; }
            break;
        }
        case 0x129a4U: { // 47f87002 lea.l $7002.w, a3
            next = 0x129a8U;
            r.address[3] = 0x7002U;
            break;
        }
        case 0x129a8U: { // 302d001a move.w $1a(a5), d0
            next = 0x129acU;
            m.dw(0U, m.word(r.address[5] + 0x1aU));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0x129acU: { // 0c400200 cmpi.w #$200, d0
            next = 0x129b0U;
            const auto source_value = 0x200U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x129b0U: { // 64000072 bcc.w $12a24
            next = 0x129b4U;
            if ((r.status & 1U) == 0U) { next = 0x12a24U; transfer_kind = 1U; }
            break;
        }
        case 0x129b4U: { // 3e00 move.w d0, d7
            next = 0x129b6U;
            const auto value = r.data[0];
            m.dw(7U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x129b6U: { // 06470020 addi.w #$20, d7
            next = 0x129baU;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[7];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(7U, value);
            break;
        }
        case 0x129baU: { // 04400020 subi.w #$20, d0
            next = 0x129beU;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x129beU: { // 6b10 bmi.b $129d0
            next = 0x129c0U;
            if ((r.status & 8U) != 0U) { next = 0x129d0U; transfer_kind = 1U; }
            break;
        }
        case 0x129c0U: { // 0c4701ff cmpi.w #$1ff, d7
            next = 0x129c4U;
            const auto source_value = 0x1ffU;
            const auto destination_value = r.data[7];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x129c4U: { // 6f04 ble.b $129ca
            next = 0x129c6U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x129caU; transfer_kind = 1U; }
            break;
        }
        case 0x129c6U: { // 3e3c01ff move.w #$1ff, d7
            next = 0x129caU;
            m.dw(7U, 0x1ffU);
            m.logic(r.data[7], 16U);
            break;
        }
        case 0x129caU: { // 9e40 sub.w d0, d7
            next = 0x129ccU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[7];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(7U, value);
            break;
        }
        case 0x129ccU: { // d040 add.w d0, d0
            next = 0x129ceU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x129ceU: { // d6c0 adda.w d0, a3
            next = 0x129d0U;
            const auto source_value = r.data[0];
            r.address[3] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x129d0U: { // 301b move.w (a3)+, d0
            next = 0x129d2U;
            const auto source_address = r.address[3];
            const auto value = m.word(source_address);
            r.address[3] += 2U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x129d2U: { // 674c beq.b $12a20
            next = 0x129d4U;
            if ((r.status & 4U) != 0U) { next = 0x12a20U; transfer_kind = 1U; }
            break;
        }
        case 0x129d4U: { // 3c40 movea.w d0, a6
            next = 0x129d6U;
            const auto value = r.data[0];
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x129d6U: { // 0c2e0004000b cmpi.b #$4, $b(a6)
            next = 0x129dcU;
            const auto source_value = 0x4U;
            const auto destination_address = r.address[6] + 0xbU;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x129dcU: { // 660c bne.b $129ea
            next = 0x129deU;
            if ((r.status & 4U) == 0U) { next = 0x129eaU; transfer_kind = 1U; }
            break;
        }
        case 0x129deU: { // 6100004a bsr.w $12a2a
            next = 0x129e2U;
            const auto result = m.call(c, 255U, 0x129deU, 0x12a2aU, 0x129e2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x129e2U: { // 643c bcc.b $12a20
            next = 0x129e4U;
            if ((r.status & 1U) == 0U) { next = 0x12a20U; transfer_kind = 1U; }
            break;
        }
        case 0x129e4U: { // 44fc0001 move.w #$1, ccr
            next = 0x129e8U;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x1U & 0x1fU));
            break;
        }
        case 0x129e8U: { // 4e75 rts 
            next = 0x129eaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x129e8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x129eaU: { // 0c2e0008000b cmpi.b #$8, $b(a6)
            next = 0x129f0U;
            const auto source_value = 0x8U;
            const auto destination_address = r.address[6] + 0xbU;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x129f0U: { // 6b2e bmi.b $12a20
            next = 0x129f2U;
            if ((r.status & 8U) != 0U) { next = 0x12a20U; transfer_kind = 1U; }
            break;
        }
        case 0x129f2U: { // 0c2e000a000b cmpi.b #$a, $b(a6)
            next = 0x129f8U;
            const auto source_value = 0xaU;
            const auto destination_address = r.address[6] + 0xbU;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x129f8U: { // 6a0e bpl.b $12a08
            next = 0x129faU;
            if ((r.status & 8U) == 0U) { next = 0x12a08U; transfer_kind = 1U; }
            break;
        }
        case 0x129faU: { // 4a2e003f tst.b $3f(a6)
            next = 0x129feU;
            m.logic(m.byte(r.address[6] + 0x3fU), 8U);
            break;
        }
        case 0x129feU: { // 6620 bne.b $12a20
            next = 0x12a00U;
            if ((r.status & 4U) == 0U) { next = 0x12a20U; transfer_kind = 1U; }
            break;
        }
        case 0x12a00U: { // 61000070 bsr.w $12a72
            next = 0x12a04U;
            const auto result = m.call(c, 256U, 0x12a00U, 0x12a72U, 0x12a04U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12a04U: { // 641a bcc.b $12a20
            next = 0x12a06U;
            if ((r.status & 1U) == 0U) { next = 0x12a20U; transfer_kind = 1U; }
            break;
        }
        case 0x12a06U: { // 600c bra.b $12a14
            next = 0x12a08U;
            if (true) { next = 0x12a14U; transfer_kind = 1U; }
            break;
        }
        case 0x12a08U: { // 4a2e003f tst.b $3f(a6)
            next = 0x12a0cU;
            m.logic(m.byte(r.address[6] + 0x3fU), 8U);
            break;
        }
        case 0x12a0cU: { // 6612 bne.b $12a20
            next = 0x12a0eU;
            if ((r.status & 4U) == 0U) { next = 0x12a20U; transfer_kind = 1U; }
            break;
        }
        case 0x12a0eU: { // 6100001a bsr.w $12a2a
            next = 0x12a12U;
            const auto result = m.call(c, 255U, 0x12a0eU, 0x12a2aU, 0x12a12U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12a12U: { // 640c bcc.b $12a20
            next = 0x12a14U;
            if ((r.status & 1U) == 0U) { next = 0x12a20U; transfer_kind = 1U; }
            break;
        }
        case 0x12a14U: { // 1d7c00f0003e move.b #$f0, $3e(a6)
            next = 0x12a1aU;
            m.byte(r.address[6] + 0x3eU, 0xf0U);
            m.logic(0xf0U, 8U);
            break;
        }
        case 0x12a1aU: { // 44fc0001 move.w #$1, ccr
            next = 0x12a1eU;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x1U & 0x1fU));
            break;
        }
        case 0x12a1eU: { // 4e75 rts 
            next = 0x12a20U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x12a1eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x12a20U: { // 51cfffae dbra d7, $129d0
            next = 0x12a24U;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x129d0U;
            break;
        }
        case 0x12a24U: { // 44fc0000 move.w #$0, ccr
            next = 0x12a28U;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x0U & 0x1fU));
            break;
        }
        case 0x12a28U: { // 4e75 rts 
            next = 0x12a2aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x12a28U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1299cU:
        case 0x129a0U:
        case 0x129a2U:
        case 0x129a4U:
        case 0x129a8U:
        case 0x129acU:
        case 0x129b0U:
        case 0x129b4U:
        case 0x129b6U:
        case 0x129baU:
        case 0x129beU:
        case 0x129c0U:
        case 0x129c4U:
        case 0x129c6U:
        case 0x129caU:
        case 0x129ccU:
        case 0x129ceU:
        case 0x129d0U:
        case 0x129d2U:
        case 0x129d4U:
        case 0x129d6U:
        case 0x129dcU:
        case 0x129deU:
        case 0x129e2U:
        case 0x129e4U:
        case 0x129e8U:
        case 0x129eaU:
        case 0x129f0U:
        case 0x129f2U:
        case 0x129f8U:
        case 0x129faU:
        case 0x129feU:
        case 0x12a00U:
        case 0x12a04U:
        case 0x12a06U:
        case 0x12a08U:
        case 0x12a0cU:
        case 0x12a0eU:
        case 0x12a12U:
        case 0x12a14U:
        case 0x12a1aU:
        case 0x12a1eU:
        case 0x12a20U:
        case 0x12a24U:
        case 0x12a28U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
