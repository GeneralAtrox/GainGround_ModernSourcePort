
FunctionResult cpu_b_stage_019a0e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19a0eU: { // 3c6d0036 movea.w $36(a5), a6
            next = 0x19a12U;
            const auto source_address = r.address[5] + 0x36U;
            const auto value = m.word(source_address);
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x19a12U: { // 4a16 tst.b (a6)
            next = 0x19a14U;
            const auto destination_address = r.address[6];
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x19a14U: { // 6b04 bmi.b $19a1a
            next = 0x19a16U;
            if ((r.status & 8U) != 0U) { next = 0x19a1aU; transfer_kind = 1U; }
            break;
        }
        case 0x19a16U: { // 4255 clr.w (a5)
            next = 0x19a18U;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19a18U: { // 4e75 rts 
            next = 0x19a1aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19a18U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19a1aU: { // 286e005c movea.l $5c(a6), a4
            next = 0x19a1eU;
            const auto source_address = r.address[6] + 0x5cU;
            const auto value = m.lng(source_address);
            r.address[4] = value;
            break;
        }
        case 0x19a1eU: { // 5c4c addq.w #$6, a4
            next = 0x19a20U;
            const auto source_value = 0x6U;
            r.address[4] += source_value;
            break;
        }
        case 0x19a20U: { // d8ed003a adda.w $3a(a5), a4
            next = 0x19a24U;
            const auto source_address = r.address[5] + 0x3aU;
            const auto source_value = m.word(source_address);
            r.address[4] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x19a24U: { // 3b5c0006 move.w (a4)+, $6(a5)
            next = 0x19a28U;
            const auto source_address = r.address[4];
            const auto value = m.word(source_address);
            r.address[4] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19a28U: { // 6b6c bmi.b $19a96
            next = 0x19a2aU;
            if ((r.status & 8U) != 0U) { next = 0x19a96U; transfer_kind = 1U; }
            break;
        }
        case 0x19a2aU: { // 101c move.b (a4)+, d0
            next = 0x19a2cU;
            const auto source_address = r.address[4];
            const auto value = m.byte(source_address);
            r.address[4] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19a2cU: { // 4880 ext.w d0
            next = 0x19a2eU;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[0])));
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19a2eU: { // d06e000c add.w $c(a6), d0
            next = 0x19a32U;
            const auto source_address = r.address[6] + 0xcU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19a32U: { // 3b40000c move.w d0, $c(a5)
            next = 0x19a36U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0xcU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19a36U: { // 101c move.b (a4)+, d0
            next = 0x19a38U;
            const auto source_address = r.address[4];
            const auto value = m.byte(source_address);
            r.address[4] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19a38U: { // 4880 ext.w d0
            next = 0x19a3aU;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[0])));
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19a3aU: { // d06e000e add.w $e(a6), d0
            next = 0x19a3eU;
            const auto source_address = r.address[6] + 0xeU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19a3eU: { // 3b40000e move.w d0, $e(a5)
            next = 0x19a42U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0xeU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19a42U: { // 3b6e00080008 move.w $8(a6), $8(a5)
            next = 0x19a48U;
            const auto source_address = r.address[6] + 0x8U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19a48U: { // 301c move.w (a4)+, d0
            next = 0x19a4aU;
            const auto source_address = r.address[4];
            const auto value = m.word(source_address);
            r.address[4] += 2U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19a4aU: { // 1b400001 move.b d0, $1(a5)
            next = 0x19a4eU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19a4eU: { // 3b6e001a001a move.w $1a(a6), $1a(a5)
            next = 0x19a54U;
            const auto source_address = r.address[6] + 0x1aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19a54U: { // 4a2e003f tst.b $3f(a6)
            next = 0x19a58U;
            const auto destination_address = r.address[6] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x19a58U: { // 6714 beq.b $19a6e
            next = 0x19a5aU;
            if ((r.status & 4U) != 0U) { next = 0x19a6eU; transfer_kind = 1U; }
            break;
        }
        case 0x19a5aU: { // 586d0066 addq.w #$4, $66(a5)
            next = 0x19a5eU;
            const auto source_value = 0x4U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19a5eU: { // 026d001c0066 andi.w #$1c, $66(a5)
            next = 0x19a64U;
            const auto source_value = 0x1cU;
            const auto destination_address = r.address[5] + 0x66U;
            const auto destination_value = m.word(destination_address);
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19a64U: { // 302d0066 move.w $66(a5), d0
            next = 0x19a68U;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19a68U: { // 41fa19ec lea.l $1b456(pc), a0
            next = 0x19a6cU;
            const auto source_address = 0x1b456U;
            r.address[0] = source_address;
            break;
        }
        case 0x19a6cU: { // 6016 bra.b $19a84
            next = 0x19a6eU;
            if (true) { next = 0x19a84U; transfer_kind = 1U; }
            break;
        }
        case 0x19a6eU: { // 102e0051 move.b $51(a6), d0
            next = 0x19a72U;
            const auto source_address = r.address[6] + 0x51U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19a72U: { // 6f1c ble.b $19a90
            next = 0x19a74U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x19a90U; transfer_kind = 1U; }
            break;
        }
        case 0x19a74U: { // 0c000005 cmpi.b #$5, d0
            next = 0x19a78U;
            const auto source_value = 0x5U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x19a78U: { // 6e16 bgt.b $19a90
            next = 0x19a7aU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19a90U; transfer_kind = 1U; }
            break;
        }
        case 0x19a7aU: { // 302e0056 move.w $56(a6), d0
            next = 0x19a7eU;
            const auto source_address = r.address[6] + 0x56U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19a7eU: { // ec40 asr.w #$6, d0
            next = 0x19a80U;
            m.shift_word(0U, 6U, false, true);
            break;
        }
        case 0x19a80U: { // 41fa19f4 lea.l $1b476(pc), a0
            next = 0x19a84U;
            const auto source_address = 0x1b476U;
            r.address[0] = source_address;
            break;
        }
        case 0x19a84U: { // 3b7000000006 move.w (a0, d0.w), $6(a5)
            next = 0x19a8aU;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19a8aU: { // 1b7000030001 move.b $3(a0, d0.w), $1(a5)
            next = 0x19a90U;
            const auto source_address = r.address[0] + 0x3U + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19a90U: { // 4eb900015df2 jsr $15df2.l
            next = 0x19a96U;
            const auto result = m.call(c, 282U, 0x19a90U, 0x15df2U, 0x19a96U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19a96U: { // 08950000 bclr.b #$0, (a5)
            next = 0x19a9aU;
            const auto destination_address = r.address[5];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x19a9aU: { // 4e75 rts 
            next = 0x19a9cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19a9aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x19a0eU:
        case 0x19a12U:
        case 0x19a14U:
        case 0x19a16U:
        case 0x19a18U:
        case 0x19a1aU:
        case 0x19a1eU:
        case 0x19a20U:
        case 0x19a24U:
        case 0x19a28U:
        case 0x19a2aU:
        case 0x19a2cU:
        case 0x19a2eU:
        case 0x19a32U:
        case 0x19a36U:
        case 0x19a38U:
        case 0x19a3aU:
        case 0x19a3eU:
        case 0x19a42U:
        case 0x19a48U:
        case 0x19a4aU:
        case 0x19a4eU:
        case 0x19a54U:
        case 0x19a58U:
        case 0x19a5aU:
        case 0x19a5eU:
        case 0x19a64U:
        case 0x19a68U:
        case 0x19a6cU:
        case 0x19a6eU:
        case 0x19a72U:
        case 0x19a74U:
        case 0x19a78U:
        case 0x19a7aU:
        case 0x19a7eU:
        case 0x19a80U:
        case 0x19a84U:
        case 0x19a8aU:
        case 0x19a90U:
        case 0x19a96U:
        case 0x19a9aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
