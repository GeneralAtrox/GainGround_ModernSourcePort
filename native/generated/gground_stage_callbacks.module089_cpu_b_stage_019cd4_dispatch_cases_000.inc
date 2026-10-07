
static bool cpu_b_stage_019cd4_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19cd4U: { // 302d0048 move.w $48(a5), d0
            next = 0x19cd8U;
            const auto source_address = r.address[5] + 0x48U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19cd8U: { // e540 asl.w #$2, d0
            next = 0x19cdaU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x19cdaU: { // 4efb0002 jmp $19cde(pc, d0.w)
            next = 0x19cdeU;
            next = (0x19cdeU + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x19cdeU: { // 60000012 bra.w $19cf2
            next = 0x19ce2U;
            if (true) { next = 0x19cf2U; transfer_kind = 1U; }
            break;
        }
        case 0x19ce2U: { // 6000003c bra.w $19d20
            next = 0x19ce6U;
            if (true) { next = 0x19d20U; transfer_kind = 1U; }
            break;
        }
        case 0x19ce6U: { // 600000fa bra.w $19de2
            next = 0x19ceaU;
            if (true) { next = 0x19de2U; transfer_kind = 1U; }
            break;
        }
        case 0x19ceaU: { // 60000120 bra.w $19e0c
            next = 0x19ceeU;
            if (true) { next = 0x19e0cU; transfer_kind = 1U; }
            break;
        }
        case 0x19ceeU: { // 6000024c bra.w $19f3c
            next = 0x19cf2U;
            if (true) { next = 0x19f3cU; transfer_kind = 1U; }
            break;
        }
        case 0x19cf2U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x19cf6U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19cf6U: { // 6e10 bgt.b $19d08
            next = 0x19cf8U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19d08U; transfer_kind = 1U; }
            break;
        }
        case 0x19cf8U: { // 3b7c00040068 move.w #$4, $68(a5)
            next = 0x19cfeU;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19cfeU: { // 536d004a subq.w #$1, $4a(a5)
            next = 0x19d02U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x4aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19d02U: { // 6f06 ble.b $19d0a
            next = 0x19d04U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x19d0aU; transfer_kind = 1U; }
            break;
        }
        case 0x19d04U: { // 526d0064 addq.w #$1, $64(a5)
            next = 0x19d08U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19d0aU: { // 3b7c00010048 move.w #$1, $48(a5)
            next = 0x19d10U;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x48U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19d10U: { // 426d0064 clr.w $64(a5)
            next = 0x19d14U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19d14U: { // 426d004a clr.w $4a(a5)
            next = 0x19d18U;
            const auto destination_address = r.address[5] + 0x4aU;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19d18U: { // 3b7c00080068 move.w #$8, $68(a5)
            next = 0x19d1eU;
            const auto value = 0x8U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19d20U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x19d24U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19d24U: { // 6e12 bgt.b $19d38
            next = 0x19d26U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19d38U; transfer_kind = 1U; }
            break;
        }
        case 0x19d26U: { // 526d0064 addq.w #$1, $64(a5)
            next = 0x19d2aU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19d2aU: { // 0c6d00040064 cmpi.w #$4, $64(a5)
            next = 0x19d30U;
            const auto source_value = 0x4U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19d30U: { // 6408 bcc.b $19d3a
            next = 0x19d32U;
            if ((r.status & 1U) == 0U) { next = 0x19d3aU; transfer_kind = 1U; }
            break;
        }
        case 0x19d32U: { // 3b7c00080068 move.w #$8, $68(a5)
            next = 0x19d38U;
            const auto value = 0x8U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19d3aU: { // 0c6d0014004a cmpi.w #$14, $4a(a5)
            next = 0x19d40U;
            const auto source_value = 0x14U;
            const auto destination_address = r.address[5] + 0x4aU;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19d40U: { // 6b18 bmi.b $19d5a
            next = 0x19d42U;
            if ((r.status & 8U) != 0U) { next = 0x19d5aU; transfer_kind = 1U; }
            break;
        }
        case 0x19d42U: { // 3b7c00020048 move.w #$2, $48(a5)
            next = 0x19d48U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x48U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19d48U: { // 3b7c0010004a move.w #$10, $4a(a5)
            next = 0x19d4eU;
            const auto value = 0x10U;
            const auto destination_address = r.address[5] + 0x4aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19d4eU: { // 426d0064 clr.w $64(a5)
            next = 0x19d52U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19d52U: { // 3b7c00040068 move.w #$4, $68(a5)
            next = 0x19d58U;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19d5aU: { // 3b7c00020068 move.w #$2, $68(a5)
            next = 0x19d60U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19d60U: { // 006d00040064 ori.w #$4, $64(a5)
            next = 0x19d66U;
            const auto source_value = 0x4U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = destination_value | source_value;
            m.logic(value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19d6aU: { // 4a40 tst.w d0
            next = 0x19d6cU;
            const auto value = r.data[0];
            m.logic(value, 16U);
            break;
        }
        case 0x19d6cU: { // 6bca bmi.b $19d38
            next = 0x19d6eU;
            if ((r.status & 8U) != 0U) { next = 0x19d38U; transfer_kind = 1U; }
            break;
        }
        case 0x19d6eU: { // 41fa18d0 lea.l $1b640(pc), a0
            next = 0x19d72U;
            const auto source_address = 0x1b640U;
            r.address[0] = source_address;
            break;
        }
    default: return false;
    }
    return true;
}
