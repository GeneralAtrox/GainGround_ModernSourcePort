
FunctionResult cpu_b_stage_018542(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18542U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x18546U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18546U: { // 6e4a bgt.b $18592
            next = 0x18548U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18592U; transfer_kind = 1U; }
            break;
        }
        case 0x18548U: { // 303c0014 move.w #$14, d0
            next = 0x1854cU;
            const auto value = 0x14U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1854cU: { // 4eb900016ff8 jsr $16ff8.l
            next = 0x18552U;
            const auto result = m.call(c, 308U, 0x1854cU, 0x16ff8U, 0x18552U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18552U: { // 3b7c00040068 move.w #$4, $68(a5)
            next = 0x18558U;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18558U: { // 302d0066 move.w $66(a5), d0
            next = 0x1855cU;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1855cU: { // 6a20 bpl.b $1857e
            next = 0x1855eU;
            if ((r.status & 8U) == 0U) { next = 0x1857eU; transfer_kind = 1U; }
            break;
        }
        case 0x1855eU: { // 302d0064 move.w $64(a5), d0
            next = 0x18562U;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18562U: { // 5840 addq.w #$4, d0
            next = 0x18564U;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18564U: { // 0240001c andi.w #$1c, d0
            next = 0x18568U;
            const auto source_value = 0x1cU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18568U: { // 3b400064 move.w d0, $64(a5)
            next = 0x1856cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x64U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1856cU: { // 670a beq.b $18578
            next = 0x1856eU;
            if ((r.status & 4U) != 0U) { next = 0x18578U; transfer_kind = 1U; }
            break;
        }
        case 0x1856eU: { // 0c400010 cmpi.w #$10, d0
            next = 0x18572U;
            const auto source_value = 0x10U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18572U: { // 661e bne.b $18592
            next = 0x18574U;
            if ((r.status & 4U) == 0U) { next = 0x18592U; transfer_kind = 1U; }
            break;
        }
        case 0x18574U: { // 303c0008 move.w #$8, d0
            next = 0x18578U;
            const auto value = 0x8U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18578U: { // 3b400066 move.w d0, $66(a5)
            next = 0x1857cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x66U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1857cU: { // 6014 bra.b $18592
            next = 0x1857eU;
            if (true) { next = 0x18592U; transfer_kind = 1U; }
            break;
        }
        case 0x1857eU: { // 5440 addq.w #$2, d0
            next = 0x18580U;
            const auto source_value = 0x2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18580U: { // 0240000e andi.w #$e, d0
            next = 0x18584U;
            const auto source_value = 0xeU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18584U: { // 3b400066 move.w d0, $66(a5)
            next = 0x18588U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x66U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18588U: { // 41fa264a lea.l $1abd4(pc), a0
            next = 0x1858cU;
            const auto source_address = 0x1abd4U;
            r.address[0] = source_address;
            break;
        }
        case 0x1858cU: { // 3b7000000064 move.w (a0, d0.w), $64(a5)
            next = 0x18592U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x64U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18592U: { // 536d0070 subq.w #$1, $70(a5)
            next = 0x18596U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x70U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18596U: { // 302d0070 move.w $70(a5), d0
            next = 0x1859aU;
            const auto source_address = r.address[5] + 0x70U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1859aU: { // 0c40001e cmpi.w #$1e, d0
            next = 0x1859eU;
            const auto source_value = 0x1eU;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1859eU: { // 6e14 bgt.b $185b4
            next = 0x185a0U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x185b4U; transfer_kind = 1U; }
            break;
        }
        case 0x185a0U: { // 6606 bne.b $185a8
            next = 0x185a2U;
            if ((r.status & 4U) == 0U) { next = 0x185a8U; transfer_kind = 1U; }
            break;
        }
        case 0x185a2U: { // 61000338 bsr.w $188dc
            next = 0x185a6U;
            const auto result = m.call(c, 878U, 0x185a2U, 0x188dcU, 0x185a6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x185a6U: { // 4e75 rts 
            next = 0x185a8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x185a6U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x185a8U: { // 4a40 tst.w d0
            next = 0x185aaU;
            const auto value = r.data[0];
            m.logic(value, 16U);
            break;
        }
        case 0x185aaU: { // 6e06 bgt.b $185b2
            next = 0x185acU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x185b2U; transfer_kind = 1U; }
            break;
        }
        case 0x185acU: { // 4255 clr.w (a5)
            next = 0x185aeU;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x185aeU: { // 6100035a bsr.w $1890a
            next = 0x185b2U;
            const auto result = m.call(c, 880U, 0x185aeU, 0x1890aU, 0x185b2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x185b2U: { // 4e75 rts 
            next = 0x185b4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x185b2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x185b4U: { // 61000268 bsr.w $1881e
            next = 0x185b8U;
            const auto result = m.call(c, 875U, 0x185b4U, 0x1881eU, 0x185b8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x185b8U: { // 6100027a bsr.w $18834
            next = 0x185bcU;
            const auto result = m.call(c, 876U, 0x185b8U, 0x18834U, 0x185bcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x185bcU: { // 4eb900015df2 jsr $15df2.l
            next = 0x185c2U;
            const auto result = m.call(c, 282U, 0x185bcU, 0x15df2U, 0x185c2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x185c2U: { // 4e75 rts 
            next = 0x185c4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x185c2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18542U:
        case 0x18546U:
        case 0x18548U:
        case 0x1854cU:
        case 0x18552U:
        case 0x18558U:
        case 0x1855cU:
        case 0x1855eU:
        case 0x18562U:
        case 0x18564U:
        case 0x18568U:
        case 0x1856cU:
        case 0x1856eU:
        case 0x18572U:
        case 0x18574U:
        case 0x18578U:
        case 0x1857cU:
        case 0x1857eU:
        case 0x18580U:
        case 0x18584U:
        case 0x18588U:
        case 0x1858cU:
        case 0x18592U:
        case 0x18596U:
        case 0x1859aU:
        case 0x1859eU:
        case 0x185a0U:
        case 0x185a2U:
        case 0x185a6U:
        case 0x185a8U:
        case 0x185aaU:
        case 0x185acU:
        case 0x185aeU:
        case 0x185b2U:
        case 0x185b4U:
        case 0x185b8U:
        case 0x185bcU:
        case 0x185c2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
