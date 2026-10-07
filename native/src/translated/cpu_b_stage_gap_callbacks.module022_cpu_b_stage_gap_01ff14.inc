
FunctionResult cpu_b_stage_gap_01ff14(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1fe9cU: { // 610009fe bsr.w $2089c
            next = 0x1fea0U;
            const auto result = m.call(c, 389U, 0x1fe9cU, 0x2089cU, 0x1fea0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1fea0U: { // 4e75 rts 
            next = 0x1fea2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1fea0U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1ff14U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1ff18U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x1ff18U: { // 6668 bne.b $1ff82
            next = 0x1ff1aU;
            if ((r.status & 4U) == 0U) { next = 0x1ff82U; transfer_kind = 1U; }
            break;
        }
        case 0x1ff1aU: { // 536d0074 subq.w #$1, $74(a5)
            next = 0x1ff1eU;
            const auto v1 = m.sub(m.word((r.address[5] + 0x74U)), 0x1U, 16U);
            m.word((r.address[5] + 0x74U), v1);
            break;
        }
        case 0x1ff1eU: { // 6f00ff7c ble.w $1fe9c
            next = 0x1ff22U;
            if (((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U)) { next = 0x1fe9cU; transfer_kind = 1U; }
            break;
        }
        case 0x1ff22U: { // 61000780 bsr.w $206a4
            next = 0x1ff26U;
            const auto result = m.call(c, 386U, 0x1ff22U, 0x206a4U, 0x1ff26U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1ff26U: { // 6500ff74 bcs.w $1fe9c
            next = 0x1ff2aU;
            if ((r.status & 1U) != 0U) { next = 0x1fe9cU; transfer_kind = 1U; }
            break;
        }
        case 0x1ff2aU: { // 610006e0 bsr.w $2060c
            next = 0x1ff2eU;
            const auto result = m.call(c, 385U, 0x1ff2aU, 0x2060cU, 0x1ff2eU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1ff2eU: { // 6500ff6c bcs.w $1fe9c
            next = 0x1ff32U;
            if ((r.status & 1U) != 0U) { next = 0x1fe9cU; transfer_kind = 1U; }
            break;
        }
        case 0x1ff32U: { // 202d001e move.l $1e(a5), d0
            next = 0x1ff36U;
            const auto v1 = m.lng((r.address[5] + 0x1eU));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x1ff36U: { // d1ad0012 add.l d0, $12(a5)
            next = 0x1ff3aU;
            const auto v1 = m.add(m.lng((r.address[5] + 0x12U)), r.data[0], 32U);
            m.lng((r.address[5] + 0x12U), v1);
            break;
        }
        case 0x1ff3aU: { // 202d0026 move.l $26(a5), d0
            next = 0x1ff3eU;
            const auto v1 = m.lng((r.address[5] + 0x26U));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x1ff3eU: { // d1ad001a add.l d0, $1a(a5)
            next = 0x1ff42U;
            const auto v1 = m.add(m.lng((r.address[5] + 0x1aU)), r.data[0], 32U);
            m.lng((r.address[5] + 0x1aU), v1);
            break;
        }
        case 0x1ff42U: { // 302d005c move.w $5c(a5), d0
            next = 0x1ff46U;
            const auto v1 = m.word((r.address[5] + 0x5cU));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ff46U: { // 064000c0 addi.w #$c0, d0
            next = 0x1ff4aU;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0xc0U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1ff4aU: { // 3b40005c move.w d0, $5c(a5)
            next = 0x1ff4eU;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x5cU), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ff4eU: { // 02400700 andi.w #$700, d0
            next = 0x1ff52U;
            const auto v1 = ((r.data[0] & 0xffffU) & 0x700U) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ff52U: { // ec48 lsr.w #$6, d0
            next = 0x1ff54U;
            m.shift_word(0, 6U, false, false);
            break;
        }
        case 0x1ff54U: { // 206d0048 movea.l $48(a5), a0
            next = 0x1ff58U;
            const auto v1 = m.lng((r.address[5] + 0x48U));
            r.address[0] = v1;
            break;
        }
        case 0x1ff58U: { // 3b7000000006 move.w (a0, d0.w), $6(a5)
            next = 0x1ff5eU;
            const auto v1 = m.word((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))));
            m.word((r.address[5] + 0x6U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ff5eU: { // 5640 addq.w #$3, d0
            next = 0x1ff60U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x3U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1ff60U: { // 10300000 move.b (a0, d0.w), d0
            next = 0x1ff64U;
            const auto v1 = m.byte((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))));
            m.db(0, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1ff64U: { // 022d00fc0001 andi.b #$fc, $1(a5)
            next = 0x1ff6aU;
            const auto v1 = (m.byte((r.address[5] + 0x1U)) & 0xfcU) & 0xffU;
            m.byte((r.address[5] + 0x1U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1ff6aU: { // 812d0001 or.b d0, $1(a5)
            next = 0x1ff6eU;
            const auto v1 = (m.byte((r.address[5] + 0x1U)) | (r.data[0] & 0xffU)) & 0xffU;
            m.byte((r.address[5] + 0x1U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1ff6eU: { // 4eb900015d24 jsr $15d24.l
            next = 0x1ff74U;
            const auto result = m.indirect_call(c, 0x1ff6eU, 0x15d24U, 0x1ff74U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1ff74U: { // 4eb9000207c2 jsr $207c2.l
            next = 0x1ff7aU;
            const auto result = m.indirect_call(c, 0x1ff74U, 0x207c2U, 0x1ff7aU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1ff7aU: { // 4eb900015df2 jsr $15df2.l
            next = 0x1ff80U;
            const auto result = m.indirect_call(c, 0x1ff7aU, 0x15df2U, 0x1ff80U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1ff80U: { // 4e75 rts 
            next = 0x1ff82U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1ff80U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1ff82U: { // 61000918 bsr.w $2089c
            next = 0x1ff86U;
            const auto result = m.call(c, 389U, 0x1ff82U, 0x2089cU, 0x1ff86U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1ff86U: { // 4e75 rts 
            next = 0x1ff88U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1ff86U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1fe9cU:
        case 0x1fea0U:
        case 0x1ff14U:
        case 0x1ff18U:
        case 0x1ff1aU:
        case 0x1ff1eU:
        case 0x1ff22U:
        case 0x1ff26U:
        case 0x1ff2aU:
        case 0x1ff2eU:
        case 0x1ff32U:
        case 0x1ff36U:
        case 0x1ff3aU:
        case 0x1ff3eU:
        case 0x1ff42U:
        case 0x1ff46U:
        case 0x1ff4aU:
        case 0x1ff4eU:
        case 0x1ff52U:
        case 0x1ff54U:
        case 0x1ff58U:
        case 0x1ff5eU:
        case 0x1ff60U:
        case 0x1ff64U:
        case 0x1ff6aU:
        case 0x1ff6eU:
        case 0x1ff74U:
        case 0x1ff7aU:
        case 0x1ff80U:
        case 0x1ff82U:
        case 0x1ff86U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
