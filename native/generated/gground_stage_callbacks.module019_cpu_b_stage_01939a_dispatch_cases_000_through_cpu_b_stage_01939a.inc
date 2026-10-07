
static bool cpu_b_stage_01939a_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1939aU: { // 302d0012 move.w $12(a5), d0
            next = 0x1939eU;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1939eU: { // 1218 move.b (a0)+, d1
            next = 0x193a0U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x193a0U: { // 4881 ext.w d1
            next = 0x193a2U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[1])));
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x193a2U: { // 9041 sub.w d1, d0
            next = 0x193a4U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x193a4U: { // 3b40002a move.w d0, $2a(a5)
            next = 0x193a8U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x2aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x193a8U: { // 1218 move.b (a0)+, d1
            next = 0x193aaU;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x193aaU: { // 4881 ext.w d1
            next = 0x193acU;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[1])));
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x193acU: { // d041 add.w d1, d0
            next = 0x193aeU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x193aeU: { // 3b40002c move.w d0, $2c(a5)
            next = 0x193b2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x2cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x193b2U: { // 302d0016 move.w $16(a5), d0
            next = 0x193b6U;
            const auto source_address = r.address[5] + 0x16U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x193b6U: { // 1218 move.b (a0)+, d1
            next = 0x193b8U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x193b8U: { // 4881 ext.w d1
            next = 0x193baU;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[1])));
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x193baU: { // 9041 sub.w d1, d0
            next = 0x193bcU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x193bcU: { // 3b40002e move.w d0, $2e(a5)
            next = 0x193c0U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x2eU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x193c0U: { // 1218 move.b (a0)+, d1
            next = 0x193c2U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x193c2U: { // 4881 ext.w d1
            next = 0x193c4U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[1])));
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x193c4U: { // d041 add.w d1, d0
            next = 0x193c6U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x193c6U: { // 3b400030 move.w d0, $30(a5)
            next = 0x193caU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x30U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x193caU: { // 302d001a move.w $1a(a5), d0
            next = 0x193ceU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x193ceU: { // 1218 move.b (a0)+, d1
            next = 0x193d0U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x193d0U: { // 4881 ext.w d1
            next = 0x193d2U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[1])));
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x193d2U: { // 9041 sub.w d1, d0
            next = 0x193d4U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x193d4U: { // 3b400032 move.w d0, $32(a5)
            next = 0x193d8U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x32U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x193d8U: { // 1218 move.b (a0)+, d1
            next = 0x193daU;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x193daU: { // 4881 ext.w d1
            next = 0x193dcU;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[1])));
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x193dcU: { // d041 add.w d1, d0
            next = 0x193deU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x193deU: { // 3b400034 move.w d0, $34(a5)
            next = 0x193e2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x34U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01939a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x193e2U: { // 4e75 rts 
            next = 0x193e4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x193e2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01939a_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1939aU:
        case 0x1939eU:
        case 0x193a0U:
        case 0x193a2U:
        case 0x193a4U:
        case 0x193a8U:
        case 0x193aaU:
        case 0x193acU:
        case 0x193aeU:
        case 0x193b2U:
        case 0x193b6U:
        case 0x193b8U:
        case 0x193baU:
        case 0x193bcU:
        case 0x193c0U:
        case 0x193c2U:
        case 0x193c4U:
        case 0x193c6U:
        case 0x193caU:
        case 0x193ceU:
        case 0x193d0U:
        case 0x193d2U:
        case 0x193d4U:
        case 0x193d8U:
        case 0x193daU:
        case 0x193dcU:
        case 0x193deU:
        case 0x193e2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
