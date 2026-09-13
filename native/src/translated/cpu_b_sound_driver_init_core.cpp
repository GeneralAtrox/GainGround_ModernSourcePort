// Implemented but unverified. Original CPU-B state-72 opcodes 0x17e44..0x17e99.
#include "gain_ground/cpu_b_init_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_sound_driver_init_core(FunctionContext &c) noexcept {
    if (!c.host) return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    CpuBInitTiming m(c);
    auto &r = c.registers;
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next;
        bool returning = false;
        switch (pc) {
        case 0x17e44U: // 49ee0180 LEA $180(A6),A4
            r.address[4] = r.address[6] + 0x180U;
            m.sequential(pc, 2U); next = 0x17e48U; break;
        case 0x17e48U: m.moveq(0U, 0x11, pc); next = 0x17e4aU; break; // 7011
        case 0x17e4aU: m.moveq(1U, 0, pc); next = 0x17e4cU; break; // 7200
        case 0x17e4cU: m.moveq(2U, 0x50, pc); next = 0x17e4eU; break; // 7450
        case 0x17e56U: m.moveq(2U, 0x1f, pc); next = 0x17e58U; break; // 741f
        case 0x17e58U: m.moveq(0U, 0x60, pc); next = 0x17e5aU; break; // 7060
        case 0x17e5aU: m.moveq(1U, 0x7f, pc); next = 0x17e5cU; break; // 727f
        case 0x17e66U: m.moveq(2U, 0x1f, pc); next = 0x17e68U; break; // 741f
        case 0x17e7aU: m.moveq(2U, 3, pc); next = 0x17e7cU; break; // 7403
        case 0x17e7cU: m.moveq(0U, 0, pc); next = 0x17e7eU; break; // 7000
        case 0x17e8eU: m.moveq(2U, 0x23, pc); next = 0x17e90U; break; // 7423
        case 0x17e90U: m.moveq(0U, 0, pc); next = 0x17e92U; break; // 7000
        case 0x17e4eU: // 3881 MOVE.W D1,(A4)
            m.logic(r.data[1], 16U);
            m.write_word(3U, r.address[4] & 0x3ffffU, static_cast<std::uint16_t>(r.data[1]));
            m.sequential(pc, 1U); next = 0x17e50U; break;
        case 0x17e50U: { // d8c2 ADDA.W D2,A4; eight clocks, CCR unchanged.
            const auto value = r.address[4] + static_cast<std::int16_t>(static_cast<std::uint16_t>(r.data[2]));
            r.address[4] = (r.address[4] & 0xffff0000U) | (value & 0xffffU);
            m.prefetch(pc + 4U); m.clocks(2U); r.address[4] = value; m.clocks(2U);
            next = 0x17e52U; break;
        }
        case 0x17e52U: next = m.dbf(pc, 0x17e4eU, 0U); break; // 51c8fffa
        case 0x17e5cU: case 0x17e6cU: case 0x17e76U: { // BSR.W; one real child call.
            const bool channel = pc == 0x17e76U;
            const auto child = m.bsr(channel ? 321U : 319U, pc, channel ? 0x18172U : 0x17ec2U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue; // The child RTS already selected the continuation PC.
        }
        case 0x17e60U: case 0x17e70U: // 5200 ADDQ.B #1,D0
            m.add(0U, 1U, 8U); m.sequential(pc, 1U); next = pc + 2U; break;
        case 0x17e62U: next = m.dbf(pc, 0x17e5cU, 2U); break; // 51cafff8
        case 0x17e68U: // 103c00e0 MOVE.B #$e0,D0
            m.move_immediate_byte(pc, 0U, 0xe0U); next = 0x17e6cU; break;
        case 0x17e72U: next = m.dbf(pc, 0x17e6cU, 2U); break;
        case 0x17e7eU: { // 3dbc00800030 MOVE.W #$80,$30(A6,D0.W)
            m.prefetch(pc + 4U); m.clocks(2U); m.prefetch(pc + 6U);
            const auto address = r.address[6] + 0x30U +
                static_cast<std::int16_t>(static_cast<std::uint16_t>(r.data[0]));
            m.logic(0x80U, 16U); m.write_word(3U, address & 0x3ffffU, 0x80U);
            m.prefetch(pc + 8U); next = 0x17e84U; break;
        }
        case 0x17e84U: // 5840 ADDQ.W #4,D0
            m.add(0U, 4U, 16U); m.sequential(pc, 1U); next = 0x17e86U; break;
        case 0x17e86U: next = m.dbf(pc, 0x17e7eU, 2U); break;
        case 0x17e8aU: // 41ee0000 LEA $0(A6),A0
            r.address[0] = r.address[6]; m.sequential(pc, 2U); next = 0x17e8eU; break;
        case 0x17e92U: { // 20c0 MOVE.L D0,(A0)+, shared RAM alias.
            const auto offset = r.address[0] & 0x3ffffU;
            const auto value = r.data[0];
            m.write_word(3U, offset, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); m.write_word(3U, offset + 2U, static_cast<std::uint16_t>(value));
            r.address[0] += 4U; m.logic(value, 32U);
            m.sequential(pc, 1U); next = 0x17e94U; break;
        }
        case 0x17e94U: next = m.dbf(pc, 0x17e92U, 2U); break;
        case 0x17e98U: next = m.rts(); returning = true; break;
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (auto result = m.boundary(pc, next)) return *result;
        if (returning) return FunctionResult::complete(1U, next);
    }
}
} // namespace gain_ground::translated
