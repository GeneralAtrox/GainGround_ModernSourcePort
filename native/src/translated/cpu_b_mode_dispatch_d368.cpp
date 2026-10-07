// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"
#include "cpu_b_mode_dispatch_d368_detail.h"

namespace gain_ground::translated {
FunctionResult cpu_b_mode_dispatch_d368(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        bool returned_from_child = false;
        switch (pc) {
        case 0xd368U: { // 30380834: move-to-data-register
            next = 0xd36cU;
            t.prefetch(pc + 4U);
            const auto source = 0x834U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd36cU: { // e540: shift-word
            next = 0xd36eU;
            scene_timing::shift_word(t, m, pc, 0U, 2U, true, true);
            break;
        }
        case 0xd36eU: { // 4efb0002: jmp-pc-indexed
            next = 0xd372U;
            t.clocks(2U);
            const auto target = (0xd370U + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])) + 0x2U);
            t.clocks(4U); t.prefetch(target); t.prefetch(target + 2U);
            next = target; transfer_kind = 1U;
            break;
        }
        case 0xd372U: { // 6000000e: bcc
            next = 0xd376U;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch_word(pc, 0xd382U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xd376U: { // 600000fa: bcc
            next = 0xd37aU;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch_word(pc, 0xd472U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xd37aU: { // 600001b2: bcc
            next = 0xd37eU;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch_word(pc, 0xd52eU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xd37eU: { // 60000264: bcc
            next = 0xd382U;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch_word(pc, 0xd5e4U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xd382U: { // 2b7c0000d3940002: move-long-register-or-immediate-to-memory
            next = 0xd38aU;
            const auto value = 0xd394U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = (r.address[5] + 0x2U);
            t.prefetch(pc + 8U);
            scene_timing::nz_high(r, static_cast<std::uint16_t>(value >> 16U));
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            m.logic(value, 32U);
            t.prefetch(pc + 10U);
            break;
        }
        case 0xd38aU: { // 30380c02: move-to-data-register
            next = 0xd38eU;
            t.prefetch(pc + 4U);
            const auto source = 0xc02U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd38eU: { // 5240: quick-register
            next = 0xd390U;
            const auto value = m.add(r.data[0], 1U, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xd390U: { // 31c08006: move-to-displacement
            next = 0xd394U;
            const auto destination = 0xffff8006U;
            m.logic(r.data[0], 16U);
            t.prefetch(pc + 4U);
            t.word(destination, r.data[0]);
            t.prefetch(pc + 6U);
            break;
        }
        default:
            if (!cpu_b_mode_dispatch_d368_detail::dispatch_alternate_paths(
                    c, r, m, t, pc, next, transfer_kind))
                return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (!returned_from_child)
            if (const auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0xd368U:
        case 0xd36cU:
        case 0xd36eU:
        case 0xd372U:
        case 0xd376U:
        case 0xd37aU:
        case 0xd37eU:
        case 0xd382U:
        case 0xd38aU:
        case 0xd38eU:
        case 0xd390U:
        case 0xd472U:
        case 0xd476U:
        case 0xd47aU:
        case 0xd47eU:
        case 0xd482U:
        case 0xd486U:
        case 0xd48aU:
        case 0xd48eU:
        case 0xd490U:
        case 0xd492U:
        case 0xd494U:
        case 0xd49cU:
        case 0xd4a2U:
        case 0xd4a6U:
        case 0xd4aaU:
        case 0xd52eU:
        case 0xd536U:
        case 0xd53aU:
        case 0xd53cU:
        case 0xd542U:
        case 0xd544U:
        case 0xd548U:
        case 0xd5e4U:
        case 0xd5eaU:
        case 0xd5f0U:
        case 0xd5f4U:
        case 0xd5f8U:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
