// Implemented but unverified. Instruction phases from the retained 68000 source.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00017b86(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x17b86U: case 0x17b88U:
            r.data[pc == 0x17b86U ? 0U : 1U] = 0U; m.logic(0U, 32U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17b8aU:
            r.address[0] = r.address[6] + 0x60U;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17b8eU: case 0x17b9eU: {
            const bool subtract = pc == 0x17b8eU;
            const auto address = r.address[0] + (subtract ? 0x20U : 0x24U);
            t.prefetch(pc + 4U);
            const auto old = t.word(address);
            const auto value = subtract ? m.sub(old, 1U, 16U) : m.add(old, 2U, 16U);
            t.prefetch(pc + 6U); t.word(address, static_cast<std::uint16_t>(value));
            next = pc + 4U; break;
        }
        case 0x17b92U: case 0x17ba2U: {
            const bool first = pc == 0x17b92U;
            const auto address = first ? r.address[0] + 0x24U : r.address[5] + 4U;
            t.prefetch(pc + 4U);
            const auto value = t.word(address); m.dw(first ? 0U : 1U, value); m.logic(value, 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17b96U:
            t.prefetch(pc + 4U); m.dw(0U, r.data[0] & 0x1eU); m.logic(r.data[0], 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17b9aU: case 0x17baaU: case 0x17bb8U: case 0x17bcaU: {
            const auto index = static_cast<std::uint32_t>(static_cast<std::int32_t>(
                static_cast<std::int16_t>(r.data[0])));
            const auto address = r.address[0] + index;
            t.clocks(2U); t.prefetch(pc + 4U);
            const auto value = t.word(address); m.dw(pc == 0x17baaU ? 1U : 0U, value);
            m.logic(value, 16U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17ba6U: {
            const auto address = r.address[5] + r.data[1];
            t.clocks(2U); t.prefetch(pc + 4U); r.address[0] = address;
            t.clocks(2U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17baeU:
            next = t.branch_word(pc, 0x17ca0U, (r.status & 4U) != 0U);
            if (next == 0x17ca0U) return t.transfer(pc, 546U, next);
            break;
        case 0x17bb2U: case 0x17bc4U: case 0x17bd2U: {
            const auto value = r.data[pc == 0x17bc4U ? 2U : 0U];
            m.dw(pc == 0x17bb2U ? 2U : pc == 0x17bc4U ? 0U : 3U, value);
            m.logic(value, 16U); t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17bb4U: case 0x17bc6U:
            t.prefetch(pc + 4U);
            m.dw(0U, m.add(r.data[0], pc == 0x17bb4U ? 0x100U : 0x200U, 16U));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17bbcU: case 0x17bceU: {
            const auto address = r.address[6] + (pc == 0x17bbcU ? 0x86U : 0x30U);
            t.prefetch(pc + 4U); m.logic(r.data[0], 16U);
            t.word(address, static_cast<std::uint16_t>(r.data[0]));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17bc0U: case 0x17bd4U: {
            const auto child = t.bsr(pc, pc == 0x17bc0U ? 482U : 483U,
                pc == 0x17bc0U ? 0x17ca2U : 0x17cc6U, pc + 4U);
            if (child.status != TranslationStatus::complete) return child;
            // 483 has already discarded its BSR return and popped 480's
            // caller return. Do not resume 17bd8 or pop another return.
            if (pc == 0x17bd4U && child.control == 8U)
                return FunctionResult::complete(1U, child.exit_program_counter);
            if (child.control != 1U) return child;
            continue;
        }
        case 0x17bd8U: {
            const auto value = r.address[0] + r.data[1];
            r.address[0] = (r.address[0] & 0xffff0000U) | (value & 0xffffU);
            t.prefetch(pc + 4U); t.clocks(2U); r.address[0] = value; t.clocks(2U);
            next = pc + 2U; break;
        }
        case 0x17bdaU:
            next = t.branch_word(pc, 0x17c3aU, true);
            return t.transfer(pc, 545U, next);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
