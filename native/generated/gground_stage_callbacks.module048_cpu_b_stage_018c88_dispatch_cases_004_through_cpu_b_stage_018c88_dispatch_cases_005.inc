
static bool cpu_b_stage_018c88_dispatch_cases_004([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x18eb8U: { // 6208 bhi.b $18ec2
            next = 0x18ebaU;
            if ((r.status & 5U) == 0U) { next = 0x18ec2U; transfer_kind = 1U; }
            break;
        }
        case 0x18ebaU: { // 426d0048 clr.w $48(a5)
            next = 0x18ebeU;
            const auto destination_address = r.address[5] + 0x48U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x18ebeU: { // 426d0040 clr.w $40(a5)
            next = 0x18ec2U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x190e0U: { // 2b7c00018c280002 move.l #$18c28, $2(a5)
            next = 0x190e8U;
            const auto value = 0x18c28U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x190e8U: { // 3b7c0c0e0008 move.w #$c0e, $8(a5)
            next = 0x190eeU;
            const auto value = 0xc0eU;
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x190eeU: { // 3b7c00960070 move.w #$96, $70(a5)
            next = 0x190f4U;
            const auto value = 0x96U;
            const auto destination_address = r.address[5] + 0x70U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x190f4U: { // 3b7c00080066 move.w #$8, $66(a5)
            next = 0x190faU;
            const auto value = 0x8U;
            const auto destination_address = r.address[5] + 0x66U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x190faU: { // 426d0068 clr.w $68(a5)
            next = 0x190feU;
            const auto destination_address = r.address[5] + 0x68U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x190feU: { // 4af80d1b tas.b $d1b.w
            next = 0x19102U;
            const auto destination_address = 0xd1bU;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0x19102U: { // 41fa1f86 lea.l $1b08a(pc), a0
            next = 0x19106U;
            const auto source_address = 0x1b08aU;
            r.address[0] = source_address;
            break;
        }
        case 0x19106U: { // 7e04 moveq #$4, d7
            next = 0x19108U;
            r.data[7] = 0x4U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x19108U: { // 4df85400 lea.l $5400.w, a6
            next = 0x1910cU;
            const auto source_address = 0x5400U;
            r.address[6] = source_address;
            break;
        }
        case 0x1910cU: { // 3c3c002f move.w #$2f, d6
            next = 0x19110U;
            const auto value = 0x2fU;
            m.dw(6U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19110U: { // 4a16 tst.b (a6)
            next = 0x19112U;
            const auto destination_address = r.address[6];
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x19112U: { // 6a0a bpl.b $1911e
            next = 0x19114U;
            if ((r.status & 8U) == 0U) { next = 0x1911eU; transfer_kind = 1U; }
            break;
        }
        case 0x19114U: { // 4dee0080 lea.l $80(a6), a6
            next = 0x19118U;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x19118U: { // 51cefff6 dbra d6, $19110
            next = 0x1911cU;
            m.dw(6U, r.data[6] - 1U);
            if ((r.data[6] & 0xffffU) != 0xffffU) next = 0x19110U;
            break;
        }
        case 0x1911eU: { // 1cbc0080 move.b #$80, (a6)
            next = 0x19122U;
            const auto value = 0x80U;
            const auto destination_address = r.address[6];
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19122U: { // 2d7c000194740002 move.l #$19474, $2(a6)
            next = 0x1912aU;
            const auto value = 0x19474U;
            const auto destination_address = r.address[6] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1912aU: { // 1d7c0000000b move.b #$0, $b(a6)
            next = 0x19130U;
            const auto value = 0x0U;
            const auto destination_address = r.address[6] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19130U: { // 302d000c move.w $c(a5), d0
            next = 0x19134U;
            const auto source_address = r.address[5] + 0xcU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19134U: { // d058 add.w (a0)+, d0
            next = 0x19136U;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19136U: { // 3d40000c move.w d0, $c(a6)
            next = 0x1913aU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0xcU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1913aU: { // 302d000e move.w $e(a5), d0
            next = 0x1913eU;
            const auto source_address = r.address[5] + 0xeU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1913eU: { // d058 add.w (a0)+, d0
            next = 0x19140U;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19140U: { // 3d40000e move.w d0, $e(a6)
            next = 0x19144U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0xeU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19144U: { // 3d6d001a001a move.w $1a(a5), $1a(a6)
            next = 0x1914aU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[6] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1914aU: { // 426e0064 clr.w $64(a6)
            next = 0x1914eU;
            const auto destination_address = r.address[6] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1914eU: { // 426e0068 clr.w $68(a6)
            next = 0x19152U;
            const auto destination_address = r.address[6] + 0x68U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19152U: { // 3d58006e move.w (a0)+, $6e(a6)
            next = 0x19156U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[6] + 0x6eU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19156U: { // 3d7c00960070 move.w #$96, $70(a6)
            next = 0x1915cU;
            const auto value = 0x96U;
            const auto destination_address = r.address[6] + 0x70U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1915cU: { // 4dee0080 lea.l $80(a6), a6
            next = 0x19160U;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
    default: return false;
    }
    return true;
}

static bool cpu_b_stage_018c88_dispatch_cases_005([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19160U: { // 51cfffae dbra d7, $19110
            next = 0x19164U;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x19110U;
            break;
        }
    default: return false;
    }
    return true;
}
