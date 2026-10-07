
static bool cpu_b_stage_gap_01e75a_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1e7beU: { // 9242 sub.w d2, d1
            next = 0x1e7c0U;
            const auto v1 = m.sub((r.data[1] & 0xffffU), (r.data[2] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e7c0U: { // d242 add.w d2, d1
            next = 0x1e7c2U;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[2] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e7c2U: { // 51c8fffc dbra d0, $1e7c0
            next = 0x1e7c6U;
            const auto v1 = static_cast<std::uint16_t>(r.data[0] - 1U); m.dw(0, v1);
            if (v1 != 0xffffU) { next = 0x1e7c0U; transfer_kind = 1U; }
            break;
        }
        case 0x1e7c6U: { // 102d0036 move.b $36(a5), d0
            next = 0x1e7caU;
            const auto v1 = m.byte((r.address[5] + 0x36U));
            m.db(0, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1e7caU: { // 3400 move.w d0, d2
            next = 0x1e7ccU;
            const auto v1 = (r.data[0] & 0xffffU);
            m.dw(2, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e7ccU: { // e540 asl.w #$2, d0
            next = 0x1e7ceU;
            m.asl_word(0, 2U);
            break;
        }
        case 0x1e7ceU: { // e342 asl.w #$1, d2
            next = 0x1e7d0U;
            m.asl_word(2, 1U);
            break;
        }
        case 0x1e7d0U: { // d042 add.w d2, d0
            next = 0x1e7d2U;
            const auto v1 = m.add((r.data[0] & 0xffffU), (r.data[2] & 0xffffU), 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1e7d2U: { // 342d0056 move.w $56(a5), d2
            next = 0x1e7d6U;
            const auto v1 = m.word((r.address[5] + 0x56U));
            m.dw(2, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e7d6U: { // d440 add.w d0, d2
            next = 0x1e7d8U;
            const auto v1 = m.add((r.data[2] & 0xffffU), (r.data[0] & 0xffffU), 16U);
            m.dw(2, v1);
            break;
        }
        case 0x1e7d8U: { // 024207ff andi.w #$7ff, d2
            next = 0x1e7dcU;
            const auto v1 = ((r.data[2] & 0xffffU) & 0x7ffU) & 0xffffU;
            m.dw(2, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e7dcU: { // 3b420056 move.w d2, $56(a5)
            next = 0x1e7e0U;
            const auto v1 = (r.data[2] & 0xffffU);
            m.word((r.address[5] + 0x56U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e7e0U: { // 02420600 andi.w #$600, d2
            next = 0x1e7e4U;
            const auto v1 = ((r.data[2] & 0xffffU) & 0x600U) & 0xffffU;
            m.dw(2, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e7e4U: { // ef5a rol.w #$7, d2
            next = 0x1e7e6U;
            const auto v1 = ((r.data[2] << 7U) | ((r.data[2] & 0xffffU) >> 9U)) & 0xffffU;
            m.dw(2, v1); m.logic(v1, 16U); r.status = static_cast<std::uint16_t>((r.status & ~1U) | (v1 & 1U));
            break;
        }
        case 0x1e7e6U: { // 0c420003 cmpi.w #$3, d2
            next = 0x1e7eaU;
            (void)m.sub((r.data[2] & 0xffffU), 0x3U, 16U, true);
            break;
        }
        case 0x1e7eaU: { // 6602 bne.b $1e7ee
            next = 0x1e7ecU;
            if ((r.status & 4U) == 0U) { next = 0x1e7eeU; transfer_kind = 1U; }
            break;
        }
        case 0x1e7ecU: { // 7401 moveq #$1, d2
            next = 0x1e7eeU;
            r.data[2] = 0x1U; m.logic(r.data[2], 32U);
            break;
        }
        case 0x1e7eeU: { // 9247 sub.w d7, d1
            next = 0x1e7f0U;
            const auto v1 = m.sub((r.data[1] & 0xffffU), (r.data[7] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e7f0U: { // d247 add.w d7, d1
            next = 0x1e7f2U;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[7] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e7f2U: { // 51cafffc dbra d2, $1e7f0
            next = 0x1e7f6U;
            const auto v1 = static_cast<std::uint16_t>(r.data[2] - 1U); m.dw(2, v1);
            if (v1 != 0xffffU) { next = 0x1e7f0U; transfer_kind = 1U; }
            break;
        }
        case 0x1e7f6U: { // 6000f9fa bra.w $1e1f2
            next = 0x1e7faU;
            next = 0x1e1f2U; transfer_kind = 1U;
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_gap_01e75a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1e220U: { // 6100ff36 bsr.w $1e158
            next = 0x1e224U;
            const auto result = m.call(c, 358U, 0x1e220U, 0x1e158U, 0x1e224U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e22aU: { // 4eb900015d24 jsr $15d24.l
            next = 0x1e230U;
            const auto result = m.indirect_call(c, 0x1e22aU, 0x15d24U, 0x1e230U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e230U: { // 4eb900015df2 jsr $15df2.l
            next = 0x1e236U;
            const auto result = m.indirect_call(c, 0x1e230U, 0x15df2U, 0x1e236U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e236U: { // 4e75 rts 
            next = 0x1e238U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1e236U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1e75aU: { // 61000494 bsr.w $1ebf0
            next = 0x1e75eU;
            const auto result = m.call(c, 363U, 0x1e75aU, 0x1ebf0U, 0x1e75eU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e794U: { // 6100f9c2 bsr.w $1e158
            next = 0x1e798U;
            const auto result = m.call(c, 358U, 0x1e794U, 0x1e158U, 0x1e798U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e798U: { // 6100054a bsr.w $1ece4
            next = 0x1e79cU;
            const auto result = m.call(c, 365U, 0x1e798U, 0x1ece4U, 0x1e79cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e79cU: { // 4eb900015d24 jsr $15d24.l
            next = 0x1e7a2U;
            const auto result = m.indirect_call(c, 0x1e79cU, 0x15d24U, 0x1e7a2U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e7a2U: { // 4eb900015df2 jsr $15df2.l
            next = 0x1e7a8U;
            const auto result = m.indirect_call(c, 0x1e7a2U, 0x15df2U, 0x1e7a8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e7a8U: { // 4e75 rts 
            next = 0x1e7aaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1e7a8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_gap_01e75a_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_gap_01e75a_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_gap_01e75a_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

static bool cpu_b_stage_gap_01e7fa_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1e1f2U:
    case 0x1e1f6U:
    case 0x1e1f8U:
    case 0x1e1fcU:
    case 0x1e200U:
    case 0x1e204U:
    case 0x1e208U:
    case 0x1e20cU:
    case 0x1e20eU:
    case 0x1e212U:
    case 0x1e216U:
    case 0x1e21aU:
    case 0x1e21cU:
    case 0x1e220U:
    case 0x1e224U:
    case 0x1e22aU:
    case 0x1e230U:
    case 0x1e236U:
    case 0x1e7faU:
    case 0x1e7feU:
    case 0x1e800U:
    case 0x1e802U:
    case 0x1e804U:
    case 0x1e806U:
    case 0x1e808U:
    case 0x1e80eU:
    case 0x1e810U:
    case 0x1e812U:
    case 0x1e814U:
    case 0x1e816U:
    case 0x1e81aU:
    case 0x1e81cU:
    case 0x1e81eU:
    case 0x1e820U:
    case 0x1e822U:
    case 0x1e826U:
    case 0x1e828U:
    case 0x1e82cU:
    case 0x1e830U:
    case 0x1e832U:
    case 0x1e834U:
    case 0x1e836U:
    case 0x1e838U:
    case 0x1e83cU:
    case 0x1e840U:
    case 0x1e844U:
    case 0x1e848U:
    case 0x1e84aU:
    case 0x1e84cU:
    case 0x1e84eU:
    case 0x1e852U:
    case 0x1e858U:
    case 0x1e85aU:
    case 0x1e860U:
    case 0x1e862U:
    case 0x1e868U:
    case 0x1e86aU:
    case 0x1e86eU:
    case 0x1e872U:
    case 0x1e876U:
    case 0x1e878U:
    case 0x1e87aU:
    case 0x1e87cU:
    case 0x1e880U:
    case 0x1e884U:
    case 0x1e886U:
    case 0x1e888U:
    case 0x1e88aU:
    case 0x1e88cU:
    case 0x1e88eU:
    case 0x1e892U:
    case 0x1e896U:
    case 0x1e89aU:
    case 0x1e89cU:
    case 0x1e89eU:
    case 0x1e8a0U:
    case 0x1e8a2U:
    case 0x1e8a4U:
    case 0x1e8a8U:
    case 0x1e8aeU:
    case 0x1e8b2U:
    case 0x1e8b4U:
    case 0x1e8b8U:
    case 0x1e8baU:
    case 0x1e8bcU:
    case 0x1e8beU:
    case 0x1e8c0U:
    case 0x1e8c2U:
    case 0x1e8c4U:
    case 0x1e8c6U:
    case 0x1e8c8U:
    case 0x1e8ccU:
    case 0x1e8ceU:
    case 0x1e8d0U:
    case 0x1e8d2U:
    case 0x1e8d6U:
        return true;
    default: return false;
    }
}
