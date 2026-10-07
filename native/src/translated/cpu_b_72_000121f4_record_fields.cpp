#include "proven_static_cpu_b_72_000121f4_detail.h"

namespace gain_ground::translated::proven_static_cpu_b_72_000121f4_detail {

bool dispatch_record_fields(FunctionContext &, CpuRegisters &registers,
    unverified::Machine &machine, std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind)
{
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
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
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::proven_static_cpu_b_72_000121f4_detail
