
FunctionResult cpu_b_stage_018c02(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18c02U: { // 61000084 bsr.w $18c88
            next = 0x18c06U;
            const auto result = m.call(c, 895U, 0x18c02U, 0x18c88U, 0x18c06U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18c06U: { // 610002bc bsr.w $18ec4
            next = 0x18c0aU;
            const auto result = m.call(c, 896U, 0x18c06U, 0x18ec4U, 0x18c0aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18c0aU: { // 61000366 bsr.w $18f72
            next = 0x18c0eU;
            const auto result = m.call(c, 897U, 0x18c0aU, 0x18f72U, 0x18c0eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18c0eU: { // 6100041e bsr.w $1902e
            next = 0x18c12U;
            const auto result = m.call(c, 898U, 0x18c0eU, 0x1902eU, 0x18c12U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18c12U: { // 49fa244a lea.l $1b05e(pc), a4
            next = 0x18c16U;
            const auto source_address = 0x1b05eU;
            r.address[4] = source_address;
            break;
        }
        case 0x18c16U: { // 61000590 bsr.w $191a8
            next = 0x18c1aU;
            const auto result = m.call(c, 901U, 0x18c16U, 0x191a8U, 0x18c1aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18c1aU: { // 41fa244e lea.l $1b06a(pc), a0
            next = 0x18c1eU;
            const auto source_address = 0x1b06aU;
            r.address[0] = source_address;
            break;
        }
        case 0x18c1eU: { // 43fa243e lea.l $1b05e(pc), a1
            next = 0x18c22U;
            const auto source_address = 0x1b05eU;
            r.address[1] = source_address;
            break;
        }
        case 0x18c22U: { // 610006b8 bsr.w $192dc
            next = 0x18c26U;
            const auto result = m.call(c, 903U, 0x18c22U, 0x192dcU, 0x18c26U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18c26U: { // 604e bra.b $18c76
            next = 0x18c28U;
            if (true) { next = 0x18c76U; transfer_kind = 1U; }
            break;
        }
        case 0x18c76U: { // 610004ee bsr.w $19166
            next = 0x18c7aU;
            const auto result = m.call(c, 900U, 0x18c76U, 0x19166U, 0x18c7aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18c7aU: { // 6100041c bsr.w $19098
            next = 0x18c7eU;
            const auto result = m.call(c, 899U, 0x18c7aU, 0x19098U, 0x18c7eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18c7eU: { // 41fa23de lea.l $1b05e(pc), a0
            next = 0x18c82U;
            const auto source_address = 0x1b05eU;
            r.address[0] = source_address;
            break;
        }
        case 0x18c82U: { // 61000716 bsr.w $1939a
            next = 0x18c86U;
            const auto result = m.call(c, 904U, 0x18c82U, 0x1939aU, 0x18c86U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18c86U: { // 4e75 rts 
            next = 0x18c88U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18c86U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18c02U:
        case 0x18c06U:
        case 0x18c0aU:
        case 0x18c0eU:
        case 0x18c12U:
        case 0x18c16U:
        case 0x18c1aU:
        case 0x18c1eU:
        case 0x18c22U:
        case 0x18c26U:
        case 0x18c76U:
        case 0x18c7aU:
        case 0x18c7eU:
        case 0x18c82U:
        case 0x18c86U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_0193e4_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x193e4U: { // 3c6d0036 movea.w $36(a5), a6
            next = 0x193e8U;
            const auto source_address = r.address[5] + 0x36U;
            const auto value = m.word(source_address);
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x193e8U: { // 4a16 tst.b (a6)
            next = 0x193eaU;
            const auto destination_address = r.address[6];
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x193eaU: { // 6b04 bmi.b $193f0
            next = 0x193ecU;
            if ((r.status & 8U) != 0U) { next = 0x193f0U; transfer_kind = 1U; }
            break;
        }
        case 0x193ecU: { // 4255 clr.w (a5)
            next = 0x193eeU;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x193f0U: { // 286e005c movea.l $5c(a6), a4
            next = 0x193f4U;
            const auto source_address = r.address[6] + 0x5cU;
            const auto value = m.lng(source_address);
            r.address[4] = value;
            break;
        }
        case 0x193f4U: { // 5c4c addq.w #$6, a4
            next = 0x193f6U;
            const auto source_value = 0x6U;
            r.address[4] += source_value;
            break;
        }
        case 0x193f6U: { // d8ed003a adda.w $3a(a5), a4
            next = 0x193faU;
            const auto source_address = r.address[5] + 0x3aU;
            const auto source_value = m.word(source_address);
            r.address[4] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x193faU: { // 3b5c0006 move.w (a4)+, $6(a5)
            next = 0x193feU;
            const auto source_address = r.address[4];
            const auto value = m.word(source_address);
            r.address[4] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x193feU: { // 6b3a bmi.b $1943a
            next = 0x19400U;
            if ((r.status & 8U) != 0U) { next = 0x1943aU; transfer_kind = 1U; }
            break;
        }
        case 0x19400U: { // 101c move.b (a4)+, d0
            next = 0x19402U;
            const auto source_address = r.address[4];
            const auto value = m.byte(source_address);
            r.address[4] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19402U: { // 4880 ext.w d0
            next = 0x19404U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[0])));
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19404U: { // d06e000c add.w $c(a6), d0
            next = 0x19408U;
            const auto source_address = r.address[6] + 0xcU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19408U: { // 3b40000c move.w d0, $c(a5)
            next = 0x1940cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0xcU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1940cU: { // 101c move.b (a4)+, d0
            next = 0x1940eU;
            const auto source_address = r.address[4];
            const auto value = m.byte(source_address);
            r.address[4] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1940eU: { // 4880 ext.w d0
            next = 0x19410U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[0])));
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19410U: { // d06e000e add.w $e(a6), d0
            next = 0x19414U;
            const auto source_address = r.address[6] + 0xeU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19414U: { // 3b40000e move.w d0, $e(a5)
            next = 0x19418U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0xeU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19418U: { // 3b6e00080008 move.w $8(a6), $8(a5)
            next = 0x1941eU;
            const auto source_address = r.address[6] + 0x8U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1941eU: { // 3b6e00100010 move.w $10(a6), $10(a5)
            next = 0x19424U;
            const auto source_address = r.address[6] + 0x10U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x10U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19424U: { // 301c move.w (a4)+, d0
            next = 0x19426U;
            const auto source_address = r.address[4];
            const auto value = m.word(source_address);
            r.address[4] += 2U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19426U: { // 803c0004 or.b #$4, d0
            next = 0x1942aU;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x1942aU: { // 1b400001 move.b d0, $1(a5)
            next = 0x1942eU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1942eU: { // 3b6e001a001a move.w $1a(a6), $1a(a5)
            next = 0x19434U;
            const auto source_address = r.address[6] + 0x1aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1943aU: { // 08950000 bclr.b #$0, (a5)
            next = 0x1943eU;
            const auto destination_address = r.address[5];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
    default: return false;
    }
    return true;
}
