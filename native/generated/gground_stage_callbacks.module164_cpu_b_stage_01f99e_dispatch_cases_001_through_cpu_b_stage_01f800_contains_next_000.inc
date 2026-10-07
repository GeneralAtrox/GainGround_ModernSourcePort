
static bool cpu_b_stage_01f99e_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1fa08U: { // 3d40001a move.w d0, $1a(a6)
            next = 0x1fa0cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1fa0cU: { // 426e001c clr.w $1c(a6)
            next = 0x1fa10U;
            const auto destination_address = r.address[6] + 0x1cU;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1fa10U: { // 322e005c move.w $5c(a6), d1
            next = 0x1fa14U;
            const auto source_address = r.address[6] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1fa1aU: { // 7800 moveq #$0, d4
            next = 0x1fa1cU;
            r.data[4] = 0x0U;
            m.logic(r.data[4], 32U);
            break;
        }
        case 0x1fa1cU: { // 206d0066 movea.l $66(a5), a0
            next = 0x1fa20U;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1fa20U: { // 7800 moveq #$0, d4
            next = 0x1fa22U;
            r.data[4] = 0x0U;
            m.logic(r.data[4], 32U);
            break;
        }
        case 0x1fa22U: { // 18280009 move.b $9(a0), d4
            next = 0x1fa26U;
            const auto source_address = r.address[0] + 0x9U;
            const auto value = m.byte(source_address);
            m.db(4U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1fa26U: { // 3e05 move.w d5, d7
            next = 0x1fa28U;
            const auto value = r.data[5];
            m.dw(7U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1fa2cU: { // 3a07 move.w d7, d5
            next = 0x1fa2eU;
            const auto value = r.data[7];
            m.dw(5U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1fa2eU: { // 2d40001e move.l d0, $1e(a6)
            next = 0x1fa32U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x1eU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1fa32U: { // 2d410026 move.l d1, $26(a6)
            next = 0x1fa36U;
            const auto value = r.data[1];
            const auto destination_address = r.address[6] + 0x26U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01f99e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1fa14U: { // 4eb90001636c jsr $1636c.l
            next = 0x1fa1aU;
            const auto result = m.call(c, 305U, 0x1fa14U, 0x1636cU, 0x1fa1aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1fa28U: { // 61000a12 bsr.w $2043c
            next = 0x1fa2cU;
            const auto result = m.call(c, 383U, 0x1fa28U, 0x2043cU, 0x1fa2cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1fa36U: { // 4e75 rts 
            next = 0x1fa38U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1fa36U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01f99e_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01f99e_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01f99e_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

static bool cpu_b_stage_0203e8_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x203e8U: { // 303c003f move.w #$3f, d0
            next = 0x203ecU;
            const auto value = 0x3fU;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x203ecU: { // 4df85400 lea.l $5400.w, a6
            next = 0x203f0U;
            const auto source_address = 0x5400U;
            r.address[6] = source_address;
            break;
        }
        case 0x203f0U: { // 9cfc0080 suba.w #$80, a6
            next = 0x203f4U;
            const auto source_value = 0x80U;
            r.address[6] -= static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x203f4U: { // 4a16 tst.b (a6)
            next = 0x203f6U;
            const auto destination_address = r.address[6];
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x203f6U: { // 6a04 bpl.b $203fc
            next = 0x203f8U;
            if ((r.status & 8U) == 0U) { next = 0x203fcU; transfer_kind = 1U; }
            break;
        }
        case 0x203f8U: { // 51c8fff6 dbra d0, $203f0
            next = 0x203fcU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x203f0U;
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_0203e8(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x203fcU: { // 4e75 rts 
            next = 0x203feU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x203fcU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_0203e8_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x203e8U:
        case 0x203ecU:
        case 0x203f0U:
        case 0x203f4U:
        case 0x203f6U:
        case 0x203f8U:
        case 0x203fcU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_01f800_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1f800U:
    case 0x1f804U:
    case 0x1f806U:
    case 0x1f808U:
    case 0x1f80eU:
    case 0x1f812U:
    case 0x1f816U:
    case 0x1f818U:
    case 0x1f81cU:
    case 0x1f81eU:
    case 0x1f822U:
    case 0x1f824U:
    case 0x1f826U:
    case 0x1f82aU:
    case 0x1f830U:
    case 0x1f836U:
    case 0x1f83cU:
    case 0x1f842U:
    case 0x1f848U:
    case 0x1f84cU:
    case 0x1f84eU:
    case 0x1f854U:
    case 0x1f856U:
    case 0x1f85aU:
    case 0x1f85cU:
    case 0x1f860U:
    case 0x1f87cU:
    case 0x1f880U:
    case 0x1f882U:
    case 0x1f886U:
    case 0x1f88aU:
    case 0x1f88cU:
    case 0x1f88eU:
    case 0x1f890U:
    case 0x1f892U:
    case 0x1f896U:
    case 0x1f898U:
    case 0x1f89cU:
    case 0x1f89eU:
    case 0x1f8a2U:
    case 0x1f8a6U:
    case 0x1f8acU:
    case 0x1f8b2U:
    case 0x1f8b8U:
    case 0x1f8baU:
    case 0x1f8beU:
    case 0x1f8c0U:
    case 0x1f8c4U:
    case 0x1f8c8U:
    case 0x1f8ceU:
    case 0x1f8d4U:
    case 0x1f8daU:
    case 0x1f8dcU:
    case 0x1f8e0U:
    case 0x1f8e6U:
    case 0x1f8eaU:
    case 0x1f8ecU:
    case 0x1f8eeU:
    case 0x1f8f2U:
    case 0x1f8f6U:
    case 0x1f8f8U:
    case 0x1f8fcU:
    case 0x1f900U:
    case 0x1f904U:
    case 0x1f908U:
    case 0x1f90aU:
    case 0x1f90cU:
    case 0x1f910U:
    case 0x1f914U:
    case 0x1f916U:
    case 0x1f918U:
    case 0x1f91aU:
    case 0x1f91cU:
    case 0x1f91eU:
    case 0x1f920U:
    case 0x1f924U:
    case 0x1f926U:
    case 0x1f928U:
    case 0x1f92aU:
    case 0x1f92eU:
    case 0x1f934U:
    case 0x1f936U:
    case 0x1f93aU:
    case 0x1f93cU:
    case 0x1f93eU:
    case 0x1f942U:
    case 0x1f946U:
    case 0x1f94cU:
    case 0x1f954U:
    case 0x1f956U:
    case 0x1f958U:
    case 0x1f95eU:
    case 0x1f960U:
    case 0x1f962U:
    case 0x1f964U:
    case 0x1f966U:
    case 0x1f96aU:
        return true;
    default: return false;
    }
}
