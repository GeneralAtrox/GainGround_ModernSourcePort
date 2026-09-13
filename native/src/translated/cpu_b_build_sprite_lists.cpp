// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_build_sprite_lists(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        bool returned_from_child = false;
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
        case 0x16ed6U: { // 6012: bcc
            next = 0x16ed8U;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch(pc, 0x16eeaU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16ed8U: { // 3abc8300: move-to-indirect
            next = 0x16edcU;
            t.prefetch(pc + 4U);
            const auto destination = r.address[5];
            m.logic(0x8300U, 16U);
            t.word(destination, 0x8300U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16edcU: { // 32bc8300: move-to-indirect
            next = 0x16ee0U;
            t.prefetch(pc + 4U);
            const auto destination = r.address[1];
            m.logic(0x8300U, 16U);
            t.word(destination, 0x8300U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16ee0U: { // 4bf900603000: lea-absolute-long
            next = 0x16ee6U;
            const auto value = 0x603000U;
            r.address[5] = (r.address[5] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[5] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x16ee6U: { // 3e3c0300: move-to-data-register
            next = 0x16eeaU;
            t.prefetch(pc + 4U);
            m.logic(0x300U, 16U);
            m.dw(7U, 0x300U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16eeaU: { // 41fa00a8: lea
            next = 0x16eeeU;
            r.address[0] = 0x16f94U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16eeeU: { // 49f87402: lea
            next = 0x16ef2U;
            r.address[4] = 0x7402U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16ef2U: { // 3c3c000f: move-to-data-register
            next = 0x16ef6U;
            t.prefetch(pc + 4U);
            m.logic(0xfU, 16U);
            m.dw(6U, 0xfU);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16ef6U: { // 3a1c: move-to-data-register
            next = 0x16ef8U;
            const auto source = r.address[4];
            r.address[4] += 2U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(5U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16ef8U: { // 6f000050: bcc
            next = 0x16efcU;
            const bool taken = scene_timing::condition(r.status, 15U);
            next = t.branch_word(pc, 0x16f4aU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16efcU: { // 5045: quick-register
            next = 0x16efeU;
            const auto value = m.add(r.data[5], 8U, 16U);
            m.dw(5U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16efeU: { // 365c: move-memory-to-register
            next = 0x16f00U;
            const auto source = r.address[4];
            r.address[4] += 2U;
            const auto value = t.word(source);
            r.address[3] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f00U: { // 381c: move-to-data-register
            next = 0x16f02U;
            const auto source = r.address[4];
            r.address[4] += 2U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(4U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f02U: { // 2c5c: move-memory-to-register
            next = 0x16f04U;
            const auto source = r.address[4];
            const auto high = t.word(source);
            r.address[4] += 4U;
            const auto low = t.word(source + 2U);
            const auto value = (std::uint32_t(high) << 16U) | low;
            r.address[6] = value;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f04U: { // 361e: move-to-data-register
            next = 0x16f06U;
            const auto source = r.address[6];
            r.address[6] += 2U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(3U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f06U: { // 341e: move-to-data-register
            next = 0x16f08U;
            const auto source = r.address[6];
            r.address[6] += 2U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(2U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f08U: { // 344b: move-from-address-register
            next = 0x16f0aU;
            r.address[2] = static_cast<std::uint32_t>(static_cast<std::int16_t>(r.address[3]));
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f0aU: { // 3202: move-to-data-register
            next = 0x16f0cU;
            m.logic(r.data[2], 16U);
            m.dw(1U, r.data[2]);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f0cU: { // 5247: quick-register
            next = 0x16f0eU;
            const auto value = m.add(r.data[7], 1U, 16U);
            m.dw(7U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f0eU: { // 3ac7: move-to-indirect
            next = 0x16f10U;
            const auto destination = r.address[5];
            m.logic(r.data[7], 16U);
            t.word(destination, r.data[7]);
            r.address[5] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f10U: { // 3ad0: move-to-indirect
            next = 0x16f12U;
            const auto source = r.address[0];
            const auto value = t.word(source);
            const auto destination = r.address[5];
            m.logic(value, 16U);
            t.word(destination, value);
            r.address[5] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f12U: { // 3ade: move-to-indirect
            next = 0x16f14U;
            const auto source = r.address[6];
            r.address[6] += 2U;
            const auto value = t.word(source);
            const auto destination = r.address[5];
            m.logic(value, 16U);
            t.word(destination, value);
            r.address[5] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f14U: { // 660a: bcc
            next = 0x16f16U;
            const bool taken = scene_timing::condition(r.status, 6U);
            next = t.branch(pc, 0x16f20U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x16f16U: { // 5341: quick-register
            next = 0x16f18U;
            const auto value = m.sub(r.data[1], 1U, 16U);
            m.dw(1U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f18U: { // 3b5efffe: move-to-displacement
            next = 0x16f1cU;
            const auto source = r.address[6];
            r.address[6] += 2U;
            const auto value = t.word(source);
            const auto destination = (r.address[5] + 0xfffffffeU);
            m.logic(value, 16U);
            t.prefetch(pc + 4U);
            t.word(destination, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16f1cU: { // d4e80002: adda-word-memory
            next = 0x16f20U;
            const auto address = (r.address[0] + 0x2U);
            t.prefetch(pc + 4U);
            const auto source = static_cast<std::uint32_t>(static_cast<std::int16_t>(t.word(address)));
            scene_timing::address_add(t, r, 2U, source, pc + 6U);
            break;
        }
        case 0x16f20U: { // 3ac4: move-to-indirect
            next = 0x16f22U;
            const auto destination = r.address[5];
            m.logic(r.data[4], 16U);
            t.word(destination, r.data[4]);
            r.address[5] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f22U: { // 300a: move-from-address-register
            next = 0x16f24U;
            const auto value = r.address[2];
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f24U: { // 02400fff: immediate-and
            next = 0x16f28U;
            t.prefetch(pc + 4U);
            const auto value = r.data[0] & 0xfffU;
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16f28U: { // 3ac0: move-to-indirect
            next = 0x16f2aU;
            const auto destination = r.address[5];
            m.logic(r.data[0], 16U);
            t.word(destination, r.data[0]);
            r.address[5] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f2aU: { // 3005: move-to-data-register
            next = 0x16f2cU;
            m.logic(r.data[5], 16U);
            m.dw(0U, r.data[5]);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f2cU: { // 02400fff: immediate-and
            next = 0x16f30U;
            t.prefetch(pc + 4U);
            const auto value = r.data[0] & 0xfffU;
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x16f30U: { // 08c0000c: immediate-bit-register
            next = 0x16f34U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto mask = 0x1000U;
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[0] & mask) ? 0U : 4U));
            r.data[0] |= mask;
            t.clocks(2U);
            break;
        }
        case 0x16f34U: { // 3ac0: move-to-indirect
            next = 0x16f36U;
            const auto destination = r.address[5];
            m.logic(r.data[0], 16U);
            t.word(destination, r.data[0]);
            r.address[5] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x16f36U: { // 584d: quick-word-address
            next = 0x16f38U;
            scene_timing::address_add(t, r, 5U, 0x4U, pc + 4U);
            break;
        }
        case 0x16f38U: { // d4e80002: adda-word-memory
            next = 0x16f3cU;
            const auto address = (r.address[0] + 0x2U);
            t.prefetch(pc + 4U);
            const auto source = static_cast<std::uint32_t>(static_cast<std::int16_t>(t.word(address)));
            scene_timing::address_add(t, r, 2U, source, pc + 6U);
            break;
        }
        case 0x16f3cU: { // 51c9ffce: dbf
            next = 0x16f40U;
            next = t.dbf(pc, 0x16f0cU, 1U);
            if ((r.data[1] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
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
        case 0x16f6eU: { // 4e75: rts
            next = 0x16f70U;
            return t.rts(pc);
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (!returned_from_child)
            if (const auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x16d16U:
        case 0x16d1aU:
        case 0x16d20U:
        case 0x16d22U:
        case 0x16d24U:
        case 0x16d26U:
        case 0x16d2aU:
        case 0x16d2eU:
        case 0x16d32U:
        case 0x16d34U:
        case 0x16d38U:
        case 0x16d3aU:
        case 0x16d3eU:
        case 0x16d42U:
        case 0x16d44U:
        case 0x16d46U:
        case 0x16d4aU:
        case 0x16d4cU:
        case 0x16d4eU:
        case 0x16d52U:
        case 0x16d54U:
        case 0x16d56U:
        case 0x16d58U:
        case 0x16d5cU:
        case 0x16d5eU:
        case 0x16d64U:
        case 0x16d68U:
        case 0x16d6aU:
        case 0x16d6cU:
        case 0x16d72U:
        case 0x16d76U:
        case 0x16d78U:
        case 0x16d7aU:
        case 0x16d7eU:
        case 0x16d82U:
        case 0x16d84U:
        case 0x16d86U:
        case 0x16d88U:
        case 0x16d8aU:
        case 0x16d8cU:
        case 0x16d8eU:
        case 0x16d92U:
        case 0x16d96U:
        case 0x16d98U:
        case 0x16d9aU:
        case 0x16d9eU:
        case 0x16da0U:
        case 0x16da2U:
        case 0x16da4U:
        case 0x16da6U:
        case 0x16da8U:
        case 0x16dacU:
        case 0x16daeU:
        case 0x16db2U:
        case 0x16db4U:
        case 0x16db8U:
        case 0x16dbcU:
        case 0x16dc0U:
        case 0x16dc2U:
        case 0x16dc4U:
        case 0x16dcaU:
        case 0x16dccU:
        case 0x16dd0U:
        case 0x16dd2U:
        case 0x16dd4U:
        case 0x16dd6U:
        case 0x16dd8U:
        case 0x16ddaU:
        case 0x16ddcU:
        case 0x16de0U:
        case 0x16de2U:
        case 0x16de4U:
        case 0x16de6U:
        case 0x16de8U:
        case 0x16deaU:
        case 0x16decU:
        case 0x16df0U:
        case 0x16df2U:
        case 0x16df6U:
        case 0x16df8U:
        case 0x16dfcU:
        case 0x16e00U:
        case 0x16e04U:
        case 0x16e06U:
        case 0x16e08U:
        case 0x16e0eU:
        case 0x16e10U:
        case 0x16e14U:
        case 0x16e16U:
        case 0x16e18U:
        case 0x16e1aU:
        case 0x16e1cU:
        case 0x16e20U:
        case 0x16e24U:
        case 0x16e28U:
        case 0x16e2aU:
        case 0x16e2cU:
        case 0x16e30U:
        case 0x16e34U:
        case 0x16e36U:
        case 0x16e38U:
        case 0x16e3cU:
        case 0x16e3eU:
        case 0x16e40U:
        case 0x16e42U:
        case 0x16e44U:
        case 0x16e48U:
        case 0x16e4aU:
        case 0x16e4eU:
        case 0x16e50U:
        case 0x16e54U:
        case 0x16e58U:
        case 0x16e5cU:
        case 0x16e5eU:
        case 0x16e60U:
        case 0x16e66U:
        case 0x16e68U:
        case 0x16e6cU:
        case 0x16e6eU:
        case 0x16e70U:
        case 0x16e72U:
        case 0x16e74U:
        case 0x16e76U:
        case 0x16e78U:
        case 0x16e7cU:
        case 0x16e7eU:
        case 0x16e80U:
        case 0x16e82U:
        case 0x16e84U:
        case 0x16e86U:
        case 0x16e8aU:
        case 0x16e8cU:
        case 0x16e90U:
        case 0x16e92U:
        case 0x16e96U:
        case 0x16e9aU:
        case 0x16e9eU:
        case 0x16ea0U:
        case 0x16ea2U:
        case 0x16ea8U:
        case 0x16eaaU:
        case 0x16eaeU:
        case 0x16eb0U:
        case 0x16eb2U:
        case 0x16eb4U:
        case 0x16eb6U:
        case 0x16eb8U:
        case 0x16ebaU:
        case 0x16ebeU:
        case 0x16ec2U:
        case 0x16ec4U:
        case 0x16ec8U:
        case 0x16eccU:
        case 0x16ed2U:
        case 0x16ed6U:
        case 0x16ed8U:
        case 0x16edcU:
        case 0x16ee0U:
        case 0x16ee6U:
        case 0x16eeaU:
        case 0x16eeeU:
        case 0x16ef2U:
        case 0x16ef6U:
        case 0x16ef8U:
        case 0x16efcU:
        case 0x16efeU:
        case 0x16f00U:
        case 0x16f02U:
        case 0x16f04U:
        case 0x16f06U:
        case 0x16f08U:
        case 0x16f0aU:
        case 0x16f0cU:
        case 0x16f0eU:
        case 0x16f10U:
        case 0x16f12U:
        case 0x16f14U:
        case 0x16f16U:
        case 0x16f18U:
        case 0x16f1cU:
        case 0x16f20U:
        case 0x16f22U:
        case 0x16f24U:
        case 0x16f28U:
        case 0x16f2aU:
        case 0x16f2cU:
        case 0x16f30U:
        case 0x16f34U:
        case 0x16f36U:
        case 0x16f38U:
        case 0x16f3cU:
        case 0x16f40U:
        case 0x16f44U:
        case 0x16f48U:
        case 0x16f4aU:
        case 0x16f4cU:
        case 0x16f4eU:
        case 0x16f52U:
        case 0x16f56U:
        case 0x16f5aU:
        case 0x16f5cU:
        case 0x16f5eU:
        case 0x16f62U:
        case 0x16f66U:
        case 0x16f68U:
        case 0x16f6aU:
        case 0x16f6eU:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
