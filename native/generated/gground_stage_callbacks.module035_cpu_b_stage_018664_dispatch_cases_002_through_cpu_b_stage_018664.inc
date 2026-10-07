
static bool cpu_b_stage_018664_dispatch_cases_002([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x18760U: { // 1d7c0007000b move.b #$7, $b(a6)
            next = 0x18766U;
            const auto value = 0x7U;
            const auto destination_address = r.address[6] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18766U: { // 3d7c01010010 move.w #$101, $10(a6)
            next = 0x1876cU;
            const auto value = 0x101U;
            const auto destination_address = r.address[6] + 0x10U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1876cU: { // 422e003f clr.b $3f(a6)
            next = 0x18770U;
            const auto destination_address = r.address[6] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x18770U: { // 426e0064 clr.w $64(a6)
            next = 0x18774U;
            const auto destination_address = r.address[6] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x18774U: { // 2d40001e move.l d0, $1e(a6)
            next = 0x18778U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x1eU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x18778U: { // 2d420022 move.l d2, $22(a6)
            next = 0x1877cU;
            const auto value = r.data[2];
            const auto destination_address = r.address[6] + 0x22U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1877cU: { // 2d410026 move.l d1, $26(a6)
            next = 0x18780U;
            const auto value = r.data[1];
            const auto destination_address = r.address[6] + 0x26U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x18780U: { // 302d0012 move.w $12(a5), d0
            next = 0x18784U;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18784U: { // 3d400012 move.w d0, $12(a6)
            next = 0x18788U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18788U: { // 3d40002a move.w d0, $2a(a6)
            next = 0x1878cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1878cU: { // 3d40002c move.w d0, $2c(a6)
            next = 0x18790U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18790U: { // 302d0016 move.w $16(a5), d0
            next = 0x18794U;
            const auto source_address = r.address[5] + 0x16U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18794U: { // 3d400016 move.w d0, $16(a6)
            next = 0x18798U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18798U: { // 3d40002e move.w d0, $2e(a6)
            next = 0x1879cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2eU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1879cU: { // 3d400030 move.w d0, $30(a6)
            next = 0x187a0U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x30U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x187a0U: { // 302d001a move.w $1a(a5), d0
            next = 0x187a4U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x187a4U: { // 5840 addq.w #$4, d0
            next = 0x187a6U;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x187a6U: { // 3d40001a move.w d0, $1a(a6)
            next = 0x187aaU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x187aaU: { // 3d400032 move.w d0, $32(a6)
            next = 0x187aeU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x32U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x187aeU: { // 3d400034 move.w d0, $34(a6)
            next = 0x187b2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x34U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x187b2U: { // 422d0051 clr.b $51(a5)
            next = 0x187b6U;
            const auto destination_address = r.address[5] + 0x51U;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_018664(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x186d8U: { // 4eb90001631a jsr $1631a.l
            next = 0x186deU;
            const auto result = m.call(c, 304U, 0x186d8U, 0x1631aU, 0x186deU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x186deU: { // 4eb9000163ba jsr $163ba.l
            next = 0x186e4U;
            const auto result = m.call(c, 867U, 0x186deU, 0x163baU, 0x186e4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x186ecU: { // 4e75 rts 
            next = 0x186eeU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x186ecU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x186faU: { // 6100044e bsr.w $18b4a
            next = 0x186feU;
            const auto result = m.call(c, 889U, 0x186faU, 0x18b4aU, 0x186feU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18704U: { // 4eb900016372 jsr $16372.l
            next = 0x1870aU;
            const auto result = m.call(c, 306U, 0x18704U, 0x16372U, 0x1870aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18728U: { // 87c2 divs.w d2, d3
            next = 0x1872aU;
            const auto divisor = r.data[2];
            if ((divisor & 0xffffU) == 0U) {
                const auto result = m.exception(c, 5U, 0x18728U, 0x1872aU);
                if (result.status != TranslationStatus::complete || result.control != 2U) return result;
                next = r.program_counter;
            } else {
                m.divide(3U, divisor, true);
            }
            break;
        }
        case 0x18732U: { // 84c3 divu.w d3, d2
            next = 0x18734U;
            const auto divisor = r.data[3];
            if ((divisor & 0xffffU) == 0U) {
                const auto result = m.exception(c, 5U, 0x18732U, 0x18734U);
                if (result.status != TranslationStatus::complete || result.control != 2U) return result;
                next = r.program_counter;
            } else {
                m.divide(2U, divisor, false);
            }
            break;
        }
        case 0x187b6U: { // 4e75 rts 
            next = 0x187b8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x187b6U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_018664_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_018664_dispatch_cases_001(m, c, pc, next, transfer_kind) || cpu_b_stage_018664_dispatch_cases_002(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_018664_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}
