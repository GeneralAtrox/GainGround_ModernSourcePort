
static bool cpu_b_stage_01cd32_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1cd32U:
    case 0x1cd36U:
    case 0x1cd38U:
    case 0x1cd3cU:
    case 0x1cd40U:
    case 0x1cd42U:
    case 0x1cd46U:
    case 0x1cd4aU:
    case 0x1cd50U:
    case 0x1cd52U:
    case 0x1cd56U:
    case 0x1cd5cU:
    case 0x1cd60U:
    case 0x1cd66U:
    case 0x1cd68U:
    case 0x1cd6eU:
    case 0x1cd70U:
    case 0x1cd74U:
    case 0x1cd78U:
    case 0x1cd7aU:
    case 0x1cd7eU:
    case 0x1cd80U:
    case 0x1cd86U:
    case 0x1cd88U:
    case 0x1cd8eU:
    case 0x1cd90U:
    case 0x1cd96U:
    case 0x1cd9cU:
    case 0x1cd9eU:
    case 0x1cda0U:
    case 0x1cda6U:
    case 0x1cdacU:
    case 0x1cdb2U:
    case 0x1cdb8U:
    case 0x1cdbeU:
    case 0x1cdc0U:
    case 0x1cdc6U:
    case 0x1cdcaU:
    case 0x1cdceU:
    case 0x1cdd0U:
    case 0x1cdd6U:
    case 0x1cdd8U:
    case 0x1cddcU:
    case 0x1cddeU:
    case 0x1cde0U:
    case 0x1cde4U:
    case 0x1cde8U:
    case 0x1cdeaU:
    case 0x1cdf0U:
    case 0x1cdf6U:
    case 0x1cdfcU:
    case 0x1ce02U:
    case 0x1ce08U:
        return true;
    default: return false;
    }
}

static bool cpu_b_stage_01cd32_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1cd32U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1cd36U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1cd36U: { // 6704 beq.b $1cd3c
            next = 0x1cd38U;
            if ((r.status & 4U) != 0U) { next = 0x1cd3cU; transfer_kind = 1U; }
            break;
        }
        case 0x1cd38U: { // 60000ca0 bra.w $1d9da
            next = 0x1cd3cU;
            if (true) { next = 0x1d9daU; transfer_kind = 1U; }
            break;
        }
        case 0x1cd3cU: { // 4a2d003e tst.b $3e(a5)
            next = 0x1cd40U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1cd40U: { // 6704 beq.b $1cd46
            next = 0x1cd42U;
            if ((r.status & 4U) != 0U) { next = 0x1cd46U; transfer_kind = 1U; }
            break;
        }
        case 0x1cd42U: { // 60000ca6 bra.w $1d9ea
            next = 0x1cd46U;
            if (true) { next = 0x1d9eaU; transfer_kind = 1U; }
            break;
        }
        case 0x1cd46U: { // 526d0054 addq.w #$1, $54(a5)
            next = 0x1cd4aU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x54U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1cd4aU: { // 082d00070040 btst.b #$7, $40(a5)
            next = 0x1cd50U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1cd50U: { // 6654 bne.b $1cda6
            next = 0x1cd52U;
            if ((r.status & 4U) == 0U) { next = 0x1cda6U; transfer_kind = 1U; }
            break;
        }
        case 0x1cd56U: { // 082d00050041 btst.b #$5, $41(a5)
            next = 0x1cd5cU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1cd5cU: { // 6600008c bne.w $1cdea
            next = 0x1cd60U;
            if ((r.status & 4U) == 0U) { next = 0x1cdeaU; transfer_kind = 1U; }
            break;
        }
        case 0x1cd60U: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1cd66U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1cd66U: { // 6708 beq.b $1cd70
            next = 0x1cd68U;
            if ((r.status & 4U) != 0U) { next = 0x1cd70U; transfer_kind = 1U; }
            break;
        }
        case 0x1cd68U: { // 08ed00010040 bset.b #$1, $40(a5)
            next = 0x1cd6eU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1cd70U: { // 302d0054 move.w $54(a5), d0
            next = 0x1cd74U;
            const auto source_address = r.address[5] + 0x54U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1cd74U: { // 0240001f andi.w #$1f, d0
            next = 0x1cd78U;
            const auto source_value = 0x1fU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1cd78U: { // 6606 bne.b $1cd80
            next = 0x1cd7aU;
            if ((r.status & 4U) == 0U) { next = 0x1cd80U; transfer_kind = 1U; }
            break;
        }
        case 0x1cd7eU: { // 602c bra.b $1cdac
            next = 0x1cd80U;
            if (true) { next = 0x1cdacU; transfer_kind = 1U; }
            break;
        }
        case 0x1cd80U: { // 082d00050040 btst.b #$5, $40(a5)
            next = 0x1cd86U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1cd86U: { // 6618 bne.b $1cda0
            next = 0x1cd88U;
            if ((r.status & 4U) == 0U) { next = 0x1cda0U; transfer_kind = 1U; }
            break;
        }
        case 0x1cd88U: { // 082d00060040 btst.b #$6, $40(a5)
            next = 0x1cd8eU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x6U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1cd8eU: { // 6610 bne.b $1cda0
            next = 0x1cd90U;
            if ((r.status & 4U) == 0U) { next = 0x1cda0U; transfer_kind = 1U; }
            break;
        }
        case 0x1cd90U: { // 08ad00070040 bclr.b #$7, $40(a5)
            next = 0x1cd96U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1cd96U: { // 082d00040041 btst.b #$4, $41(a5)
            next = 0x1cd9cU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x4U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1cd9cU: { // 6622 bne.b $1cdc0
            next = 0x1cd9eU;
            if ((r.status & 4U) == 0U) { next = 0x1cdc0U; transfer_kind = 1U; }
            break;
        }
        case 0x1cda0U: { // 08ad00010040 bclr.b #$1, $40(a5)
            next = 0x1cda6U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1cda6U: { // 08ad00040041 bclr.b #$4, $41(a5)
            next = 0x1cdacU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x4U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1cdacU: { // 08ad00000040 bclr.b #$0, $40(a5)
            next = 0x1cdb2U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1cdb2U: { // 08ed00030040 bset.b #$3, $40(a5)
            next = 0x1cdb8U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1cdb8U: { // 08ed00020040 bset.b #$2, $40(a5)
            next = 0x1cdbeU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1cdc0U: { // 08ad00010040 bclr.b #$1, $40(a5)
            next = 0x1cdc6U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1cdc6U: { // 302d005c move.w $5c(a5), d0
            next = 0x1cdcaU;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1cdcaU: { // b06d003a cmp.w $3a(a5), d0
            next = 0x1cdceU;
            const auto source_address = r.address[5] + 0x3aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1cdceU: { // 6608 bne.b $1cdd8
            next = 0x1cdd0U;
            if ((r.status & 4U) == 0U) { next = 0x1cdd8U; transfer_kind = 1U; }
            break;
        }
        case 0x1cdd0U: { // 08ad00040041 bclr.b #$4, $41(a5)
            next = 0x1cdd6U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x4U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
    default: return false;
    }
    return true;
}
