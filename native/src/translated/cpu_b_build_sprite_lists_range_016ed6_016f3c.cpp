#include "cpu_b_build_sprite_lists_detail.h"

namespace gain_ground::translated::cpu_b_build_sprite_lists_detail {

bool dispatch_pc_range_05(FunctionContext &,
    CpuRegisters &registers, unverified::Machine &machine,
    CpuBIrqTiming &timing, std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind)
{
    auto &r = registers;
    auto &m = machine;
    auto &t = timing;
    switch (pc) {
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
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_build_sprite_lists_detail
