CpuAPlain000811A0Step handle_cpu_a_plain_000811a0_pc_0811a0_0811d0(
    FunctionContext &c, CpuRegisters &r, unverified::Machine &m,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind, std::uint8_t &instruction_kind)
{
    (void)c;
    switch (pc) {
        case 0x811a0U: { // e859 ror.w #$4, d1
            next = 0x811a2U;
            m.rotate_right(1U, 4U, 16U);
            (void)m.word(0x811a4U);
            break;
        }
        case 0x811a2U: { // 7000 moveq #$0, d0
            next = 0x811a4U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            (void)m.word(0x811a6U);
            break;
        }
        case 0x811a4U: { // 1018 move.b (a0)+, d0
            next = 0x811a6U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            (void)m.word(0x811a8U);
            break;
        }
        case 0x811a6U: { // e148 lsl.w #$8, d0
            next = 0x811a8U;
            m.shift_word(0U, 8U, true, false);
            (void)m.word(0x811aaU);
            break;
        }
        case 0x811a8U: { // 1018 move.b (a0)+, d0
            next = 0x811aaU;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            (void)m.word(0x811acU);
            break;
        }
        case 0x811aaU: { // 43f900200000 lea.l $200000.l, a1
            next = 0x811b0U;
            (void)m.word(0x811aeU);
            (void)m.word(0x811b0U);
            r.address[1] = 0x200000U;
            (void)m.word(0x811b2U);
            break;
        }
        case 0x811b0U: { // d2c0 adda.w d0, a1
            next = 0x811b2U;
            const auto source_value = r.data[0];
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x811b4U);
            break;
        }
        case 0x811b2U: { // 7000 moveq #$0, d0
            next = 0x811b4U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            (void)m.word(0x811b6U);
            break;
        }
        case 0x811b4U: { // 1018 move.b (a0)+, d0
            next = 0x811b6U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            (void)m.word(0x811b8U);
            break;
        }
        case 0x811b6U: { // 6728 beq.b $811e0
            next = 0x811b8U;
            instruction_kind = 1U;
            if ((r.status & 4U) != 0U) { next = 0x811e0U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811b8U: { // 45fa0006 lea.l $811c0(pc), a2
            next = 0x811bcU;
            (void)m.word(0x811bcU);
            const auto source_address = 0x811c0U;
            r.address[2] = source_address;
            (void)m.word(0x811beU);
            break;
        }
        case 0x811bcU: { // 4ef22000 jmp (a2, d2.w)
            next = 0x811c0U;
            instruction_kind = 1U;
            const auto target_address = r.address[2] + static_cast<std::int16_t>(r.data[2]);
            next = target_address & 0xffffffU;
            (void)m.word(next);
            (void)m.word(next + 2U);
            transfer_kind = 1U;
            break;
        }
        case 0x811c0U: { // 6004 bra.b $811c6
            next = 0x811c2U;
            instruction_kind = 1U;
            if (true) { next = 0x811c6U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811c2U: { // 601e bra.b $811e2
            next = 0x811c4U;
            instruction_kind = 1U;
            if (true) { next = 0x811e2U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811c4U: { // 6042 bra.b $81208
            next = 0x811c6U;
            instruction_kind = 1U;
            if (true) { next = 0x81208U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811c6U: { // b07c0020 cmp.w #$20, d0
            next = 0x811caU;
            (void)m.word(0x811caU);
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            (void)m.word(0x811ccU);
            break;
        }
        case 0x811caU: { // 670a beq.b $811d6
            next = 0x811ccU;
            instruction_kind = 1U;
            if ((r.status & 4U) != 0U) { next = 0x811d6U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811ccU: { // 8041 or.w d1, d0
            next = 0x811ceU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            (void)m.word(0x811d0U);
            break;
        }
        case 0x811ceU: { // 3280 move.w d0, (a1)
            next = 0x811d0U;
            const auto value = r.data[0];
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x811d2U);
            break;
        }
    default: return {};
    }
    return {true, std::nullopt};
}
