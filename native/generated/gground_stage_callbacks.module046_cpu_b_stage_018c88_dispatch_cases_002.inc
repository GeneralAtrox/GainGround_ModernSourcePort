
static bool cpu_b_stage_018c88_dispatch_cases_002([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x18da8U: { // 926d000c sub.w $c(a5), d1
            next = 0x18dacU;
            const auto source_address = r.address[5] + 0xcU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x18dacU: { // 9258 sub.w (a0)+, d1
            next = 0x18daeU;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x18daeU: { // 302d0012 move.w $12(a5), d0
            next = 0x18db2U;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18db2U: { // d058 add.w (a0)+, d0
            next = 0x18db4U;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18db4U: { // 906e0012 sub.w $12(a6), d0
            next = 0x18db8U;
            const auto source_address = r.address[6] + 0x12U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18db8U: { // 926e001a sub.w $1a(a6), d1
            next = 0x18dbcU;
            const auto source_address = r.address[6] + 0x1aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x18dbcU: { // 4440 neg.w d0
            next = 0x18dbeU;
            const auto old = r.data[0];
            const auto value = m.sub(0U, old, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18dbeU: { // 4441 neg.w d1
            next = 0x18dc0U;
            const auto old = r.data[1];
            const auto value = m.sub(0U, old, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x18dccU: { // 302d004e move.w $4e(a5), d0
            next = 0x18dd0U;
            const auto source_address = r.address[5] + 0x4eU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18dd0U: { // d27c0040 add.w #$40, d1
            next = 0x18dd4U;
            const auto source_value = 0x40U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x18dd4U: { // 024107ff andi.w #$7ff, d1
            next = 0x18dd8U;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[1];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x18dd8U: { // ee41 asr.w #$7, d1
            next = 0x18ddaU;
            m.shift_word(1U, 7U, false, true);
            break;
        }
        case 0x18ddaU: { // 5340 subq.w #$1, d0
            next = 0x18ddcU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18ddcU: { // d240 add.w d0, d1
            next = 0x18ddeU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x18ddeU: { // b27c000a cmp.w #$a, d1
            next = 0x18de2U;
            const auto source_value = 0xaU;
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18de2U: { // 6e0e bgt.b $18df2
            next = 0x18de4U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18df2U; transfer_kind = 1U; }
            break;
        }
        case 0x18de4U: { // 3b410056 move.w d1, $56(a5)
            next = 0x18de8U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x56U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18de8U: { // 41fa2294 lea.l $1b07e(pc), a0
            next = 0x18decU;
            const auto source_address = 0x1b07eU;
            r.address[0] = source_address;
            break;
        }
        case 0x18decU: { // b0301000 cmp.b (a0, d1.w), d0
            next = 0x18df0U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[1]);
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x18df0U: { // 6710 beq.b $18e02
            next = 0x18df2U;
            if ((r.status & 4U) != 0U) { next = 0x18e02U; transfer_kind = 1U; }
            break;
        }
        case 0x18df2U: { // 302d004e move.w $4e(a5), d0
            next = 0x18df6U;
            const auto source_address = r.address[5] + 0x4eU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18df6U: { // 3200 move.w d0, d1
            next = 0x18df8U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18df8U: { // d040 add.w d0, d0
            next = 0x18dfaU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18dfaU: { // d041 add.w d1, d0
            next = 0x18dfcU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18dfcU: { // 5340 subq.w #$1, d0
            next = 0x18dfeU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18dfeU: { // 3b400056 move.w d0, $56(a5)
            next = 0x18e02U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x56U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18e02U: { // 3b7c00030040 move.w #$3, $40(a5)
            next = 0x18e08U;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x40U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18e08U: { // 426d0066 clr.w $66(a5)
            next = 0x18e0cU;
            const auto destination_address = r.address[5] + 0x66U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x18e0cU: { // 3b7c00080068 move.w #$8, $68(a5)
            next = 0x18e12U;
            const auto value = 0x8U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18e12U: { // 3b7c00040060 move.w #$4, $60(a5)
            next = 0x18e18U;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x60U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18e1aU: { // 3b7c00030048 move.w #$3, $48(a5)
            next = 0x18e20U;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x48U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18e20U: { // 0c6d0002004e cmpi.w #$2, $4e(a5)
            next = 0x18e26U;
            const auto source_value = 0x2U;
            const auto destination_address = r.address[5] + 0x4eU;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
    default: return false;
    }
    return true;
}
