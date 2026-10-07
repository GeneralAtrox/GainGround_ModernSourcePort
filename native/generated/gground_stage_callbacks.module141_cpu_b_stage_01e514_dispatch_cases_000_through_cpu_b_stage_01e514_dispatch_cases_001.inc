
static bool cpu_b_stage_01e514_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1e514U: { // 082d00070041 btst.b #$7, $41(a5)
            next = 0x1e51aU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e51aU: { // 6702 beq.b $1e51e
            next = 0x1e51cU;
            if ((r.status & 4U) != 0U) { next = 0x1e51eU; transfer_kind = 1U; }
            break;
        }
        case 0x1e51eU: { // 7000 moveq #$0, d0
            next = 0x1e520U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1e520U: { // 102d0058 move.b $58(a5), d0
            next = 0x1e524U;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1e524U: { // 0240003f andi.w #$3f, d0
            next = 0x1e528U;
            const auto source_value = 0x3fU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e528U: { // e248 lsr.w #$1, d0
            next = 0x1e52aU;
            m.shift_word(0U, 1U, false, false);
            break;
        }
        case 0x1e52aU: { // 0c400010 cmpi.w #$10, d0
            next = 0x1e52eU;
            const auto source_value = 0x10U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1e52eU: { // 673e beq.b $1e56e
            next = 0x1e530U;
            if ((r.status & 4U) != 0U) { next = 0x1e56eU; transfer_kind = 1U; }
            break;
        }
        case 0x1e530U: { // 6e1c bgt.b $1e54e
            next = 0x1e532U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1e54eU; transfer_kind = 1U; }
            break;
        }
        case 0x1e532U: { // 323c0018 move.w #$18, d1
            next = 0x1e536U;
            const auto value = 0x18U;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e536U: { // 0c400008 cmpi.w #$8, d0
            next = 0x1e53aU;
            const auto source_value = 0x8U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1e53aU: { // 6e00fcb6 bgt.w $1e1f2
            next = 0x1e53eU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e53eU: { // 02400003 andi.w #$3, d0
            next = 0x1e542U;
            const auto source_value = 0x3U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e542U: { // d040 add.w d0, d0
            next = 0x1e544U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e544U: { // 3200 move.w d0, d1
            next = 0x1e546U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e546U: { // d040 add.w d0, d0
            next = 0x1e548U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e548U: { // d240 add.w d0, d1
            next = 0x1e54aU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e54aU: { // 6000fca6 bra.w $1e1f2
            next = 0x1e54eU;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e54eU: { // 0240000f andi.w #$f, d0
            next = 0x1e552U;
            const auto source_value = 0xfU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e552U: { // 323c0018 move.w #$18, d1
            next = 0x1e556U;
            const auto value = 0x18U;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e556U: { // 0c400008 cmpi.w #$8, d0
            next = 0x1e55aU;
            const auto source_value = 0x8U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1e55aU: { // 6d00fc96 blt.w $1e1f2
            next = 0x1e55eU;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e55eU: { // 02400003 andi.w #$3, d0
            next = 0x1e562U;
            const auto source_value = 0x3U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e562U: { // d040 add.w d0, d0
            next = 0x1e564U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e564U: { // 3200 move.w d0, d1
            next = 0x1e566U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e566U: { // d040 add.w d0, d0
            next = 0x1e568U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e568U: { // d240 add.w d0, d1
            next = 0x1e56aU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e56aU: { // 6000fc86 bra.w $1e1f2
            next = 0x1e56eU;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e56eU: { // 526d0056 addq.w #$1, $56(a5)
            next = 0x1e572U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x56U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1e572U: { // 323c001e move.w #$1e, d1
            next = 0x1e576U;
            const auto value = 0x1eU;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e576U: { // 302d005c move.w $5c(a5), d0
            next = 0x1e57aU;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e57aU: { // 06400080 addi.w #$80, d0
            next = 0x1e57eU;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e57eU: { // 02400700 andi.w #$700, d0
            next = 0x1e582U;
            const auto source_value = 0x700U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e582U: { // ea48 lsr.w #$5, d0
            next = 0x1e584U;
            m.shift_word(0U, 5U, false, false);
            break;
        }
        case 0x1e584U: { // d240 add.w d0, d1
            next = 0x1e586U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
    default: return false;
    }
    return true;
}

static bool cpu_b_stage_01e514_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1e586U: { // e248 lsr.w #$1, d0
            next = 0x1e588U;
            m.shift_word(0U, 1U, false, false);
            break;
        }
        case 0x1e588U: { // d240 add.w d0, d1
            next = 0x1e58aU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e58aU: { // 302d0056 move.w $56(a5), d0
            next = 0x1e58eU;
            const auto source_address = r.address[5] + 0x56U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e58eU: { // 02400002 andi.w #$2, d0
            next = 0x1e592U;
            const auto source_value = 0x2U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e592U: { // 6702 beq.b $1e596
            next = 0x1e594U;
            if ((r.status & 4U) != 0U) { next = 0x1e596U; transfer_kind = 1U; }
            break;
        }
        case 0x1e594U: { // 5c41 addq.w #$6, d1
            next = 0x1e596U;
            const auto source_value = 0x6U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e596U: { // 6000fc5a bra.w $1e1f2
            next = 0x1e59aU;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}
