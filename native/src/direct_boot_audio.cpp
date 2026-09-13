#include "gain_ground/direct_boot_audio.h"
#include "gain_ground/system24_devices.h"
#include "translated/cpu_b_scene_timing.h"

namespace gain_ground {
void prepare_direct_boot_devices(System24Devices &devices)
{
    // BIOS 0x410 and 0x420: release active-low YM reset and enable DAC output.
    // These are initial state for the approved instantaneous loader.
    devices.write(0x80001cU, 4U, 0x00ffU);
    // Fast loading starts playback here. Preload the DAC's unsigned midpoint
    // before enabling its port, avoiding a pulse from the reset latch's zero.
    // The retained BIOS sound initialization writes this same midpoint later.
    devices.write(0x80000eU, 0x80U, 0x00ffU);
    devices.write(0x80001eU, 0x88U, 0x00ffU);
}
FunctionResult run_direct_boot_audio(FunctionContext &c) noexcept
{
    if (!c.host || c.cpu != 0U || c.state != 0xffU ||
        c.registers.status != 0x2700U || c.registers.address[7] != 0U)
        return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    auto &r = c.registers;
    r.program_counter = 0x4f0U;
    translated::unverified::Machine m{*c.host, r, c.cpu, c.state};
    translated::CpuBIrqTiming t(c, m);
    // BIOS bytes 61002f82 and 61002f6e: both BSR.W instructions have
    // 18 ordinary clocks. Children own their original table/writer timing.
    for (const auto site : {0x4f0U, 0x4f4U}) {
        const auto target = site == 0x4f0U ? 0x3474U : 0x3464U;
        const auto next = site + 4U;
        const auto result = translated::scene_timing::call(
            c, t, m, site, target, next, translated::scene_timing::CallForm::bsr);
        if (result.status != TranslationStatus::complete || result.control != 1U)
            return result;
        if (r.program_counter != next || r.address[7] != 0U || (r.status & 0xff00U) != 0x2700U)
            return {TranslationStatus::contract_violation, result.control, r.program_counter};
        if (site == 0x4f0U) t.begin(next);
    }
    return FunctionResult::complete(1U, r.program_counter);
}
}
