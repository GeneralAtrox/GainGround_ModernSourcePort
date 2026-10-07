CpuAProcessAssetLoadRequestStep handle_cpu_a_process_asset_load_request_pc_080460_0804a0(
    FunctionContext &c, CpuRegisters &r, unverified::Machine &m,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind)
{
    switch (pc) {
        case 0x80460U: { // 610000c0 bsr.w $80522
            next = 0x80464U;
            const auto result = m.call(c, 64U, 0x80460U, 0x80522U, 0x80464U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
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
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x8046aU: { // 61000138 bsr.w $805a4
            next = 0x8046eU;
            const auto result = m.call(c, 67U, 0x8046aU, 0x805a4U, 0x8046eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
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
    default: return {};
    }
    return {true, std::nullopt};
}
