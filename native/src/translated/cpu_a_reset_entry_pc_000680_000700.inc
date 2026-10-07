CpuAResetEntryStep handle_cpu_a_reset_entry_pc_000680_000700(
    FunctionContext &c, CpuRegisters &r, unverified::Machine &m,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind)
{
    switch (pc) {
        case 0x680U: { // 3a3c0036 move.w #$36, d5
            next = 0x684U;
            m.dw(5U, 0x36U);
            m.logic(r.data[5], 16U);
            break;
        }
        case 0x684U: { // 383c2000 move.w #$2000, d4
            next = 0x688U;
            m.dw(4U, 0x2000U);
            m.logic(r.data[4], 16U);
            break;
        }
        case 0x688U: { // 610000ce bsr.w $758
            next = 0x68cU;
            const auto result = m.call(c, 3U, 0x688U, 0x758U, 0x68cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x68cU: { // 31fcfffffc80 move.w #$ffff, $fc80.w
            next = 0x692U;
            const auto value = 0xffffU;
            const auto destination_address = 0xfffffc80U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x692U: { // 7001 moveq #$1, d0
            next = 0x694U;
            r.data[0] = 0x1U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x694U: { // 4e48 trap #$8
            next = 0x696U;
            const auto result = m.exception(c, 40U, 0x694U, 0x696U);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x696U) return {true, (result)};
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x696U: { // 307cc000 movea.w #$c000, a0
            next = 0x69aU;
            const auto value = 0xc000U;
            r.address[0] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x69aU: { // 3c7cfc00 movea.w #$fc00, a6
            next = 0x69eU;
            const auto value = 0xfc00U;
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x69eU: { // 224e movea.l a6, a1
            next = 0x6a0U;
            const auto value = r.address[6];
            r.address[1] = value;
            break;
        }
        case 0x6a0U: { // 7000 moveq #$0, d0
            next = 0x6a2U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x6a2U: { // 727f moveq #$7f, d1
            next = 0x6a4U;
            r.data[1] = 0x7fU;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x6a4U: { // 5241 addq.w #$1, d1
            next = 0x6a6U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x6a6U: { // 4e43 trap #$3
            next = 0x6a8U;
            const auto result = m.exception(c, 35U, 0x6a6U, 0x6a8U);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x6a8U) return {true, (result)};
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x6a8U: { // 31fc000afc82 move.w #$a, $fc82.w
            next = 0x6aeU;
            const auto value = 0xaU;
            const auto destination_address = 0xfffffc82U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x6aeU: { // 610001a2 bsr.w $852
            next = 0x6b2U;
            const auto result = m.call(c, 4U, 0x6aeU, 0x852U, 0x6b2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x6b2U: { // 221e move.l (a6)+, d1
            next = 0x6b4U;
            const auto source_address = r.address[6];
            const auto value = m.lng(source_address);
            r.address[6] += 4U;
            r.data[1] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6b4U: { // 6712 beq.b $6c8
            next = 0x6b6U;
            if ((r.status & 4U) != 0U) { next = 0x6c8U; transfer_kind = 1U; }
            break;
        }
        case 0x6b6U: { // 201e move.l (a6)+, d0
            next = 0x6b8U;
            const auto source_address = r.address[6];
            const auto value = m.lng(source_address);
            r.address[6] += 4U;
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6b8U: { // 225e movea.l (a6)+, a1
            next = 0x6baU;
            const auto source_address = r.address[6];
            const auto value = m.lng(source_address);
            r.address[6] += 4U;
            r.address[1] = value;
            break;
        }
        case 0x6baU: { // 6100003a bsr.w $6f6
            next = 0x6beU;
            const auto result = m.call(c, 2U, 0x6baU, 0x6f6U, 0x6beU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x6beU: { // 204a movea.l a2, a0
            next = 0x6c0U;
            const auto value = r.address[2];
            r.address[0] = value;
            break;
        }
        case 0x6c0U: { // 4e4a trap #$a
            next = 0x6c2U;
            const auto result = m.exception(c, 42U, 0x6c0U, 0x6c2U);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x6c2U) return {true, (result)};
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x6c2U: { // 5378fc82 subq.w #$1, $fc82.w
            next = 0x6c6U;
            const auto source_value = 0x1U;
            const auto destination_address = 0xfffffc82U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x6c6U: { // 66ea bne.b $6b2
            next = 0x6c8U;
            if ((r.status & 4U) == 0U) { next = 0x6b2U; transfer_kind = 1U; }
            break;
        }
        case 0x6c8U: { // 7000 moveq #$0, d0
            next = 0x6caU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x6caU: { // 4e48 trap #$8
            next = 0x6ccU;
            const auto result = m.exception(c, 40U, 0x6caU, 0x6ccU);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x6ccU) return {true, (result)};
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x6ccU: { // 7000 moveq #$0, d0
            next = 0x6ceU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x6ceU: { // 2200 move.l d0, d1
            next = 0x6d0U;
            const auto value = r.data[0];
            r.data[1] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6d0U: { // 2400 move.l d0, d2
            next = 0x6d2U;
            const auto value = r.data[0];
            r.data[2] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6d2U: { // 2600 move.l d0, d3
            next = 0x6d4U;
            const auto value = r.data[0];
            r.data[3] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6d4U: { // 2800 move.l d0, d4
            next = 0x6d6U;
            const auto value = r.data[0];
            r.data[4] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6d6U: { // 2a00 move.l d0, d5
            next = 0x6d8U;
            const auto value = r.data[0];
            r.data[5] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6d8U: { // 2c00 move.l d0, d6
            next = 0x6daU;
            const auto value = r.data[0];
            r.data[6] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6daU: { // 2e00 move.l d0, d7
            next = 0x6dcU;
            const auto value = r.data[0];
            r.data[7] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x6dcU: { // 2040 movea.l d0, a0
            next = 0x6deU;
            const auto value = r.data[0];
            r.address[0] = value;
            break;
        }
        case 0x6deU: { // 2240 movea.l d0, a1
            next = 0x6e0U;
            const auto value = r.data[0];
            r.address[1] = value;
            break;
        }
        case 0x6e0U: { // 2440 movea.l d0, a2
            next = 0x6e2U;
            const auto value = r.data[0];
            r.address[2] = value;
            break;
        }
        case 0x6e2U: { // 2640 movea.l d0, a3
            next = 0x6e4U;
            const auto value = r.data[0];
            r.address[3] = value;
            break;
        }
        case 0x6e4U: { // 2840 movea.l d0, a4
            next = 0x6e6U;
            const auto value = r.data[0];
            r.address[4] = value;
            break;
        }
        case 0x6e6U: { // 2a40 movea.l d0, a5
            next = 0x6e8U;
            const auto value = r.data[0];
            r.address[5] = value;
            break;
        }
        case 0x6e8U: { // 2c40 movea.l d0, a6
            next = 0x6eaU;
            const auto value = r.data[0];
            r.address[6] = value;
            break;
        }
        case 0x6eaU: { // 2e40 movea.l d0, a7
            next = 0x6ecU;
            const auto value = r.data[0];
            r.address[7] = value;
            break;
        }
        case 0x6ecU: { // 46fc2700 move.w #$2700, sr
            next = 0x6f0U;
            r.status = static_cast<std::uint16_t>(0x2700U);
            break;
        }
        case 0x6f0U: { // 4ef9000800c0 jmp $800c0.l
            next = 0x6f6U;
            next = 0x800c0U;
            transfer_kind = 1U;
            break;
        }
    default: return {};
    }
    return {true, std::nullopt};
}
