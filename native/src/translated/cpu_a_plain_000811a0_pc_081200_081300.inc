CpuAPlain000811A0Step handle_cpu_a_plain_000811a0_pc_081200_081300(
    FunctionContext &c, CpuRegisters &r, unverified::Machine &m,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind, std::uint8_t &instruction_kind)
{
    (void)c;
    switch (pc) {
        case 0x81202U: { // d2fc007e adda.w #$7e, a1
            next = 0x81206U;
            (void)m.word(0x81206U);
            const auto source_value = 0x7eU;
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x81208U);
            break;
        }
        case 0x81206U: { // 60aa bra.b $811b2
            next = 0x81208U;
            instruction_kind = 1U;
            if (true) { next = 0x811b2U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x81208U: { // b07c0020 cmp.w #$20, d0
            next = 0x8120cU;
            (void)m.word(0x8120cU);
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            (void)m.word(0x8120eU);
            break;
        }
        case 0x8120cU: { // 6722 beq.b $81230
            next = 0x8120eU;
            instruction_kind = 1U;
            if ((r.status & 4U) != 0U) { next = 0x81230U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x8120eU: { // d040 add.w d0, d0
            next = 0x81210U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x81212U);
            break;
        }
        case 0x81210U: { // d040 add.w d0, d0
            next = 0x81212U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x81214U);
            break;
        }
        case 0x81212U: { // d07c0080 add.w #$80, d0
            next = 0x81216U;
            (void)m.word(0x81216U);
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x81218U);
            break;
        }
        case 0x81216U: { // 8041 or.w d1, d0
            next = 0x81218U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            (void)m.word(0x8121aU);
            break;
        }
        case 0x81218U: { // 3280 move.w d0, (a1)
            next = 0x8121aU;
            const auto value = r.data[0];
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x8121cU);
            break;
        }
        case 0x8121aU: { // 5440 addq.w #$2, d0
            next = 0x8121cU;
            const auto source_value = 0x2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x8121eU);
            break;
        }
        case 0x8121cU: { // 3300 move.w d0, -(a1)
            next = 0x8121eU;
            (void)m.word(0x81220U);
            const auto value = r.data[0];
            r.address[1] -= 2U;
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x8121eU: { // d2fc0082 adda.w #$82, a1
            next = 0x81222U;
            (void)m.word(0x81222U);
            const auto source_value = 0x82U;
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x81224U);
            break;
        }
        case 0x81222U: { // 5340 subq.w #$1, d0
            next = 0x81224U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x81226U);
            break;
        }
        case 0x81224U: { // 3280 move.w d0, (a1)
            next = 0x81226U;
            const auto value = r.data[0];
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x81228U);
            break;
        }
        case 0x81226U: { // 5440 addq.w #$2, d0
            next = 0x81228U;
            const auto source_value = 0x2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x8122aU);
            break;
        }
        case 0x81228U: { // 3300 move.w d0, -(a1)
            next = 0x8122aU;
            (void)m.word(0x8122cU);
            const auto value = r.data[0];
            r.address[1] -= 2U;
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x8122aU: { // d2fc0082 adda.w #$82, a1
            next = 0x8122eU;
            (void)m.word(0x8122eU);
            const auto source_value = 0x82U;
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x81230U);
            break;
        }
        case 0x8122eU: { // 6082 bra.b $811b2
            next = 0x81230U;
            instruction_kind = 1U;
            if (true) { next = 0x811b2U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x81230U: { // 92fc0002 suba.w #$2, a1
            next = 0x81234U;
            (void)m.word(0x81234U);
            const auto source_value = 0x2U;
            r.address[1] -= static_cast<std::int16_t>(source_value);
            (void)m.word(0x81236U);
            break;
        }
        case 0x81234U: { // 22fc00000000 move.l #$0, (a1)+
            next = 0x8123aU;
            (void)m.word(0x81238U);
            (void)m.word(0x8123aU);
            const auto value = 0x0U;
            const auto destination_address = r.address[1];
            m.lng(destination_address, value);
            r.address[1] += 4U;
            m.logic(value, 32U);
            (void)m.word(0x8123cU);
            break;
        }
        case 0x8123aU: { // d2fc007e adda.w #$7e, a1
            next = 0x8123eU;
            (void)m.word(0x8123eU);
            const auto source_value = 0x7eU;
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x81240U);
            break;
        }
        case 0x8123eU: { // 92fc0002 suba.w #$2, a1
            next = 0x81242U;
            (void)m.word(0x81242U);
            const auto source_value = 0x2U;
            r.address[1] -= static_cast<std::int16_t>(source_value);
            (void)m.word(0x81244U);
            break;
        }
        case 0x81242U: { // 22fc00000000 move.l #$0, (a1)+
            next = 0x81248U;
            (void)m.word(0x81246U);
            (void)m.word(0x81248U);
            const auto value = 0x0U;
            const auto destination_address = r.address[1];
            m.lng(destination_address, value);
            r.address[1] += 4U;
            m.logic(value, 32U);
            (void)m.word(0x8124aU);
            break;
        }
        case 0x81248U: { // d2fc007e adda.w #$7e, a1
            next = 0x8124cU;
            (void)m.word(0x8124cU);
            const auto source_value = 0x7eU;
            r.address[1] += static_cast<std::int16_t>(source_value);
            (void)m.word(0x8124eU);
            break;
        }
        case 0x8124cU: { // 6000ff64 bra.w $811b2
            next = 0x81250U;
            instruction_kind = 1U;
            if (true) { next = 0x811b2U; transfer_kind = 1U; }
            if (transfer_kind == 1U) (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
    default: return {};
    }
    return {true, std::nullopt};
}
