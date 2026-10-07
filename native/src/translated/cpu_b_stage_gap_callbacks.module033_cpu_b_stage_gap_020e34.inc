
FunctionResult cpu_b_stage_gap_020e34(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20e34U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x20e3aU;
            const auto v1 = m.byte((r.address[5] + 0x41U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x20e3aU: { // 6728 beq.b $20e64
            next = 0x20e3cU;
            if ((r.status & 4U) != 0U) { next = 0x20e64U; transfer_kind = 1U; }
            break;
        }
        case 0x20e3cU: { // 4a2d003f tst.b $3f(a5)
            next = 0x20e40U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x20e40U: { // 6708 beq.b $20e4a
            next = 0x20e42U;
            if ((r.status & 4U) != 0U) { next = 0x20e4aU; transfer_kind = 1U; }
            break;
        }
        case 0x20e42U: { // 4df83400 lea.l $3400.w, a6
            next = 0x20e46U;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x20e46U: { // 522e0022 addq.b #$1, $22(a6)
            next = 0x20e4aU;
            const auto v1 = m.add(m.byte((r.address[6] + 0x22U)), 0x1U, 8U);
            m.byte((r.address[6] + 0x22U), v1);
            break;
        }
        case 0x20e4aU: { // 6100c0aa bsr.w $1cef6
            next = 0x20e4eU;
            const auto result = m.call(c, 334U, 0x20e4aU, 0x1cef6U, 0x20e4eU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e4eU: { // 6100cd74 bsr.w $1dbc4
            next = 0x20e52U;
            const auto result = m.call(c, 352U, 0x20e4eU, 0x1dbc4U, 0x20e52U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e52U: { // 6100cd9c bsr.w $1dbf0
            next = 0x20e56U;
            const auto result = m.call(c, 353U, 0x20e52U, 0x1dbf0U, 0x20e56U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e56U: { // 6100d2cc bsr.w $1e124
            next = 0x20e5aU;
            const auto result = m.call(c, 357U, 0x20e56U, 0x1e124U, 0x20e5aU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e5aU: { // 6100d324 bsr.w $1e180
            next = 0x20e5eU;
            const auto result = m.call(c, 359U, 0x20e5aU, 0x1e180U, 0x20e5eU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e5eU: { // 6100de84 bsr.w $1ece4
            next = 0x20e62U;
            const auto result = m.call(c, 365U, 0x20e5eU, 0x1ece4U, 0x20e62U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e62U: { // 4e75 rts 
            next = 0x20e64U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20e62U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x20e64U: { // 4a2d003f tst.b $3f(a5)
            next = 0x20e68U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x20e68U: { // 6708 beq.b $20e72
            next = 0x20e6aU;
            if ((r.status & 4U) != 0U) { next = 0x20e72U; transfer_kind = 1U; }
            break;
        }
        case 0x20e6aU: { // 4df83400 lea.l $3400.w, a6
            next = 0x20e6eU;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x20e6eU: { // 522e0022 addq.b #$1, $22(a6)
            next = 0x20e72U;
            const auto v1 = m.add(m.byte((r.address[6] + 0x22U)), 0x1U, 8U);
            m.byte((r.address[6] + 0x22U), v1);
            break;
        }
        case 0x20e72U: { // 6100acb8 bsr.w $1bb2c
            next = 0x20e76U;
            const auto result = m.call(c, 564U, 0x20e72U, 0x1bb2cU, 0x20e76U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e76U: { // 6100d276 bsr.w $1e0ee
            next = 0x20e7aU;
            const auto result = m.call(c, 356U, 0x20e76U, 0x1e0eeU, 0x20e7aU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e7aU: { // 6100cd48 bsr.w $1dbc4
            next = 0x20e7eU;
            const auto result = m.call(c, 352U, 0x20e7aU, 0x1dbc4U, 0x20e7eU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e7eU: { // 6100cd70 bsr.w $1dbf0
            next = 0x20e82U;
            const auto result = m.call(c, 353U, 0x20e7eU, 0x1dbf0U, 0x20e82U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e82U: { // 6100cfac bsr.w $1de30
            next = 0x20e86U;
            const auto result = m.call(c, 355U, 0x20e82U, 0x1de30U, 0x20e86U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e86U: { // 6100d29c bsr.w $1e124
            next = 0x20e8aU;
            const auto result = m.call(c, 357U, 0x20e86U, 0x1e124U, 0x20e8aU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e8aU: { // 6100d2f4 bsr.w $1e180
            next = 0x20e8eU;
            const auto result = m.call(c, 359U, 0x20e8aU, 0x1e180U, 0x20e8eU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e8eU: { // 4e75 rts 
            next = 0x20e90U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20e8eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20e34U:
        case 0x20e3aU:
        case 0x20e3cU:
        case 0x20e40U:
        case 0x20e42U:
        case 0x20e46U:
        case 0x20e4aU:
        case 0x20e4eU:
        case 0x20e52U:
        case 0x20e56U:
        case 0x20e5aU:
        case 0x20e5eU:
        case 0x20e62U:
        case 0x20e64U:
        case 0x20e68U:
        case 0x20e6aU:
        case 0x20e6eU:
        case 0x20e72U:
        case 0x20e76U:
        case 0x20e7aU:
        case 0x20e7eU:
        case 0x20e82U:
        case 0x20e86U:
        case 0x20e8aU:
        case 0x20e8eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
