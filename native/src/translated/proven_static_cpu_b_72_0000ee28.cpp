// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0000ee28(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xee28U: { // 2b7c0000ee3e0002 move.l #$ee3e, $2(a5)
            next = 0xee30U;
            m.lng(r.address[5] + 0x2U, 0xee3eU);
            m.logic(0xee3eU, 32U);
            break;
        }
        case 0xee30U: { // 386d0060 movea.w $60(a5), a4
            next = 0xee34U;
            const auto source_address = r.address[5] + 0x60U;
            const auto value = m.word(source_address);
            r.address[4] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xee34U: { // 61000ce8 bsr.w $fb1e
            next = 0xee38U;
            const auto result = m.call(c, 185U, 0xee34U, 0xfb1eU, 0xee38U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xee38U: { // 61000d00 bsr.w $fb3a
            next = 0xee3cU;
            const auto result = m.call(c, 186U, 0xee38U, 0xfb3aU, 0xee3cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xee3cU: { // 6008 bra.b $ee46
            next = 0xee3eU;
            if (true) { next = 0xee46U; transfer_kind = 1U; }
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0xee46U) return c.host->call_function(522U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0xee28U:
        case 0xee30U:
        case 0xee34U:
        case 0xee38U:
        case 0xee3cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
