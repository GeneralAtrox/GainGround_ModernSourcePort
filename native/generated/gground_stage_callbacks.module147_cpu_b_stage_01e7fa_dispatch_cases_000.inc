
static bool cpu_b_stage_01e7fa_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1e7feU: { // 3007 move.w d7, d0
            next = 0x1e800U;
            const auto value = r.data[7];
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e800U: { // e940 asl.w #$4, d0
            next = 0x1e802U;
            m.asl_word(0U, 4U);
            break;
        }
        case 0x1e802U: { // 3200 move.w d0, d1
            next = 0x1e804U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e804U: { // d240 add.w d0, d1
            next = 0x1e806U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e806U: { // d240 add.w d0, d1
            next = 0x1e808U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e808U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x1e80eU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e80eU: { // 6742 beq.b $1e852
            next = 0x1e810U;
            if ((r.status & 4U) != 0U) { next = 0x1e852U; transfer_kind = 1U; }
            break;
        }
        case 0x1e810U: { // d247 add.w d7, d1
            next = 0x1e812U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e812U: { // d247 add.w d7, d1
            next = 0x1e814U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e814U: { // 7000 moveq #$0, d0
            next = 0x1e816U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1e816U: { // 102d0036 move.b $36(a5), d0
            next = 0x1e81aU;
            const auto source_address = r.address[5] + 0x36U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1e81aU: { // 3400 move.w d0, d2
            next = 0x1e81cU;
            const auto value = r.data[0];
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e81cU: { // e540 asl.w #$2, d0
            next = 0x1e81eU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x1e81eU: { // e342 asl.w #$1, d2
            next = 0x1e820U;
            m.asl_word(2U, 1U);
            break;
        }
        case 0x1e820U: { // d042 add.w d2, d0
            next = 0x1e822U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e822U: { // 342d0056 move.w $56(a5), d2
            next = 0x1e826U;
            const auto source_address = r.address[5] + 0x56U;
            const auto value = m.word(source_address);
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e826U: { // d440 add.w d0, d2
            next = 0x1e828U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e828U: { // 3b420056 move.w d2, $56(a5)
            next = 0x1e82cU;
            const auto value = r.data[2];
            const auto destination_address = r.address[5] + 0x56U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e82cU: { // 02420600 andi.w #$600, d2
            next = 0x1e830U;
            const auto source_value = 0x600U;
            const auto destination_value = r.data[2];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e830U: { // ef5a rol.w #$7, d2
            next = 0x1e832U;
            const auto old = r.data[2] & 0xffffU;
            const auto value = ((old << 7U) | (old >> 9U)) & 0xffffU;
            m.dw(2U, value);
            m.logic(value, 16U);
            r.status = static_cast<std::uint16_t>(r.status | (value & 1U));
            break;
        }
        case 0x1e832U: { // 3007 move.w d7, d0
            next = 0x1e834U;
            const auto value = r.data[7];
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e834U: { // 9240 sub.w d0, d1
            next = 0x1e836U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e836U: { // d240 add.w d0, d1
            next = 0x1e838U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e838U: { // 51cafffc dbra d2, $1e836
            next = 0x1e83cU;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0x1e836U;
            break;
        }
        case 0x1e83cU: { // 302d005c move.w $5c(a5), d0
            next = 0x1e840U;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e840U: { // 02400400 andi.w #$400, d0
            next = 0x1e844U;
            const auto source_value = 0x400U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e844U: { // 6700f9ac beq.w $1e1f2
            next = 0x1e848U;
            if ((r.status & 4U) != 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e848U: { // 3007 move.w d7, d0
            next = 0x1e84aU;
            const auto value = r.data[7];
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e84aU: { // e548 lsl.w #$2, d0
            next = 0x1e84cU;
            m.shift_word(0U, 2U, true, false);
            break;
        }
        case 0x1e84cU: { // d240 add.w d0, d1
            next = 0x1e84eU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e84eU: { // 6000f9a2 bra.w $1e1f2
            next = 0x1e852U;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e852U: { // 082d00030078 btst.b #$3, $78(a5)
            next = 0x1e858U;
            const auto destination_address = r.address[5] + 0x78U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e858U: { // 665e bne.b $1e8b8
            next = 0x1e85aU;
            if ((r.status & 4U) == 0U) { next = 0x1e8b8U; transfer_kind = 1U; }
            break;
        }
        case 0x1e85aU: { // 082d00020078 btst.b #$2, $78(a5)
            next = 0x1e860U;
            const auto destination_address = r.address[5] + 0x78U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e860U: { // 6756 beq.b $1e8b8
            next = 0x1e862U;
            if ((r.status & 4U) != 0U) { next = 0x1e8b8U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}
