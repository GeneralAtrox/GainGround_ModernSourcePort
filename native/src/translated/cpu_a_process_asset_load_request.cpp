// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult cpu_a_process_asset_load_request(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 0U, 255U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x80414U: { // 30388006 move.w $8006.w, d0
            next = 0x80418U;
            const auto source_address = 0xffff8006U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x80418U: { // 67000106 beq.w $80520
            next = 0x8041cU;
            if ((r.status & 4U) != 0U) { next = 0x80520U; transfer_kind = 1U; }
            break;
        }
        case 0x8041cU: { // 31c08008 move.w d0, $8008.w
            next = 0x80420U;
            const auto value = r.data[0];
            const auto destination_address = 0xffff8008U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x80420U: { // 6100017a bsr.w $8059c
            next = 0x80424U;
            const auto result = m.call(c, 66U, 0x80420U, 0x8059cU, 0x80424U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x80424U: { // 7000 moveq #$0, d0
            next = 0x80426U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x80426U: { // 30388006 move.w $8006.w, d0
            next = 0x8042aU;
            const auto source_address = 0xffff8006U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x8042aU: { // 5340 subq.w #$1, d0
            next = 0x8042cU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x8042cU: { // d040 add.w d0, d0
            next = 0x8042eU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x8042eU: { // d040 add.w d0, d0
            next = 0x80430U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x80430U: { // 49f900084f6a lea.l $84f6a.l, a4
            next = 0x80436U;
            r.address[4] = 0x84f6aU;
            break;
        }
        case 0x80436U: { // 28740800 movea.l (a4, d0.l), a4
            next = 0x8043aU;
            const auto source_address = r.address[4] + r.data[0];
            const auto value = m.lng(source_address);
            r.address[4] = value;
            break;
        }
        case 0x8043aU: { // 4a54 tst.w (a4)
            next = 0x8043cU;
            const auto destination_address = r.address[4];
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x8043cU: { // 6b08 bmi.b $80446
            next = 0x8043eU;
            if ((r.status & 8U) != 0U) { next = 0x80446U; transfer_kind = 1U; }
            break;
        }
        case 0x8043eU: { // 7600 moveq #$0, d3
            next = 0x80440U;
            r.data[3] = 0x0U;
            m.logic(r.data[3], 32U);
            break;
        }
        case 0x80440U: { // 610000e0 bsr.w $80522
            next = 0x80444U;
            const auto result = m.call(c, 64U, 0x80440U, 0x80522U, 0x80444U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x80444U: { // 60f4 bra.b $8043a
            next = 0x80446U;
            if (true) { next = 0x8043aU; transfer_kind = 1U; }
            break;
        }
        case 0x80446U: { // 7000 moveq #$0, d0
            next = 0x80448U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x80448U: { // 30388006 move.w $8006.w, d0
            next = 0x8044cU;
            const auto source_address = 0xffff8006U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x8044cU: { // 5340 subq.w #$1, d0
            next = 0x8044eU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x8044eU: { // d040 add.w d0, d0
            next = 0x80450U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x80450U: { // d040 add.w d0, d0
            next = 0x80452U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x80452U: { // 49f90008ebf2 lea.l $8ebf2.l, a4
            next = 0x80458U;
            r.address[4] = 0x8ebf2U;
            break;
        }
        case 0x80458U: { // 28740800 movea.l (a4, d0.l), a4
            next = 0x8045cU;
            const auto source_address = r.address[4] + r.data[0];
            const auto value = m.lng(source_address);
            r.address[4] = value;
            break;
        }
        case 0x8045cU: { // 361c move.w (a4)+, d3
            next = 0x8045eU;
            const auto source_address = r.address[4];
            const auto value = m.word(source_address);
            r.address[4] += 2U;
            m.dw(3U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x8045eU: { // 6b06 bmi.b $80466
            next = 0x80460U;
            if ((r.status & 8U) != 0U) { next = 0x80466U; transfer_kind = 1U; }
            break;
        }
        case 0x80460U: { // 610000c0 bsr.w $80522
            next = 0x80464U;
            const auto result = m.call(c, 64U, 0x80460U, 0x80522U, 0x80464U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x80464U: { // 60f6 bra.b $8045c
            next = 0x80466U;
            if (true) { next = 0x8045cU; transfer_kind = 1U; }
            break;
        }
        case 0x80466U: { // 61000162 bsr.w $805ca
            next = 0x8046aU;
            const auto result = m.call(c, 69U, 0x80466U, 0x805caU, 0x8046aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x8046aU: { // 61000138 bsr.w $805a4
            next = 0x8046eU;
            const auto result = m.call(c, 67U, 0x8046aU, 0x805a4U, 0x8046eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x8046eU: { // 30388006 move.w $8006.w, d0
            next = 0x80472U;
            const auto source_address = 0xffff8006U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x80472U: { // 0440000a subi.w #$a, d0
            next = 0x80476U;
            const auto source_value = 0xaU;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x80476U: { // 7e00 moveq #$0, d7
            next = 0x80478U;
            r.data[7] = 0x0U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x80478U: { // 4a40 tst.w d0
            next = 0x8047aU;
            const auto value = r.data[0];
            m.logic(value, 16U);
            break;
        }
        case 0x8047aU: { // 6f00000a ble.w $80486
            next = 0x8047eU;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x80486U; transfer_kind = 1U; }
            break;
        }
        case 0x8047eU: { // 0440000a subi.w #$a, d0
            next = 0x80482U;
            const auto source_value = 0xaU;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x80482U: { // 5847 addq.w #$4, d7
            next = 0x80484U;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[7];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(7U, value);
            break;
        }
        case 0x80484U: { // 60f2 bra.b $80478
            next = 0x80486U;
            if (true) { next = 0x80478U; transfer_kind = 1U; }
            break;
        }
        case 0x80486U: { // 7000 moveq #$0, d0
            next = 0x80488U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x80488U: { // 30388006 move.w $8006.w, d0
            next = 0x8048cU;
            const auto source_address = 0xffff8006U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x8048cU: { // 5340 subq.w #$1, d0
            next = 0x8048eU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x8048eU: { // e948 lsl.w #$4, d0
            next = 0x80490U;
            m.shift_word(0U, 4U, true, false);
            break;
        }
        case 0x80490U: { // 49f900091efa lea.l $91efa.l, a4
            next = 0x80496U;
            r.address[4] = 0x91efaU;
            break;
        }
        case 0x80496U: { // d9c0 adda.l d0, a4
            next = 0x80498U;
            const auto source_value = r.data[0];
            r.address[4] += source_value;
            break;
        }
        case 0x80498U: { // 43fa0ad0 lea.l $80f6a(pc), a1
            next = 0x8049cU;
            const auto source_address = 0x80f6aU;
            r.address[1] = source_address;
            break;
        }
        case 0x8049cU: { // 7603 moveq #$3, d3
            next = 0x8049eU;
            r.data[3] = 0x3U;
            m.logic(r.data[3], 32U);
            break;
        }
        case 0x8049eU: { // 265c movea.l (a4)+, a3
            next = 0x804a0U;
            const auto source_address = r.address[4];
            const auto value = m.lng(source_address);
            r.address[4] += 4U;
            r.address[3] = value;
            break;
        }
        case 0x804a0U: { // 41f9ffff97e2 lea.l $ffff97e2.l, a0
            next = 0x804a6U;
            r.address[0] = 0xffff97e2U;
            break;
        }
        case 0x804a6U: { // d0d9 adda.w (a1)+, a0
            next = 0x804a8U;
            const auto source_address = r.address[1];
            const auto source_value = m.word(source_address);
            r.address[1] += 2U;
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x804a8U: { // 610000ac bsr.w $80556
            next = 0x804acU;
            const auto result = m.call(c, 65U, 0x804a8U, 0x80556U, 0x804acU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x804acU: { // 51cbfff0 dbra d3, $8049e
            next = 0x804b0U;
            m.dw(3U, r.data[3] - 1U);
            if ((r.data[3] & 0xffffU) != 0xffffU) next = 0x8049eU;
            break;
        }
        case 0x804b0U: { // 6100013c bsr.w $805ee
            next = 0x804b4U;
            const auto result = m.call(c, 70U, 0x804b0U, 0x805eeU, 0x804b4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x804b4U: { // 61000102 bsr.w $805b8
            next = 0x804b8U;
            const auto result = m.call(c, 68U, 0x804b4U, 0x805b8U, 0x804b8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x804b8U: { // 7000 moveq #$0, d0
            next = 0x804baU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x804baU: { // 30388006 move.w $8006.w, d0
            next = 0x804beU;
            const auto source_address = 0xffff8006U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x804beU: { // 5340 subq.w #$1, d0
            next = 0x804c0U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x804c0U: { // d040 add.w d0, d0
            next = 0x804c2U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x804c2U: { // d040 add.w d0, d0
            next = 0x804c4U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x804c4U: { // 43f9000843f8 lea.l $843f8.l, a1
            next = 0x804caU;
            r.address[1] = 0x843f8U;
            break;
        }
        case 0x804caU: { // 22710000 movea.l (a1, d0.w), a1
            next = 0x804ceU;
            const auto source_address = r.address[1] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.lng(source_address);
            r.address[1] = value;
            break;
        }
        case 0x804ceU: { // 0c11ffff cmpi.b #$ff, (a1)
            next = 0x804d2U;
            const auto source_value = 0xffU;
            const auto destination_address = r.address[1];
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x804d2U: { // 6700003a beq.w $8050e
            next = 0x804d6U;
            if ((r.status & 4U) != 0U) { next = 0x8050eU; transfer_kind = 1U; }
            break;
        }
        case 0x804d6U: { // 41f8af22 lea.l $af22.w, a0
            next = 0x804daU;
            r.address[0] = 0xffffaf22U;
            break;
        }
        case 0x804daU: { // 1019 move.b (a1)+, d0
            next = 0x804dcU;
            const auto source_address = r.address[1];
            const auto value = m.byte(source_address);
            r.address[1] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x804dcU: { // e148 lsl.w #$8, d0
            next = 0x804deU;
            m.shift_word(0U, 8U, true, false);
            break;
        }
        case 0x804deU: { // 1019 move.b (a1)+, d0
            next = 0x804e0U;
            const auto source_address = r.address[1];
            const auto value = m.byte(source_address);
            r.address[1] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x804e0U: { // d0c0 adda.w d0, a0
            next = 0x804e2U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x804e2U: { // 7200 moveq #$0, d1
            next = 0x804e4U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x804e4U: { // 1219 move.b (a1)+, d1
            next = 0x804e6U;
            const auto source_address = r.address[1];
            const auto value = m.byte(source_address);
            r.address[1] += 1U;
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x804e6U: { // 7400 moveq #$0, d2
            next = 0x804e8U;
            r.data[2] = 0x0U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0x804e8U: { // 1411 move.b (a1), d2
            next = 0x804eaU;
            const auto source_address = r.address[1];
            const auto value = m.byte(source_address);
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x804eaU: { // 4842 swap d2
            next = 0x804ecU;
            const auto value = (r.data[2] << 16U) | (r.data[2] >> 16U);
            r.data[2] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x804ecU: { // 1419 move.b (a1)+, d2
            next = 0x804eeU;
            const auto source_address = r.address[1];
            const auto value = m.byte(source_address);
            r.address[1] += 1U;
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x804eeU: { // 1019 move.b (a1)+, d0
            next = 0x804f0U;
            const auto source_address = r.address[1];
            const auto value = m.byte(source_address);
            r.address[1] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x804f0U: { // 10c0 move.b d0, (a0)+
            next = 0x804f2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[0];
            m.byte(destination_address, value);
            r.address[0] += 1U;
            m.logic(value, 8U);
            break;
        }
        case 0x804f2U: { // 51cafffc dbra d2, $804f0
            next = 0x804f6U;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0x804f0U;
            break;
        }
        case 0x804f6U: { // 4842 swap d2
            next = 0x804f8U;
            const auto value = (r.data[2] << 16U) | (r.data[2] >> 16U);
            r.data[2] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x804f8U: { // 3602 move.w d2, d3
            next = 0x804faU;
            const auto value = r.data[2];
            m.dw(3U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x804faU: { // 4842 swap d2
            next = 0x804fcU;
            const auto value = (r.data[2] << 16U) | (r.data[2] >> 16U);
            r.data[2] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x804fcU: { // 3403 move.w d3, d2
            next = 0x804feU;
            const auto value = r.data[3];
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x804feU: { // 04430040 subi.w #$40, d3
            next = 0x80502U;
            const auto source_value = 0x40U;
            const auto destination_value = r.data[3];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(3U, value);
            break;
        }
        case 0x80502U: { // 4443 neg.w d3
            next = 0x80504U;
            const auto old = r.data[3];
            const auto value = m.sub(0U, old, 16U);
            m.dw(3U, value);
            break;
        }
        case 0x80504U: { // 5343 subq.w #$1, d3
            next = 0x80506U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[3];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(3U, value);
            break;
        }
        case 0x80506U: { // d0c3 adda.w d3, a0
            next = 0x80508U;
            const auto source_value = r.data[3];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x80508U: { // 51c9ffe6 dbra d1, $804f0
            next = 0x8050cU;
            m.dw(1U, r.data[1] - 1U);
            if ((r.data[1] & 0xffffU) != 0xffffU) next = 0x804f0U;
            break;
        }
        case 0x8050cU: { // 60c0 bra.b $804ce
            next = 0x8050eU;
            if (true) { next = 0x804ceU; transfer_kind = 1U; }
            break;
        }
        case 0x8050eU: { // 30388006 move.w $8006.w, d0
            next = 0x80512U;
            const auto source_address = 0xffff8006U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x80512U: { // 670c beq.b $80520
            next = 0x80514U;
            if ((r.status & 4U) != 0U) { next = 0x80520U; transfer_kind = 1U; }
            break;
        }
        case 0x80514U: { // b0788008 cmp.w $8008.w, d0
            next = 0x80518U;
            const auto source_address = 0xffff8008U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x80518U: { // 6600fefa bne.w $80414
            next = 0x8051cU;
            if ((r.status & 4U) == 0U) { next = 0x80414U; transfer_kind = 1U; }
            break;
        }
        case 0x8051cU: { // 42788006 clr.w $8006.w
            next = 0x80520U;
            const auto destination_address = 0xffff8006U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x80520U: { // 4e75 rts 
            next = 0x80522U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x80520U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x80414U:
        case 0x80418U:
        case 0x8041cU:
        case 0x80420U:
        case 0x80424U:
        case 0x80426U:
        case 0x8042aU:
        case 0x8042cU:
        case 0x8042eU:
        case 0x80430U:
        case 0x80436U:
        case 0x8043aU:
        case 0x8043cU:
        case 0x8043eU:
        case 0x80440U:
        case 0x80444U:
        case 0x80446U:
        case 0x80448U:
        case 0x8044cU:
        case 0x8044eU:
        case 0x80450U:
        case 0x80452U:
        case 0x80458U:
        case 0x8045cU:
        case 0x8045eU:
        case 0x80460U:
        case 0x80464U:
        case 0x80466U:
        case 0x8046aU:
        case 0x8046eU:
        case 0x80472U:
        case 0x80476U:
        case 0x80478U:
        case 0x8047aU:
        case 0x8047eU:
        case 0x80482U:
        case 0x80484U:
        case 0x80486U:
        case 0x80488U:
        case 0x8048cU:
        case 0x8048eU:
        case 0x80490U:
        case 0x80496U:
        case 0x80498U:
        case 0x8049cU:
        case 0x8049eU:
        case 0x804a0U:
        case 0x804a6U:
        case 0x804a8U:
        case 0x804acU:
        case 0x804b0U:
        case 0x804b4U:
        case 0x804b8U:
        case 0x804baU:
        case 0x804beU:
        case 0x804c0U:
        case 0x804c2U:
        case 0x804c4U:
        case 0x804caU:
        case 0x804ceU:
        case 0x804d2U:
        case 0x804d6U:
        case 0x804daU:
        case 0x804dcU:
        case 0x804deU:
        case 0x804e0U:
        case 0x804e2U:
        case 0x804e4U:
        case 0x804e6U:
        case 0x804e8U:
        case 0x804eaU:
        case 0x804ecU:
        case 0x804eeU:
        case 0x804f0U:
        case 0x804f2U:
        case 0x804f6U:
        case 0x804f8U:
        case 0x804faU:
        case 0x804fcU:
        case 0x804feU:
        case 0x80502U:
        case 0x80504U:
        case 0x80506U:
        case 0x80508U:
        case 0x8050cU:
        case 0x8050eU:
        case 0x80512U:
        case 0x80514U:
        case 0x80518U:
        case 0x8051cU:
        case 0x80520U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
