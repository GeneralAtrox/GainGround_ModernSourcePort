#include "cpu_b_advance_phase_palette_overlay_dispatch_detail.h"

namespace gain_ground::translated::cpu_b_advance_phase_palette_overlay_dispatch_detail {

bool dispatch_overlay_updates(FunctionContext &, CpuRegisters &registers,
    unverified::Machine &machine, CpuBIrqTiming &timing, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind)
{
    auto &r = registers;
    auto &m = machine;
    auto &t = timing;
    switch (pc) {
        case 0xd402U: { // e740: shift-word
            next = 0xd404U;
            scene_timing::shift_word(t, m, pc, 0U, 3U, true, true);
            break;
        }
        case 0xd404U: { // 323c0198: move-to-data-register
            next = 0xd408U;
            t.prefetch(pc + 4U);
            m.logic(0x198U, 16U);
            m.dw(1U, 0x198U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd408U: { // 9240: register-sub
            next = 0xd40aU;
            const auto value = m.sub(r.data[1], r.data[0], 16U);
            m.dw(1U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xd40aU: { // 31c1740e: move-to-displacement
            next = 0xd40eU;
            const auto destination = 0x740eU;
            m.logic(r.data[1], 16U);
            t.prefetch(pc + 4U);
            t.word(destination, r.data[1]);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd40eU: { // d07cff78: word-source-add
            next = 0xd412U;
            const auto source = 0xff78U;
            t.prefetch(pc + 4U);
            m.dw(0U, m.add(r.data[0], source, 16U));
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd412U: { // 31c07404: move-to-displacement
            next = 0xd416U;
            const auto destination = 0x7404U;
            m.logic(r.data[0], 16U);
            t.prefetch(pc + 4U);
            t.word(destination, r.data[0]);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd416U: { // 526d0010: quick-memory
            next = 0xd41aU;
            const auto address = (r.address[5] + 0x10U);
            t.prefetch(pc + 4U);
            const auto old = t.word(address);
            const auto value = m.add(old, 1U, 16U);
            t.prefetch(pc + 6U);
            t.word(address, value);
            break;
        }
        case 0xd41aU: { // 0c6d002d0010: compare-immediate-word-memory
            next = 0xd420U;
            t.prefetch(pc + 4U);
            const auto address = (r.address[5] + 0x10U);
            t.prefetch(pc + 6U);
            const auto old = t.word(address);
            (void)m.sub(old, 0x2dU, 16U, true);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd420U: { // 6f14: bcc
            next = 0xd422U;
            const bool taken = scene_timing::condition(r.status, 15U);
            next = t.branch(pc, 0xd436U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xd422U: { // 3b7c0001000c: move-immediate-16-to-displacement
            next = 0xd428U;
            const auto value = 0x1U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0xcU);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd428U: { // 426d0010: clear-memory
            next = 0xd42cU;
            const auto address = (r.address[5] + 0x10U);
            t.prefetch(pc + 4U);
            (void)t.word(address);
            m.logic(0U, 16U);
            t.prefetch(pc + 6U);
            t.word(address, 0U);
            break;
        }
        case 0xd42cU: { // 303c0052: move-to-data-register
            next = 0xd430U;
            t.prefetch(pc + 4U);
            m.logic(0x52U, 16U);
            m.dw(0U, 0x52U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd438U: { // 302d0010: move-to-data-register
            next = 0xd43cU;
            t.prefetch(pc + 4U);
            const auto source = (r.address[5] + 0x10U);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd43cU: { // 0c400023: immediate-cmp
            next = 0xd440U;
            t.prefetch(pc + 4U);
            const auto value = m.sub(r.data[0], 0x23U, 16U, true);
            (void)value; // CMPI updates flags without storing a result.
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd440U: { // 6e1c: bcc
            next = 0xd442U;
            const bool taken = scene_timing::condition(r.status, 14U);
            next = t.branch(pc, 0xd45eU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xd442U: { // e540: shift-word
            next = 0xd444U;
            scene_timing::shift_word(t, m, pc, 0U, 2U, true, true);
            break;
        }
        case 0xd444U: { // 41f900024f36: lea-absolute-long
            next = 0xd44aU;
            const auto value = 0x24f36U;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd44aU: { // 28700000: move-memory-to-register
            next = 0xd44eU;
            t.clocks(2U);
            const auto source = (r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])) + 0x0U);
            t.prefetch(pc + 4U);
            const auto high = t.word(source);
            const auto low = t.word(source + 2U);
            const auto value = (std::uint32_t(high) << 16U) | low;
            r.address[4] = value;
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd44eU: { // 3e1c: move-to-data-register
            next = 0xd450U;
            const auto source = r.address[4];
            r.address[4] += 2U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(7U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xd450U: { // 47f900024ef6: lea-absolute-long
            next = 0xd456U;
            const auto value = 0x24ef6U;
            r.address[3] = (r.address[3] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[3] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd45aU: { // 51cffffa: dbf
            next = 0xd45eU;
            next = t.dbf(pc, 0xd456U, 7U);
            if ((r.data[7] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0xd45eU: { // 526d0010: quick-memory
            next = 0xd462U;
            const auto address = (r.address[5] + 0x10U);
            t.prefetch(pc + 4U);
            const auto old = t.word(address);
            const auto value = m.add(old, 1U, 16U);
            t.prefetch(pc + 6U);
            t.word(address, value);
            break;
        }
        case 0xd462U: { // 0c6d010b0010: compare-immediate-word-memory
            next = 0xd468U;
            t.prefetch(pc + 4U);
            const auto address = (r.address[5] + 0x10U);
            t.prefetch(pc + 6U);
            const auto old = t.word(address);
            (void)m.sub(old, 0x10bU, 16U, true);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd468U: { // 6f06: bcc
            next = 0xd46aU;
            const bool taken = scene_timing::condition(r.status, 15U);
            next = t.branch(pc, 0xd470U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xd46aU: { // 4ef900009bc4: jmp-absolute-long
            next = 0xd470U;
            t.prefetch(pc + 4U);
            t.prefetch(0x9bc4U); t.prefetch(0x9bc4U + 2U);
            next = 0x9bc4U; transfer_kind = 1U;
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_advance_phase_palette_overlay_dispatch_detail
