#include "proven_static_cpu_b_72_000121f4_detail.h"

namespace gain_ground::translated::proven_static_cpu_b_72_000121f4_detail {

bool dispatch_tail_fields(FunctionContext &, CpuRegisters &registers,
    unverified::Machine &machine, std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind)
{
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
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
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::proven_static_cpu_b_72_000121f4_detail
