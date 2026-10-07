// Implemented but unverified. Validation is recorded in the current function work packet.
#include "unverified_cpu_b_machine.h"
#include "proven_static_cpu_b_72_000121f4_detail.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_000121f4(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1220eU: { // 6100078c bsr.w $1299c
            next = 0x12212U;
            const auto result = m.call(c, 535U, 0x1220eU, 0x1299cU, 0x12212U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12214U: { // 61000b24 bsr.w $12d3a
            next = 0x12218U;
            const auto result = m.call(c, 263U, 0x12214U, 0x12d3aU, 0x12218U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12218U: { // 61000a22 bsr.w $12c3c
            next = 0x1221cU;
            const auto result = m.call(c, 259U, 0x12218U, 0x12c3cU, 0x1221cU);
            if (result.status == TranslationStatus::complete && result.control == 8U)
                return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12222U: { // 61000b16 bsr.w $12d3a
            next = 0x12226U;
            const auto result = m.call(c, 263U, 0x12222U, 0x12d3aU, 0x12226U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1227eU: { // 6100071c bsr.w $1299c
            next = 0x12282U;
            const auto result = m.call(c, 535U, 0x1227eU, 0x1299cU, 0x12282U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1229eU: { // 610006fc bsr.w $1299c
            next = 0x122a2U;
            const auto result = m.call(c, 535U, 0x1229eU, 0x1299cU, 0x122a2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x122d0U: { // 61000a36 bsr.w $12d08
            next = 0x122d4U;
            const auto result = m.call(c, 261U, 0x122d0U, 0x12d08U, 0x122d4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x122d4U: { // 4e75 rts 
            next = 0x122d6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x122d4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x12328U: { // 4e75 rts 
            next = 0x1232aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x12328U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x12332U: { // 4eb900015d24 jsr $15d24.l
            next = 0x12338U;
            const auto result = m.call(c, 280U, 0x12332U, 0x15d24U, 0x12338U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12338U: { // 4eb900015d3c jsr $15d3c.l
            next = 0x1233eU;
            const auto result = m.call(c, 281U, 0x12338U, 0x15d3cU, 0x1233eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1233eU: { // 4eb900015df2 jsr $15df2.l
            next = 0x12344U;
            const auto result = m.call(c, 282U, 0x1233eU, 0x15df2U, 0x12344U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12344U: { // 4e75 rts 
            next = 0x12346U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x12344U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (proven_static_cpu_b_72_000121f4_detail::dispatch_descriptor_setup(
                    c, r, m, pc, next, transfer_kind))
                break;
            if (proven_static_cpu_b_72_000121f4_detail::dispatch_record_fields(
                    c, r, m, pc, next, transfer_kind))
                break;
            if (proven_static_cpu_b_72_000121f4_detail::dispatch_tail_fields(
                    c, r, m, pc, next, transfer_kind))
                break;
            return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x121f4U:
        case 0x121f8U:
        case 0x121faU:
        case 0x121feU:
        case 0x12202U:
        case 0x12206U:
        case 0x1220aU:
        case 0x1220eU:
        case 0x12212U:
        case 0x12214U:
        case 0x12218U:
        case 0x1221cU:
        case 0x12220U:
        case 0x12222U:
        case 0x12226U:
        case 0x1222aU:
        case 0x1222cU:
        case 0x1222eU:
        case 0x12232U:
        case 0x12236U:
        case 0x1223cU:
        case 0x12240U:
        case 0x12244U:
        case 0x12248U:
        case 0x1224cU:
        case 0x12254U:
        case 0x12258U:
        case 0x1225cU:
        case 0x12260U:
        case 0x12266U:
        case 0x1226aU:
        case 0x1226eU:
        case 0x12274U:
        case 0x12276U:
        case 0x1227cU:
        case 0x1227eU:
        case 0x12282U:
        case 0x12288U:
        case 0x1228aU:
        case 0x1228eU:
        case 0x12294U:
        case 0x12296U:
        case 0x1229cU:
        case 0x1229eU:
        case 0x122a2U:
        case 0x122a6U:
        case 0x122a8U:
        case 0x122aeU:
        case 0x122b0U:
        case 0x122b4U:
        case 0x122baU:
        case 0x122bcU:
        case 0x122c2U:
        case 0x122c4U:
        case 0x122c8U:
        case 0x122ceU:
        case 0x122d0U:
        case 0x122d4U:
        case 0x122d6U:
        case 0x122daU:
        case 0x122deU:
        case 0x122e2U:
        case 0x122e6U:
        case 0x122e8U:
        case 0x122ecU:
        case 0x122f0U:
        case 0x122f2U:
        case 0x122f6U:
        case 0x122f8U:
        case 0x122fcU:
        case 0x12300U:
        case 0x12304U:
        case 0x12306U:
        case 0x1230aU:
        case 0x1230eU:
        case 0x12310U:
        case 0x12314U:
        case 0x12316U:
        case 0x1231aU:
        case 0x1231cU:
        case 0x1231eU:
        case 0x12320U:
        case 0x12322U:
        case 0x12324U:
        case 0x12326U:
        case 0x12328U:
        case 0x1232aU:
        case 0x1232eU:
        case 0x12332U:
        case 0x12338U:
        case 0x1233eU:
        case 0x12344U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
