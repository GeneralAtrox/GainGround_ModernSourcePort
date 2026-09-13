// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult cpu_b_enter_callback_state_ee3e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xee1aU: { // 386d0060 movea.w $60(a5), a4
            next = 0xee1eU;
            const auto source_address = r.address[5] + 0x60U;
            const auto value = m.word(source_address);
            r.address[4] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xee1eU: { // 61000ba4 bsr.w $f9c4
            next = 0xee22U;
            const auto result = m.call(c, 180U, 0xee1eU, 0xf9c4U, 0xee22U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xee22U: { // 61000c6e bsr.w $fa92
            next = 0xee26U;
            const auto result = m.call(c, 183U, 0xee22U, 0xfa92U, 0xee26U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xee26U: { // 6422 bcc.b $ee4a
            next = 0xee28U;
            if ((r.status & 1U) == 0U) { next = 0xee4aU; transfer_kind = 1U; }
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0xee28U) return c.host->call_function(521U, m.cpu, m.state, transfer_kind, pc, next, c);
        if (next == 0xee4aU) return c.host->call_function(523U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0xee1aU:
        case 0xee1eU:
        case 0xee22U:
        case 0xee26U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
