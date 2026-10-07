CpuAResetEntryStep handle_cpu_a_reset_entry_pc_000480_000500(
    FunctionContext &c, CpuRegisters &r, unverified::Machine &m,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind)
{
    switch (pc) {
        case 0x482U: { // 2a5e movea.l (a6)+, a5
            next = 0x484U;
            const auto source_address = r.address[6];
            const auto value = m.lng(source_address);
            r.address[6] += 4U;
            r.address[5] = value;
            (void)m.word(0x486U);
            break;
        }
        case 0x484U: { // 2c1e move.l (a6)+, d6
            next = 0x486U;
            const auto source_address = r.address[6];
            const auto value = m.lng(source_address);
            r.address[6] += 4U;
            r.data[6] = value;
            m.logic(value, 32U);
            (void)m.word(0x488U);
            break;
        }
        case 0x486U: { // 7a00 moveq #$0, d5
            next = 0x488U;
            r.data[5] = 0x0U;
            m.logic(r.data[5], 32U);
            (void)m.word(0x48aU);
            break;
        }
        case 0x488U: { // 3005 move.w d5, d0
            next = 0x48aU;
            const auto value = r.data[5];
            m.dw(0U, value);
            m.logic(value, 16U);
            (void)m.word(0x48cU);
            break;
        }
        case 0x48aU: { // 204d movea.l a5, a0
            next = 0x48cU;
            const auto value = r.address[5];
            r.address[0] = value;
            (void)m.word(0x48eU);
            break;
        }
        case 0x48cU: { // 3606 move.w d6, d3
            next = 0x48eU;
            const auto value = r.data[6];
            m.dw(3U, value);
            m.logic(value, 16U);
            (void)m.word(0x490U);
            break;
        }
        case 0x48eU: { // 30c0 move.w d0, (a0)+
            next = 0x490U;
            const auto value = r.data[0];
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            r.address[0] += 2U;
            m.logic(value, 16U);
            (void)m.word(0x492U);
            break;
        }
        case 0x490U: { // d044 add.w d4, d0
            next = 0x492U;
            const auto source_value = r.data[4];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x494U);
            break;
        }
        case 0x492U: { // 51cbfffa dbra d3, $48e
            next = 0x496U;
            (void)m.word(0x48eU);
            m.dw(3U, r.data[3] - 1U);
            if ((r.data[3] & 0xffffU) != 0xffffU) next = 0x48eU;
            if (next == 0x496U) { (void)m.word(0x496U); }
            (void)m.word(next + 2U);
            break;
        }
        case 0x496U: { // 3005 move.w d5, d0
            next = 0x498U;
            const auto value = r.data[5];
            m.dw(0U, value);
            m.logic(value, 16U);
            (void)m.word(0x49aU);
            break;
        }
        case 0x498U: { // 204d movea.l a5, a0
            next = 0x49aU;
            const auto value = r.address[5];
            r.address[0] = value;
            (void)m.word(0x49cU);
            break;
        }
        case 0x49aU: { // 3606 move.w d6, d3
            next = 0x49cU;
            const auto value = r.data[6];
            m.dw(3U, value);
            m.logic(value, 16U);
            (void)m.word(0x49eU);
            break;
        }
        case 0x49cU: { // b058 cmp.w (a0)+, d0
            next = 0x49eU;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            (void)m.word(0x4a0U);
            break;
        }
        case 0x49eU: { // 670a beq.b $4aa
            next = 0x4a0U;
            if ((r.status & 4U) != 0U) { next = 0x4aaU; transfer_kind = 1U; }
            if (transfer_kind == 1U) { (void)m.word(next); }
            (void)m.word(next + 2U);
            break;
        }
        case 0x4aaU: { // d044 add.w d4, d0
            next = 0x4acU;
            const auto source_value = r.data[4];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            (void)m.word(0x4aeU);
            break;
        }
        case 0x4acU: { // 51cbffee dbra d3, $49c
            next = 0x4b0U;
            (void)m.word(0x49cU);
            m.dw(3U, r.data[3] - 1U);
            if ((r.data[3] & 0xffffU) != 0xffffU) next = 0x49cU;
            if (next == 0x4b0U) { (void)m.word(0x4b0U); }
            (void)m.word(next + 2U);
            break;
        }
        case 0x4b0U: { // 51cfffd0 dbra d7, $482
            next = 0x4b4U;
            (void)m.word(0x482U);
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x482U;
            if (next == 0x4b4U) { (void)m.word(0x4b4U); }
            (void)m.word(next + 2U);
            break;
        }
        case 0x4b4U: { // 307c000c movea.w #$c, a0
            next = 0x4b8U;
            (void)m.word(0x4b8U);
            const auto value = 0xcU;
            r.address[0] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            (void)m.word(0x4baU);
            break;
        }
        case 0x4b8U: { // 61001874 bsr.w $1d2e
            next = 0x4bcU;
            const auto result = m.call_prefetched(c, 30U, 0x4b8U, 0x1d2eU, 0x4bcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x4bcU: { // 7e00 moveq #$0, d7
            next = 0x4beU;
            r.data[7] = 0x0U;
            m.logic(r.data[7], 32U);
            (void)m.word(0x4c0U);
            break;
        }
        case 0x4beU: { // 307c0008 movea.w #$8, a0
            next = 0x4c2U;
            (void)m.word(0x4c2U);
            const auto value = 0x8U;
            r.address[0] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            (void)m.word(0x4c4U);
            break;
        }
        case 0x4c2U: { // 07080000 movep.w $0(a0), d3
            next = 0x4c6U;
            (void)m.word(0x4c6U);
            const auto memory_address = r.address[0] + 0x0U;
            std::uint32_t value = 0U;
            value = (value << 8U) | m.byte(memory_address + 0U);
            value = (value << 8U) | m.byte(memory_address + 2U);
            m.dw(3U, value);
            (void)m.word(0x4c8U);
            break;
        }
        case 0x4c6U: { // b043 cmp.w d3, d0
            next = 0x4c8U;
            const auto source_value = r.data[3];
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            (void)m.word(0x4caU);
            break;
        }
        case 0x4c8U: { // 6704 beq.b $4ce
            next = 0x4caU;
            if ((r.status & 4U) != 0U) { next = 0x4ceU; transfer_kind = 1U; }
            if (transfer_kind == 1U) { (void)m.word(next); }
            (void)m.word(next + 2U);
            break;
        }
        case 0x4ceU: { // 307c000d movea.w #$d, a0
            next = 0x4d2U;
            (void)m.word(0x4d2U);
            const auto value = 0xdU;
            r.address[0] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            (void)m.word(0x4d4U);
            break;
        }
        case 0x4d2U: { // 6100185a bsr.w $1d2e
            next = 0x4d6U;
            const auto result = m.call_prefetched(c, 30U, 0x4d2U, 0x1d2eU, 0x4d6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x4d6U: { // 323c00ff move.w #$ff, d1
            next = 0x4daU;
            (void)m.word(0x4daU);
            m.dw(1U, 0xffU);
            m.logic(r.data[1], 16U);
            (void)m.word(0x4dcU);
            break;
        }
        case 0x4daU: { // 307c0009 movea.w #$9, a0
            next = 0x4deU;
            (void)m.word(0x4deU);
            const auto value = 0x9U;
            r.address[0] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            (void)m.word(0x4e0U);
            break;
        }
        case 0x4deU: { // 07080000 movep.w $0(a0), d3
            next = 0x4e2U;
            (void)m.word(0x4e2U);
            const auto memory_address = r.address[0] + 0x0U;
            std::uint32_t value = 0U;
            value = (value << 8U) | m.byte(memory_address + 0U);
            value = (value << 8U) | m.byte(memory_address + 2U);
            m.dw(3U, value);
            (void)m.word(0x4e4U);
            break;
        }
        case 0x4e2U: { // b043 cmp.w d3, d0
            next = 0x4e4U;
            const auto source_value = r.data[3];
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            (void)m.word(0x4e6U);
            break;
        }
        case 0x4e4U: { // 6704 beq.b $4ea
            next = 0x4e6U;
            if ((r.status & 4U) != 0U) { next = 0x4eaU; transfer_kind = 1U; }
            if (transfer_kind == 1U) { (void)m.word(next); }
            (void)m.word(next + 2U);
            break;
        }
        case 0x4eaU: { // 3207 move.w d7, d1
            next = 0x4ecU;
            const auto value = r.data[7];
            m.dw(1U, value);
            m.logic(value, 16U);
            (void)m.word(0x4eeU);
            break;
        }
        case 0x4ecU: { // 6600185c bne.w $1d4a
            next = 0x4f0U;
            if ((r.status & 4U) == 0U) { next = 0x1d4aU; transfer_kind = 1U; }
            (void)m.word(next);
            (void)m.word(next + 2U);
            break;
        }
        case 0x4f0U: { // 61002f82 bsr.w $3474
            next = 0x4f4U;
            const auto result = m.call_prefetched(c, 39U, 0x4f0U, 0x3474U, 0x4f4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x4f4U: { // 61002f6e bsr.w $3464
            next = 0x4f8U;
            const auto result = m.call_prefetched(c, 38U, 0x4f4U, 0x3464U, 0x4f8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x4f8U: { // 61001bd2 bsr.w $20cc
            next = 0x4fcU;
            const auto result = m.call(c, 31U, 0x4f8U, 0x20ccU, 0x4fcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x4fcU: { // 0839000200800009 btst.b #$2, $800009.l
            next = 0x504U;
            const auto destination_address = 0x800009U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
    default: return {};
    }
    return {true, std::nullopt};
}
