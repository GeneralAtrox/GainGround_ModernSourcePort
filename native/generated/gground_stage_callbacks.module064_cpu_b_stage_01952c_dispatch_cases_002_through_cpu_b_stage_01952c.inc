
static bool cpu_b_stage_01952c_dispatch_cases_002([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19670U: { // 426d0064 clr.w $64(a5)
            next = 0x19674U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19674U: { // 3b7c00040068 move.w #$4, $68(a5)
            next = 0x1967aU;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1967cU: { // 303c0081 move.w #$81, d0
            next = 0x19680U;
            const auto value = 0x81U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19680U: { // 082d0000006b btst.b #$0, $6b(a5)
            next = 0x19686U;
            const auto destination_address = r.address[5] + 0x6bU;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x19686U: { // 6702 beq.b $1968a
            next = 0x19688U;
            if ((r.status & 4U) != 0U) { next = 0x1968aU; transfer_kind = 1U; }
            break;
        }
        case 0x19688U: { // 4440 neg.w d0
            next = 0x1968aU;
            const auto old = r.data[0];
            const auto value = m.sub(0U, old, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1968aU: { // d16d0056 add.w d0, $56(a5)
            next = 0x1968eU;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x56U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1968eU: { // 026d03ff0056 andi.w #$3ff, $56(a5)
            next = 0x19694U;
            const auto source_value = 0x3ffU;
            const auto destination_address = r.address[5] + 0x56U;
            const auto destination_value = m.word(destination_address);
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19694U: { // 3b7c00060048 move.w #$6, $48(a5)
            next = 0x1969aU;
            const auto value = 0x6U;
            const auto destination_address = r.address[5] + 0x48U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1969aU: { // 3b7c00010040 move.w #$1, $40(a5)
            next = 0x196a0U;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x40U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x196a0U: { // 1b7c00060051 move.b #$6, $51(a5)
            next = 0x196a6U;
            const auto value = 0x6U;
            const auto destination_address = r.address[5] + 0x51U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x196a6U: { // 422d0050 clr.b $50(a5)
            next = 0x196aaU;
            const auto destination_address = r.address[5] + 0x50U;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x196acU: { // 3b7c00070048 move.w #$7, $48(a5)
            next = 0x196b2U;
            const auto value = 0x7U;
            const auto destination_address = r.address[5] + 0x48U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x196b2U: { // 3b7c00020040 move.w #$2, $40(a5)
            next = 0x196b8U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x40U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x196b8U: { // 426d0064 clr.w $64(a5)
            next = 0x196bcU;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x196bcU: { // 3b7c00040068 move.w #$4, $68(a5)
            next = 0x196c2U;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x196c2U: { // 2b7c000460000022 move.l #$46000, $22(a5)
            next = 0x196caU;
            const auto value = 0x46000U;
            const auto destination_address = r.address[5] + 0x22U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x196caU: { // 2b7cfffe00000026 move.l #$fffe0000, $26(a5)
            next = 0x196d2U;
            const auto value = 0xfffe0000U;
            const auto destination_address = r.address[5] + 0x26U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x196d4U: { // 426d0048 clr.w $48(a5)
            next = 0x196d8U;
            const auto destination_address = r.address[5] + 0x48U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x196d8U: { // 3b7c00030040 move.w #$3, $40(a5)
            next = 0x196deU;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x40U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x196deU: { // 426d0064 clr.w $64(a5)
            next = 0x196e2U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x196e2U: { // 3b7c00030068 move.w #$3, $68(a5)
            next = 0x196e8U;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x196e8U: { // 3b7c00030054 move.w #$3, $54(a5)
            next = 0x196eeU;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x196eeU: { // 3b7c0058001a move.w #$58, $1a(a5)
            next = 0x196f4U;
            const auto value = 0x58U;
            const auto destination_address = r.address[5] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01952c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1958aU: { // 4e75 rts 
            next = 0x1958cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1958aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x195beU: { // 4eb90001631a jsr $1631a.l
            next = 0x195c4U;
            const auto result = m.call(c, 304U, 0x195beU, 0x1631aU, 0x195c4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x195c4U: { // 4eb9000163ba jsr $163ba.l
            next = 0x195caU;
            const auto result = m.call(c, 867U, 0x195c4U, 0x163baU, 0x195caU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x195feU: { // 4e75 rts 
            next = 0x19600U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x195feU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19614U: { // 4e75 rts 
            next = 0x19616U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19614U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1962cU: { // 4e75 rts 
            next = 0x1962eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1962cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1964aU: { // 4e75 rts 
            next = 0x1964cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1964aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19662U: { // 4e75 rts 
            next = 0x19664U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19662U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1967aU: { // 4e75 rts 
            next = 0x1967cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1967aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x196aaU: { // 4e75 rts 
            next = 0x196acU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x196aaU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x196d2U: { // 4e75 rts 
            next = 0x196d4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x196d2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x196f4U: { // 4e75 rts 
            next = 0x196f6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x196f4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01952c_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01952c_dispatch_cases_001(m, c, pc, next, transfer_kind) || cpu_b_stage_01952c_dispatch_cases_002(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01952c_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}
