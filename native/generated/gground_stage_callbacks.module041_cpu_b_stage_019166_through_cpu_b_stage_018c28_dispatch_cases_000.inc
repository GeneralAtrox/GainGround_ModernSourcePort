
FunctionResult cpu_b_stage_019166(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19166U: { // 41f9000363c6 lea.l $363c6.l, a0
            next = 0x1916cU;
            const auto source_address = 0x363c6U;
            r.address[0] = source_address;
            break;
        }
        case 0x1916cU: { // 302d0058 move.w $58(a5), d0
            next = 0x19170U;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19170U: { // 6b34 bmi.b $191a6
            next = 0x19172U;
            if ((r.status & 8U) != 0U) { next = 0x191a6U; transfer_kind = 1U; }
            break;
        }
        case 0x19172U: { // d040 add.w d0, d0
            next = 0x19174U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19174U: { // 3200 move.w d0, d1
            next = 0x19176U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19176U: { // e940 asl.w #$4, d0
            next = 0x19178U;
            m.asl_word(0U, 4U);
            break;
        }
        case 0x19178U: { // 9041 sub.w d1, d0
            next = 0x1917aU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1917aU: { // d0c0 adda.w d0, a0
            next = 0x1917cU;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1917cU: { // 2b48005c move.l a0, $5c(a5)
            next = 0x19180U;
            const auto value = r.address[0];
            const auto destination_address = r.address[5] + 0x5cU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x19180U: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x19184U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19184U: { // 6b20 bmi.b $191a6
            next = 0x19186U;
            if ((r.status & 8U) != 0U) { next = 0x191a6U; transfer_kind = 1U; }
            break;
        }
        case 0x19186U: { // 1b580010 move.b (a0)+, $10(a5)
            next = 0x1918aU;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_address = r.address[5] + 0x10U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1918aU: { // 1b580011 move.b (a0)+, $11(a5)
            next = 0x1918eU;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_address = r.address[5] + 0x11U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1918eU: { // 10280001 move.b $1(a0), d0
            next = 0x19192U;
            const auto source_address = r.address[0] + 0x1U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19192U: { // 803c0004 or.b #$4, d0
            next = 0x19196U;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x19196U: { // 1b400001 move.b d0, $1(a5)
            next = 0x1919aU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1919aU: { // 4eb900015d24 jsr $15d24.l
            next = 0x191a0U;
            const auto result = m.call(c, 280U, 0x1919aU, 0x15d24U, 0x191a0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x191a0U: { // 4eb900015df2 jsr $15df2.l
            next = 0x191a6U;
            const auto result = m.call(c, 282U, 0x191a0U, 0x15df2U, 0x191a6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x191a6U: { // 4e75 rts 
            next = 0x191a8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x191a6U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x19166U:
        case 0x1916cU:
        case 0x19170U:
        case 0x19172U:
        case 0x19174U:
        case 0x19176U:
        case 0x19178U:
        case 0x1917aU:
        case 0x1917cU:
        case 0x19180U:
        case 0x19184U:
        case 0x19186U:
        case 0x1918aU:
        case 0x1918eU:
        case 0x19192U:
        case 0x19196U:
        case 0x1919aU:
        case 0x191a0U:
        case 0x191a6U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_018c28_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x18c28U: { // 536d0070 subq.w #$1, $70(a5)
            next = 0x18c2cU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x70U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18c2cU: { // 6e0c bgt.b $18c3a
            next = 0x18c2eU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18c3aU; transfer_kind = 1U; }
            break;
        }
        case 0x18c2eU: { // 4255 clr.w (a5)
            next = 0x18c30U;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x18c34U: { // 42380d1b clr.b $d1b.w
            next = 0x18c38U;
            const auto destination_address = 0xd1bU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x18c3aU: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x18c3eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18c3eU: { // 6e36 bgt.b $18c76
            next = 0x18c40U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18c76U; transfer_kind = 1U; }
            break;
        }
        case 0x18c40U: { // 303c0014 move.w #$14, d0
            next = 0x18c44U;
            const auto value = 0x14U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18c4aU: { // 3b7c00080068 move.w #$8, $68(a5)
            next = 0x18c50U;
            const auto value = 0x8U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18c50U: { // 526d0066 addq.w #$1, $66(a5)
            next = 0x18c54U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18c54U: { // 026d00030066 andi.w #$3, $66(a5)
            next = 0x18c5aU;
            const auto source_value = 0x3U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto destination_value = m.word(destination_address);
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18c5aU: { // 41fa239c lea.l $1aff8(pc), a0
            next = 0x18c5eU;
            const auto source_address = 0x1aff8U;
            r.address[0] = source_address;
            break;
        }
        case 0x18c5eU: { // 302d005a move.w $5a(a5), d0
            next = 0x18c62U;
            const auto source_address = r.address[5] + 0x5aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18c62U: { // 5340 subq.w #$1, d0
            next = 0x18c64U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18c64U: { // e740 asl.w #$3, d0
            next = 0x18c66U;
            m.asl_word(0U, 3U);
            break;
        }
        case 0x18c66U: { // d07c0048 add.w #$48, d0
            next = 0x18c6aU;
            const auto source_value = 0x48U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18c6aU: { // d0c0 adda.w d0, a0
            next = 0x18c6cU;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x18c6cU: { // 302d0066 move.w $66(a5), d0
            next = 0x18c70U;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18c70U: { // 1b7000000059 move.b (a0, d0.w), $59(a5)
            next = 0x18c76U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x59U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18c7eU: { // 41fa23de lea.l $1b05e(pc), a0
            next = 0x18c82U;
            const auto source_address = 0x1b05eU;
            r.address[0] = source_address;
            break;
        }
    default: return false;
    }
    return true;
}
