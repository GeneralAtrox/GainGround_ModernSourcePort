
static bool cpu_b_stage_gap_01d04e_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1d04eU: { // 4a2d003f tst.b $3f(a5)
            next = 0x1d052U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x1d052U: { // 6704 beq.b $1d058
            next = 0x1d054U;
            if ((r.status & 4U) != 0U) { next = 0x1d058U; transfer_kind = 1U; }
            break;
        }
        case 0x1d054U: { // 60000984 bra.w $1d9da
            next = 0x1d058U;
            next = 0x1d9daU; transfer_kind = 1U;
            break;
        }
        case 0x1d058U: { // 4a2d003e tst.b $3e(a5)
            next = 0x1d05cU;
            m.logic(m.byte((r.address[5] + 0x3eU)), 8U);
            break;
        }
        case 0x1d05cU: { // 6704 beq.b $1d062
            next = 0x1d05eU;
            if ((r.status & 4U) != 0U) { next = 0x1d062U; transfer_kind = 1U; }
            break;
        }
        case 0x1d05eU: { // 6000098a bra.w $1d9ea
            next = 0x1d062U;
            next = 0x1d9eaU; transfer_kind = 1U;
            break;
        }
        case 0x1d062U: { // 08ad00010040 bclr.b #$1, $40(a5)
            next = 0x1d068U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 1U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 & ~(1U << 1U)) & 0xffU);
            break;
        }
        case 0x1d06cU: { // 082d00010040 btst.b #$1, $40(a5)
            next = 0x1d072U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 1U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x1d072U: { // 660a bne.b $1d07e
            next = 0x1d074U;
            if ((r.status & 4U) == 0U) { next = 0x1d07eU; transfer_kind = 1U; }
            break;
        }
        case 0x1d074U: { // 082d00060040 btst.b #$6, $40(a5)
            next = 0x1d07aU;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 6U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x1d07aU: { // 6602 bne.b $1d07e
            next = 0x1d07cU;
            if ((r.status & 4U) == 0U) { next = 0x1d07eU; transfer_kind = 1U; }
            break;
        }
        case 0x1d07cU: { // 6026 bra.b $1d0a4
            next = 0x1d07eU;
            next = 0x1d0a4U; transfer_kind = 1U;
            break;
        }
        case 0x1d07eU: { // 08ad00010040 bclr.b #$1, $40(a5)
            next = 0x1d084U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 1U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 & ~(1U << 1U)) & 0xffU);
            break;
        }
        case 0x1d084U: { // 08ad00050040 bclr.b #$5, $40(a5)
            next = 0x1d08aU;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 5U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 & ~(1U << 5U)) & 0xffU);
            break;
        }
        case 0x1d08aU: { // 3b7c0200005c move.w #$200, $5c(a5)
            next = 0x1d090U;
            const auto v1 = 0x200U;
            m.word((r.address[5] + 0x5cU), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1d090U: { // 08ad00000040 bclr.b #$0, $40(a5)
            next = 0x1d096U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 0U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 & ~(1U << 0U)) & 0xffU);
            break;
        }
        case 0x1d096U: { // 08ed00030040 bset.b #$3, $40(a5)
            next = 0x1d09cU;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 | (1U << 3U)) & 0xffU);
            break;
        }
        case 0x1d09cU: { // 08ed00020040 bset.b #$2, $40(a5)
            next = 0x1d0a2U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 | (1U << 2U)) & 0xffU);
            break;
        }
        case 0x1d0a4U: { // 08ad00000040 bclr.b #$0, $40(a5)
            next = 0x1d0aaU;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 0U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 & ~(1U << 0U)) & 0xffU);
            break;
        }
        case 0x1d0aaU: { // 08ad00030041 bclr.b #$3, $41(a5)
            next = 0x1d0b0U;
            const auto v1 = m.byte((r.address[5] + 0x41U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x41U), (v1 & ~(1U << 3U)) & 0xffU);
            break;
        }
        case 0x1d0b0U: { // 08ad00010040 bclr.b #$1, $40(a5)
            next = 0x1d0b6U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 1U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 & ~(1U << 1U)) & 0xffU);
            break;
        }
        case 0x1d0b6U: { // 08ed00030040 bset.b #$3, $40(a5)
            next = 0x1d0bcU;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 | (1U << 3U)) & 0xffU);
            break;
        }
        case 0x1d0bcU: { // 08ed00020040 bset.b #$2, $40(a5)
            next = 0x1d0c2U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 | (1U << 2U)) & 0xffU);
            break;
        }
        case 0x1d0c2U: { // 08ad00050040 bclr.b #$5, $40(a5)
            next = 0x1d0c8U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 5U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 & ~(1U << 5U)) & 0xffU);
            break;
        }
        case 0x1d0c8U: { // 1b7c0009000b move.b #$9, $b(a5)
            next = 0x1d0ceU;
            const auto v1 = 0x9U;
            m.byte((r.address[5] + 0xbU), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1d0ceU: { // 3b7c00000074 move.w #$0, $74(a5)
            next = 0x1d0d4U;
            const auto v1 = 0x0U;
            m.word((r.address[5] + 0x74U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1d9daU: { // 2b7c0001ed980002 move.l #$1ed98, $2(a5)
            next = 0x1d9e2U;
            const auto v1 = 0x1ed98U;
            m.lng((r.address[5] + 0x2U), v1);
            m.logic(v1, 32U);
            break;
        }
        case 0x1d9e2U: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x1d9e8U;
            const auto v1 = 0x0U;
            m.word((r.address[5] + 0x54U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1d9eaU: { // 1b7c00010054 move.b #$1, $54(a5)
            next = 0x1d9f0U;
            const auto v1 = 0x1U;
            m.byte((r.address[5] + 0x54U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1d9f0U: { // 532d003e subq.b #$1, $3e(a5)
            next = 0x1d9f4U;
            const auto v1 = m.sub(m.byte((r.address[5] + 0x3eU)), 0x1U, 8U);
            m.byte((r.address[5] + 0x3eU), v1);
            break;
        }
        case 0x1d9f4U: { // 022d00fe003e andi.b #$fe, $3e(a5)
            next = 0x1d9faU;
            const auto v1 = (m.byte((r.address[5] + 0x3eU)) & 0xfeU) & 0xffU;
            m.byte((r.address[5] + 0x3eU), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1d9faU: { // 08ed00010040 bset.b #$1, $40(a5)
            next = 0x1da00U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 1U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 | (1U << 1U)) & 0xffU);
            break;
        }
        case 0x1da00U: { // 08ed00030040 bset.b #$3, $40(a5)
            next = 0x1da06U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 | (1U << 3U)) & 0xffU);
            break;
        }
        case 0x1da06U: { // 066d0080005c addi.w #$80, $5c(a5)
            next = 0x1da0cU;
            const auto v1 = m.add(m.word((r.address[5] + 0x5cU)), 0x80U, 16U);
            m.word((r.address[5] + 0x5cU), v1);
            break;
        }
        case 0x1da0cU: { // 026d0780005c andi.w #$780, $5c(a5)
            next = 0x1da12U;
            const auto v1 = (m.word((r.address[5] + 0x5cU)) & 0x780U) & 0xffffU;
            m.word((r.address[5] + 0x5cU), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1da12U: { // 1b7c0009005e move.b #$9, $5e(a5)
            next = 0x1da18U;
            const auto v1 = 0x9U;
            m.byte((r.address[5] + 0x5eU), v1);
            m.logic(v1, 8U);
            break;
        }
    default: return false;
    }
    return true;
}
