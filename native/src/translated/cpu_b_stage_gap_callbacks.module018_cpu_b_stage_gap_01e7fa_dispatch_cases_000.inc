
static bool cpu_b_stage_gap_01e7fa_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
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
        case 0x1e7feU: { // 3007 move.w d7, d0
            next = 0x1e800U;
            const auto v1 = (r.data[7] & 0xffffU);
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e800U: { // e940 asl.w #$4, d0
            next = 0x1e802U;
            m.asl_word(0, 4U);
            break;
        }
        case 0x1e802U: { // 3200 move.w d0, d1
            next = 0x1e804U;
            const auto v1 = (r.data[0] & 0xffffU);
            m.dw(1, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e804U: { // d240 add.w d0, d1
            next = 0x1e806U;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[0] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e806U: { // d240 add.w d0, d1
            next = 0x1e808U;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[0] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e808U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x1e80eU;
            const auto v1 = m.byte((r.address[5] + 0x41U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x1e80eU: { // 6742 beq.b $1e852
            next = 0x1e810U;
            if ((r.status & 4U) != 0U) { next = 0x1e852U; transfer_kind = 1U; }
            break;
        }
        case 0x1e810U: { // d247 add.w d7, d1
            next = 0x1e812U;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[7] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e812U: { // d247 add.w d7, d1
            next = 0x1e814U;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[7] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e814U: { // 7000 moveq #$0, d0
            next = 0x1e816U;
            r.data[0] = 0x0U; m.logic(r.data[0], 32U);
            break;
        }
        case 0x1e816U: { // 102d0036 move.b $36(a5), d0
            next = 0x1e81aU;
            const auto v1 = m.byte((r.address[5] + 0x36U));
            m.db(0, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1e81aU: { // 3400 move.w d0, d2
            next = 0x1e81cU;
            const auto v1 = (r.data[0] & 0xffffU);
            m.dw(2, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e81cU: { // e540 asl.w #$2, d0
            next = 0x1e81eU;
            m.asl_word(0, 2U);
            break;
        }
        case 0x1e81eU: { // e342 asl.w #$1, d2
            next = 0x1e820U;
            m.asl_word(2, 1U);
            break;
        }
        case 0x1e820U: { // d042 add.w d2, d0
            next = 0x1e822U;
            const auto v1 = m.add((r.data[0] & 0xffffU), (r.data[2] & 0xffffU), 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1e822U: { // 342d0056 move.w $56(a5), d2
            next = 0x1e826U;
            const auto v1 = m.word((r.address[5] + 0x56U));
            m.dw(2, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e826U: { // d440 add.w d0, d2
            next = 0x1e828U;
            const auto v1 = m.add((r.data[2] & 0xffffU), (r.data[0] & 0xffffU), 16U);
            m.dw(2, v1);
            break;
        }
        case 0x1e828U: { // 3b420056 move.w d2, $56(a5)
            next = 0x1e82cU;
            const auto v1 = (r.data[2] & 0xffffU);
            m.word((r.address[5] + 0x56U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e82cU: { // 02420600 andi.w #$600, d2
            next = 0x1e830U;
            const auto v1 = ((r.data[2] & 0xffffU) & 0x600U) & 0xffffU;
            m.dw(2, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e830U: { // ef5a rol.w #$7, d2
            next = 0x1e832U;
            const auto v1 = ((r.data[2] << 7U) | ((r.data[2] & 0xffffU) >> 9U)) & 0xffffU;
            m.dw(2, v1); m.logic(v1, 16U); r.status = static_cast<std::uint16_t>((r.status & ~1U) | (v1 & 1U));
            break;
        }
        case 0x1e832U: { // 3007 move.w d7, d0
            next = 0x1e834U;
            const auto v1 = (r.data[7] & 0xffffU);
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1e834U: { // 9240 sub.w d0, d1
            next = 0x1e836U;
            const auto v1 = m.sub((r.data[1] & 0xffffU), (r.data[0] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e836U: { // d240 add.w d0, d1
            next = 0x1e838U;
            const auto v1 = m.add((r.data[1] & 0xffffU), (r.data[0] & 0xffffU), 16U);
            m.dw(1, v1);
            break;
        }
        case 0x1e838U: { // 51cafffc dbra d2, $1e836
            next = 0x1e83cU;
            const auto v1 = static_cast<std::uint16_t>(r.data[2] - 1U); m.dw(2, v1);
            if (v1 != 0xffffU) { next = 0x1e836U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}
