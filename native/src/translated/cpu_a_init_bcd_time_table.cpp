// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_a_init_bcd_time_table(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 0x0U || c.state != 0xffU)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 0x0U, 0xffU};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        bool returned_from_child = false;
        switch (pc) {
        case 0x801f6U: { // 41f9fff07800: lea-absolute-long
            next = 0x801fcU;
            const auto value = 0xfff07800U;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x801fcU: { // 43fa044a: lea
            next = 0x80200U;
            r.address[1] = 0x80648U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x80200U: { // 7013: moveq
            next = 0x80202U;
            r.data[0] = 0x13U;
            m.logic(0x13U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x80202U: { // 20d9: move-long-postincrement-copy
            next = 0x80204U;
            const auto source = r.address[1];
            const auto high = t.word(source);
            r.address[1] += 4U;
            const auto low = t.word(source + 2U);
            const auto destination = r.address[0];
            m.logic(low, 16U); t.word(destination, high);
            m.logic((std::uint32_t(high) << 16U) | low, 32U);
            t.word(destination + 2U, low);
            r.address[0] += 4U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x80204U: { // 51c8fffc: dbf
            next = 0x80208U;
            next = t.dbf(pc, 0x80202U, 0U);
            if ((r.data[0] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0x80208U: { // 303c0058: move-to-data-register
            next = 0x8020cU;
            t.prefetch(pc + 4U);
            m.logic(0x58U, 16U);
            m.dw(0U, 0x58U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x8020cU: { // 43f9fff00826: lea-absolute-long
            next = 0x80212U;
            const auto value = 0xfff00826U;
            r.address[1] = (r.address[1] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[1] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x80212U: { // 22fc00000010: move-long-register-or-immediate-to-memory
            next = 0x80218U;
            const auto value = 0x10U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = r.address[1];
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            r.address[1] += 4U;
            m.logic(value, 32U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x80218U: { // 22bc00000990: move-long-immediate-to-indirect
            next = 0x8021eU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = r.address[1];
            t.word(destination, 0x0U);
            m.logic(0x990U, 16U);
            t.word(destination + 2U, 0x990U);
            m.logic(0x990U, 32U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x8021eU: { // 45d1: lea
            next = 0x80220U;
            r.address[2] = r.address[1];
            t.prefetch(pc + 4U);
            break;
        }
        case 0x80220U: { // 20d9: move-long-postincrement-copy
            next = 0x80222U;
            const auto source = r.address[1];
            const auto high = t.word(source);
            r.address[1] += 4U;
            const auto low = t.word(source + 2U);
            const auto destination = r.address[0];
            m.logic(low, 16U); t.word(destination, high);
            m.logic((std::uint32_t(high) << 16U) | low, 32U);
            t.word(destination + 2U, low);
            r.address[0] += 4U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x80222U: { // 4298: clear-memory
            next = 0x80224U;
            const auto address = r.address[0];
            (void)t.word(address);
            r.address[0] += 4U;
            (void)t.word(address + 2U);
            m.logic(0U, 32U);
            t.prefetch(pc + 4U);
            t.word(address + 2U, 0U);
            t.word(address, 0U);
            break;
        }
        case 0x80224U: { // 44fc0000: move-immediate-ccr
            next = 0x80228U;
            t.prefetch(pc + 4U);
            t.clocks(2U);
            r.status = static_cast<std::uint16_t>((r.status & 0xa700U) | 0x0U);
            t.clocks(2U);
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x80228U: { // 830a: sbcd-memory-predecrement
            next = 0x8022aU;
            t.clocks(2U);
            r.address[2] -= 1U;
            const auto source_value = t.byte(r.address[2]);
            r.address[1] -= 1U;
            const auto address = r.address[1];
            const auto destination_value = t.byte(address);
            const auto value = m.sbcd(destination_value, source_value);
            t.prefetch(pc + 4U);
            t.byte(address, value);
            break;
        }
        case 0x8022aU: { // 830a: sbcd-memory-predecrement
            next = 0x8022cU;
            t.clocks(2U);
            r.address[2] -= 1U;
            const auto source_value = t.byte(r.address[2]);
            r.address[1] -= 1U;
            const auto address = r.address[1];
            const auto destination_value = t.byte(address);
            const auto value = m.sbcd(destination_value, source_value);
            t.prefetch(pc + 4U);
            t.byte(address, value);
            break;
        }
        case 0x8022cU: { // 830a: sbcd-memory-predecrement
            next = 0x8022eU;
            t.clocks(2U);
            r.address[2] -= 1U;
            const auto source_value = t.byte(r.address[2]);
            r.address[1] -= 1U;
            const auto address = r.address[1];
            const auto destination_value = t.byte(address);
            const auto value = m.sbcd(destination_value, source_value);
            t.prefetch(pc + 4U);
            t.byte(address, value);
            break;
        }
        case 0x8022eU: { // 830a: sbcd-memory-predecrement
            next = 0x80230U;
            t.clocks(2U);
            r.address[2] -= 1U;
            const auto source_value = t.byte(r.address[2]);
            r.address[1] -= 1U;
            const auto address = r.address[1];
            const auto destination_value = t.byte(address);
            const auto value = m.sbcd(destination_value, source_value);
            t.prefetch(pc + 4U);
            t.byte(address, value);
            break;
        }
        case 0x80230U: { // 51c8ffec: dbf
            next = 0x80234U;
            next = t.dbf(pc, 0x8021eU, 0U);
            if ((r.data[0] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0x80234U: { // 4e75: rts
            next = 0x80236U;
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
        case 0x801f6U:
        case 0x801fcU:
        case 0x80200U:
        case 0x80202U:
        case 0x80204U:
        case 0x80208U:
        case 0x8020cU:
        case 0x80212U:
        case 0x80218U:
        case 0x8021eU:
        case 0x80220U:
        case 0x80222U:
        case 0x80224U:
        case 0x80228U:
        case 0x8022aU:
        case 0x8022cU:
        case 0x8022eU:
        case 0x80230U:
        case 0x80234U:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
