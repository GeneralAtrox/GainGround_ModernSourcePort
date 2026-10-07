
FunctionResult cpu_b_stage_gap_0209fe(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x209feU: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x20a04U;
            const auto v1 = m.byte((r.address[5] + 0x41U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x20a04U: { // 6728 beq.b $20a2e
            next = 0x20a06U;
            if ((r.status & 4U) != 0U) { next = 0x20a2eU; transfer_kind = 1U; }
            break;
        }
        case 0x20a06U: { // 4a2d003f tst.b $3f(a5)
            next = 0x20a0aU;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x20a0aU: { // 6708 beq.b $20a14
            next = 0x20a0cU;
            if ((r.status & 4U) != 0U) { next = 0x20a14U; transfer_kind = 1U; }
            break;
        }
        case 0x20a0cU: { // 4df83400 lea.l $3400.w, a6
            next = 0x20a10U;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x20a10U: { // 522e0024 addq.b #$1, $24(a6)
            next = 0x20a14U;
            const auto v1 = m.add(m.byte((r.address[6] + 0x24U)), 0x1U, 8U);
            m.byte((r.address[6] + 0x24U), v1);
            break;
        }
        case 0x20a14U: { // 6100c4e0 bsr.w $1cef6
            next = 0x20a18U;
            const auto result = m.call(c, 334U, 0x20a14U, 0x1cef6U, 0x20a18U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20a18U: { // 6100d1aa bsr.w $1dbc4
            next = 0x20a1cU;
            const auto result = m.call(c, 352U, 0x20a18U, 0x1dbc4U, 0x20a1cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20a1cU: { // 6100d1d2 bsr.w $1dbf0
            next = 0x20a20U;
            const auto result = m.call(c, 353U, 0x20a1cU, 0x1dbf0U, 0x20a20U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20a20U: { // 6100d702 bsr.w $1e124
            next = 0x20a24U;
            const auto result = m.call(c, 357U, 0x20a20U, 0x1e124U, 0x20a24U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20a24U: { // 6100d8de bsr.w $1e304
            next = 0x20a28U;
            const auto result = m.call(c, 360U, 0x20a24U, 0x1e304U, 0x20a28U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20a28U: { // 6100e2ba bsr.w $1ece4
            next = 0x20a2cU;
            const auto result = m.call(c, 365U, 0x20a28U, 0x1ece4U, 0x20a2cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20a2cU: { // 4e75 rts 
            next = 0x20a2eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20a2cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x20a2eU: { // 4a2d003f tst.b $3f(a5)
            next = 0x20a32U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x20a32U: { // 6708 beq.b $20a3c
            next = 0x20a34U;
            if ((r.status & 4U) != 0U) { next = 0x20a3cU; transfer_kind = 1U; }
            break;
        }
        case 0x20a34U: { // 4df83400 lea.l $3400.w, a6
            next = 0x20a38U;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x20a38U: { // 522e0024 addq.b #$1, $24(a6)
            next = 0x20a3cU;
            const auto v1 = m.add(m.byte((r.address[6] + 0x24U)), 0x1U, 8U);
            m.byte((r.address[6] + 0x24U), v1);
            break;
        }
        case 0x20a3cU: { // 6100b6ca bsr.w $1c108
            next = 0x20a40U;
            const auto result = m.call(c, 329U, 0x20a3cU, 0x1c108U, 0x20a40U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20a40U: { // 6100e59a bsr.w $1efdc
            next = 0x20a44U;
            const auto result = m.call(c, 368U, 0x20a40U, 0x1efdcU, 0x20a44U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20a44U: { // 6100d17e bsr.w $1dbc4
            next = 0x20a48U;
            const auto result = m.call(c, 352U, 0x20a44U, 0x1dbc4U, 0x20a48U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20a48U: { // 6100d1a6 bsr.w $1dbf0
            next = 0x20a4cU;
            const auto result = m.call(c, 353U, 0x20a48U, 0x1dbf0U, 0x20a4cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20a4cU: { // 6100d3e2 bsr.w $1de30
            next = 0x20a50U;
            const auto result = m.call(c, 355U, 0x20a4cU, 0x1de30U, 0x20a50U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20a50U: { // 6100d69c bsr.w $1e0ee
            next = 0x20a54U;
            const auto result = m.call(c, 356U, 0x20a50U, 0x1e0eeU, 0x20a54U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20a54U: { // 6100d6ce bsr.w $1e124
            next = 0x20a58U;
            const auto result = m.call(c, 357U, 0x20a54U, 0x1e124U, 0x20a58U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20a58U: { // 6100d8aa bsr.w $1e304
            next = 0x20a5cU;
            const auto result = m.call(c, 360U, 0x20a58U, 0x1e304U, 0x20a5cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20a5cU: { // 4e75 rts 
            next = 0x20a5eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20a5cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x209feU:
        case 0x20a04U:
        case 0x20a06U:
        case 0x20a0aU:
        case 0x20a0cU:
        case 0x20a10U:
        case 0x20a14U:
        case 0x20a18U:
        case 0x20a1cU:
        case 0x20a20U:
        case 0x20a24U:
        case 0x20a28U:
        case 0x20a2cU:
        case 0x20a2eU:
        case 0x20a32U:
        case 0x20a34U:
        case 0x20a38U:
        case 0x20a3cU:
        case 0x20a40U:
        case 0x20a44U:
        case 0x20a48U:
        case 0x20a4cU:
        case 0x20a50U:
        case 0x20a54U:
        case 0x20a58U:
        case 0x20a5cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
