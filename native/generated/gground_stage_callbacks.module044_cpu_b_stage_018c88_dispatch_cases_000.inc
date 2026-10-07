
static bool cpu_b_stage_018c88_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x18c88U: { // 4a2d003f tst.b $3f(a5)
            next = 0x18c8cU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x18c8cU: { // 66000452 bne.w $190e0
            next = 0x18c90U;
            if ((r.status & 4U) == 0U) { next = 0x190e0U; transfer_kind = 1U; }
            break;
        }
        case 0x18c90U: { // 302d0048 move.w $48(a5), d0
            next = 0x18c94U;
            const auto source_address = r.address[5] + 0x48U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18c94U: { // e540 asl.w #$2, d0
            next = 0x18c96U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x18c96U: { // 4efb0002 jmp $18c9a(pc, d0.w)
            next = 0x18c9aU;
            next = (0x18c9aU + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x18c9aU: { // 60000016 bra.w $18cb2
            next = 0x18c9eU;
            if (true) { next = 0x18cb2U; transfer_kind = 1U; }
            break;
        }
        case 0x18c9eU: { // 600000bc bra.w $18d5c
            next = 0x18ca2U;
            if (true) { next = 0x18d5cU; transfer_kind = 1U; }
            break;
        }
        case 0x18ca2U: { // 600000d0 bra.w $18d74
            next = 0x18ca6U;
            if (true) { next = 0x18d74U; transfer_kind = 1U; }
            break;
        }
        case 0x18ca6U: { // 600001a2 bra.w $18e4a
            next = 0x18caaU;
            if (true) { next = 0x18e4aU; transfer_kind = 1U; }
            break;
        }
        case 0x18caaU: { // 600001d2 bra.w $18e7e
            next = 0x18caeU;
            if (true) { next = 0x18e7eU; transfer_kind = 1U; }
            break;
        }
        case 0x18caeU: { // 600001f6 bra.w $18ea6
            next = 0x18cb2U;
            if (true) { next = 0x18ea6U; transfer_kind = 1U; }
            break;
        }
        case 0x18cb2U: { // 4a6d0040 tst.w $40(a5)
            next = 0x18cb6U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x18cb6U: { // 66000090 bne.w $18d48
            next = 0x18cbaU;
            if ((r.status & 4U) == 0U) { next = 0x18d48U; transfer_kind = 1U; }
            break;
        }
        case 0x18cbaU: { // 536d004c subq.w #$1, $4c(a5)
            next = 0x18cbeU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x4cU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18cbeU: { // 6e000088 bgt.w $18d48
            next = 0x18cc2U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18d48U; transfer_kind = 1U; }
            break;
        }
        case 0x18cc2U: { // 3b7c000a004c move.w #$a, $4c(a5)
            next = 0x18cc8U;
            const auto value = 0xaU;
            const auto destination_address = r.address[5] + 0x4cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18cc8U: { // 302d0052 move.w $52(a5), d0
            next = 0x18cccU;
            const auto source_address = r.address[5] + 0x52U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18cccU: { // d07c0080 add.w #$80, d0
            next = 0x18cd0U;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18cd0U: { // 0c400100 cmpi.w #$100, d0
            next = 0x18cd4U;
            const auto source_value = 0x100U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18cd4U: { // 6f02 ble.b $18cd8
            next = 0x18cd6U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x18cd8U; transfer_kind = 1U; }
            break;
        }
        case 0x18cd6U: { // 7000 moveq #$0, d0
            next = 0x18cd8U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x18cd8U: { // 3b400052 move.w d0, $52(a5)
            next = 0x18cdcU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x52U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18cdcU: { // 4df81480 lea.l $1480.w, a6
            next = 0x18ce0U;
            const auto source_address = 0x1480U;
            r.address[6] = source_address;
            break;
        }
        case 0x18ce0U: { // dcc0 adda.w d0, a6
            next = 0x18ce2U;
            const auto source_value = r.data[0];
            r.address[6] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x18ce2U: { // 0c6e00050044 cmpi.w #$5, $44(a6)
            next = 0x18ce8U;
            const auto source_value = 0x5U;
            const auto destination_address = r.address[6] + 0x44U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18ce8U: { // 6660 bne.b $18d4a
            next = 0x18ceaU;
            if ((r.status & 4U) == 0U) { next = 0x18d4aU; transfer_kind = 1U; }
            break;
        }
        case 0x18ceaU: { // 322e001a move.w $1a(a6), d1
            next = 0x18ceeU;
            const auto source_address = r.address[6] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18ceeU: { // 0c410170 cmpi.w #$170, d1
            next = 0x18cf2U;
            const auto source_value = 0x170U;
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18cf2U: { // 6e56 bgt.b $18d4a
            next = 0x18cf4U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18d4aU; transfer_kind = 1U; }
            break;
        }
        case 0x18cf4U: { // 302e0012 move.w $12(a6), d0
            next = 0x18cf8U;
            const auto source_address = r.address[6] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18cf8U: { // 906d0012 sub.w $12(a5), d0
            next = 0x18cfcU;
            const auto source_address = r.address[5] + 0x12U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18cfcU: { // 926d001a sub.w $1a(a5), d1
            next = 0x18d00U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x18d0cU: { // 7001 moveq #$1, d0
            next = 0x18d0eU;
            r.data[0] = 0x1U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x18d0eU: { // 927c0180 sub.w #$180, d1
            next = 0x18d12U;
            const auto source_value = 0x180U;
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x18d12U: { // 6b12 bmi.b $18d26
            next = 0x18d14U;
            if ((r.status & 8U) != 0U) { next = 0x18d26U; transfer_kind = 1U; }
            break;
        }
        case 0x18d14U: { // 7002 moveq #$2, d0
            next = 0x18d16U;
            r.data[0] = 0x2U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x18d16U: { // 927c0100 sub.w #$100, d1
            next = 0x18d1aU;
            const auto source_value = 0x100U;
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x18d1aU: { // 6b0a bmi.b $18d26
            next = 0x18d1cU;
            if ((r.status & 8U) != 0U) { next = 0x18d26U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}
