
static bool cpu_b_stage_018f72_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x18f72U:
    case 0x18f74U:
    case 0x18f78U:
    case 0x18f7cU:
    case 0x18f80U:
    case 0x18f84U:
    case 0x18f8aU:
    case 0x18f8eU:
    case 0x18f90U:
    case 0x18f94U:
    case 0x18f96U:
    case 0x18f9aU:
    case 0x18f9eU:
    case 0x18fa2U:
    case 0x18fa6U:
    case 0x18fa8U:
    case 0x18faeU:
    case 0x18fb6U:
    case 0x18fb8U:
    case 0x18fbaU:
    case 0x18fbcU:
    case 0x18fc0U:
    case 0x18fc2U:
    case 0x18fc6U:
    case 0x18fcaU:
    case 0x18fd0U:
    case 0x18fd4U:
    case 0x18fd6U:
    case 0x18fdaU:
    case 0x18fdeU:
    case 0x18fe0U:
    case 0x18fe4U:
    case 0x18fe8U:
    case 0x18fecU:
    case 0x18ff0U:
    case 0x18ff4U:
    case 0x18ff8U:
    case 0x18ffaU:
    case 0x18ffeU:
    case 0x19002U:
    case 0x19006U:
    case 0x1900aU:
    case 0x1900eU:
    case 0x19012U:
    case 0x19016U:
    case 0x1901cU:
    case 0x19020U:
    case 0x19026U:
    case 0x1902cU:
        return true;
    default: return false;
    }
}

static bool cpu_b_stage_018f72_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x18f72U: { // 7200 moveq #$0, d1
            next = 0x18f74U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x18f74U: { // 122d0051 move.b $51(a5), d1
            next = 0x18f78U;
            const auto source_address = r.address[5] + 0x51U;
            const auto value = m.byte(source_address);
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18f78U: { // 6f0000b2 ble.w $1902c
            next = 0x18f7cU;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1902cU; transfer_kind = 1U; }
            break;
        }
        case 0x18f7cU: { // 532d0050 subq.b #$1, $50(a5)
            next = 0x18f80U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x50U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x18f80U: { // 6e0000aa bgt.w $1902c
            next = 0x18f84U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1902cU; transfer_kind = 1U; }
            break;
        }
        case 0x18f84U: { // 1b7c00020050 move.b #$2, $50(a5)
            next = 0x18f8aU;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x50U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18f8eU: { // 4a40 tst.w d0
            next = 0x18f90U;
            const auto value = r.data[0];
            m.logic(value, 16U);
            break;
        }
        case 0x18f90U: { // 6b00009a bmi.w $1902c
            next = 0x18f94U;
            if ((r.status & 8U) != 0U) { next = 0x1902cU; transfer_kind = 1U; }
            break;
        }
        case 0x18f94U: { // 5341 subq.w #$1, d1
            next = 0x18f96U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x18f96U: { // 1b410051 move.b d1, $51(a5)
            next = 0x18f9aU;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x51U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18f9aU: { // 302d0056 move.w $56(a5), d0
            next = 0x18f9eU;
            const auto source_address = r.address[5] + 0x56U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18f9eU: { // 3cbc8000 move.w #$8000, (a6)
            next = 0x18fa2U;
            const auto value = 0x8000U;
            const auto destination_address = r.address[6];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18fa2U: { // 0c400004 cmpi.w #$4, d0
            next = 0x18fa6U;
            const auto source_value = 0x4U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18fa6U: { // 6e06 bgt.b $18fae
            next = 0x18fa8U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18faeU; transfer_kind = 1U; }
            break;
        }
        case 0x18fa8U: { // 1d7c00020001 move.b #$2, $1(a6)
            next = 0x18faeU;
            const auto value = 0x2U;
            const auto destination_address = r.address[6] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18faeU: { // 2d7c000194400002 move.l #$19440, $2(a6)
            next = 0x18fb6U;
            const auto value = 0x19440U;
            const auto destination_address = r.address[6] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x18fb6U: { // eb40 asl.w #$5, d0
            next = 0x18fb8U;
            m.asl_word(0U, 5U);
            break;
        }
        case 0x18fb8U: { // e741 asl.w #$3, d1
            next = 0x18fbaU;
            m.asl_word(1U, 3U);
            break;
        }
        case 0x18fbaU: { // d041 add.w d1, d0
            next = 0x18fbcU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18fbcU: { // 41fa2132 lea.l $1b0f0(pc), a0
            next = 0x18fc0U;
            const auto source_address = 0x1b0f0U;
            r.address[0] = source_address;
            break;
        }
        case 0x18fc0U: { // d0c0 adda.w d0, a0
            next = 0x18fc2U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x18fc2U: { // 3d580006 move.w (a0)+, $6(a6)
            next = 0x18fc6U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[6] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18fc6U: { // 3d580008 move.w (a0)+, $8(a6)
            next = 0x18fcaU;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[6] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18fcaU: { // 1d7c0007000b move.b #$7, $b(a6)
            next = 0x18fd0U;
            const auto value = 0x7U;
            const auto destination_address = r.address[6] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18fd0U: { // 302d000c move.w $c(a5), d0
            next = 0x18fd4U;
            const auto source_address = r.address[5] + 0xcU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18fd4U: { // d058 add.w (a0)+, d0
            next = 0x18fd6U;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18fd6U: { // 3d40000c move.w d0, $c(a6)
            next = 0x18fdaU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0xcU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18fdaU: { // 907c0200 sub.w #$200, d0
            next = 0x18fdeU;
            const auto source_value = 0x200U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18fdeU: { // 4440 neg.w d0
            next = 0x18fe0U;
            const auto old = r.data[0];
            const auto value = m.sub(0U, old, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18fe0U: { // 3d40001a move.w d0, $1a(a6)
            next = 0x18fe4U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18fe4U: { // 907c0010 sub.w #$10, d0
            next = 0x18fe8U;
            const auto source_value = 0x10U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18fe8U: { // 3d400032 move.w d0, $32(a6)
            next = 0x18fecU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x32U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18fecU: { // d07c0020 add.w #$20, d0
            next = 0x18ff0U;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
    default: return false;
    }
    return true;
}
