// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0000ee52(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xee52U: { // 61000d4e bsr.w $fba2
            next = 0xee56U;
            const auto result = m.call(c, 189U, 0xee52U, 0xfba2U, 0xee56U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xee56U: { // 386d0060 movea.w $60(a5), a4
            next = 0xee5aU;
            const auto source_address = r.address[5] + 0x60U;
            const auto value = m.word(source_address);
            r.address[4] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xee5aU: { // 102d006c move.b $6c(a5), d0
            next = 0xee5eU;
            const auto source_address = r.address[5] + 0x6cU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xee5eU: { // c0380820 and.b $820.w, d0
            next = 0xee62U;
            const auto source_address = 0x820U;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0xee62U: { // 6726 beq.b $ee8a
            next = 0xee64U;
            if ((r.status & 4U) != 0U) { next = 0xee8aU; transfer_kind = 1U; }
            break;
        }
        case 0xee64U: { // 3b7c00010042 move.w #$1, $42(a5)
            next = 0xee6aU;
            m.word(r.address[5] + 0x42U, 0x1U);
            m.logic(0x1U, 16U);
            break;
        }
        case 0xee6aU: { // 426d0044 clr.w $44(a5)
            next = 0xee6eU;
            const auto destination_address = r.address[5] + 0x44U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0xee6eU) return c.host->call_function(524U, m.cpu, m.state, transfer_kind, pc, next, c);
        if (next == 0xee8aU) return c.host->call_function(582U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0xee52U:
        case 0xee56U:
        case 0xee5aU:
        case 0xee5eU:
        case 0xee62U:
        case 0xee64U:
        case 0xee6aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
