#include "proven_static_cpu_b_72_000121f4_detail.h"

namespace gain_ground::translated::proven_static_cpu_b_72_000121f4_detail {

bool dispatch_descriptor_setup(FunctionContext &, CpuRegisters &registers,
    unverified::Machine &machine, std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind)
{
    auto &r = registers;
    auto &m = machine;
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
        case 0x12212U: { // 6404 bcc.b $12218
            next = 0x12214U;
            if ((r.status & 1U) == 0U) { next = 0x12218U; transfer_kind = 1U; }
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
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::proven_static_cpu_b_72_000121f4_detail
