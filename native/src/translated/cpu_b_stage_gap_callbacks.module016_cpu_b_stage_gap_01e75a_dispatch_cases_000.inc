
static bool cpu_b_stage_gap_01e75a_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1e1f2U: { // 206d0048 movea.l $48(a5), a0
            next = 0x1e1f6U;
            const auto v1 = m.lng((r.address[5] + 0x48U));
            r.address[0] = v1;
            break;
        }
        case 0x1e1f6U: { // d0c1 adda.w d1, a0
            next = 0x1e1f8U;
            r.address[0] += static_cast<std::uint32_t>(static_cast<std::int16_t>((r.data[1] & 0xffffU)));
            break;
        }
        case 0x1e1f8U: { // 2b48004c move.l a0, $4c(a5)
            next = 0x1e1fcU;
            const auto v1 = r.address[0];
            m.lng((r.address[5] + 0x4cU), v1);
            m.logic(v1, 32U);
            break;
        }
        case 0x1e1fcU: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x1e200U;
            const auto ea2 = r.address[0]; r.address[0] += 2U;
            const auto v1 = m.word(ea2);
            m.word((r.address[5] + 0x6U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e200U: { // 1b580010 move.b (a0)+, $10(a5)
            next = 0x1e204U;
            const auto ea2 = r.address[0]; r.address[0] += 1U;
            const auto v1 = m.byte(ea2);
            m.byte((r.address[5] + 0x10U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1e204U: { // 1b580011 move.b (a0)+, $11(a5)
            next = 0x1e208U;
            const auto ea2 = r.address[0]; r.address[0] += 1U;
            const auto v1 = m.byte(ea2);
            m.byte((r.address[5] + 0x11U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1e208U: { // 303c0c00 move.w #$c00, d0
            next = 0x1e20cU;
            const auto v1 = 0xc00U;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e20cU: { // d018 add.b (a0)+, d0
            next = 0x1e20eU;
            const auto ea2 = r.address[0]; r.address[0] += 1U;
            const auto v1 = m.add((r.data[0] & 0xffU), m.byte(ea2), 8U);
            m.db(0, v1);
            break;
        }
        case 0x1e20eU: { // 3b400008 move.w d0, $8(a5)
            next = 0x1e212U;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x8U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e212U: { // 102d0001 move.b $1(a5), d0
            next = 0x1e216U;
            const auto v1 = m.byte((r.address[5] + 0x1U));
            m.db(0, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1e216U: { // 0200fff8 andi.b #$f8, d0
            next = 0x1e21aU;
            const auto v1 = ((r.data[0] & 0xffU) & 0xf8U) & 0xffU;
            m.db(0, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1e21aU: { // 8018 or.b (a0)+, d0
            next = 0x1e21cU;
            const auto ea2 = r.address[0]; r.address[0] += 1U;
            const auto v1 = ((r.data[0] & 0xffU) | m.byte(ea2)) & 0xffU;
            m.db(0, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1e21cU: { // 1b400001 move.b d0, $1(a5)
            next = 0x1e220U;
            const auto v1 = (r.data[0] & 0xffU);
            m.byte((r.address[5] + 0x1U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1e224U: { // 08ad00030040 bclr.b #$3, $40(a5)
            next = 0x1e22aU;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 & ~(1U << 3U)) & 0xffU);
            break;
        }
        case 0x1e75eU: { // 7200 moveq #$0, d1
            next = 0x1e760U;
            r.data[1] = 0x0U; m.logic(r.data[1], 32U);
            break;
        }
        case 0x1e760U: { // 082d00010078 btst.b #$1, $78(a5)
            next = 0x1e766U;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 1U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x1e766U: { // 6742 beq.b $1e7aa
            next = 0x1e768U;
            if ((r.status & 4U) != 0U) { next = 0x1e7aaU; transfer_kind = 1U; }
            break;
        }
        case 0x1e768U: { // 9247 sub.w d7, d1
            next = 0x1e76aU;
            const auto v1 = m.sub((r.data[1] & 0xffffU), (r.data[7] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e76aU: { // 206d0048 movea.l $48(a5), a0
            next = 0x1e76eU;
            const auto v1 = m.lng((r.address[5] + 0x48U));
            r.address[0] = v1;
            break;
        }
        case 0x1e76eU: { // d0c1 adda.w d1, a0
            next = 0x1e770U;
            r.address[0] += static_cast<std::uint32_t>(static_cast<std::int16_t>((r.data[1] & 0xffffU)));
            break;
        }
        case 0x1e770U: { // 2b48004c move.l a0, $4c(a5)
            next = 0x1e774U;
            const auto v1 = r.address[0];
            m.lng((r.address[5] + 0x4cU), v1);
            m.logic(v1, 32U);
            break;
        }
        case 0x1e774U: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x1e778U;
            const auto ea2 = r.address[0]; r.address[0] += 2U;
            const auto v1 = m.word(ea2);
            m.word((r.address[5] + 0x6U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e778U: { // d0fc0002 adda.w #$2, a0
            next = 0x1e77cU;
            r.address[0] += 0x2U;
            break;
        }
        case 0x1e77cU: { // 303c0c00 move.w #$c00, d0
            next = 0x1e780U;
            const auto v1 = 0xc00U;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e780U: { // d018 add.b (a0)+, d0
            next = 0x1e782U;
            const auto ea2 = r.address[0]; r.address[0] += 1U;
            const auto v1 = m.add((r.data[0] & 0xffU), m.byte(ea2), 8U);
            m.db(0, v1);
            break;
        }
        case 0x1e782U: { // 3b400008 move.w d0, $8(a5)
            next = 0x1e786U;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x8U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e786U: { // 102d0001 move.b $1(a5), d0
            next = 0x1e78aU;
            const auto v1 = m.byte((r.address[5] + 0x1U));
            m.db(0, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1e78aU: { // 0200fffc andi.b #$fc, d0
            next = 0x1e78eU;
            const auto v1 = ((r.data[0] & 0xffU) & 0xfcU) & 0xffU;
            m.db(0, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1e78eU: { // 8018 or.b (a0)+, d0
            next = 0x1e790U;
            const auto ea2 = r.address[0]; r.address[0] += 1U;
            const auto v1 = ((r.data[0] & 0xffU) | m.byte(ea2)) & 0xffU;
            m.db(0, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1e790U: { // 1b400001 move.b d0, $1(a5)
            next = 0x1e794U;
            const auto v1 = (r.data[0] & 0xffU);
            m.byte((r.address[5] + 0x1U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1e7aaU: { // 302d005c move.w $5c(a5), d0
            next = 0x1e7aeU;
            const auto v1 = m.word((r.address[5] + 0x5cU));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e7aeU: { // 06400080 addi.w #$80, d0
            next = 0x1e7b2U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x80U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1e7b2U: { // 02400700 andi.w #$700, d0
            next = 0x1e7b6U;
            const auto v1 = ((r.data[0] & 0xffffU) & 0x700U) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e7b6U: { // e040 asr.w #$8, d0
            next = 0x1e7b8U;
            m.shift_word(0, 8U, false, true);
            break;
        }
        case 0x1e7b8U: { // 3407 move.w d7, d2
            next = 0x1e7baU;
            const auto v1 = (r.data[7] & 0xffffU);
            m.dw(2, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e7baU: { // d447 add.w d7, d2
            next = 0x1e7bcU;
            const auto v1 = m.add((r.data[2] & 0xffffU), (r.data[7] & 0xffffU), 16U);
            m.dw(2, v1);
            break;
        }
        case 0x1e7bcU: { // d447 add.w d7, d2
            next = 0x1e7beU;
            const auto v1 = m.add((r.data[2] & 0xffffU), (r.data[7] & 0xffffU), 16U);
            m.dw(2, v1);
            break;
        }
    default: return false;
    }
    return true;
}
