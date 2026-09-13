// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0000ee6e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xee6eU: { // 386d0060 movea.w $60(a5), a4
            next = 0xee72U;
            const auto source_address = r.address[5] + 0x60U;
            const auto value = m.word(source_address);
            r.address[4] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xee72U: { // 61000caa bsr.w $fb1e
            next = 0xee76U;
            const auto result = m.call(c, 185U, 0xee72U, 0xfb1eU, 0xee76U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xee76U: { // 61000cc2 bsr.w $fb3a
            next = 0xee7aU;
            const auto result = m.call(c, 186U, 0xee76U, 0xfb3aU, 0xee7aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xee7aU: { // 61000baa bsr.w $fa26
            next = 0xee7eU;
            const auto result = m.call(c, 181U, 0xee7aU, 0xfa26U, 0xee7eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xee7eU: { // 7000 moveq #$0, d0
            next = 0xee80U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xee80U: { // 102c0000 move.b $0(a4), d0
            next = 0xee84U;
            const auto source_address = r.address[4] + 0x0U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xee84U: { // d1780c10 add.w d0, $c10.w
            next = 0xee88U;
            const auto source_value = r.data[0];
            const auto destination_address = 0xc10U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xee88U: { // 600c bra.b $ee96
            next = 0xee8aU;
            if (true) { next = 0xee96U; transfer_kind = 1U; }
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0xee96U) return c.host->call_function(525U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0xee6eU:
        case 0xee72U:
        case 0xee76U:
        case 0xee7aU:
        case 0xee7eU:
        case 0xee80U:
        case 0xee84U:
        case 0xee88U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
