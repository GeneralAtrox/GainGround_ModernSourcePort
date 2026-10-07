
static bool cpu_b_stage_0123ea_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x123eaU:
    case 0x123eeU:
    case 0x123f2U:
    case 0x123f6U:
    case 0x123faU:
    case 0x123feU:
    case 0x12404U:
    case 0x12406U:
    case 0x1240cU:
    case 0x1240eU:
    case 0x12414U:
    case 0x12418U:
    case 0x1241cU:
    case 0x12422U:
    case 0x12426U:
    case 0x1242aU:
    case 0x1242eU:
    case 0x12430U:
    case 0x12434U:
    case 0x12438U:
    case 0x1243aU:
    case 0x1243eU:
    case 0x12442U:
    case 0x12444U:
    case 0x12446U:
    case 0x12448U:
    case 0x1244aU:
    case 0x1244cU:
    case 0x12450U:
    case 0x12454U:
    case 0x12456U:
    case 0x1245aU:
    case 0x1245eU:
    case 0x12462U:
    case 0x12466U:
    case 0x1246aU:
    case 0x1246eU:
    case 0x12474U:
    case 0x1247aU:
    case 0x12480U:
    case 0x12486U:
    case 0x12488U:
    case 0x1248cU:
    case 0x12492U:
    case 0x12498U:
    case 0x1249aU:
        return true;
    default: return false;
    }
}

static bool cpu_b_stage_0123ea_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x123eeU: { // 65000098 bcs.w $12488
            next = 0x123f2U;
            if ((r.status & 1U) != 0U) { next = 0x12488U; transfer_kind = 1U; }
            break;
        }
        case 0x123f6U: { // 65000090 bcs.w $12488
            next = 0x123faU;
            if ((r.status & 1U) != 0U) { next = 0x12488U; transfer_kind = 1U; }
            break;
        }
        case 0x123faU: { // 3c6d0036 movea.w $36(a5), a6
            next = 0x123feU;
            const auto source_address = r.address[5] + 0x36U;
            const auto value = m.word(source_address);
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x123feU: { // 0c6e00040044 cmpi.w #$4, $44(a6)
            next = 0x12404U;
            const auto source_value = 0x4U;
            const auto destination_address = r.address[6] + 0x44U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x12404U: { // 6708 beq.b $1240e
            next = 0x12406U;
            if ((r.status & 4U) != 0U) { next = 0x1240eU; transfer_kind = 1U; }
            break;
        }
        case 0x12406U: { // 0c6e00050044 cmpi.w #$5, $44(a6)
            next = 0x1240cU;
            const auto source_value = 0x5U;
            const auto destination_address = r.address[6] + 0x44U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1240cU: { // 667a bne.b $12488
            next = 0x1240eU;
            if ((r.status & 4U) == 0U) { next = 0x12488U; transfer_kind = 1U; }
            break;
        }
        case 0x1240eU: { // 066d00100058 addi.w #$10, $58(a5)
            next = 0x12414U;
            const auto source_value = 0x10U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x12414U: { // 322d0058 move.w $58(a5), d1
            next = 0x12418U;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12418U: { // 024107ff andi.w #$7ff, d1
            next = 0x1241cU;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[1];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x12422U: { // 586d0046 addq.w #$4, $46(a5)
            next = 0x12426U;
            const auto source_value = 0x4U;
            const auto destination_address = r.address[5] + 0x46U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x12426U: { // 342d0046 move.w $46(a5), d2
            next = 0x1242aU;
            const auto source_address = r.address[5] + 0x46U;
            const auto value = m.word(source_address);
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1242aU: { // 0c42007e cmpi.w #$7e, d2
            next = 0x1242eU;
            const auto source_value = 0x7eU;
            const auto destination_value = r.data[2];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1242eU: { // 6504 bcs.b $12434
            next = 0x12430U;
            if ((r.status & 1U) != 0U) { next = 0x12434U; transfer_kind = 1U; }
            break;
        }
        case 0x12430U: { // 343c007e move.w #$7e, d2
            next = 0x12434U;
            const auto value = 0x7eU;
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12434U: { // 3c6d0036 movea.w $36(a5), a6
            next = 0x12438U;
            const auto source_address = r.address[5] + 0x36U;
            const auto value = m.word(source_address);
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x12438U: { // c1c2 muls.w d2, d0
            next = 0x1243aU;
            const auto source = static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[2]));
            const auto destination = static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[0]));
            r.data[0] = static_cast<std::uint32_t>(source * destination);
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1243aU: { // d0ae0012 add.l $12(a6), d0
            next = 0x1243eU;
            const auto source_address = r.address[6] + 0x12U;
            const auto source_value = m.lng(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 32U);
            r.data[0] = value;
            break;
        }
        case 0x1243eU: { // 2b400012 move.l d0, $12(a5)
            next = 0x12442U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x12U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x12442U: { // e441 asr.w #$2, d1
            next = 0x12444U;
            m.shift_word(1U, 2U, false, true);
            break;
        }
        case 0x12444U: { // 3001 move.w d1, d0
            next = 0x12446U;
            const auto value = r.data[1];
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12446U: { // d241 add.w d1, d1
            next = 0x12448U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x12448U: { // d240 add.w d0, d1
            next = 0x1244aU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1244aU: { // c3c2 muls.w d2, d1
            next = 0x1244cU;
            const auto source = static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[2]));
            const auto destination = static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[1]));
            r.data[1] = static_cast<std::uint32_t>(source * destination);
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1244cU: { // d2ae001a add.l $1a(a6), d1
            next = 0x12450U;
            const auto source_address = r.address[6] + 0x1aU;
            const auto source_value = m.lng(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 32U);
            r.data[1] = value;
            break;
        }
        case 0x12450U: { // 2b41001a move.l d1, $1a(a5)
            next = 0x12454U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x1aU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x12454U: { // e242 asr.w #$1, d2
            next = 0x12456U;
            m.shift_word(2U, 1U, false, true);
            break;
        }
        case 0x12456U: { // 1b420010 move.b d2, $10(a5)
            next = 0x1245aU;
            const auto value = r.data[2];
            const auto destination_address = r.address[5] + 0x10U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1245aU: { // 1b420011 move.b d2, $11(a5)
            next = 0x1245eU;
            const auto value = r.data[2];
            const auto destination_address = r.address[5] + 0x11U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1245eU: { // 522d003d addq.b #$1, $3d(a5)
            next = 0x12462U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x3dU;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x12462U: { // 102d003d move.b $3d(a5), d0
            next = 0x12466U;
            const auto source_address = r.address[5] + 0x3dU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x12466U: { // 02400003 andi.w #$3, d0
            next = 0x1246aU;
            const auto source_value = 0x3U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1246aU: { // 41fa0a82 lea.l $12eee(pc), a0
            next = 0x1246eU;
            const auto source_address = 0x12eeeU;
            r.address[0] = source_address;
            break;
        }
    default: return false;
    }
    return true;
}
