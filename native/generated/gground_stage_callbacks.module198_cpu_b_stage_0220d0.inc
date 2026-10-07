
FunctionResult cpu_b_stage_0220d0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x220d0U: { // 4a2d003f tst.b $3f(a5)
            next = 0x220d4U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x220d4U: { // 671e beq.b $220f4
            next = 0x220d6U;
            if ((r.status & 4U) != 0U) { next = 0x220f4U; transfer_kind = 1U; }
            break;
        }
        case 0x220d6U: { // 4df83400 lea.l $3400.w, a6
            next = 0x220daU;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x220daU: { // 522e0022 addq.b #$1, $22(a6)
            next = 0x220deU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x22U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x220deU: { // 2b7c0001eeb60002 move.l #$1eeb6, $2(a5)
            next = 0x220e6U;
            const auto value = 0x1eeb6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x220e6U: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x220ecU;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x220ecU: { // 7200 moveq #$0, d1
            next = 0x220eeU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x220eeU: { // 6100c214 bsr.w $1e304
            next = 0x220f2U;
            const auto result = m.call(c, 360U, 0x220eeU, 0x1e304U, 0x220f2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x220f2U: { // 4e75 rts 
            next = 0x220f4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x220f2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x220f4U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x220faU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x220faU: { // 671a beq.b $22116
            next = 0x220fcU;
            if ((r.status & 4U) != 0U) { next = 0x22116U; transfer_kind = 1U; }
            break;
        }
        case 0x220fcU: { // 6100adf8 bsr.w $1cef6
            next = 0x22100U;
            const auto result = m.call(c, 334U, 0x220fcU, 0x1cef6U, 0x22100U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22100U: { // 6100bac2 bsr.w $1dbc4
            next = 0x22104U;
            const auto result = m.call(c, 352U, 0x22100U, 0x1dbc4U, 0x22104U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22104U: { // 6100baea bsr.w $1dbf0
            next = 0x22108U;
            const auto result = m.call(c, 353U, 0x22104U, 0x1dbf0U, 0x22108U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22108U: { // 6100c01a bsr.w $1e124
            next = 0x2210cU;
            const auto result = m.call(c, 357U, 0x22108U, 0x1e124U, 0x2210cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2210cU: { // 6100c1f6 bsr.w $1e304
            next = 0x22110U;
            const auto result = m.call(c, 360U, 0x2210cU, 0x1e304U, 0x22110U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22110U: { // 6100cbd2 bsr.w $1ece4
            next = 0x22114U;
            const auto result = m.call(c, 365U, 0x22110U, 0x1ece4U, 0x22114U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22114U: { // 4e75 rts 
            next = 0x22116U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22114U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22116U: { // 61009ff0 bsr.w $1c108
            next = 0x2211aU;
            const auto result = m.call(c, 329U, 0x22116U, 0x1c108U, 0x2211aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2211aU: { // 6100cec0 bsr.w $1efdc
            next = 0x2211eU;
            const auto result = m.call(c, 368U, 0x2211aU, 0x1efdcU, 0x2211eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2211eU: { // 6100baa4 bsr.w $1dbc4
            next = 0x22122U;
            const auto result = m.call(c, 352U, 0x2211eU, 0x1dbc4U, 0x22122U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22122U: { // 6100bacc bsr.w $1dbf0
            next = 0x22126U;
            const auto result = m.call(c, 353U, 0x22122U, 0x1dbf0U, 0x22126U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22126U: { // 6100bd08 bsr.w $1de30
            next = 0x2212aU;
            const auto result = m.call(c, 355U, 0x22126U, 0x1de30U, 0x2212aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2212aU: { // 6100bfc2 bsr.w $1e0ee
            next = 0x2212eU;
            const auto result = m.call(c, 356U, 0x2212aU, 0x1e0eeU, 0x2212eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2212eU: { // 6100bff4 bsr.w $1e124
            next = 0x22132U;
            const auto result = m.call(c, 357U, 0x2212eU, 0x1e124U, 0x22132U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22132U: { // 6100c1d0 bsr.w $1e304
            next = 0x22136U;
            const auto result = m.call(c, 360U, 0x22132U, 0x1e304U, 0x22136U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22136U: { // 4e75 rts 
            next = 0x22138U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22136U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x220d0U:
        case 0x220d4U:
        case 0x220d6U:
        case 0x220daU:
        case 0x220deU:
        case 0x220e6U:
        case 0x220ecU:
        case 0x220eeU:
        case 0x220f2U:
        case 0x220f4U:
        case 0x220faU:
        case 0x220fcU:
        case 0x22100U:
        case 0x22104U:
        case 0x22108U:
        case 0x2210cU:
        case 0x22110U:
        case 0x22114U:
        case 0x22116U:
        case 0x2211aU:
        case 0x2211eU:
        case 0x22122U:
        case 0x22126U:
        case 0x2212aU:
        case 0x2212eU:
        case 0x22132U:
        case 0x22136U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
