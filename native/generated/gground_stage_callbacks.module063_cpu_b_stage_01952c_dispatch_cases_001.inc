
static bool cpu_b_stage_01952c_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x195d0U: { // 303c0081 move.w #$81, d0
            next = 0x195d4U;
            const auto value = 0x81U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x195d4U: { // 082d0000006b btst.b #$0, $6b(a5)
            next = 0x195daU;
            const auto destination_address = r.address[5] + 0x6bU;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x195daU: { // 6702 beq.b $195de
            next = 0x195dcU;
            if ((r.status & 4U) != 0U) { next = 0x195deU; transfer_kind = 1U; }
            break;
        }
        case 0x195dcU: { // 4440 neg.w d0
            next = 0x195deU;
            const auto old = r.data[0];
            const auto value = m.sub(0U, old, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x195deU: { // d16d0056 add.w d0, $56(a5)
            next = 0x195e2U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x56U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x195e2U: { // 026d03ff0056 andi.w #$3ff, $56(a5)
            next = 0x195e8U;
            const auto source_value = 0x3ffU;
            const auto destination_address = r.address[5] + 0x56U;
            const auto destination_value = m.word(destination_address);
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x195e8U: { // 3b7c00010048 move.w #$1, $48(a5)
            next = 0x195eeU;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x48U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x195eeU: { // 3b7c00010040 move.w #$1, $40(a5)
            next = 0x195f4U;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x40U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x195f4U: { // 1b7c00060051 move.b #$6, $51(a5)
            next = 0x195faU;
            const auto value = 0x6U;
            const auto destination_address = r.address[5] + 0x51U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x195faU: { // 422d0050 clr.b $50(a5)
            next = 0x195feU;
            const auto destination_address = r.address[5] + 0x50U;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x19600U: { // 426d0048 clr.w $48(a5)
            next = 0x19604U;
            const auto destination_address = r.address[5] + 0x48U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19604U: { // 3b7c00030040 move.w #$3, $40(a5)
            next = 0x1960aU;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x40U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1960aU: { // 426d0064 clr.w $64(a5)
            next = 0x1960eU;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1960eU: { // 3b7c00030068 move.w #$3, $68(a5)
            next = 0x19614U;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19616U: { // 3b7c00030048 move.w #$3, $48(a5)
            next = 0x1961cU;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x48U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1961cU: { // 3b7c00040040 move.w #$4, $40(a5)
            next = 0x19622U;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x40U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19622U: { // 426d0064 clr.w $64(a5)
            next = 0x19626U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19626U: { // 3b7c00040068 move.w #$4, $68(a5)
            next = 0x1962cU;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1962eU: { // 3b7c00040048 move.w #$4, $48(a5)
            next = 0x19634U;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x48U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19634U: { // 3b7c00030040 move.w #$3, $40(a5)
            next = 0x1963aU;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x40U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1963aU: { // 426d0064 clr.w $64(a5)
            next = 0x1963eU;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1963eU: { // 3b7c00030068 move.w #$3, $68(a5)
            next = 0x19644U;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19644U: { // 3b7c00030066 move.w #$3, $66(a5)
            next = 0x1964aU;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x66U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1964cU: { // 536d0066 subq.w #$1, $66(a5)
            next = 0x19650U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19650U: { // 6b12 bmi.b $19664
            next = 0x19652U;
            if ((r.status & 8U) != 0U) { next = 0x19664U; transfer_kind = 1U; }
            break;
        }
        case 0x19652U: { // 3b7c00030040 move.w #$3, $40(a5)
            next = 0x19658U;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x40U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19658U: { // 426d0064 clr.w $64(a5)
            next = 0x1965cU;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1965cU: { // 3b7c00030068 move.w #$3, $68(a5)
            next = 0x19662U;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19664U: { // 3b7c00050048 move.w #$5, $48(a5)
            next = 0x1966aU;
            const auto value = 0x5U;
            const auto destination_address = r.address[5] + 0x48U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1966aU: { // 3b7c00050040 move.w #$5, $40(a5)
            next = 0x19670U;
            const auto value = 0x5U;
            const auto destination_address = r.address[5] + 0x40U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
