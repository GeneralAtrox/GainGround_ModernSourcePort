
static bool cpu_b_stage_01a3b4_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1a43eU: { // 2b40001e move.l d0, $1e(a5)
            next = 0x1a442U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x1eU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1a442U: { // 2b410026 move.l d1, $26(a5)
            next = 0x1a446U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x26U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1a446U: { // 526d0066 addq.w #$1, $66(a5)
            next = 0x1a44aU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1a44aU: { // 202d001e move.l $1e(a5), d0
            next = 0x1a44eU;
            const auto source_address = r.address[5] + 0x1eU;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1a44eU: { // d1ad0012 add.l d0, $12(a5)
            next = 0x1a452U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x12U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x1a452U: { // 202d0026 move.l $26(a5), d0
            next = 0x1a456U;
            const auto source_address = r.address[5] + 0x26U;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1a456U: { // d1ad001a add.l d0, $1a(a5)
            next = 0x1a45aU;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x1a45aU: { // 302d005a move.w $5a(a5), d0
            next = 0x1a45eU;
            const auto source_address = r.address[5] + 0x5aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a45eU: { // d07c0040 add.w #$40, d0
            next = 0x1a462U;
            const auto source_value = 0x40U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a462U: { // ea48 lsr.w #$5, d0
            next = 0x1a464U;
            m.shift_word(0U, 5U, false, false);
            break;
        }
        case 0x1a464U: { // 0240003c andi.w #$3c, d0
            next = 0x1a468U;
            const auto source_value = 0x3cU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a468U: { // 41fa12e8 lea.l $1b752(pc), a0
            next = 0x1a46cU;
            const auto source_address = 0x1b752U;
            r.address[0] = source_address;
            break;
        }
        case 0x1a46cU: { // 3b7000000006 move.w (a0, d0.w), $6(a5)
            next = 0x1a472U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a472U: { // 1b7000030001 move.b $3(a0, d0.w), $1(a5)
            next = 0x1a478U;
            const auto source_address = r.address[0] + 0x3U + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a478U: { // 526d0064 addq.w #$1, $64(a5)
            next = 0x1a47cU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1a47cU: { // 302d0064 move.w $64(a5), d0
            next = 0x1a480U;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a480U: { // 02400003 andi.w #$3, d0
            next = 0x1a484U;
            const auto source_value = 0x3U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a484U: { // d07c0c94 add.w #$c94, d0
            next = 0x1a488U;
            const auto source_value = 0xc94U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a488U: { // 3b400008 move.w d0, $8(a5)
            next = 0x1a48cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a492U: { // 41fa147e lea.l $1b912(pc), a0
            next = 0x1a496U;
            const auto source_address = 0x1b912U;
            r.address[0] = source_address;
            break;
        }
        case 0x1a4a2U: { // 2b7c0001a4b00002 move.l #$1a4b0, $2(a5)
            next = 0x1a4aaU;
            const auto value = 0x1a4b0U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1a4aaU: { // 426d0064 clr.w $64(a5)
            next = 0x1a4aeU;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1a4aeU: { // 6026 bra.b $1a4d6
            next = 0x1a4b0U;
            if (true) { next = 0x1a4d6U; transfer_kind = 1U; }
            break;
        }
        case 0x1a4d6U: { // 302d0064 move.w $64(a5), d0
            next = 0x1a4daU;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a4daU: { // 41fa1256 lea.l $1b732(pc), a0
            next = 0x1a4deU;
            const auto source_address = 0x1b732U;
            r.address[0] = source_address;
            break;
        }
        case 0x1a4deU: { // d0c0 adda.w d0, a0
            next = 0x1a4e0U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1a4e0U: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x1a4e4U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a4e4U: { // 1b580001 move.b (a0)+, $1(a5)
            next = 0x1a4e8U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a4e8U: { // 303c0900 move.w #$900, d0
            next = 0x1a4ecU;
            const auto value = 0x900U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a4ecU: { // 1018 move.b (a0)+, d0
            next = 0x1a4eeU;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a4eeU: { // 3b400008 move.w d0, $8(a5)
            next = 0x1a4f2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
