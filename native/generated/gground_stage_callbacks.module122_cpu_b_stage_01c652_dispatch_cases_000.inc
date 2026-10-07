
static bool cpu_b_stage_01c652_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1c652U: { // 526d0054 addq.w #$1, $54(a5)
            next = 0x1c656U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x54U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1c656U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x1c65cU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1c65cU: { // 6668 bne.b $1c6c6
            next = 0x1c65eU;
            if ((r.status & 4U) == 0U) { next = 0x1c6c6U; transfer_kind = 1U; }
            break;
        }
        case 0x1c65eU: { // 302d0054 move.w $54(a5), d0
            next = 0x1c662U;
            const auto source_address = r.address[5] + 0x54U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c662U: { // 02400030 andi.w #$30, d0
            next = 0x1c666U;
            const auto source_value = 0x30U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1c666U: { // 660a bne.b $1c672
            next = 0x1c668U;
            if ((r.status & 4U) == 0U) { next = 0x1c672U; transfer_kind = 1U; }
            break;
        }
        case 0x1c66cU: { // 4a2d0060 tst.b $60(a5)
            next = 0x1c670U;
            const auto destination_address = r.address[5] + 0x60U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1c670U: { // 661c bne.b $1c68e
            next = 0x1c672U;
            if ((r.status & 4U) == 0U) { next = 0x1c68eU; transfer_kind = 1U; }
            break;
        }
        case 0x1c672U: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1c678U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c678U: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1c67eU;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c67eU: { // 422d003f clr.b $3f(a5)
            next = 0x1c682U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c682U: { // 422d003e clr.b $3e(a5)
            next = 0x1c686U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c686U: { // 08ed00070041 bset.b #$7, $41(a5)
            next = 0x1c68cU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c68eU: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1c694U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c694U: { // 08ad00050041 bclr.b #$5, $41(a5)
            next = 0x1c69aU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c69aU: { // 08ed00030041 bset.b #$3, $41(a5)
            next = 0x1c6a0U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c6a0U: { // 1b7c00000058 move.b #$0, $58(a5)
            next = 0x1c6a6U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x58U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1c6a6U: { // 08ad00070041 bclr.b #$7, $41(a5)
            next = 0x1c6acU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c6b0U: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1c6b6U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c6b6U: { // 422d003f clr.b $3f(a5)
            next = 0x1c6baU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c6baU: { // 422d003e clr.b $3e(a5)
            next = 0x1c6beU;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c6beU: { // 3b7c001e005a move.w #$1e, $5a(a5)
            next = 0x1c6c4U;
            const auto value = 0x1eU;
            const auto destination_address = r.address[5] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c6c6U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1c6caU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1c6caU: { // 6704 beq.b $1c6d0
            next = 0x1c6ccU;
            if ((r.status & 4U) != 0U) { next = 0x1c6d0U; transfer_kind = 1U; }
            break;
        }
        case 0x1c6ccU: { // 6000130c bra.w $1d9da
            next = 0x1c6d0U;
            if (true) { next = 0x1d9daU; transfer_kind = 1U; }
            break;
        }
        case 0x1c6d0U: { // 0c2d00100058 cmpi.b #$10, $58(a5)
            next = 0x1c6d6U;
            const auto source_value = 0x10U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x1c6d6U: { // 6c16 bge.b $1c6ee
            next = 0x1c6d8U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1c6eeU; transfer_kind = 1U; }
            break;
        }
        case 0x1c6d8U: { // 422d003e clr.b $3e(a5)
            next = 0x1c6dcU;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c6dcU: { // 522d0058 addq.b #$1, $58(a5)
            next = 0x1c6e0U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x1c6e0U: { // 3b7c00000074 move.w #$0, $74(a5)
            next = 0x1c6e6U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c6e6U: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1c6ecU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c6eeU: { // 4a2d003e tst.b $3e(a5)
            next = 0x1c6f2U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
    default: return false;
    }
    return true;
}
