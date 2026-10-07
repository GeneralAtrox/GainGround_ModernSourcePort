#include "cpu_b_build_sprite_lists_detail.h"

namespace gain_ground::translated::cpu_b_build_sprite_lists_detail {

bool dispatch_pc_range_04(FunctionContext &,
    CpuRegisters &registers, unverified::Machine &machine,
    CpuBIrqTiming &timing, std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind)
{
    auto &r = registers;
    auto &m = machine;
    auto &t = timing;
    switch (pc) {
        case 0x16e66U: { // 6704: bcc
            next = 0x16e68U;
            const bool taken = scene_timing::condition(r.status, 7U);
            next = t.branch(pc, 0x16e6cU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16e68U: { // 08c2000f: immediate-bit-register
            next = 0x16e6cU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto mask = 0x8000U;
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[2] & mask) ? 0U : 4U));
            r.data[2] |= mask;
            t.clocks(2U);
            break;
        }
        case 0x16e6cU: { // 8642: register-or
            next = 0x16e6eU;
            const auto value = r.data[3] | r.data[2];
            m.logic(value, 16U);
            m.dw(3U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e6eU: { // 3ac3: move-to-indirect
            next = 0x16e70U;
            const auto destination = r.address[5];
            m.logic(r.data[3], 16U);
            t.word(destination, r.data[3]);
            r.address[5] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e70U: { // 8842: register-or
            next = 0x16e72U;
            const auto value = r.data[4] | r.data[2];
            m.logic(value, 16U);
            m.dw(4U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e72U: { // 32c4: move-to-indirect
            next = 0x16e74U;
            const auto destination = r.address[1];
            m.logic(r.data[4], 16U);
            t.word(destination, r.data[4]);
            r.address[1] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e74U: { // 7400: moveq
            next = 0x16e76U;
            r.data[2] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e76U: { // 141b: move-to-data-register
            next = 0x16e78U;
            const auto source = r.address[3];
            r.address[3] += 1U;
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(2U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e78U: { // 362e000c: move-to-data-register
            next = 0x16e7cU;
            t.prefetch(pc + 4U);
            const auto source = (r.address[6] + 0xcU);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(3U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16e7cU: { // 7004: moveq
            next = 0x16e7eU;
            r.data[0] = 0x4U;
            m.logic(0x4U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e7eU: { // e560: shift-word
            next = 0x16e80U;
            scene_timing::shift_word(t, m, pc, 0U, (r.data[2] & 63U), true, true);
            break;
        }
        case 0x16e80U: { // 9640: register-sub
            next = 0x16e82U;
            const auto value = m.sub(r.data[3], r.data[0], 16U);
            m.dw(3U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e82U: { // 5043: quick-register
            next = 0x16e84U;
            const auto value = m.add(r.data[3], 8U, 16U);
            m.dw(3U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e84U: { // 3803: move-to-data-register
            next = 0x16e86U;
            m.logic(r.data[3], 16U);
            m.dw(4U, r.data[3]);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e86U: { // 08d60000: immediate-bit-memory
            next = 0x16e8aU;
            t.prefetch(pc + 4U);
            const auto address = r.address[6];
            const auto old = t.byte(address);
            t.prefetch(pc + 6U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 0x1U) ? 0U : 4U));
            t.byte(address, old | 0x1U);
            break;
        }
        case 0x16e8aU: { // 6706: bcc
            next = 0x16e8cU;
            const bool taken = scene_timing::condition(r.status, 7U);
            next = t.branch(pc, 0x16e92U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16e8cU: { // d86e007c: word-source-add
            next = 0x16e90U;
            const auto address = (r.address[6] + 0x7cU);
            t.prefetch(pc + 4U);
            const auto source = t.word(address);
            m.dw(4U, m.add(r.data[4], source, 16U));
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16e90U: { // e244: shift-word
            next = 0x16e92U;
            scene_timing::shift_word(t, m, pc, 4U, 1U, false, true);
            break;
        }
        case 0x16e92U: { // 3d43007c: move-to-displacement
            next = 0x16e96U;
            const auto destination = (r.address[6] + 0x7cU);
            t.prefetch(pc + 4U);
            m.logic(r.data[3], 16U);
            t.word(destination, r.data[3]);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16e96U: { // 02430fff: immediate-and
            next = 0x16e9aU;
            t.prefetch(pc + 4U);
            const auto value = r.data[3] & 0xfffU;
            m.logic(value, 16U);
            m.dw(3U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16e9aU: { // 02440fff: immediate-and
            next = 0x16e9eU;
            t.prefetch(pc + 4U);
            const auto value = r.data[4] & 0xfffU;
            m.logic(value, 16U);
            m.dw(4U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16e9eU: { // 4842: swap
            next = 0x16ea0U;
            const auto value = (r.data[2] << 16U) | (r.data[2] >> 16U);
            r.data[2] = value; m.logic(value, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16ea0U: { // e88a: lsr-long-immediate
            next = 0x16ea2U;
            scene_timing::lsr_long(t, m, pc, 2U, 4U);
            break;
        }
        case 0x16ea2U: { // 082e00000001: immediate-bit-memory
            next = 0x16ea8U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto address = (r.address[6] + 0x1U);
            const auto old = t.byte(address);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 0x1U) ? 0U : 4U));
            t.prefetch(pc + 8U);
            break;
        }
        case 0x16ea8U: { // 6704: bcc
            next = 0x16eaaU;
            const bool taken = scene_timing::condition(r.status, 7U);
            next = t.branch(pc, 0x16eaeU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16eaaU: { // 08c2000f: immediate-bit-register
            next = 0x16eaeU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto mask = 0x8000U;
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[2] & mask) ? 0U : 4U));
            r.data[2] |= mask;
            t.clocks(2U);
            break;
        }
        case 0x16eaeU: { // 8642: register-or
            next = 0x16eb0U;
            const auto value = r.data[3] | r.data[2];
            m.logic(value, 16U);
            m.dw(3U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16eb0U: { // 3ac3: move-to-indirect
            next = 0x16eb2U;
            const auto destination = r.address[5];
            m.logic(r.data[3], 16U);
            t.word(destination, r.data[3]);
            r.address[5] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16eb2U: { // 8842: register-or
            next = 0x16eb4U;
            const auto value = r.data[4] | r.data[2];
            m.logic(value, 16U);
            m.dw(4U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16eb4U: { // 32c4: move-to-indirect
            next = 0x16eb6U;
            const auto destination = r.address[1];
            m.logic(r.data[4], 16U);
            t.word(destination, r.data[4]);
            r.address[1] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16eb6U: { // 584d: quick-word-address
            next = 0x16eb8U;
            scene_timing::address_add(t, r, 5U, 0x4U, pc + 4U);
            break;
        }
        case 0x16eb8U: { // 5849: quick-word-address
            next = 0x16ebaU;
            scene_timing::address_add(t, r, 1U, 0x4U, pc + 4U);
            break;
        }
        case 0x16ebaU: { // 51cefe86: dbf
            next = 0x16ebeU;
            next = t.dbf(pc, 0x16d42U, 6U);
            if ((r.data[6] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0x16ebeU: { // 4a786c00: tst-memory
            next = 0x16ec2U;
            t.prefetch(pc + 4U);
            const auto address = 0x6c00U;
            const auto value = t.word(address);
            m.logic(value, 16U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16ec2U: { // 6a14: bcc
            next = 0x16ec4U;
            const bool taken = scene_timing::condition(r.status, 10U);
            next = t.branch(pc, 0x16ed8U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16ec4U: { // 3abc8700: move-to-indirect
            next = 0x16ec8U;
            t.prefetch(pc + 4U);
            const auto destination = r.address[5];
            m.logic(0x8700U, 16U);
            t.word(destination, 0x8700U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16ec8U: { // 32bc8700: move-to-indirect
            next = 0x16eccU;
            t.prefetch(pc + 4U);
            const auto destination = r.address[1];
            m.logic(0x8700U, 16U);
            t.word(destination, 0x8700U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16eccU: { // 4bf900607000: lea-absolute-long
            next = 0x16ed2U;
            const auto value = 0x607000U;
            r.address[5] = (r.address[5] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[5] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x16ed2U: { // 3e3c0700: move-to-data-register
            next = 0x16ed6U;
            t.prefetch(pc + 4U);
            m.logic(0x700U, 16U);
            m.dw(7U, 0x700U);
            t.prefetch(pc + 6U);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_build_sprite_lists_detail
