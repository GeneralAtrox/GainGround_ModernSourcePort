
static bool cpu_b_stage_018664_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x186eeU: { // 322d005a move.w $5a(a5), d1
            next = 0x186f2U;
            const auto source_address = r.address[5] + 0x5aU;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x186f2U: { // b26d0054 cmp.w $54(a5), d1
            next = 0x186f6U;
            const auto source_address = r.address[5] + 0x54U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x186f6U: { // 660000be bne.w $187b6
            next = 0x186faU;
            if ((r.status & 4U) == 0U) { next = 0x187b6U; transfer_kind = 1U; }
            break;
        }
        case 0x186feU: { // 4a40 tst.w d0
            next = 0x18700U;
            const auto value = r.data[0];
            m.logic(value, 16U);
            break;
        }
        case 0x18700U: { // 6b0000b4 bmi.w $187b6
            next = 0x18704U;
            if ((r.status & 8U) != 0U) { next = 0x187b6U; transfer_kind = 1U; }
            break;
        }
        case 0x1870aU: { // 7600 moveq #$0, d3
            next = 0x1870cU;
            r.data[3] = 0x0U;
            m.logic(r.data[3], 32U);
            break;
        }
        case 0x1870cU: { // 3400 move.w d0, d2
            next = 0x1870eU;
            const auto value = r.data[0];
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1870eU: { // 670a beq.b $1871a
            next = 0x18710U;
            if ((r.status & 4U) != 0U) { next = 0x1871aU; transfer_kind = 1U; }
            break;
        }
        case 0x18710U: { // 362d0056 move.w $56(a5), d3
            next = 0x18714U;
            const auto source_address = r.address[5] + 0x56U;
            const auto value = m.word(source_address);
            m.dw(3U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18714U: { // 966d0012 sub.w $12(a5), d3
            next = 0x18718U;
            const auto source_address = r.address[5] + 0x12U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[3];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(3U, value);
            break;
        }
        case 0x18718U: { // 600a bra.b $18724
            next = 0x1871aU;
            if (true) { next = 0x18724U; transfer_kind = 1U; }
            break;
        }
        case 0x1871aU: { // 3401 move.w d1, d2
            next = 0x1871cU;
            const auto value = r.data[1];
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1871cU: { // 362d0058 move.w $58(a5), d3
            next = 0x18720U;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.word(source_address);
            m.dw(3U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18720U: { // 966d001a sub.w $1a(a5), d3
            next = 0x18724U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[3];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(3U, value);
            break;
        }
        case 0x18724U: { // 4843 swap d3
            next = 0x18726U;
            const auto value = (r.data[3] << 16U) | (r.data[3] >> 16U);
            r.data[3] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x18726U: { // e483 asr.l #$2, d3
            next = 0x18728U;
            auto value = r.data[3];
            bool carry = false;
            bool overflow = false;
            for (unsigned step = 0U; step != 2U; ++step) {
                const auto before = value;
                carry = (before & 1U) != 0U;
                value = (before >> 1U) | (before & 0x80000000U);
            }
            r.data[3] = value;
            m.logic(value, 32U);
            r.status = static_cast<std::uint16_t>((r.status & ~0x13U) | (carry ? 0x11U : 0U) | (overflow ? 2U : 0U));
            break;
        }
        case 0x1872aU: { // e543 asl.w #$2, d3
            next = 0x1872cU;
            m.asl_word(3U, 2U);
            break;
        }
        case 0x1872cU: { // 243c001c0000 move.l #$1c0000, d2
            next = 0x18732U;
            const auto value = 0x1c0000U;
            r.data[2] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x18734U: { // 48c0 ext.l d0
            next = 0x18736U;
            r.data[0] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[0])));
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x18736U: { // 48c2 ext.l d2
            next = 0x18738U;
            r.data[2] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[2])));
            m.logic(r.data[2], 32U);
            break;
        }
        case 0x18738U: { // 48c1 ext.l d1
            next = 0x1873aU;
            r.data[1] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[1])));
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1873aU: { // e980 asl.l #$4, d0
            next = 0x1873cU;
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
        case 0x1873cU: { // e982 asl.l #$4, d2
            next = 0x1873eU;
            auto value = r.data[2];
            bool carry = false;
            bool overflow = false;
            for (unsigned step = 0U; step != 4U; ++step) {
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
        case 0x1873eU: { // 4482 neg.l d2
            next = 0x18740U;
            const auto old = r.data[2];
            const auto value = m.sub(0U, old, 32U);
            r.data[2] = value;
            break;
        }
        case 0x18740U: { // e981 asl.l #$4, d1
            next = 0x18742U;
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
        case 0x18742U: { // 1cbc0080 move.b #$80, (a6)
            next = 0x18746U;
            const auto value = 0x80U;
            const auto destination_address = r.address[6];
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18746U: { // 1d7c00040001 move.b #$4, $1(a6)
            next = 0x1874cU;
            const auto value = 0x4U;
            const auto destination_address = r.address[6] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1874cU: { // 2d7c00018b600002 move.l #$18b60, $2(a6)
            next = 0x18754U;
            const auto value = 0x18b60U;
            const auto destination_address = r.address[6] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x18754U: { // 3d7c01f00006 move.w #$1f0, $6(a6)
            next = 0x1875aU;
            const auto value = 0x1f0U;
            const auto destination_address = r.address[6] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1875aU: { // 3d7c0c150008 move.w #$c15, $8(a6)
            next = 0x18760U;
            const auto value = 0xc15U;
            const auto destination_address = r.address[6] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
