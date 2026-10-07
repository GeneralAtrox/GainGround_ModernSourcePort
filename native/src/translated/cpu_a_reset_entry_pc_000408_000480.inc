CpuAResetEntryStep handle_cpu_a_reset_entry_pc_000408_000480(
    FunctionContext &c, CpuRegisters &r, unverified::Machine &m,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind)
{
    switch (pc) {
        case 0x408U: { // 3e7c0000 movea.w #$0, a7
            next = 0x40cU;
            (void)m.word(0x40cU);
            const auto value = 0x0U;
            r.address[7] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            (void)m.word(0x40eU);
            break;
        }
        case 0x40cU: { // 46fc2600 move.w #$2600, sr
            next = 0x410U;
            (void)m.word(0x410U);
            r.status = static_cast<std::uint16_t>(0x2600U);
            (void)m.word(0x410U);
            (void)m.word(0x412U);
            break;
        }
        case 0x410U: { // 13fc00040080001d move.b #$4, $80001d.l
            next = 0x418U;
            (void)m.word(0x414U);
            (void)m.word(0x416U);
            (void)m.word(0x418U);
            const auto value = 0x4U;
            const auto destination_address = 0x80001dU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            (void)m.word(0x41aU);
            break;
        }
        case 0x418U: { // 13fc000000800007 move.b #$0, $800007.l
            next = 0x420U;
            (void)m.word(0x41cU);
            (void)m.word(0x41eU);
            (void)m.word(0x420U);
            const auto value = 0x0U;
            const auto destination_address = 0x800007U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            (void)m.word(0x422U);
            break;
        }
        case 0x420U: { // 13fc00880080001f move.b #$88, $80001f.l
            next = 0x428U;
            (void)m.word(0x424U);
            (void)m.word(0x426U);
            (void)m.word(0x428U);
            const auto value = 0x88U;
            const auto destination_address = 0x80001fU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            c.host->write_hardware(4U, m.cpu, m.state, 0x420U, 0x80000eU, 0U, 0xffU);
            (void)m.word(0x42aU);
            break;
        }
        case 0x428U: { // 13fc000000404019 move.b #$0, $404019.l
            next = 0x430U;
            (void)m.word(0x42cU);
            (void)m.word(0x42eU);
            (void)m.word(0x430U);
            const auto value = 0x0U;
            const auto destination_address = 0x404019U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            (void)m.word(0x432U);
            break;
        }
        case 0x430U: { // 13fc000000270001 move.b #$0, $270001.l
            next = 0x438U;
            (void)m.word(0x434U);
            (void)m.word(0x436U);
            (void)m.word(0x438U);
            const auto value = 0x0U;
            const auto destination_address = 0x270001U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            (void)m.word(0x43aU);
            break;
        }
        case 0x438U: { // 33fcffc600240000 move.w #$ffc6, $240000.l
            next = 0x440U;
            (void)m.word(0x43cU);
            (void)m.word(0x43eU);
            (void)m.word(0x440U);
            const auto value = 0xffc6U;
            const auto destination_address = 0x240000U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x442U);
            break;
        }
        case 0x440U: { // 33fcfff000260000 move.w #$fff0, $260000.l
            next = 0x448U;
            (void)m.word(0x444U);
            (void)m.word(0x446U);
            (void)m.word(0x448U);
            const auto value = 0xfff0U;
            const auto destination_address = 0x260000U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x44aU);
            break;
        }
        case 0x448U: { // 13fc000800a00005 move.b #$8, $a00005.l
            next = 0x450U;
            (void)m.word(0x44cU);
            (void)m.word(0x44eU);
            (void)m.word(0x450U);
            const auto value = 0x8U;
            const auto destination_address = 0xa00005U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            (void)m.word(0x452U);
            break;
        }
        case 0x450U: { // 13fc000000a00007 move.b #$0, $a00007.l
            next = 0x458U;
            (void)m.word(0x454U);
            (void)m.word(0x456U);
            (void)m.word(0x458U);
            const auto value = 0x0U;
            const auto destination_address = 0xa00007U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            (void)m.word(0x45aU);
            break;
        }
        case 0x458U: { // 383c1111 move.w #$1111, d4
            next = 0x45cU;
            (void)m.word(0x45cU);
            m.dw(4U, 0x1111U);
            m.logic(r.data[4], 16U);
            (void)m.word(0x45eU);
            break;
        }
        case 0x45cU: { // 4dfa19f6 lea.l $1e54(pc), a6
            next = 0x460U;
            (void)m.word(0x460U);
            const auto source_address = 0x1e54U;
            r.address[6] = source_address;
            (void)m.word(0x462U);
            break;
        }
        case 0x460U: { // 7e09 moveq #$9, d7
            next = 0x462U;
            r.data[7] = 0x9U;
            m.logic(r.data[7], 32U);
            (void)m.word(0x464U);
            break;
        }
        case 0x462U: { // 33fc111100080000 move.w #$1111, $80000.l
            next = 0x46aU;
            (void)m.word(0x466U);
            (void)m.word(0x468U);
            (void)m.word(0x46aU);
            const auto value = 0x1111U;
            const auto destination_address = 0x80000U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x46cU);
            break;
        }
        case 0x46aU: { // 33fc2222000c0000 move.w #$2222, $c0000.l
            next = 0x472U;
            (void)m.word(0x46eU);
            (void)m.word(0x470U);
            (void)m.word(0x472U);
            const auto value = 0x2222U;
            const auto destination_address = 0xc0000U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            (void)m.word(0x474U);
            break;
        }
        case 0x472U: { // 0c79111100080000 cmpi.w #$1111, $80000.l
            next = 0x47aU;
            (void)m.word(0x476U);
            (void)m.word(0x478U);
            (void)m.word(0x47aU);
            const auto source_value = 0x1111U;
            const auto destination_address = 0x80000U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            (void)m.word(0x47cU);
            break;
        }
        case 0x47aU: { // 6606 bne.b $482
            next = 0x47cU;
            if ((r.status & 4U) == 0U) { next = 0x482U; transfer_kind = 1U; }
            if (transfer_kind == 1U) { (void)m.word(next); }
            (void)m.word(next + 2U);
            break;
        }
    default: return {};
    }
    return {true, std::nullopt};
}
