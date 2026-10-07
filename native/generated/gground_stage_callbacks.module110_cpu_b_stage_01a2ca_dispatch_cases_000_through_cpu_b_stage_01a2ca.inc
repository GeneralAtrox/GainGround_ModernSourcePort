
static bool cpu_b_stage_01a2ca_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1a2caU: { // 3c6d0036 movea.w $36(a5), a6
            next = 0x1a2ceU;
            const auto source_address = r.address[5] + 0x36U;
            const auto value = m.word(source_address);
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x1a2ceU: { // 4a16 tst.b (a6)
            next = 0x1a2d0U;
            const auto destination_address = r.address[6];
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1a2d0U: { // 6b04 bmi.b $1a2d6
            next = 0x1a2d2U;
            if ((r.status & 8U) != 0U) { next = 0x1a2d6U; transfer_kind = 1U; }
            break;
        }
        case 0x1a2d2U: { // 4255 clr.w (a5)
            next = 0x1a2d4U;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1a2d6U: { // 286e005c movea.l $5c(a6), a4
            next = 0x1a2daU;
            const auto source_address = r.address[6] + 0x5cU;
            const auto value = m.lng(source_address);
            r.address[4] = value;
            break;
        }
        case 0x1a2daU: { // 5c4c addq.w #$6, a4
            next = 0x1a2dcU;
            const auto source_value = 0x6U;
            r.address[4] += source_value;
            break;
        }
        case 0x1a2dcU: { // d8ed003a adda.w $3a(a5), a4
            next = 0x1a2e0U;
            const auto source_address = r.address[5] + 0x3aU;
            const auto source_value = m.word(source_address);
            r.address[4] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1a2e0U: { // 3b5c0006 move.w (a4)+, $6(a5)
            next = 0x1a2e4U;
            const auto source_address = r.address[4];
            const auto value = m.word(source_address);
            r.address[4] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a2e4U: { // 6b40 bmi.b $1a326
            next = 0x1a2e6U;
            if ((r.status & 8U) != 0U) { next = 0x1a326U; transfer_kind = 1U; }
            break;
        }
        case 0x1a2e6U: { // 101c move.b (a4)+, d0
            next = 0x1a2e8U;
            const auto source_address = r.address[4];
            const auto value = m.byte(source_address);
            r.address[4] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a2e8U: { // 4880 ext.w d0
            next = 0x1a2eaU;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[0])));
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a2eaU: { // d06e000c add.w $c(a6), d0
            next = 0x1a2eeU;
            const auto source_address = r.address[6] + 0xcU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a2eeU: { // 3b40000c move.w d0, $c(a5)
            next = 0x1a2f2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0xcU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a2f2U: { // 101c move.b (a4)+, d0
            next = 0x1a2f4U;
            const auto source_address = r.address[4];
            const auto value = m.byte(source_address);
            r.address[4] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a2f4U: { // 4880 ext.w d0
            next = 0x1a2f6U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[0])));
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a2f6U: { // d06e000e add.w $e(a6), d0
            next = 0x1a2faU;
            const auto source_address = r.address[6] + 0xeU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a2faU: { // 3b40000e move.w d0, $e(a5)
            next = 0x1a2feU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0xeU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a2feU: { // 303c0c00 move.w #$c00, d0
            next = 0x1a302U;
            const auto value = 0xc00U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a302U: { // d01c add.b (a4)+, d0
            next = 0x1a304U;
            const auto source_address = r.address[4];
            const auto source_value = m.byte(source_address);
            r.address[4] += 1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x1a304U: { // 3b400008 move.w d0, $8(a5)
            next = 0x1a308U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a308U: { // 1b5c0001 move.b (a4)+, $1(a5)
            next = 0x1a30cU;
            const auto source_address = r.address[4];
            const auto value = m.byte(source_address);
            r.address[4] += 1U;
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a30cU: { // 3b6e001a001a move.w $1a(a6), $1a(a5)
            next = 0x1a312U;
            const auto source_address = r.address[6] + 0x1aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a312U: { // 0c6e001a0058 cmpi.w #$1a, $58(a6)
            next = 0x1a318U;
            const auto source_value = 0x1aU;
            const auto destination_address = r.address[6] + 0x58U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a318U: { // 6606 bne.b $1a320
            next = 0x1a31aU;
            if ((r.status & 4U) == 0U) { next = 0x1a320U; transfer_kind = 1U; }
            break;
        }
        case 0x1a31aU: { // 3b6e00080008 move.w $8(a6), $8(a5)
            next = 0x1a320U;
            const auto source_address = r.address[6] + 0x8U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a326U: { // 08950000 bclr.b #$0, (a5)
            next = 0x1a32aU;
            const auto destination_address = r.address[5];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01a2ca(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1a2d4U: { // 4e75 rts 
            next = 0x1a2d6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a2d4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1a320U: { // 4eb900015df2 jsr $15df2.l
            next = 0x1a326U;
            const auto result = m.call(c, 282U, 0x1a320U, 0x15df2U, 0x1a326U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a32aU: { // 4e75 rts 
            next = 0x1a32cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a32aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01a2ca_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1a2caU:
        case 0x1a2ceU:
        case 0x1a2d0U:
        case 0x1a2d2U:
        case 0x1a2d4U:
        case 0x1a2d6U:
        case 0x1a2daU:
        case 0x1a2dcU:
        case 0x1a2e0U:
        case 0x1a2e4U:
        case 0x1a2e6U:
        case 0x1a2e8U:
        case 0x1a2eaU:
        case 0x1a2eeU:
        case 0x1a2f2U:
        case 0x1a2f4U:
        case 0x1a2f6U:
        case 0x1a2faU:
        case 0x1a2feU:
        case 0x1a302U:
        case 0x1a304U:
        case 0x1a308U:
        case 0x1a30cU:
        case 0x1a312U:
        case 0x1a318U:
        case 0x1a31aU:
        case 0x1a320U:
        case 0x1a326U:
        case 0x1a32aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
