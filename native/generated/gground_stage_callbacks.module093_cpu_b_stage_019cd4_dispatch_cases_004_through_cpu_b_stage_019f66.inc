
static bool cpu_b_stage_019cd4_dispatch_cases_004([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19f1cU: { // 3d40000c move.w d0, $c(a6)
            next = 0x19f20U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0xcU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19f20U: { // 302d000e move.w $e(a5), d0
            next = 0x19f24U;
            const auto source_address = r.address[5] + 0xeU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19f24U: { // d058 add.w (a0)+, d0
            next = 0x19f26U;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19f26U: { // 3d40000e move.w d0, $e(a6)
            next = 0x19f2aU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0xeU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19f2aU: { // 3d6d001a001a move.w $1a(a5), $1a(a6)
            next = 0x19f30U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[6] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19f30U: { // 51caffac dbra d2, $19ede
            next = 0x19f34U;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0x19edeU;
            break;
        }
        case 0x19f34U: { // 1d7c00040001 move.b #$4, $1(a6)
            next = 0x19f3aU;
            const auto value = 0x4U;
            const auto destination_address = r.address[6] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19f3cU: { // 536d004a subq.w #$1, $4a(a5)
            next = 0x19f40U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x4aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19f40U: { // 6f0e ble.b $19f50
            next = 0x19f42U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x19f50U; transfer_kind = 1U; }
            break;
        }
        case 0x19f42U: { // 0c6d001e004a cmpi.w #$1e, $4a(a5)
            next = 0x19f48U;
            const auto source_value = 0x1eU;
            const auto destination_address = r.address[5] + 0x4aU;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19f48U: { // 6a1a bpl.b $19f64
            next = 0x19f4aU;
            if ((r.status & 8U) == 0U) { next = 0x19f64U; transfer_kind = 1U; }
            break;
        }
        case 0x19f4aU: { // 4af80d2c tas.b $d2c.w
            next = 0x19f4eU;
            const auto destination_address = 0xd2cU;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0x19f50U: { // 426d0048 clr.w $48(a5)
            next = 0x19f54U;
            const auto destination_address = r.address[5] + 0x48U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19f54U: { // 3b7c0010004a move.w #$10, $4a(a5)
            next = 0x19f5aU;
            const auto value = 0x10U;
            const auto destination_address = r.address[5] + 0x4aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19f5aU: { // 426d0064 clr.w $64(a5)
            next = 0x19f5eU;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19f5eU: { // 3b7c00040068 move.w #$4, $68(a5)
            next = 0x19f64U;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_019cd4(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19d08U: { // 4e75 rts 
            next = 0x19d0aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19d08U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19d1eU: { // 4e75 rts 
            next = 0x19d20U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19d1eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19d38U: { // 4e75 rts 
            next = 0x19d3aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19d38U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19d58U: { // 4e75 rts 
            next = 0x19d5aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19d58U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19d66U: { // 6100ede2 bsr.w $18b4a
            next = 0x19d6aU;
            const auto result = m.call(c, 889U, 0x19d66U, 0x18b4aU, 0x19d6aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19de0U: { // 4e75 rts 
            next = 0x19de2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19de0U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19df8U: { // 4e75 rts 
            next = 0x19dfaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19df8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19e0aU: { // 4e75 rts 
            next = 0x19e0cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19e0aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19e38U: { // 6100ed10 bsr.w $18b4a
            next = 0x19e3cU;
            const auto result = m.call(c, 889U, 0x19e38U, 0x18b4aU, 0x19e3cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19edeU: { // 6100ec6a bsr.w $18b4a
            next = 0x19ee2U;
            const auto result = m.call(c, 889U, 0x19edeU, 0x18b4aU, 0x19ee2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19f3aU: { // 4e75 rts 
            next = 0x19f3cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19f3aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19f4eU: { // 4e75 rts 
            next = 0x19f50U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19f4eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19f64U: { // 4e75 rts 
            next = 0x19f66U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19f64U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_019cd4_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_019cd4_dispatch_cases_001(m, c, pc, next, transfer_kind) || cpu_b_stage_019cd4_dispatch_cases_002(m, c, pc, next, transfer_kind) || cpu_b_stage_019cd4_dispatch_cases_003(m, c, pc, next, transfer_kind) || cpu_b_stage_019cd4_dispatch_cases_004(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_019cd4_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

FunctionResult cpu_b_stage_019f66(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19f66U: { // 302d0048 move.w $48(a5), d0
            next = 0x19f6aU;
            const auto source_address = r.address[5] + 0x48U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19f6aU: { // e740 asl.w #$3, d0
            next = 0x19f6cU;
            m.asl_word(0U, 3U);
            break;
        }
        case 0x19f6cU: { // 41fa17a2 lea.l $1b710(pc), a0
            next = 0x19f70U;
            const auto source_address = 0x1b710U;
            r.address[0] = source_address;
            break;
        }
        case 0x19f70U: { // d0c0 adda.w d0, a0
            next = 0x19f72U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x19f72U: { // 302d0064 move.w $64(a5), d0
            next = 0x19f76U;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19f76U: { // 02400007 andi.w #$7, d0
            next = 0x19f7aU;
            const auto source_value = 0x7U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19f7aU: { // 1b7000000059 move.b (a0, d0.w), $59(a5)
            next = 0x19f80U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x59U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19f80U: { // 4e75 rts 
            next = 0x19f82U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19f80U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x19f66U:
        case 0x19f6aU:
        case 0x19f6cU:
        case 0x19f70U:
        case 0x19f72U:
        case 0x19f76U:
        case 0x19f7aU:
        case 0x19f80U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
