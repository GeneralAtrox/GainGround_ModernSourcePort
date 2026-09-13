// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult cpu_b_update_record_and_restart_on_carry(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xecfeU: { // 386d0060 movea.w $60(a5), a4
            next = 0xed02U;
            const auto source_address = r.address[5] + 0x60U;
            const auto value = m.word(source_address);
            r.address[4] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xed02U: { // 61000cc0 bsr.w $f9c4
            next = 0xed06U;
            const auto result = m.call(c, 180U, 0xed02U, 0xf9c4U, 0xed06U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xed06U: { // 61000d8a bsr.w $fa92
            next = 0xed0aU;
            const auto result = m.call(c, 183U, 0xed06U, 0xfa92U, 0xed0aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xed0aU: { // 6406 bcc.b $ed12
            next = 0xed0cU;
            if ((r.status & 1U) == 0U) { next = 0xed12U; transfer_kind = 1U; }
            break;
        }
        case 0xed0cU: { // 4ef90000a532 jmp $a532.l
            next = 0xed12U;
            next = 0xa532U;
            transfer_kind = 1U;
            break;
        }
        case 0xed12U: { // 4e75 rts 
            next = 0xed14U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xed12U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0xa532U) return c.host->call_function(520U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0xecfeU:
        case 0xed02U:
        case 0xed06U:
        case 0xed0aU:
        case 0xed0cU:
        case 0xed12U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
