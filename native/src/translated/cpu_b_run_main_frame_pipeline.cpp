// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult cpu_b_run_main_frame_pipeline(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xedb8U: { // 386d0060 movea.w $60(a5), a4
            next = 0xedbcU;
            const auto source_address = r.address[5] + 0x60U;
            const auto value = m.word(source_address);
            r.address[4] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xedbcU: { // 61000c06 bsr.w $f9c4
            next = 0xedc0U;
            const auto result = m.call(c, 180U, 0xedbcU, 0xf9c4U, 0xedc0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xedc0U: { // 61000cd0 bsr.w $fa92
            next = 0xedc4U;
            const auto result = m.call(c, 183U, 0xedc0U, 0xfa92U, 0xedc4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xedc4U: { // 6406 bcc.b $edcc
            next = 0xedc6U;
            if ((r.status & 1U) == 0U) { next = 0xedccU; transfer_kind = 1U; }
            break;
        }
        case 0xedc6U: { // 4ef90000a532 jmp $a532.l
            next = 0xedccU;
            next = 0xa532U;
            transfer_kind = 1U;
            break;
        }
        case 0xedccU: { // 302d0044 move.w $44(a5), d0
            next = 0xedd0U;
            m.dw(0U, m.word(r.address[5] + 0x44U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xedd0U: { // 5b40 subq.w #$5, d0
            next = 0xedd2U;
            const auto source_value = 0x5U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xedd2U: { // 0c400002 cmpi.w #$2, d0
            next = 0xedd6U;
            const auto source_value = 0x2U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xedd6U: { // 6204 bhi.b $eddc
            next = 0xedd8U;
            if ((r.status & 5U) == 0U) { next = 0xeddcU; transfer_kind = 1U; }
            break;
        }
        case 0xedd8U: { // 610005e6 bsr.w $f3c0
            next = 0xeddcU;
            const auto result = m.call(c, 178U, 0xedd8U, 0xf3c0U, 0xeddcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xeddcU: { // 4e75 rts 
            next = 0xeddeU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xeddcU, r.program_counter)) return *event;
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
        case 0xedb8U:
        case 0xedbcU:
        case 0xedc0U:
        case 0xedc4U:
        case 0xedc6U:
        case 0xedccU:
        case 0xedd0U:
        case 0xedd2U:
        case 0xedd6U:
        case 0xedd8U:
        case 0xeddcU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
