
static bool cpu_b_stage_018c88_dispatch_cases_003([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x18e26U: { // 6604 bne.b $18e2c
            next = 0x18e28U;
            if ((r.status & 4U) == 0U) { next = 0x18e2cU; transfer_kind = 1U; }
            break;
        }
        case 0x18e28U: { // 546d0048 addq.w #$2, $48(a5)
            next = 0x18e2cU;
            const auto source_value = 0x2U;
            const auto destination_address = r.address[5] + 0x48U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18e2cU: { // 3b7c0002005a move.w #$2, $5a(a5)
            next = 0x18e32U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18e32U: { // 3b7c00020040 move.w #$2, $40(a5)
            next = 0x18e38U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x40U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18e38U: { // 426d0066 clr.w $66(a5)
            next = 0x18e3cU;
            const auto destination_address = r.address[5] + 0x66U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x18e3cU: { // 3b7c00030068 move.w #$3, $68(a5)
            next = 0x18e42U;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18e42U: { // 3b7c0014004a move.w #$14, $4a(a5)
            next = 0x18e48U;
            const auto value = 0x14U;
            const auto destination_address = r.address[5] + 0x4aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18e4aU: { // 536d004a subq.w #$1, $4a(a5)
            next = 0x18e4eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x4aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18e4eU: { // 6f0e ble.b $18e5e
            next = 0x18e50U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x18e5eU; transfer_kind = 1U; }
            break;
        }
        case 0x18e50U: { // 302d001a move.w $1a(a5), d0
            next = 0x18e54U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18e54U: { // 04400078 subi.w #$78, d0
            next = 0x18e58U;
            const auto source_value = 0x78U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18e58U: { // 0c400010 cmpi.w #$10, d0
            next = 0x18e5cU;
            const auto source_value = 0x10U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18e5cU: { // 621e bhi.b $18e7c
            next = 0x18e5eU;
            if ((r.status & 5U) == 0U) { next = 0x18e7cU; transfer_kind = 1U; }
            break;
        }
        case 0x18e5eU: { // 3b7c0001005a move.w #$1, $5a(a5)
            next = 0x18e64U;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18e64U: { // 0c6d0003004e cmpi.w #$3, $4e(a5)
            next = 0x18e6aU;
            const auto source_value = 0x3U;
            const auto destination_address = r.address[5] + 0x4eU;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18e6aU: { // 6704 beq.b $18e70
            next = 0x18e6cU;
            if ((r.status & 4U) != 0U) { next = 0x18e70U; transfer_kind = 1U; }
            break;
        }
        case 0x18e6cU: { // 546d005a addq.w #$2, $5a(a5)
            next = 0x18e70U;
            const auto source_value = 0x2U;
            const auto destination_address = r.address[5] + 0x5aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18e70U: { // 3b7c00040048 move.w #$4, $48(a5)
            next = 0x18e76U;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x48U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18e76U: { // 3b7c0014004a move.w #$14, $4a(a5)
            next = 0x18e7cU;
            const auto value = 0x14U;
            const auto destination_address = r.address[5] + 0x4aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18e7eU: { // 536d004a subq.w #$1, $4a(a5)
            next = 0x18e82U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x4aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18e82U: { // 6f0e ble.b $18e92
            next = 0x18e84U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x18e92U; transfer_kind = 1U; }
            break;
        }
        case 0x18e84U: { // 302d0012 move.w $12(a5), d0
            next = 0x18e88U;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18e88U: { // 044000b8 subi.w #$b8, d0
            next = 0x18e8cU;
            const auto source_value = 0xb8U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18e8cU: { // 0c400010 cmpi.w #$10, d0
            next = 0x18e90U;
            const auto source_value = 0x10U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18e90U: { // 6212 bhi.b $18ea4
            next = 0x18e92U;
            if ((r.status & 5U) == 0U) { next = 0x18ea4U; transfer_kind = 1U; }
            break;
        }
        case 0x18e92U: { // 3b7c0002005a move.w #$2, $5a(a5)
            next = 0x18e98U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18e98U: { // 3b7c00050048 move.w #$5, $48(a5)
            next = 0x18e9eU;
            const auto value = 0x5U;
            const auto destination_address = r.address[5] + 0x48U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18e9eU: { // 3b7c0014004a move.w #$14, $4a(a5)
            next = 0x18ea4U;
            const auto value = 0x14U;
            const auto destination_address = r.address[5] + 0x4aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18ea6U: { // 536d004a subq.w #$1, $4a(a5)
            next = 0x18eaaU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x4aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18eaaU: { // 6f0e ble.b $18eba
            next = 0x18eacU;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x18ebaU; transfer_kind = 1U; }
            break;
        }
        case 0x18eacU: { // 302d001a move.w $1a(a5), d0
            next = 0x18eb0U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18eb0U: { // 04400078 subi.w #$78, d0
            next = 0x18eb4U;
            const auto source_value = 0x78U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18eb4U: { // 0c400010 cmpi.w #$10, d0
            next = 0x18eb8U;
            const auto source_value = 0x10U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
    default: return false;
    }
    return true;
}
