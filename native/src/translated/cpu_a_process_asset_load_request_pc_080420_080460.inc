CpuAProcessAssetLoadRequestStep handle_cpu_a_process_asset_load_request_pc_080420_080460(
    FunctionContext &c, CpuRegisters &r, unverified::Machine &m,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind)
{
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
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
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
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
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
    default: return {};
    }
    return {true, std::nullopt};
}
