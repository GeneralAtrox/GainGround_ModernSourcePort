
FunctionResult cpu_b_stage_01891c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1891cU: { // 536d0070 subq.w #$1, $70(a5)
            next = 0x18920U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x70U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18920U: { // 086d00000068 bchg.b #$0, $68(a5)
            next = 0x18926U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old ^ bit_mask);
            break;
        }
        case 0x18926U: { // 6636 bne.b $1895e
            next = 0x18928U;
            if ((r.status & 4U) == 0U) { next = 0x1895eU; transfer_kind = 1U; }
            break;
        }
        case 0x18928U: { // 302d0064 move.w $64(a5), d0
            next = 0x1892cU;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1892cU: { // 5240 addq.w #$1, d0
            next = 0x1892eU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1892eU: { // b06d006e cmp.w $6e(a5), d0
            next = 0x18932U;
            const auto source_address = r.address[5] + 0x6eU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18932U: { // 6b0c bmi.b $18940
            next = 0x18934U;
            if ((r.status & 8U) != 0U) { next = 0x18940U; transfer_kind = 1U; }
            break;
        }
        case 0x18934U: { // 7000 moveq #$0, d0
            next = 0x18936U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x18936U: { // 4a6d0070 tst.w $70(a5)
            next = 0x1893aU;
            const auto destination_address = r.address[5] + 0x70U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x1893aU: { // 6e04 bgt.b $18940
            next = 0x1893cU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18940U; transfer_kind = 1U; }
            break;
        }
        case 0x1893cU: { // 4255 clr.w (a5)
            next = 0x1893eU;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1893eU: { // 4e75 rts 
            next = 0x18940U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1893eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x18940U: { // 3b400064 move.w d0, $64(a5)
            next = 0x18944U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x64U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18944U: { // d040 add.w d0, d0
            next = 0x18946U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18946U: { // 3200 move.w d0, d1
            next = 0x18948U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18948U: { // d040 add.w d0, d0
            next = 0x1894aU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1894aU: { // d041 add.w d1, d0
            next = 0x1894cU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1894cU: { // 41fa2374 lea.l $1acc2(pc), a0
            next = 0x18950U;
            const auto source_address = 0x1acc2U;
            r.address[0] = source_address;
            break;
        }
        case 0x18950U: { // d0c0 adda.w d0, a0
            next = 0x18952U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x18952U: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x18956U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18956U: { // 3b580008 move.w (a0)+, $8(a5)
            next = 0x1895aU;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1895aU: { // 1b580001 move.b (a0)+, $1(a5)
            next = 0x1895eU;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1895eU: { // 4eb900015df2 jsr $15df2.l
            next = 0x18964U;
            const auto result = m.call(c, 282U, 0x1895eU, 0x15df2U, 0x18964U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18964U: { // 4e75 rts 
            next = 0x18966U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18964U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1891cU:
        case 0x18920U:
        case 0x18926U:
        case 0x18928U:
        case 0x1892cU:
        case 0x1892eU:
        case 0x18932U:
        case 0x18934U:
        case 0x18936U:
        case 0x1893aU:
        case 0x1893cU:
        case 0x1893eU:
        case 0x18940U:
        case 0x18944U:
        case 0x18946U:
        case 0x18948U:
        case 0x1894aU:
        case 0x1894cU:
        case 0x18950U:
        case 0x18952U:
        case 0x18956U:
        case 0x1895aU:
        case 0x1895eU:
        case 0x18964U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
