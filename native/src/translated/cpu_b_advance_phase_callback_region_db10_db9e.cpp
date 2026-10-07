#include "cpu_b_advance_phase_callback_dispatch_detail.h"

namespace gain_ground::translated::cpu_b_advance_phase_callback_dispatch_detail {

bool dispatch_instruction_region_08(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind,
    std::optional<FunctionResult> &outcome)
{
    auto &c = context;
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xdb10U: { // 4e75 rts 
            next = 0xdb12U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xdb10U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xdb12U: { // 43f900025cb0 lea.l $25cb0.l, a1
            next = 0xdb18U;
            r.address[1] = 0x25cb0U;
            break;
        }
        case 0xdb18U: { // 6000ffd4 bra.w $daee
            next = 0xdb1cU;
            if (true) { next = 0xdaeeU; transfer_kind = 1U; }
            break;
        }
        case 0xdb1cU: { // 43f900025cbc lea.l $25cbc.l, a1
            next = 0xdb22U;
            r.address[1] = 0x25cbcU;
            break;
        }
        case 0xdb22U: { // 7206 moveq #$6, d1
            next = 0xdb24U;
            r.data[1] = 0x6U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xdb24U: { // 6000ffca bra.w $daf0
            next = 0xdb28U;
            if (true) { next = 0xdaf0U; transfer_kind = 1U; }
            break;
        }
        case 0xdb28U: { // 43f900025cd4 lea.l $25cd4.l, a1
            next = 0xdb2eU;
            r.address[1] = 0x25cd4U;
            break;
        }
        case 0xdb2eU: { // 7206 moveq #$6, d1
            next = 0xdb30U;
            r.data[1] = 0x6U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xdb30U: { // 6000ffbe bra.w $daf0
            next = 0xdb34U;
            if (true) { next = 0xdaf0U; transfer_kind = 1U; }
            break;
        }
        case 0xdb34U: { // 43f900025cb0 lea.l $25cb0.l, a1
            next = 0xdb3aU;
            r.address[1] = 0x25cb0U;
            break;
        }
        case 0xdb3aU: { // 6000ffb2 bra.w $daee
            next = 0xdb3eU;
            if (true) { next = 0xdaeeU; transfer_kind = 1U; }
            break;
        }
        case 0xdb3eU: { // 4e75 rts 
            next = 0xdb40U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xdb3eU, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xdb40U: { // 43f900025cbc lea.l $25cbc.l, a1
            next = 0xdb46U;
            r.address[1] = 0x25cbcU;
            break;
        }
        case 0xdb46U: { // 7206 moveq #$6, d1
            next = 0xdb48U;
            r.data[1] = 0x6U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xdb48U: { // 6000ffa6 bra.w $daf0
            next = 0xdb4cU;
            if (true) { next = 0xdaf0U; transfer_kind = 1U; }
            break;
        }
        case 0xdb4cU: { // 4e75 rts 
            next = 0xdb4eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xdb4cU, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xdb4eU: { // 31fc00070c16 move.w #$7, $c16.w
            next = 0xdb54U;
            const auto value = 0x7U;
            const auto destination_address = 0xc16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xdb54U: { // 426d0010 clr.w $10(a5)
            next = 0xdb58U;
            const auto destination_address = r.address[5] + 0x10U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xdb58U: { // 526d0016 addq.w #$1, $16(a5)
            next = 0xdb5cU;
            const auto address = r.address[5] + 0x16U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xdb5cU: { // 0c6d08680016 cmpi.w #$868, $16(a5)
            next = 0xdb62U;
            const auto source_value = 0x868U;
            const auto destination_address = r.address[5] + 0x16U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xdb62U: { // 6606 bne.b $db6a
            next = 0xdb64U;
            if ((r.status & 4U) == 0U) { next = 0xdb6aU; transfer_kind = 1U; }
            break;
        }
        case 0xdb64U: { // 4eb900017054 jsr $17054.l
            next = 0xdb6aU;
            const auto result = m.call(c, 311U, 0xdb64U, 0x17054U, 0xdb6aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xdb6aU: { // 526d0010 addq.w #$1, $10(a5)
            next = 0xdb6eU;
            const auto address = r.address[5] + 0x10U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xdb6eU: { // 0c6d00960010 cmpi.w #$96, $10(a5)
            next = 0xdb74U;
            const auto source_value = 0x96U;
            const auto destination_address = r.address[5] + 0x10U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xdb74U: { // 6b28 bmi.b $db9e
            next = 0xdb76U;
            if ((r.status & 8U) != 0U) { next = 0xdb9eU; transfer_kind = 1U; }
            break;
        }
        case 0xdb76U: { // 31fc00018002 move.w #$1, $8002.w
            next = 0xdb7cU;
            const auto value = 0x1U;
            const auto destination_address = 0xffff8002U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xdb7cU: { // 42780c00 clr.w $c00.w
            next = 0xdb80U;
            const auto destination_address = 0xc00U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xdb80U: { // 42780c02 clr.w $c02.w
            next = 0xdb84U;
            const auto destination_address = 0xc02U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xdb84U: { // 4eb90001826e jsr $1826e.l
            next = 0xdb8aU;
            const auto result = m.call(c, 322U, 0xdb84U, 0x1826eU, 0xdb8aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xdb8aU: { // 42780820 clr.w $820.w
            next = 0xdb8eU;
            const auto destination_address = 0x820U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xdb8eU: { // 30380836 move.w $836.w, d0
            next = 0xdb92U;
            const auto source_address = 0x836U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xdb92U: { // 5940 subq.w #$4, d0
            next = 0xdb94U;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xdb94U: { // 61000148 bsr.w $dcde
            next = 0xdb98U;
            const auto result = m.call(c, 160U, 0xdb94U, 0xdcdeU, 0xdb98U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xdb98U: { // 4ef900009c8e jmp $9c8e.l
            next = 0xdb9eU;
            next = 0x9c8eU;
            transfer_kind = 1U;
            break;
        }
        case 0xdb9eU: { // 4e75 rts 
            next = 0xdba0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xdb9eU, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_advance_phase_callback_dispatch_detail
