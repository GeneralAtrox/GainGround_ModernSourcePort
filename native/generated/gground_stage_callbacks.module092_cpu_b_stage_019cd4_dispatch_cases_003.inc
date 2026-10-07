
static bool cpu_b_stage_019cd4_dispatch_cases_003([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19e8eU: { // 3d7c0010002e move.w #$10, $2e(a6)
            next = 0x19e94U;
            const auto value = 0x10U;
            const auto destination_address = r.address[6] + 0x2eU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19e94U: { // 3d7c00100030 move.w #$10, $30(a6)
            next = 0x19e9aU;
            const auto value = 0x10U;
            const auto destination_address = r.address[6] + 0x30U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19e9aU: { // 302d001a move.w $1a(a5), d0
            next = 0x19e9eU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19e9eU: { // 5940 subq.w #$4, d0
            next = 0x19ea0U;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19ea0U: { // 3d40001a move.w d0, $1a(a6)
            next = 0x19ea4U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19ea4U: { // 3d400032 move.w d0, $32(a6)
            next = 0x19ea8U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x32U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19ea8U: { // 3d400034 move.w d0, $34(a6)
            next = 0x19eacU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x34U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19eacU: { // 7202 moveq #$2, d1
            next = 0x19eaeU;
            r.data[1] = 0x2U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x19eaeU: { // 49f81480 lea.l $1480.w, a4
            next = 0x19eb2U;
            const auto source_address = 0x1480U;
            r.address[4] = source_address;
            break;
        }
        case 0x19eb2U: { // 302d0052 move.w $52(a5), d0
            next = 0x19eb6U;
            const auto source_address = r.address[5] + 0x52U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19eb6U: { // d07c0080 add.w #$80, d0
            next = 0x19ebaU;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19ebaU: { // 0c400100 cmpi.w #$100, d0
            next = 0x19ebeU;
            const auto source_value = 0x100U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19ebeU: { // 6f02 ble.b $19ec2
            next = 0x19ec0U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x19ec2U; transfer_kind = 1U; }
            break;
        }
        case 0x19ec0U: { // 7000 moveq #$0, d0
            next = 0x19ec2U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x19ec2U: { // 0c7400050044 cmpi.w #$5, $44(a4, d0.w)
            next = 0x19ec8U;
            const auto source_value = 0x5U;
            const auto destination_address = r.address[4] + 0x44U + static_cast<std::int16_t>(r.data[0]);
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19ec8U: { // 6704 beq.b $19ece
            next = 0x19ecaU;
            if ((r.status & 4U) != 0U) { next = 0x19eceU; transfer_kind = 1U; }
            break;
        }
        case 0x19ecaU: { // 51c9ffea dbra d1, $19eb6
            next = 0x19eceU;
            m.dw(1U, r.data[1] - 1U);
            if ((r.data[1] & 0xffffU) != 0xffffU) next = 0x19eb6U;
            break;
        }
        case 0x19eceU: { // 3b400052 move.w d0, $52(a5)
            next = 0x19ed2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x52U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19ed2U: { // d8c0 adda.w d0, a4
            next = 0x19ed4U;
            const auto source_value = r.data[0];
            r.address[4] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x19ed4U: { // 3d4c0036 move.w a4, $36(a6)
            next = 0x19ed8U;
            const auto value = r.address[4];
            const auto destination_address = r.address[6] + 0x36U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19ed8U: { // 51caff5e dbra d2, $19e38
            next = 0x19edcU;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0x19e38U;
            break;
        }
        case 0x19edcU: { // 7401 moveq #$1, d2
            next = 0x19edeU;
            r.data[2] = 0x1U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0x19ee2U: { // 4a40 tst.w d0
            next = 0x19ee4U;
            const auto value = r.data[0];
            m.logic(value, 16U);
            break;
        }
        case 0x19ee4U: { // 6b54 bmi.b $19f3a
            next = 0x19ee6U;
            if ((r.status & 8U) != 0U) { next = 0x19f3aU; transfer_kind = 1U; }
            break;
        }
        case 0x19ee6U: { // 3cbc8006 move.w #$8006, (a6)
            next = 0x19eeaU;
            const auto value = 0x8006U;
            const auto destination_address = r.address[6];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19eeaU: { // 2d7c0001a5000002 move.l #$1a500, $2(a6)
            next = 0x19ef2U;
            const auto value = 0x1a500U;
            const auto destination_address = r.address[6] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x19ef2U: { // 3d7c04b50006 move.w #$4b5, $6(a6)
            next = 0x19ef8U;
            const auto value = 0x4b5U;
            const auto destination_address = r.address[6] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19ef8U: { // 3d7c0c5f0008 move.w #$c5f, $8(a6)
            next = 0x19efeU;
            const auto value = 0xc5fU;
            const auto destination_address = r.address[6] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19efeU: { // 3d7c32590010 move.w #$3259, $10(a6)
            next = 0x19f04U;
            const auto value = 0x3259U;
            const auto destination_address = r.address[6] + 0x10U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19f04U: { // 1d7c0000000b move.b #$0, $b(a6)
            next = 0x19f0aU;
            const auto value = 0x0U;
            const auto destination_address = r.address[6] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19f0aU: { // 426e0064 clr.w $64(a6)
            next = 0x19f0eU;
            const auto destination_address = r.address[6] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19f0eU: { // 3d7c00030068 move.w #$3, $68(a6)
            next = 0x19f14U;
            const auto value = 0x3U;
            const auto destination_address = r.address[6] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19f14U: { // 302d000c move.w $c(a5), d0
            next = 0x19f18U;
            const auto source_address = r.address[5] + 0xcU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19f18U: { // d07c0026 add.w #$26, d0
            next = 0x19f1cU;
            const auto source_value = 0x26U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
    default: return false;
    }
    return true;
}
