
static bool cpu_b_stage_01902e_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1902eU: { // 42ad001e clr.l $1e(a5)
            next = 0x19032U;
            const auto destination_address = r.address[5] + 0x1eU;
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0x19032U: { // 42ad0026 clr.l $26(a5)
            next = 0x19036U;
            const auto destination_address = r.address[5] + 0x26U;
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0x19036U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1903aU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1903aU: { // 6702 beq.b $1903e
            next = 0x1903cU;
            if ((r.status & 4U) != 0U) { next = 0x1903eU; transfer_kind = 1U; }
            break;
        }
        case 0x1903eU: { // 342d005a move.w $5a(a5), d2
            next = 0x19042U;
            const auto source_address = r.address[5] + 0x5aU;
            const auto value = m.word(source_address);
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19042U: { // 223c00016a0a move.l #$16a0a, d1
            next = 0x19048U;
            const auto value = 0x16a0aU;
            r.data[1] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x19048U: { // 302d0048 move.w $48(a5), d0
            next = 0x1904cU;
            const auto source_address = r.address[5] + 0x48U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1904cU: { // e540 asl.w #$2, d0
            next = 0x1904eU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x1904eU: { // 4efb0002 jmp $19052(pc, d0.w)
            next = 0x19052U;
            next = (0x19052U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x19052U: { // 60000042 bra.w $19096
            next = 0x19056U;
            if (true) { next = 0x19096U; transfer_kind = 1U; }
            break;
        }
        case 0x19056U: { // 60000012 bra.w $1906a
            next = 0x1905aU;
            if (true) { next = 0x1906aU; transfer_kind = 1U; }
            break;
        }
        case 0x1905aU: { // 6000003a bra.w $19096
            next = 0x1905eU;
            if (true) { next = 0x19096U; transfer_kind = 1U; }
            break;
        }
        case 0x1905eU: { // 60000030 bra.w $19090
            next = 0x19062U;
            if (true) { next = 0x19090U; transfer_kind = 1U; }
            break;
        }
        case 0x19062U: { // 60000006 bra.w $1906a
            next = 0x19066U;
            if (true) { next = 0x1906aU; transfer_kind = 1U; }
            break;
        }
        case 0x19066U: { // 60000028 bra.w $19090
            next = 0x1906aU;
            if (true) { next = 0x19090U; transfer_kind = 1U; }
            break;
        }
        case 0x1906aU: { // 5342 subq.w #$1, d2
            next = 0x1906cU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[2];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1906cU: { // 660a bne.b $19078
            next = 0x1906eU;
            if ((r.status & 4U) == 0U) { next = 0x19078U; transfer_kind = 1U; }
            break;
        }
        case 0x1906eU: { // 2b41001e move.l d1, $1e(a5)
            next = 0x19072U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x1eU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x19072U: { // 2b410026 move.l d1, $26(a5)
            next = 0x19076U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x26U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x19078U: { // 5342 subq.w #$1, d2
            next = 0x1907aU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[2];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1907aU: { // 6608 bne.b $19084
            next = 0x1907cU;
            if ((r.status & 4U) == 0U) { next = 0x19084U; transfer_kind = 1U; }
            break;
        }
        case 0x1907cU: { // 3b7c00020026 move.w #$2, $26(a5)
            next = 0x19082U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x26U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19084U: { // 2b410026 move.l d1, $26(a5)
            next = 0x19088U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x26U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x19088U: { // 4481 neg.l d1
            next = 0x1908aU;
            const auto old = r.data[1];
            const auto value = m.sub(0U, old, 32U);
            r.data[1] = value;
            break;
        }
        case 0x1908aU: { // 2b41001e move.l d1, $1e(a5)
            next = 0x1908eU;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x1eU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x19090U: { // 3b7cfffe0026 move.w #$fffe, $26(a5)
            next = 0x19096U;
            const auto value = 0xfffeU;
            const auto destination_address = r.address[5] + 0x26U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01902e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1903cU: { // 4e75 rts 
            next = 0x1903eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1903cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19076U: { // 4e75 rts 
            next = 0x19078U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19076U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19082U: { // 4e75 rts 
            next = 0x19084U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19082U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1908eU: { // 4e75 rts 
            next = 0x19090U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1908eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19096U: { // 4e75 rts 
            next = 0x19098U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19096U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01902e_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1902eU:
        case 0x19032U:
        case 0x19036U:
        case 0x1903aU:
        case 0x1903cU:
        case 0x1903eU:
        case 0x19042U:
        case 0x19048U:
        case 0x1904cU:
        case 0x1904eU:
        case 0x19052U:
        case 0x19056U:
        case 0x1905aU:
        case 0x1905eU:
        case 0x19062U:
        case 0x19066U:
        case 0x1906aU:
        case 0x1906cU:
        case 0x1906eU:
        case 0x19072U:
        case 0x19076U:
        case 0x19078U:
        case 0x1907aU:
        case 0x1907cU:
        case 0x19082U:
        case 0x19084U:
        case 0x19088U:
        case 0x1908aU:
        case 0x1908eU:
        case 0x19090U:
        case 0x19096U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
