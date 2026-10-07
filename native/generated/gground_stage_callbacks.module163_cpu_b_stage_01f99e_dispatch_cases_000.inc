
static bool cpu_b_stage_01f99e_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1f99eU: { // 206d0066 movea.l $66(a5), a0
            next = 0x1f9a2U;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1f9a2U: { // 30280018 move.w $18(a0), d0
            next = 0x1f9a6U;
            const auto source_address = r.address[0] + 0x18U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f9a6U: { // d040 add.w d0, d0
            next = 0x1f9a8U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1f9a8U: { // 3200 move.w d0, d1
            next = 0x1f9aaU;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f9aaU: { // e941 asl.w #$4, d1
            next = 0x1f9acU;
            m.asl_word(1U, 4U);
            break;
        }
        case 0x1f9acU: { // d240 add.w d0, d1
            next = 0x1f9aeU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1f9aeU: { // 41f90002a6e0 lea.l $2a6e0.l, a0
            next = 0x1f9b4U;
            const auto source_address = 0x2a6e0U;
            r.address[0] = source_address;
            break;
        }
        case 0x1f9b4U: { // d0c1 adda.w d1, a0
            next = 0x1f9b6U;
            const auto source_value = r.data[1];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1f9b6U: { // 302d0016 move.w $16(a5), d0
            next = 0x1f9baU;
            const auto source_address = r.address[5] + 0x16U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f9baU: { // d058 add.w (a0)+, d0
            next = 0x1f9bcU;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1f9bcU: { // 3d40002e move.w d0, $2e(a6)
            next = 0x1f9c0U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2eU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f9c0U: { // 3d400030 move.w d0, $30(a6)
            next = 0x1f9c4U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x30U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f9c4U: { // 3d400016 move.w d0, $16(a6)
            next = 0x1f9c8U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f9c8U: { // 3d7c00000018 move.w #$0, $18(a6)
            next = 0x1f9ceU;
            const auto value = 0x0U;
            const auto destination_address = r.address[6] + 0x18U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f9ceU: { // 42ae002e clr.l $2e(a6)
            next = 0x1f9d2U;
            const auto destination_address = r.address[6] + 0x2eU;
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0x1f9d2U: { // 3005 move.w d5, d0
            next = 0x1f9d4U;
            const auto value = r.data[5];
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f9d4U: { // 3d40005c move.w d0, $5c(a6)
            next = 0x1f9d8U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x5cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f9d8U: { // 06400080 addi.w #$80, d0
            next = 0x1f9dcU;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1f9dcU: { // 02400700 andi.w #$700, d0
            next = 0x1f9e0U;
            const auto source_value = 0x700U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1f9e0U: { // ec48 lsr.w #$6, d0
            next = 0x1f9e2U;
            m.shift_word(0U, 6U, false, false);
            break;
        }
        case 0x1f9e2U: { // d0c0 adda.w d0, a0
            next = 0x1f9e4U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1f9e4U: { // 302d0012 move.w $12(a5), d0
            next = 0x1f9e8U;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f9e8U: { // d058 add.w (a0)+, d0
            next = 0x1f9eaU;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1f9eaU: { // 3d40002a move.w d0, $2a(a6)
            next = 0x1f9eeU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f9eeU: { // 3d40002c move.w d0, $2c(a6)
            next = 0x1f9f2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f9f2U: { // 3d400012 move.w d0, $12(a6)
            next = 0x1f9f6U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f9f6U: { // 426e0014 clr.w $14(a6)
            next = 0x1f9faU;
            const auto destination_address = r.address[6] + 0x14U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1f9faU: { // 302d001a move.w $1a(a5), d0
            next = 0x1f9feU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f9feU: { // d058 add.w (a0)+, d0
            next = 0x1fa00U;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1fa00U: { // 3d400032 move.w d0, $32(a6)
            next = 0x1fa04U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x32U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1fa04U: { // 3d400034 move.w d0, $34(a6)
            next = 0x1fa08U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x34U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
