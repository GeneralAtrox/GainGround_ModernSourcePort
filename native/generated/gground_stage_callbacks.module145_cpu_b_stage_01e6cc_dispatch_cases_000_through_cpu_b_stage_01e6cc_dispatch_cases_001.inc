
static bool cpu_b_stage_01e6cc_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1e6ccU: { // 082d00070041 btst.b #$7, $41(a5)
            next = 0x1e6d2U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e6d2U: { // 6702 beq.b $1e6d6
            next = 0x1e6d4U;
            if ((r.status & 4U) != 0U) { next = 0x1e6d6U; transfer_kind = 1U; }
            break;
        }
        case 0x1e6d6U: { // 7000 moveq #$0, d0
            next = 0x1e6d8U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1e6d8U: { // 102d0058 move.b $58(a5), d0
            next = 0x1e6dcU;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1e6dcU: { // 0240003f andi.w #$3f, d0
            next = 0x1e6e0U;
            const auto source_value = 0x3fU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e6e0U: { // e248 lsr.w #$1, d0
            next = 0x1e6e2U;
            m.shift_word(0U, 1U, false, false);
            break;
        }
        case 0x1e6e2U: { // 0c400010 cmpi.w #$10, d0
            next = 0x1e6e6U;
            const auto source_value = 0x10U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1e6e6U: { // 671c beq.b $1e704
            next = 0x1e6e8U;
            if ((r.status & 4U) != 0U) { next = 0x1e704U; transfer_kind = 1U; }
            break;
        }
        case 0x1e6e8U: { // 6d06 blt.b $1e6f0
            next = 0x1e6eaU;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1e6f0U; transfer_kind = 1U; }
            break;
        }
        case 0x1e6eaU: { // 4440 neg.w d0
            next = 0x1e6ecU;
            const auto old = r.data[0];
            const auto value = m.sub(0U, old, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e6ecU: { // 0640001f addi.w #$1f, d0
            next = 0x1e6f0U;
            const auto source_value = 0x1fU;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e6f0U: { // e248 lsr.w #$1, d0
            next = 0x1e6f2U;
            m.shift_word(0U, 1U, false, false);
            break;
        }
        case 0x1e6f2U: { // 343c0012 move.w #$12, d2
            next = 0x1e6f6U;
            const auto value = 0x12U;
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e6f6U: { // 7200 moveq #$0, d1
            next = 0x1e6f8U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1e6f8U: { // 9242 sub.w d2, d1
            next = 0x1e6faU;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e6faU: { // d242 add.w d2, d1
            next = 0x1e6fcU;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e6fcU: { // 51c8fffc dbra d0, $1e6fa
            next = 0x1e700U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1e6faU;
            break;
        }
        case 0x1e700U: { // 6000faf0 bra.w $1e1f2
            next = 0x1e704U;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e704U: { // 526d0056 addq.w #$1, $56(a5)
            next = 0x1e708U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x56U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1e708U: { // 323c0090 move.w #$90, d1
            next = 0x1e70cU;
            const auto value = 0x90U;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e70cU: { // 302d005c move.w $5c(a5), d0
            next = 0x1e710U;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e710U: { // 06400080 addi.w #$80, d0
            next = 0x1e714U;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e714U: { // 02400700 andi.w #$700, d0
            next = 0x1e718U;
            const auto source_value = 0x700U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e718U: { // e648 lsr.w #$3, d0
            next = 0x1e71aU;
            m.shift_word(0U, 3U, false, false);
            break;
        }
        case 0x1e71aU: { // d240 add.w d0, d1
            next = 0x1e71cU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e71cU: { // e248 lsr.w #$1, d0
            next = 0x1e71eU;
            m.shift_word(0U, 1U, false, false);
            break;
        }
        case 0x1e71eU: { // d240 add.w d0, d1
            next = 0x1e720U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e720U: { // e448 lsr.w #$2, d0
            next = 0x1e722U;
            m.shift_word(0U, 2U, false, false);
            break;
        }
        case 0x1e722U: { // d240 add.w d0, d1
            next = 0x1e724U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e724U: { // e248 lsr.w #$1, d0
            next = 0x1e726U;
            m.shift_word(0U, 1U, false, false);
            break;
        }
        case 0x1e726U: { // d240 add.w d0, d1
            next = 0x1e728U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e728U: { // 082d00000041 btst.b #$0, $41(a5)
            next = 0x1e72eU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e72eU: { // 6714 beq.b $1e744
            next = 0x1e730U;
            if ((r.status & 4U) != 0U) { next = 0x1e744U; transfer_kind = 1U; }
            break;
        }
        case 0x1e730U: { // 302d0056 move.w $56(a5), d0
            next = 0x1e734U;
            const auto source_address = r.address[5] + 0x56U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e734U: { // 02400004 andi.w #$4, d0
            next = 0x1e738U;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e738U: { // 6700fab8 beq.w $1e1f2
            next = 0x1e73cU;
            if ((r.status & 4U) != 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}

static bool cpu_b_stage_01e6cc_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1e73cU: { // 06410012 addi.w #$12, d1
            next = 0x1e740U;
            const auto source_value = 0x12U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e740U: { // 6000fab0 bra.w $1e1f2
            next = 0x1e744U;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e744U: { // 06410012 addi.w #$12, d1
            next = 0x1e748U;
            const auto source_value = 0x12U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e748U: { // 0c6d00040074 cmpi.w #$4, $74(a5)
            next = 0x1e74eU;
            const auto source_value = 0x4U;
            const auto destination_address = r.address[5] + 0x74U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1e74eU: { // 6e00faa2 bgt.w $1e1f2
            next = 0x1e752U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e752U: { // 06410012 addi.w #$12, d1
            next = 0x1e756U;
            const auto source_value = 0x12U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e756U: { // 6000fa9a bra.w $1e1f2
            next = 0x1e75aU;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}
