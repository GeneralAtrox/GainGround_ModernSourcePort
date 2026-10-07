#include "cpu_b_state72_dispatch_record_phase_detail.h"

namespace gain_ground::translated::cpu_b_state72_dispatch_record_phase_detail {

bool dispatch_record_region_06(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind,
    std::optional<FunctionResult> &outcome)
{
    auto &c = context;
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xf7d8U: { // 42700018 clr.w $18(a0, d0.w)
            next = 0xf7dcU;
            const auto destination_address = r.address[0] + 0x18U + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf7dcU: { // 06400020 addi.w #$20, d0
            next = 0xf7e0U;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xf7e0U: { // 0c400400 cmpi.w #$400, d0
            next = 0xf7e4U;
            const auto source_value = 0x400U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xf7e4U: { // 6508 bcs.b $f7ee
            next = 0xf7e6U;
            if ((r.status & 1U) != 0U) { next = 0xf7eeU; transfer_kind = 1U; }
            break;
        }
        case 0xf7e6U: { // 3b7c00050044 move.w #$5, $44(a5)
            next = 0xf7ecU;
            m.word(r.address[5] + 0x44U, 0x5U);
            m.logic(0x5U, 16U);
            break;
        }
        case 0xf7ecU: { // 4240 clr.w d0
            next = 0xf7eeU;
            const auto value = r.data[0];
            (void)value;
            m.dw(0U, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf7eeU: { // 3b400046 move.w d0, $46(a5)
            next = 0xf7f2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x46U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf7f2U: { // 062d0020005a addi.b #$20, $5a(a5)
            next = 0xf7f8U;
            const auto source_value = 0x20U;
            const auto destination_address = r.address[5] + 0x5aU;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0xf7f8U: { // 42ad001e clr.l $1e(a5)
            next = 0xf7fcU;
            const auto destination_address = r.address[5] + 0x1eU;
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0xf7fcU: { // 2b7cfffe80000026 move.l #$fffe8000, $26(a5)
            next = 0xf804U;
            m.lng(r.address[5] + 0x26U, 0xfffe8000U);
            m.logic(0xfffe8000U, 32U);
            break;
        }
        case 0xf804U: { // 4eb90000fed6 jsr $fed6.l
            next = 0xf80aU;
            const auto result = m.call(c, 201U, 0xf804U, 0xfed6U, 0xf80aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xf80aU: { // 4eb9000102aa jsr $102aa.l
            next = 0xf810U;
            const auto result = m.call(c, 204U, 0xf80aU, 0x102aaU, 0xf810U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xf810U: { // 4eb90000fc68 jsr $fc68.l
            next = 0xf816U;
            const auto result = m.call(c, 193U, 0xf810U, 0xfc68U, 0xf816U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xf816U: { // 61000496 bsr.w $fcae
            next = 0xf81aU;
            const auto result = m.call(c, 195U, 0xf816U, 0xfcaeU, 0xf81aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xf81aU: { // 4e75 rts 
            next = 0xf81cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf81aU, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xf81cU: { // 4a2d003f tst.b $3f(a5)
            next = 0xf820U;
            m.logic(m.byte(r.address[5] + 0x3fU), 8U);
            break;
        }
        case 0xf820U: { // 6634 bne.b $f856
            next = 0xf822U;
            if ((r.status & 4U) == 0U) { next = 0xf856U; transfer_kind = 1U; }
            break;
        }
        case 0xf822U: { // 2f0c move.l a4, -(a7)
            next = 0xf824U;
            const auto value = r.address[4];
            r.address[7] -= 4U;
            const auto destination_address = r.address[7];
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf824U: { // 303c004a move.w #$4a, d0
            next = 0xf828U;
            m.dw(0U, 0x4aU);
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xf828U: { // 4eb900016ff8 jsr $16ff8.l
            next = 0xf82eU;
            const auto result = m.call(c, 308U, 0xf828U, 0x16ff8U, 0xf82eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xf82eU: { // 285f movea.l (a7)+, a4
            next = 0xf830U;
            const auto source_address = r.address[7];
            const auto value = m.lng(source_address);
            r.address[7] += 4U;
            r.address[4] = value;
            break;
        }
        case 0xf830U: { // 296d00540092 move.l $54(a5), $92(a4)
            next = 0xf836U;
            const auto source_address = r.address[5] + 0x54U;
            const auto value = m.lng(source_address);
            const auto destination_address = r.address[4] + 0x92U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf836U: { // 422d003c clr.b $3c(a5)
            next = 0xf83aU;
            const auto destination_address = r.address[5] + 0x3cU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0xf83aU: { // 4a6d005c tst.w $5c(a5)
            next = 0xf83eU;
            m.logic(m.word(r.address[5] + 0x5cU), 16U);
            break;
        }
        case 0xf83eU: { // 6716 beq.b $f856
            next = 0xf840U;
            if ((r.status & 4U) != 0U) { next = 0xf856U; transfer_kind = 1U; }
            break;
        }
        case 0xf840U: { // 3c6d005c movea.w $5c(a5), a6
            next = 0xf844U;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xf844U: { // 3d7c00020044 move.w #$2, $44(a6)
            next = 0xf84aU;
            m.word(r.address[6] + 0x44U, 0x2U);
            m.logic(0x2U, 16U);
            break;
        }
        case 0xf84aU: { // 2d6d00120062 move.l $12(a5), $62(a6)
            next = 0xf850U;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.lng(source_address);
            const auto destination_address = r.address[6] + 0x62U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf850U: { // 2d6d001a0066 move.l $1a(a5), $66(a6)
            next = 0xf856U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.lng(source_address);
            const auto destination_address = r.address[6] + 0x66U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf856U: { // 206d0054 movea.l $54(a5), a0
            next = 0xf85aU;
            const auto source_address = r.address[5] + 0x54U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0xf85aU: { // 532d003c subq.b #$1, $3c(a5)
            next = 0xf85eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x3cU;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0xf85eU: { // 6e32 bgt.b $f892
            next = 0xf860U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0xf892U; transfer_kind = 1U; }
            break;
        }
        case 0xf860U: { // 522d003f addq.b #$1, $3f(a5)
            next = 0xf864U;
            const auto address = r.address[5] + 0x3fU;
            const auto value = m.add(m.byte(address), 0x1U, 8U);
            m.byte(address, value);
            break;
        }
        case 0xf864U: { // 7000 moveq #$0, d0
            next = 0xf866U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xf866U: { // 102d003f move.b $3f(a5), d0
            next = 0xf86aU;
            const auto source_address = r.address[5] + 0x3fU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf86aU: { // 0c400011 cmpi.w #$11, d0
            next = 0xf86eU;
            const auto source_value = 0x11U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xf86eU: { // 6a28 bpl.b $f898
            next = 0xf870U;
            if ((r.status & 8U) == 0U) { next = 0xf898U; transfer_kind = 1U; }
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_state72_dispatch_record_phase_detail
