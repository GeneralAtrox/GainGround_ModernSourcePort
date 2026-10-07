// Implemented but unverified. Validation is recorded in the current function work packet.
#include "unverified_cpu_b_machine.h"
#include "cpu_b_advance_phase_callback_dispatch_detail.h"

namespace gain_ground::translated {
FunctionResult cpu_b_advance_phase_callback_dispatch(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        default: {
            std::optional<FunctionResult> outcome;
            if (cpu_b_advance_phase_callback_dispatch_detail::dispatch_instruction_region_01(
                    c, r, m, pc, next, transfer_kind, outcome)) {
                if (outcome) return *outcome;
                break;
            }
            if (cpu_b_advance_phase_callback_dispatch_detail::dispatch_instruction_region_02(
                    c, r, m, pc, next, transfer_kind, outcome)) {
                if (outcome) return *outcome;
                break;
            }
            if (cpu_b_advance_phase_callback_dispatch_detail::dispatch_instruction_region_03(
                    c, r, m, pc, next, transfer_kind, outcome)) {
                if (outcome) return *outcome;
                break;
            }
            if (cpu_b_advance_phase_callback_dispatch_detail::dispatch_instruction_region_04(
                    c, r, m, pc, next, transfer_kind, outcome)) {
                if (outcome) return *outcome;
                break;
            }
            if (cpu_b_advance_phase_callback_dispatch_detail::dispatch_instruction_region_05(
                    c, r, m, pc, next, transfer_kind, outcome)) {
                if (outcome) return *outcome;
                break;
            }
            if (cpu_b_advance_phase_callback_dispatch_detail::dispatch_instruction_region_06(
                    c, r, m, pc, next, transfer_kind, outcome)) {
                if (outcome) return *outcome;
                break;
            }
            if (cpu_b_advance_phase_callback_dispatch_detail::dispatch_instruction_region_07(
                    c, r, m, pc, next, transfer_kind, outcome)) {
                if (outcome) return *outcome;
                break;
            }
            if (cpu_b_advance_phase_callback_dispatch_detail::dispatch_instruction_region_08(
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
        if (next == 0x9c8eU) return c.host->call_function(519U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0xd734U:
        case 0xd738U:
        case 0xd73cU:
        case 0xd740U:
        case 0xd744U:
        case 0xd746U:
        case 0xd74aU:
        case 0xd74eU:
        case 0xd752U:
        case 0xd756U:
        case 0xd75aU:
        case 0xd75eU:
        case 0xd762U:
        case 0xd766U:
        case 0xd76aU:
        case 0xd76eU:
        case 0xd772U:
        case 0xd776U:
        case 0xd77aU:
        case 0xd77cU:
        case 0xd780U:
        case 0xd784U:
        case 0xd786U:
        case 0xd78cU:
        case 0xd78eU:
        case 0xd794U:
        case 0xd79aU:
        case 0xd7a0U:
        case 0xd7a4U:
        case 0xd7a6U:
        case 0xd7aaU:
        case 0xd7aeU:
        case 0xd7b2U:
        case 0xd7b6U:
        case 0xd7baU:
        case 0xd7bcU:
        case 0xd7c2U:
        case 0xd7c6U:
        case 0xd7caU:
        case 0xd7ceU:
        case 0xd7d2U:
        case 0xd7d4U:
        case 0xd7d8U:
        case 0xd7deU:
        case 0xd7e4U:
        case 0xd7e6U:
        case 0xd7eaU:
        case 0xd7ecU:
        case 0xd7f0U:
        case 0xd7f2U:
        case 0xd7f8U:
        case 0xd7fcU:
        case 0xd800U:
        case 0xd802U:
        case 0xd806U:
        case 0xd80aU:
        case 0xd80eU:
        case 0xd810U:
        case 0xd812U:
        case 0xd816U:
        case 0xd81aU:
        case 0xd81cU:
        case 0xd820U:
        case 0xd822U:
        case 0xd828U:
        case 0xd82cU:
        case 0xd82eU:
        case 0xd830U:
        case 0xd832U:
        case 0xd834U:
        case 0xd838U:
        case 0xd83cU:
        case 0xd840U:
        case 0xd844U:
        case 0xd846U:
        case 0xd84aU:
        case 0xd850U:
        case 0xd852U:
        case 0xd858U:
        case 0xd85eU:
        case 0xd862U:
        case 0xd866U:
        case 0xd86aU:
        case 0xd86cU:
        case 0xd86eU:
        case 0xd872U:
        case 0xd874U:
        case 0xd876U:
        case 0xd878U:
        case 0xd87cU:
        case 0xd880U:
        case 0xd882U:
        case 0xd886U:
        case 0xd888U:
        case 0xd88cU:
        case 0xd892U:
        case 0xd894U:
        case 0xd896U:
        case 0xd89aU:
        case 0xd8a0U:
        case 0xd8a2U:
        case 0xd8a6U:
        case 0xd8a8U:
        case 0xd8aaU:
        case 0xd8aeU:
        case 0xd8b2U:
        case 0xd8b4U:
        case 0xd8b8U:
        case 0xd8baU:
        case 0xd8beU:
        case 0xd8c0U:
        case 0xd8c4U:
        case 0xd8c6U:
        case 0xd8caU:
        case 0xd8ccU:
        case 0xd8d2U:
        case 0xd8d4U:
        case 0xd8daU:
        case 0xd8deU:
        case 0xd8e2U:
        case 0xd8e6U:
        case 0xd8eaU:
        case 0xd8f0U:
        case 0xd8f4U:
        case 0xd8f6U:
        case 0xd8faU:
        case 0xd900U:
        case 0xd904U:
        case 0xd908U:
        case 0xd90cU:
        case 0xd90eU:
        case 0xd912U:
        case 0xd914U:
        case 0xd918U:
        case 0xd91aU:
        case 0xd91eU:
        case 0xd922U:
        case 0xd926U:
        case 0xd928U:
        case 0xd92cU:
        case 0xd92eU:
        case 0xd932U:
        case 0xd934U:
        case 0xd938U:
        case 0xd93aU:
        case 0xd93eU:
        case 0xd940U:
        case 0xd946U:
        case 0xd94aU:
        case 0xd94cU:
        case 0xd950U:
        case 0xd952U:
        case 0xd958U:
        case 0xd95cU:
        case 0xd960U:
        case 0xd964U:
        case 0xd968U:
        case 0xd96aU:
        case 0xd96eU:
        case 0xd974U:
        case 0xd976U:
        case 0xd97aU:
        case 0xd97eU:
        case 0xd982U:
        case 0xd984U:
        case 0xd98aU:
        case 0xd98eU:
        case 0xd994U:
        case 0xd998U:
        case 0xd99eU:
        case 0xd9a0U:
        case 0xd9a4U:
        case 0xd9a8U:
        case 0xd9aaU:
        case 0xd9b0U:
        case 0xd9b4U:
        case 0xd9b6U:
        case 0xd9baU:
        case 0xd9bcU:
        case 0xd9beU:
        case 0xd9c2U:
        case 0xd9c6U:
        case 0xd9c8U:
        case 0xd9ceU:
        case 0xd9d2U:
        case 0xd9d6U:
        case 0xd9d8U:
        case 0xd9daU:
        case 0xd9e0U:
        case 0xd9e4U:
        case 0xd9e8U:
        case 0xd9eaU:
        case 0xd9eeU:
        case 0xd9f0U:
        case 0xd9f6U:
        case 0xd9fcU:
        case 0xda00U:
        case 0xda04U:
        case 0xda08U:
        case 0xda0eU:
        case 0xda10U:
        case 0xda14U:
        case 0xda1aU:
        case 0xda1cU:
        case 0xda22U:
        case 0xda26U:
        case 0xda2aU:
        case 0xda2eU:
        case 0xda30U:
        case 0xda34U:
        case 0xda38U:
        case 0xda3cU:
        case 0xda3eU:
        case 0xda42U:
        case 0xda48U:
        case 0xda4cU:
        case 0xda50U:
        case 0xda56U:
        case 0xda5aU:
        case 0xda5cU:
        case 0xda5eU:
        case 0xda62U:
        case 0xda66U:
        case 0xda6aU:
        case 0xda6eU:
        case 0xda72U:
        case 0xda76U:
        case 0xda7aU:
        case 0xda7eU:
        case 0xda82U:
        case 0xda86U:
        case 0xda8aU:
        case 0xda8eU:
        case 0xda92U:
        case 0xda96U:
        case 0xda9aU:
        case 0xda9eU:
        case 0xdaa0U:
        case 0xdaa4U:
        case 0xdaa6U:
        case 0xdaaaU:
        case 0xdaacU:
        case 0xdab0U:
        case 0xdab2U:
        case 0xdab6U:
        case 0xdabcU:
        case 0xdabeU:
        case 0xdac0U:
        case 0xdac2U:
        case 0xdac4U:
        case 0xdac6U:
        case 0xdac8U:
        case 0xdacaU:
        case 0xdaccU:
        case 0xdad0U:
        case 0xdad4U:
        case 0xdad8U:
        case 0xdadaU:
        case 0xdadcU:
        case 0xdae2U:
        case 0xdae4U:
        case 0xdae8U:
        case 0xdaeeU:
        case 0xdaf0U:
        case 0xdaf2U:
        case 0xdaf6U:
        case 0xdaf8U:
        case 0xdafaU:
        case 0xdafcU:
        case 0xdb00U:
        case 0xdb02U:
        case 0xdb08U:
        case 0xdb0aU:
        case 0xdb10U:
        case 0xdb12U:
        case 0xdb18U:
        case 0xdb1cU:
        case 0xdb22U:
        case 0xdb24U:
        case 0xdb28U:
        case 0xdb2eU:
        case 0xdb30U:
        case 0xdb34U:
        case 0xdb3aU:
        case 0xdb3eU:
        case 0xdb40U:
        case 0xdb46U:
        case 0xdb48U:
        case 0xdb4cU:
        case 0xdb4eU:
        case 0xdb54U:
        case 0xdb58U:
        case 0xdb5cU:
        case 0xdb62U:
        case 0xdb64U:
        case 0xdb6aU:
        case 0xdb6eU:
        case 0xdb74U:
        case 0xdb76U:
        case 0xdb7cU:
        case 0xdb80U:
        case 0xdb84U:
        case 0xdb8aU:
        case 0xdb8eU:
        case 0xdb92U:
        case 0xdb94U:
        case 0xdb98U:
        case 0xdb9eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
