CpuAResetEntryStep handle_cpu_a_reset_entry_pc_000600_000680(
    FunctionContext &c, CpuRegisters &r, unverified::Machine &m,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind)
{
    switch (pc) {
        case 0x600U: { // 51cefff2 dbra d6, $5f4
            next = 0x604U;
            m.dw(6U, r.data[6] - 1U);
            if ((r.data[6] & 0xffffU) != 0xffffU) next = 0x5f4U;
            break;
        }
        case 0x604U: { // 41e80020 lea.l $20(a0), a0
            next = 0x608U;
            r.address[0] = r.address[0] + 0x20U;
            break;
        }
        case 0x608U: { // 51cfffe8 dbra d7, $5f2
            next = 0x60cU;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x5f2U;
            break;
        }
        case 0x60cU: { // 41f900007380 lea.l $7380.l, a0
            next = 0x612U;
            r.address[0] = 0x7380U;
            break;
        }
        case 0x612U: { // 43f900280020 lea.l $280020.l, a1
            next = 0x618U;
            r.address[1] = 0x280020U;
            break;
        }
        case 0x618U: { // 303c0dbf move.w #$dbf, d0
            next = 0x61cU;
            m.dw(0U, 0xdbfU);
            m.logic(r.data[0], 16U);
            break;
        }
        case 0x61cU: { // 22d8 move.l (a0)+, (a1)+
            next = 0x61eU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[1];
            m.lng(destination_address, value);
            r.address[1] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x61eU: { // 51c8fffc dbra d0, $61c
            next = 0x622U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x61cU;
            break;
        }
        case 0x622U: { // 13fc00000040401b move.b #$0, $40401b.l
            next = 0x62aU;
            const auto value = 0x0U;
            const auto destination_address = 0x40401bU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x62aU: { // 4e722300 stop #$2300
            next = 0x62eU;
            r.status = static_cast<std::uint16_t>(0x2300U);
            for (;;) { if (auto event = m.interrupt(c, 0x62aU, 0x62eU)) return {true, (*event)}; }
            break;
        }
        case 0x62eU: { // 4278fc84 clr.w $fc84.w
            next = 0x632U;
            const auto destination_address = 0xfffffc84U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x632U: { // 70ff moveq #$ff, d0
            next = 0x634U;
            r.data[0] = 0xffffffffU;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x634U: { // 4a78fc84 tst.w $fc84.w
            next = 0x638U;
            const auto destination_address = 0xfffffc84U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x638U: { // 56c8fffa dbne d0, $634
            next = 0x63cU;
            if ((r.status & 4U) != 0U) {
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x634U;
            }
            break;
        }
        case 0x63cU: { // b07ce000 cmp.w #$e000, d0
            next = 0x640U;
            const auto source_value = 0xe000U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x640U: { // 6504 bcs.b $646
            next = 0x642U;
            if ((r.status & 1U) != 0U) { next = 0x646U; transfer_kind = 1U; }
            break;
        }
        case 0x642U: { // 46fc2600 move.w #$2600, sr
            next = 0x646U;
            r.status = static_cast<std::uint16_t>(0x2600U);
            break;
        }
        case 0x646U: { // 207c0000aa80 movea.l #$aa80, a0
            next = 0x64cU;
            const auto value = 0xaa80U;
            r.address[0] = value;
            break;
        }
        case 0x64cU: { // 227c0020441a movea.l #$20441a, a1
            next = 0x652U;
            const auto value = 0x20441aU;
            r.address[1] = value;
            break;
        }
        case 0x652U: { // 7e23 moveq #$23, d7
            next = 0x654U;
            r.data[7] = 0x23U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x654U: { // 7c0c moveq #$c, d6
            next = 0x656U;
            r.data[6] = 0xcU;
            m.logic(r.data[6], 32U);
            break;
        }
        case 0x656U: { // 3a3c0038 move.w #$38, d5
            next = 0x65aU;
            m.dw(5U, 0x38U);
            m.logic(r.data[5], 16U);
            break;
        }
        case 0x65aU: { // 7800 moveq #$0, d4
            next = 0x65cU;
            r.data[4] = 0x0U;
            m.logic(r.data[4], 32U);
            break;
        }
        case 0x65cU: { // 610000fa bsr.w $758
            next = 0x660U;
            const auto result = m.call(c, 3U, 0x65cU, 0x758U, 0x660U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x660U: { // 227c00204d8a movea.l #$204d8a, a1
            next = 0x666U;
            const auto value = 0x204d8aU;
            r.address[1] = value;
            break;
        }
        case 0x666U: { // 7e32 moveq #$32, d7
            next = 0x668U;
            r.data[7] = 0x32U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x668U: { // 7c05 moveq #$5, d6
            next = 0x66aU;
            r.data[6] = 0x5U;
            m.logic(r.data[6], 32U);
            break;
        }
        case 0x66aU: { // 3a3c001a move.w #$1a, d5
            next = 0x66eU;
            m.dw(5U, 0x1aU);
            m.logic(r.data[5], 16U);
            break;
        }
        case 0x66eU: { // 383c1000 move.w #$1000, d4
            next = 0x672U;
            m.dw(4U, 0x1000U);
            m.logic(r.data[4], 16U);
            break;
        }
        case 0x672U: { // 610000e4 bsr.w $758
            next = 0x676U;
            const auto result = m.call(c, 3U, 0x672U, 0x758U, 0x676U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x676U: { // 227c0020539a movea.l #$20539a, a1
            next = 0x67cU;
            const auto value = 0x20539aU;
            r.address[1] = value;
            break;
        }
        case 0x67cU: { // 7e24 moveq #$24, d7
            next = 0x67eU;
            r.data[7] = 0x24U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x67eU: { // 7c01 moveq #$1, d6
            next = 0x680U;
            r.data[6] = 0x1U;
            m.logic(r.data[6], 32U);
            break;
        }
    default: return {};
    }
    return {true, std::nullopt};
}
