
static bool cpu_b_stage_gap_01e7fa_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1e83cU: { // 302d005c move.w $5c(a5), d0
            next = 0x1e840U;
            const auto v1 = m.word((r.address[5] + 0x5cU));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e840U: { // 02400400 andi.w #$400, d0
            next = 0x1e844U;
            const auto v1 = ((r.data[0] & 0xffffU) & 0x400U) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e844U: { // 6700f9ac beq.w $1e1f2
            next = 0x1e848U;
            if ((r.status & 4U) != 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e848U: { // 3007 move.w d7, d0
            next = 0x1e84aU;
            const auto v1 = (r.data[7] & 0xffffU);
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e84aU: { // e548 lsl.w #$2, d0
            next = 0x1e84cU;
            m.shift_word(0, 2U, true, false);
            break;
        }
        case 0x1e84cU: { // d240 add.w d0, d1
            next = 0x1e84eU;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[0] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e84eU: { // 6000f9a2 bra.w $1e1f2
            next = 0x1e852U;
            next = 0x1e1f2U; transfer_kind = 1U;
            break;
        }
        case 0x1e852U: { // 082d00030078 btst.b #$3, $78(a5)
            next = 0x1e858U;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x1e858U: { // 665e bne.b $1e8b8
            next = 0x1e85aU;
            if ((r.status & 4U) == 0U) { next = 0x1e8b8U; transfer_kind = 1U; }
            break;
        }
        case 0x1e85aU: { // 082d00020078 btst.b #$2, $78(a5)
            next = 0x1e860U;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x1e860U: { // 6756 beq.b $1e8b8
            next = 0x1e862U;
            if ((r.status & 4U) != 0U) { next = 0x1e8b8U; transfer_kind = 1U; }
            break;
        }
        case 0x1e862U: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1e868U;
            const auto v1 = m.byte((r.address[5] + 0x41U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 1U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x1e868U: { // 661a bne.b $1e884
            next = 0x1e86aU;
            if ((r.status & 4U) == 0U) { next = 0x1e884U; transfer_kind = 1U; }
            break;
        }
        case 0x1e86aU: { // 302d005c move.w $5c(a5), d0
            next = 0x1e86eU;
            const auto v1 = m.word((r.address[5] + 0x5cU));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e86eU: { // 06400400 addi.w #$400, d0
            next = 0x1e872U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x400U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1e872U: { // 02400400 andi.w #$400, d0
            next = 0x1e876U;
            const auto v1 = ((r.data[0] & 0xffffU) & 0x400U) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e876U: { // ed58 rol.w #$6, d0
            next = 0x1e878U;
            const auto v1 = ((r.data[0] << 6U) | ((r.data[0] & 0xffffU) >> 10U)) & 0xffffU;
            m.dw(0, v1); m.logic(v1, 16U); r.status = static_cast<std::uint16_t>((r.status & ~1U) | (v1 & 1U));
            break;
        }
        case 0x1e878U: { // 9247 sub.w d7, d1
            next = 0x1e87aU;
            const auto v1 = m.sub((r.data[1] & 0xffffU), (r.data[7] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e87aU: { // d247 add.w d7, d1
            next = 0x1e87cU;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[7] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e87cU: { // 51c8fffc dbra d0, $1e87a
            next = 0x1e880U;
            const auto v1 = static_cast<std::uint16_t>(r.data[0] - 1U); m.dw(0, v1);
            if (v1 != 0xffffU) { next = 0x1e87aU; transfer_kind = 1U; }
            break;
        }
        case 0x1e880U: { // 6000f970 bra.w $1e1f2
            next = 0x1e884U;
            next = 0x1e1f2U; transfer_kind = 1U;
            break;
        }
        case 0x1e884U: { // 3007 move.w d7, d0
            next = 0x1e886U;
            const auto v1 = (r.data[7] & 0xffffU);
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e886U: { // d040 add.w d0, d0
            next = 0x1e888U;
            const auto v1 = m.add((r.data[0] & 0xffffU), (r.data[0] & 0xffffU), 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1e888U: { // d240 add.w d0, d1
            next = 0x1e88aU;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[0] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e88aU: { // e548 lsl.w #$2, d0
            next = 0x1e88cU;
            m.shift_word(0, 2U, true, false);
            break;
        }
        case 0x1e88cU: { // d240 add.w d0, d1
            next = 0x1e88eU;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[0] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e88eU: { // 302d005c move.w $5c(a5), d0
            next = 0x1e892U;
            const auto v1 = m.word((r.address[5] + 0x5cU));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e892U: { // 06400080 addi.w #$80, d0
            next = 0x1e896U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x80U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1e896U: { // 02400700 andi.w #$700, d0
            next = 0x1e89aU;
            const auto v1 = ((r.data[0] & 0xffffU) & 0x700U) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e89aU: { // e158 rol.w #$8, d0
            next = 0x1e89cU;
            const auto v1 = ((r.data[0] << 8U) | ((r.data[0] & 0xffffU) >> 8U)) & 0xffffU;
            m.dw(0, v1); m.logic(v1, 16U); r.status = static_cast<std::uint16_t>((r.status & ~1U) | (v1 & 1U));
            break;
        }
        case 0x1e89cU: { // 3407 move.w d7, d2
            next = 0x1e89eU;
            const auto v1 = (r.data[7] & 0xffffU);
            m.dw(2, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e89eU: { // d442 add.w d2, d2
            next = 0x1e8a0U;
            const auto v1 = m.add((r.data[2] & 0xffffU), (r.data[2] & 0xffffU), 16U);
            m.dw(2, v1);
            break;
        }
        case 0x1e8a0U: { // 9242 sub.w d2, d1
            next = 0x1e8a2U;
            const auto v1 = m.sub((r.data[1] & 0xffffU), (r.data[2] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e8a2U: { // d242 add.w d2, d1
            next = 0x1e8a4U;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[2] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e8a4U: { // 51c8fffc dbra d0, $1e8a2
            next = 0x1e8a8U;
            const auto v1 = static_cast<std::uint16_t>(r.data[0] - 1U); m.dw(0, v1);
            if (v1 != 0xffffU) { next = 0x1e8a2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e8a8U: { // 0c6d00030074 cmpi.w #$3, $74(a5)
            next = 0x1e8aeU;
            (void)m.sub(m.word((r.address[5] + 0x74U)), 0x3U, 16U, true);
            break;
        }
        case 0x1e8aeU: { // 6c00f942 bge.w $1e1f2
            next = 0x1e8b2U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e8b2U: { // d247 add.w d7, d1
            next = 0x1e8b4U;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[7] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e8b4U: { // 6000f93c bra.w $1e1f2
            next = 0x1e8b8U;
            next = 0x1e1f2U; transfer_kind = 1U;
            break;
        }
        case 0x1e8b8U: { // 3007 move.w d7, d0
            next = 0x1e8baU;
            const auto v1 = (r.data[7] & 0xffffU);
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e8baU: { // d040 add.w d0, d0
            next = 0x1e8bcU;
            const auto v1 = m.add((r.data[0] & 0xffffU), (r.data[0] & 0xffffU), 16U);
            m.dw(0, v1);
            break;
        }
    default: return false;
    }
    return true;
}

static bool cpu_b_stage_gap_01e7fa_dispatch_cases_002([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1e8bcU: { // d240 add.w d0, d1
            next = 0x1e8beU;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[0] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e8beU: { // e548 lsl.w #$2, d0
            next = 0x1e8c0U;
            m.shift_word(0, 2U, true, false);
            break;
        }
        case 0x1e8c0U: { // d240 add.w d0, d1
            next = 0x1e8c2U;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[0] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e8c2U: { // d240 add.w d0, d1
            next = 0x1e8c4U;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[0] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e8c4U: { // d240 add.w d0, d1
            next = 0x1e8c6U;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[0] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e8c6U: { // 7000 moveq #$0, d0
            next = 0x1e8c8U;
            r.data[0] = 0x0U; m.logic(r.data[0], 32U);
            break;
        }
        case 0x1e8c8U: { // 102d0058 move.b $58(a5), d0
            next = 0x1e8ccU;
            const auto v1 = m.byte((r.address[5] + 0x58U));
            m.db(0, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1e8ccU: { // e648 lsr.w #$3, d0
            next = 0x1e8ceU;
            m.shift_word(0, 3U, false, false);
            break;
        }
        case 0x1e8ceU: { // 9247 sub.w d7, d1
            next = 0x1e8d0U;
            const auto v1 = m.sub((r.data[1] & 0xffffU), (r.data[7] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e8d0U: { // d247 add.w d7, d1
            next = 0x1e8d2U;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[7] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e8d2U: { // 51c8fffc dbra d0, $1e8d0
            next = 0x1e8d6U;
            const auto v1 = static_cast<std::uint16_t>(r.data[0] - 1U); m.dw(0, v1);
            if (v1 != 0xffffU) { next = 0x1e8d0U; transfer_kind = 1U; }
            break;
        }
        case 0x1e8d6U: { // 6000f91a bra.w $1e1f2
            next = 0x1e8daU;
            next = 0x1e1f2U; transfer_kind = 1U;
            break;
        }
    default: return false;
    }
    return true;
}
