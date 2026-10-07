
FunctionResult cpu_b_stage_01a32c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1a32cU: { // 4a2d003f tst.b $3f(a5)
            next = 0x1a330U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1a330U: { // 6600004a bne.w $1a37c
            next = 0x1a334U;
            if ((r.status & 4U) == 0U) { next = 0x1a37cU; transfer_kind = 1U; }
            break;
        }
        case 0x1a334U: { // 61000380 bsr.w $1a6b6
            next = 0x1a338U;
            const auto result = m.call(c, 940U, 0x1a334U, 0x1a6b6U, 0x1a338U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a338U: { // 65000042 bcs.w $1a37c
            next = 0x1a33cU;
            if ((r.status & 1U) != 0U) { next = 0x1a37cU; transfer_kind = 1U; }
            break;
        }
        case 0x1a33cU: { // 61000488 bsr.w $1a7c6
            next = 0x1a340U;
            const auto result = m.call(c, 942U, 0x1a33cU, 0x1a7c6U, 0x1a340U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a340U: { // 6500003a bcs.w $1a37c
            next = 0x1a344U;
            if ((r.status & 1U) != 0U) { next = 0x1a37cU; transfer_kind = 1U; }
            break;
        }
        case 0x1a344U: { // 536d0066 subq.w #$1, $66(a5)
            next = 0x1a348U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1a348U: { // 6f000032 ble.w $1a37c
            next = 0x1a34cU;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1a37cU; transfer_kind = 1U; }
            break;
        }
        case 0x1a34cU: { // 202d001e move.l $1e(a5), d0
            next = 0x1a350U;
            const auto source_address = r.address[5] + 0x1eU;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1a350U: { // d1ad0012 add.l d0, $12(a5)
            next = 0x1a354U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x12U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x1a354U: { // 202d0026 move.l $26(a5), d0
            next = 0x1a358U;
            const auto source_address = r.address[5] + 0x26U;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1a358U: { // d1ad001a add.l d0, $1a(a5)
            next = 0x1a35cU;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x1a35cU: { // 526d0008 addq.w #$1, $8(a5)
            next = 0x1a360U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x8U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1a360U: { // 0c6d0c9a0008 cmpi.w #$c9a, $8(a5)
            next = 0x1a366U;
            const auto source_value = 0xc9aU;
            const auto destination_address = r.address[5] + 0x8U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a366U: { // 6f06 ble.b $1a36e
            next = 0x1a368U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1a36eU; transfer_kind = 1U; }
            break;
        }
        case 0x1a368U: { // 3b7c0c980008 move.w #$c98, $8(a5)
            next = 0x1a36eU;
            const auto value = 0xc98U;
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a36eU: { // 4eb900015d24 jsr $15d24.l
            next = 0x1a374U;
            const auto result = m.call(c, 280U, 0x1a36eU, 0x15d24U, 0x1a374U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a374U: { // 4eb900015df2 jsr $15df2.l
            next = 0x1a37aU;
            const auto result = m.call(c, 282U, 0x1a374U, 0x15df2U, 0x1a37aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a37aU: { // 4e75 rts 
            next = 0x1a37cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a37aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1a37cU: { // 08ed00020001 bset.b #$2, $1(a5)
            next = 0x1a382U;
            const auto destination_address = r.address[5] + 0x1U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1a382U: { // 2b7c0001a3960002 move.l #$1a396, $2(a5)
            next = 0x1a38aU;
            const auto value = 0x1a396U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1a38aU: { // 4aed003f tas.b $3f(a5)
            next = 0x1a38eU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0x1a38eU: { // 3b7c37370010 move.w #$3737, $10(a5)
            next = 0x1a394U;
            const auto value = 0x3737U;
            const auto destination_address = r.address[5] + 0x10U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a394U: { // 600c bra.b $1a3a2
            next = 0x1a396U;
            if (true) { next = 0x1a3a2U; transfer_kind = 1U; }
            break;
        }
        case 0x1a3a2U: { // 08950000 bclr.b #$0, (a5)
            next = 0x1a3a6U;
            const auto destination_address = r.address[5];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1a3a6U: { // 4eb900015d24 jsr $15d24.l
            next = 0x1a3acU;
            const auto result = m.call(c, 280U, 0x1a3a6U, 0x15d24U, 0x1a3acU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a3acU: { // 4eb900015df2 jsr $15df2.l
            next = 0x1a3b2U;
            const auto result = m.call(c, 282U, 0x1a3acU, 0x15df2U, 0x1a3b2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a3b2U: { // 4e75 rts 
            next = 0x1a3b4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a3b2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1a32cU:
        case 0x1a330U:
        case 0x1a334U:
        case 0x1a338U:
        case 0x1a33cU:
        case 0x1a340U:
        case 0x1a344U:
        case 0x1a348U:
        case 0x1a34cU:
        case 0x1a350U:
        case 0x1a354U:
        case 0x1a358U:
        case 0x1a35cU:
        case 0x1a360U:
        case 0x1a366U:
        case 0x1a368U:
        case 0x1a36eU:
        case 0x1a374U:
        case 0x1a37aU:
        case 0x1a37cU:
        case 0x1a382U:
        case 0x1a38aU:
        case 0x1a38eU:
        case 0x1a394U:
        case 0x1a3a2U:
        case 0x1a3a6U:
        case 0x1a3acU:
        case 0x1a3b2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
