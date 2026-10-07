
FunctionResult cpu_b_stage_022138(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22138U: { // 4a2d003f tst.b $3f(a5)
            next = 0x2213cU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x2213cU: { // 671e beq.b $2215c
            next = 0x2213eU;
            if ((r.status & 4U) != 0U) { next = 0x2215cU; transfer_kind = 1U; }
            break;
        }
        case 0x2213eU: { // 4df83400 lea.l $3400.w, a6
            next = 0x22142U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x22142U: { // 522e0024 addq.b #$1, $24(a6)
            next = 0x22146U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x24U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x22146U: { // 2b7c0001eeb60002 move.l #$1eeb6, $2(a5)
            next = 0x2214eU;
            const auto value = 0x1eeb6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x2214eU: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x22154U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x22154U: { // 7200 moveq #$0, d1
            next = 0x22156U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x22156U: { // 6100c1ac bsr.w $1e304
            next = 0x2215aU;
            const auto result = m.call(c, 360U, 0x22156U, 0x1e304U, 0x2215aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2215aU: { // 4e75 rts 
            next = 0x2215cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2215aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x2215cU: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x22162U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x22162U: { // 671a beq.b $2217e
            next = 0x22164U;
            if ((r.status & 4U) != 0U) { next = 0x2217eU; transfer_kind = 1U; }
            break;
        }
        case 0x22164U: { // 6100ad90 bsr.w $1cef6
            next = 0x22168U;
            const auto result = m.call(c, 334U, 0x22164U, 0x1cef6U, 0x22168U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22168U: { // 6100ba5a bsr.w $1dbc4
            next = 0x2216cU;
            const auto result = m.call(c, 352U, 0x22168U, 0x1dbc4U, 0x2216cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2216cU: { // 6100ba82 bsr.w $1dbf0
            next = 0x22170U;
            const auto result = m.call(c, 353U, 0x2216cU, 0x1dbf0U, 0x22170U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22170U: { // 6100bfb2 bsr.w $1e124
            next = 0x22174U;
            const auto result = m.call(c, 357U, 0x22170U, 0x1e124U, 0x22174U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22174U: { // 6100c18e bsr.w $1e304
            next = 0x22178U;
            const auto result = m.call(c, 360U, 0x22174U, 0x1e304U, 0x22178U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22178U: { // 6100cb6a bsr.w $1ece4
            next = 0x2217cU;
            const auto result = m.call(c, 365U, 0x22178U, 0x1ece4U, 0x2217cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2217cU: { // 4e75 rts 
            next = 0x2217eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2217cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x2217eU: { // 61009f88 bsr.w $1c108
            next = 0x22182U;
            const auto result = m.call(c, 329U, 0x2217eU, 0x1c108U, 0x22182U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22182U: { // 6100ce58 bsr.w $1efdc
            next = 0x22186U;
            const auto result = m.call(c, 368U, 0x22182U, 0x1efdcU, 0x22186U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22186U: { // 6100ba3c bsr.w $1dbc4
            next = 0x2218aU;
            const auto result = m.call(c, 352U, 0x22186U, 0x1dbc4U, 0x2218aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2218aU: { // 6100ba64 bsr.w $1dbf0
            next = 0x2218eU;
            const auto result = m.call(c, 353U, 0x2218aU, 0x1dbf0U, 0x2218eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2218eU: { // 6100bca0 bsr.w $1de30
            next = 0x22192U;
            const auto result = m.call(c, 355U, 0x2218eU, 0x1de30U, 0x22192U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22192U: { // 6100bf5a bsr.w $1e0ee
            next = 0x22196U;
            const auto result = m.call(c, 356U, 0x22192U, 0x1e0eeU, 0x22196U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22196U: { // 6100bf8c bsr.w $1e124
            next = 0x2219aU;
            const auto result = m.call(c, 357U, 0x22196U, 0x1e124U, 0x2219aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2219aU: { // 6100c168 bsr.w $1e304
            next = 0x2219eU;
            const auto result = m.call(c, 360U, 0x2219aU, 0x1e304U, 0x2219eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2219eU: { // 4e75 rts 
            next = 0x221a0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2219eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22138U:
        case 0x2213cU:
        case 0x2213eU:
        case 0x22142U:
        case 0x22146U:
        case 0x2214eU:
        case 0x22154U:
        case 0x22156U:
        case 0x2215aU:
        case 0x2215cU:
        case 0x22162U:
        case 0x22164U:
        case 0x22168U:
        case 0x2216cU:
        case 0x22170U:
        case 0x22174U:
        case 0x22178U:
        case 0x2217cU:
        case 0x2217eU:
        case 0x22182U:
        case 0x22186U:
        case 0x2218aU:
        case 0x2218eU:
        case 0x22192U:
        case 0x22196U:
        case 0x2219aU:
        case 0x2219eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
