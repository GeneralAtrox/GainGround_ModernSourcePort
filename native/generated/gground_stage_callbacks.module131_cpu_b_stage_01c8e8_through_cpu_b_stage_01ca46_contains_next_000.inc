
FunctionResult cpu_b_stage_01c8e8(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1c918U: { // 4e75 rts 
            next = 0x1c91aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c918U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c938U: { // 610000e2 bsr.w $1ca1c
            next = 0x1c93cU;
            const auto result = m.call(c, 662U, 0x1c938U, 0x1ca1cU, 0x1c93cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1c958U: { // 4e75 rts 
            next = 0x1c95aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c958U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c988U: { // 4e75 rts 
            next = 0x1c98aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c988U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c9a8U: { // 4e75 rts 
            next = 0x1c9aaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c9a8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c9b2U: { // 4e75 rts 
            next = 0x1c9b4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c9b2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c9baU: { // 4e75 rts 
            next = 0x1c9bcU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c9baU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c9e2U: { // 4e75 rts 
            next = 0x1c9e4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c9e2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1ca1aU: { // 4e75 rts 
            next = 0x1ca1cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1ca1aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01c8e8_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01c8e8_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01c8e8_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

static bool cpu_b_stage_01ca46_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1ca46U:
    case 0x1ca4aU:
    case 0x1ca4cU:
    case 0x1ca50U:
    case 0x1ca54U:
    case 0x1ca56U:
    case 0x1ca5aU:
    case 0x1ca5eU:
    case 0x1ca60U:
    case 0x1ca64U:
    case 0x1ca68U:
    case 0x1ca6eU:
    case 0x1ca74U:
    case 0x1ca76U:
    case 0x1ca7aU:
    case 0x1ca7eU:
    case 0x1ca80U:
    case 0x1ca82U:
    case 0x1ca88U:
    case 0x1ca8aU:
    case 0x1ca8cU:
    case 0x1ca8eU:
    case 0x1ca90U:
    case 0x1ca94U:
    case 0x1ca96U:
    case 0x1ca9aU:
    case 0x1ca9eU:
    case 0x1caa0U:
    case 0x1caa6U:
    case 0x1caa8U:
    case 0x1caaaU:
    case 0x1caacU:
    case 0x1caaeU:
    case 0x1cab2U:
    case 0x1cab4U:
    case 0x1cab8U:
    case 0x1cabaU:
    case 0x1cabcU:
    case 0x1cac2U:
    case 0x1cac4U:
    case 0x1cacaU:
    case 0x1cad0U:
    case 0x1cad6U:
    case 0x1cadcU:
    case 0x1cae2U:
    case 0x1cae6U:
    case 0x1caeaU:
    case 0x1caeeU:
    case 0x1caf0U:
    case 0x1caf6U:
    case 0x1cafcU:
    case 0x1cb02U:
    case 0x1cb08U:
    case 0x1cb0eU:
    case 0x1cb14U:
    case 0x1cb1aU:
    case 0x1cb1cU:
    case 0x1cb1eU:
    case 0x1cb24U:
    case 0x1cb26U:
    case 0x1cb2aU:
    case 0x1cb30U:
        return true;
    default: return false;
    }
}
