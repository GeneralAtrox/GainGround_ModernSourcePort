
static bool cpu_b_stage_019806_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19806U: { // 7200 moveq #$0, d1
            next = 0x19808U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x19808U: { // 122d0051 move.b $51(a5), d1
            next = 0x1980cU;
            const auto source_address = r.address[5] + 0x51U;
            const auto value = m.byte(source_address);
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1980cU: { // 6f000128 ble.w $19936
            next = 0x19810U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x19936U; transfer_kind = 1U; }
            break;
        }
        case 0x19810U: { // 532d0050 subq.b #$1, $50(a5)
            next = 0x19814U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x50U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x19814U: { // 6e000120 bgt.w $19936
            next = 0x19818U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19936U; transfer_kind = 1U; }
            break;
        }
        case 0x19818U: { // 1b7c00020050 move.b #$2, $50(a5)
            next = 0x1981eU;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x50U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1981eU: { // 5341 subq.w #$1, d1
            next = 0x19820U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x19820U: { // 1b410051 move.b d1, $51(a5)
            next = 0x19824U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x51U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19828U: { // 4a40 tst.w d0
            next = 0x1982aU;
            const auto value = r.data[0];
            m.logic(value, 16U);
            break;
        }
        case 0x1982aU: { // 6b00010a bmi.w $19936
            next = 0x1982eU;
            if ((r.status & 8U) != 0U) { next = 0x19936U; transfer_kind = 1U; }
            break;
        }
        case 0x1982eU: { // 0c2d00050051 cmpi.b #$5, $51(a5)
            next = 0x19834U;
            const auto source_value = 0x5U;
            const auto destination_address = r.address[5] + 0x51U;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x19834U: { // 6660 bne.b $19896
            next = 0x19836U;
            if ((r.status & 4U) == 0U) { next = 0x19896U; transfer_kind = 1U; }
            break;
        }
        case 0x19836U: { // 302d0056 move.w $56(a5), d0
            next = 0x1983aU;
            const auto source_address = r.address[5] + 0x56U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1983aU: { // 3200 move.w d0, d1
            next = 0x1983cU;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1983cU: { // ee40 asr.w #$7, d0
            next = 0x1983eU;
            m.shift_word(0U, 7U, false, true);
            break;
        }
        case 0x1983eU: { // 41fa1bb4 lea.l $1b3f4(pc), a0
            next = 0x19842U;
            const auto source_address = 0x1b3f4U;
            r.address[0] = source_address;
            break;
        }
        case 0x19842U: { // d040 add.w d0, d0
            next = 0x19844U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19844U: { // 30300000 move.w (a0, d0.w), d0
            next = 0x19848U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19848U: { // 3b400056 move.w d0, $56(a5)
            next = 0x1984cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x56U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1984cU: { // 9240 sub.w d0, d1
            next = 0x1984eU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1984eU: { // e841 asr.w #$4, d1
            next = 0x19850U;
            m.shift_word(1U, 4U, false, true);
            break;
        }
        case 0x19850U: { // 6b02 bmi.b $19854
            next = 0x19852U;
            if ((r.status & 8U) != 0U) { next = 0x19854U; transfer_kind = 1U; }
            break;
        }
        case 0x19852U: { // 5241 addq.w #$1, d1
            next = 0x19854U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x19854U: { // 3b410072 move.w d1, $72(a5)
            next = 0x19858U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x72U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19858U: { // 303c0010 move.w #$10, d0
            next = 0x1985cU;
            const auto value = 0x10U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1985cU: { // 323c0008 move.w #$8, d1
            next = 0x19860U;
            const auto value = 0x8U;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19860U: { // 0c6d00080058 cmpi.w #$8, $58(a5)
            next = 0x19866U;
            const auto source_value = 0x8U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19866U: { // 6f10 ble.b $19878
            next = 0x19868U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x19878U; transfer_kind = 1U; }
            break;
        }
        case 0x19868U: { // 4441 neg.w d1
            next = 0x1986aU;
            const auto old = r.data[1];
            const auto value = m.sub(0U, old, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1986aU: { // 0c6d00120058 cmpi.w #$12, $58(a5)
            next = 0x19870U;
            const auto source_value = 0x12U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19870U: { // 6a06 bpl.b $19878
            next = 0x19872U;
            if ((r.status & 8U) == 0U) { next = 0x19878U; transfer_kind = 1U; }
            break;
        }
        case 0x19872U: { // 303c000f move.w #$f, d0
            next = 0x19876U;
            const auto value = 0xfU;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19876U: { // 7200 moveq #$0, d1
            next = 0x19878U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x19878U: { // d06d000c add.w $c(a5), d0
            next = 0x1987cU;
            const auto source_address = r.address[5] + 0xcU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1987cU: { // d26d000e add.w $e(a5), d1
            next = 0x19880U;
            const auto source_address = r.address[5] + 0xeU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
    default: return false;
    }
    return true;
}
