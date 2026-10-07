
static bool cpu_b_stage_019fcc_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19fccU: { // 0c2d00080051 cmpi.b #$8, $51(a5)
            next = 0x19fd2U;
            const auto source_value = 0x8U;
            const auto destination_address = r.address[5] + 0x51U;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x19fd2U: { // 6500010a bcs.w $1a0de
            next = 0x19fd6U;
            if ((r.status & 1U) != 0U) { next = 0x1a0deU; transfer_kind = 1U; }
            break;
        }
        case 0x19fd6U: { // 532d0050 subq.b #$1, $50(a5)
            next = 0x19fdaU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x50U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x19fdaU: { // 6e000102 bgt.w $1a0de
            next = 0x19fdeU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1a0deU; transfer_kind = 1U; }
            break;
        }
        case 0x19fdeU: { // 1b7c001e0050 move.b #$1e, $50(a5)
            next = 0x19fe4U;
            const auto value = 0x1eU;
            const auto destination_address = r.address[5] + 0x50U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19fe8U: { // 4a40 tst.w d0
            next = 0x19feaU;
            const auto value = r.data[0];
            m.logic(value, 16U);
            break;
        }
        case 0x19feaU: { // 6b0000f2 bmi.w $1a0de
            next = 0x19feeU;
            if ((r.status & 8U) != 0U) { next = 0x1a0deU; transfer_kind = 1U; }
            break;
        }
        case 0x19feeU: { // 2d7c0001a53c0002 move.l #$1a53c, $2(a6)
            next = 0x19ff6U;
            const auto value = 0x1a53cU;
            const auto destination_address = r.address[6] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x19ff6U: { // 3d7c04b00006 move.w #$4b0, $6(a6)
            next = 0x19ffcU;
            const auto value = 0x4b0U;
            const auto destination_address = r.address[6] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19ffcU: { // 3d7c0ca30008 move.w #$ca3, $8(a6)
            next = 0x1a002U;
            const auto value = 0xca3U;
            const auto destination_address = r.address[6] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a002U: { // 1d7c0007000b move.b #$7, $b(a6)
            next = 0x1a008U;
            const auto value = 0x7U;
            const auto destination_address = r.address[6] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a008U: { // 422e003f clr.b $3f(a6)
            next = 0x1a00cU;
            const auto destination_address = r.address[6] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1a00cU: { // 426e0064 clr.w $64(a6)
            next = 0x1a010U;
            const auto destination_address = r.address[6] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1a010U: { // 426e0060 clr.w $60(a6)
            next = 0x1a014U;
            const auto destination_address = r.address[6] + 0x60U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1a014U: { // 302d0012 move.w $12(a5), d0
            next = 0x1a018U;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a018U: { // 3d400012 move.w d0, $12(a6)
            next = 0x1a01cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a01cU: { // 3d40002a move.w d0, $2a(a6)
            next = 0x1a020U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a020U: { // 3d40002c move.w d0, $2c(a6)
            next = 0x1a024U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a024U: { // 303c0040 move.w #$40, d0
            next = 0x1a028U;
            const auto value = 0x40U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a028U: { // 3d400016 move.w d0, $16(a6)
            next = 0x1a02cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a02cU: { // 3d40002e move.w d0, $2e(a6)
            next = 0x1a030U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2eU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a030U: { // 3d400030 move.w d0, $30(a6)
            next = 0x1a034U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x30U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a034U: { // 302d001a move.w $1a(a5), d0
            next = 0x1a038U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a038U: { // 3d40001a move.w d0, $1a(a6)
            next = 0x1a03cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a03cU: { // 3d400032 move.w d0, $32(a6)
            next = 0x1a040U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x32U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a040U: { // 3d400034 move.w d0, $34(a6)
            next = 0x1a044U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x34U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a044U: { // 7202 moveq #$2, d1
            next = 0x1a046U;
            r.data[1] = 0x2U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1a046U: { // 49f81480 lea.l $1480.w, a4
            next = 0x1a04aU;
            const auto source_address = 0x1480U;
            r.address[4] = source_address;
            break;
        }
        case 0x1a04aU: { // 302d0052 move.w $52(a5), d0
            next = 0x1a04eU;
            const auto source_address = r.address[5] + 0x52U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a04eU: { // d07c0080 add.w #$80, d0
            next = 0x1a052U;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a052U: { // 0c400100 cmpi.w #$100, d0
            next = 0x1a056U;
            const auto source_value = 0x100U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a056U: { // 6f02 ble.b $1a05a
            next = 0x1a058U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1a05aU; transfer_kind = 1U; }
            break;
        }
        case 0x1a058U: { // 7000 moveq #$0, d0
            next = 0x1a05aU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
    default: return false;
    }
    return true;
}
