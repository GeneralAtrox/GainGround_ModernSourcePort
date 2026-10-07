
static bool cpu_b_stage_01c51c_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1c51cU:
    case 0x1c520U:
    case 0x1c526U:
    case 0x1c528U:
    case 0x1c52cU:
    case 0x1c530U:
    case 0x1c532U:
    case 0x1c536U:
    case 0x1c53aU:
    case 0x1c53cU:
    case 0x1c542U:
    case 0x1c548U:
    case 0x1c54cU:
    case 0x1c550U:
    case 0x1c556U:
    case 0x1c55cU:
    case 0x1c55eU:
    case 0x1c564U:
    case 0x1c56aU:
    case 0x1c570U:
    case 0x1c576U:
    case 0x1c57cU:
    case 0x1c580U:
    case 0x1c586U:
    case 0x1c58aU:
    case 0x1c58eU:
    case 0x1c590U:
    case 0x1c594U:
    case 0x1c596U:
    case 0x1c59aU:
    case 0x1c5a0U:
    case 0x1c5a2U:
    case 0x1c5a8U:
    case 0x1c5aaU:
    case 0x1c5aeU:
    case 0x1c5b2U:
    case 0x1c5b8U:
    case 0x1c5beU:
    case 0x1c5c0U:
    case 0x1c5c4U:
    case 0x1c5c6U:
    case 0x1c5caU:
    case 0x1c5ceU:
    case 0x1c5d0U:
    case 0x1c5d4U:
    case 0x1c5d6U:
    case 0x1c5dcU:
    case 0x1c5deU:
    case 0x1c5e0U:
    case 0x1c5e6U:
    case 0x1c5e8U:
    case 0x1c5eeU:
    case 0x1c5f0U:
    case 0x1c5f4U:
    case 0x1c5faU:
    case 0x1c600U:
    case 0x1c604U:
    case 0x1c606U:
    case 0x1c60cU:
    case 0x1c612U:
    case 0x1c618U:
    case 0x1c61eU:
    case 0x1c624U:
        return true;
    default: return false;
    }
}

static bool cpu_b_stage_01c51c_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1c51cU: { // 526d0054 addq.w #$1, $54(a5)
            next = 0x1c520U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x54U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1c520U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x1c526U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1c526U: { // 6668 bne.b $1c590
            next = 0x1c528U;
            if ((r.status & 4U) == 0U) { next = 0x1c590U; transfer_kind = 1U; }
            break;
        }
        case 0x1c528U: { // 302d0054 move.w $54(a5), d0
            next = 0x1c52cU;
            const auto source_address = r.address[5] + 0x54U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c52cU: { // 0240001f andi.w #$1f, d0
            next = 0x1c530U;
            const auto source_value = 0x1fU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1c530U: { // 660a bne.b $1c53c
            next = 0x1c532U;
            if ((r.status & 4U) == 0U) { next = 0x1c53cU; transfer_kind = 1U; }
            break;
        }
        case 0x1c536U: { // 4a2d0060 tst.b $60(a5)
            next = 0x1c53aU;
            const auto destination_address = r.address[5] + 0x60U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1c53aU: { // 6622 bne.b $1c55e
            next = 0x1c53cU;
            if ((r.status & 4U) == 0U) { next = 0x1c55eU; transfer_kind = 1U; }
            break;
        }
        case 0x1c53cU: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1c542U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c542U: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1c548U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c548U: { // 422d003f clr.b $3f(a5)
            next = 0x1c54cU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c54cU: { // 422d003e clr.b $3e(a5)
            next = 0x1c550U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c550U: { // 08ed00070041 bset.b #$7, $41(a5)
            next = 0x1c556U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c556U: { // 3b7c001e005a move.w #$1e, $5a(a5)
            next = 0x1c55cU;
            const auto value = 0x1eU;
            const auto destination_address = r.address[5] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c55eU: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1c564U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c564U: { // 08ad00050041 bclr.b #$5, $41(a5)
            next = 0x1c56aU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c56aU: { // 08ed00030041 bset.b #$3, $41(a5)
            next = 0x1c570U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c570U: { // 1b7c00000058 move.b #$0, $58(a5)
            next = 0x1c576U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x58U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1c576U: { // 08ad00070041 bclr.b #$7, $41(a5)
            next = 0x1c57cU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c580U: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1c586U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c586U: { // 422d003f clr.b $3f(a5)
            next = 0x1c58aU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c58aU: { // 422d003e clr.b $3e(a5)
            next = 0x1c58eU;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c590U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1c594U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1c594U: { // 6704 beq.b $1c59a
            next = 0x1c596U;
            if ((r.status & 4U) != 0U) { next = 0x1c59aU; transfer_kind = 1U; }
            break;
        }
        case 0x1c596U: { // 60001442 bra.w $1d9da
            next = 0x1c59aU;
            if (true) { next = 0x1d9daU; transfer_kind = 1U; }
            break;
        }
        case 0x1c59aU: { // 082d00050041 btst.b #$5, $41(a5)
            next = 0x1c5a0U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1c5a0U: { // 6646 bne.b $1c5e8
            next = 0x1c5a2U;
            if ((r.status & 4U) == 0U) { next = 0x1c5e8U; transfer_kind = 1U; }
            break;
        }
        case 0x1c5a2U: { // 0c2d00200058 cmpi.b #$20, $58(a5)
            next = 0x1c5a8U;
            const auto source_value = 0x20U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x1c5a8U: { // 6c16 bge.b $1c5c0
            next = 0x1c5aaU;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1c5c0U; transfer_kind = 1U; }
            break;
        }
        case 0x1c5aaU: { // 422d003e clr.b $3e(a5)
            next = 0x1c5aeU;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c5aeU: { // 522d0058 addq.b #$1, $58(a5)
            next = 0x1c5b2U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x1c5b2U: { // 3b7c00000074 move.w #$0, $74(a5)
            next = 0x1c5b8U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
