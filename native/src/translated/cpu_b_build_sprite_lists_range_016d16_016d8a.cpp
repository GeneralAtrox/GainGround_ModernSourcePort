#include "cpu_b_build_sprite_lists_detail.h"

namespace gain_ground::translated::cpu_b_build_sprite_lists_detail {

bool dispatch_pc_range_01(FunctionContext &,
    CpuRegisters &registers, unverified::Machine &machine,
    CpuBIrqTiming &timing, std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind)
{
    auto &r = registers;
    auto &m = machine;
    auto &t = timing;
    switch (pc) {
        case 0x16d16U: { // 49f86c00: lea
            next = 0x16d1aU;
            r.address[4] = 0x6c00U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16d1aU: { // 4bf900600010: lea-absolute-long
            next = 0x16d20U;
            const auto value = 0x600010U;
            r.address[5] = (r.address[5] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[5] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x16d20U: { // 7e01: moveq
            next = 0x16d22U;
            r.data[7] = 0x1U;
            m.logic(0x1U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d22U: { // 4a5c: tst-memory
            next = 0x16d24U;
            const auto address = r.address[4];
            r.address[4] += 2U;
            const auto value = t.word(address);
            m.logic(value, 16U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d24U: { // 6a0c: bcc
            next = 0x16d26U;
            const bool taken = scene_timing::condition(r.status, 10U);
            next = t.branch(pc, 0x16d32U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16d26U: { // 49f87002: lea
            next = 0x16d2aU;
            r.address[4] = 0x7002U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16d2aU: { // dafc4000: adda-word
            next = 0x16d2eU;
            t.prefetch(pc + 4U);
            scene_timing::address_add(t, r, 5U, 0x4000U, pc + 6U);
            break;
        }
        case 0x16d2eU: { // 3e3c0401: move-to-data-register
            next = 0x16d32U;
            t.prefetch(pc + 4U);
            m.logic(0x401U, 16U);
            m.dw(7U, 0x401U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16d32U: { // 43d5: lea
            next = 0x16d34U;
            r.address[1] = r.address[5];
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d34U: { // d2fc1000: adda-word
            next = 0x16d38U;
            t.prefetch(pc + 4U);
            scene_timing::address_add(t, r, 1U, 0x1000U, pc + 6U);
            break;
        }
        case 0x16d38U: { // 3a07: move-to-data-register
            next = 0x16d3aU;
            m.logic(r.data[7], 16U);
            m.dw(5U, r.data[7]);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d3aU: { // da7c0100: word-source-add
            next = 0x16d3eU;
            const auto source = 0x100U;
            t.prefetch(pc + 4U);
            m.dw(5U, m.add(r.data[5], source, 16U));
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16d3eU: { // 3c3c01ff: move-to-data-register
            next = 0x16d42U;
            t.prefetch(pc + 4U);
            m.logic(0x1ffU, 16U);
            m.dw(6U, 0x1ffU);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16d42U: { // 3c54: move-memory-to-register
            next = 0x16d44U;
            const auto source = r.address[4];
            const auto value = t.word(source);
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d44U: { // 4a5c: tst-memory
            next = 0x16d46U;
            const auto address = r.address[4];
            r.address[4] += 2U;
            const auto value = t.word(address);
            m.logic(value, 16U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d46U: { // 67000172: bcc
            next = 0x16d4aU;
            const bool taken = scene_timing::condition(r.status, 7U);
            next = t.branch_word(pc, 0x16ebaU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16d4aU: { // 5247: quick-register
            next = 0x16d4cU;
            const auto value = m.add(r.data[7], 1U, 16U);
            m.dw(7U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d4cU: { // 3007: move-to-data-register
            next = 0x16d4eU;
            m.logic(r.data[7], 16U);
            m.dw(0U, r.data[7]);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d4eU: { // 08c0000d: immediate-bit-register
            next = 0x16d52U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto mask = 0x2000U;
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[0] & mask) ? 0U : 4U));
            r.data[0] |= mask;
            t.clocks(2U);
            break;
        }
        case 0x16d52U: { // 3ac0: move-to-indirect
            next = 0x16d54U;
            const auto destination = r.address[5];
            m.logic(r.data[0], 16U);
            t.word(destination, r.data[0]);
            r.address[5] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d54U: { // 5245: quick-register
            next = 0x16d56U;
            const auto value = m.add(r.data[5], 1U, 16U);
            m.dw(5U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d56U: { // 3005: move-to-data-register
            next = 0x16d58U;
            m.logic(r.data[5], 16U);
            m.dw(0U, r.data[5]);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d58U: { // 08c0000d: immediate-bit-register
            next = 0x16d5cU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto mask = 0x2000U;
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[0] & mask) ? 0U : 4U));
            r.data[0] |= mask;
            t.clocks(2U);
            break;
        }
        case 0x16d5cU: { // 32c0: move-to-indirect
            next = 0x16d5eU;
            const auto destination = r.address[1];
            m.logic(r.data[0], 16U);
            t.word(destination, r.data[0]);
            r.address[1] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d5eU: { // 47f90003aeca: lea-absolute-long
            next = 0x16d64U;
            const auto value = 0x3aecaU;
            r.address[3] = (r.address[3] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[3] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x16d64U: { // 302e0006: move-to-data-register
            next = 0x16d68U;
            t.prefetch(pc + 4U);
            const auto source = (r.address[6] + 0x6U);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16d68U: { // e540: shift-word
            next = 0x16d6aU;
            scene_timing::shift_word(t, m, pc, 0U, 2U, true, true);
            break;
        }
        case 0x16d6aU: { // d6c0: adda-word
            next = 0x16d6cU;
            scene_timing::address_add(t, r, 3U, static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])), pc + 4U);
            break;
        }
        case 0x16d6cU: { // 082e00020001: immediate-bit-memory
            next = 0x16d72U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto address = (r.address[6] + 0x1U);
            const auto old = t.byte(address);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 0x4U) ? 0U : 4U));
            t.prefetch(pc + 8U);
            break;
        }
        case 0x16d72U: { // 670000ac: bcc
            next = 0x16d76U;
            const bool taken = scene_timing::condition(r.status, 7U);
            next = t.branch_word(pc, 0x16e20U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16d76U: { // 7000: moveq
            next = 0x16d78U;
            r.data[0] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d78U: { // 7200: moveq
            next = 0x16d7aU;
            r.data[1] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d7aU: { // 102e0010: move-to-data-register
            next = 0x16d7eU;
            t.prefetch(pc + 4U);
            const auto source = (r.address[6] + 0x10U);
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16d7eU: { // 122e0011: move-to-data-register
            next = 0x16d82U;
            t.prefetch(pc + 4U);
            const auto source = (r.address[6] + 0x11U);
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(1U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16d82U: { // 1ac0: move-to-indirect
            next = 0x16d84U;
            const auto destination = r.address[5];
            m.logic(r.data[0], 8U);
            t.byte(destination, r.data[0]);
            r.address[5] += 1U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d84U: { // 1ac1: move-to-indirect
            next = 0x16d86U;
            const auto destination = r.address[5];
            m.logic(r.data[1], 8U);
            t.byte(destination, r.data[1]);
            r.address[5] += 1U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d86U: { // 12c0: move-to-indirect
            next = 0x16d88U;
            const auto destination = r.address[1];
            m.logic(r.data[0], 8U);
            t.byte(destination, r.data[0]);
            r.address[1] += 1U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d88U: { // 12c1: move-to-indirect
            next = 0x16d8aU;
            const auto destination = r.address[1];
            m.logic(r.data[1], 8U);
            t.byte(destination, r.data[1]);
            r.address[1] += 1U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d8aU: { // 3ad3: move-to-indirect
            next = 0x16d8cU;
            const auto source = r.address[3];
            const auto value = t.word(source);
            const auto destination = r.address[5];
            m.logic(value, 16U);
            t.word(destination, value);
            r.address[5] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_build_sprite_lists_detail
