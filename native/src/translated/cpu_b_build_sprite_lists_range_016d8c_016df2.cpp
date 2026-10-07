#include "cpu_b_build_sprite_lists_detail.h"

namespace gain_ground::translated::cpu_b_build_sprite_lists_detail {

bool dispatch_pc_range_02(FunctionContext &,
    CpuRegisters &registers, unverified::Machine &machine,
    CpuBIrqTiming &timing, std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind)
{
    auto &r = registers;
    auto &m = machine;
    auto &t = timing;
    switch (pc) {
        case 0x16d8cU: { // 32db: move-to-indirect
            next = 0x16d8eU;
            const auto source = r.address[3];
            r.address[3] += 2U;
            const auto value = t.word(source);
            const auto destination = r.address[1];
            m.logic(value, 16U);
            t.word(destination, value);
            r.address[1] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d8eU: { // 3aee0008: move-to-indirect
            next = 0x16d92U;
            t.prefetch(pc + 4U);
            const auto source = (r.address[6] + 0x8U);
            const auto value = t.word(source);
            const auto destination = r.address[5];
            m.logic(value, 16U);
            t.word(destination, value);
            r.address[5] += 2U;
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16d92U: { // 32ee0008: move-to-indirect
            next = 0x16d96U;
            t.prefetch(pc + 4U);
            const auto source = (r.address[6] + 0x8U);
            const auto value = t.word(source);
            const auto destination = r.address[1];
            m.logic(value, 16U);
            t.word(destination, value);
            r.address[1] += 2U;
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16d96U: { // 7400: moveq
            next = 0x16d98U;
            r.data[2] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d98U: { // 141b: move-to-data-register
            next = 0x16d9aU;
            const auto source = r.address[3];
            r.address[3] += 1U;
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(2U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16d9aU: { // 362e000e: move-to-data-register
            next = 0x16d9eU;
            t.prefetch(pc + 4U);
            const auto source = (r.address[6] + 0xeU);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(3U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16d9eU: { // 5241: quick-register
            next = 0x16da0U;
            const auto value = m.add(r.data[1], 1U, 16U);
            m.dw(1U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16da0U: { // e561: shift-word
            next = 0x16da2U;
            scene_timing::shift_word(t, m, pc, 1U, (r.data[2] & 63U), true, true);
            break;
        }
        case 0x16da2U: { // e841: shift-word
            next = 0x16da4U;
            scene_timing::shift_word(t, m, pc, 1U, 4U, false, true);
            break;
        }
        case 0x16da4U: { // 9641: register-sub
            next = 0x16da6U;
            const auto value = m.sub(r.data[3], r.data[1], 16U);
            m.dw(3U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16da6U: { // 3803: move-to-data-register
            next = 0x16da8U;
            m.logic(r.data[3], 16U);
            m.dw(4U, r.data[3]);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16da8U: { // 08160000: immediate-bit-memory
            next = 0x16dacU;
            t.prefetch(pc + 4U);
            const auto address = r.address[6];
            const auto old = t.byte(address);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 0x1U) ? 0U : 4U));
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16dacU: { // 6706: bcc
            next = 0x16daeU;
            const bool taken = scene_timing::condition(r.status, 7U);
            next = t.branch(pc, 0x16db4U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16daeU: { // d86e007e: word-source-add
            next = 0x16db2U;
            const auto address = (r.address[6] + 0x7eU);
            t.prefetch(pc + 4U);
            const auto source = t.word(address);
            m.dw(4U, m.add(r.data[4], source, 16U));
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16db2U: { // e244: shift-word
            next = 0x16db4U;
            scene_timing::shift_word(t, m, pc, 4U, 1U, false, true);
            break;
        }
        case 0x16db4U: { // 3d43007e: move-to-displacement
            next = 0x16db8U;
            const auto destination = (r.address[6] + 0x7eU);
            t.prefetch(pc + 4U);
            m.logic(r.data[3], 16U);
            t.word(destination, r.data[3]);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16db8U: { // 02430fff: immediate-and
            next = 0x16dbcU;
            t.prefetch(pc + 4U);
            const auto value = r.data[3] & 0xfffU;
            m.logic(value, 16U);
            m.dw(3U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16dbcU: { // 02440fff: immediate-and
            next = 0x16dc0U;
            t.prefetch(pc + 4U);
            const auto value = r.data[4] & 0xfffU;
            m.logic(value, 16U);
            m.dw(4U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16dc0U: { // 4842: swap
            next = 0x16dc2U;
            const auto value = (r.data[2] << 16U) | (r.data[2] >> 16U);
            r.data[2] = value; m.logic(value, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16dc2U: { // e88a: lsr-long-immediate
            next = 0x16dc4U;
            scene_timing::lsr_long(t, m, pc, 2U, 4U);
            break;
        }
        case 0x16dc4U: { // 082e00010001: immediate-bit-memory
            next = 0x16dcaU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto address = (r.address[6] + 0x1U);
            const auto old = t.byte(address);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 0x2U) ? 0U : 4U));
            t.prefetch(pc + 8U);
            break;
        }
        case 0x16dcaU: { // 6704: bcc
            next = 0x16dccU;
            const bool taken = scene_timing::condition(r.status, 7U);
            next = t.branch(pc, 0x16dd0U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16dccU: { // 08c2000f: immediate-bit-register
            next = 0x16dd0U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto mask = 0x8000U;
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[2] & mask) ? 0U : 4U));
            r.data[2] |= mask;
            t.clocks(2U);
            break;
        }
        case 0x16dd0U: { // 8642: register-or
            next = 0x16dd2U;
            const auto value = r.data[3] | r.data[2];
            m.logic(value, 16U);
            m.dw(3U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16dd2U: { // 3ac3: move-to-indirect
            next = 0x16dd4U;
            const auto destination = r.address[5];
            m.logic(r.data[3], 16U);
            t.word(destination, r.data[3]);
            r.address[5] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16dd4U: { // 8842: register-or
            next = 0x16dd6U;
            const auto value = r.data[4] | r.data[2];
            m.logic(value, 16U);
            m.dw(4U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16dd6U: { // 32c4: move-to-indirect
            next = 0x16dd8U;
            const auto destination = r.address[1];
            m.logic(r.data[4], 16U);
            t.word(destination, r.data[4]);
            r.address[1] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16dd8U: { // 7400: moveq
            next = 0x16ddaU;
            r.data[2] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16ddaU: { // 141b: move-to-data-register
            next = 0x16ddcU;
            const auto source = r.address[3];
            r.address[3] += 1U;
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(2U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16ddcU: { // 362e000c: move-to-data-register
            next = 0x16de0U;
            t.prefetch(pc + 4U);
            const auto source = (r.address[6] + 0xcU);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(3U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16de0U: { // 5240: quick-register
            next = 0x16de2U;
            const auto value = m.add(r.data[0], 1U, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16de2U: { // e560: shift-word
            next = 0x16de4U;
            scene_timing::shift_word(t, m, pc, 0U, (r.data[2] & 63U), true, true);
            break;
        }
        case 0x16de4U: { // e840: shift-word
            next = 0x16de6U;
            scene_timing::shift_word(t, m, pc, 0U, 4U, false, true);
            break;
        }
        case 0x16de6U: { // 9640: register-sub
            next = 0x16de8U;
            const auto value = m.sub(r.data[3], r.data[0], 16U);
            m.dw(3U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16de8U: { // 5043: quick-register
            next = 0x16deaU;
            const auto value = m.add(r.data[3], 8U, 16U);
            m.dw(3U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16deaU: { // 3803: move-to-data-register
            next = 0x16decU;
            m.logic(r.data[3], 16U);
            m.dw(4U, r.data[3]);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16decU: { // 08d60000: immediate-bit-memory
            next = 0x16df0U;
            t.prefetch(pc + 4U);
            const auto address = r.address[6];
            const auto old = t.byte(address);
            t.prefetch(pc + 6U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 0x1U) ? 0U : 4U));
            t.byte(address, old | 0x1U);
            break;
        }
        case 0x16df0U: { // 6706: bcc
            next = 0x16df2U;
            const bool taken = scene_timing::condition(r.status, 7U);
            next = t.branch(pc, 0x16df8U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16df2U: { // d86e007c: word-source-add
            next = 0x16df6U;
            const auto address = (r.address[6] + 0x7cU);
            t.prefetch(pc + 4U);
            const auto source = t.word(address);
            m.dw(4U, m.add(r.data[4], source, 16U));
            t.prefetch(pc + 6U);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_build_sprite_lists_detail
