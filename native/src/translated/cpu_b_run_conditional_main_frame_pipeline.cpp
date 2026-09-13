// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult cpu_b_run_conditional_main_frame_pipeline(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xede6U: { // 4a788002 tst.w $8002.w
            next = 0xedeaU;
            const auto destination_address = 0xffff8002U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xedeaU: { // 661a bne.b $ee06
            next = 0xedecU;
            if ((r.status & 4U) == 0U) { next = 0xee06U; transfer_kind = 1U; }
            break;
        }
        case 0xedecU: { // 386d0060 movea.w $60(a5), a4
            next = 0xedf0U;
            const auto source_address = r.address[5] + 0x60U;
            const auto value = m.word(source_address);
            r.address[4] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xedf0U: { // 3c6d0068 movea.w $68(a5), a6
            next = 0xedf4U;
            const auto source_address = r.address[5] + 0x68U;
            const auto value = m.word(source_address);
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xedf4U: { // 1e16 move.b (a6), d7
            next = 0xedf6U;
            const auto source_address = r.address[6];
            const auto value = m.byte(source_address);
            m.db(7U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xedf6U: { // 8e380404 or.b $404.w, d7
            next = 0xedfaU;
            const auto source_address = 0x404U;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[7];
            const auto value = destination_value | source_value;
            m.logic(value, 8U);
            m.db(7U, value);
            break;
        }
        case 0xedfaU: { // 61000c96 bsr.w $fa92
            next = 0xedfeU;
            const auto result = m.call(c, 183U, 0xedfaU, 0xfa92U, 0xedfeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xedfeU: { // 6406 bcc.b $ee06
            next = 0xee00U;
            if ((r.status & 1U) == 0U) { next = 0xee06U; transfer_kind = 1U; }
            break;
        }
        case 0xee00U: { // 4ef90000a532 jmp $a532.l
            next = 0xee06U;
            next = 0xa532U;
            transfer_kind = 1U;
            break;
        }
        case 0xee06U: { // 4e75 rts 
            next = 0xee08U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xee06U, r.program_counter)) return *event;
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
        case 0xede6U:
        case 0xedeaU:
        case 0xedecU:
        case 0xedf0U:
        case 0xedf4U:
        case 0xedf6U:
        case 0xedfaU:
        case 0xedfeU:
        case 0xee00U:
        case 0xee06U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
