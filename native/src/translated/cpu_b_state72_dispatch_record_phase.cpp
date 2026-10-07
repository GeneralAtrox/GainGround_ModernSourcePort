// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"
#include "cpu_b_state72_dispatch_record_phase_detail.h"

namespace gain_ground::translated {
FunctionResult cpu_b_state72_dispatch_record_phase(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        if (r.program_counter >= 0xfe18U && r.program_counter <= 0xfe3aU) {
            FunctionResult character_result;
            if (c.host->run_character_update(c, character_result)) return character_result;
        }
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        default: {
            std::optional<FunctionResult> outcome;
            if (cpu_b_state72_dispatch_record_phase_detail::dispatch_record_region_01(
                    c, r, m, pc, next, transfer_kind, outcome)) {
                if (outcome) return *outcome;
                break;
            }
            if (cpu_b_state72_dispatch_record_phase_detail::dispatch_record_region_02(
                    c, r, m, pc, next, transfer_kind, outcome)) {
                if (outcome) return *outcome;
                break;
            }
            if (cpu_b_state72_dispatch_record_phase_detail::dispatch_record_region_03(
                    c, r, m, pc, next, transfer_kind, outcome)) {
                if (outcome) return *outcome;
                break;
            }
            if (cpu_b_state72_dispatch_record_phase_detail::dispatch_record_region_04(
                    c, r, m, pc, next, transfer_kind, outcome)) {
                if (outcome) return *outcome;
                break;
            }
            if (cpu_b_state72_dispatch_record_phase_detail::dispatch_record_region_05(
                    c, r, m, pc, next, transfer_kind, outcome)) {
                if (outcome) return *outcome;
                break;
            }
            if (cpu_b_state72_dispatch_record_phase_detail::dispatch_record_region_06(
                    c, r, m, pc, next, transfer_kind, outcome)) {
                if (outcome) return *outcome;
                break;
            }
            if (cpu_b_state72_dispatch_record_phase_detail::dispatch_record_region_07(
                    c, r, m, pc, next, transfer_kind, outcome)) {
                if (outcome) return *outcome;
                break;
            }
            if (cpu_b_state72_dispatch_record_phase_detail::dispatch_record_region_08(
                    c, r, m, pc, next, transfer_kind, outcome)) {
                if (outcome) return *outcome;
                break;
            }
            return {TranslationStatus::contract_violation, 0U, pc};
        }
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0xf3c0U:
        case 0xf3c4U:
        case 0xf3c6U:
        case 0xf3caU:
        case 0xf3ccU:
        case 0xf3d0U:
        case 0xf3d4U:
        case 0xf3d6U:
        case 0xf3daU:
        case 0xf3deU:
        case 0xf3e2U:
        case 0xf3e6U:
        case 0xf3eaU:
        case 0xf3eeU:
        case 0xf3f2U:
        case 0xf3f6U:
        case 0xf3faU: case 0xf3feU: case 0xf402U:
        case 0xf51aU:
        case 0xf51eU:
        case 0xf522U:
        case 0xf526U:
        case 0xf528U:
        case 0xf52cU:
        case 0xf52eU:
        case 0xf532U:
        case 0xf538U:
        case 0xf53cU:
        case 0xf53eU:
        case 0xf542U:
        case 0xf548U:
        case 0xf54cU:
        case 0xf552U:
        case 0xf556U:
        case 0xf558U:
        case 0xf55cU:
        case 0xf55eU: case 0xf564U: case 0xf56aU: case 0xf570U:
        case 0xf572U:
        case 0xf578U:
        case 0xf57eU:
        case 0xf584U:
        case 0xf588U:
        case 0xf58cU:
        case 0xf590U:
        case 0xf594U:
        case 0xf598U:
        case 0xf59cU:
        case 0xf5a0U:
        case 0xf5a4U:
        case 0xf5a8U:
        case 0xf5aaU:
        case 0xf5acU:
        case 0xf5aeU:
        case 0xf5b0U:
        case 0xf5b2U:
        case 0xf5b4U:
        case 0xf5b6U:
        case 0xf5baU:
        case 0xf5beU:
        case 0xf5c0U:
        case 0xf5c2U:
        case 0xf5c4U:
        case 0xf5c6U:
        case 0xf5c8U:
        case 0xf5ccU:
        case 0xf5d0U:
        case 0xf5d2U:
        case 0xf5d4U:
        case 0xf5d6U:
        case 0xf5d8U:
        case 0xf5daU:
        case 0xf5dcU:
        case 0xf5deU:
        case 0xf5e4U:
        case 0xf5e8U:
        case 0xf5eaU:
        case 0xf5ecU:
        case 0xf5f0U:
        case 0xf5f4U:
        case 0xf5f6U:
        case 0xf5fcU:
        case 0xf600U:
        case 0xf604U:
        case 0xf608U:
        case 0xf60cU:
        case 0xf610U:
        case 0xf614U:
        case 0xf618U:
        case 0xf61cU:
        case 0xf620U:
        case 0xf624U:
        case 0xf626U:
        case 0xf62cU:
        case 0xf630U:
        case 0xf634U:
        case 0xf636U:
        case 0xf638U:
        case 0xf63cU:
        case 0xf63eU:
        case 0xf642U:
        case 0xf646U:
        case 0xf648U:
        case 0xf64eU:
        case 0xf652U:
        case 0xf656U:
        case 0xf658U:
        case 0xf65cU:
        case 0xf662U:
        case 0xf664U:
        case 0xf668U:
        case 0xf66cU:
        case 0xf66eU:
        case 0xf670U:
        case 0xf676U:
        case 0xf678U:
        case 0xf67cU:
        case 0xf67eU:
        case 0xf682U:
        case 0xf684U: case 0xf68aU: case 0xf68eU:
        case 0xf690U:
        case 0xf696U:
        case 0xf69aU:
        case 0xf69eU:
        case 0xf6a4U:
        case 0xf6a6U:
        case 0xf6aaU:
        case 0xf6b0U:
        case 0xf6b2U:
        case 0xf6b6U:
        case 0xf6b8U:
        case 0xf6bcU:
        case 0xf6beU:
        case 0xf6c2U:
        case 0xf6c4U:
        case 0xf6c8U:
        case 0xf6ccU:
        case 0xf6ceU:
        case 0xf6d2U:
        case 0xf6d6U:
        case 0xf6daU:
        case 0xf6deU:
        case 0xf6e0U:
        case 0xf6e2U:
        case 0xf6e6U:
        case 0xf6e8U:
        case 0xf6eaU:
        case 0xf6eeU:
        case 0xf6f0U:
        case 0xf6f4U:
        case 0xf6faU:
        case 0xf6fcU:
        case 0xf702U:
        case 0xf706U:
        case 0xf70aU:
        case 0xf70cU:
        case 0xf710U:
        case 0xf712U:
        case 0xf718U:
        case 0xf71cU:
        case 0xf71eU:
        case 0xf720U:
        case 0xf724U:
        case 0xf728U:
        case 0xf72eU:
        case 0xf732U:
        case 0xf734U:
        case 0xf738U:
        case 0xf73aU:
        case 0xf740U:
        case 0xf742U:
        case 0xf746U:
        case 0xf74cU:
        case 0xf752U:
        case 0xf758U:
        case 0xf75cU:
        case 0xf762U:
        case 0xf768U:
        case 0xf76eU:
        case 0xf774U:
        case 0xf77aU:
        case 0xf780U:
        case 0xf786U:
        case 0xf78cU:
        case 0xf792U:
        case 0xf796U:
        case 0xf79aU:
        case 0xf79cU:
        case 0xf7a0U:
        case 0xf7a4U:
        case 0xf7aaU:
        case 0xf7aeU:
        case 0xf7b4U:
        case 0xf7b6U:
        case 0xf7bcU:
        case 0xf7beU:
        case 0xf7c4U:
        case 0xf7c8U:
        case 0xf7ccU:
        case 0xf7d0U:
        case 0xf7d4U:
        case 0xf7d8U:
        case 0xf7dcU:
        case 0xf7e0U:
        case 0xf7e4U:
        case 0xf7e6U:
        case 0xf7ecU:
        case 0xf7eeU:
        case 0xf7f2U:
        case 0xf7f8U:
        case 0xf7fcU:
        case 0xf804U:
        case 0xf80aU:
        case 0xf810U:
        case 0xf816U:
        case 0xf81aU:
        case 0xf81cU:
        case 0xf820U:
        case 0xf822U:
        case 0xf824U:
        case 0xf828U:
        case 0xf82eU:
        case 0xf830U:
        case 0xf836U:
        case 0xf83aU:
        case 0xf83eU:
        case 0xf840U:
        case 0xf844U:
        case 0xf84aU:
        case 0xf850U:
        case 0xf856U:
        case 0xf85aU:
        case 0xf85eU:
        case 0xf860U:
        case 0xf864U:
        case 0xf866U:
        case 0xf86aU:
        case 0xf86eU:
        case 0xf870U:
        case 0xf872U:
        case 0xf874U:
        case 0xf876U:
        case 0xf878U:
        case 0xf87aU:
        case 0xf87eU:
        case 0xf880U:
        case 0xf884U:
        case 0xf888U:
        case 0xf88cU:
        case 0xf88eU:
        case 0xf890U:
        case 0xf892U:
        case 0xf896U:
        case 0xf898U:
        case 0xf89eU:
        case 0xf8a2U:
        case 0xf8a6U:
        case 0xf8aaU:
        case 0xf8acU:
        case 0xf8b4U:
        case 0xf8baU:
        case 0xf8c0U:
        case 0xf8c6U:
        case 0xf8ccU:
        case 0xf8d2U:
        case 0xf8d8U:
        case 0xf8deU:
        case 0xf8e2U:
        case 0xf8e8U:
        case 0xf8eeU:
        case 0xf8f4U:
        case 0xf8f6U:
        case 0xf8faU:
        case 0xf8feU:
        case 0xfe18U:
        case 0xf900U: case 0xf902U: case 0xf908U: case 0xf90cU: case 0xf910U:
        case 0xf912U: case 0xf916U: case 0xf91aU: case 0xf920U: case 0xf924U:
        case 0xf928U: case 0xf92eU: case 0xf932U: case 0xf936U: case 0xf93cU:
        case 0xf93eU: case 0xf942U: case 0xf944U: case 0xf94aU: case 0xf950U:
        case 0xf952U: case 0xf958U: case 0xf95eU: case 0xf960U: case 0xf966U:
        case 0xf968U: case 0xf96aU: case 0xf970U: case 0xf972U: case 0xf974U:
        case 0xf976U: case 0xf97cU: case 0xf982U: case 0xf986U: case 0xf988U:
        case 0xf98cU: case 0xf992U: case 0xf996U: case 0xf998U: case 0xf99cU:
        case 0xf9a2U: case 0xf9a4U: case 0xf9a6U: case 0xf9acU: case 0xf9b0U:
        case 0xf9b4U: case 0xf9b6U: case 0xf9bcU: case 0xf9beU: case 0xf9c2U:
        case 0xfe1cU:
        case 0xfe20U:
        case 0xfe24U:
        case 0xfe2aU:
        case 0xfe2eU:
        case 0xfe32U:
        case 0xfe36U:
        case 0xfe3aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
