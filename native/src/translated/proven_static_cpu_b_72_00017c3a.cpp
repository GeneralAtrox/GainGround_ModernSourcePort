// Implemented but unverified. Original control-stream sound-record initialization.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00017ca0(FunctionContext &) noexcept;
namespace {
FunctionResult finish_record_reset(FunctionContext &c, FunctionResult child) {
    // A capture host can stop at 484's shared RTS owner. Execute that actual
    // instruction only when it is still pending; a live child already returns.
    if (child.status == TranslationStatus::complete && child.control == 3U
        && child.exit_program_counter == 0x17ca0U && c.registers.program_counter == 0x17ca0U)
        return proven_static_cpu_b_72_00017ca0(c);
    return child;
}
} // namespace

FunctionResult proven_static_cpu_b_72_00017c3a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x17c3aU: {
            const auto source = r.address[0]; r.address[0] += 2U;
            const auto value = t.word(source); m.dw(4U, value); m.logic(value, 16U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17c3cU:
            r.address[3] = r.address[6]; t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17c3eU: case 0x17c88U: {
            const auto source = pc == 0x17c3eU ? r.address[0] + 2U : r.address[0] - 9U;
            t.prefetch(pc + 4U); const auto value = t.byte(source);
            m.db(5U, value); m.logic(value, 8U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17c42U: case 0x17c70U: {
            const auto reg = pc == 0x17c42U ? 5U : 7U;
            const auto mask = pc == 0x17c42U ? 0x1fU : 7U;
            t.prefetch(pc + 4U); m.dw(reg, r.data[reg] & mask); m.logic(r.data[reg], 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17c46U:
            m.dw(5U, m.add(r.data[5], r.data[5], 16U));
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17c48U:
            r.address[2] = 0x17834U; t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            next = pc + 4U; break;
        case 0x17c4cU: {
            const auto source = r.address[2] + static_cast<std::int16_t>(r.data[5]);
            t.clocks(2U); t.prefetch(pc + 4U); const auto offset = t.word(source);
            const auto value = r.address[3] + static_cast<std::int16_t>(offset);
            r.address[3] = (r.address[3] & 0xffff0000U) | (value & 0xffffU);
            t.prefetch(pc + 6U); t.clocks(2U); r.address[3] = value; t.clocks(2U);
            next = pc + 4U; break;
        }
        case 0x17c50U:
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[5] & 0x10U) ? 0U : 4U));
            t.clocks(2U); next = pc + 4U; break;
        case 0x17c54U: next = t.branch(pc, 0x17c68U, (r.status & 4U) != 0U); break;
        case 0x17c56U: case 0x17c62U: {
            const bool set = pc == 0x17c56U;
            const auto address = r.address[3] - (set ? 0x320U : 0x280U);
            const auto mask = set ? 4U : 0x80U;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); const auto old = t.byte(address);
            t.prefetch(pc + 8U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & mask) ? 0U : 4U));
            t.byte(address, static_cast<std::uint8_t>(set ? old | mask : old & ~mask));
            next = pc + 6U; break;
        }
        case 0x17c5cU:
            t.prefetch(pc + 4U); (void)m.sub(r.data[5], 0x1cU, 8U, true);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17c60U: next = t.branch(pc, 0x17c68U, (r.status & 1U) != 0U); break;
        case 0x17c68U: {
            const auto source = r.address[0]; r.address[0] += 2U;
            const auto value = t.word(source); const auto destination = r.address[3];
            m.logic(value, 16U); t.word(destination, value); r.address[3] = destination + 2U;
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17c6aU: case 0x17c90U: case 0x17c94U: {
            const auto reg = pc == 0x17c6aU ? 7U : pc == 0x17c90U ? 5U : 6U;
            r.data[reg] = pc == 0x17c94U ? 0xfU : 0U; m.logic(r.data[reg], 32U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17c6cU: {
            const auto source = r.address[0]++;
            const auto value = t.byte(source); m.db(7U, value); m.logic(value, 8U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17c6eU: case 0x17c8eU: case 0x17c92U: {
            const auto value = static_cast<std::uint8_t>(r.data[pc == 0x17c6eU ? 7U : 5U]);
            const auto destination = r.address[3]; m.logic(value, 8U);
            t.byte(destination, value); r.address[3] = destination + 1U;
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17c74U: {
            const auto child = t.bsr(pc, 484U, 0x17d9eU, 0x17c78U, finish_record_reset);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x17c78U: {
            const auto source = r.address[0]++;
            const auto value = t.byte(source); const auto destination = r.address[3];
            m.logic(value, 8U); t.byte(destination, value); r.address[3] = destination + 1U;
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17c7aU:
            r.data[5] = r.address[0]; m.logic(r.data[5], 32U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17c7cU: {
            const auto source = r.address[0]; const auto high = t.word(source);
            r.address[0] = source + 4U; const auto low = t.word(source + 2U);
            const auto value = m.add(r.data[5], (std::uint32_t(high) << 16U) | low, 32U);
            m.dw(5U, value); t.prefetch(pc + 4U); r.data[5] = value; t.clocks(2U);
            next = pc + 2U; break;
        }
        case 0x17c7eU: {
            const auto left = r.data[5]; const auto right = r.address[5];
            m.dw(5U, m.sub(left, right, 16U)); t.prefetch(pc + 4U);
            const auto value = m.sub(left, right, 32U); t.clocks(2U);
            r.data[5] = value; t.clocks(2U); next = pc + 2U; break;
        }
        case 0x17c80U: case 0x17c96U: {
            const auto destination = r.address[3]; const auto value = r.data[5];
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            r.address[3] = destination + 4U; m.logic(value, 32U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17c82U: {
            const auto source = r.address[0]; const auto high = t.word(source);
            r.address[0] = source + 4U; const auto low = t.word(source + 2U);
            const auto value = (std::uint32_t(high) << 16U) | low; const auto destination = r.address[3];
            m.logic(low, 16U); t.word(destination, high);
            m.logic(value, 32U); t.word(destination + 2U, low); r.address[3] = destination + 4U;
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17c84U: {
            t.prefetch(pc + 4U); const auto destination = r.address[3];
            m.logic(0x5001U, 16U); t.word(destination, 0x5001U); r.address[3] = destination + 2U;
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17c8cU:
            m.db(5U, m.sub(r.data[5], 1U, 8U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17c98U: next = t.dbf(pc, 0x17c96U, 6U); break;
        case 0x17c9cU:
            next = t.dbf(pc, 0x17c3cU, 4U);
            if (next == 0x17ca0U) return t.transfer(pc, 546U, next);
            break;
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
