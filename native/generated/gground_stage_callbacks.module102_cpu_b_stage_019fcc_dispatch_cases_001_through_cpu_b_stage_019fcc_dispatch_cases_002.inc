
static bool cpu_b_stage_019fcc_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1a05aU: { // 0c7400050044 cmpi.w #$5, $44(a4, d0.w)
            next = 0x1a060U;
            const auto source_value = 0x5U;
            const auto destination_address = r.address[4] + 0x44U + static_cast<std::int16_t>(r.data[0]);
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a060U: { // 6706 beq.b $1a068
            next = 0x1a062U;
            if ((r.status & 4U) != 0U) { next = 0x1a068U; transfer_kind = 1U; }
            break;
        }
        case 0x1a062U: { // 51c9ffea dbra d1, $1a04e
            next = 0x1a066U;
            m.dw(1U, r.data[1] - 1U);
            if ((r.data[1] & 0xffffU) != 0xffffU) next = 0x1a04eU;
            break;
        }
        case 0x1a066U: { // 6076 bra.b $1a0de
            next = 0x1a068U;
            if (true) { next = 0x1a0deU; transfer_kind = 1U; }
            break;
        }
        case 0x1a068U: { // 3b400052 move.w d0, $52(a5)
            next = 0x1a06cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x52U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a06cU: { // d8c0 adda.w d0, a4
            next = 0x1a06eU;
            const auto source_value = r.data[0];
            r.address[4] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1a06eU: { // 302c0012 move.w $12(a4), d0
            next = 0x1a072U;
            const auto source_address = r.address[4] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a072U: { // 322c001a move.w $1a(a4), d1
            next = 0x1a076U;
            const auto source_address = r.address[4] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a076U: { // 906e0012 sub.w $12(a6), d0
            next = 0x1a07aU;
            const auto source_address = r.address[6] + 0x12U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a07aU: { // 6602 bne.b $1a07e
            next = 0x1a07cU;
            if ((r.status & 4U) == 0U) { next = 0x1a07eU; transfer_kind = 1U; }
            break;
        }
        case 0x1a07cU: { // 7001 moveq #$1, d0
            next = 0x1a07eU;
            r.data[0] = 0x1U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1a07eU: { // 3a00 move.w d0, d5
            next = 0x1a080U;
            const auto value = r.data[0];
            m.dw(5U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a080U: { // 926e001a sub.w $1a(a6), d1
            next = 0x1a084U;
            const auto source_address = r.address[6] + 0x1aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1a090U: { // 3d41005a move.w d1, $5a(a6)
            next = 0x1a094U;
            const auto value = r.data[1];
            const auto destination_address = r.address[6] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a09aU: { // 3400 move.w d0, d2
            next = 0x1a09cU;
            const auto value = r.data[0];
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a09cU: { // 7600 moveq #$0, d3
            next = 0x1a09eU;
            r.data[3] = 0x0U;
            m.logic(r.data[3], 32U);
            break;
        }
        case 0x1a09eU: { // 3605 move.w d5, d3
            next = 0x1a0a0U;
            const auto value = r.data[5];
            m.dw(3U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a0a0U: { // 4843 swap d3
            next = 0x1a0a2U;
            const auto value = (r.data[3] << 16U) | (r.data[3] >> 16U);
            r.data[3] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1a0a2U: { // e883 asr.l #$4, d3
            next = 0x1a0a4U;
            auto value = r.data[3];
            bool carry = false;
            bool overflow = false;
            for (unsigned step = 0U; step != 4U; ++step) {
                const auto before = value;
                carry = (before & 1U) != 0U;
                value = (before >> 1U) | (before & 0x80000000U);
            }
            r.data[3] = value;
            m.logic(value, 32U);
            r.status = static_cast<std::uint16_t>((r.status & ~0x13U) | (carry ? 0x11U : 0U) | (overflow ? 2U : 0U));
            break;
        }
        case 0x1a0a6U: { // 3803 move.w d3, d4
            next = 0x1a0a8U;
            const auto value = r.data[3];
            m.dw(4U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a0a8U: { // 243c00003800 move.l #$3800, d2
            next = 0x1a0aeU;
            const auto value = 0x3800U;
            r.data[2] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1a0b0U: { // 02820000ffff andi.l #$ffff, d2
            next = 0x1a0b6U;
            r.data[2] &= 0xffffU;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0x1a0b6U: { // e182 asl.l #$8, d2
            next = 0x1a0b8U;
            auto value = r.data[2];
            bool carry = false;
            bool overflow = false;
            for (unsigned step = 0U; step != 8U; ++step) {
                const auto before = value;
                carry = (before & 0x80000000U) != 0U;
                value <<= 1U;
                overflow = overflow || ((before ^ value) & 0x80000000U) != 0U;
            }
            r.data[2] = value;
            m.logic(value, 32U);
            r.status = static_cast<std::uint16_t>((r.status & ~0x13U) | (carry ? 0x11U : 0U) | (overflow ? 2U : 0U));
            break;
        }
        case 0x1a0b8U: { // 5343 subq.w #$1, d3
            next = 0x1a0baU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[3];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(3U, value);
            break;
        }
        case 0x1a0baU: { // 4843 swap d3
            next = 0x1a0bcU;
            const auto value = (r.data[3] << 16U) | (r.data[3] >> 16U);
            r.data[3] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1a0bcU: { // 4243 clr.w d3
            next = 0x1a0beU;
            const auto value = r.data[3];
            (void)value;
            m.dw(3U, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1a0beU: { // e883 asr.l #$4, d3
            next = 0x1a0c0U;
            auto value = r.data[3];
            bool carry = false;
            bool overflow = false;
            for (unsigned step = 0U; step != 4U; ++step) {
                const auto before = value;
                carry = (before & 1U) != 0U;
                value = (before >> 1U) | (before & 0x80000000U);
            }
            r.data[3] = value;
            m.logic(value, 32U);
            r.status = static_cast<std::uint16_t>((r.status & ~0x13U) | (carry ? 0x11U : 0U) | (overflow ? 2U : 0U));
            break;
        }
        case 0x1a0c0U: { // 9682 sub.l d2, d3
            next = 0x1a0c2U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[3];
            const auto value = m.sub(destination_value, source_value, 32U);
            r.data[3] = value;
            break;
        }
        case 0x1a0c2U: { // 48c0 ext.l d0
            next = 0x1a0c4U;
            r.data[0] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[0])));
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1a0c4U: { // e980 asl.l #$4, d0
            next = 0x1a0c6U;
            auto value = r.data[0];
            bool carry = false;
            bool overflow = false;
            for (unsigned step = 0U; step != 4U; ++step) {
                const auto before = value;
                carry = (before & 0x80000000U) != 0U;
                value <<= 1U;
                overflow = overflow || ((before ^ value) & 0x80000000U) != 0U;
            }
            r.data[0] = value;
            m.logic(value, 32U);
            r.status = static_cast<std::uint16_t>((r.status & ~0x13U) | (carry ? 0x11U : 0U) | (overflow ? 2U : 0U));
            break;
        }
        case 0x1a0c6U: { // 48c1 ext.l d1
            next = 0x1a0c8U;
            r.data[1] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[1])));
            m.logic(r.data[1], 32U);
            break;
        }
    default: return false;
    }
    return true;
}

static bool cpu_b_stage_019fcc_dispatch_cases_002([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1a0c8U: { // e981 asl.l #$4, d1
            next = 0x1a0caU;
            auto value = r.data[1];
            bool carry = false;
            bool overflow = false;
            for (unsigned step = 0U; step != 4U; ++step) {
                const auto before = value;
                carry = (before & 0x80000000U) != 0U;
                value <<= 1U;
                overflow = overflow || ((before ^ value) & 0x80000000U) != 0U;
            }
            r.data[1] = value;
            m.logic(value, 32U);
            r.status = static_cast<std::uint16_t>((r.status & ~0x13U) | (carry ? 0x11U : 0U) | (overflow ? 2U : 0U));
            break;
        }
        case 0x1a0caU: { // 2d40001e move.l d0, $1e(a6)
            next = 0x1a0ceU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x1eU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1a0ceU: { // 2d430022 move.l d3, $22(a6)
            next = 0x1a0d2U;
            const auto value = r.data[3];
            const auto destination_address = r.address[6] + 0x22U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1a0d2U: { // 2d410026 move.l d1, $26(a6)
            next = 0x1a0d6U;
            const auto value = r.data[1];
            const auto destination_address = r.address[6] + 0x26U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1a0d6U: { // 512d0051 subq.b #$8, $51(a5)
            next = 0x1a0daU;
            const auto source_value = 0x8U;
            const auto destination_address = r.address[5] + 0x51U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x1a0daU: { // 3cbc8004 move.w #$8004, (a6)
            next = 0x1a0deU;
            const auto value = 0x8004U;
            const auto destination_address = r.address[6];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
