CpuAFdcStep handle_cpu_a_dynamic_fdc_request_sample_and_idle_return(
    FunctionContext &c, CpuRegisters &r, unverified::Machine &m,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind)
{
    switch (pc) {
        case 0x80356U: { // 1039fff00820 move.b $fff00820.l, d0
            next = 0x8035cU;
            const auto source_address = 0xfff00820U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x8035cU: { // 02400008 andi.w #$8, d0
            next = 0x80360U;
            const auto source_value = 0x8U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x80360U: { // 6638 bne.b $8039a
            next = 0x80362U;
            if ((r.status & 4U) == 0U) { next = 0x8039aU; transfer_kind = 1U; }
            break;
        }
        case 0x80362U: { // 4a39fff00418 tst.b $fff00418.l
            next = 0x80368U;
            const auto destination_address = 0xfff00418U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x80368U: { // 6730 beq.b $8039a
            next = 0x8036aU;
            if ((r.status & 4U) != 0U) { next = 0x8039aU; transfer_kind = 1U; }
            break;
        }
        case 0x8036aU: { // 46fc2700 move.w #$2700, sr
            next = 0x8036eU;
            r.status = static_cast<std::uint16_t>(0x2700U);
            break;
        }
        case 0x8036eU: { // 4239fff00418 clr.b $fff00418.l
            next = 0x80374U;
            const auto destination_address = 0xfff00418U;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x80374U: { // 50f88001 st.b $8001.w
            next = 0x80378U;
            const auto destination_address = 0xffff8001U;
            m.byte(destination_address, 0xffU);
            break;
        }
        case 0x80378U: { // 70ff moveq #$ff, d0
            next = 0x8037aU;
            r.data[0] = 0xffffffffU;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x8037aU: { // 4e48 trap #$8
            next = 0x8037cU;
            const auto result = m.exception(c, 40U, 0x8037aU, 0x8037cU);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x8037cU) return {true, (result)};
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x8037cU: { // 41f8d000 lea.l $d000.w, a0
            next = 0x80380U;
            r.address[0] = 0xffffd000U;
            break;
        }
        case 0x80380U: { // 43f9fff07b20 lea.l $fff07b20.l, a1
            next = 0x80386U;
            r.address[1] = 0xfff07b20U;
            break;
        }
        case 0x80386U: { // 223c0000004c move.l #$4c, d1
            next = 0x8038cU;
            const auto value = 0x4cU;
            r.data[1] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x8038cU: { // 4e47 trap #$7
            next = 0x8038eU;
            const auto result = m.exception(c, 39U, 0x8038cU, 0x8038eU);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x8038eU) return {true, (result)};
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x8038eU: { // 7000 moveq #$0, d0
            next = 0x80390U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x80390U: { // 4e48 trap #$8
            next = 0x80392U;
            const auto result = m.exception(c, 40U, 0x80390U, 0x80392U);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x80392U) return {true, (result)};
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x80392U: { // 42388001 clr.b $8001.w
            next = 0x80396U;
            const auto destination_address = 0xffff8001U;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x80396U: { // 46fc2000 move.w #$2000, sr
            next = 0x8039aU;
            r.status = static_cast<std::uint16_t>(0x2000U);
            break;
        }
        case 0x8039aU: { // 30388002 move.w $8002.w, d0
            next = 0x8039eU;
            const auto source_address = 0xffff8002U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x8039eU: { // 6602 bne.b $803a2
            next = 0x803a0U;
            if ((r.status & 4U) == 0U) { next = 0x803a2U; transfer_kind = 1U; }
            break;
        }
        case 0x803a0U: { // 4e75 rts 
            next = 0x803a2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x803a0U, r.program_counter)) return {true, (*event)};
            return {true, (result)};
            break;
        }
    default: return {};
    }
    return {true, std::nullopt};
}
