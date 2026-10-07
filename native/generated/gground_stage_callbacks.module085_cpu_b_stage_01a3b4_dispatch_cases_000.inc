
static bool cpu_b_stage_01a3b4_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1a3b4U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1a3b8U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1a3b8U: { // 660000e8 bne.w $1a4a2
            next = 0x1a3bcU;
            if ((r.status & 4U) == 0U) { next = 0x1a4a2U; transfer_kind = 1U; }
            break;
        }
        case 0x1a3c2U: { // 302d0066 move.w $66(a5), d0
            next = 0x1a3c6U;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a3c6U: { // b07c0040 cmp.w #$40, d0
            next = 0x1a3caU;
            const auto source_value = 0x40U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a3caU: { // 6a00007e bpl.w $1a44a
            next = 0x1a3ceU;
            if ((r.status & 8U) == 0U) { next = 0x1a44aU; transfer_kind = 1U; }
            break;
        }
        case 0x1a3ceU: { // 3c6d0036 movea.w $36(a5), a6
            next = 0x1a3d2U;
            const auto source_address = r.address[5] + 0x36U;
            const auto value = m.word(source_address);
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x1a3d2U: { // 0c6e00050044 cmpi.w #$5, $44(a6)
            next = 0x1a3d8U;
            const auto source_value = 0x5U;
            const auto destination_address = r.address[6] + 0x44U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a3d8U: { // 6650 bne.b $1a42a
            next = 0x1a3daU;
            if ((r.status & 4U) == 0U) { next = 0x1a42aU; transfer_kind = 1U; }
            break;
        }
        case 0x1a3daU: { // 302e0012 move.w $12(a6), d0
            next = 0x1a3deU;
            const auto source_address = r.address[6] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a3deU: { // 322e001a move.w $1a(a6), d1
            next = 0x1a3e2U;
            const auto source_address = r.address[6] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a3e2U: { // 906d0012 sub.w $12(a5), d0
            next = 0x1a3e6U;
            const auto source_address = r.address[5] + 0x12U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a3e6U: { // 926d001a sub.w $1a(a5), d1
            next = 0x1a3eaU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1a3f6U: { // 303c0020 move.w #$20, d0
            next = 0x1a3faU;
            const auto value = 0x20U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a3faU: { // 926d005a sub.w $5a(a5), d1
            next = 0x1a3feU;
            const auto source_address = r.address[5] + 0x5aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1a3feU: { // 024107ff andi.w #$7ff, d1
            next = 0x1a402U;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[1];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1a402U: { // 6726 beq.b $1a42a
            next = 0x1a404U;
            if ((r.status & 4U) != 0U) { next = 0x1a42aU; transfer_kind = 1U; }
            break;
        }
        case 0x1a404U: { // 0c410400 cmpi.w #$400, d1
            next = 0x1a408U;
            const auto source_value = 0x400U;
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a408U: { // 6a08 bpl.b $1a412
            next = 0x1a40aU;
            if ((r.status & 8U) == 0U) { next = 0x1a412U; transfer_kind = 1U; }
            break;
        }
        case 0x1a40aU: { // b240 cmp.w d0, d1
            next = 0x1a40cU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a40cU: { // 6b02 bmi.b $1a410
            next = 0x1a40eU;
            if ((r.status & 8U) != 0U) { next = 0x1a410U; transfer_kind = 1U; }
            break;
        }
        case 0x1a40eU: { // 3200 move.w d0, d1
            next = 0x1a410U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a410U: { // 600c bra.b $1a41e
            next = 0x1a412U;
            if (true) { next = 0x1a41eU; transfer_kind = 1U; }
            break;
        }
        case 0x1a412U: { // 04410800 subi.w #$800, d1
            next = 0x1a416U;
            const auto source_value = 0x800U;
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1a416U: { // 4440 neg.w d0
            next = 0x1a418U;
            const auto old = r.data[0];
            const auto value = m.sub(0U, old, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a418U: { // b240 cmp.w d0, d1
            next = 0x1a41aU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a41aU: { // 6a02 bpl.b $1a41e
            next = 0x1a41cU;
            if ((r.status & 8U) == 0U) { next = 0x1a41eU; transfer_kind = 1U; }
            break;
        }
        case 0x1a41cU: { // 3200 move.w d0, d1
            next = 0x1a41eU;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a41eU: { // d26d005a add.w $5a(a5), d1
            next = 0x1a422U;
            const auto source_address = r.address[5] + 0x5aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1a422U: { // 024107ff andi.w #$7ff, d1
            next = 0x1a426U;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[1];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1a426U: { // 3b41005a move.w d1, $5a(a5)
            next = 0x1a42aU;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a42aU: { // 322d005a move.w $5a(a5), d1
            next = 0x1a42eU;
            const auto source_address = r.address[5] + 0x5aU;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a434U: { // 342d0066 move.w $66(a5), d2
            next = 0x1a438U;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.word(source_address);
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a438U: { // e24a lsr.w #$1, d2
            next = 0x1a43aU;
            m.shift_word(2U, 1U, false, false);
            break;
        }
        case 0x1a43aU: { // c1c2 muls.w d2, d0
            next = 0x1a43cU;
            const auto source = static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[2]));
            const auto destination = static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[0]));
            r.data[0] = static_cast<std::uint32_t>(source * destination);
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1a43cU: { // c3c2 muls.w d2, d1
            next = 0x1a43eU;
            const auto source = static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[2]));
            const auto destination = static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[1]));
            r.data[1] = static_cast<std::uint32_t>(source * destination);
            m.logic(r.data[1], 32U);
            break;
        }
    default: return false;
    }
    return true;
}
