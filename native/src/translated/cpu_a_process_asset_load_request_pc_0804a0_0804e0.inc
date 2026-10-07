CpuAProcessAssetLoadRequestStep handle_cpu_a_process_asset_load_request_pc_0804a0_0804e0(
    FunctionContext &c, CpuRegisters &r, unverified::Machine &m,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind)
{
    switch (pc) {
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
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
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
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x804b4U: { // 61000102 bsr.w $805b8
            next = 0x804b8U;
            const auto result = m.call(c, 68U, 0x804b4U, 0x805b8U, 0x804b8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
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
    default: return {};
    }
    return {true, std::nullopt};
}
