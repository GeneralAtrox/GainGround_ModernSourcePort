CpuAResetEntryStep handle_cpu_a_reset_entry_pc_002400_002500(
    FunctionContext &c, CpuRegisters &r, unverified::Machine &m,
    std::uint32_t pc, std::uint32_t &next)
{
    switch (pc) {
        case 0x2400U: { // 227c00404001 movea.l #$404001, a1
            next = 0x2406U;
            const auto value = 0x404001U;
            r.address[1] = value;
            break;
        }
        case 0x2406U: { // 2018 move.l (a0)+, d0
            next = 0x2408U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x2408U: { // 01c90000 movep.l d0, $0(a1)
            next = 0x240cU;
            const auto memory_address = r.address[1] + 0x0U;
            m.byte(memory_address + 0U, r.data[0] >> 24U);
            m.byte(memory_address + 2U, r.data[0] >> 16U);
            m.byte(memory_address + 4U, r.data[0] >> 8U);
            m.byte(memory_address + 6U, r.data[0] >> 0U);
            break;
        }
        case 0x240cU: { // 2018 move.l (a0)+, d0
            next = 0x240eU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x240eU: { // 01c90008 movep.l d0, $8(a1)
            next = 0x2412U;
            const auto memory_address = r.address[1] + 0x8U;
            m.byte(memory_address + 0U, r.data[0] >> 24U);
            m.byte(memory_address + 2U, r.data[0] >> 16U);
            m.byte(memory_address + 4U, r.data[0] >> 8U);
            m.byte(memory_address + 6U, r.data[0] >> 0U);
            break;
        }
        case 0x2412U: { // 2018 move.l (a0)+, d0
            next = 0x2414U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x2414U: { // 01c90010 movep.l d0, $10(a1)
            next = 0x2418U;
            const auto memory_address = r.address[1] + 0x10U;
            m.byte(memory_address + 0U, r.data[0] >> 24U);
            m.byte(memory_address + 2U, r.data[0] >> 16U);
            m.byte(memory_address + 4U, r.data[0] >> 8U);
            m.byte(memory_address + 6U, r.data[0] >> 0U);
            break;
        }
        case 0x2418U: { // 61000020 bsr.w $243a
            next = 0x241cU;
            const auto result = m.call(c, 37U, 0x2418U, 0x243aU, 0x241cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
        case 0x241cU: { // 3e3c0100 move.w #$100, d7
            next = 0x2420U;
            m.dw(7U, 0x100U);
            m.logic(r.data[7], 16U);
            break;
        }
        case 0x2420U: { // 43fa167a lea.l $3a9c(pc), a1
            next = 0x2424U;
            const auto source_address = 0x3a9cU;
            r.address[1] = source_address;
            break;
        }
        case 0x2424U: { // 61001468 bsr.w $388e
            next = 0x2428U;
            const auto result = m.call(c, 45U, 0x2424U, 0x388eU, 0x2428U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return {true, (result)};
            next = r.program_counter;
            break;
        }
    default: return {};
    }
    return {true, std::nullopt};
}
