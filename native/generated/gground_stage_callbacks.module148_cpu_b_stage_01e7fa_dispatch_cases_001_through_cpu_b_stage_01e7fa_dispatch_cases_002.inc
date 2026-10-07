
static bool cpu_b_stage_01e7fa_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1e862U: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1e868U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e868U: { // 661a bne.b $1e884
            next = 0x1e86aU;
            if ((r.status & 4U) == 0U) { next = 0x1e884U; transfer_kind = 1U; }
            break;
        }
        case 0x1e86aU: { // 302d005c move.w $5c(a5), d0
            next = 0x1e86eU;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e86eU: { // 06400400 addi.w #$400, d0
            next = 0x1e872U;
            const auto source_value = 0x400U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e872U: { // 02400400 andi.w #$400, d0
            next = 0x1e876U;
            const auto source_value = 0x400U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e876U: { // ed58 rol.w #$6, d0
            next = 0x1e878U;
            const auto old = r.data[0] & 0xffffU;
            const auto value = ((old << 6U) | (old >> 10U)) & 0xffffU;
            m.dw(0U, value);
            m.logic(value, 16U);
            r.status = static_cast<std::uint16_t>(r.status | (value & 1U));
            break;
        }
        case 0x1e878U: { // 9247 sub.w d7, d1
            next = 0x1e87aU;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e87aU: { // d247 add.w d7, d1
            next = 0x1e87cU;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e87cU: { // 51c8fffc dbra d0, $1e87a
            next = 0x1e880U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1e87aU;
            break;
        }
        case 0x1e880U: { // 6000f970 bra.w $1e1f2
            next = 0x1e884U;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e884U: { // 3007 move.w d7, d0
            next = 0x1e886U;
            const auto value = r.data[7];
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e886U: { // d040 add.w d0, d0
            next = 0x1e888U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e888U: { // d240 add.w d0, d1
            next = 0x1e88aU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e88aU: { // e548 lsl.w #$2, d0
            next = 0x1e88cU;
            m.shift_word(0U, 2U, true, false);
            break;
        }
        case 0x1e88cU: { // d240 add.w d0, d1
            next = 0x1e88eU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e88eU: { // 302d005c move.w $5c(a5), d0
            next = 0x1e892U;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e892U: { // 06400080 addi.w #$80, d0
            next = 0x1e896U;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e896U: { // 02400700 andi.w #$700, d0
            next = 0x1e89aU;
            const auto source_value = 0x700U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e89aU: { // e158 rol.w #$8, d0
            next = 0x1e89cU;
            const auto old = r.data[0] & 0xffffU;
            const auto value = ((old << 8U) | (old >> 8U)) & 0xffffU;
            m.dw(0U, value);
            m.logic(value, 16U);
            r.status = static_cast<std::uint16_t>(r.status | (value & 1U));
            break;
        }
        case 0x1e89cU: { // 3407 move.w d7, d2
            next = 0x1e89eU;
            const auto value = r.data[7];
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e89eU: { // d442 add.w d2, d2
            next = 0x1e8a0U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e8a0U: { // 9242 sub.w d2, d1
            next = 0x1e8a2U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e8a2U: { // d242 add.w d2, d1
            next = 0x1e8a4U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e8a4U: { // 51c8fffc dbra d0, $1e8a2
            next = 0x1e8a8U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1e8a2U;
            break;
        }
        case 0x1e8a8U: { // 0c6d00030074 cmpi.w #$3, $74(a5)
            next = 0x1e8aeU;
            const auto source_value = 0x3U;
            const auto destination_address = r.address[5] + 0x74U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1e8aeU: { // 6c00f942 bge.w $1e1f2
            next = 0x1e8b2U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e8b2U: { // d247 add.w d7, d1
            next = 0x1e8b4U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e8b4U: { // 6000f93c bra.w $1e1f2
            next = 0x1e8b8U;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e8b8U: { // 3007 move.w d7, d0
            next = 0x1e8baU;
            const auto value = r.data[7];
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e8baU: { // d040 add.w d0, d0
            next = 0x1e8bcU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e8bcU: { // d240 add.w d0, d1
            next = 0x1e8beU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e8beU: { // e548 lsl.w #$2, d0
            next = 0x1e8c0U;
            m.shift_word(0U, 2U, true, false);
            break;
        }
        case 0x1e8c0U: { // d240 add.w d0, d1
            next = 0x1e8c2U;
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

static bool cpu_b_stage_01e7fa_dispatch_cases_002([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1e8c2U: { // d240 add.w d0, d1
            next = 0x1e8c4U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e8c4U: { // d240 add.w d0, d1
            next = 0x1e8c6U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e8c6U: { // 7000 moveq #$0, d0
            next = 0x1e8c8U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1e8c8U: { // 102d0058 move.b $58(a5), d0
            next = 0x1e8ccU;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1e8ccU: { // e648 lsr.w #$3, d0
            next = 0x1e8ceU;
            m.shift_word(0U, 3U, false, false);
            break;
        }
        case 0x1e8ceU: { // 9247 sub.w d7, d1
            next = 0x1e8d0U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e8d0U: { // d247 add.w d7, d1
            next = 0x1e8d2U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e8d2U: { // 51c8fffc dbra d0, $1e8d0
            next = 0x1e8d6U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1e8d0U;
            break;
        }
        case 0x1e8d6U: { // 6000f91a bra.w $1e1f2
            next = 0x1e8daU;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}
