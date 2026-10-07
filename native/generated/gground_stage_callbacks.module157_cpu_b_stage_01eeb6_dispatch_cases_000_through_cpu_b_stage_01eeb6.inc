
static bool cpu_b_stage_01eeb6_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1eeb6U: { // 206d0050 movea.l $50(a5), a0
            next = 0x1eebaU;
            const auto source_address = r.address[5] + 0x50U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1eebaU: { // 302d0054 move.w $54(a5), d0
            next = 0x1eebeU;
            const auto source_address = r.address[5] + 0x54U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1eebeU: { // b018 cmp.b (a0)+, d0
            next = 0x1eec0U;
            const auto source_address = r.address[0];
            const auto source_value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x1eec0U: { // 6d06 blt.b $1eec8
            next = 0x1eec2U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1eec8U; transfer_kind = 1U; }
            break;
        }
        case 0x1eec2U: { // d0fc0005 adda.w #$5, a0
            next = 0x1eec6U;
            const auto source_value = 0x5U;
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1eec6U: { // 60f6 bra.b $1eebe
            next = 0x1eec8U;
            if (true) { next = 0x1eebeU; transfer_kind = 1U; }
            break;
        }
        case 0x1eec8U: { // 303c0c00 move.w #$c00, d0
            next = 0x1eeccU;
            const auto value = 0xc00U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1eeccU: { // d018 add.b (a0)+, d0
            next = 0x1eeceU;
            const auto source_address = r.address[0];
            const auto source_value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x1eeceU: { // 3b400008 move.w d0, $8(a5)
            next = 0x1eed2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1eed2U: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x1eed6U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1eed6U: { // 1218 move.b (a0)+, d1
            next = 0x1eed8U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1eed8U: { // 6714 beq.b $1eeee
            next = 0x1eedaU;
            if ((r.status & 4U) != 0U) { next = 0x1eeeeU; transfer_kind = 1U; }
            break;
        }
        case 0x1eedaU: { // 08d50007 bset.b #$7, (a5)
            next = 0x1eedeU;
            const auto destination_address = r.address[5];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1eedeU: { // 1b580001 move.b (a0)+, $1(a5)
            next = 0x1eee2U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1eee2U: { // 526d0054 addq.w #$1, $54(a5)
            next = 0x1eee6U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x54U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1eeeeU: { // 44fc0000 move.w #$0, ccr
            next = 0x1eef2U;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x0U & 0x1fU));
            break;
        }
        case 0x1eef2U: { // 10380c14 move.b $c14.w, d0
            next = 0x1eef6U;
            const auto source_address = 0xc14U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1eef6U: { // 12380c15 move.b $c15.w, d1
            next = 0x1eefaU;
            const auto source_address = 0xc15U;
            const auto value = m.byte(source_address);
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1eefaU: { // 7601 moveq #$1, d3
            next = 0x1eefcU;
            r.data[3] = 0x1U;
            m.logic(r.data[3], 32U);
            break;
        }
        case 0x1eefcU: { // 7400 moveq #$0, d2
            next = 0x1eefeU;
            r.data[2] = 0x0U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0x1eefeU: { // 8303 sbcd.b d3, d1
            next = 0x1ef00U;
            const auto source_value = r.data[3];
            const auto value = m.sbcd(r.data[1], source_value);
            m.db(1U, value);
            break;
        }
        case 0x1ef00U: { // d542 addx.w d2, d2
            next = 0x1ef02U;
            const auto source = r.data[2] & 0xffffU;
            const auto destination = r.data[2] & 0xffffU;
            const auto old_status = r.status;
            const auto wide = destination + source + ((old_status >> 4U) & 1U);
            const auto value = wide & 0xffffU;
            m.dw(2U, value);
            r.status = static_cast<std::uint16_t>((old_status & ~0x1fU) |
                (wide > 0xffffU ? 0x11U : 0U) | (value & 0x8000U ? 8U : 0U) |
                (value == 0U ? old_status & 4U : 0U) |
                ((~(source ^ destination) & (destination ^ value) & 0x8000U) ? 2U : 0U));
            break;
        }
        case 0x1ef02U: { // 8102 sbcd.b d2, d0
            next = 0x1ef04U;
            const auto source_value = r.data[2];
            const auto value = m.sbcd(r.data[0], source_value);
            m.db(0U, value);
            break;
        }
        case 0x1ef04U: { // 11c00c14 move.b d0, $c14.w
            next = 0x1ef08U;
            const auto value = r.data[0];
            const auto destination_address = 0xc14U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1ef08U: { // 11c10c15 move.b d1, $c15.w
            next = 0x1ef0cU;
            const auto value = r.data[1];
            const auto destination_address = 0xc15U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1ef0cU: { // 08950007 bclr.b #$7, (a5)
            next = 0x1ef10U;
            const auto destination_address = r.address[5];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01eeb6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1eee6U: { // 4eb900015df2 jsr $15df2.l
            next = 0x1eeecU;
            const auto result = m.call(c, 282U, 0x1eee6U, 0x15df2U, 0x1eeecU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1eeecU: { // 4e75 rts 
            next = 0x1eeeeU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1eeecU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1ef10U: { // 4e75 rts 
            next = 0x1ef12U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1ef10U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01eeb6_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1eeb6U:
        case 0x1eebaU:
        case 0x1eebeU:
        case 0x1eec0U:
        case 0x1eec2U:
        case 0x1eec6U:
        case 0x1eec8U:
        case 0x1eeccU:
        case 0x1eeceU:
        case 0x1eed2U:
        case 0x1eed6U:
        case 0x1eed8U:
        case 0x1eedaU:
        case 0x1eedeU:
        case 0x1eee2U:
        case 0x1eee6U:
        case 0x1eeecU:
        case 0x1eeeeU:
        case 0x1eef2U:
        case 0x1eef6U:
        case 0x1eefaU:
        case 0x1eefcU:
        case 0x1eefeU:
        case 0x1ef00U:
        case 0x1ef02U:
        case 0x1ef04U:
        case 0x1ef08U:
        case 0x1ef0cU:
        case 0x1ef10U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
