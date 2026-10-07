#include "cpu_b_advance_phase_callback_dispatch_detail.h"

namespace gain_ground::translated::cpu_b_advance_phase_callback_dispatch_detail {

bool dispatch_instruction_region_07(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind,
    std::optional<FunctionResult> &outcome)
{
    auto &c = context;
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xda92U: { // 600000b8 bra.w $db4c
            next = 0xda96U;
            if (true) { next = 0xdb4cU; transfer_kind = 1U; }
            break;
        }
        case 0xda96U: { // 600000b4 bra.w $db4c
            next = 0xda9aU;
            if (true) { next = 0xdb4cU; transfer_kind = 1U; }
            break;
        }
        case 0xda9aU: { // 302d0010 move.w $10(a5), d0
            next = 0xda9eU;
            m.dw(0U, m.word(r.address[5] + 0x10U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xda9eU: { // 3200 move.w d0, d1
            next = 0xdaa0U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xdaa0U: { // 0241000e andi.w #$e, d1
            next = 0xdaa4U;
            const auto source_value = 0xeU;
            const auto destination_value = r.data[1];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0xdaa4U: { // d241 add.w d1, d1
            next = 0xdaa6U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0xdaa6U: { // 45fa0706 lea.l $e1ae(pc), a2
            next = 0xdaaaU;
            const auto source_address = 0xe1aeU;
            r.address[2] = source_address;
            break;
        }
        case 0xdaaaU: { // d4c1 adda.w d1, a2
            next = 0xdaacU;
            const auto source_value = r.data[1];
            r.address[2] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xdaacU: { // 02400001 andi.w #$1, d0
            next = 0xdab0U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xdab0U: { // 6704 beq.b $dab6
            next = 0xdab2U;
            if ((r.status & 4U) != 0U) { next = 0xdab6U; transfer_kind = 1U; }
            break;
        }
        case 0xdab2U: { // 303c0d97 move.w #$d97, d0
            next = 0xdab6U;
            m.dw(0U, 0xd97U);
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xdab6U: { // 41f900204000 lea.l $204000.l, a0
            next = 0xdabcU;
            r.address[0] = 0x204000U;
            break;
        }
        case 0xdabcU: { // d0da adda.w (a2)+, a0
            next = 0xdabeU;
            const auto source_address = r.address[2];
            const auto source_value = m.word(source_address);
            r.address[2] += 2U;
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xdabeU: { // 7600 moveq #$0, d3
            next = 0xdac0U;
            r.data[3] = 0x0U;
            m.logic(r.data[3], 32U);
            break;
        }
        case 0xdac0U: { // 7400 moveq #$0, d2
            next = 0xdac2U;
            r.data[2] = 0x0U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0xdac2U: { // 161a move.b (a2)+, d3
            next = 0xdac4U;
            const auto source_address = r.address[2];
            const auto value = m.byte(source_address);
            r.address[2] += 1U;
            m.db(3U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xdac4U: { // 141a move.b (a2)+, d2
            next = 0xdac6U;
            const auto source_address = r.address[2];
            const auto value = m.byte(source_address);
            r.address[2] += 1U;
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xdac6U: { // 43d0 lea.l (a0), a1
            next = 0xdac8U;
            const auto source_address = r.address[0];
            r.address[1] = source_address;
            break;
        }
        case 0xdac8U: { // 3203 move.w d3, d1
            next = 0xdacaU;
            const auto value = r.data[3];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xdacaU: { // 32c0 move.w d0, (a1)+
            next = 0xdaccU;
            const auto value = r.data[0];
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            r.address[1] += 2U;
            m.logic(value, 16U);
            break;
        }
        case 0xdaccU: { // 51c9fffc dbra d1, $daca
            next = 0xdad0U;
            m.dw(1U, r.data[1] - 1U);
            if ((r.data[1] & 0xffffU) != 0xffffU) next = 0xdacaU;
            break;
        }
        case 0xdad0U: { // 41e80080 lea.l $80(a0), a0
            next = 0xdad4U;
            r.address[0] = r.address[0] + 0x80U;
            break;
        }
        case 0xdad4U: { // 51cafff0 dbra d2, $dac6
            next = 0xdad8U;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0xdac6U;
            break;
        }
        case 0xdad8U: { // 4e75 rts 
            next = 0xdadaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xdad8U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xdadaU: { // 4e75 rts 
            next = 0xdadcU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xdadaU, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xdadcU: { // 43f900025c9c lea.l $25c9c.l, a1
            next = 0xdae2U;
            r.address[1] = 0x25c9cU;
            break;
        }
        case 0xdae2U: { // 7202 moveq #$2, d1
            next = 0xdae4U;
            r.data[1] = 0x2U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xdae4U: { // 6000000a bra.w $daf0
            next = 0xdae8U;
            if (true) { next = 0xdaf0U; transfer_kind = 1U; }
            break;
        }
        case 0xdae8U: { // 43f900025ca4 lea.l $25ca4.l, a1
            next = 0xdaeeU;
            r.address[1] = 0x25ca4U;
            break;
        }
        case 0xdaeeU: { // 7203 moveq #$3, d1
            next = 0xdaf0U;
            r.data[1] = 0x3U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xdaf0U: { // 7000 moveq #$0, d0
            next = 0xdaf2U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xdaf2U: { // 302d0010 move.w $10(a5), d0
            next = 0xdaf6U;
            m.dw(0U, m.word(r.address[5] + 0x10U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xdaf6U: { // 80c1 divu.w d1, d0
            next = 0xdaf8U;
            const auto divisor = r.data[1];
            if ((divisor & 0xffffU) == 0U) {
                const auto result = m.exception(c, 5U, 0xdaf6U, 0xdaf8U);
                if (result.status != TranslationStatus::complete || result.control != 2U) { outcome = result; return true; }
                next = r.program_counter;
            } else {
                m.divide(0U, divisor, false);
            }
            break;
        }
        case 0xdaf8U: { // 4840 swap d0
            next = 0xdafaU;
            const auto value = (r.data[0] << 16U) | (r.data[0] >> 16U);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0xdafaU: { // e540 asl.w #$2, d0
            next = 0xdafcU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xdafcU: { // 22710000 movea.l (a1, d0.w), a1
            next = 0xdb00U;
            const auto source_address = r.address[1] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.lng(source_address);
            r.address[1] = value;
            break;
        }
        case 0xdb00U: { // 2019 move.l (a1)+, d0
            next = 0xdb02U;
            const auto source_address = r.address[1];
            const auto value = m.lng(source_address);
            r.address[1] += 4U;
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0xdb02U: { // 4eb900015e4e jsr $15e4e.l
            next = 0xdb08U;
            const auto result = m.call(c, 284U, 0xdb02U, 0x15e4eU, 0xdb08U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xdb08U: { // 2019 move.l (a1)+, d0
            next = 0xdb0aU;
            const auto source_address = r.address[1];
            const auto value = m.lng(source_address);
            r.address[1] += 4U;
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0xdb0aU: { // 4eb900015e4e jsr $15e4e.l
            next = 0xdb10U;
            const auto result = m.call(c, 284U, 0xdb0aU, 0x15e4eU, 0xdb10U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_advance_phase_callback_dispatch_detail
