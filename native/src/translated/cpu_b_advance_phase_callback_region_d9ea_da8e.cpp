#include "cpu_b_advance_phase_callback_dispatch_detail.h"

namespace gain_ground::translated::cpu_b_advance_phase_callback_dispatch_detail {

bool dispatch_instruction_region_06(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind,
    std::optional<FunctionResult> &outcome)
{
    auto &c = context;
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xd9eaU: { // 4a380c04 tst.b $c04.w
            next = 0xd9eeU;
            const auto destination_address = 0xc04U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0xd9eeU: { // 661e bne.b $da0e
            next = 0xd9f0U;
            if ((r.status & 4U) == 0U) { next = 0xda0eU; transfer_kind = 1U; }
            break;
        }
        case 0xd9f0U: { // 31fc00060c16 move.w #$6, $c16.w
            next = 0xd9f6U;
            const auto value = 0x6U;
            const auto destination_address = 0xc16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xd9f6U: { // 3b7c0001000c move.w #$1, $c(a5)
            next = 0xd9fcU;
            m.word(r.address[5] + 0xcU, 0x1U);
            m.logic(0x1U, 16U);
            break;
        }
        case 0xd9fcU: { // 426d0010 clr.w $10(a5)
            next = 0xda00U;
            const auto destination_address = r.address[5] + 0x10U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xda00U: { // 426d0012 clr.w $12(a5)
            next = 0xda04U;
            const auto destination_address = r.address[5] + 0x12U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xda04U: { // 426d0014 clr.w $14(a5)
            next = 0xda08U;
            const auto destination_address = r.address[5] + 0x14U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xda08U: { // 4eb90000b684 jsr $b684.l
            next = 0xda0eU;
            const auto result = m.call(c, 628U, 0xda08U, 0xb684U, 0xda0eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xda0eU: { // 4e75 rts 
            next = 0xda10U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xda0eU, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        case 0xda10U: { // 526d0016 addq.w #$1, $16(a5)
            next = 0xda14U;
            const auto address = r.address[5] + 0x16U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xda14U: { // 0c6d08680016 cmpi.w #$868, $16(a5)
            next = 0xda1aU;
            const auto source_value = 0x868U;
            const auto destination_address = r.address[5] + 0x16U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xda1aU: { // 6606 bne.b $da22
            next = 0xda1cU;
            if ((r.status & 4U) == 0U) { next = 0xda22U; transfer_kind = 1U; }
            break;
        }
        case 0xda1cU: { // 4eb900017054 jsr $17054.l
            next = 0xda22U;
            const auto result = m.call(c, 311U, 0xda1cU, 0x17054U, 0xda22U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xda22U: { // 6100053e bsr.w $df62
            next = 0xda26U;
            const auto result = m.call(c, 637U, 0xda22U, 0xdf62U, 0xda26U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xda26U: { // 526d0010 addq.w #$1, $10(a5)
            next = 0xda2aU;
            const auto address = r.address[5] + 0x10U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xda2aU: { // 302d000c move.w $c(a5), d0
            next = 0xda2eU;
            m.dw(0U, m.word(r.address[5] + 0xcU));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xda2eU: { // d040 add.w d0, d0
            next = 0xda30U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xda30U: { // 41fa065e lea.l $e090(pc), a0
            next = 0xda34U;
            const auto source_address = 0xe090U;
            r.address[0] = source_address;
            break;
        }
        case 0xda34U: { // 30300000 move.w (a0, d0.w), d0
            next = 0xda38U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xda38U: { // b06d0010 cmp.w $10(a5), d0
            next = 0xda3cU;
            const auto source_address = r.address[5] + 0x10U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xda3cU: { // 6e18 bgt.b $da56
            next = 0xda3eU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0xda56U; transfer_kind = 1U; }
            break;
        }
        case 0xda3eU: { // 526d000c addq.w #$1, $c(a5)
            next = 0xda42U;
            const auto address = r.address[5] + 0xcU;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xda42U: { // 0c6d000f000c cmpi.w #$f, $c(a5)
            next = 0xda48U;
            const auto source_value = 0xfU;
            const auto destination_address = r.address[5] + 0xcU;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xda48U: { // 6a000104 bpl.w $db4e
            next = 0xda4cU;
            if ((r.status & 8U) == 0U) { next = 0xdb4eU; transfer_kind = 1U; }
            break;
        }
        case 0xda4cU: { // 426d0010 clr.w $10(a5)
            next = 0xda50U;
            const auto destination_address = r.address[5] + 0x10U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xda50U: { // 4eb90000b6c8 jsr $b6c8.l
            next = 0xda56U;
            const auto result = m.call(c, 629U, 0xda50U, 0xb6c8U, 0xda56U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xda56U: { // 302d000c move.w $c(a5), d0
            next = 0xda5aU;
            m.dw(0U, m.word(r.address[5] + 0xcU));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xda5aU: { // 5340 subq.w #$1, d0
            next = 0xda5cU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xda5cU: { // e540 asl.w #$2, d0
            next = 0xda5eU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xda5eU: { // 4efb0002 jmp $da62(pc, d0.w)
            next = 0xda62U;
            next = (0xda62U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0xda62U: { // 60000036 bra.w $da9a
            next = 0xda66U;
            if (true) { next = 0xda9aU; transfer_kind = 1U; }
            break;
        }
        case 0xda66U: { // 60000072 bra.w $dada
            next = 0xda6aU;
            if (true) { next = 0xdadaU; transfer_kind = 1U; }
            break;
        }
        case 0xda6aU: { // 60000070 bra.w $dadc
            next = 0xda6eU;
            if (true) { next = 0xdadcU; transfer_kind = 1U; }
            break;
        }
        case 0xda6eU: { // 60000078 bra.w $dae8
            next = 0xda72U;
            if (true) { next = 0xdae8U; transfer_kind = 1U; }
            break;
        }
        case 0xda72U: { // 6000009e bra.w $db12
            next = 0xda76U;
            if (true) { next = 0xdb12U; transfer_kind = 1U; }
            break;
        }
        case 0xda76U: { // 600000a4 bra.w $db1c
            next = 0xda7aU;
            if (true) { next = 0xdb1cU; transfer_kind = 1U; }
            break;
        }
        case 0xda7aU: { // 600000ac bra.w $db28
            next = 0xda7eU;
            if (true) { next = 0xdb28U; transfer_kind = 1U; }
            break;
        }
        case 0xda7eU: { // 600000b4 bra.w $db34
            next = 0xda82U;
            if (true) { next = 0xdb34U; transfer_kind = 1U; }
            break;
        }
        case 0xda82U: { // 600000ba bra.w $db3e
            next = 0xda86U;
            if (true) { next = 0xdb3eU; transfer_kind = 1U; }
            break;
        }
        case 0xda86U: { // 600000b6 bra.w $db3e
            next = 0xda8aU;
            if (true) { next = 0xdb3eU; transfer_kind = 1U; }
            break;
        }
        case 0xda8aU: { // 600000b4 bra.w $db40
            next = 0xda8eU;
            if (true) { next = 0xdb40U; transfer_kind = 1U; }
            break;
        }
        case 0xda8eU: { // 600000bc bra.w $db4c
            next = 0xda92U;
            if (true) { next = 0xdb4cU; transfer_kind = 1U; }
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_advance_phase_callback_dispatch_detail
