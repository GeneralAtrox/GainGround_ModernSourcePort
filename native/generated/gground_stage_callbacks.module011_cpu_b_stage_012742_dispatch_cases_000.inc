
static bool cpu_b_stage_012742_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x12746U: { // 650005c0 bcs.w $12d08
            next = 0x1274aU;
            if ((r.status & 1U) != 0U) { next = 0x12d08U; transfer_kind = 1U; }
            break;
        }
        case 0x1274eU: { // 650005b8 bcs.w $12d08
            next = 0x12752U;
            if ((r.status & 1U) != 0U) { next = 0x12d08U; transfer_kind = 1U; }
            break;
        }
        case 0x12752U: { // 302d0042 move.w $42(a5), d0
            next = 0x12756U;
            const auto source_address = r.address[5] + 0x42U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12756U: { // e540 asl.w #$2, d0
            next = 0x12758U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x12758U: { // 4efb0002 jmp $1275c(pc, d0.w)
            next = 0x1275cU;
            next = (0x1275cU + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x1275cU: { // 6000000a bra.w $12768
            next = 0x12760U;
            if (true) { next = 0x12768U; transfer_kind = 1U; }
            break;
        }
        case 0x12760U: { // 6000002c bra.w $1278e
            next = 0x12764U;
            if (true) { next = 0x1278eU; transfer_kind = 1U; }
            break;
        }
        case 0x12764U: { // 60000040 bra.w $127a6
            next = 0x12768U;
            if (true) { next = 0x127a6U; transfer_kind = 1U; }
            break;
        }
        case 0x12768U: { // 202d001e move.l $1e(a5), d0
            next = 0x1276cU;
            const auto source_address = r.address[5] + 0x1eU;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1276cU: { // d1ad0012 add.l d0, $12(a5)
            next = 0x12770U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x12U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x12770U: { // 202d0026 move.l $26(a5), d0
            next = 0x12774U;
            const auto source_address = r.address[5] + 0x26U;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x12774U: { // d1ad001a add.l d0, $1a(a5)
            next = 0x12778U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x12778U: { // 536d0046 subq.w #$1, $46(a5)
            next = 0x1277cU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x46U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1277cU: { // 6e000084 bgt.w $12802
            next = 0x12780U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x12802U; transfer_kind = 1U; }
            break;
        }
        case 0x12780U: { // 3b7c00010042 move.w #$1, $42(a5)
            next = 0x12786U;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x42U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12786U: { // 3b7c002a0046 move.w #$2a, $46(a5)
            next = 0x1278cU;
            const auto value = 0x2aU;
            const auto destination_address = r.address[5] + 0x46U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1278cU: { // 6074 bra.b $12802
            next = 0x1278eU;
            if (true) { next = 0x12802U; transfer_kind = 1U; }
            break;
        }
        case 0x1278eU: { // 536d0046 subq.w #$1, $46(a5)
            next = 0x12792U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x46U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x12792U: { // 6e30 bgt.b $127c4
            next = 0x12794U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x127c4U; transfer_kind = 1U; }
            break;
        }
        case 0x12794U: { // 3b7c00020042 move.w #$2, $42(a5)
            next = 0x1279aU;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x42U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1279aU: { // 3b7c002a0046 move.w #$2a, $46(a5)
            next = 0x127a0U;
            const auto value = 0x2aU;
            const auto destination_address = r.address[5] + 0x46U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x127a0U: { // e0ed0048 asr.w $48(a5)
            next = 0x127a4U;
            const auto address = r.address[5] + 0x48U;
            const auto old = m.word(address);
            const auto value = static_cast<std::uint16_t>((old >> 1U) | (old & 0x8000U));
            m.logic(value, 16U);
            r.status = static_cast<std::uint16_t>((r.status & ~0x11U) | ((old & 1U) ? 0x11U : 0U));
            m.word(address, value);
            break;
        }
        case 0x127a4U: { // 601e bra.b $127c4
            next = 0x127a6U;
            if (true) { next = 0x127c4U; transfer_kind = 1U; }
            break;
        }
        case 0x127a6U: { // 536d0046 subq.w #$1, $46(a5)
            next = 0x127aaU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x46U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x127aaU: { // 6e18 bgt.b $127c4
            next = 0x127acU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x127c4U; transfer_kind = 1U; }
            break;
        }
        case 0x127acU: { // 302d0046 move.w $46(a5), d0
            next = 0x127b0U;
            const auto source_address = r.address[5] + 0x46U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x127b0U: { // d07c001f add.w #$1f, d0
            next = 0x127b4U;
            const auto source_value = 0x1fU;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x127b4U: { // 6a04 bpl.b $127ba
            next = 0x127b6U;
            if ((r.status & 8U) == 0U) { next = 0x127baU; transfer_kind = 1U; }
            break;
        }
        case 0x127b6U: { // 4215 clr.b (a5)
            next = 0x127b8U;
            const auto destination_address = r.address[5];
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x127baU: { // d040 add.w d0, d0
            next = 0x127bcU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x127bcU: { // 1b400010 move.b d0, $10(a5)
            next = 0x127c0U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x10U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x127c0U: { // 1b400011 move.b d0, $11(a5)
            next = 0x127c4U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x11U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x127c4U: { // 3c6d0036 movea.w $36(a5), a6
            next = 0x127c8U;
            const auto source_address = r.address[5] + 0x36U;
            const auto value = m.word(source_address);
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x127c8U: { // 0c6e00010042 cmpi.w #$1, $42(a6)
            next = 0x127ceU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x42U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x127ceU: { // 6610 bne.b $127e0
            next = 0x127d0U;
            if ((r.status & 4U) == 0U) { next = 0x127e0U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}
