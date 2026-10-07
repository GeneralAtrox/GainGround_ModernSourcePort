#include "cpu_b_advance_phase_callback_dispatch_detail.h"

namespace gain_ground::translated::cpu_b_advance_phase_callback_dispatch_detail {

bool dispatch_instruction_region_05(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind,
    std::optional<FunctionResult> &outcome)
{
    auto &c = context;
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xd958U: { // 42780c00 clr.w $c00.w
            next = 0xd95cU;
            const auto destination_address = 0xc00U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd95cU: { // 42780c02 clr.w $c02.w
            next = 0xd960U;
            const auto destination_address = 0xc02U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd960U: { // 42780820 clr.w $820.w
            next = 0xd964U;
            const auto destination_address = 0x820U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd964U: { // 30380836 move.w $836.w, d0
            next = 0xd968U;
            const auto source_address = 0x836U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd968U: { // 5940 subq.w #$4, d0
            next = 0xd96aU;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd96aU: { // 61000372 bsr.w $dcde
            next = 0xd96eU;
            const auto result = m.call(c, 160U, 0xd96aU, 0xdcdeU, 0xd96eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xd96eU: { // 4ef900009c8e jmp $9c8e.l
            next = 0xd974U;
            next = 0x9c8eU;
            transfer_kind = 1U;
            break;
        }
        case 0xd974U: { // 4e75 rts 
            next = 0xd976U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd974U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xd976U: { // 526d0016 addq.w #$1, $16(a5)
            next = 0xd97aU;
            const auto address = r.address[5] + 0x16U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xd97aU: { // 302d0010 move.w $10(a5), d0
            next = 0xd97eU;
            m.dw(0U, m.word(r.address[5] + 0x10U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xd97eU: { // 02400007 andi.w #$7, d0
            next = 0xd982U;
            const auto source_value = 0x7U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd982U: { // e540 asl.w #$2, d0
            next = 0xd984U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xd984U: { // 41f900025a14 lea.l $25a14.l, a0
            next = 0xd98aU;
            r.address[0] = 0x25a14U;
            break;
        }
        case 0xd98aU: { // 20300000 move.l (a0, d0.w), d0
            next = 0xd98eU;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0xd98eU: { // 4eb900015e4e jsr $15e4e.l
            next = 0xd994U;
            const auto result = m.call(c, 284U, 0xd98eU, 0x15e4eU, 0xd994U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xd994U: { // 526d0010 addq.w #$1, $10(a5)
            next = 0xd998U;
            const auto address = r.address[5] + 0x10U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xd998U: { // 0c6d02940010 cmpi.w #$294, $10(a5)
            next = 0xd99eU;
            const auto source_value = 0x294U;
            const auto destination_address = r.address[5] + 0x10U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xd99eU: { // 6a4a bpl.b $d9ea
            next = 0xd9a0U;
            if ((r.status & 8U) == 0U) { next = 0xd9eaU; transfer_kind = 1U; }
            break;
        }
        case 0xd9a0U: { // 302d0010 move.w $10(a5), d0
            next = 0xd9a4U;
            m.dw(0U, m.word(r.address[5] + 0x10U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xd9a4U: { // 0c400240 cmpi.w #$240, d0
            next = 0xd9a8U;
            const auto source_value = 0x240U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xd9a8U: { // 6a14 bpl.b $d9be
            next = 0xd9aaU;
            if ((r.status & 8U) == 0U) { next = 0xd9beU; transfer_kind = 1U; }
            break;
        }
        case 0xd9aaU: { // 43f900025a34 lea.l $25a34.l, a1
            next = 0xd9b0U;
            r.address[1] = 0x25a34U;
            break;
        }
        case 0xd9b0U: { // 02400001 andi.w #$1, d0
            next = 0xd9b4U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd9b4U: { // e540 asl.w #$2, d0
            next = 0xd9b6U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xd9b6U: { // 22710000 movea.l (a1, d0.w), a1
            next = 0xd9baU;
            const auto source_address = r.address[1] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.lng(source_address);
            r.address[1] = value;
            break;
        }
        case 0xd9baU: { // 7407 moveq #$7, d2
            next = 0xd9bcU;
            r.data[2] = 0x7U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0xd9bcU: { // 601a bra.b $d9d8
            next = 0xd9beU;
            if (true) { next = 0xd9d8U; transfer_kind = 1U; }
            break;
        }
        case 0xd9beU: { // 04400240 subi.w #$240, d0
            next = 0xd9c2U;
            const auto source_value = 0x240U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd9c2U: { // 0c400020 cmpi.w #$20, d0
            next = 0xd9c6U;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xd9c6U: { // 6a20 bpl.b $d9e8
            next = 0xd9c8U;
            if ((r.status & 8U) == 0U) { next = 0xd9e8U; transfer_kind = 1U; }
            break;
        }
        case 0xd9c8U: { // 43f900025a3c lea.l $25a3c.l, a1
            next = 0xd9ceU;
            r.address[1] = 0x25a3cU;
            break;
        }
        case 0xd9ceU: { // 0240001c andi.w #$1c, d0
            next = 0xd9d2U;
            const auto source_value = 0x1cU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xd9d2U: { // 22710000 movea.l (a1, d0.w), a1
            next = 0xd9d6U;
            const auto source_address = r.address[1] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.lng(source_address);
            r.address[1] = value;
            break;
        }
        case 0xd9d6U: { // 740f moveq #$f, d2
            next = 0xd9d8U;
            r.data[2] = 0xfU;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0xd9d8U: { // 2019 move.l (a1)+, d0
            next = 0xd9daU;
            const auto source_address = r.address[1];
            const auto value = m.lng(source_address);
            r.address[1] += 4U;
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0xd9daU: { // 4eb900015e4e jsr $15e4e.l
            next = 0xd9e0U;
            const auto result = m.call(c, 284U, 0xd9daU, 0x15e4eU, 0xd9e0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xd9e0U: { // 51cafff6 dbra d2, $d9d8
            next = 0xd9e4U;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0xd9d8U;
            break;
        }
        case 0xd9e4U: { // 6100052a bsr.w $df10
            next = 0xd9e8U;
            const auto result = m.call(c, 636U, 0xd9e4U, 0xdf10U, 0xd9e8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xd9e8U: { // 4e75 rts 
            next = 0xd9eaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd9e8U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_advance_phase_callback_dispatch_detail
