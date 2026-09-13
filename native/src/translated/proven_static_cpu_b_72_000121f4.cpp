// Implemented but unverified. Validation is recorded in the current function work packet.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_000121f4(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x121f4U: { // 302d0042 move.w $42(a5), d0
            next = 0x121f8U;
            m.dw(0U, m.word(r.address[5] + 0x42U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0x121f8U: { // e540 asl.w #$2, d0
            next = 0x121faU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x121faU: { // 4efb0002 jmp $121fe(pc, d0.w)
            next = 0x121feU;
            next = (0x121feU + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x121feU: { // 6000000e bra.w $1220e
            next = 0x12202U;
            if (true) { next = 0x1220eU; transfer_kind = 1U; }
            break;
        }
        case 0x12202U: { // 6000007a bra.w $1227e
            next = 0x12206U;
            if (true) { next = 0x1227eU; transfer_kind = 1U; }
            break;
        }
        case 0x12206U: { // 60000096 bra.w $1229e
            next = 0x1220aU;
            if (true) { next = 0x1229eU; transfer_kind = 1U; }
            break;
        }
        case 0x1220aU: { // 600000b0 bra.w $122bc
            next = 0x1220eU;
            if (true) { next = 0x122bcU; transfer_kind = 1U; }
            break;
        }
        case 0x1220eU: { // 6100078c bsr.w $1299c
            next = 0x12212U;
            const auto result = m.call(c, 535U, 0x1220eU, 0x1299cU, 0x12212U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12212U: { // 6404 bcc.b $12218
            next = 0x12214U;
            if ((r.status & 1U) == 0U) { next = 0x12218U; transfer_kind = 1U; }
            break;
        }
        case 0x12214U: { // 61000b24 bsr.w $12d3a
            next = 0x12218U;
            const auto result = m.call(c, 263U, 0x12214U, 0x12d3aU, 0x12218U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12218U: { // 61000a22 bsr.w $12c3c
            next = 0x1221cU;
            const auto result = m.call(c, 259U, 0x12218U, 0x12c3cU, 0x1221cU);
            if (result.status == TranslationStatus::complete && result.control == 8U)
                return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1221cU: { // 0c440100 cmpi.w #$100, d4
            next = 0x12220U;
            const auto source_value = 0x100U;
            const auto destination_value = r.data[4];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x12220U: { // 6504 bcs.b $12226
            next = 0x12222U;
            if ((r.status & 1U) != 0U) { next = 0x12226U; transfer_kind = 1U; }
            break;
        }
        case 0x12222U: { // 61000b16 bsr.w $12d3a
            next = 0x12226U;
            const auto result = m.call(c, 263U, 0x12222U, 0x12d3aU, 0x12226U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12226U: { // 0c440400 cmpi.w #$400, d4
            next = 0x1222aU;
            const auto source_value = 0x400U;
            const auto destination_value = r.data[4];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1222aU: { // 6514 bcs.b $12240
            next = 0x1222cU;
            if ((r.status & 1U) != 0U) { next = 0x12240U; transfer_kind = 1U; }
            break;
        }
        case 0x1222cU: { // 4a04 tst.b d4
            next = 0x1222eU;
            const auto value = r.data[4];
            m.logic(value, 8U);
            break;
        }
        case 0x1222eU: { // 6a0000a6 bpl.w $122d6
            next = 0x12232U;
            if ((r.status & 8U) == 0U) { next = 0x122d6U; transfer_kind = 1U; }
            break;
        }
        case 0x12232U: { // 422d003c clr.b $3c(a5)
            next = 0x12236U;
            const auto destination_address = r.address[5] + 0x3cU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x12236U: { // 3b7c00030042 move.w #$3, $42(a5)
            next = 0x1223cU;
            m.word(r.address[5] + 0x42U, 0x3U);
            m.logic(0x3U, 16U);
            break;
        }
        case 0x1223cU: { // 600000a0 bra.w $122de
            next = 0x12240U;
            if (true) { next = 0x122deU; transfer_kind = 1U; }
            break;
        }
        case 0x12240U: { // 202d001e move.l $1e(a5), d0
            next = 0x12244U;
            r.data[0] = m.lng(r.address[5] + 0x1eU);
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x12244U: { // d1ad0012 add.l d0, $12(a5)
            next = 0x12248U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x12U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x12248U: { // 202d0022 move.l $22(a5), d0
            next = 0x1224cU;
            r.data[0] = m.lng(r.address[5] + 0x22U);
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1224cU: { // 04ad000040000022 subi.l #$4000, $22(a5)
            next = 0x12254U;
            const auto source_value = 0x4000U;
            const auto destination_address = r.address[5] + 0x22U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.sub(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x12254U: { // d1ad0016 add.l d0, $16(a5)
            next = 0x12258U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x16U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x12258U: { // 202d0026 move.l $26(a5), d0
            next = 0x1225cU;
            r.data[0] = m.lng(r.address[5] + 0x26U);
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1225cU: { // d1ad001a add.l d0, $1a(a5)
            next = 0x12260U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x12260U: { // 086d0000003c bchg.b #$0, $3c(a5)
            next = 0x12266U;
            const auto destination_address = r.address[5] + 0x3cU;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old ^ bit_mask);
            break;
        }
        case 0x12266U: { // 66000098 bne.w $12300
            next = 0x1226aU;
            if ((r.status & 4U) == 0U) { next = 0x12300U; transfer_kind = 1U; }
            break;
        }
        case 0x1226aU: { // 522d003d addq.b #$1, $3d(a5)
            next = 0x1226eU;
            const auto address = r.address[5] + 0x3dU;
            const auto value = m.add(m.byte(address), 0x1U, 8U);
            m.byte(address, value);
            break;
        }
        case 0x1226eU: { // 0c2d0005003d cmpi.b #$5, $3d(a5)
            next = 0x12274U;
            const auto source_value = 0x5U;
            const auto destination_address = r.address[5] + 0x3dU;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x12274U: { // 6b6c bmi.b $122e2
            next = 0x12276U;
            if ((r.status & 8U) != 0U) { next = 0x122e2U; transfer_kind = 1U; }
            break;
        }
        case 0x12276U: { // 1b7c0003003d move.b #$3, $3d(a5)
            next = 0x1227cU;
            m.byte(r.address[5] + 0x3dU, 0x3U);
            m.logic(0x3U, 8U);
            break;
        }
        case 0x1227cU: { // 6064 bra.b $122e2
            next = 0x1227eU;
            if (true) { next = 0x122e2U; transfer_kind = 1U; }
            break;
        }
        case 0x1227eU: { // 6100071c bsr.w $1299c
            next = 0x12282U;
            const auto result = m.call(c, 535U, 0x1227eU, 0x1299cU, 0x12282U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12282U: { // 086d0000003c bchg.b #$0, $3c(a5)
            next = 0x12288U;
            const auto destination_address = r.address[5] + 0x3cU;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old ^ bit_mask);
            break;
        }
        case 0x12288U: { // 6676 bne.b $12300
            next = 0x1228aU;
            if ((r.status & 4U) == 0U) { next = 0x12300U; transfer_kind = 1U; }
            break;
        }
        case 0x1228aU: { // 522d003d addq.b #$1, $3d(a5)
            next = 0x1228eU;
            const auto address = r.address[5] + 0x3dU;
            const auto value = m.add(m.byte(address), 0x1U, 8U);
            m.byte(address, value);
            break;
        }
        case 0x1228eU: { // 0c2d0005003d cmpi.b #$5, $3d(a5)
            next = 0x12294U;
            const auto source_value = 0x5U;
            const auto destination_address = r.address[5] + 0x3dU;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x12294U: { // 6b4c bmi.b $122e2
            next = 0x12296U;
            if ((r.status & 8U) != 0U) { next = 0x122e2U; transfer_kind = 1U; }
            break;
        }
        case 0x12296U: { // 3b7c005a0046 move.w #$5a, $46(a5)
            next = 0x1229cU;
            m.word(r.address[5] + 0x46U, 0x5aU);
            m.logic(0x5aU, 16U);
            break;
        }
        case 0x1229cU: { // 603c bra.b $122da
            next = 0x1229eU;
            if (true) { next = 0x122daU; transfer_kind = 1U; }
            break;
        }
        case 0x1229eU: { // 610006fc bsr.w $1299c
            next = 0x122a2U;
            const auto result = m.call(c, 535U, 0x1229eU, 0x1299cU, 0x122a2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x122a2U: { // 536d0046 subq.w #$1, $46(a5)
            next = 0x122a6U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x46U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x122a6U: { // 6f8a ble.b $12232
            next = 0x122a8U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x12232U; transfer_kind = 1U; }
            break;
        }
        case 0x122a8U: { // 086d0000003c bchg.b #$0, $3c(a5)
            next = 0x122aeU;
            const auto destination_address = r.address[5] + 0x3cU;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old ^ bit_mask);
            break;
        }
        case 0x122aeU: { // 6650 bne.b $12300
            next = 0x122b0U;
            if ((r.status & 4U) == 0U) { next = 0x12300U; transfer_kind = 1U; }
            break;
        }
        case 0x122b0U: { // 522d003d addq.b #$1, $3d(a5)
            next = 0x122b4U;
            const auto address = r.address[5] + 0x3dU;
            const auto value = m.add(m.byte(address), 0x1U, 8U);
            m.byte(address, value);
            break;
        }
        case 0x122b4U: { // 022d0003003d andi.b #$3, $3d(a5)
            next = 0x122baU;
            const auto source_value = 0x3U;
            const auto destination_address = r.address[5] + 0x3dU;
            const auto destination_value = m.byte(destination_address);
            const auto value = destination_value & source_value;
            m.logic(value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x122baU: { // 6026 bra.b $122e2
            next = 0x122bcU;
            if (true) { next = 0x122e2U; transfer_kind = 1U; }
            break;
        }
        case 0x122bcU: { // 086d0000003c bchg.b #$0, $3c(a5)
            next = 0x122c2U;
            const auto destination_address = r.address[5] + 0x3cU;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old ^ bit_mask);
            break;
        }
        case 0x122c2U: { // 663c bne.b $12300
            next = 0x122c4U;
            if ((r.status & 4U) == 0U) { next = 0x12300U; transfer_kind = 1U; }
            break;
        }
        case 0x122c4U: { // 522d003d addq.b #$1, $3d(a5)
            next = 0x122c8U;
            const auto address = r.address[5] + 0x3dU;
            const auto value = m.add(m.byte(address), 0x1U, 8U);
            m.byte(address, value);
            break;
        }
        case 0x122c8U: { // 0c2d0005003d cmpi.b #$5, $3d(a5)
            next = 0x122ceU;
            const auto source_value = 0x5U;
            const auto destination_address = r.address[5] + 0x3dU;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x122ceU: { // 6b12 bmi.b $122e2
            next = 0x122d0U;
            if ((r.status & 8U) != 0U) { next = 0x122e2U; transfer_kind = 1U; }
            break;
        }
        case 0x122d0U: { // 61000a36 bsr.w $12d08
            next = 0x122d4U;
            const auto result = m.call(c, 261U, 0x122d0U, 0x12d08U, 0x122d4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x122d4U: { // 4e75 rts 
            next = 0x122d6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x122d4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x122d6U: { // 422d003c clr.b $3c(a5)
            next = 0x122daU;
            const auto destination_address = r.address[5] + 0x3cU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x122daU: { // 526d0042 addq.w #$1, $42(a5)
            next = 0x122deU;
            const auto address = r.address[5] + 0x42U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0x122deU: { // 422d003d clr.b $3d(a5)
            next = 0x122e2U;
            const auto destination_address = r.address[5] + 0x3dU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x122e2U: { // 302d0042 move.w $42(a5), d0
            next = 0x122e6U;
            m.dw(0U, m.word(r.address[5] + 0x42U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0x122e6U: { // e540 asl.w #$2, d0
            next = 0x122e8U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x122e8U: { // d06d0042 add.w $42(a5), d0
            next = 0x122ecU;
            const auto source_address = r.address[5] + 0x42U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x122ecU: { // d02d003d add.b $3d(a5), d0
            next = 0x122f0U;
            const auto source_address = r.address[5] + 0x3dU;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x122f0U: { // e540 asl.w #$2, d0
            next = 0x122f2U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x122f2U: { // 41fa0b7a lea.l $12e6e(pc), a0
            next = 0x122f6U;
            const auto source_address = 0x12e6eU;
            r.address[0] = source_address;
            break;
        }
        case 0x122f6U: { // d0c0 adda.w d0, a0
            next = 0x122f8U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x122f8U: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x122fcU;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x122fcU: { // 1b580001 move.b (a0)+, $1(a5)
            next = 0x12300U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x12300U: { // 302d0042 move.w $42(a5), d0
            next = 0x12304U;
            m.dw(0U, m.word(r.address[5] + 0x42U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0x12304U: { // e540 asl.w #$2, d0
            next = 0x12306U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x12306U: { // d06d0042 add.w $42(a5), d0
            next = 0x1230aU;
            const auto source_address = r.address[5] + 0x42U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1230aU: { // d02d003d add.b $3d(a5), d0
            next = 0x1230eU;
            const auto source_address = r.address[5] + 0x3dU;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x1230eU: { // d040 add.w d0, d0
            next = 0x12310U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x12310U: { // 41fa0bac lea.l $12ebe(pc), a0
            next = 0x12314U;
            const auto source_address = 0x12ebeU;
            r.address[0] = source_address;
            break;
        }
        case 0x12314U: { // d0c0 adda.w d0, a0
            next = 0x12316U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x12316U: { // 302d0016 move.w $16(a5), d0
            next = 0x1231aU;
            m.dw(0U, m.word(r.address[5] + 0x16U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0x1231aU: { // e240 asr.w #$1, d0
            next = 0x1231cU;
            m.shift_word(0U, 1U, false, true);
            break;
        }
        case 0x1231cU: { // 3200 move.w d0, d1
            next = 0x1231eU;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1231eU: { // d018 add.b (a0)+, d0
            next = 0x12320U;
            const auto source_address = r.address[0];
            const auto source_value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x12320U: { // 6f04 ble.b $12326
            next = 0x12322U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x12326U; transfer_kind = 1U; }
            break;
        }
        case 0x12322U: { // d218 add.b (a0)+, d1
            next = 0x12324U;
            const auto source_address = r.address[0];
            const auto source_value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(1U, value);
            break;
        }
        case 0x12324U: { // 6e04 bgt.b $1232a
            next = 0x12326U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1232aU; transfer_kind = 1U; }
            break;
        }
        case 0x12326U: { // 4215 clr.b (a5)
            next = 0x12328U;
            const auto destination_address = r.address[5];
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x12328U: { // 4e75 rts 
            next = 0x1232aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x12328U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1232aU: { // 1b400010 move.b d0, $10(a5)
            next = 0x1232eU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x10U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1232eU: { // 1b410011 move.b d1, $11(a5)
            next = 0x12332U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x11U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x12332U: { // 4eb900015d24 jsr $15d24.l
            next = 0x12338U;
            const auto result = m.call(c, 280U, 0x12332U, 0x15d24U, 0x12338U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12338U: { // 4eb900015d3c jsr $15d3c.l
            next = 0x1233eU;
            const auto result = m.call(c, 281U, 0x12338U, 0x15d3cU, 0x1233eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1233eU: { // 4eb900015df2 jsr $15df2.l
            next = 0x12344U;
            const auto result = m.call(c, 282U, 0x1233eU, 0x15df2U, 0x12344U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12344U: { // 4e75 rts 
            next = 0x12346U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x12344U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x121f4U:
        case 0x121f8U:
        case 0x121faU:
        case 0x121feU:
        case 0x12202U:
        case 0x12206U:
        case 0x1220aU:
        case 0x1220eU:
        case 0x12212U:
        case 0x12214U:
        case 0x12218U:
        case 0x1221cU:
        case 0x12220U:
        case 0x12222U:
        case 0x12226U:
        case 0x1222aU:
        case 0x1222cU:
        case 0x1222eU:
        case 0x12232U:
        case 0x12236U:
        case 0x1223cU:
        case 0x12240U:
        case 0x12244U:
        case 0x12248U:
        case 0x1224cU:
        case 0x12254U:
        case 0x12258U:
        case 0x1225cU:
        case 0x12260U:
        case 0x12266U:
        case 0x1226aU:
        case 0x1226eU:
        case 0x12274U:
        case 0x12276U:
        case 0x1227cU:
        case 0x1227eU:
        case 0x12282U:
        case 0x12288U:
        case 0x1228aU:
        case 0x1228eU:
        case 0x12294U:
        case 0x12296U:
        case 0x1229cU:
        case 0x1229eU:
        case 0x122a2U:
        case 0x122a6U:
        case 0x122a8U:
        case 0x122aeU:
        case 0x122b0U:
        case 0x122b4U:
        case 0x122baU:
        case 0x122bcU:
        case 0x122c2U:
        case 0x122c4U:
        case 0x122c8U:
        case 0x122ceU:
        case 0x122d0U:
        case 0x122d4U:
        case 0x122d6U:
        case 0x122daU:
        case 0x122deU:
        case 0x122e2U:
        case 0x122e6U:
        case 0x122e8U:
        case 0x122ecU:
        case 0x122f0U:
        case 0x122f2U:
        case 0x122f6U:
        case 0x122f8U:
        case 0x122fcU:
        case 0x12300U:
        case 0x12304U:
        case 0x12306U:
        case 0x1230aU:
        case 0x1230eU:
        case 0x12310U:
        case 0x12314U:
        case 0x12316U:
        case 0x1231aU:
        case 0x1231cU:
        case 0x1231eU:
        case 0x12320U:
        case 0x12322U:
        case 0x12324U:
        case 0x12326U:
        case 0x12328U:
        case 0x1232aU:
        case 0x1232eU:
        case 0x12332U:
        case 0x12338U:
        case 0x1233eU:
        case 0x12344U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
