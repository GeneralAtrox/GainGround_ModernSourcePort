// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"
#include "cpu_b_build_sprite_lists_detail.h"

namespace gain_ground::translated {
FunctionResult cpu_b_build_sprite_lists(FunctionContext &c) noexcept {
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
        case 0x16f6eU: { // 4e75: rts
            next = 0x16f70U;
            return t.rts(pc);
            break;
        }
        default:
            if (cpu_b_build_sprite_lists_detail::dispatch_pc_range_01(
                    c, r, m, t, pc, next, transfer_kind))
                break;
            if (cpu_b_build_sprite_lists_detail::dispatch_pc_range_02(
                    c, r, m, t, pc, next, transfer_kind))
                break;
            if (cpu_b_build_sprite_lists_detail::dispatch_pc_range_03(
                    c, r, m, t, pc, next, transfer_kind))
                break;
            if (cpu_b_build_sprite_lists_detail::dispatch_pc_range_04(
                    c, r, m, t, pc, next, transfer_kind))
                break;
            if (cpu_b_build_sprite_lists_detail::dispatch_pc_range_05(
                    c, r, m, t, pc, next, transfer_kind))
                break;
            if (cpu_b_build_sprite_lists_detail::dispatch_pc_range_06(
                    c, r, m, t, pc, next, transfer_kind))
                break;
            return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (!returned_from_child)
            if (const auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x16d16U:
        case 0x16d1aU:
        case 0x16d20U:
        case 0x16d22U:
        case 0x16d24U:
        case 0x16d26U:
        case 0x16d2aU:
        case 0x16d2eU:
        case 0x16d32U:
        case 0x16d34U:
        case 0x16d38U:
        case 0x16d3aU:
        case 0x16d3eU:
        case 0x16d42U:
        case 0x16d44U:
        case 0x16d46U:
        case 0x16d4aU:
        case 0x16d4cU:
        case 0x16d4eU:
        case 0x16d52U:
        case 0x16d54U:
        case 0x16d56U:
        case 0x16d58U:
        case 0x16d5cU:
        case 0x16d5eU:
        case 0x16d64U:
        case 0x16d68U:
        case 0x16d6aU:
        case 0x16d6cU:
        case 0x16d72U:
        case 0x16d76U:
        case 0x16d78U:
        case 0x16d7aU:
        case 0x16d7eU:
        case 0x16d82U:
        case 0x16d84U:
        case 0x16d86U:
        case 0x16d88U:
        case 0x16d8aU:
        case 0x16d8cU:
        case 0x16d8eU:
        case 0x16d92U:
        case 0x16d96U:
        case 0x16d98U:
        case 0x16d9aU:
        case 0x16d9eU:
        case 0x16da0U:
        case 0x16da2U:
        case 0x16da4U:
        case 0x16da6U:
        case 0x16da8U:
        case 0x16dacU:
        case 0x16daeU:
        case 0x16db2U:
        case 0x16db4U:
        case 0x16db8U:
        case 0x16dbcU:
        case 0x16dc0U:
        case 0x16dc2U:
        case 0x16dc4U:
        case 0x16dcaU:
        case 0x16dccU:
        case 0x16dd0U:
        case 0x16dd2U:
        case 0x16dd4U:
        case 0x16dd6U:
        case 0x16dd8U:
        case 0x16ddaU:
        case 0x16ddcU:
        case 0x16de0U:
        case 0x16de2U:
        case 0x16de4U:
        case 0x16de6U:
        case 0x16de8U:
        case 0x16deaU:
        case 0x16decU:
        case 0x16df0U:
        case 0x16df2U:
        case 0x16df6U:
        case 0x16df8U:
        case 0x16dfcU:
        case 0x16e00U:
        case 0x16e04U:
        case 0x16e06U:
        case 0x16e08U:
        case 0x16e0eU:
        case 0x16e10U:
        case 0x16e14U:
        case 0x16e16U:
        case 0x16e18U:
        case 0x16e1aU:
        case 0x16e1cU:
        case 0x16e20U:
        case 0x16e24U:
        case 0x16e28U:
        case 0x16e2aU:
        case 0x16e2cU:
        case 0x16e30U:
        case 0x16e34U:
        case 0x16e36U:
        case 0x16e38U:
        case 0x16e3cU:
        case 0x16e3eU:
        case 0x16e40U:
        case 0x16e42U:
        case 0x16e44U:
        case 0x16e48U:
        case 0x16e4aU:
        case 0x16e4eU:
        case 0x16e50U:
        case 0x16e54U:
        case 0x16e58U:
        case 0x16e5cU:
        case 0x16e5eU:
        case 0x16e60U:
        case 0x16e66U:
        case 0x16e68U:
        case 0x16e6cU:
        case 0x16e6eU:
        case 0x16e70U:
        case 0x16e72U:
        case 0x16e74U:
        case 0x16e76U:
        case 0x16e78U:
        case 0x16e7cU:
        case 0x16e7eU:
        case 0x16e80U:
        case 0x16e82U:
        case 0x16e84U:
        case 0x16e86U:
        case 0x16e8aU:
        case 0x16e8cU:
        case 0x16e90U:
        case 0x16e92U:
        case 0x16e96U:
        case 0x16e9aU:
        case 0x16e9eU:
        case 0x16ea0U:
        case 0x16ea2U:
        case 0x16ea8U:
        case 0x16eaaU:
        case 0x16eaeU:
        case 0x16eb0U:
        case 0x16eb2U:
        case 0x16eb4U:
        case 0x16eb6U:
        case 0x16eb8U:
        case 0x16ebaU:
        case 0x16ebeU:
        case 0x16ec2U:
        case 0x16ec4U:
        case 0x16ec8U:
        case 0x16eccU:
        case 0x16ed2U:
        case 0x16ed6U:
        case 0x16ed8U:
        case 0x16edcU:
        case 0x16ee0U:
        case 0x16ee6U:
        case 0x16eeaU:
        case 0x16eeeU:
        case 0x16ef2U:
        case 0x16ef6U:
        case 0x16ef8U:
        case 0x16efcU:
        case 0x16efeU:
        case 0x16f00U:
        case 0x16f02U:
        case 0x16f04U:
        case 0x16f06U:
        case 0x16f08U:
        case 0x16f0aU:
        case 0x16f0cU:
        case 0x16f0eU:
        case 0x16f10U:
        case 0x16f12U:
        case 0x16f14U:
        case 0x16f16U:
        case 0x16f18U:
        case 0x16f1cU:
        case 0x16f20U:
        case 0x16f22U:
        case 0x16f24U:
        case 0x16f28U:
        case 0x16f2aU:
        case 0x16f2cU:
        case 0x16f30U:
        case 0x16f34U:
        case 0x16f36U:
        case 0x16f38U:
        case 0x16f3cU:
        case 0x16f40U:
        case 0x16f44U:
        case 0x16f48U:
        case 0x16f4aU:
        case 0x16f4cU:
        case 0x16f4eU:
        case 0x16f52U:
        case 0x16f56U:
        case 0x16f5aU:
        case 0x16f5cU:
        case 0x16f5eU:
        case 0x16f62U:
        case 0x16f66U:
        case 0x16f68U:
        case 0x16f6aU:
        case 0x16f6eU:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
