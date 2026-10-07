
static bool cpu_b_stage_01d920_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1d920U:
    case 0x1d924U:
    case 0x1d926U:
    case 0x1d928U:
    case 0x1d92cU:
    case 0x1d930U:
    case 0x1d932U:
    case 0x1d936U:
    case 0x1d93aU:
    case 0x1d93cU:
    case 0x1d940U:
    case 0x1d942U:
    case 0x1d946U:
    case 0x1d948U:
    case 0x1d94aU:
    case 0x1d94cU:
    case 0x1d94eU:
    case 0x1d952U:
    case 0x1d954U:
    case 0x1d956U:
    case 0x1d95aU:
    case 0x1d95cU:
    case 0x1d960U:
    case 0x1d964U:
    case 0x1d968U:
    case 0x1d96cU:
    case 0x1d970U:
    case 0x1d974U:
    case 0x1d978U:
    case 0x1d97cU:
    case 0x1d980U:
    case 0x1d984U:
    case 0x1d988U:
    case 0x1d98cU:
    case 0x1d992U:
    case 0x1d996U:
    case 0x1d99aU:
    case 0x1d99eU:
    case 0x1d9a2U:
    case 0x1d9a4U:
    case 0x1d9a6U:
    case 0x1d9a8U:
    case 0x1d9acU:
    case 0x1d9aeU:
    case 0x1d9b0U:
    case 0x1d9b4U:
    case 0x1d9b6U:
    case 0x1d9b8U:
    case 0x1d9baU:
    case 0x1d9bcU:
    case 0x1d9c0U:
    case 0x1d9c2U:
    case 0x1d9c4U:
    case 0x1d9c6U:
    case 0x1d9c8U:
    case 0x1d9ccU:
    case 0x1d9ceU:
    case 0x1d9d2U:
    case 0x1d9d6U:
        return true;
    default: return false;
    }
}

static bool cpu_b_stage_01d920_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1d920U: { // 47f86c00 lea.l $6c00.w, a3
            next = 0x1d924U;
            const auto source_address = 0x6c00U;
            r.address[3] = source_address;
            break;
        }
        case 0x1d924U: { // 4a5b tst.w (a3)+
            next = 0x1d926U;
            const auto destination_address = r.address[3];
            const auto value = m.word(destination_address);
            r.address[3] += 2U;
            m.logic(value, 16U);
            break;
        }
        case 0x1d926U: { // 6b04 bmi.b $1d92c
            next = 0x1d928U;
            if ((r.status & 8U) != 0U) { next = 0x1d92cU; transfer_kind = 1U; }
            break;
        }
        case 0x1d928U: { // 47f87002 lea.l $7002.w, a3
            next = 0x1d92cU;
            const auto source_address = 0x7002U;
            r.address[3] = source_address;
            break;
        }
        case 0x1d92cU: { // 302a0002 move.w $2(a2), d0
            next = 0x1d930U;
            const auto source_address = r.address[2] + 0x2U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1d930U: { // 3e00 move.w d0, d7
            next = 0x1d932U;
            const auto value = r.data[0];
            m.dw(7U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1d932U: { // 06470020 addi.w #$20, d7
            next = 0x1d936U;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[7];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(7U, value);
            break;
        }
        case 0x1d936U: { // 04400020 subi.w #$20, d0
            next = 0x1d93aU;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1d93aU: { // 6b10 bmi.b $1d94c
            next = 0x1d93cU;
            if ((r.status & 8U) != 0U) { next = 0x1d94cU; transfer_kind = 1U; }
            break;
        }
        case 0x1d93cU: { // 0c4701ff cmpi.w #$1ff, d7
            next = 0x1d940U;
            const auto source_value = 0x1ffU;
            const auto destination_value = r.data[7];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1d940U: { // 6f04 ble.b $1d946
            next = 0x1d942U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1d946U; transfer_kind = 1U; }
            break;
        }
        case 0x1d942U: { // 3e3c01ff move.w #$1ff, d7
            next = 0x1d946U;
            const auto value = 0x1ffU;
            m.dw(7U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1d946U: { // 9e40 sub.w d0, d7
            next = 0x1d948U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[7];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(7U, value);
            break;
        }
        case 0x1d948U: { // d040 add.w d0, d0
            next = 0x1d94aU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1d94aU: { // d6c0 adda.w d0, a3
            next = 0x1d94cU;
            const auto source_value = r.data[0];
            r.address[3] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1d94cU: { // 301b move.w (a3)+, d0
            next = 0x1d94eU;
            const auto source_address = r.address[3];
            const auto value = m.word(source_address);
            r.address[3] += 2U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1d94eU: { // 6f00007e ble.w $1d9ce
            next = 0x1d952U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1d9ceU; transfer_kind = 1U; }
            break;
        }
        case 0x1d952U: { // 3c40 movea.w d0, a6
            next = 0x1d954U;
            const auto value = r.data[0];
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x1d954U: { // 7000 moveq #$0, d0
            next = 0x1d956U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1d956U: { // 102e000b move.b $b(a6), d0
            next = 0x1d95aU;
            const auto source_address = r.address[6] + 0xbU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1d95aU: { // e540 asl.w #$2, d0
            next = 0x1d95cU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x1d95cU: { // 4efb0002 jmp $1d960(pc, d0.w)
            next = 0x1d960U;
            next = (0x1d960U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x1d960U: { // 6000006c bra.w $1d9ce
            next = 0x1d964U;
            if (true) { next = 0x1d9ceU; transfer_kind = 1U; }
            break;
        }
        case 0x1d964U: { // 6000002c bra.w $1d992
            next = 0x1d968U;
            if (true) { next = 0x1d992U; transfer_kind = 1U; }
            break;
        }
        case 0x1d968U: { // 60000064 bra.w $1d9ce
            next = 0x1d96cU;
            if (true) { next = 0x1d9ceU; transfer_kind = 1U; }
            break;
        }
        case 0x1d96cU: { // 60000060 bra.w $1d9ce
            next = 0x1d970U;
            if (true) { next = 0x1d9ceU; transfer_kind = 1U; }
            break;
        }
        case 0x1d970U: { // 60000020 bra.w $1d992
            next = 0x1d974U;
            if (true) { next = 0x1d992U; transfer_kind = 1U; }
            break;
        }
        case 0x1d974U: { // 60000058 bra.w $1d9ce
            next = 0x1d978U;
            if (true) { next = 0x1d9ceU; transfer_kind = 1U; }
            break;
        }
        case 0x1d978U: { // 60000054 bra.w $1d9ce
            next = 0x1d97cU;
            if (true) { next = 0x1d9ceU; transfer_kind = 1U; }
            break;
        }
        case 0x1d97cU: { // 60000050 bra.w $1d9ce
            next = 0x1d980U;
            if (true) { next = 0x1d9ceU; transfer_kind = 1U; }
            break;
        }
        case 0x1d980U: { // 60000010 bra.w $1d992
            next = 0x1d984U;
            if (true) { next = 0x1d992U; transfer_kind = 1U; }
            break;
        }
        case 0x1d984U: { // 6000000c bra.w $1d992
            next = 0x1d988U;
            if (true) { next = 0x1d992U; transfer_kind = 1U; }
            break;
        }
        case 0x1d988U: { // 60000008 bra.w $1d992
            next = 0x1d98cU;
            if (true) { next = 0x1d992U; transfer_kind = 1U; }
            break;
        }
        case 0x1d98cU: { // 60000004 bra.w $1d992
            next = 0x1d990U;
            if (true) { next = 0x1d992U; transfer_kind = 1U; }
            break;
        }
        case 0x1d992U: { // 323cffec move.w #$ffec, d1
            next = 0x1d996U;
            const auto value = 0xffecU;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1d996U: { // 343c0014 move.w #$14, d2
            next = 0x1d99aU;
            const auto value = 0x14U;
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1d99aU: { // 363cffec move.w #$ffec, d3
            next = 0x1d99eU;
            const auto value = 0xffecU;
            m.dw(3U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1d99eU: { // 383c0012 move.w #$12, d4
            next = 0x1d9a2U;
            const auto value = 0x12U;
            m.dw(4U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1d9a2U: { // 3012 move.w (a2), d0
            next = 0x1d9a4U;
            const auto source_address = r.address[2];
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
