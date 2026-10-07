CpuAResetEntryStep handle_cpu_a_reset_entry_pc_000500_000600(
    FunctionContext &c, CpuRegisters &r, unverified::Machine &m,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind)
{
    switch (pc) {
        case 0x504U: { // 6604 bne.b $50a
            next = 0x506U;
            if ((r.status & 4U) == 0U) { next = 0x50aU; transfer_kind = 1U; }
            break;
        }
        case 0x506U: { // 60001ebc bra.w $23c4
            next = 0x50aU;
            if (true) { next = 0x23c4U; transfer_kind = 1U; }
            break;
        }
        case 0x50aU: { // 103900b00005 move.b $b00005.l, d0
            next = 0x510U;
            const auto source_address = 0xb00005U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x510U: { // 4600 not.b d0
            next = 0x512U;
            const auto old = r.data[0];
            const auto value = ~old;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x512U: { // 13c000b00005 move.b d0, $b00005.l
            next = 0x518U;
            const auto value = r.data[0];
            const auto destination_address = 0xb00005U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x518U: { // 4aa7 tst.l -(a7)
            next = 0x51aU;
            r.address[7] -= 4U;
            const auto destination_address = r.address[7];
            const auto value = m.lng(destination_address);
            m.logic(value, 32U);
            break;
        }
        case 0x51aU: { // 4a9f tst.l (a7)+
            next = 0x51cU;
            const auto destination_address = r.address[7];
            const auto value = m.lng(destination_address);
            r.address[7] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x51cU: { // b03900b00005 cmp.b $b00005.l, d0
            next = 0x522U;
            const auto source_address = 0xb00005U;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x522U: { // 6700007c beq.w $5a0
            next = 0x526U;
            if ((r.status & 4U) != 0U) { next = 0x5a0U; transfer_kind = 1U; }
            break;
        }
        case 0x5a0U: { // 103900b00005 move.b $b00005.l, d0
            next = 0x5a6U;
            const auto source_address = 0xb00005U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x5a6U: { // 4600 not.b d0
            next = 0x5a8U;
            const auto old = r.data[0];
            const auto value = ~old;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x5a8U: { // 13c000b00005 move.b d0, $b00005.l
            next = 0x5aeU;
            const auto value = r.data[0];
            const auto destination_address = 0xb00005U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x5aeU: { // 4aa7 tst.l -(a7)
            next = 0x5b0U;
            r.address[7] -= 4U;
            const auto destination_address = r.address[7];
            const auto value = m.lng(destination_address);
            m.logic(value, 32U);
            break;
        }
        case 0x5b0U: { // 4a9f tst.l (a7)+
            next = 0x5b2U;
            const auto destination_address = r.address[7];
            const auto value = m.lng(destination_address);
            r.address[7] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x5b2U: { // b03900b00005 cmp.b $b00005.l, d0
            next = 0x5b8U;
            const auto source_address = 0xb00005U;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x5b8U: { // 660c bne.b $5c6
            next = 0x5baU;
            if ((r.status & 4U) == 0U) { next = 0x5c6U; transfer_kind = 1U; }
            break;
        }
        case 0x5baU: { // 0839000300800009 btst.b #$3, $800009.l
            next = 0x5c2U;
            const auto destination_address = 0x800009U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x5c2U: { // 67000d4c beq.w $1310
            next = 0x5c6U;
            if ((r.status & 4U) != 0U) { next = 0x1310U; transfer_kind = 1U; }
            break;
        }
        case 0x5c6U: { // 13fc00010040401b move.b #$1, $40401b.l
            next = 0x5ceU;
            const auto value = 0x1U;
            const auto destination_address = 0x40401bU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x5ceU: { // 4e722300 stop #$2300
            next = 0x5d2U;
            r.status = static_cast<std::uint16_t>(0x2300U);
            r.program_counter = 0x5d2U;
            for (;;) {
                const auto pending = c.host->consume_pending_interrupt(0U, 0xffU, 0x5d2U);
                if (!pending.asserted || pending.level <= ((r.status >> 8U) & 7U)) continue;
                if (pending.level != 4U) return {true, FunctionResult{TranslationStatus::contract_violation, 0U, r.program_counter}};
                if (c.host->resumes_interrupts_inline()) {
                    const auto result = service_cpu_a_autovector(c, pending.level, 0x5d2U, 0x5d2U);
                    if (result.status != TranslationStatus::complete) return {true, (result)};
                    next = r.program_counter;
                    break;
                }
                const auto saved_status = r.status;
                r.address[7] -= 4U; m.word(r.address[7] + 2U, 0x5d2U);
                r.address[7] -= 2U; m.word(r.address[7], saved_status); m.word(r.address[7] + 2U, 0U);
                r.status = static_cast<std::uint16_t>((saved_status & 0x38ffU) | 0x2400U);
                const auto target = m.lng(0x70U);
                (void)m.word(target); (void)m.word(target + 2U);
                r.program_counter = target;
                if (target != 0x80048U) return {true, FunctionResult{TranslationStatus::contract_violation, 0U, target}};
                const auto marker = c.host->call_function(47U, 0U, 0xffU, 0U, 0x5d2U, target, c);
                if (marker.status != TranslationStatus::complete) return {true, (marker)};
                const auto result = cpu_a_irq4_vector_trampoline(c);
                if (result.status != TranslationStatus::complete) return {true, (result)};
                if (r.program_counter != 0x5d2U) return {true, FunctionResult{TranslationStatus::contract_violation, 0U, r.program_counter}};
                next = r.program_counter;
                break;
            }
            break;
        }
        case 0x5d2U: { // 610005c4 bsr.w $b98
            next = 0x5d6U;
            const auto result = m.call(c, 5U, 0x5d2U, 0xb98U, 0x5d6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x5d6U: { // 610005fc bsr.w $bd4
            next = 0x5daU;
            const auto result = m.call(c, 8U, 0x5d6U, 0xbd4U, 0x5daU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x5daU: { // 610005d0 bsr.w $bac
            next = 0x5deU;
            const auto result = m.call(c, 6U, 0x5daU, 0xbacU, 0x5deU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x5deU: { // 610005e0 bsr.w $bc0
            next = 0x5e2U;
            const auto result = m.call(c, 7U, 0x5deU, 0xbc0U, 0x5e2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x5e2U: { // 61000604 bsr.w $be8
            next = 0x5e6U;
            const auto result = m.call(c, 9U, 0x5e2U, 0xbe8U, 0x5e6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x5e6U: { // 41fa0186 lea.l $76e(pc), a0
            next = 0x5eaU;
            const auto source_address = 0x76eU;
            r.address[0] = source_address;
            break;
        }
        case 0x5eaU: { // 227c00400000 movea.l #$400000, a1
            next = 0x5f0U;
            const auto value = 0x400000U;
            r.address[1] = value;
            break;
        }
        case 0x5f0U: { // 7e02 moveq #$2, d7
            next = 0x5f2U;
            r.data[7] = 0x2U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x5f2U: { // 7c1f moveq #$1f, d6
            next = 0x5f4U;
            r.data[6] = 0x1fU;
            m.logic(r.data[6], 32U);
            break;
        }
        case 0x5f4U: { // 2448 movea.l a0, a2
            next = 0x5f6U;
            const auto value = r.address[0];
            r.address[2] = value;
            break;
        }
        case 0x5f6U: { // 3a3c0007 move.w #$7, d5
            next = 0x5faU;
            m.dw(5U, 0x7U);
            m.logic(r.data[5], 16U);
            break;
        }
        case 0x5faU: { // 22da move.l (a2)+, (a1)+
            next = 0x5fcU;
            const auto source_address = r.address[2];
            const auto value = m.lng(source_address);
            r.address[2] += 4U;
            const auto destination_address = r.address[1];
            m.lng(destination_address, value);
            r.address[1] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x5fcU: { // 51cdfffc dbra d5, $5fa
            next = 0x600U;
            m.dw(5U, r.data[5] - 1U);
            if ((r.data[5] & 0xffffU) != 0xffffU) next = 0x5faU;
            break;
        }
    default: return {};
    }
    return {true, std::nullopt};
}
