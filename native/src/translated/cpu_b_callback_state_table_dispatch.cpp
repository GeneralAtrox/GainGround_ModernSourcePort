// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"
#include "cpu_b_initials_entry.h"
#include "cpu_b_callback_state_table_dispatch_detail.h"

namespace gain_ground::translated {
FunctionResult cpu_b_callback_state_table_dispatch(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        if (is_initials_entry_pc(r.program_counter)) return cpu_b_initials_entry(c);
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xeec8U: { // 61000afa bsr.w $f9c4
            next = 0xeeccU;
            const auto result = m.call(c, 180U, 0xeec8U, 0xf9c4U, 0xeeccU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xeeccU: { // 61000bc4 bsr.w $fa92
            next = 0xeed0U;
            const auto result = m.call(c, 183U, 0xeeccU, 0xfa92U, 0xeed0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xeed2U: { // 4e75 rts 
            next = 0xeed4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xeed2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xef0eU: { // 4e75 rts 
            next = 0xef10U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xef0eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xef2aU: { // 4e75 rts 
            next = 0xef2cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xef2aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xef2cU: { // 61000c16 bsr.w $fb44
            next = 0xef30U;
            const auto result = m.call(c, 187U, 0xef2cU, 0xfb44U, 0xef30U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xef4cU: { // 61000318 bsr.w $f266
            next = 0xef50U;
            const auto result = m.call(c, 176U, 0xef4cU, 0xf266U, 0xef50U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xef7eU: { // 4eb900015fd4 jsr $15fd4.l
            next = 0xef84U;
            const auto result = m.call(c, 290U, 0xef7eU, 0x15fd4U, 0xef84U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xef94U: { // 4e75 rts 
            next = 0xef96U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xef94U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xefa8U: { // 4eb900016234 jsr $16234.l
            next = 0xefaeU;
            const auto result = m.call(c, 299U, 0xefa8U, 0x16234U, 0xefaeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xefb6U: { // 4eb90001610e jsr $1610e.l
            next = 0xefbcU;
            const auto result = m.call(c, 296U, 0xefb6U, 0x1610eU, 0xefbcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xefc0U: { // 4eb900015fd4 jsr $15fd4.l
            next = 0xefc6U;
            const auto result = m.call(c, 290U, 0xefc0U, 0x15fd4U, 0xefc6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xeff4U: { // 4eb9000161ea jsr $161ea.l
            next = 0xeffaU;
            const auto result = m.call(c, 298U, 0xeff4U, 0x161eaU, 0xeffaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf002U: { // 4eb900015fde jsr $15fde.l
            next = 0xf008U;
            const auto result = m.call(c, 540U, 0xf002U, 0x15fdeU, 0xf008U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf038U: { // 61000266 bsr.w $f2a0
            next = 0xf03cU;
            const auto result = m.call(c, 526U, 0xf038U, 0xf2a0U, 0xf03cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf054U: { // 6100025e bsr.w $f2b4
            next = 0xf058U;
            const auto result = m.call(c, 527U, 0xf054U, 0xf2b4U, 0xf058U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf058U: { // 4e75 rts 
            next = 0xf05aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf058U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xf07eU: { // 61000a9e bsr.w $fb1e
            next = 0xf082U;
            const auto result = m.call(c, 185U, 0xf07eU, 0xfb1eU, 0xf082U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf082U: { // 61000aee bsr.w $fb72
            next = 0xf086U;
            const auto result = m.call(c, 188U, 0xf082U, 0xfb72U, 0xf086U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf092U: { // 4e75 rts 
            next = 0xf094U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf092U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xf264U: { // 4e75 rts 
            next = 0xf266U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf264U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xf3acU: { // 083800030833 btst.b #$3, $833.w
            next = 0xf3b2U;
            const auto destination_address = 0x833U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0xf3b2U: { // 6704 beq.b $f3b8
            next = 0xf3b4U;
            if ((r.status & 4U) != 0U) { next = 0xf3b8U; transfer_kind = 1U; }
            break;
        }
        case 0xf3b4U: { // 61000670 bsr.w $fa26
            const auto result = m.call(c, 181U, pc, 0xfa26U, 0xf3b8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; break;
        }
        case 0xf3b8U: { // 6100078a bsr.w $fb44
            next = 0xf3bcU;
            const auto result = m.call(c, 187U, 0xf3b8U, 0xfb44U, 0xf3bcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf3bcU: { // 61000048 bsr.w $f406
            next = 0xf3c0U;
            const auto result = m.call(c, 179U, 0xf3bcU, 0xf406U, 0xf3c0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        default:
            if (cpu_b_callback_state_table_dispatch_detail::dispatch_table_region_01(
                    c, r, m, pc, next, transfer_kind))
                break;
            if (cpu_b_callback_state_table_dispatch_detail::dispatch_table_region_02(
                    c, r, m, pc, next, transfer_kind))
                break;
            if (cpu_b_callback_state_table_dispatch_detail::dispatch_table_region_03(
                    c, r, m, pc, next, transfer_kind))
                break;
            if (cpu_b_callback_state_table_dispatch_detail::dispatch_table_region_04(
                    c, r, m, pc, next, transfer_kind))
                break;
            if (cpu_b_callback_state_table_dispatch_detail::dispatch_table_region_05(
                    c, r, m, pc, next, transfer_kind))
                break;
            return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0xf3c0U) return c.host->call_function(178U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0xee9eU:
        case 0xeea2U:
        case 0xeea6U:
        case 0xeeaaU:
        case 0xeeaeU:
        case 0xeeb0U:
        case 0xeeb4U:
        case 0xeeb8U:
        case 0xeebcU:
        case 0xeec0U:
        case 0xeec6U:
        case 0xeec8U:
        case 0xeeccU:
        case 0xeed0U:
        case 0xeed2U:
        case 0xeed4U:
        case 0xeed8U:
        case 0xeedaU:
        case 0xeedcU:
        case 0xeee2U:
        case 0xeee8U:
        case 0xeeecU:
        case 0xeef0U:
        case 0xeef2U:
        case 0xeef6U:
        case 0xeef8U:
        case 0xeefcU:
        case 0xef00U:
        case 0xef04U:
        case 0xef06U:
        case 0xef0aU:
        case 0xef0eU:
        case 0xef10U:
        case 0xef16U:
        case 0xef1cU:
        case 0xef22U:
        case 0xef26U:
        case 0xef2aU:
        case 0xef2cU:
        case 0xef30U:
        case 0xef34U:
        case 0xef36U:
        case 0xef3aU:
        case 0xef3eU:
        case 0xef42U:
        case 0xef46U:
        case 0xef4aU:
        case 0xef4cU:
        case 0xef50U:
        case 0xef56U:
        case 0xef58U:
        case 0xef5cU:
        case 0xef60U:
        case 0xef62U:
        case 0xef66U:
        case 0xef6aU:
        case 0xef6cU:
        case 0xef6eU:
        case 0xef70U:
        case 0xef72U:
        case 0xef74U:
        case 0xef76U:
        case 0xef7aU:
        case 0xef7eU:
        case 0xef84U:
        case 0xef8aU:
        case 0xef90U:
        case 0xef94U:
        case 0xef96U:
        case 0xef9aU:
        case 0xef9cU:
        case 0xef9eU:
        case 0xefa0U:
        case 0xefa2U:
        case 0xefa6U:
        case 0xefa8U:
        case 0xefaeU:
        case 0xefb0U:
        case 0xefb2U:
        case 0xefb6U:
        case 0xefbcU:
        case 0xefc0U:
        case 0xefc6U:
        case 0xefc8U:
        case 0xefccU:
        case 0xefd0U:
        case 0xefd6U:
        case 0xefdaU:
        case 0xefe0U:
        case 0xefe4U:
        case 0xefe6U:
        case 0xefe8U:
        case 0xefecU:
        case 0xeff0U:
        case 0xeff4U:
        case 0xeffaU:
        case 0xeffcU:
        case 0xf000U:
        case 0xf002U:
        case 0xf008U:
        case 0xf00eU:
        case 0xf012U:
        case 0xf014U:
        case 0xf01aU:
        case 0xf020U:
        case 0xf026U:
        case 0xf02cU:
        case 0xf032U:
        case 0xf038U:
        case 0xf03cU:
        case 0xf042U:
        case 0xf046U:
        case 0xf048U:
        case 0xf04cU:
        case 0xf04eU:
        case 0xf054U:
        case 0xf058U:
        case 0xf05aU:
        case 0xf05eU:
        case 0xf060U:
        case 0xf066U:
        case 0xf068U:
        case 0xf06cU:
        case 0xf070U:
        case 0xf072U:
        case 0xf076U:
        case 0xf07aU:
        case 0xf07eU:
        case 0xf082U:
        case 0xf086U:
        case 0xf08aU:
        case 0xf08cU:
        case 0xf092U:
        case 0xf094U:
        case 0xf23eU:
        case 0xf242U:
        case 0xf246U:
        case 0xf248U:
        case 0xf24aU:
        case 0xf24cU:
        case 0xf24eU:
        case 0xf252U:
        case 0xf254U:
        case 0xf258U:
        case 0xf25eU:
        case 0xf264U:
        case 0xf3acU:
        case 0xf3b2U:
        case 0xf3b4U:
        case 0xf3b8U:
        case 0xf3bcU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
