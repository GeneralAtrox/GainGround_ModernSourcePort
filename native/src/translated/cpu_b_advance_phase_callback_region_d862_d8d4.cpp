#include "cpu_b_advance_phase_callback_dispatch_detail.h"

namespace gain_ground::translated::cpu_b_advance_phase_callback_dispatch_detail {

bool dispatch_instruction_region_03(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind,
    std::optional<FunctionResult> &outcome)
{
    auto &c = context;
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xd862U: { // 52780c02 addq.w #$1, $c02.w
            next = 0xd866U;
            const auto source_value = 0x1U;
            const auto destination_address = 0xc02U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xd866U: { // 30380c02 move.w $c02.w, d0
            next = 0xd86aU;
            const auto source_address = 0xc02U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd86aU: { // 7200 moveq #$0, d1
            next = 0xd86cU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xd86cU: { // 3200 move.w d0, d1
            next = 0xd86eU;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd86eU: { // 83fc000a divs.w #$a, d1
            next = 0xd872U;
            const auto divisor = 0xaU;
            if ((divisor & 0xffffU) == 0U) {
                const auto result = m.exception(c, 5U, 0xd86eU, 0xd872U);
                if (result.status != TranslationStatus::complete || result.control != 2U) { outcome = result; return true; }
                next = r.program_counter;
            } else {
                m.divide(1U, divisor, true);
            }
            break;
        }
        case 0xd872U: { // 4841 swap d1
            next = 0xd874U;
            const auto value = (r.data[1] << 16U) | (r.data[1] >> 16U);
            r.data[1] = value;
            m.logic(value, 32U);
            break;
        }
        case 0xd874U: { // 4a41 tst.w d1
            next = 0xd876U;
            const auto value = r.data[1];
            m.logic(value, 16U);
            break;
        }
        case 0xd876U: { // 661c bne.b $d894
            next = 0xd878U;
            if ((r.status & 4U) == 0U) { next = 0xd894U; transfer_kind = 1U; }
            break;
        }
        case 0xd878U: { // 52780c00 addq.w #$1, $c00.w
            next = 0xd87cU;
            const auto source_value = 0x1U;
            const auto destination_address = 0xc00U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xd87cU: { // 32380c00 move.w $c00.w, d1
            next = 0xd880U;
            const auto source_address = 0xc00U;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd880U: { // 5241 addq.w #$1, d1
            next = 0xd882U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0xd882U: { // 31c18002 move.w d1, $8002.w
            next = 0xd886U;
            const auto value = r.data[1];
            const auto destination_address = 0xffff8002U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd886U: { // 5240 addq.w #$1, d0
            next = 0xd888U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd888U: { // 31c08006 move.w d0, $8006.w
            next = 0xd88cU;
            const auto value = r.data[0];
            const auto destination_address = 0xffff8006U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd88cU: { // 4eb90000a8b6 jsr $a8b6.l
            next = 0xd892U;
            const auto result = m.call(c, 615U, 0xd88cU, 0xa8b6U, 0xd892U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xd892U: { // 4e75 rts 
            next = 0xd894U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd892U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xd894U: { // 5240 addq.w #$1, d0
            next = 0xd896U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd896U: { // 31c08006 move.w d0, $8006.w
            next = 0xd89aU;
            const auto value = r.data[0];
            const auto destination_address = 0xffff8006U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd89aU: { // 4eb90000a87c jsr $a87c.l
            next = 0xd8a0U;
            const auto result = m.call(c, 614U, 0xd89aU, 0xa87cU, 0xd8a0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xd8a0U: { // 4e75 rts 
            next = 0xd8a2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd8a0U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xd8a2U: { // 302d0018 move.w $18(a5), d0
            next = 0xd8a6U;
            m.dw(0U, m.word(r.address[5] + 0x18U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xd8a6U: { // 6712 beq.b $d8ba
            next = 0xd8a8U;
            if ((r.status & 4U) != 0U) { next = 0xd8baU; transfer_kind = 1U; }
            break;
        }
        case 0xd8a8U: { // 5340 subq.w #$1, d0
            next = 0xd8aaU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd8aaU: { // 3b400018 move.w d0, $18(a5)
            next = 0xd8aeU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x18U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd8aeU: { // 0c40003c cmpi.w #$3c, d0
            next = 0xd8b2U;
            const auto source_value = 0x3cU;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xd8b2U: { // 6a04 bpl.b $d8b8
            next = 0xd8b4U;
            if ((r.status & 8U) == 0U) { next = 0xd8b8U; transfer_kind = 1U; }
            break;
        }
        case 0xd8b4U: { // 42380d0c clr.b $d0c.w
            next = 0xd8b8U;
            const auto destination_address = 0xd0cU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0xd8b8U: { // 4e75 rts 
            next = 0xd8baU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd8b8U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xd8baU: { // 4a380c05 tst.b $c05.w
            next = 0xd8beU;
            const auto destination_address = 0xc05U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0xd8beU: { // 664c bne.b $d90c
            next = 0xd8c0U;
            if ((r.status & 4U) == 0U) { next = 0xd90cU; transfer_kind = 1U; }
            break;
        }
        case 0xd8c0U: { // 4a788002 tst.w $8002.w
            next = 0xd8c4U;
            const auto destination_address = 0xffff8002U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd8c4U: { // 6646 bne.b $d90c
            next = 0xd8c6U;
            if ((r.status & 4U) == 0U) { next = 0xd90cU; transfer_kind = 1U; }
            break;
        }
        case 0xd8c6U: { // 4a788006 tst.w $8006.w
            next = 0xd8caU;
            const auto destination_address = 0xffff8006U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd8caU: { // 6640 bne.b $d90c
            next = 0xd8ccU;
            if ((r.status & 4U) == 0U) { next = 0xd90cU; transfer_kind = 1U; }
            break;
        }
        case 0xd8ccU: { // 0c7800040c00 cmpi.w #$4, $c00.w
            next = 0xd8d2U;
            const auto source_value = 0x4U;
            const auto destination_address = 0xc00U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xd8d2U: { // 6b22 bmi.b $d8f6
            next = 0xd8d4U;
            if ((r.status & 8U) != 0U) { next = 0xd8f6U; transfer_kind = 1U; }
            break;
        }
        case 0xd8d4U: { // 31fc00050c16 move.w #$5, $c16.w
            next = 0xd8daU;
            const auto value = 0x5U;
            const auto destination_address = 0xc16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_advance_phase_callback_dispatch_detail
