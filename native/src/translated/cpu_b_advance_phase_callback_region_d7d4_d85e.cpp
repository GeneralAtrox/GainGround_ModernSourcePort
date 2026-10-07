#include "cpu_b_advance_phase_callback_dispatch_detail.h"

namespace gain_ground::translated::cpu_b_advance_phase_callback_dispatch_detail {

bool dispatch_instruction_region_02(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind,
    std::optional<FunctionResult> &outcome)
{
    auto &c = context;
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xd7d4U: { // 61000508 bsr.w $dcde
            next = 0xd7d8U;
            const auto result = m.call(c, 160U, 0xd7d4U, 0xdcdeU, 0xd7d8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xd7d8U: { // 4ef900009c8e jmp $9c8e.l
            next = 0xd7deU;
            next = 0x9c8eU;
            transfer_kind = 1U;
            break;
        }
        case 0xd7deU: { // 0c3800020d2d cmpi.b #$2, $d2d.w
            next = 0xd7e4U;
            const auto source_value = 0x2U;
            const auto destination_address = 0xd2dU;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0xd7e4U: { // 6a06 bpl.b $d7ec
            next = 0xd7e6U;
            if ((r.status & 8U) == 0U) { next = 0xd7ecU; transfer_kind = 1U; }
            break;
        }
        case 0xd7e6U: { // 4a780c14 tst.w $c14.w
            next = 0xd7eaU;
            const auto destination_address = 0xc14U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd7eaU: { // 6e30 bgt.b $d81c
            next = 0xd7ecU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0xd81cU; transfer_kind = 1U; }
            break;
        }
        case 0xd7ecU: { // 4a380c06 tst.b $c06.w
            next = 0xd7f0U;
            const auto destination_address = 0xc06U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0xd7f0U: { // 6652 bne.b $d844
            next = 0xd7f2U;
            if ((r.status & 4U) == 0U) { next = 0xd844U; transfer_kind = 1U; }
            break;
        }
        case 0xd7f2U: { // 31fc00010c16 move.w #$1, $c16.w
            next = 0xd7f8U;
            const auto value = 0x1U;
            const auto destination_address = 0xc16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd7f8U: { // 426d0018 clr.w $18(a5)
            next = 0xd7fcU;
            const auto destination_address = r.address[5] + 0x18U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd7fcU: { // 41f80ea2 lea.l $ea2.w, a0
            next = 0xd800U;
            r.address[0] = 0xea2U;
            break;
        }
        case 0xd800U: { // 5250 addq.w #$1, (a0)
            next = 0xd802U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[0];
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xd802U: { // 52680200 addq.w #$1, $200(a0)
            next = 0xd806U;
            const auto address = r.address[0] + 0x200U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xd806U: { // 52680400 addq.w #$1, $400(a0)
            next = 0xd80aU;
            const auto address = r.address[0] + 0x400U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xd80aU: { // 4a780c14 tst.w $c14.w
            next = 0xd80eU;
            const auto destination_address = 0xc14U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd80eU: { // 670a beq.b $d81a
            next = 0xd810U;
            if ((r.status & 4U) != 0U) { next = 0xd81aU; transfer_kind = 1U; }
            break;
        }
        case 0xd810U: { // 4250 clr.w (a0)
            next = 0xd812U;
            const auto destination_address = r.address[0];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd812U: { // 42680200 clr.w $200(a0)
            next = 0xd816U;
            const auto destination_address = r.address[0] + 0x200U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd816U: { // 42680400 clr.w $400(a0)
            next = 0xd81aU;
            const auto destination_address = r.address[0] + 0x400U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd81aU: { // 4e75 rts 
            next = 0xd81cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd81aU, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xd81cU: { // 4a780c0a tst.w $c0a.w
            next = 0xd820U;
            const auto destination_address = 0xc0aU;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd820U: { // 6622 bne.b $d844
            next = 0xd822U;
            if ((r.status & 4U) == 0U) { next = 0xd844U; transfer_kind = 1U; }
            break;
        }
        case 0xd822U: { // 31fc00030c16 move.w #$3, $c16.w
            next = 0xd828U;
            const auto value = 0x3U;
            const auto destination_address = 0xc16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd828U: { // 4df81780 lea.l $1780.w, a6
            next = 0xd82cU;
            r.address[6] = 0x1780U;
            break;
        }
        case 0xd82cU: { // 7402 moveq #$2, d2
            next = 0xd82eU;
            r.data[2] = 0x2U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0xd82eU: { // 7200 moveq #$0, d1
            next = 0xd830U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xd830U: { // 7007 moveq #$7, d0
            next = 0xd832U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xd832U: { // 3c81 move.w d1, (a6)
            next = 0xd834U;
            const auto value = r.data[1];
            const auto destination_address = r.address[6];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd834U: { // 4dee0080 lea.l $80(a6), a6
            next = 0xd838U;
            r.address[6] = r.address[6] + 0x80U;
            break;
        }
        case 0xd838U: { // 51c8fff8 dbra d0, $d832
            next = 0xd83cU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0xd832U;
            break;
        }
        case 0xd83cU: { // 4dee0180 lea.l $180(a6), a6
            next = 0xd840U;
            r.address[6] = r.address[6] + 0x180U;
            break;
        }
        case 0xd840U: { // 51caffee dbra d2, $d830
            next = 0xd844U;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0xd830U;
            break;
        }
        case 0xd844U: { // 4e75 rts 
            next = 0xd846U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd844U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xd846U: { // 526d0018 addq.w #$1, $18(a5)
            next = 0xd84aU;
            const auto address = r.address[5] + 0x18U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xd84aU: { // 0c6d00010018 cmpi.w #$1, $18(a5)
            next = 0xd850U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x18U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xd850U: { // 654e bcs.b $d8a0
            next = 0xd852U;
            if ((r.status & 1U) != 0U) { next = 0xd8a0U; transfer_kind = 1U; }
            break;
        }
        case 0xd852U: { // 31fc00020c16 move.w #$2, $c16.w
            next = 0xd858U;
            const auto value = 0x2U;
            const auto destination_address = 0xc16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd858U: { // 3b7c005a0018 move.w #$5a, $18(a5)
            next = 0xd85eU;
            m.word(r.address[5] + 0x18U, 0x5aU);
            m.logic(0x5aU, 16U);
            break;
        }
        case 0xd85eU: { // 50f80d0c st.b $d0c.w
            next = 0xd862U;
            const auto destination_address = 0xd0cU;
            (void)m.byte(destination_address);
            m.byte(destination_address, 0xffU);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_advance_phase_callback_dispatch_detail
