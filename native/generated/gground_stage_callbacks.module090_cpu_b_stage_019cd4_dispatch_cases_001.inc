
static bool cpu_b_stage_019cd4_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19d72U: { // 302d004a move.w $4a(a5), d0
            next = 0x19d76U;
            const auto source_address = r.address[5] + 0x4aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19d76U: { // d040 add.w d0, d0
            next = 0x19d78U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19d78U: { // 3200 move.w d0, d1
            next = 0x19d7aU;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19d7aU: { // e540 asl.w #$2, d0
            next = 0x19d7cU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x19d7cU: { // d041 add.w d1, d0
            next = 0x19d7eU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19d7eU: { // d0c0 adda.w d0, a0
            next = 0x19d80U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x19d80U: { // 3cbc8000 move.w #$8000, (a6)
            next = 0x19d84U;
            const auto value = 0x8000U;
            const auto destination_address = r.address[6];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19d84U: { // 2d7c0001a32c0002 move.l #$1a32c, $2(a6)
            next = 0x19d8cU;
            const auto value = 0x1a32cU;
            const auto destination_address = r.address[6] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x19d8cU: { // 3d7c04bd0006 move.w #$4bd, $6(a6)
            next = 0x19d92U;
            const auto value = 0x4bdU;
            const auto destination_address = r.address[6] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19d92U: { // 3d7c0c980008 move.w #$c98, $8(a6)
            next = 0x19d98U;
            const auto value = 0xc98U;
            const auto destination_address = r.address[6] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19d98U: { // 1d7c0006000b move.b #$6, $b(a6)
            next = 0x19d9eU;
            const auto value = 0x6U;
            const auto destination_address = r.address[6] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19d9eU: { // 422e003f clr.b $3f(a6)
            next = 0x19da2U;
            const auto destination_address = r.address[6] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x19da2U: { // 426e0064 clr.w $64(a6)
            next = 0x19da6U;
            const auto destination_address = r.address[6] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19da6U: { // 3d7c00500066 move.w #$50, $66(a6)
            next = 0x19dacU;
            const auto value = 0x50U;
            const auto destination_address = r.address[6] + 0x66U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19dacU: { // 302d0012 move.w $12(a5), d0
            next = 0x19db0U;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19db0U: { // d058 add.w (a0)+, d0
            next = 0x19db2U;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19db2U: { // 3d400012 move.w d0, $12(a6)
            next = 0x19db6U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19db6U: { // 3d7c00080016 move.w #$8, $16(a6)
            next = 0x19dbcU;
            const auto value = 0x8U;
            const auto destination_address = r.address[6] + 0x16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19dbcU: { // 3d7c0008002e move.w #$8, $2e(a6)
            next = 0x19dc2U;
            const auto value = 0x8U;
            const auto destination_address = r.address[6] + 0x2eU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19dc2U: { // 3d7c00080030 move.w #$8, $30(a6)
            next = 0x19dc8U;
            const auto value = 0x8U;
            const auto destination_address = r.address[6] + 0x30U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19dc8U: { // 302d001a move.w $1a(a5), d0
            next = 0x19dccU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19dccU: { // 0640000c addi.w #$c, d0
            next = 0x19dd0U;
            const auto source_value = 0xcU;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19dd0U: { // 3d40001a move.w d0, $1a(a6)
            next = 0x19dd4U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19dd4U: { // 2d58001e move.l (a0)+, $1e(a6)
            next = 0x19dd8U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6] + 0x1eU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x19dd8U: { // 2d580026 move.l (a0)+, $26(a6)
            next = 0x19ddcU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6] + 0x26U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x19ddcU: { // 526d004a addq.w #$1, $4a(a5)
            next = 0x19de0U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x4aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19de2U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x19de6U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19de6U: { // 6e10 bgt.b $19df8
            next = 0x19de8U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19df8U; transfer_kind = 1U; }
            break;
        }
        case 0x19de8U: { // 3b7c00040068 move.w #$4, $68(a5)
            next = 0x19deeU;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19deeU: { // 536d004a subq.w #$1, $4a(a5)
            next = 0x19df2U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x4aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19df2U: { // 6f06 ble.b $19dfa
            next = 0x19df4U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x19dfaU; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}
