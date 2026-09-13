// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult cpu_b_run_callback_state_dispatch(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xee3eU: { // 083800030833 btst.b #$3, $833.w
            next = 0xee44U;
            const auto destination_address = 0x833U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0xee44U: { // 6704 beq.b $ee4a
            next = 0xee46U;
            if ((r.status & 4U) != 0U) { next = 0xee4aU; transfer_kind = 1U; }
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0xee46U) return c.host->call_function(522U, m.cpu, m.state, transfer_kind, pc, next, c);
        if (next == 0xee4aU) return c.host->call_function(523U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0xee3eU:
        case 0xee44U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
