CpuAProcessAssetLoadRequestStep handle_cpu_a_process_asset_load_request_pc_0804e0_080520(
    FunctionContext &c, CpuRegisters &r, unverified::Machine &m,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind)
{
    (void)c;
    switch (pc) {
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
    default: return {};
    }
    return {true, std::nullopt};
}
