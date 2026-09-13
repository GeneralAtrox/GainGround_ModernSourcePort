// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0000ee8a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xee8aU: { // 426d0042 clr.w $42(a5)
            next = 0xee8eU;
            const auto destination_address = r.address[5] + 0x42U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xee8eU: { // 61000c8e bsr.w $fb1e
            next = 0xee92U;
            const auto result = m.call(c, 185U, 0xee8eU, 0xfb1eU, 0xee92U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xee92U: { // 61000cde bsr.w $fb72
            next = 0xee96U;
            const auto result = m.call(c, 188U, 0xee92U, 0xfb72U, 0xee96U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0xee96U) return c.host->call_function(525U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0xee8aU:
        case 0xee8eU:
        case 0xee92U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
