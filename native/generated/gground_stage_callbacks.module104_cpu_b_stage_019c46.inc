
FunctionResult cpu_b_stage_019c46(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19c46U: { // 536d0070 subq.w #$1, $70(a5)
            next = 0x19c4aU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x70U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19c4aU: { // 6e0c bgt.b $19c58
            next = 0x19c4cU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19c58U; transfer_kind = 1U; }
            break;
        }
        case 0x19c4cU: { // 4255 clr.w (a5)
            next = 0x19c4eU;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19c4eU: { // 6100ecba bsr.w $1890a
            next = 0x19c52U;
            const auto result = m.call(c, 880U, 0x19c4eU, 0x1890aU, 0x19c52U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19c52U: { // 42380d1b clr.b $d1b.w
            next = 0x19c56U;
            const auto destination_address = 0xd1bU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x19c56U: { // 4e75 rts 
            next = 0x19c58U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19c56U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19c58U: { // 302d0070 move.w $70(a5), d0
            next = 0x19c5cU;
            const auto source_address = r.address[5] + 0x70U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19c5cU: { // 0c40005a cmpi.w #$5a, d0
            next = 0x19c60U;
            const auto source_value = 0x5aU;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19c60U: { // 6e40 bgt.b $19ca2
            next = 0x19c62U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19ca2U; transfer_kind = 1U; }
            break;
        }
        case 0x19c62U: { // 6616 bne.b $19c7a
            next = 0x19c64U;
            if ((r.status & 4U) == 0U) { next = 0x19c7aU; transfer_kind = 1U; }
            break;
        }
        case 0x19c64U: { // 3b7c001a0058 move.w #$1a, $58(a5)
            next = 0x19c6aU;
            const auto value = 0x1aU;
            const auto destination_address = r.address[5] + 0x58U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19c6aU: { // 3b7c0c6a0008 move.w #$c6a, $8(a5)
            next = 0x19c70U;
            const auto value = 0xc6aU;
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19c70U: { // 426d0064 clr.w $64(a5)
            next = 0x19c74U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19c74U: { // 3b7c00040068 move.w #$4, $68(a5)
            next = 0x19c7aU;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19c7aU: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x19c7eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19c7eU: { // 6e46 bgt.b $19cc6
            next = 0x19c80U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19cc6U; transfer_kind = 1U; }
            break;
        }
        case 0x19c80U: { // 3b7c00040068 move.w #$4, $68(a5)
            next = 0x19c86U;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19c86U: { // 302d0064 move.w $64(a5), d0
            next = 0x19c8aU;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19c8aU: { // 0c400026 cmpi.w #$26, d0
            next = 0x19c8eU;
            const auto source_value = 0x26U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19c8eU: { // 6a06 bpl.b $19c96
            next = 0x19c90U;
            if ((r.status & 8U) == 0U) { next = 0x19c96U; transfer_kind = 1U; }
            break;
        }
        case 0x19c90U: { // 5440 addq.w #$2, d0
            next = 0x19c92U;
            const auto source_value = 0x2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19c92U: { // 3b400064 move.w d0, $64(a5)
            next = 0x19c96U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x64U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19c96U: { // 41fa1980 lea.l $1b618(pc), a0
            next = 0x19c9aU;
            const auto source_address = 0x1b618U;
            r.address[0] = source_address;
            break;
        }
        case 0x19c9aU: { // 3b7000000008 move.w (a0, d0.w), $8(a5)
            next = 0x19ca0U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19ca0U: { // 6024 bra.b $19cc6
            next = 0x19ca2U;
            if (true) { next = 0x19cc6U; transfer_kind = 1U; }
            break;
        }
        case 0x19ca2U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x19ca6U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19ca6U: { // 6e1e bgt.b $19cc6
            next = 0x19ca8U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19cc6U; transfer_kind = 1U; }
            break;
        }
        case 0x19ca8U: { // 3b7c00050068 move.w #$5, $68(a5)
            next = 0x19caeU;
            const auto value = 0x5U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19caeU: { // 526d0064 addq.w #$1, $64(a5)
            next = 0x19cb2U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19cb2U: { // 302d0064 move.w $64(a5), d0
            next = 0x19cb6U;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19cb6U: { // 02400007 andi.w #$7, d0
            next = 0x19cbaU;
            const auto source_value = 0x7U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19cbaU: { // 41fa1954 lea.l $1b610(pc), a0
            next = 0x19cbeU;
            const auto source_address = 0x1b610U;
            r.address[0] = source_address;
            break;
        }
        case 0x19cbeU: { // d0c0 adda.w d0, a0
            next = 0x19cc0U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x19cc0U: { // 1b7000000059 move.b (a0, d0.w), $59(a5)
            next = 0x19cc6U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x59U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19cc6U: { // 610004a2 bsr.w $1a16a
            next = 0x19ccaU;
            const auto result = m.call(c, 928U, 0x19cc6U, 0x1a16aU, 0x19ccaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19ccaU: { // 41fa1936 lea.l $1b602(pc), a0
            next = 0x19cceU;
            const auto source_address = 0x1b602U;
            r.address[0] = source_address;
            break;
        }
        case 0x19cceU: { // 6100f6ca bsr.w $1939a
            next = 0x19cd2U;
            const auto result = m.call(c, 904U, 0x19cceU, 0x1939aU, 0x19cd2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19cd2U: { // 4e75 rts 
            next = 0x19cd4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19cd2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x19c46U:
        case 0x19c4aU:
        case 0x19c4cU:
        case 0x19c4eU:
        case 0x19c52U:
        case 0x19c56U:
        case 0x19c58U:
        case 0x19c5cU:
        case 0x19c60U:
        case 0x19c62U:
        case 0x19c64U:
        case 0x19c6aU:
        case 0x19c70U:
        case 0x19c74U:
        case 0x19c7aU:
        case 0x19c7eU:
        case 0x19c80U:
        case 0x19c86U:
        case 0x19c8aU:
        case 0x19c8eU:
        case 0x19c90U:
        case 0x19c92U:
        case 0x19c96U:
        case 0x19c9aU:
        case 0x19ca0U:
        case 0x19ca2U:
        case 0x19ca6U:
        case 0x19ca8U:
        case 0x19caeU:
        case 0x19cb2U:
        case 0x19cb6U:
        case 0x19cbaU:
        case 0x19cbeU:
        case 0x19cc0U:
        case 0x19cc6U:
        case 0x19ccaU:
        case 0x19cceU:
        case 0x19cd2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
