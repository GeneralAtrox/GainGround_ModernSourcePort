
static bool cpu_b_stage_01cb42_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1cb42U:
    case 0x1cb46U:
    case 0x1cb4aU:
    case 0x1cb4eU:
    case 0x1cb54U:
    case 0x1cb56U:
    case 0x1cb5cU:
    case 0x1cb60U:
    case 0x1cb64U:
    case 0x1cb68U:
    case 0x1cb6aU:
    case 0x1cb70U:
    case 0x1cb72U:
    case 0x1cb78U:
    case 0x1cb7eU:
    case 0x1cb82U:
    case 0x1cb86U:
    case 0x1cb8cU:
    case 0x1cb92U:
    case 0x1cb94U:
    case 0x1cb9aU:
    case 0x1cba0U:
    case 0x1cba6U:
    case 0x1cbacU:
    case 0x1cbb2U:
    case 0x1cbb6U:
    case 0x1cbbaU:
    case 0x1cbbcU:
    case 0x1cbc0U:
    case 0x1cbc2U:
    case 0x1cbc8U:
    case 0x1cbcaU:
    case 0x1cbd0U:
    case 0x1cbd2U:
    case 0x1cbd8U:
    case 0x1cbdaU:
    case 0x1cbdeU:
    case 0x1cbe2U:
    case 0x1cbe8U:
    case 0x1cbeeU:
    case 0x1cbf0U:
    case 0x1cbf4U:
    case 0x1cbf6U:
    case 0x1cbfaU:
    case 0x1cbfeU:
    case 0x1cc00U:
    case 0x1cc04U:
    case 0x1cc06U:
    case 0x1cc0cU:
    case 0x1cc0eU:
    case 0x1cc10U:
    case 0x1cc16U:
    case 0x1cc18U:
    case 0x1cc1eU:
    case 0x1cc20U:
    case 0x1cc24U:
    case 0x1cc2aU:
    case 0x1cc30U:
    case 0x1cc34U:
    case 0x1cc36U:
    case 0x1cc3cU:
    case 0x1cc42U:
    case 0x1cc48U:
    case 0x1cc4eU:
    case 0x1cc54U:
    case 0x1cc56U:
    case 0x1cc5cU:
    case 0x1cc5eU:
    case 0x1cc60U:
    case 0x1cc64U:
        return true;
    default: return false;
    }
}

static bool cpu_b_stage_01cb42_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1cb42U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1cb46U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1cb46U: { // 6600010e bne.w $1cc56
            next = 0x1cb4aU;
            if ((r.status & 4U) == 0U) { next = 0x1cc56U; transfer_kind = 1U; }
            break;
        }
        case 0x1cb4aU: { // 526d0054 addq.w #$1, $54(a5)
            next = 0x1cb4eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x54U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1cb4eU: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x1cb54U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1cb54U: { // 6666 bne.b $1cbbc
            next = 0x1cb56U;
            if ((r.status & 4U) == 0U) { next = 0x1cbbcU; transfer_kind = 1U; }
            break;
        }
        case 0x1cb56U: { // 3b6d00760046 move.w $76(a5), $46(a5)
            next = 0x1cb5cU;
            const auto source_address = r.address[5] + 0x76U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x46U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1cb5cU: { // 422d003f clr.b $3f(a5)
            next = 0x1cb60U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1cb60U: { // 302d0054 move.w $54(a5), d0
            next = 0x1cb64U;
            const auto source_address = r.address[5] + 0x54U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1cb64U: { // 0240000f andi.w #$f, d0
            next = 0x1cb68U;
            const auto source_value = 0xfU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1cb68U: { // 6608 bne.b $1cb72
            next = 0x1cb6aU;
            if ((r.status & 4U) == 0U) { next = 0x1cb72U; transfer_kind = 1U; }
            break;
        }
        case 0x1cb70U: { // 6522 bcs.b $1cb94
            next = 0x1cb72U;
            if ((r.status & 1U) != 0U) { next = 0x1cb94U; transfer_kind = 1U; }
            break;
        }
        case 0x1cb72U: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1cb78U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1cb78U: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1cb7eU;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1cb7eU: { // 422d003f clr.b $3f(a5)
            next = 0x1cb82U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1cb82U: { // 422d003e clr.b $3e(a5)
            next = 0x1cb86U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1cb86U: { // 08ed00070041 bset.b #$7, $41(a5)
            next = 0x1cb8cU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1cb8cU: { // 3b7c001e005a move.w #$1e, $5a(a5)
            next = 0x1cb92U;
            const auto value = 0x1eU;
            const auto destination_address = r.address[5] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1cb94U: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1cb9aU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1cb9aU: { // 08ad00050041 bclr.b #$5, $41(a5)
            next = 0x1cba0U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1cba0U: { // 08ed00030041 bset.b #$3, $41(a5)
            next = 0x1cba6U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1cba6U: { // 1b7c00000058 move.b #$0, $58(a5)
            next = 0x1cbacU;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x58U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1cbacU: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1cbb2U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1cbb2U: { // 422d003f clr.b $3f(a5)
            next = 0x1cbb6U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1cbb6U: { // 422d003e clr.b $3e(a5)
            next = 0x1cbbaU;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1cbbcU: { // 4a2d003f tst.b $3f(a5)
            next = 0x1cbc0U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1cbc0U: { // 6708 beq.b $1cbca
            next = 0x1cbc2U;
            if ((r.status & 4U) != 0U) { next = 0x1cbcaU; transfer_kind = 1U; }
            break;
        }
        case 0x1cbc2U: { // 1b7c00000058 move.b #$0, $58(a5)
            next = 0x1cbc8U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x58U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1cbcaU: { // 082d00050041 btst.b #$5, $41(a5)
            next = 0x1cbd0U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1cbd0U: { // 6646 bne.b $1cc18
            next = 0x1cbd2U;
            if ((r.status & 4U) == 0U) { next = 0x1cc18U; transfer_kind = 1U; }
            break;
        }
        case 0x1cbd2U: { // 0c2d00100058 cmpi.b #$10, $58(a5)
            next = 0x1cbd8U;
            const auto source_value = 0x10U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x1cbd8U: { // 6c16 bge.b $1cbf0
            next = 0x1cbdaU;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1cbf0U; transfer_kind = 1U; }
            break;
        }
        case 0x1cbdaU: { // 422d003e clr.b $3e(a5)
            next = 0x1cbdeU;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
    default: return false;
    }
    return true;
}
