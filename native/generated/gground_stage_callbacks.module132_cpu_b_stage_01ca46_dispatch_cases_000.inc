
static bool cpu_b_stage_01ca46_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1ca46U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1ca4aU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1ca4aU: { // 6704 beq.b $1ca50
            next = 0x1ca4cU;
            if ((r.status & 4U) != 0U) { next = 0x1ca50U; transfer_kind = 1U; }
            break;
        }
        case 0x1ca4cU: { // 60000f8c bra.w $1d9da
            next = 0x1ca50U;
            if (true) { next = 0x1d9daU; transfer_kind = 1U; }
            break;
        }
        case 0x1ca50U: { // 4a2d003e tst.b $3e(a5)
            next = 0x1ca54U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1ca54U: { // 6704 beq.b $1ca5a
            next = 0x1ca56U;
            if ((r.status & 4U) != 0U) { next = 0x1ca5aU; transfer_kind = 1U; }
            break;
        }
        case 0x1ca56U: { // 60000f92 bra.w $1d9ea
            next = 0x1ca5aU;
            if (true) { next = 0x1d9eaU; transfer_kind = 1U; }
            break;
        }
        case 0x1ca5aU: { // 4a2d0059 tst.b $59(a5)
            next = 0x1ca5eU;
            const auto destination_address = r.address[5] + 0x59U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1ca5eU: { // 6716 beq.b $1ca76
            next = 0x1ca60U;
            if ((r.status & 4U) != 0U) { next = 0x1ca76U; transfer_kind = 1U; }
            break;
        }
        case 0x1ca60U: { // 532d0059 subq.b #$1, $59(a5)
            next = 0x1ca64U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x59U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x1ca64U: { // 6700008a beq.w $1caf0
            next = 0x1ca68U;
            if ((r.status & 4U) != 0U) { next = 0x1caf0U; transfer_kind = 1U; }
            break;
        }
        case 0x1ca68U: { // 08ed00010040 bset.b #$1, $40(a5)
            next = 0x1ca6eU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1ca6eU: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1ca74U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ca76U: { // 206d006e movea.l $6e(a5), a0
            next = 0x1ca7aU;
            const auto source_address = r.address[5] + 0x6eU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1ca7aU: { // 30280008 move.w $8(a0), d0
            next = 0x1ca7eU;
            const auto source_address = r.address[0] + 0x8U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ca7eU: { // d040 add.w d0, d0
            next = 0x1ca80U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1ca80U: { // d040 add.w d0, d0
            next = 0x1ca82U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1ca82U: { // 41f900032dec lea.l $32dec.l, a0
            next = 0x1ca88U;
            const auto source_address = 0x32decU;
            r.address[0] = source_address;
            break;
        }
        case 0x1ca88U: { // d0c0 adda.w d0, a0
            next = 0x1ca8aU;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1ca8aU: { // 7000 moveq #$0, d0
            next = 0x1ca8cU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1ca8cU: { // 1018 move.b (a0)+, d0
            next = 0x1ca8eU;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1ca8eU: { // e748 lsl.w #$3, d0
            next = 0x1ca90U;
            m.shift_word(0U, 3U, true, false);
            break;
        }
        case 0x1ca90U: { // 906d0012 sub.w $12(a5), d0
            next = 0x1ca94U;
            const auto source_address = r.address[5] + 0x12U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1ca94U: { // 5840 addq.w #$4, d0
            next = 0x1ca96U;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1ca96U: { // 0240fff8 andi.w #$fff8, d0
            next = 0x1ca9aU;
            const auto source_value = 0xfff8U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1ca9aU: { // 6700008a beq.w $1cb26
            next = 0x1ca9eU;
            if ((r.status & 4U) != 0U) { next = 0x1cb26U; transfer_kind = 1U; }
            break;
        }
        case 0x1ca9eU: { // 6d08 blt.b $1caa8
            next = 0x1caa0U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1caa8U; transfer_kind = 1U; }
            break;
        }
        case 0x1caa0U: { // 3b7c0000005c move.w #$0, $5c(a5)
            next = 0x1caa6U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x5cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1caa8U: { // 7000 moveq #$0, d0
            next = 0x1caaaU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1caaaU: { // 1018 move.b (a0)+, d0
            next = 0x1caacU;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1caacU: { // e748 lsl.w #$3, d0
            next = 0x1caaeU;
            m.shift_word(0U, 3U, true, false);
            break;
        }
        case 0x1caaeU: { // 906d0012 sub.w $12(a5), d0
            next = 0x1cab2U;
            const auto source_address = r.address[5] + 0x12U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1cab2U: { // 5840 addq.w #$4, d0
            next = 0x1cab4U;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1cab4U: { // 0240fff8 andi.w #$fff8, d0
            next = 0x1cab8U;
            const auto source_value = 0xfff8U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1cab8U: { // 676c beq.b $1cb26
            next = 0x1cabaU;
            if ((r.status & 4U) != 0U) { next = 0x1cb26U; transfer_kind = 1U; }
            break;
        }
        case 0x1cabaU: { // 6e08 bgt.b $1cac4
            next = 0x1cabcU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1cac4U; transfer_kind = 1U; }
            break;
        }
        case 0x1cabcU: { // 3b7c0400005c move.w #$400, $5c(a5)
            next = 0x1cac2U;
            const auto value = 0x400U;
            const auto destination_address = r.address[5] + 0x5cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
