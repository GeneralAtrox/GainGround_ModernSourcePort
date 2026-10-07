#include "cpu_b_build_sprite_lists_detail.h"

namespace gain_ground::translated::cpu_b_build_sprite_lists_detail {

bool dispatch_pc_range_03(FunctionContext &,
    CpuRegisters &registers, unverified::Machine &machine,
    CpuBIrqTiming &timing, std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind)
{
    auto &r = registers;
    auto &m = machine;
    auto &t = timing;
    switch (pc) {
        case 0x16df6U: { // e244: shift-word
            next = 0x16df8U;
            scene_timing::shift_word(t, m, pc, 4U, 1U, false, true);
            break;
        }
        case 0x16df8U: { // 3d43007c: move-to-displacement
            next = 0x16dfcU;
            const auto destination = (r.address[6] + 0x7cU);
            t.prefetch(pc + 4U);
            m.logic(r.data[3], 16U);
            t.word(destination, r.data[3]);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16dfcU: { // 02430fff: immediate-and
            next = 0x16e00U;
            t.prefetch(pc + 4U);
            const auto value = r.data[3] & 0xfffU;
            m.logic(value, 16U);
            m.dw(3U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16e00U: { // 02440fff: immediate-and
            next = 0x16e04U;
            t.prefetch(pc + 4U);
            const auto value = r.data[4] & 0xfffU;
            m.logic(value, 16U);
            m.dw(4U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16e04U: { // 4842: swap
            next = 0x16e06U;
            const auto value = (r.data[2] << 16U) | (r.data[2] >> 16U);
            r.data[2] = value; m.logic(value, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e06U: { // e88a: lsr-long-immediate
            next = 0x16e08U;
            scene_timing::lsr_long(t, m, pc, 2U, 4U);
            break;
        }
        case 0x16e08U: { // 082e00000001: immediate-bit-memory
            next = 0x16e0eU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto address = (r.address[6] + 0x1U);
            const auto old = t.byte(address);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 0x1U) ? 0U : 4U));
            t.prefetch(pc + 8U);
            break;
        }
        case 0x16e0eU: { // 6704: bcc
            next = 0x16e10U;
            const bool taken = scene_timing::condition(r.status, 7U);
            next = t.branch(pc, 0x16e14U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16e10U: { // 08c2000f: immediate-bit-register
            next = 0x16e14U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto mask = 0x8000U;
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[2] & mask) ? 0U : 4U));
            r.data[2] |= mask;
            t.clocks(2U);
            break;
        }
        case 0x16e14U: { // 8642: register-or
            next = 0x16e16U;
            const auto value = r.data[3] | r.data[2];
            m.logic(value, 16U);
            m.dw(3U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e16U: { // 3ac3: move-to-indirect
            next = 0x16e18U;
            const auto destination = r.address[5];
            m.logic(r.data[3], 16U);
            t.word(destination, r.data[3]);
            r.address[5] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e18U: { // 8842: register-or
            next = 0x16e1aU;
            const auto value = r.data[4] | r.data[2];
            m.logic(value, 16U);
            m.dw(4U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e1aU: { // 32c4: move-to-indirect
            next = 0x16e1cU;
            const auto destination = r.address[1];
            m.logic(r.data[4], 16U);
            t.word(destination, r.data[4]);
            r.address[1] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e1cU: { // 60000098: bcc
            next = 0x16e20U;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch_word(pc, 0x16eb6U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16e20U: { // 3afc3f3f: move-to-indirect
            next = 0x16e24U;
            t.prefetch(pc + 4U);
            const auto destination = r.address[5];
            m.logic(0x3f3fU, 16U);
            t.word(destination, 0x3f3fU);
            r.address[5] += 2U;
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16e24U: { // 32fc3f3f: move-to-indirect
            next = 0x16e28U;
            t.prefetch(pc + 4U);
            const auto destination = r.address[1];
            m.logic(0x3f3fU, 16U);
            t.word(destination, 0x3f3fU);
            r.address[1] += 2U;
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16e28U: { // 3ad3: move-to-indirect
            next = 0x16e2aU;
            const auto source = r.address[3];
            const auto value = t.word(source);
            const auto destination = r.address[5];
            m.logic(value, 16U);
            t.word(destination, value);
            r.address[5] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e2aU: { // 32db: move-to-indirect
            next = 0x16e2cU;
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
        case 0x16e2cU: { // 3aee0008: move-to-indirect
            next = 0x16e30U;
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
        case 0x16e30U: { // 32ee0008: move-to-indirect
            next = 0x16e34U;
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
        case 0x16e34U: { // 7400: moveq
            next = 0x16e36U;
            r.data[2] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e36U: { // 141b: move-to-data-register
            next = 0x16e38U;
            const auto source = r.address[3];
            r.address[3] += 1U;
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(2U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e38U: { // 362e000e: move-to-data-register
            next = 0x16e3cU;
            t.prefetch(pc + 4U);
            const auto source = (r.address[6] + 0xeU);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(3U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16e3cU: { // 7204: moveq
            next = 0x16e3eU;
            r.data[1] = 0x4U;
            m.logic(0x4U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e3eU: { // e561: shift-word
            next = 0x16e40U;
            scene_timing::shift_word(t, m, pc, 1U, (r.data[2] & 63U), true, true);
            break;
        }
        case 0x16e40U: { // 9641: register-sub
            next = 0x16e42U;
            const auto value = m.sub(r.data[3], r.data[1], 16U);
            m.dw(3U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e42U: { // 3803: move-to-data-register
            next = 0x16e44U;
            m.logic(r.data[3], 16U);
            m.dw(4U, r.data[3]);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e44U: { // 08160000: immediate-bit-memory
            next = 0x16e48U;
            t.prefetch(pc + 4U);
            const auto address = r.address[6];
            const auto old = t.byte(address);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 0x1U) ? 0U : 4U));
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16e48U: { // 6706: bcc
            next = 0x16e4aU;
            const bool taken = scene_timing::condition(r.status, 7U);
            next = t.branch(pc, 0x16e50U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16e4aU: { // d86e007e: word-source-add
            next = 0x16e4eU;
            const auto address = (r.address[6] + 0x7eU);
            t.prefetch(pc + 4U);
            const auto source = t.word(address);
            m.dw(4U, m.add(r.data[4], source, 16U));
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16e4eU: { // e244: shift-word
            next = 0x16e50U;
            scene_timing::shift_word(t, m, pc, 4U, 1U, false, true);
            break;
        }
        case 0x16e50U: { // 3d43007e: move-to-displacement
            next = 0x16e54U;
            const auto destination = (r.address[6] + 0x7eU);
            t.prefetch(pc + 4U);
            m.logic(r.data[3], 16U);
            t.word(destination, r.data[3]);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16e54U: { // 02430fff: immediate-and
            next = 0x16e58U;
            t.prefetch(pc + 4U);
            const auto value = r.data[3] & 0xfffU;
            m.logic(value, 16U);
            m.dw(3U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16e58U: { // 02440fff: immediate-and
            next = 0x16e5cU;
            t.prefetch(pc + 4U);
            const auto value = r.data[4] & 0xfffU;
            m.logic(value, 16U);
            m.dw(4U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16e5cU: { // 4842: swap
            next = 0x16e5eU;
            const auto value = (r.data[2] << 16U) | (r.data[2] >> 16U);
            r.data[2] = value; m.logic(value, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16e5eU: { // e88a: lsr-long-immediate
            next = 0x16e60U;
            scene_timing::lsr_long(t, m, pc, 2U, 4U);
            break;
        }
        case 0x16e60U: { // 082e00010001: immediate-bit-memory
            next = 0x16e66U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto address = (r.address[6] + 0x1U);
            const auto old = t.byte(address);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 0x2U) ? 0U : 4U));
            t.prefetch(pc + 8U);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_build_sprite_lists_detail
