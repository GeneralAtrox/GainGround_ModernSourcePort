#include "cpu_b_advance_phase_callback_dispatch_detail.h"

namespace gain_ground::translated::cpu_b_advance_phase_callback_dispatch_detail {

bool dispatch_instruction_region_04(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind,
    std::optional<FunctionResult> &outcome)
{
    auto &c = context;
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xd8daU: { // 426d000c clr.w $c(a5)
            next = 0xd8deU;
            const auto destination_address = r.address[5] + 0xcU;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd8deU: { // 426d0010 clr.w $10(a5)
            next = 0xd8e2U;
            const auto destination_address = r.address[5] + 0x10U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd8e2U: { // 426d0016 clr.w $16(a5)
            next = 0xd8e6U;
            const auto destination_address = r.address[5] + 0x16U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd8e6U: { // 48e70004 movem.l a5, -(a7)
            next = 0xd8eaU;
            const auto saved_13 = r.address[5];
            auto address = r.address[7];
            address -= 4U;
            m.word(address + 2U, saved_13);
            m.word(address, saved_13 >> 16U);
            r.address[7] = address;
            break;
        }
        case 0xd8eaU: { // 4eb90000a8f0 jsr $a8f0.l
            next = 0xd8f0U;
            const auto result = m.call(c, 616U, 0xd8eaU, 0xa8f0U, 0xd8f0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xd8f0U: { // 4cdf2000 movem.l (a7)+, a5
            next = 0xd8f4U;
            const auto memory_address = r.address[7];
            auto address = memory_address;
            r.address[5] = m.lng(address);
            address += 4U;
            (void)m.word(address);
            r.address[7] = address;
            break;
        }
        case 0xd8f4U: { // 4e75 rts 
            next = 0xd8f6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd8f4U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xd8f6U: { // 48e70004 movem.l a5, -(a7)
            next = 0xd8faU;
            const auto saved_13 = r.address[5];
            auto address = r.address[7];
            address -= 4U;
            m.word(address + 2U, saved_13);
            m.word(address, saved_13 >> 16U);
            r.address[7] = address;
            break;
        }
        case 0xd8faU: { // 4eb90000a618 jsr $a618.l
            next = 0xd900U;
            const auto result = m.call(c, 140U, 0xd8faU, 0xa618U, 0xd900U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xd900U: { // 4cdf2000 movem.l (a7)+, a5
            next = 0xd904U;
            const auto memory_address = r.address[7];
            auto address = memory_address;
            r.address[5] = m.lng(address);
            address += 4U;
            (void)m.word(address);
            r.address[7] = address;
            break;
        }
        case 0xd904U: { // 42780c16 clr.w $c16.w
            next = 0xd908U;
            const auto destination_address = 0xc16U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd908U: { // 42780c10 clr.w $c10.w
            next = 0xd90cU;
            const auto destination_address = 0xc10U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd90cU: { // 4e75 rts 
            next = 0xd90eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd90cU, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xd90eU: { // 42780c16 clr.w $c16.w
            next = 0xd912U;
            const auto destination_address = 0xc16U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd912U: { // 4e75 rts 
            next = 0xd914U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd912U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xd914U: { // 4a780c10 tst.w $c10.w
            next = 0xd918U;
            const auto destination_address = 0xc10U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd918U: { // 670e beq.b $d928
            next = 0xd91aU;
            if ((r.status & 4U) != 0U) { next = 0xd928U; transfer_kind = 1U; }
            break;
        }
        case 0xd91aU: { // 42780c16 clr.w $c16.w
            next = 0xd91eU;
            const auto destination_address = 0xc16U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd91eU: { // 42787452 clr.w $7452.w
            next = 0xd922U;
            const auto destination_address = 0x7452U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd922U: { // 4278745c clr.w $745c.w
            next = 0xd926U;
            const auto destination_address = 0x745cU;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xd926U: { // 4e75 rts 
            next = 0xd928U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xd926U, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xd928U: { // 4a380c06 tst.b $c06.w
            next = 0xd92cU;
            const auto destination_address = 0xc06U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0xd92cU: { // 6646 bne.b $d974
            next = 0xd92eU;
            if ((r.status & 4U) == 0U) { next = 0xd974U; transfer_kind = 1U; }
            break;
        }
        case 0xd92eU: { // 4a380c04 tst.b $c04.w
            next = 0xd932U;
            const auto destination_address = 0xc04U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0xd932U: { // 6640 bne.b $d974
            next = 0xd934U;
            if ((r.status & 4U) == 0U) { next = 0xd974U; transfer_kind = 1U; }
            break;
        }
        case 0xd934U: { // 536d0018 subq.w #$1, $18(a5)
            next = 0xd938U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x18U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xd938U: { // 6e3a bgt.b $d974
            next = 0xd93aU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0xd974U; transfer_kind = 1U; }
            break;
        }
        case 0xd93aU: { // 536d001a subq.w #$1, $1a(a5)
            next = 0xd93eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xd93eU: { // 6b0c bmi.b $d94c
            next = 0xd940U;
            if ((r.status & 8U) != 0U) { next = 0xd94cU; transfer_kind = 1U; }
            break;
        }
        case 0xd940U: { // 3b7c001e0018 move.w #$1e, $18(a5)
            next = 0xd946U;
            m.word(r.address[5] + 0x18U, 0x1eU);
            m.logic(0x1eU, 16U);
            break;
        }
        case 0xd946U: { // 61000538 bsr.w $de80
            next = 0xd94aU;
            const auto result = m.call(c, 168U, 0xd946U, 0xde80U, 0xd94aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xd94aU: { // 6028 bra.b $d974
            next = 0xd94cU;
            if (true) { next = 0xd974U; transfer_kind = 1U; }
            break;
        }
        case 0xd94cU: { // 4a780c00 tst.w $c00.w
            next = 0xd950U;
            const auto destination_address = 0xc00U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0xd950U: { // 670a beq.b $d95c
            next = 0xd952U;
            if ((r.status & 4U) != 0U) { next = 0xd95cU; transfer_kind = 1U; }
            break;
        }
        case 0xd952U: { // 31fc00018002 move.w #$1, $8002.w
            next = 0xd958U;
            const auto value = 0x1U;
            const auto destination_address = 0xffff8002U;
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
