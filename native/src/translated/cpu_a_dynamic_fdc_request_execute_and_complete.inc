CpuAFdcStep handle_cpu_a_dynamic_fdc_request_execute_and_complete(
    FunctionContext &c, CpuRegisters &r, unverified::Machine &m,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind)
{
    switch (pc) {
        case 0x803a2U: { // 0c400001 cmpi.w #$1, d0
            next = 0x803a6U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x803a6U: { // 6604 bne.b $803ac
            next = 0x803a8U;
            if ((r.status & 4U) == 0U) { next = 0x803acU; transfer_kind = 1U; }
            break;
        }
        case 0x803a8U: { // 6100ff3c bsr.w $802e6
            next = 0x803acU;
            const auto result = m.call(c, 60U, 0x803a8U, 0x802e6U, 0x803acU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x803acU: { // 30388002 move.w $8002.w, d0
            next = 0x803b0U;
            const auto source_address = 0xffff8002U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x803b0U: { // 0c400007 cmpi.w #$7, d0
            next = 0x803b4U;
            const auto source_value = 0x7U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x803b4U: { // 6c00fd0a bge.w $800c0
            next = 0x803b8U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x800c0U; transfer_kind = 1U; }
            break;
        }
        case 0x803b8U: { // b0788004 cmp.w $8004.w, d0
            next = 0x803bcU;
            const auto source_address = 0xffff8004U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x803bcU: { // 6750 beq.b $8040e
            next = 0x803beU;
            if ((r.status & 4U) != 0U) { next = 0x8040eU; transfer_kind = 1U; }
            break;
        }
        case 0x803beU: { // 31c08004 move.w d0, $8004.w
            next = 0x803c2U;
            const auto value = r.data[0];
            const auto destination_address = 0xffff8004U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x803c2U: { // 46fc2700 move.w #$2700, sr
            next = 0x803c6U;
            r.status = static_cast<std::uint16_t>(0x2700U);
            break;
        }
        case 0x803c6U: { // 50f88001 st.b $8001.w
            next = 0x803caU;
            const auto destination_address = 0xffff8001U;
            m.byte(destination_address, 0xffU);
            break;
        }
        case 0x803caU: { // 70ff moveq #$ff, d0
            next = 0x803ccU;
            r.data[0] = 0xffffffffU;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x803ccU: { // 4e48 trap #$8
            next = 0x803ceU;
            const auto result = m.exception(c, 40U, 0x803ccU, 0x803ceU);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x803ceU) return {true, (result)};
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x803ceU: { // 30388002 move.w $8002.w, d0
            next = 0x803d2U;
            const auto source_address = 0xffff8002U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x803d2U: { // d040 add.w d0, d0
            next = 0x803d4U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x803d4U: { // 4dfa02c2 lea.l $80698(pc), a6
            next = 0x803d8U;
            const auto source_address = 0x80698U;
            r.address[6] = source_address;
            break;
        }
        case 0x803d8U: { // dcc0 adda.w d0, a6
            next = 0x803daU;
            const auto source_value = r.data[0];
            r.address[6] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x803daU: { // dcd6 adda.w (a6), a6
            next = 0x803dcU;
            const auto source_address = r.address[6];
            const auto source_value = m.word(source_address);
            r.address[6] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x803dcU: { // 3e1e move.w (a6)+, d7
            next = 0x803deU;
            const auto source_address = r.address[6];
            const auto value = m.word(source_address);
            r.address[6] += 2U;
            m.dw(7U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x803deU: { // 41f8d000 lea.l $d000.w, a0
            next = 0x803e2U;
            r.address[0] = 0xffffd000U;
            break;
        }
        case 0x803e2U: { // 225e movea.l (a6)+, a1
            next = 0x803e4U;
            const auto source_address = r.address[6];
            const auto value = m.lng(source_address);
            r.address[6] += 4U;
            r.address[1] = value;
            break;
        }
        case 0x803e4U: { // 201e move.l (a6)+, d0
            next = 0x803e6U;
            const auto source_address = r.address[6];
            const auto value = m.lng(source_address);
            r.address[6] += 4U;
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x803e6U: { // 221e move.l (a6)+, d1
            next = 0x803e8U;
            const auto source_address = r.address[6];
            const auto value = m.lng(source_address);
            r.address[6] += 4U;
            r.data[1] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x803e8U: { // 3f07 move.w d7, -(a7)
            next = 0x803eaU;
            const auto value = r.data[7];
            r.address[7] -= 2U;
            const auto destination_address = r.address[7];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x803eaU: { // 4e4a trap #$a
            next = 0x803ecU;
            const auto result = m.exception(c, 42U, 0x803eaU, 0x803ecU);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x803ecU) return {true, (result)};
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x803ecU: { // 3e1f move.w (a7)+, d7
            next = 0x803eeU;
            const auto source_address = r.address[7];
            const auto value = m.word(source_address);
            r.address[7] += 2U;
            m.dw(7U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x803eeU: { // 51cfffee dbra d7, $803de
            next = 0x803f2U;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x803deU;
            break;
        }
        case 0x803f2U: { // 7000 moveq #$0, d0
            next = 0x803f4U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x803f4U: { // 4e48 trap #$8
            next = 0x803f6U;
            const auto result = m.exception(c, 40U, 0x803f4U, 0x803f6U);
            if (result.status != TranslationStatus::complete || r.program_counter != 0x803f6U) return {true, (result)};
            c.state = m.state;
            next = r.program_counter;
            break;
        }
        case 0x803f6U: { // 42388001 clr.b $8001.w
            next = 0x803faU;
            const auto destination_address = 0xffff8001U;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x803faU: { // 46fc2000 move.w #$2000, sr
            next = 0x803feU;
            r.status = static_cast<std::uint16_t>(0x2000U);
            break;
        }
        case 0x803feU: { // 30388002 move.w $8002.w, d0
            next = 0x80402U;
            const auto source_address = 0xffff8002U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x80402U: { // 6700000a beq.w $8040e
            next = 0x80406U;
            if ((r.status & 4U) != 0U) { next = 0x8040eU; transfer_kind = 1U; }
            break;
        }
        case 0x80406U: { // b0788004 cmp.w $8004.w, d0
            next = 0x8040aU;
            const auto source_address = 0xffff8004U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x8040aU: { // 6600ff4a bne.w $80356
            next = 0x8040eU;
            if ((r.status & 4U) == 0U) { next = 0x80356U; transfer_kind = 1U; }
            break;
        }
        case 0x8040eU: { // 42788002 clr.w $8002.w
            next = 0x80412U;
            const auto destination_address = 0xffff8002U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x80412U: { // 4e75 rts 
            next = 0x80414U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x80412U, r.program_counter)) return {true, (*event)};
            return {true, (result)};
            break;
        }
    default: return {};
    }
    return {true, std::nullopt};
}
