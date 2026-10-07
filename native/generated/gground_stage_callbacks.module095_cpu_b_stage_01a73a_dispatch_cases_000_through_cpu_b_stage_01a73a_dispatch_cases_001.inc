
static bool cpu_b_stage_01a73a_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1a73aU: { // 47f86c00 lea.l $6c00.w, a3
            next = 0x1a73eU;
            const auto source_address = 0x6c00U;
            r.address[3] = source_address;
            break;
        }
        case 0x1a73eU: { // 4a5b tst.w (a3)+
            next = 0x1a740U;
            const auto destination_address = r.address[3];
            const auto value = m.word(destination_address);
            r.address[3] += 2U;
            m.logic(value, 16U);
            break;
        }
        case 0x1a740U: { // 6b04 bmi.b $1a746
            next = 0x1a742U;
            if ((r.status & 8U) != 0U) { next = 0x1a746U; transfer_kind = 1U; }
            break;
        }
        case 0x1a742U: { // 47f87002 lea.l $7002.w, a3
            next = 0x1a746U;
            const auto source_address = 0x7002U;
            r.address[3] = source_address;
            break;
        }
        case 0x1a746U: { // 302d001a move.w $1a(a5), d0
            next = 0x1a74aU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a74aU: { // 0c400200 cmpi.w #$200, d0
            next = 0x1a74eU;
            const auto source_value = 0x200U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a74eU: { // 64000070 bcc.w $1a7c0
            next = 0x1a752U;
            if ((r.status & 1U) == 0U) { next = 0x1a7c0U; transfer_kind = 1U; }
            break;
        }
        case 0x1a752U: { // 3e00 move.w d0, d7
            next = 0x1a754U;
            const auto value = r.data[0];
            m.dw(7U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a754U: { // 06470020 addi.w #$20, d7
            next = 0x1a758U;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[7];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(7U, value);
            break;
        }
        case 0x1a758U: { // 04400020 subi.w #$20, d0
            next = 0x1a75cU;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a75cU: { // 6b20 bmi.b $1a77e
            next = 0x1a75eU;
            if ((r.status & 8U) != 0U) { next = 0x1a77eU; transfer_kind = 1U; }
            break;
        }
        case 0x1a75eU: { // 0c4701ff cmpi.w #$1ff, d7
            next = 0x1a762U;
            const auto source_value = 0x1ffU;
            const auto destination_value = r.data[7];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a762U: { // 6f04 ble.b $1a768
            next = 0x1a764U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1a768U; transfer_kind = 1U; }
            break;
        }
        case 0x1a764U: { // 3e3c01ff move.w #$1ff, d7
            next = 0x1a768U;
            const auto value = 0x1ffU;
            m.dw(7U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a768U: { // 9e40 sub.w d0, d7
            next = 0x1a76aU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[7];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(7U, value);
            break;
        }
        case 0x1a76aU: { // d040 add.w d0, d0
            next = 0x1a76cU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a76cU: { // d6c0 adda.w d0, a3
            next = 0x1a76eU;
            const auto source_value = r.data[0];
            r.address[3] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1a76eU: { // 322d002a move.w $2a(a5), d1
            next = 0x1a772U;
            const auto source_address = r.address[5] + 0x2aU;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a772U: { // 342d002c move.w $2c(a5), d2
            next = 0x1a776U;
            const auto source_address = r.address[5] + 0x2cU;
            const auto value = m.word(source_address);
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a776U: { // 362d0032 move.w $32(a5), d3
            next = 0x1a77aU;
            const auto source_address = r.address[5] + 0x32U;
            const auto value = m.word(source_address);
            m.dw(3U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a77aU: { // 382d0034 move.w $34(a5), d4
            next = 0x1a77eU;
            const auto source_address = r.address[5] + 0x34U;
            const auto value = m.word(source_address);
            m.dw(4U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a77eU: { // 301b move.w (a3)+, d0
            next = 0x1a780U;
            const auto source_address = r.address[3];
            const auto value = m.word(source_address);
            r.address[3] += 2U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a780U: { // 673a beq.b $1a7bc
            next = 0x1a782U;
            if ((r.status & 4U) != 0U) { next = 0x1a7bcU; transfer_kind = 1U; }
            break;
        }
        case 0x1a782U: { // 3c40 movea.w d0, a6
            next = 0x1a784U;
            const auto value = r.data[0];
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x1a784U: { // 7000 moveq #$0, d0
            next = 0x1a786U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1a786U: { // 0c2e000a000b cmpi.b #$a, $b(a6)
            next = 0x1a78cU;
            const auto source_value = 0xaU;
            const auto destination_address = r.address[6] + 0xbU;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x1a78cU: { // 662e bne.b $1a7bc
            next = 0x1a78eU;
            if ((r.status & 4U) == 0U) { next = 0x1a7bcU; transfer_kind = 1U; }
            break;
        }
        case 0x1a78eU: { // 4a2e003f tst.b $3f(a6)
            next = 0x1a792U;
            const auto destination_address = r.address[6] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1a792U: { // 6628 bne.b $1a7bc
            next = 0x1a794U;
            if ((r.status & 4U) == 0U) { next = 0x1a7bcU; transfer_kind = 1U; }
            break;
        }
        case 0x1a794U: { // b46e002a cmp.w $2a(a6), d2
            next = 0x1a798U;
            const auto source_address = r.address[6] + 0x2aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[2];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a798U: { // 6b22 bmi.b $1a7bc
            next = 0x1a79aU;
            if ((r.status & 8U) != 0U) { next = 0x1a7bcU; transfer_kind = 1U; }
            break;
        }
        case 0x1a79aU: { // b26e002c cmp.w $2c(a6), d1
            next = 0x1a79eU;
            const auto source_address = r.address[6] + 0x2cU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a79eU: { // 6e1c bgt.b $1a7bc
            next = 0x1a7a0U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1a7bcU; transfer_kind = 1U; }
            break;
        }
        case 0x1a7a0U: { // b86e0032 cmp.w $32(a6), d4
            next = 0x1a7a4U;
            const auto source_address = r.address[6] + 0x32U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[4];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a7a4U: { // 6b16 bmi.b $1a7bc
            next = 0x1a7a6U;
            if ((r.status & 8U) != 0U) { next = 0x1a7bcU; transfer_kind = 1U; }
            break;
        }
        case 0x1a7a6U: { // b66e0034 cmp.w $34(a6), d3
            next = 0x1a7aaU;
            const auto source_address = r.address[6] + 0x34U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[3];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a7aaU: { // 6e10 bgt.b $1a7bc
            next = 0x1a7acU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1a7bcU; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}

static bool cpu_b_stage_01a73a_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1a7acU: { // 556e0046 subq.w #$2, $46(a6)
            next = 0x1a7b0U;
            const auto source_value = 0x2U;
            const auto destination_address = r.address[6] + 0x46U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1a7b0U: { // 6e04 bgt.b $1a7b6
            next = 0x1a7b2U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1a7b6U; transfer_kind = 1U; }
            break;
        }
        case 0x1a7b2U: { // 4aee003f tas.b $3f(a6)
            next = 0x1a7b6U;
            const auto destination_address = r.address[6] + 0x3fU;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0x1a7b6U: { // 44fc0001 move.w #$1, ccr
            next = 0x1a7baU;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x1U & 0x1fU));
            break;
        }
        case 0x1a7bcU: { // 51cfffc0 dbra d7, $1a77e
            next = 0x1a7c0U;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x1a77eU;
            break;
        }
        case 0x1a7c0U: { // 44fc0000 move.w #$0, ccr
            next = 0x1a7c4U;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x0U & 0x1fU));
            break;
        }
    default: return false;
    }
    return true;
}
