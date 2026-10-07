CpuAResetEntryStep handle_cpu_a_reset_entry_pc_0023c4_002400(
    FunctionContext &c, CpuRegisters &r, unverified::Machine &m,
    std::uint32_t pc, std::uint32_t &next)
{
    switch (pc) {
        case 0x23c4U: { // 3f3c0000 move.w #$0, -(a7)
            next = 0x23c8U;
            const auto value = 0x0U;
            r.address[7] -= 2U;
            const auto destination_address = r.address[7];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x23c8U: { // 13fc00040080001d move.b #$4, $80001d.l
            next = 0x23d0U;
            const auto value = 0x4U;
            const auto destination_address = 0x80001dU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x23d0U: { // 13fc000800a00005 move.b #$8, $a00005.l
            next = 0x23d8U;
            const auto value = 0x8U;
            const auto destination_address = 0xa00005U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x23d8U: { // 6100fcf2 bsr.w $20cc
            next = 0x23dcU;
            const auto result = m.call(c, 31U, 0x23d8U, 0x20ccU, 0x23dcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x23dcU: { // 61001086 bsr.w $3464
            next = 0x23e0U;
            const auto result = m.call(c, 38U, 0x23dcU, 0x3464U, 0x23e0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x23e0U: { // 33fcffc600240000 move.w #$ffc6, $240000.l
            next = 0x23e8U;
            const auto value = 0xffc6U;
            const auto destination_address = 0x240000U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x23e8U: { // 33fcfff000260000 move.w #$fff0, $260000.l
            next = 0x23f0U;
            const auto value = 0xfff0U;
            const auto destination_address = 0x260000U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x23f0U: { // 13fc00010040401b move.b #$1, $40401b.l
            next = 0x23f8U;
            const auto value = 0x1U;
            const auto destination_address = 0x40401bU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x23f8U: { // 4e722300 stop #$2300
            next = 0x23fcU;
            r.status = static_cast<std::uint16_t>(0x2300U);
            for (;;) { if (auto event = m.interrupt(c, 0x23f8U, 0x23fcU)) return {true, (*event)}; }
            break;
        }
        case 0x23fcU: { // 41faffba lea.l $23b8(pc), a0
            next = 0x2400U;
            const auto source_address = 0x23b8U;
            r.address[0] = source_address;
            break;
        }
    default: return {};
    }
    return {true, std::nullopt};
}
