// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

#include <optional>

namespace gain_ground::translated {
namespace {
struct CpuAResetEntryStep {
    bool handled = false;
    std::optional<FunctionResult> result;
};
#include "cpu_a_reset_entry_pc_000408_000480.inc" // gground-source-split
#include "cpu_a_reset_entry_pc_000480_000500.inc" // gground-source-split
#include "cpu_a_reset_entry_pc_000500_000600.inc" // gground-source-split
#include "cpu_a_reset_entry_pc_000600_000680.inc" // gground-source-split
#include "cpu_a_reset_entry_pc_000680_000700.inc" // gground-source-split
#include "cpu_a_reset_entry_pc_0023c4_002400.inc" // gground-source-split
#include "cpu_a_reset_entry_pc_002400_002500.inc" // gground-source-split
}

FunctionResult cpu_a_reset_entry(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 0U, 255U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        auto step = handle_cpu_a_reset_entry_pc_000408_000480(c, r, m, pc, next, transfer_kind);
        if (!step.handled)
            step = handle_cpu_a_reset_entry_pc_000480_000500(c, r, m, pc, next, transfer_kind);
        if (!step.handled)
            step = handle_cpu_a_reset_entry_pc_000500_000600(c, r, m, pc, next, transfer_kind);
        if (!step.handled)
            step = handle_cpu_a_reset_entry_pc_000600_000680(c, r, m, pc, next, transfer_kind);
        if (!step.handled)
            step = handle_cpu_a_reset_entry_pc_000680_000700(c, r, m, pc, next, transfer_kind);
        if (!step.handled)
            step = handle_cpu_a_reset_entry_pc_0023c4_002400(c, r, m, pc, next);
        if (!step.handled)
            step = handle_cpu_a_reset_entry_pc_002400_002500(c, r, m, pc, next);
        if (!step.handled)
            return {TranslationStatus::contract_violation, 0U, pc};
        if (step.result) return *step.result;
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0x800c0U) return c.host->call_function(54U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0x408U:
        case 0x40cU:
        case 0x410U:
        case 0x418U:
        case 0x420U:
        case 0x428U:
        case 0x430U:
        case 0x438U:
        case 0x440U:
        case 0x448U:
        case 0x450U:
        case 0x458U:
        case 0x45cU:
        case 0x460U:
        case 0x462U:
        case 0x46aU:
        case 0x472U:
        case 0x47aU:
        case 0x482U:
        case 0x484U:
        case 0x486U:
        case 0x488U:
        case 0x48aU:
        case 0x48cU:
        case 0x48eU:
        case 0x490U:
        case 0x492U:
        case 0x496U:
        case 0x498U:
        case 0x49aU:
        case 0x49cU:
        case 0x49eU:
        case 0x4aaU:
        case 0x4acU:
        case 0x4b0U:
        case 0x4b4U:
        case 0x4b8U:
        case 0x4bcU:
        case 0x4beU:
        case 0x4c2U:
        case 0x4c6U:
        case 0x4c8U:
        case 0x4ceU:
        case 0x4d2U:
        case 0x4d6U:
        case 0x4daU:
        case 0x4deU:
        case 0x4e2U:
        case 0x4e4U:
        case 0x4eaU:
        case 0x4ecU:
        case 0x4f0U:
        case 0x4f4U:
        case 0x4f8U:
        case 0x4fcU:
        case 0x504U:
        case 0x506U:
        case 0x50aU:
        case 0x510U:
        case 0x512U:
        case 0x518U:
        case 0x51aU:
        case 0x51cU:
        case 0x522U:
        case 0x5a0U:
        case 0x5a6U:
        case 0x5a8U:
        case 0x5aeU:
        case 0x5b0U:
        case 0x5b2U:
        case 0x5b8U:
        case 0x5baU:
        case 0x5c2U:
        case 0x5c6U:
        case 0x5ceU:
        case 0x5d2U:
        case 0x5d6U:
        case 0x5daU:
        case 0x5deU:
        case 0x5e2U:
        case 0x5e6U:
        case 0x5eaU:
        case 0x5f0U:
        case 0x5f2U:
        case 0x5f4U:
        case 0x5f6U:
        case 0x5faU:
        case 0x5fcU:
        case 0x600U:
        case 0x604U:
        case 0x608U:
        case 0x60cU:
        case 0x612U:
        case 0x618U:
        case 0x61cU:
        case 0x61eU:
        case 0x622U:
        case 0x62aU:
        case 0x62eU:
        case 0x632U:
        case 0x634U:
        case 0x638U:
        case 0x63cU:
        case 0x640U:
        case 0x642U:
        case 0x646U:
        case 0x64cU:
        case 0x652U:
        case 0x654U:
        case 0x656U:
        case 0x65aU:
        case 0x65cU:
        case 0x660U:
        case 0x666U:
        case 0x668U:
        case 0x66aU:
        case 0x66eU:
        case 0x672U:
        case 0x676U:
        case 0x67cU:
        case 0x67eU:
        case 0x680U:
        case 0x684U:
        case 0x688U:
        case 0x68cU:
        case 0x692U:
        case 0x694U:
        case 0x696U:
        case 0x69aU:
        case 0x69eU:
        case 0x6a0U:
        case 0x6a2U:
        case 0x6a4U:
        case 0x6a6U:
        case 0x6a8U:
        case 0x6aeU:
        case 0x6b2U:
        case 0x6b4U:
        case 0x6b6U:
        case 0x6b8U:
        case 0x6baU:
        case 0x6beU:
        case 0x6c0U:
        case 0x6c2U:
        case 0x6c6U:
        case 0x6c8U:
        case 0x6caU:
        case 0x6ccU:
        case 0x6ceU:
        case 0x6d0U:
        case 0x6d2U:
        case 0x6d4U:
        case 0x6d6U:
        case 0x6d8U:
        case 0x6daU:
        case 0x6dcU:
        case 0x6deU:
        case 0x6e0U:
        case 0x6e2U:
        case 0x6e4U:
        case 0x6e6U:
        case 0x6e8U:
        case 0x6eaU:
        case 0x6ecU:
        case 0x6f0U:
        case 0x23c4U:
        case 0x23c8U:
        case 0x23d0U:
        case 0x23d8U:
        case 0x23dcU:
        case 0x23e0U:
        case 0x23e8U:
        case 0x23f0U:
        case 0x23f8U:
        case 0x23fcU:
        case 0x2400U:
        case 0x2406U:
        case 0x2408U:
        case 0x240cU:
        case 0x240eU:
        case 0x2412U:
        case 0x2414U:
        case 0x2418U:
        case 0x241cU:
        case 0x2420U:
        case 0x2424U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

} // namespace gain_ground::translated
