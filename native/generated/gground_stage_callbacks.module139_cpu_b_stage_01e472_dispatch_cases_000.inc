
static bool cpu_b_stage_01e472_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1e472U: { // 4a2d003e tst.b $3e(a5)
            next = 0x1e476U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1e476U: { // 6716 beq.b $1e48e
            next = 0x1e478U;
            if ((r.status & 4U) != 0U) { next = 0x1e48eU; transfer_kind = 1U; }
            break;
        }
        case 0x1e478U: { // 342d0056 move.w $56(a5), d2
            next = 0x1e47cU;
            const auto source_address = r.address[5] + 0x56U;
            const auto value = m.word(source_address);
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e47cU: { // d47c0080 add.w #$80, d2
            next = 0x1e480U;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e480U: { // 024207ff andi.w #$7ff, d2
            next = 0x1e484U;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[2];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e484U: { // 3b420056 move.w d2, $56(a5)
            next = 0x1e488U;
            const auto value = r.data[2];
            const auto destination_address = r.address[5] + 0x56U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e488U: { // 02420600 andi.w #$600, d2
            next = 0x1e48cU;
            const auto source_value = 0x600U;
            const auto destination_value = r.data[2];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e48cU: { // 6024 bra.b $1e4b2
            next = 0x1e48eU;
            if (true) { next = 0x1e4b2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e48eU: { // 7400 moveq #$0, d2
            next = 0x1e490U;
            r.data[2] = 0x0U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0x1e490U: { // 142d0036 move.b $36(a5), d2
            next = 0x1e494U;
            const auto source_address = r.address[5] + 0x36U;
            const auto value = m.byte(source_address);
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1e494U: { // e942 asl.w #$4, d2
            next = 0x1e496U;
            m.asl_word(2U, 4U);
            break;
        }
        case 0x1e496U: { // d46d0056 add.w $56(a5), d2
            next = 0x1e49aU;
            const auto source_address = r.address[5] + 0x56U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e49aU: { // 024207ff andi.w #$7ff, d2
            next = 0x1e49eU;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[2];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e49eU: { // 3b420056 move.w d2, $56(a5)
            next = 0x1e4a2U;
            const auto value = r.data[2];
            const auto destination_address = r.address[5] + 0x56U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e4a2U: { // 02420600 andi.w #$600, d2
            next = 0x1e4a6U;
            const auto source_value = 0x600U;
            const auto destination_value = r.data[2];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e4a6U: { // 082d00050041 btst.b #$5, $41(a5)
            next = 0x1e4acU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e4acU: { // 6704 beq.b $1e4b2
            next = 0x1e4aeU;
            if ((r.status & 4U) != 0U) { next = 0x1e4b2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e4aeU: { // d47c0800 add.w #$800, d2
            next = 0x1e4b2U;
            const auto source_value = 0x800U;
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e4b6U: { // 7200 moveq #$0, d1
            next = 0x1e4b8U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1e4b8U: { // ef5a rol.w #$7, d2
            next = 0x1e4baU;
            const auto old = r.data[2] & 0xffffU;
            const auto value = ((old << 7U) | (old >> 9U)) & 0xffffU;
            m.dw(2U, value);
            m.logic(value, 16U);
            r.status = static_cast<std::uint16_t>(r.status | (value & 1U));
            break;
        }
        case 0x1e4baU: { // 5342 subq.w #$1, d2
            next = 0x1e4bcU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[2];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e4bcU: { // 6b06 bmi.b $1e4c4
            next = 0x1e4beU;
            if ((r.status & 8U) != 0U) { next = 0x1e4c4U; transfer_kind = 1U; }
            break;
        }
        case 0x1e4beU: { // d247 add.w d7, d1
            next = 0x1e4c0U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e4c0U: { // 51cafffc dbra d2, $1e4be
            next = 0x1e4c4U;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0x1e4beU;
            break;
        }
        case 0x1e4c4U: { // 102d0058 move.b $58(a5), d0
            next = 0x1e4c8U;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1e4c8U: { // e208 lsr.b #$1, d0
            next = 0x1e4caU;
            const auto old = r.data[0] & 0xffU;
            const auto carry = (old >> 0U) & 1U;
            const auto value = old >> 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            r.status = static_cast<std::uint16_t>((r.status & ~0x11U) | (carry ? 0x11U : 0U));
            break;
        }
        case 0x1e4caU: { // 1b400010 move.b d0, $10(a5)
            next = 0x1e4ceU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x10U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1e4ceU: { // 1b400011 move.b d0, $11(a5)
            next = 0x1e4d2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x11U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1e4d2U: { // 08ed00020001 bset.b #$2, $1(a5)
            next = 0x1e4d8U;
            const auto destination_address = r.address[5] + 0x1U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1e4d8U: { // 206d0048 movea.l $48(a5), a0
            next = 0x1e4dcU;
            const auto source_address = r.address[5] + 0x48U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1e4dcU: { // d0c1 adda.w d1, a0
            next = 0x1e4deU;
            const auto source_value = r.data[1];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1e4deU: { // 2b48004c move.l a0, $4c(a5)
            next = 0x1e4e2U;
            const auto value = r.address[0];
            const auto destination_address = r.address[5] + 0x4cU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1e4e2U: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x1e4e6U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
