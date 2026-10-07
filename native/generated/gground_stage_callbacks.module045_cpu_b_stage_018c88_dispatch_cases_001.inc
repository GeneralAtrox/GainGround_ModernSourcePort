
static bool cpu_b_stage_018c88_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x18d1cU: { // 7003 moveq #$3, d0
            next = 0x18d1eU;
            r.data[0] = 0x3U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x18d1eU: { // 927c0380 sub.w #$380, d1
            next = 0x18d22U;
            const auto source_value = 0x380U;
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x18d22U: { // 6b02 bmi.b $18d26
            next = 0x18d24U;
            if ((r.status & 8U) != 0U) { next = 0x18d26U; transfer_kind = 1U; }
            break;
        }
        case 0x18d24U: { // 7001 moveq #$1, d0
            next = 0x18d26U;
            r.data[0] = 0x1U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x18d26U: { // 3b40004e move.w d0, $4e(a5)
            next = 0x18d2aU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x4eU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18d2aU: { // 3b40005a move.w d0, $5a(a5)
            next = 0x18d2eU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18d2eU: { // 526d0048 addq.w #$1, $48(a5)
            next = 0x18d32U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x48U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18d32U: { // 3b7c00020040 move.w #$2, $40(a5)
            next = 0x18d38U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x40U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18d38U: { // 426d0066 clr.w $66(a5)
            next = 0x18d3cU;
            const auto destination_address = r.address[5] + 0x66U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x18d3cU: { // 3b7c00030068 move.w #$3, $68(a5)
            next = 0x18d42U;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18d42U: { // 3b7c0014004a move.w #$14, $4a(a5)
            next = 0x18d48U;
            const auto value = 0x14U;
            const auto destination_address = r.address[5] + 0x4aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18d4aU: { // 3b7c00010040 move.w #$1, $40(a5)
            next = 0x18d50U;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x40U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18d50U: { // 426d0066 clr.w $66(a5)
            next = 0x18d54U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x18d54U: { // 3b7c000a0068 move.w #$a, $68(a5)
            next = 0x18d5aU;
            const auto value = 0xaU;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18d5cU: { // 536d004a subq.w #$1, $4a(a5)
            next = 0x18d60U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x4aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18d60U: { // 6e10 bgt.b $18d72
            next = 0x18d62U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18d72U; transfer_kind = 1U; }
            break;
        }
        case 0x18d62U: { // 3b7c00020048 move.w #$2, $48(a5)
            next = 0x18d68U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x48U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18d68U: { // 426d0040 clr.w $40(a5)
            next = 0x18d6cU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x18d6cU: { // 3b7c00030054 move.w #$3, $54(a5)
            next = 0x18d72U;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18d74U: { // 4a6d0040 tst.w $40(a5)
            next = 0x18d78U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x18d78U: { // 660000ce bne.w $18e48
            next = 0x18d7cU;
            if ((r.status & 4U) == 0U) { next = 0x18e48U; transfer_kind = 1U; }
            break;
        }
        case 0x18d7cU: { // 536d0054 subq.w #$1, $54(a5)
            next = 0x18d80U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x54U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18d80U: { // 6b000098 bmi.w $18e1a
            next = 0x18d84U;
            if ((r.status & 8U) != 0U) { next = 0x18e1aU; transfer_kind = 1U; }
            break;
        }
        case 0x18d84U: { // 302d0052 move.w $52(a5), d0
            next = 0x18d88U;
            const auto source_address = r.address[5] + 0x52U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18d88U: { // 4df81480 lea.l $1480.w, a6
            next = 0x18d8cU;
            const auto source_address = 0x1480U;
            r.address[6] = source_address;
            break;
        }
        case 0x18d8cU: { // dcc0 adda.w d0, a6
            next = 0x18d8eU;
            const auto source_value = r.data[0];
            r.address[6] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x18d8eU: { // 0c6e00050044 cmpi.w #$5, $44(a6)
            next = 0x18d94U;
            const auto source_value = 0x5U;
            const auto destination_address = r.address[6] + 0x44U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18d94U: { // 665c bne.b $18df2
            next = 0x18d96U;
            if ((r.status & 4U) == 0U) { next = 0x18df2U; transfer_kind = 1U; }
            break;
        }
        case 0x18d96U: { // 41fa22da lea.l $1b072(pc), a0
            next = 0x18d9aU;
            const auto source_address = 0x1b072U;
            r.address[0] = source_address;
            break;
        }
        case 0x18d9aU: { // 302d004e move.w $4e(a5), d0
            next = 0x18d9eU;
            const auto source_address = r.address[5] + 0x4eU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18d9eU: { // 5340 subq.w #$1, d0
            next = 0x18da0U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18da0U: { // e540 asl.w #$2, d0
            next = 0x18da2U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x18da2U: { // d0c0 adda.w d0, a0
            next = 0x18da4U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x18da4U: { // 323c0200 move.w #$200, d1
            next = 0x18da8U;
            const auto value = 0x200U;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
