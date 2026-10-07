
FunctionResult cpu_b_stage_0229c4(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x229c4U: { // 61000e1c bsr.w $237e2
            next = 0x229c8U;
            const auto result = m.call(c, 412U, 0x229c4U, 0x237e2U, 0x229c8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x229c8U: { // 302d001c move.w $1c(a5), d0
            next = 0x229ccU;
            const auto source_address = r.address[5] + 0x1cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x229ccU: { // e540 asl.w #$2, d0
            next = 0x229ceU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x229ceU: { // 4efb0002 jmp $229d2(pc, d0.w)
            next = 0x229d2U;
            next = (0x229d2U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x229d2U: { // 6000000a bra.w $229de
            next = 0x229d6U;
            if (true) { next = 0x229deU; transfer_kind = 1U; }
            break;
        }
        case 0x229d6U: { // 60000032 bra.w $22a0a
            next = 0x229daU;
            if (true) { next = 0x22a0aU; transfer_kind = 1U; }
            break;
        }
        case 0x229daU: { // 60000064 bra.w $22a40
            next = 0x229deU;
            if (true) { next = 0x22a40U; transfer_kind = 1U; }
            break;
        }
        case 0x229deU: { // 7002 moveq #$2, d0
            next = 0x229e0U;
            r.data[0] = 0x2U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x229e0U: { // 323c0012 move.w #$12, d1
            next = 0x229e4U;
            const auto value = 0x12U;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x229e4U: { // 34351000 move.w (a5, d1.w), d2
            next = 0x229e8U;
            const auto source_address = r.address[5] + static_cast<std::int16_t>(r.data[1]);
            const auto value = m.word(source_address);
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x229e8U: { // 6b06 bmi.b $229f0
            next = 0x229eaU;
            if ((r.status & 8U) != 0U) { next = 0x229f0U; transfer_kind = 1U; }
            break;
        }
        case 0x229eaU: { // 0c420150 cmpi.w #$150, d2
            next = 0x229eeU;
            const auto source_value = 0x150U;
            const auto destination_value = r.data[2];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x229eeU: { // 6d08 blt.b $229f8
            next = 0x229f0U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x229f8U; transfer_kind = 1U; }
            break;
        }
        case 0x229f0U: { // 5841 addq.w #$4, d1
            next = 0x229f2U;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x229f2U: { // 51c8fff0 dbra d0, $229e4
            next = 0x229f6U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x229e4U;
            break;
        }
        case 0x229f6U: { // 4e75 rts 
            next = 0x229f8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x229f6U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x229f8U: { // 1b7c00010026 move.b #$1, $26(a5)
            next = 0x229feU;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x26U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x229feU: { // 1b7c00010028 move.b #$1, $28(a5)
            next = 0x22a04U;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x28U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x22a04U: { // 526d001c addq.w #$1, $1c(a5)
            next = 0x22a08U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x1cU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x22a08U: { // 4e75 rts 
            next = 0x22a0aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22a08U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22a0aU: { // 45f90002401a lea.l $2401a.l, a2
            next = 0x22a10U;
            const auto source_address = 0x2401aU;
            r.address[2] = source_address;
            break;
        }
        case 0x22a10U: { // 61000f18 bsr.w $2392a
            next = 0x22a14U;
            const auto result = m.call(c, 419U, 0x22a10U, 0x2392aU, 0x22a14U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22a14U: { // 7002 moveq #$2, d0
            next = 0x22a16U;
            r.data[0] = 0x2U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22a16U: { // 323c0012 move.w #$12, d1
            next = 0x22a1aU;
            const auto value = 0x12U;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x22a1aU: { // 34351000 move.w (a5, d1.w), d2
            next = 0x22a1eU;
            const auto source_address = r.address[5] + static_cast<std::int16_t>(r.data[1]);
            const auto value = m.word(source_address);
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x22a1eU: { // 6b06 bmi.b $22a26
            next = 0x22a20U;
            if ((r.status & 8U) != 0U) { next = 0x22a26U; transfer_kind = 1U; }
            break;
        }
        case 0x22a20U: { // 0c420110 cmpi.w #$110, d2
            next = 0x22a24U;
            const auto source_value = 0x110U;
            const auto destination_value = r.data[2];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x22a24U: { // 6d08 blt.b $22a2e
            next = 0x22a26U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x22a2eU; transfer_kind = 1U; }
            break;
        }
        case 0x22a26U: { // 5841 addq.w #$4, d1
            next = 0x22a28U;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x22a28U: { // 51c8fff0 dbra d0, $22a1a
            next = 0x22a2cU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22a1aU;
            break;
        }
        case 0x22a2cU: { // 4e75 rts 
            next = 0x22a2eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22a2cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22a2eU: { // 1b7c00010022 move.b #$1, $22(a5)
            next = 0x22a34U;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x22U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x22a34U: { // 1b7c00010024 move.b #$1, $24(a5)
            next = 0x22a3aU;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x24U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x22a3aU: { // 526d001c addq.w #$1, $1c(a5)
            next = 0x22a3eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x1cU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x22a3eU: { // 4e75 rts 
            next = 0x22a40U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22a3eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22a40U: { // 302d0008 move.w $8(a5), d0
            next = 0x22a44U;
            const auto source_address = r.address[5] + 0x8U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x22a44U: { // 0240003f andi.w #$3f, d0
            next = 0x22a48U;
            const auto source_value = 0x3fU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x22a48U: { // 6702 beq.b $22a4c
            next = 0x22a4aU;
            if ((r.status & 4U) != 0U) { next = 0x22a4cU; transfer_kind = 1U; }
            break;
        }
        case 0x22a4aU: { // 4e75 rts 
            next = 0x22a4cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22a4aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22a4cU: { // 45f90002401a lea.l $2401a.l, a2
            next = 0x22a52U;
            const auto source_address = 0x2401aU;
            r.address[2] = source_address;
            break;
        }
        case 0x22a52U: { // 61000ed6 bsr.w $2392a
            next = 0x22a56U;
            const auto result = m.call(c, 419U, 0x22a52U, 0x2392aU, 0x22a56U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22a56U: { // 4e75 rts 
            next = 0x22a58U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22a56U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x229c4U:
        case 0x229c8U:
        case 0x229ccU:
        case 0x229ceU:
        case 0x229d2U:
        case 0x229d6U:
        case 0x229daU:
        case 0x229deU:
        case 0x229e0U:
        case 0x229e4U:
        case 0x229e8U:
        case 0x229eaU:
        case 0x229eeU:
        case 0x229f0U:
        case 0x229f2U:
        case 0x229f6U:
        case 0x229f8U:
        case 0x229feU:
        case 0x22a04U:
        case 0x22a08U:
        case 0x22a0aU:
        case 0x22a10U:
        case 0x22a14U:
        case 0x22a16U:
        case 0x22a1aU:
        case 0x22a1eU:
        case 0x22a20U:
        case 0x22a24U:
        case 0x22a26U:
        case 0x22a28U:
        case 0x22a2cU:
        case 0x22a2eU:
        case 0x22a34U:
        case 0x22a3aU:
        case 0x22a3eU:
        case 0x22a40U:
        case 0x22a44U:
        case 0x22a48U:
        case 0x22a4aU:
        case 0x22a4cU:
        case 0x22a52U:
        case 0x22a56U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
