#include "cpu_b_state72_dispatch_record_phase_detail.h"

namespace gain_ground::translated::cpu_b_state72_dispatch_record_phase_detail {

bool dispatch_record_region_08(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind,
    std::optional<FunctionResult> &outcome)
{
    auto &c = context;
    auto &r = registers;
    auto &m = machine;
    switch (pc) {
        case 0xf910U: // 672c beq.b $f93e
            next = (r.status & 4U) ? 0xf93eU : 0xf912U; transfer_kind = 1U; break;
        case 0xf912U: case 0xf920U: case 0xf92eU: {
            const auto offset = pc == 0xf912U ? 0x86U : pc == 0xf920U ? 0xa4U : 0xa8U;
            next = pc + 4U; r.data[2] = m.lng(r.address[4] + offset); m.logic(r.data[2], 32U); break;
        }
        case 0xf916U: case 0xf924U: case 0xf932U: case 0xf988U:
            next = pc + 4U;
            r.address[1] = pc == 0xf916U ? 0x107d8U : pc == 0xf924U ? 0x107dcU : 0x107e0U;
            break;
        case 0xf91aU: case 0xf928U: case 0xf936U: case 0xf98cU: {
            const auto result = m.call(c, 296U, pc, 0x1610eU, pc + 6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter; break;
        }
        case 0xf93eU: // 222c00a8 move.l $a8(a4),d1
            next = 0xf942U; r.data[1] = m.lng(r.address[4] + 0xa8U); m.logic(r.data[1], 32U); break;
        case 0xf942U: // 6762 beq.b $f9a6
            next = (r.status & 4U) ? 0xf9a6U : 0xf944U; transfer_kind = 1U; break;
        case 0xf944U: case 0xf952U: case 0xf960U: case 0xf96aU:
            next = pc + 6U;
            r.data[0] = pc == 0xf944U ? 0x50000U : pc == 0xf952U ? 0x10000U
                : pc == 0xf960U ? 0x3000U : 0x200U;
            m.logic(r.data[0], 32U); break;
        case 0xf94aU: case 0xf958U:
            next = pc + 6U;
            (void)m.sub(r.data[1], pc == 0xf94aU ? 0x2000000U : 0x100000U, 32U, true); break;
        case 0xf966U: case 0xf970U:
            next = pc + 2U; (void)m.sub(r.data[1], r.data[0], 32U, true); break;
        case 0xf950U: case 0xf95eU: case 0xf968U: case 0xf972U:
            next = (r.status & 8U) ? pc + 2U : 0xf976U; transfer_kind = 1U; break;
        case 0xf974U:
            next = 0xf976U; r.data[0] = r.data[1]; m.logic(r.data[0], 32U); break;
        case 0xf976U: case 0xf97cU: {
            const auto result = m.call(c, pc == 0xf976U ? 286U : 285U, pc,
                pc == 0xf976U ? 0x15ebeU : 0x15ea0U, pc + 6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter; break;
        }
        case 0xf982U:
            next = 0xf986U; m.lng(r.address[4] + 0xa8U, r.data[0]); m.logic(r.data[0], 32U); break;
        case 0xf986U:
            next = 0xf988U; r.data[2] = r.data[0]; m.logic(r.data[2], 32U); break;
        case 0xf992U: {
            const auto result = m.call(c, 187U, pc, 0xfb44U, 0xf996U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter; break;
        }
        case 0xf996U:
            next = 0xf998U; r.address[7] -= 4U; m.lng(r.address[7], r.address[4]);
            m.logic(r.address[4], 32U); break;
        case 0xf998U:
            next = 0xf99cU; m.dw(0U, 0x40U); m.logic(0x40U, 16U); break;
        case 0xf99cU: {
            const auto result = m.call(c, 308U, pc, 0x16ff8U, 0xf9a2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter; break;
        }
        case 0xf9a2U:
            next = 0xf9a4U; r.address[4] = m.lng(r.address[7]); r.address[7] += 4U; break;
        case 0xf9a6U:
            next = 0xf9acU; m.word(r.address[5] + 0x44U, 10U); m.logic(10U, 16U); break;
        case 0xf9acU:
            next = 0xf9b0U; m.db(0U, m.byte(r.address[5] + 0x6dU)); m.logic(r.data[0], 8U); break;
        case 0xf9b0U: {
            next = 0xf9b4U;
            const auto old = m.byte(0xc05U);
            const auto bit = static_cast<std::uint8_t>(1U << (r.data[0] & 7U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit) ? 0U : 4U));
            m.byte(0xc05U, old & ~bit); break;
        }
        case 0xf9b6U:
            next = 0xf9bcU; (void)m.sub(m.word(0xc16U), 5U, 16U, true); break;
        case 0xf9bcU:
            next = (r.status & 4U) ? 0xf9beU : 0xf9c2U; transfer_kind = 1U; break;
        case 0xf9beU: { // 6100fbb2 bsr.w $f572; source-defined interior entry.
            const auto result = m.call(c, 178U, pc, 0xf572U, 0xf9c2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter; break;
        }
        case 0xfe18U: { // 61000022 bsr.w $fe3c
            next = 0xfe1cU;
            const auto result = m.call(c, 199U, 0xfe18U, 0xfe3cU, 0xfe1cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xfe1cU: { // 61000036 bsr.w $fe54
            next = 0xfe20U;
            const auto result = m.call(c, 200U, 0xfe1cU, 0xfe54U, 0xfe20U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xfe20U: { // 610000b4 bsr.w $fed6
            next = 0xfe24U;
            const auto result = m.call(c, 201U, 0xfe20U, 0xfed6U, 0xfe24U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xfe24U: { // 4eb900010a24 jsr $10a24.l
            next = 0xfe2aU;
            const auto result = m.call(c, 208U, 0xfe24U, 0x10a24U, 0xfe2aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xfe2aU: { // 6100047e bsr.w $102aa
            next = 0xfe2eU;
            const auto result = m.call(c, 204U, 0xfe2aU, 0x102aaU, 0xfe2eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xfe2eU: { // 61000544 bsr.w $10374
            next = 0xfe32U;
            const auto result = m.call(c, 205U, 0xfe2eU, 0x10374U, 0xfe32U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xfe32U: { // 610005b0 bsr.w $103e4
            next = 0xfe36U;
            const auto result = m.call(c, 206U, 0xfe32U, 0x103e4U, 0xfe36U);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xfe36U: { // 6100fe76 bsr.w $fcae
            next = 0xfe3aU;
            const auto result = m.call(c, 195U, 0xfe36U, 0xfcaeU, 0xfe3aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) { outcome = result; return true; }
            next = r.program_counter;
            break;
        }
        case 0xfe3aU: { // 4e75 rts 
            next = 0xfe3cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xfe3aU, r.program_counter)) { outcome = *event; return true; }
            { outcome = result; return true; }
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_state72_dispatch_record_phase_detail
