#include "cpu_b_advance_phase_palette_overlay_dispatch_detail.h"

namespace gain_ground::translated::cpu_b_advance_phase_palette_overlay_dispatch_detail {

bool dispatch_lookup_setup(FunctionContext &, CpuRegisters &registers,
    unverified::Machine &machine, CpuBIrqTiming &timing, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind)
{
    auto &r = registers;
    auto &m = machine;
    auto &t = timing;
    switch (pc) {
        case 0xd3a0U: { // 302d000c: move-to-data-register
            next = 0xd3a4U;
            t.prefetch(pc + 4U);
            const auto source = (r.address[5] + 0xcU);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd3a4U: { // e540: shift-word
            next = 0xd3a6U;
            scene_timing::shift_word(t, m, pc, 0U, 2U, true, true);
            break;
        }
        case 0xd3a6U: { // 4efb0002: jmp-pc-indexed
            next = 0xd3aaU;
            t.clocks(2U);
            const auto target = (0xd3a8U + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])) + 0x2U);
            t.clocks(4U); t.prefetch(target); t.prefetch(target + 2U);
            next = target; transfer_kind = 1U;
            break;
        }
        case 0xd3aaU: { // 60000006: bcc
            next = 0xd3aeU;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch_word(pc, 0xd3b2U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xd3aeU: { // 60000088: bcc
            next = 0xd3b2U;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch_word(pc, 0xd438U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xd3b2U: { // 342d0010: move-to-data-register
            next = 0xd3b6U;
            t.prefetch(pc + 4U);
            const auto source = (r.address[5] + 0x10U);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(2U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd3b6U: { // 0c420018: immediate-cmp
            next = 0xd3baU;
            t.prefetch(pc + 4U);
            const auto value = m.sub(r.data[2], 0x18U, 16U, true);
            (void)value; // CMPI updates flags without storing a result.
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd3baU: { // 6a42: bcc
            next = 0xd3bcU;
            const bool taken = scene_timing::condition(r.status, 10U);
            next = t.branch(pc, 0xd3feU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xd3bcU: { // 41f900202000: lea-absolute-long
            next = 0xd3c2U;
            const auto value = 0x202000U;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd3c2U: { // 303c0017: move-to-data-register
            next = 0xd3c6U;
            t.prefetch(pc + 4U);
            m.logic(0x17U, 16U);
            m.dw(0U, 0x17U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd3c6U: { // 9042: register-sub
            next = 0xd3c8U;
            const auto value = m.sub(r.data[0], r.data[2], 16U);
            m.dw(0U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xd3c8U: { // c0fc0062: multiply-unsigned-immediate
            next = 0xd3ccU;
            scene_timing::multiply_unsigned(t, m, pc, 0U, 0x62U);
            break;
        }
        case 0xd3ccU: { // 43f900008960: lea-absolute-long
            next = 0xd3d2U;
            const auto value = 0x8960U;
            r.address[1] = (r.address[1] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[1] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd3d2U: { // d2c0: adda-word
            next = 0xd3d4U;
            scene_timing::address_add(t, r, 1U, static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])), pc + 4U);
            break;
        }
        case 0xd3d4U: { // 3202: move-to-data-register
            next = 0xd3d6U;
            m.logic(r.data[2], 16U);
            m.dw(1U, r.data[2]);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xd3daU: { // 51c9fffa: dbf
            next = 0xd3deU;
            next = t.dbf(pc, 0xd3d6U, 1U);
            if ((r.data[1] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0xd3deU: { // 303c002f: move-to-data-register
            next = 0xd3e2U;
            t.prefetch(pc + 4U);
            m.logic(0x2fU, 16U);
            m.dw(0U, 0x2fU);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd3e2U: { // 9042: register-sub
            next = 0xd3e4U;
            const auto value = m.sub(r.data[0], r.data[2], 16U);
            m.dw(0U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xd3e4U: { // ef40: shift-word
            next = 0xd3e6U;
            scene_timing::shift_word(t, m, pc, 0U, 7U, true, true);
            break;
        }
        case 0xd3e6U: { // 41f900202000: lea-absolute-long
            next = 0xd3ecU;
            const auto value = 0x202000U;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd3ecU: { // d0c0: adda-word
            next = 0xd3eeU;
            scene_timing::address_add(t, r, 0U, static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])), pc + 4U);
            break;
        }
        case 0xd3eeU: { // 43f900009290: lea-absolute-long
            next = 0xd3f4U;
            const auto value = 0x9290U;
            r.address[1] = (r.address[1] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[1] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd3f4U: { // 3202: move-to-data-register
            next = 0xd3f6U;
            m.logic(r.data[2], 16U);
            m.dw(1U, r.data[2]);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xd3faU: { // 51c9fffa: dbf
            next = 0xd3feU;
            next = t.dbf(pc, 0xd3f6U, 1U);
            if ((r.data[1] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0xd3feU: { // 302d0010: move-to-data-register
            next = 0xd402U;
            t.prefetch(pc + 4U);
            const auto source = (r.address[5] + 0x10U);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_advance_phase_palette_overlay_dispatch_detail
