
static bool cpu_b_stage_01c85e_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1c864U: { // e258 ror.w #$1, d0
            next = 0x1c866U;
            m.rotate_right(0U, 1U, 16U);
            break;
        }
        case 0x1c866U: { // 206d0066 movea.l $66(a5), a0
            next = 0x1c86aU;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1c86aU: { // 2050 movea.l (a0), a0
            next = 0x1c86cU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1c86cU: { // 7200 moveq #$0, d1
            next = 0x1c86eU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1c86eU: { // 1218 move.b (a0)+, d1
            next = 0x1c870U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1c870U: { // c041 and.w d1, d0
            next = 0x1c872U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1c872U: { // d040 add.w d0, d0
            next = 0x1c874U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1c874U: { // 3200 move.w d0, d1
            next = 0x1c876U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c876U: { // d040 add.w d0, d0
            next = 0x1c878U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1c878U: { // d240 add.w d0, d1
            next = 0x1c87aU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1c87aU: { // 206d0066 movea.l $66(a5), a0
            next = 0x1c87eU;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1c87eU: { // 2068001a movea.l $1a(a0), a0
            next = 0x1c882U;
            const auto source_address = r.address[0] + 0x1aU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1c882U: { // d0c1 adda.w d1, a0
            next = 0x1c884U;
            const auto source_value = r.data[1];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1c884U: { // 3b580012 move.w (a0)+, $12(a5)
            next = 0x1c888U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c888U: { // 3b580016 move.w (a0)+, $16(a5)
            next = 0x1c88cU;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c88cU: { // 3b58001a move.w (a0)+, $1a(a5)
            next = 0x1c890U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c890U: { // 43f83400 lea.l $3400.w, a1
            next = 0x1c894U;
            const auto source_address = 0x3400U;
            r.address[1] = source_address;
            break;
        }
        case 0x1c894U: { // 45f83400 lea.l $3400.w, a2
            next = 0x1c898U;
            const auto source_address = 0x3400U;
            r.address[2] = source_address;
            break;
        }
        case 0x1c898U: { // d2fc000a adda.w #$a, a1
            next = 0x1c89cU;
            const auto source_value = 0xaU;
            r.address[1] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1c89cU: { // d4fc0010 adda.w #$10, a2
            next = 0x1c8a0U;
            const auto source_value = 0x10U;
            r.address[2] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1c8a0U: { // 343c0002 move.w #$2, d2
            next = 0x1c8a4U;
            const auto value = 0x2U;
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c8a4U: { // 4a19 tst.b (a1)+
            next = 0x1c8a6U;
            const auto destination_address = r.address[1];
            const auto value = m.byte(destination_address);
            r.address[1] += 1U;
            m.logic(value, 8U);
            break;
        }
        case 0x1c8a6U: { // 6736 beq.b $1c8de
            next = 0x1c8a8U;
            if ((r.status & 4U) != 0U) { next = 0x1c8deU; transfer_kind = 1U; }
            break;
        }
        case 0x1c8a8U: { // 3012 move.w (a2), d0
            next = 0x1c8aaU;
            const auto source_address = r.address[2];
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c8aaU: { // 3200 move.w d0, d1
            next = 0x1c8acU;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c8acU: { // 907c0018 sub.w #$18, d0
            next = 0x1c8b0U;
            const auto source_value = 0x18U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1c8b0U: { // d27c0018 add.w #$18, d1
            next = 0x1c8b4U;
            const auto source_value = 0x18U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1c8b4U: { // b06d0012 cmp.w $12(a5), d0
            next = 0x1c8b8U;
            const auto source_address = r.address[5] + 0x12U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1c8b8U: { // 6e24 bgt.b $1c8de
            next = 0x1c8baU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1c8deU; transfer_kind = 1U; }
            break;
        }
        case 0x1c8baU: { // b26d0012 cmp.w $12(a5), d1
            next = 0x1c8beU;
            const auto source_address = r.address[5] + 0x12U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1c8beU: { // 6d1e blt.b $1c8de
            next = 0x1c8c0U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1c8deU; transfer_kind = 1U; }
            break;
        }
        case 0x1c8c0U: { // 302a0002 move.w $2(a2), d0
            next = 0x1c8c4U;
            const auto source_address = r.address[2] + 0x2U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c8c4U: { // 3200 move.w d0, d1
            next = 0x1c8c6U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c8c6U: { // 907c0018 sub.w #$18, d0
            next = 0x1c8caU;
            const auto source_value = 0x18U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
    default: return false;
    }
    return true;
}

static bool cpu_b_stage_01c85e_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1c8caU: { // d27c0018 add.w #$18, d1
            next = 0x1c8ceU;
            const auto source_value = 0x18U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1c8ceU: { // b06d001a cmp.w $1a(a5), d0
            next = 0x1c8d2U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1c8d2U: { // 6e0a bgt.b $1c8de
            next = 0x1c8d4U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1c8deU; transfer_kind = 1U; }
            break;
        }
        case 0x1c8d4U: { // b26d001a cmp.w $1a(a5), d1
            next = 0x1c8d8U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1c8d8U: { // 6d04 blt.b $1c8de
            next = 0x1c8daU;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1c8deU; transfer_kind = 1U; }
            break;
        }
        case 0x1c8daU: { // 6000ff82 bra.w $1c85e
            next = 0x1c8deU;
            if (true) { next = 0x1c85eU; transfer_kind = 1U; }
            break;
        }
        case 0x1c8deU: { // d4fc0004 adda.w #$4, a2
            next = 0x1c8e2U;
            const auto source_value = 0x4U;
            r.address[2] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1c8e2U: { // 51caffc0 dbra d2, $1c8a4
            next = 0x1c8e6U;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0x1c8a4U;
            break;
        }
    default: return false;
    }
    return true;
}
