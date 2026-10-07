
static bool cpu_b_stage_01bde0_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1be7aU: { // 08ad00070040 bclr.b #$7, $40(a5)
            next = 0x1be80U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1be82U: { // 08ad00010040 bclr.b #$1, $40(a5)
            next = 0x1be88U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1be88U: { // 08ad00040041 bclr.b #$4, $41(a5)
            next = 0x1be8eU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x4U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1be8eU: { // 08ad00000040 bclr.b #$0, $40(a5)
            next = 0x1be94U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1be94U: { // 08ed00030040 bset.b #$3, $40(a5)
            next = 0x1be9aU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1be9aU: { // 08ed00020040 bset.b #$2, $40(a5)
            next = 0x1bea0U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1bea2U: { // 1b7c00000060 move.b #$0, $60(a5)
            next = 0x1bea8U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x60U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1beacU: { // 6000ffd4 bra.w $1be82
            next = 0x1beb0U;
            if (true) { next = 0x1be82U; transfer_kind = 1U; }
            break;
        }
        case 0x1beb0U: { // 4a2d0058 tst.b $58(a5)
            next = 0x1beb4U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1beb4U: { // 670c beq.b $1bec2
            next = 0x1beb6U;
            if ((r.status & 4U) != 0U) { next = 0x1bec2U; transfer_kind = 1U; }
            break;
        }
        case 0x1beb6U: { // 532d0058 subq.b #$1, $58(a5)
            next = 0x1bebaU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x1bebaU: { // 08ed00010040 bset.b #$1, $40(a5)
            next = 0x1bec0U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1bec2U: { // 41f83400 lea.l $3400.w, a0
            next = 0x1bec6U;
            const auto source_address = 0x3400U;
            r.address[0] = source_address;
            break;
        }
        case 0x1bec6U: { // 30280034 move.w $34(a0), d0
            next = 0x1becaU;
            const auto source_address = r.address[0] + 0x34U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1becaU: { // 5240 addq.w #$1, d0
            next = 0x1beccU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1beccU: { // 0240001f andi.w #$1f, d0
            next = 0x1bed0U;
            const auto source_value = 0x1fU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1bed0U: { // 6702 beq.b $1bed4
            next = 0x1bed2U;
            if ((r.status & 4U) != 0U) { next = 0x1bed4U; transfer_kind = 1U; }
            break;
        }
        case 0x1bed4U: { // 52680034 addq.w #$1, $34(a0)
            next = 0x1bed8U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[0] + 0x34U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1bed8U: { // 246d0066 movea.l $66(a5), a2
            next = 0x1bedcU;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[2] = value;
            break;
        }
        case 0x1bedcU: { // 3a1a move.w (a2)+, d5
            next = 0x1bedeU;
            const auto source_address = r.address[2];
            const auto value = m.word(source_address);
            r.address[2] += 2U;
            m.dw(5U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1bee2U: { // 640a bcc.b $1beee
            next = 0x1bee4U;
            if ((r.status & 1U) == 0U) { next = 0x1beeeU; transfer_kind = 1U; }
            break;
        }
        case 0x1bee4U: { // d4fc0004 adda.w #$4, a2
            next = 0x1bee8U;
            const auto source_value = 0x4U;
            r.address[2] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1bee8U: { // 51cdfff4 dbra d5, $1bede
            next = 0x1beecU;
            m.dw(5U, r.data[5] - 1U);
            if ((r.data[5] & 0xffffU) != 0xffffU) next = 0x1bedeU;
            break;
        }
        case 0x1beeeU: { // 301a move.w (a2)+, d0
            next = 0x1bef0U;
            const auto source_address = r.address[2];
            const auto value = m.word(source_address);
            r.address[2] += 2U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1bef0U: { // 3b400062 move.w d0, $62(a5)
            next = 0x1bef4U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x62U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1bef4U: { // 3b400012 move.w d0, $12(a5)
            next = 0x1bef8U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1bef8U: { // 3b7c00000014 move.w #$0, $14(a5)
            next = 0x1befeU;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x14U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1befeU: { // 301a move.w (a2)+, d0
            next = 0x1bf00U;
            const auto source_address = r.address[2];
            const auto value = m.word(source_address);
            r.address[2] += 2U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1bf00U: { // 3b400064 move.w d0, $64(a5)
            next = 0x1bf04U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x64U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1bf04U: { // 3b40001a move.w d0, $1a(a5)
            next = 0x1bf08U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1bf08U: { // 3b7c0000001c move.w #$0, $1c(a5)
            next = 0x1bf0eU;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x1cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1bf0eU: { // 08ad00030041 bclr.b #$3, $41(a5)
            next = 0x1bf14U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
    default: return false;
    }
    return true;
}

static bool cpu_b_stage_01bde0_dispatch_cases_002([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1bf14U: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x1bf1aU;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
