
static bool cpu_b_stage_019cd4_dispatch_cases_002([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19df4U: { // 526d0064 addq.w #$1, $64(a5)
            next = 0x19df8U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19dfaU: { // 3b7c00030048 move.w #$3, $48(a5)
            next = 0x19e00U;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x48U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19e00U: { // 426d0064 clr.w $64(a5)
            next = 0x19e04U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19e04U: { // 3b7c00080068 move.w #$8, $68(a5)
            next = 0x19e0aU;
            const auto value = 0x8U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19e0cU: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x19e10U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19e10U: { // 6e000128 bgt.w $19f3a
            next = 0x19e14U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19f3aU; transfer_kind = 1U; }
            break;
        }
        case 0x19e14U: { // 3b7c00080068 move.w #$8, $68(a5)
            next = 0x19e1aU;
            const auto value = 0x8U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19e1aU: { // 526d0064 addq.w #$1, $64(a5)
            next = 0x19e1eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19e1eU: { // 0c6d00040064 cmpi.w #$4, $64(a5)
            next = 0x19e24U;
            const auto source_value = 0x4U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19e24U: { // 6400012a bcc.w $19f50
            next = 0x19e28U;
            if ((r.status & 1U) == 0U) { next = 0x19f50U; transfer_kind = 1U; }
            break;
        }
        case 0x19e28U: { // 0c6d00030064 cmpi.w #$3, $64(a5)
            next = 0x19e2eU;
            const auto source_value = 0x3U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19e2eU: { // 6600010a bne.w $19f3a
            next = 0x19e32U;
            if ((r.status & 4U) == 0U) { next = 0x19f3aU; transfer_kind = 1U; }
            break;
        }
        case 0x19e32U: { // 41fa18d4 lea.l $1b708(pc), a0
            next = 0x19e36U;
            const auto source_address = 0x1b708U;
            r.address[0] = source_address;
            break;
        }
        case 0x19e36U: { // 7401 moveq #$1, d2
            next = 0x19e38U;
            r.data[2] = 0x1U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0x19e3cU: { // 4a40 tst.w d0
            next = 0x19e3eU;
            const auto value = r.data[0];
            m.logic(value, 16U);
            break;
        }
        case 0x19e3eU: { // 6b0000fa bmi.w $19f3a
            next = 0x19e42U;
            if ((r.status & 8U) != 0U) { next = 0x19f3aU; transfer_kind = 1U; }
            break;
        }
        case 0x19e42U: { // 3cbc8000 move.w #$8000, (a6)
            next = 0x19e46U;
            const auto value = 0x8000U;
            const auto destination_address = r.address[6];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19e46U: { // 2d7c0001a3b40002 move.l #$1a3b4, $2(a6)
            next = 0x19e4eU;
            const auto value = 0x1a3b4U;
            const auto destination_address = r.address[6] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x19e4eU: { // 3d7c04af0006 move.w #$4af, $6(a6)
            next = 0x19e54U;
            const auto value = 0x4afU;
            const auto destination_address = r.address[6] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19e54U: { // 3d7c0c940008 move.w #$c94, $8(a6)
            next = 0x19e5aU;
            const auto value = 0xc94U;
            const auto destination_address = r.address[6] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19e5aU: { // 1d7c0007000b move.b #$7, $b(a6)
            next = 0x19e60U;
            const auto value = 0x7U;
            const auto destination_address = r.address[6] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19e60U: { // 422e003f clr.b $3f(a6)
            next = 0x19e64U;
            const auto destination_address = r.address[6] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x19e64U: { // 426e0064 clr.w $64(a6)
            next = 0x19e68U;
            const auto destination_address = r.address[6] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19e68U: { // 426e0066 clr.w $66(a6)
            next = 0x19e6cU;
            const auto destination_address = r.address[6] + 0x66U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19e6cU: { // 426e0068 clr.w $68(a6)
            next = 0x19e70U;
            const auto destination_address = r.address[6] + 0x68U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19e70U: { // 3d7c0200005a move.w #$200, $5a(a6)
            next = 0x19e76U;
            const auto value = 0x200U;
            const auto destination_address = r.address[6] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19e76U: { // 302d0012 move.w $12(a5), d0
            next = 0x19e7aU;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19e7aU: { // d058 add.w (a0)+, d0
            next = 0x19e7cU;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19e7cU: { // 3d400012 move.w d0, $12(a6)
            next = 0x19e80U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19e80U: { // 3d40002a move.w d0, $2a(a6)
            next = 0x19e84U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19e84U: { // 3d40002c move.w d0, $2c(a6)
            next = 0x19e88U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19e88U: { // 3d7c00100016 move.w #$10, $16(a6)
            next = 0x19e8eU;
            const auto value = 0x10U;
            const auto destination_address = r.address[6] + 0x16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
