// Implemented but unverified. Validation is recorded in the current function work packet.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult cpu_a_reset_entry(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 0U, 255U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x408U: { // 3e7c0000 movea.w #$0, a7
            next = 0x40cU;
            (void)m.word(0x40cU);
            const auto value = 0x0U;
            r.address[7] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            (void)m.word(0x40eU);
            break;
        }
        case 0x40cU: { // 46fc2600 move.w #$2600, sr
            next = 0x410U;
            (void)m.word(0x410U);
            r.status = static_cast<std::uint16_t>(0x2600U);
            (void)m.word(0x410U);
            (void)m.word(0x412U);
            break;
        }
        case 0x410U: { // 13fc00040080001d move.b #$4, $80001d.l
            next = 0x418U;
            (void)m.word(0x414U);
            (void)m.word(0x416U);
            (void)m.word(0x418U);
            const auto value = 0x4U;
            const auto destination_address = 0x80001dU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            (void)m.word(0x41aU);
            break;
        }
        case 0x418U: { // 13fc000000800007 move.b #$0, $800007.l
            next = 0x420U;
            (void)m.word(0x41cU);
            (void)m.word(0x41eU);
            (void)m.word(0x420U);
            const auto value = 0x0U;
            const auto destination_address = 0x800007U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            (void)m.word(0x422U);
            break;
        }
        case 0x420U: { // 13fc00880080001f move.b #$88, $80001f.l
            next = 0x428U;
            (void)m.word(0x424U);
            (void)m.word(0x426U);
            (void)m.word(0x428U);
            const auto value = 0x88U;
            const auto destination_address = 0x80001fU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            c.host->write_hardware(4U, m.cpu, m.state, 0x420U, 0x80000eU, 0U, 0xffU);
            (void)m.word(0x42aU);
            break;
        }
        case 0x428U: { // 13fc000000404019 move.b #$0, $404019.l
            next = 0x430U;
            (void)m.word(0x42cU);
            (void)m.word(0x42eU);
            (void)m.word(0x430U);
            const auto value = 0x0U;
            const auto destination_address = 0x404019U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            (void)m.word(0x432U);
            break;
        }
        case 0x430U: { // 13fc000000270001 move.b #$0, $270001.l
            next = 0x438U;
            (void)m.word(0x434U);
            (void)m.word(0x436U);
            (void)m.word(0x438U);
            const auto value = 0x0U;
            const auto destination_address = 0x270001U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            (void)m.word(0x43aU);
            break;
        }
        case 0x438U: { // 33fcffc600240000 move.w #$ffc6, $240000.l
            next = 0x440U;
            (void)m.word(0x43cU);
            (void)m.word(0x43eU);
            (void)m.word(0x440U);
            const auto value = 0xffc6U;
            const auto destination_address = 0x240000U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x442U);
            break;
        }
        case 0x440U: { // 33fcfff000260000 move.w #$fff0, $260000.l
            next = 0x448U;
            (void)m.word(0x444U);
            (void)m.word(0x446U);
            (void)m.word(0x448U);
            const auto value = 0xfff0U;
            const auto destination_address = 0x260000U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x44aU);
            break;
        }
        case 0x448U: { // 13fc000800a00005 move.b #$8, $a00005.l
            next = 0x450U;
            (void)m.word(0x44cU);
            (void)m.word(0x44eU);
            (void)m.word(0x450U);
            const auto value = 0x8U;
            const auto destination_address = 0xa00005U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            (void)m.word(0x452U);
            break;
        }
        case 0x450U: { // 13fc000000a00007 move.b #$0, $a00007.l
            next = 0x458U;
            (void)m.word(0x454U);
            (void)m.word(0x456U);
            (void)m.word(0x458U);
            const auto value = 0x0U;
            const auto destination_address = 0xa00007U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            (void)m.word(0x45aU);
            break;
        }
        case 0x458U: { // 383c1111 move.w #$1111, d4
            next = 0x45cU;
            (void)m.word(0x45cU);
            m.dw(4U, 0x1111U);
            m.logic(r.data[4], 16U);
            (void)m.word(0x45eU);
            break;
        }
        case 0x45cU: { // 4dfa19f6 lea.l $1e54(pc), a6
            next = 0x460U;
            (void)m.word(0x460U);
            const auto source_address = 0x1e54U;
            r.address[6] = source_address;
            (void)m.word(0x462U);
            break;
        }
        case 0x460U: { // 7e09 moveq #$9, d7
            next = 0x462U;
            r.data[7] = 0x9U;
            m.logic(r.data[7], 32U);
            (void)m.word(0x464U);
            break;
        }
        case 0x462U: { // 33fc111100080000 move.w #$1111, $80000.l
            next = 0x46aU;
            (void)m.word(0x466U);
            (void)m.word(0x468U);
            (void)m.word(0x46aU);
            const auto value = 0x1111U;
            const auto destination_address = 0x80000U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x46cU);
            break;
        }
        case 0x46aU: { // 33fc2222000c0000 move.w #$2222, $c0000.l
            next = 0x472U;
            (void)m.word(0x46eU);
            (void)m.word(0x470U);
            (void)m.word(0x472U);
            const auto value = 0x2222U;
            const auto destination_address = 0xc0000U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x474U);
            break;
        }
        case 0x472U: { // 0c79111100080000 cmpi.w #$1111, $80000.l
            next = 0x47aU;
            (void)m.word(0x476U);
            (void)m.word(0x478U);
            (void)m.word(0x47aU);
            const auto source_value = 0x1111U;
            const auto destination_address = 0x80000U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            (void)m.word(0x47cU);
            break;
        }
        case 0x47aU: { // 6606 bne.b $482
            next = 0x47cU;
            if ((r.status & 4U) == 0U) { next = 0x482U; transfer_kind = 1U; }
            if (transfer_kind == 1U) { (void)m.word(next); }
            (void)m.word(next + 2U);
            break;
        }
        case 0x482U: { // 2a5e movea.l (a6)+, a5
            next = 0x484U;
            const auto source_address = r.address[6];
            const auto value = m.lng(source_address);
            r.address[6] += 4U;
            r.address[5] = value;
            (void)m.word(0x486U);
            break;
        }
        case 0x484U: { // 2c1e move.l (a6)+, d6
            next = 0x486U;
            const auto source_address = r.address[6];
            const auto value = m.lng(source_address);
            r.address[6] += 4U;
            r.data[6] = value;
            m.logic(value, 32U);
            (void)m.word(0x488U);
            break;
        }
        case 0x486U: { // 7a00 moveq #$0, d5
            next = 0x488U;
            r.data[5] = 0x0U;
            m.logic(r.data[5], 32U);
            (void)m.word(0x48aU);
            break;
        }
        case 0x488U: { // 3005 move.w d5, d0
            next = 0x48aU;
            const auto value = r.data[5];
            m.dw(0U, value);
            m.logic(value, 16U);
            (void)m.word(0x48cU);
            break;
        }
        case 0x48aU: { // 204d movea.l a5, a0
            next = 0x48cU;
            const auto value = r.address[5];
            r.address[0] = value;
            (void)m.word(0x48eU);
            break;
        }
        case 0x48cU: { // 3606 move.w d6, d3
            next = 0x48eU;
            const auto value = r.data[6];
            m.dw(3U, value);
            m.logic(value, 16U);
            (void)m.word(0x490U);
            break;
        }
        case 0x48eU: { // 30c0 move.w d0, (a0)+
            next = 0x490U;
            const auto value = r.data[0];
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            r.address[0] += 2U;
            m.logic(value, 16U);
            (void)m.word(0x492U);
            break;
        }
        case 0x490U: { // d044 add.w d4, d0
            next = 0x492U;
            const auto source_value = r.data[4];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x494U);
            break;
        }
        case 0x492U: { // 51cbfffa dbra d3, $48e
            next = 0x496U;
            (void)m.word(0x48eU);
            m.dw(3U, r.data[3] - 1U);
            if ((r.data[3] & 0xffffU) != 0xffffU) next = 0x48eU;
            if (next == 0x496U) { (void)m.word(0x496U); }
            (void)m.word(next + 2U);
            break;
        }
        case 0x496U: { // 3005 move.w d5, d0
            next = 0x498U;
            const auto value = r.data[5];
            m.dw(0U, value);
            m.logic(value, 16U);
            (void)m.word(0x49aU);
            break;
        }
        case 0x498U: { // 204d movea.l a5, a0
            next = 0x49aU;
            const auto value = r.address[5];
            r.address[0] = value;
            (void)m.word(0x49cU);
            break;
        }
        case 0x49aU: { // 3606 move.w d6, d3
            next = 0x49cU;
            const auto value = r.data[6];
            m.dw(3U, value);
            m.logic(value, 16U);
            (void)m.word(0x49eU);
            break;
        }
        case 0x49cU: { // b058 cmp.w (a0)+, d0
            next = 0x49eU;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            (void)m.word(0x4a0U);
            break;
        }
        case 0x49eU: { // 670a beq.b $4aa
            next = 0x4a0U;
            if ((r.status & 4U) != 0U) { next = 0x4aaU; transfer_kind = 1U; }
            if (transfer_kind == 1U) { (void)m.word(next); }
            (void)m.word(next + 2U);
            break;
        }
        case 0x4aaU: { // d044 add.w d4, d0
            next = 0x4acU;
            const auto source_value = r.data[4];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x4aeU);
            break;
        }
        case 0x4acU: { // 51cbffee dbra d3, $49c
            next = 0x4b0U;
            (void)m.word(0x49cU);
            m.dw(3U, r.data[3] - 1U);
            if ((r.data[3] & 0xffffU) != 0xffffU) next = 0x49cU;
            if (next == 0x4b0U) { (void)m.word(0x4b0U); }
            (void)m.word(next + 2U);
            break;
        }
        case 0x4b0U: { // 51cfffd0 dbra d7, $482
            next = 0x4b4U;
            (void)m.word(0x482U);
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x482U;
            if (next == 0x4b4U) { (void)m.word(0x4b4U); }
            (void)m.word(next + 2U);
            break;
        }
        case 0x4b4U: { // 307c000c movea.w #$c, a0
            next = 0x4b8U;
            (void)m.word(0x4b8U);
            const auto value = 0xcU;
            r.address[0] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            (void)m.word(0x4baU);
            break;
        }
        case 0x4b8U: { // 61001874 bsr.w $1d2e
            next = 0x4bcU;
            const auto result = m.call_prefetched(c, 30U, 0x4b8U, 0x1d2eU, 0x4bcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x4bcU: { // 7e00 moveq #$0, d7
            next = 0x4beU;
            r.data[7] = 0x0U;
            m.logic(r.data[7], 32U);
            (void)m.word(0x4c0U);
            break;
        }
        case 0x4beU: { // 307c0008 movea.w #$8, a0
            next = 0x4c2U;
            (void)m.word(0x4c2U);
            const auto value = 0x8U;
            r.address[0] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            (void)m.word(0x4c4U);
            break;
        }
        case 0x4c2U: { // 07080000 movep.w $0(a0), d3
            next = 0x4c6U;
            (void)m.word(0x4c6U);
            const auto memory_address = r.address[0] + 0x0U;
            std::uint32_t value = 0U;
            value = (value << 8U) | m.byte(memory_address + 0U);
            value = (value << 8U) | m.byte(memory_address + 2U);
            m.dw(3U, value);
            (void)m.word(0x4c8U);
            break;
        }
        case 0x4c6U: { // b043 cmp.w d3, d0
            next = 0x4c8U;
            const auto source_value = r.data[3];
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            (void)m.word(0x4caU);
            break;
        }
        case 0x4c8U: { // 6704 beq.b $4ce
            next = 0x4caU;
            if ((r.status & 4U) != 0U) { next = 0x4ceU; transfer_kind = 1U; }
            if (transfer_kind == 1U) { (void)m.word(next); }
            (void)m.word(next + 2U);
            break;
        }
        case 0x4ceU: { // 307c000d movea.w #$d, a0
            next = 0x4d2U;
            (void)m.word(0x4d2U);
            const auto value = 0xdU;
            r.address[0] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            (void)m.word(0x4d4U);
            break;
        }
        case 0x4d2U: { // 6100185a bsr.w $1d2e
            next = 0x4d6U;
            const auto result = m.call_prefetched(c, 30U, 0x4d2U, 0x1d2eU, 0x4d6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x4d6U: { // 323c00ff move.w #$ff, d1
            next = 0x4daU;
            (void)m.word(0x4daU);
            m.dw(1U, 0xffU);
            m.logic(r.data[1], 16U);
            (void)m.word(0x4dcU);
            break;
        }
        case 0x4daU: { // 307c0009 movea.w #$9, a0
            next = 0x4deU;
            (void)m.word(0x4deU);
            const auto value = 0x9U;
            r.address[0] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            (void)m.word(0x4e0U);
            break;
        }
        case 0x4deU: { // 07080000 movep.w $0(a0), d3
            next = 0x4e2U;
            (void)m.word(0x4e2U);
            const auto memory_address = r.address[0] + 0x0U;
            std::uint32_t value = 0U;
            value = (value << 8U) | m.byte(memory_address + 0U);
            value = (value << 8U) | m.byte(memory_address + 2U);
            m.dw(3U, value);
            (void)m.word(0x4e4U);
            break;
        }
        case 0x4e2U: { // b043 cmp.w d3, d0
            next = 0x4e4U;
            const auto source_value = r.data[3];
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            (void)m.word(0x4e6U);
            break;
        }
        case 0x4e4U: { // 6704 beq.b $4ea
            next = 0x4e6U;
            if ((r.status & 4U) != 0U) { next = 0x4eaU; transfer_kind = 1U; }
            if (transfer_kind == 1U) { (void)m.word(next); }
            (void)m.word(next + 2U);
            break;
        }
        case 0x4eaU: { // 3207 move.w d7, d1
            next = 0x4ecU;
            const auto value = r.data[7];
            m.dw(1U, value);
            m.logic(value, 16U);
            (void)m.word(0x4eeU);
            break;
        }
        case 0x4ecU: { // 6600185c bne.w $1d4a
            next = 0x4f0U;
            if ((r.status & 4U) == 0U) { next = 0x1d4aU; transfer_kind = 1U; }
            (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x4f0U: { // 61002f82 bsr.w $3474
            next = 0x4f4U;
            const auto result = m.call_prefetched(c, 39U, 0x4f0U, 0x3474U, 0x4f4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x4f4U: { // 61002f6e bsr.w $3464
            next = 0x4f8U;
            const auto result = m.call_prefetched(c, 38U, 0x4f4U, 0x3464U, 0x4f8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x4f8U: { // 61001bd2 bsr.w $20cc
            next = 0x4fcU;
            const auto result = m.call(c, 31U, 0x4f8U, 0x20ccU, 0x4fcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x4fcU: { // 0839000200800009 btst.b #$2, $800009.l
            next = 0x504U;
            const auto destination_address = 0x800009U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x504U: { // 6604 bne.b $50a
            next = 0x506U;
            if ((r.status & 4U) == 0U) { next = 0x50aU; transfer_kind = 1U; }
            break;
        }
        case 0x506U: { // 60001ebc bra.w $23c4
            next = 0x50aU;
            if (true) { next = 0x23c4U; transfer_kind = 1U; }
            break;
        }
        case 0x50aU: { // 103900b00005 move.b $b00005.l, d0
            next = 0x510U;
            const auto source_address = 0xb00005U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x510U: { // 4600 not.b d0
            next = 0x512U;
            const auto old = r.data[0];
            const auto value = ~old;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x512U: { // 13c000b00005 move.b d0, $b00005.l
            next = 0x518U;
            const auto value = r.data[0];
            const auto destination_address = 0xb00005U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x518U: { // 4aa7 tst.l -(a7)
            next = 0x51aU;
            r.address[7] -= 4U;
            const auto destination_address = r.address[7];
            const auto value = m.lng(destination_address);
            m.logic(value, 32U);
            break;
        }
        case 0x51aU: { // 4a9f tst.l (a7)+
            next = 0x51cU;
            const auto destination_address = r.address[7];
            const auto value = m.lng(destination_address);
            r.address[7] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x51cU: { // b03900b00005 cmp.b $b00005.l, d0
            next = 0x522U;
            const auto source_address = 0xb00005U;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x522U: { // 6700007c beq.w $5a0
            next = 0x526U;
            if ((r.status & 4U) != 0U) { next = 0x5a0U; transfer_kind = 1U; }
            break;
        }
        case 0x5a0U: { // 103900b00005 move.b $b00005.l, d0
            next = 0x5a6U;
            const auto source_address = 0xb00005U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x5a6U: { // 4600 not.b d0
            next = 0x5a8U;
            const auto old = r.data[0];
            const auto value = ~old;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x5a8U: { // 13c000b00005 move.b d0, $b00005.l
            next = 0x5aeU;
            const auto value = r.data[0];
            const auto destination_address = 0xb00005U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x5aeU: { // 4aa7 tst.l -(a7)
            next = 0x5b0U;
            r.address[7] -= 4U;
            const auto destination_address = r.address[7];
            const auto value = m.lng(destination_address);
            m.logic(value, 32U);
            break;
        }
        case 0x5b0U: { // 4a9f tst.l (a7)+
            next = 0x5b2U;
            const auto destination_address = r.address[7];
            const auto value = m.lng(destination_address);
            r.address[7] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x5b2U: { // b03900b00005 cmp.b $b00005.l, d0
            next = 0x5b8U;
            const auto source_address = 0xb00005U;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x5b8U: { // 660c bne.b $5c6
            next = 0x5baU;
            if ((r.status & 4U) == 0U) { next = 0x5c6U; transfer_kind = 1U; }
            break;
        }
        case 0x5baU: { // 0839000300800009 btst.b #$3, $800009.l
            next = 0x5c2U;
            const auto destination_address = 0x800009U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x5c2U: { // 67000d4c beq.w $1310
            next = 0x5c6U;
            if ((r.status & 4U) != 0U) { next = 0x1310U; transfer_kind = 1U; }
            break;
        }
        case 0x5c6U: { // 13fc00010040401b move.b #$1, $40401b.l
            next = 0x5ceU;
            const auto value = 0x1U;
            const auto destination_address = 0x40401bU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x5ceU: { // 4e722300 stop #$2300
            next = 0x5d2U;
            r.status = static_cast<std::uint16_t>(0x2300U);
            r.program_counter = 0x5d2U;
            for (;;) {
                const auto pending = c.host->consume_pending_interrupt(0U, 0xffU, 0x5d2U);
                if (!pending.asserted || pending.level <= ((r.status >> 8U) & 7U)) continue;
                if (pending.level != 4U) return {TranslationStatus::contract_violation, 0U, r.program_counter};
                if (c.host->resumes_interrupts_inline()) {
                    const auto result = service_cpu_a_autovector(c, pending.level, 0x5d2U, 0x5d2U);
                    if (result.status != TranslationStatus::complete) return result;
                    next = r.program_counter;
                    break;
                }
                const auto saved_status = r.status;
                r.address[7] -= 4U; m.word(r.address[7] + 2U, 0x5d2U);
                r.address[7] -= 2U; m.word(r.address[7], saved_status); m.word(r.address[7] + 2U, 0U);
                r.status = static_cast<std::uint16_t>((saved_status & 0x38ffU) | 0x2400U);
                const auto target = m.lng(0x70U);
                (void)m.word(target); (void)m.word(target + 2U);
                r.program_counter = target;
                if (target != 0x80048U) return {TranslationStatus::contract_violation, 0U, target};
                const auto marker = c.host->call_function(47U, 0U, 0xffU, 0U, 0x5d2U, target, c);
                if (marker.status != TranslationStatus::complete) return marker;
                const auto result = cpu_a_irq4_vector_trampoline(c);
                if (result.status != TranslationStatus::complete) return result;
                if (r.program_counter != 0x5d2U) return {TranslationStatus::contract_violation, 0U, r.program_counter};
                next = r.program_counter;
                break;
            }
            break;
        }
        case 0x5d2U: { // 610005c4 bsr.w $b98
            next = 0x5d6U;
            const auto result = m.call(c, 5U, 0x5d2U, 0xb98U, 0x5d6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x5d6U: { // 610005fc bsr.w $bd4
            next = 0x5daU;
            const auto result = m.call(c, 8U, 0x5d6U, 0xbd4U, 0x5daU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x5daU: { // 610005d0 bsr.w $bac
            next = 0x5deU;
            const auto result = m.call(c, 6U, 0x5daU, 0xbacU, 0x5deU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x5deU: { // 610005e0 bsr.w $bc0
            next = 0x5e2U;
            const auto result = m.call(c, 7U, 0x5deU, 0xbc0U, 0x5e2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x5e2U: { // 61000604 bsr.w $be8
            next = 0x5e6U;
            const auto result = m.call(c, 9U, 0x5e2U, 0xbe8U, 0x5e6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x5e6U: { // 41fa0186 lea.l $76e(pc), a0
            next = 0x5eaU;
            const auto source_address = 0x76eU;
            r.address[0] = source_address;
            break;
        }
        case 0x5eaU: { // 227c00400000 movea.l #$400000, a1
            next = 0x5f0U;
            const auto value = 0x400000U;
            r.address[1] = value;
            break;
        }
        case 0x5f0U: { // 7e02 moveq #$2, d7
            next = 0x5f2U;
            r.data[7] = 0x2U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x5f2U: { // 7c1f moveq #$1f, d6
            next = 0x5f4U;
            r.data[6] = 0x1fU;
            m.logic(r.data[6], 32U);
            break;
        }
        case 0x5f4U: { // 2448 movea.l a0, a2
            next = 0x5f6U;
            const auto value = r.address[0];
            r.address[2] = value;
            break;
        }
        case 0x5f6U: { // 3a3c0007 move.w #$7, d5
            next = 0x5faU;
            m.dw(5U, 0x7U);
            m.logic(r.data[5], 16U);
            break;
        }
        case 0x5faU: { // 22da move.l (a2)+, (a1)+
            next = 0x5fcU;
            const auto source_address = r.address[2];
            const auto value = m.lng(source_address);
            r.address[2] += 4U;
            const auto destination_address = r.address[1];
            m.lng(destination_address, value);
            r.address[1] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x5fcU: { // 51cdfffc dbra d5, $5fa
            next = 0x600U;
            m.dw(5U, r.data[5] - 1U);
            if ((r.data[5] & 0xffffU) != 0xffffU) next = 0x5faU;
            break;
        }
        case 0x600U: { // 51cefff2 dbra d6, $5f4
            next = 0x604U;
            m.dw(6U, r.data[6] - 1U);
            if ((r.data[6] & 0xffffU) != 0xffffU) next = 0x5f4U;
            break;
        }
        case 0x604U: { // 41e80020 lea.l $20(a0), a0
            next = 0x608U;
            r.address[0] = r.address[0] + 0x20U;
            break;
        }
        case 0x608U: { // 51cfffe8 dbra d7, $5f2
            next = 0x60cU;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x5f2U;
            break;
        }
        case 0x60cU: { // 41f900007380 lea.l $7380.l, a0
            next = 0x612U;
            r.address[0] = 0x7380U;
            break;
        }
        case 0x612U: { // 43f900280020 lea.l $280020.l, a1
            next = 0x618U;
            r.address[1] = 0x280020U;
            break;
        }
        case 0x618U: { // 303c0dbf move.w #$dbf, d0
            next = 0x61cU;
            m.dw(0U, 0xdbfU);
            m.logic(r.data[0], 16U);
            break;
        }
        case 0x61cU: { // 22d8 move.l (a0)+, (a1)+
            next = 0x61eU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[1];
            m.lng(destination_address, value);
            r.address[1] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x61eU: { // 51c8fffc dbra d0, $61c
            next = 0x622U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x61cU;
            break;
        }
        case 0x622U: { // 13fc00000040401b move.b #$0, $40401b.l
            next = 0x62aU;
            const auto value = 0x0U;
            const auto destination_address = 0x40401bU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x62aU: { // 4e722300 stop #$2300
            next = 0x62eU;
            r.status = static_cast<std::uint16_t>(0x2300U);
            for (;;) { if (auto event = m.interrupt(c, 0x62aU, 0x62eU)) return *event; }
            break;
        }
        case 0x62eU: { // 4278fc84 clr.w $fc84.w
            next = 0x632U;
            const auto destination_address = 0xfffffc84U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x632U: { // 70ff moveq #$ff, d0
            next = 0x634U;
            r.data[0] = 0xffffffffU;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x634U: { // 4a78fc84 tst.w $fc84.w
            next = 0x638U;
            const auto destination_address = 0xfffffc84U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x638U: { // 56c8fffa dbne d0, $634
            next = 0x63cU;
            if ((r.status & 4U) != 0U) {
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x634U;
            }
            break;
        }
        case 0x63cU: { // b07ce000 cmp.w #$e000, d0
            next = 0x640U;
            const auto source_value = 0xe000U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x640U: { // 6504 bcs.b $646
            next = 0x642U;
            if ((r.status & 1U) != 0U) { next = 0x646U; transfer_kind = 1U; }
            break;
        }
        case 0x642U: { // 46fc2600 move.w #$2600, sr
            next = 0x646U;
            r.status = static_cast<std::uint16_t>(0x2600U);
            break;
        }
        case 0x646U: { // 207c0000aa80 movea.l #$aa80, a0
            next = 0x64cU;
            const auto value = 0xaa80U;
            r.address[0] = value;
            break;
        }
        case 0x64cU: { // 227c0020441a movea.l #$20441a, a1
            next = 0x652U;
            const auto value = 0x20441aU;
            r.address[1] = value;
            break;
        }
        case 0x652U: { // 7e23 moveq #$23, d7
            next = 0x654U;
            r.data[7] = 0x23U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x654U: { // 7c0c moveq #$c, d6
            next = 0x656U;
            r.data[6] = 0xcU;
            m.logic(r.data[6], 32U);
            break;
        }
        case 0x656U: { // 3a3c0038 move.w #$38, d5
            next = 0x65aU;
            m.dw(5U, 0x38U);
            m.logic(r.data[5], 16U);
            break;
        }
        case 0x65aU: { // 7800 moveq #$0, d4
            next = 0x65cU;
            r.data[4] = 0x0U;
            m.logic(r.data[4], 32U);
            break;
        }
        case 0x65cU: { // 610000fa bsr.w $758
            next = 0x660U;
            const auto result = m.call(c, 3U, 0x65cU, 0x758U, 0x660U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x660U: { // 227c00204d8a movea.l #$204d8a, a1
            next = 0x666U;
            const auto value = 0x204d8aU;
            r.address[1] = value;
            break;
        }
        case 0x666U: { // 7e32 moveq #$32, d7
            next = 0x668U;
            r.data[7] = 0x32U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x668U: { // 7c05 moveq #$5, d6
            next = 0x66aU;
            r.data[6] = 0x5U;
            m.logic(r.data[6], 32U);
            break;
        }
        case 0x66aU: { // 3a3c001a move.w #$1a, d5
            next = 0x66eU;
            m.dw(5U, 0x1aU);
            m.logic(r.data[5], 16U);
            break;
        }
        case 0x66eU: { // 383c1000 move.w #$1000, d4
            next = 0x672U;
            m.dw(4U, 0x1000U);
            m.logic(r.data[4], 16U);
            break;
        }
        case 0x672U: { // 610000e4 bsr.w $758
            next = 0x676U;
            const auto result = m.call(c, 3U, 0x672U, 0x758U, 0x676U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x676U: { // 227c0020539a movea.l #$20539a, a1
            next = 0x67cU;
            const auto value = 0x20539aU;
            r.address[1] = value;
            break;
        }
        case 0x67cU: { // 7e24 moveq #$24, d7
            next = 0x67eU;
            r.data[7] = 0x24U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x67eU: { // 7c01 moveq #$1, d6
            next = 0x680U;
            r.data[6] = 0x1U;
            m.logic(r.data[6], 32U);
            break;
        }
        case 0x680U: { // 3a3c0036 move.w #$36, d5
            next = 0x684U;
            m.dw(5U, 0x36U);
            m.logic(r.data[5], 16U);
            break;
        }
        case 0x684U: { // 383c2000 move.w #$2000, d4
            next = 0x688U;
            m.dw(4U, 0x2000U);
            m.logic(r.data[4], 16U);
            break;
        }
        case 0x688U: { // 610000ce bsr.w $758
            next = 0x68cU;
            const auto result = m.call(c, 3U, 0x688U, 0x758U, 0x68cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x68cU: { // 31fcfffffc80 move.w #$ffff, $fc80.w
            next = 0x692U;
            const auto value = 0xffffU;
            const auto destination_address = 0xfffffc80U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x692U: { // 7001 moveq #$1, d0
            next = 0x694U;
            r.data[0] = 0x1U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x694U: { // 4e48 trap #$8
            next = 0x696U;
            const auto result = m.exception(c, 40U, 0x694U, 0x696U);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x696U) return result;
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x696U: { // 307cc000 movea.w #$c000, a0
            next = 0x69aU;
            const auto value = 0xc000U;
            r.address[0] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x69aU: { // 3c7cfc00 movea.w #$fc00, a6
            next = 0x69eU;
            const auto value = 0xfc00U;
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x69eU: { // 224e movea.l a6, a1
            next = 0x6a0U;
            const auto value = r.address[6];
            r.address[1] = value;
            break;
        }
        case 0x6a0U: { // 7000 moveq #$0, d0
            next = 0x6a2U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x6a2U: { // 727f moveq #$7f, d1
            next = 0x6a4U;
            r.data[1] = 0x7fU;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x6a4U: { // 5241 addq.w #$1, d1
            next = 0x6a6U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x6a6U: { // 4e43 trap #$3
            next = 0x6a8U;
            const auto result = m.exception(c, 35U, 0x6a6U, 0x6a8U);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x6a8U) return result;
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x6a8U: { // 31fc000afc82 move.w #$a, $fc82.w
            next = 0x6aeU;
            const auto value = 0xaU;
            const auto destination_address = 0xfffffc82U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x6aeU: { // 610001a2 bsr.w $852
            next = 0x6b2U;
            const auto result = m.call(c, 4U, 0x6aeU, 0x852U, 0x6b2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x6b2U: { // 221e move.l (a6)+, d1
            next = 0x6b4U;
            const auto source_address = r.address[6];
            const auto value = m.lng(source_address);
            r.address[6] += 4U;
            r.data[1] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6b4U: { // 6712 beq.b $6c8
            next = 0x6b6U;
            if ((r.status & 4U) != 0U) { next = 0x6c8U; transfer_kind = 1U; }
            break;
        }
        case 0x6b6U: { // 201e move.l (a6)+, d0
            next = 0x6b8U;
            const auto source_address = r.address[6];
            const auto value = m.lng(source_address);
            r.address[6] += 4U;
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6b8U: { // 225e movea.l (a6)+, a1
            next = 0x6baU;
            const auto source_address = r.address[6];
            const auto value = m.lng(source_address);
            r.address[6] += 4U;
            r.address[1] = value;
            break;
        }
        case 0x6baU: { // 6100003a bsr.w $6f6
            next = 0x6beU;
            const auto result = m.call(c, 2U, 0x6baU, 0x6f6U, 0x6beU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x6beU: { // 204a movea.l a2, a0
            next = 0x6c0U;
            const auto value = r.address[2];
            r.address[0] = value;
            break;
        }
        case 0x6c0U: { // 4e4a trap #$a
            next = 0x6c2U;
            const auto result = m.exception(c, 42U, 0x6c0U, 0x6c2U);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x6c2U) return result;
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x6c2U: { // 5378fc82 subq.w #$1, $fc82.w
            next = 0x6c6U;
            const auto source_value = 0x1U;
            const auto destination_address = 0xfffffc82U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x6c6U: { // 66ea bne.b $6b2
            next = 0x6c8U;
            if ((r.status & 4U) == 0U) { next = 0x6b2U; transfer_kind = 1U; }
            break;
        }
        case 0x6c8U: { // 7000 moveq #$0, d0
            next = 0x6caU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x6caU: { // 4e48 trap #$8
            next = 0x6ccU;
            const auto result = m.exception(c, 40U, 0x6caU, 0x6ccU);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x6ccU) return result;
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x6ccU: { // 7000 moveq #$0, d0
            next = 0x6ceU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x6ceU: { // 2200 move.l d0, d1
            next = 0x6d0U;
            const auto value = r.data[0];
            r.data[1] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6d0U: { // 2400 move.l d0, d2
            next = 0x6d2U;
            const auto value = r.data[0];
            r.data[2] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6d2U: { // 2600 move.l d0, d3
            next = 0x6d4U;
            const auto value = r.data[0];
            r.data[3] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6d4U: { // 2800 move.l d0, d4
            next = 0x6d6U;
            const auto value = r.data[0];
            r.data[4] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6d6U: { // 2a00 move.l d0, d5
            next = 0x6d8U;
            const auto value = r.data[0];
            r.data[5] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6d8U: { // 2c00 move.l d0, d6
            next = 0x6daU;
            const auto value = r.data[0];
            r.data[6] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6daU: { // 2e00 move.l d0, d7
            next = 0x6dcU;
            const auto value = r.data[0];
            r.data[7] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6dcU: { // 2040 movea.l d0, a0
            next = 0x6deU;
            const auto value = r.data[0];
            r.address[0] = value;
            break;
        }
        case 0x6deU: { // 2240 movea.l d0, a1
            next = 0x6e0U;
            const auto value = r.data[0];
            r.address[1] = value;
            break;
        }
        case 0x6e0U: { // 2440 movea.l d0, a2
            next = 0x6e2U;
            const auto value = r.data[0];
            r.address[2] = value;
            break;
        }
        case 0x6e2U: { // 2640 movea.l d0, a3
            next = 0x6e4U;
            const auto value = r.data[0];
            r.address[3] = value;
            break;
        }
        case 0x6e4U: { // 2840 movea.l d0, a4
            next = 0x6e6U;
            const auto value = r.data[0];
            r.address[4] = value;
            break;
        }
        case 0x6e6U: { // 2a40 movea.l d0, a5
            next = 0x6e8U;
            const auto value = r.data[0];
            r.address[5] = value;
            break;
        }
        case 0x6e8U: { // 2c40 movea.l d0, a6
            next = 0x6eaU;
            const auto value = r.data[0];
            r.address[6] = value;
            break;
        }
        case 0x6eaU: { // 2e40 movea.l d0, a7
            next = 0x6ecU;
            const auto value = r.data[0];
            r.address[7] = value;
            break;
        }
        case 0x6ecU: { // 46fc2700 move.w #$2700, sr
            next = 0x6f0U;
            r.status = static_cast<std::uint16_t>(0x2700U);
            break;
        }
        case 0x6f0U: { // 4ef9000800c0 jmp $800c0.l
            next = 0x6f6U;
            next = 0x800c0U;
            transfer_kind = 1U;
            break;
        }
        case 0x23c4U: { // 3f3c0000 move.w #$0, -(a7)
            next = 0x23c8U;
            const auto value = 0x0U;
            r.address[7] -= 2U;
            const auto destination_address = r.address[7];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x23c8U: { // 13fc00040080001d move.b #$4, $80001d.l
            next = 0x23d0U;
            const auto value = 0x4U;
            const auto destination_address = 0x80001dU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x23d0U: { // 13fc000800a00005 move.b #$8, $a00005.l
            next = 0x23d8U;
            const auto value = 0x8U;
            const auto destination_address = 0xa00005U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x23d8U: { // 6100fcf2 bsr.w $20cc
            next = 0x23dcU;
            const auto result = m.call(c, 31U, 0x23d8U, 0x20ccU, 0x23dcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23dcU: { // 61001086 bsr.w $3464
            next = 0x23e0U;
            const auto result = m.call(c, 38U, 0x23dcU, 0x3464U, 0x23e0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23e0U: { // 33fcffc600240000 move.w #$ffc6, $240000.l
            next = 0x23e8U;
            const auto value = 0xffc6U;
            const auto destination_address = 0x240000U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x23e8U: { // 33fcfff000260000 move.w #$fff0, $260000.l
            next = 0x23f0U;
            const auto value = 0xfff0U;
            const auto destination_address = 0x260000U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x23f0U: { // 13fc00010040401b move.b #$1, $40401b.l
            next = 0x23f8U;
            const auto value = 0x1U;
            const auto destination_address = 0x40401bU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x23f8U: { // 4e722300 stop #$2300
            next = 0x23fcU;
            r.status = static_cast<std::uint16_t>(0x2300U);
            for (;;) { if (auto event = m.interrupt(c, 0x23f8U, 0x23fcU)) return *event; }
            break;
        }
        case 0x23fcU: { // 41faffba lea.l $23b8(pc), a0
            next = 0x2400U;
            const auto source_address = 0x23b8U;
            r.address[0] = source_address;
            break;
        }
        case 0x2400U: { // 227c00404001 movea.l #$404001, a1
            next = 0x2406U;
            const auto value = 0x404001U;
            r.address[1] = value;
            break;
        }
        case 0x2406U: { // 2018 move.l (a0)+, d0
            next = 0x2408U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x2408U: { // 01c90000 movep.l d0, $0(a1)
            next = 0x240cU;
            const auto memory_address = r.address[1] + 0x0U;
            m.byte(memory_address + 0U, r.data[0] >> 24U);
            m.byte(memory_address + 2U, r.data[0] >> 16U);
            m.byte(memory_address + 4U, r.data[0] >> 8U);
            m.byte(memory_address + 6U, r.data[0] >> 0U);
            break;
        }
        case 0x240cU: { // 2018 move.l (a0)+, d0
            next = 0x240eU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x240eU: { // 01c90008 movep.l d0, $8(a1)
            next = 0x2412U;
            const auto memory_address = r.address[1] + 0x8U;
            m.byte(memory_address + 0U, r.data[0] >> 24U);
            m.byte(memory_address + 2U, r.data[0] >> 16U);
            m.byte(memory_address + 4U, r.data[0] >> 8U);
            m.byte(memory_address + 6U, r.data[0] >> 0U);
            break;
        }
        case 0x2412U: { // 2018 move.l (a0)+, d0
            next = 0x2414U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x2414U: { // 01c90010 movep.l d0, $10(a1)
            next = 0x2418U;
            const auto memory_address = r.address[1] + 0x10U;
            m.byte(memory_address + 0U, r.data[0] >> 24U);
            m.byte(memory_address + 2U, r.data[0] >> 16U);
            m.byte(memory_address + 4U, r.data[0] >> 8U);
            m.byte(memory_address + 6U, r.data[0] >> 0U);
            break;
        }
        case 0x2418U: { // 61000020 bsr.w $243a
            next = 0x241cU;
            const auto result = m.call(c, 37U, 0x2418U, 0x243aU, 0x241cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x241cU: { // 3e3c0100 move.w #$100, d7
            next = 0x2420U;
            m.dw(7U, 0x100U);
            m.logic(r.data[7], 16U);
            break;
        }
        case 0x2420U: { // 43fa167a lea.l $3a9c(pc), a1
            next = 0x2424U;
            const auto source_address = 0x3a9cU;
            r.address[1] = source_address;
            break;
        }
        case 0x2424U: { // 61001468 bsr.w $388e
            next = 0x2428U;
            const auto result = m.call(c, 45U, 0x2424U, 0x388eU, 0x2428U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0x800c0U) return c.host->call_function(54U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0x408U:
        case 0x40cU:
        case 0x410U:
        case 0x418U:
        case 0x420U:
        case 0x428U:
        case 0x430U:
        case 0x438U:
        case 0x440U:
        case 0x448U:
        case 0x450U:
        case 0x458U:
        case 0x45cU:
        case 0x460U:
        case 0x462U:
        case 0x46aU:
        case 0x472U:
        case 0x47aU:
        case 0x482U:
        case 0x484U:
        case 0x486U:
        case 0x488U:
        case 0x48aU:
        case 0x48cU:
        case 0x48eU:
        case 0x490U:
        case 0x492U:
        case 0x496U:
        case 0x498U:
        case 0x49aU:
        case 0x49cU:
        case 0x49eU:
        case 0x4aaU:
        case 0x4acU:
        case 0x4b0U:
        case 0x4b4U:
        case 0x4b8U:
        case 0x4bcU:
        case 0x4beU:
        case 0x4c2U:
        case 0x4c6U:
        case 0x4c8U:
        case 0x4ceU:
        case 0x4d2U:
        case 0x4d6U:
        case 0x4daU:
        case 0x4deU:
        case 0x4e2U:
        case 0x4e4U:
        case 0x4eaU:
        case 0x4ecU:
        case 0x4f0U:
        case 0x4f4U:
        case 0x4f8U:
        case 0x4fcU:
        case 0x504U:
        case 0x506U:
        case 0x50aU:
        case 0x510U:
        case 0x512U:
        case 0x518U:
        case 0x51aU:
        case 0x51cU:
        case 0x522U:
        case 0x5a0U:
        case 0x5a6U:
        case 0x5a8U:
        case 0x5aeU:
        case 0x5b0U:
        case 0x5b2U:
        case 0x5b8U:
        case 0x5baU:
        case 0x5c2U:
        case 0x5c6U:
        case 0x5ceU:
        case 0x5d2U:
        case 0x5d6U:
        case 0x5daU:
        case 0x5deU:
        case 0x5e2U:
        case 0x5e6U:
        case 0x5eaU:
        case 0x5f0U:
        case 0x5f2U:
        case 0x5f4U:
        case 0x5f6U:
        case 0x5faU:
        case 0x5fcU:
        case 0x600U:
        case 0x604U:
        case 0x608U:
        case 0x60cU:
        case 0x612U:
        case 0x618U:
        case 0x61cU:
        case 0x61eU:
        case 0x622U:
        case 0x62aU:
        case 0x62eU:
        case 0x632U:
        case 0x634U:
        case 0x638U:
        case 0x63cU:
        case 0x640U:
        case 0x642U:
        case 0x646U:
        case 0x64cU:
        case 0x652U:
        case 0x654U:
        case 0x656U:
        case 0x65aU:
        case 0x65cU:
        case 0x660U:
        case 0x666U:
        case 0x668U:
        case 0x66aU:
        case 0x66eU:
        case 0x672U:
        case 0x676U:
        case 0x67cU:
        case 0x67eU:
        case 0x680U:
        case 0x684U:
        case 0x688U:
        case 0x68cU:
        case 0x692U:
        case 0x694U:
        case 0x696U:
        case 0x69aU:
        case 0x69eU:
        case 0x6a0U:
        case 0x6a2U:
        case 0x6a4U:
        case 0x6a6U:
        case 0x6a8U:
        case 0x6aeU:
        case 0x6b2U:
        case 0x6b4U:
        case 0x6b6U:
        case 0x6b8U:
        case 0x6baU:
        case 0x6beU:
        case 0x6c0U:
        case 0x6c2U:
        case 0x6c6U:
        case 0x6c8U:
        case 0x6caU:
        case 0x6ccU:
        case 0x6ceU:
        case 0x6d0U:
        case 0x6d2U:
        case 0x6d4U:
        case 0x6d6U:
        case 0x6d8U:
        case 0x6daU:
        case 0x6dcU:
        case 0x6deU:
        case 0x6e0U:
        case 0x6e2U:
        case 0x6e4U:
        case 0x6e6U:
        case 0x6e8U:
        case 0x6eaU:
        case 0x6ecU:
        case 0x6f0U:
        case 0x23c4U:
        case 0x23c8U:
        case 0x23d0U:
        case 0x23d8U:
        case 0x23dcU:
        case 0x23e0U:
        case 0x23e8U:
        case 0x23f0U:
        case 0x23f8U:
        case 0x23fcU:
        case 0x2400U:
        case 0x2406U:
        case 0x2408U:
        case 0x240cU:
        case 0x240eU:
        case 0x2412U:
        case 0x2414U:
        case 0x2418U:
        case 0x241cU:
        case 0x2420U:
        case 0x2424U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
