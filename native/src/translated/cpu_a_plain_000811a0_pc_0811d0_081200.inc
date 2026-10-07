CpuAPlain000811A0Step handle_cpu_a_plain_000811a0_pc_0811d0_081200(
    FunctionContext &c, CpuRegisters &r, unverified::Machine &m,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind, std::uint8_t &instruction_kind)
{
    switch (pc) {
        case 0x811d0U: { // d2fc0080 adda.w #$80, a1
            next = 0x811d4U;
            (void)m.word(0x811d4U);
            const auto source_value = 0x80U;
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x811d6U);
            break;
        }
        case 0x811d4U: { // 60dc bra.b $811b2
            next = 0x811d6U;
            instruction_kind = 1U;
            if (true) { next = 0x811b2U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811d6U: { // 32bc0000 move.w #$0, (a1)
            next = 0x811daU;
            (void)m.word(0x811daU);
            const auto value = 0x0U;
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x811dcU);
            break;
        }
        case 0x811daU: { // d2fc0080 adda.w #$80, a1
            next = 0x811deU;
            (void)m.word(0x811deU);
            const auto source_value = 0x80U;
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x811e0U);
            break;
        }
        case 0x811deU: { // 60d2 bra.b $811b2
            next = 0x811e0U;
            instruction_kind = 1U;
            if (true) { next = 0x811b2U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811e0U: { // 4e75 rts 
            next = 0x811e2U;
            const auto result = m.ret();
            (void)m.word(r.program_counter); (void)m.word(r.program_counter + 2U);
            if (auto event = unverified::diagnostic_irq3(c, m, 0x811e0U, r.program_counter, 3U)) return {true, (*event)};
            return {true, (result)};
            break;
        }
        case 0x811e2U: { // b07c0020 cmp.w #$20, d0
            next = 0x811e6U;
            (void)m.word(0x811e6U);
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            (void)m.word(0x811e8U);
            break;
        }
        case 0x811e6U: { // 6710 beq.b $811f8
            next = 0x811e8U;
            instruction_kind = 1U;
            if ((r.status & 4U) != 0U) { next = 0x811f8U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811e8U: { // d040 add.w d0, d0
            next = 0x811eaU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x811ecU);
            break;
        }
        case 0x811eaU: { // 8041 or.w d1, d0
            next = 0x811ecU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            (void)m.word(0x811eeU);
            break;
        }
        case 0x811ecU: { // 3280 move.w d0, (a1)
            next = 0x811eeU;
            const auto value = r.data[0];
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x811f0U);
            break;
        }
        case 0x811eeU: { // 5240 addq.w #$1, d0
            next = 0x811f0U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x811f2U);
            break;
        }
        case 0x811f0U: { // 3300 move.w d0, -(a1)
            next = 0x811f2U;
            (void)m.word(0x811f4U);
            const auto value = r.data[0];
            r.address[1] -= 2U;
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x811f2U: { // d2fc0082 adda.w #$82, a1
            next = 0x811f6U;
            (void)m.word(0x811f6U);
            const auto source_value = 0x82U;
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x811f8U);
            break;
        }
        case 0x811f6U: { // 60ba bra.b $811b2
            next = 0x811f8U;
            instruction_kind = 1U;
            if (true) { next = 0x811b2U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x811f8U: { // 92fc0002 suba.w #$2, a1
            next = 0x811fcU;
            (void)m.word(0x811fcU);
            const auto source_value = 0x2U;
            r.address[1] -= static_cast<std::int16_t>(source_value);
            (void)m.word(0x811feU);
            break;
        }
        case 0x811fcU: { // 22fc00000000 move.l #$0, (a1)+
            next = 0x81202U;
            (void)m.word(0x81200U);
            (void)m.word(0x81202U);
            const auto value = 0x0U;
            const auto destination_address = r.address[1];
            m.lng(destination_address, value);
            r.address[1] += 4U;
            m.logic(value, 32U);
            (void)m.word(0x81204U);
            break;
        }
    default: return {};
    }
    return {true, std::nullopt};
}
