
FunctionResult cpu_b_stage_020e12(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20e12U: { // 6100bf1e bsr.w $1cd32
            next = 0x20e16U;
            const auto result = m.call(c, 666U, 0x20e12U, 0x1cd32U, 0x20e16U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e16U: { // 6100d2d6 bsr.w $1e0ee
            next = 0x20e1aU;
            const auto result = m.call(c, 356U, 0x20e16U, 0x1e0eeU, 0x20e1aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e1aU: { // 6100e3e0 bsr.w $1f1fc
            next = 0x20e1eU;
            const auto result = m.call(c, 371U, 0x20e1aU, 0x1f1fcU, 0x20e1eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e1eU: { // 6100cda4 bsr.w $1dbc4
            next = 0x20e22U;
            const auto result = m.call(c, 352U, 0x20e1eU, 0x1dbc4U, 0x20e22U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e22U: { // 6100cdcc bsr.w $1dbf0
            next = 0x20e26U;
            const auto result = m.call(c, 353U, 0x20e22U, 0x1dbf0U, 0x20e26U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e26U: { // 6100d008 bsr.w $1de30
            next = 0x20e2aU;
            const auto result = m.call(c, 355U, 0x20e26U, 0x1de30U, 0x20e2aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e2aU: { // 6100d2f8 bsr.w $1e124
            next = 0x20e2eU;
            const auto result = m.call(c, 357U, 0x20e2aU, 0x1e124U, 0x20e2eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e2eU: { // 6100d4d4 bsr.w $1e304
            next = 0x20e32U;
            const auto result = m.call(c, 360U, 0x20e2eU, 0x1e304U, 0x20e32U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20e32U: { // 4e75 rts 
            next = 0x20e34U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20e32U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20e12U:
        case 0x20e16U:
        case 0x20e1aU:
        case 0x20e1eU:
        case 0x20e22U:
        case 0x20e26U:
        case 0x20e2aU:
        case 0x20e2eU:
        case 0x20e32U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_020e90(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20e90U: { // 4a2d003f tst.b $3f(a5)
            next = 0x20e94U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x20e94U: { // 6708 beq.b $20e9e
            next = 0x20e96U;
            if ((r.status & 4U) != 0U) { next = 0x20e9eU; transfer_kind = 1U; }
            break;
        }
        case 0x20e96U: { // 4df83400 lea.l $3400.w, a6
            next = 0x20e9aU;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x20e9aU: { // 522e0026 addq.b #$1, $26(a6)
            next = 0x20e9eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x26U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x20e9eU: { // 6100b5d4 bsr.w $1c474
            next = 0x20ea2U;
            const auto result = m.call(c, 654U, 0x20e9eU, 0x1c474U, 0x20ea2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20ea2U: { // 6100cd20 bsr.w $1dbc4
            next = 0x20ea6U;
            const auto result = m.call(c, 352U, 0x20ea2U, 0x1dbc4U, 0x20ea6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20ea6U: { // 6100e134 bsr.w $1efdc
            next = 0x20eaaU;
            const auto result = m.call(c, 368U, 0x20ea6U, 0x1efdcU, 0x20eaaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20eaaU: { // 6100cd44 bsr.w $1dbf0
            next = 0x20eaeU;
            const auto result = m.call(c, 353U, 0x20eaaU, 0x1dbf0U, 0x20eaeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20eaeU: { // 6100d274 bsr.w $1e124
            next = 0x20eb2U;
            const auto result = m.call(c, 357U, 0x20eaeU, 0x1e124U, 0x20eb2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20eb2U: { // 6100d450 bsr.w $1e304
            next = 0x20eb6U;
            const auto result = m.call(c, 360U, 0x20eb2U, 0x1e304U, 0x20eb6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20eb6U: { // 4e75 rts 
            next = 0x20eb8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20eb6U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20e90U:
        case 0x20e94U:
        case 0x20e96U:
        case 0x20e9aU:
        case 0x20e9eU:
        case 0x20ea2U:
        case 0x20ea6U:
        case 0x20eaaU:
        case 0x20eaeU:
        case 0x20eb2U:
        case 0x20eb6U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
