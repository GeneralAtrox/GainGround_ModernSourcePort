#include "cpu_b_build_sprite_lists_detail.h"

namespace gain_ground::translated::cpu_b_build_sprite_lists_detail {

bool dispatch_pc_range_06(FunctionContext &,
    CpuRegisters &registers, unverified::Machine &machine,
    CpuBIrqTiming &timing, std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind)
{
    auto &r = registers;
    auto &m = machine;
    auto &t = timing;
    switch (pc) {
        case 0x16f40U: { // 9a680004: word-source-sub
            next = 0x16f44U;
            const auto address = (r.address[0] + 0x4U);
            t.prefetch(pc + 4U);
            const auto source = t.word(address);
            m.dw(5U, m.sub(r.data[5], source, 16U));
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16f44U: { // 51cbffc2: dbf
            next = 0x16f48U;
            next = t.dbf(pc, 0x16f08U, 3U);
            if ((r.data[3] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0x16f48U: { // 6002: bcc
            next = 0x16f4aU;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch(pc, 0x16f4cU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16f4aU: { // 504c: quick-word-address
            next = 0x16f4cU;
            scene_timing::address_add(t, r, 4U, 0x8U, pc + 4U);
            break;
        }
        case 0x16f4cU: { // 5c48: quick-word-address
            next = 0x16f4eU;
            scene_timing::address_add(t, r, 0U, 0x6U, pc + 4U);
            break;
        }
        case 0x16f4eU: { // 51ceffa6: dbf
            next = 0x16f52U;
            next = t.dbf(pc, 0x16ef6U, 6U);
            if ((r.data[6] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0x16f52U: { // 3abcffff: move-to-indirect
            next = 0x16f56U;
            t.prefetch(pc + 4U);
            const auto destination = r.address[5];
            m.logic(0xffffU, 16U);
            t.word(destination, 0xffffU);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16f56U: { // 41f86c00: lea
            next = 0x16f5aU;
            r.address[0] = 0x6c00U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16f5aU: { // 4a58: tst-memory
            next = 0x16f5cU;
            const auto address = r.address[0];
            r.address[0] += 2U;
            const auto value = t.word(address);
            m.logic(value, 16U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f5cU: { // 6b04: bcc
            next = 0x16f5eU;
            const bool taken = scene_timing::condition(r.status, 11U);
            next = t.branch(pc, 0x16f62U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16f5eU: { // 41f87002: lea
            next = 0x16f62U;
            r.address[0] = 0x7002U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16f62U: { // 303c00ff: move-to-data-register
            next = 0x16f66U;
            t.prefetch(pc + 4U);
            m.logic(0xffU, 16U);
            m.dw(0U, 0xffU);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16f66U: { // 7200: moveq
            next = 0x16f68U;
            r.data[1] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f68U: { // 20c1: move-long-register-or-immediate-to-memory
            next = 0x16f6aU;
            const auto value = r.data[1];
            const auto destination = r.address[0];
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            r.address[0] += 4U;
            m.logic(value, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f6aU: { // 51c8fffc: dbf
            next = 0x16f6eU;
            next = t.dbf(pc, 0x16f68U, 0U);
            if ((r.data[0] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_build_sprite_lists_detail
