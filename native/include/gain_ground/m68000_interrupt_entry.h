#pragma once
#include "gain_ground/sound_caller_timing.h"
#include "gground_functions.h"
#include "gground_memory_map.h"
#include <optional>

namespace gain_ground {
// System 24 CPU-A supervisor/autovector entry. Source-backed, unverified.
// ReadWord returns optional<uint16_t>; WriteWord reports whether it resolved.
// Call only after the caller has accepted an unmasked interrupt. CPU-B's
// FD1094 transition and user-mode stack switching are separate contracts.
template <class ReadWord, class WriteWord>
FunctionResult enter_cpu_a_autovector(FunctionContext &context,
    std::uint8_t level, std::uint32_t resume, ReadWord read, WriteWord write)
{
    auto &r = context.registers;
    if (!context.host || context.cpu != 0U || context.state != 0xffU ||
        level == 0U || level > 7U || !(r.status & 0x2000U) || (r.address[7] & 1U))
        return {TranslationStatus::contract_violation, 0U, r.program_counter};

    SoundCallerTiming timing(context);
    timing.begin(r.program_counter,
        "CPU-A interrupt entry timing has no complete instruction observations");
    const auto saved_status = r.status;
    // state_interrupt: itlx1, itlx2, itlx3, then PC low at SSP-2.
    timing.clocks(2U);
    r.status = static_cast<std::uint16_t>((saved_status & 0x38ffU) |
        0x2000U | (static_cast<std::uint16_t>(level) << 8U));
    timing.clocks(4U);
    const auto frame = r.address[7] - 6U;
    if (!write(frame + 4U, static_cast<std::uint16_t>(resume)))
        return {TranslationStatus::contract_violation, 0U, frame + 4U};
    timing.clocks(4U);

    // Default MAME autovectors_map: VPA synchronizes to the 10-clock E
    // period (next period if phase >=7), adds one clock after access, and
    // then the CPU completes its four-clock acknowledge bus cycle.
    // CPU-A's runtime clock epoch is zero; its reference phase is unverified.
    const auto phase = (context.host->execution_time_ns() / 100U) % 10U;
    timing.clocks(static_cast<std::uint32_t>((phase < 7U ? 10U : 20U) - phase));
    const auto vector = static_cast<std::uint32_t>(24U + level) * 4U;
    timing.clocks(1U + 4U);
    timing.clocks(4U); // itlx6/itlx7 before the SR write.
    if (!write(frame, saved_status))
        return {TranslationStatus::contract_violation, 0U, frame};
    r.address[7] = frame;
    timing.clocks(4U);
    if (!write(frame + 2U, static_cast<std::uint16_t>(resume >> 16U)))
        return {TranslationStatus::contract_violation, 0U, frame + 2U};
    timing.clocks(4U);
    const auto high = read(vector);
    if (!high) return {TranslationStatus::contract_violation, 0U, vector};
    timing.clocks(4U);
    const auto low = read(vector + 2U);
    if (!low) return {TranslationStatus::contract_violation, 0U, vector + 2U};
    timing.clocks(4U);
    const auto target = (static_cast<std::uint32_t>(*high) << 16U) | *low;
    if (target & 1U)
        return {TranslationStatus::contract_violation, 0U, target};
    if (!read(target)) return {TranslationStatus::contract_violation, 0U, target};
    timing.clocks(4U);
    timing.clocks(2U); // trap9 between the two handler prefetches.
    if (!read(target + 2U)) return {TranslationStatus::contract_violation, 0U, target + 2U};
    timing.clocks(4U);
    r.program_counter = target;
    return FunctionResult::complete(0U, target);
}

// Resolve the actual BIOS/shared-RAM windows, including mirrors. An unknown
// target must not silently become a shared-RAM read. The generic machine
// supplies its own address resolver to enter_cpu_a_autovector above.
inline FunctionResult service_cpu_a_autovector(FunctionContext &context,
    std::uint8_t level, std::uint32_t site, std::uint32_t resume)
{
    if (!context.host)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto resolve = [&](std::uint32_t address, bool writing,
                       std::uint16_t &region, std::uint32_t &offset) {
        address &= 0x00ffffffU;
        for (const auto &window : generated::kMemoryWindows) {
            if (window.address_space != "program" || !(window.cpu_mask & 1U)) continue;
            const auto normalized = address & ~window.mirror;
            if (normalized < window.start || normalized > window.end) continue;
            offset = normalized - window.start;
            if (window.backing_store == "share1") { region = 3U; return true; }
            if (!writing && window.backing_store == "maincpu_rom") {
                region = window.start == 0U ? 1U : 4U;
                return true;
            }
            break;
        }
        return false;
    };
    const auto entered = enter_cpu_a_autovector(context, level, resume,
        [&](std::uint32_t address) -> std::optional<std::uint16_t> {
            std::uint16_t region{};
            std::uint32_t offset{};
            if (!resolve(address, false, region, offset)) return std::nullopt;
            return host.read_memory_word(region, offset, 0xffffU);
        },
        [&](std::uint32_t address, std::uint16_t value) {
            std::uint16_t region{};
            std::uint32_t offset{};
            if (!resolve(address, true, region, offset)) return false;
            host.write_memory_word(region, offset, value, 0xffffU);
            return true;
        });
    if (entered.status != TranslationStatus::complete) return entered;
    const auto target = entered.exit_program_counter;
    for (const auto &function : generated::kFunctions) {
        if (function.cpu != 0U || function.state != 0xffU || function.address != target)
            continue;
        const auto child = host.call_function(function.id, 0U, 0xffU, 3U, site, target, context);
        if (child.status != TranslationStatus::complete) return child;
        if (child.control == 9U) return child; // Stack reset: unwinding past the ISR.
        if (child.control != 2U || child.exit_program_counter != resume ||
            context.registers.program_counter != resume || context.state != 0xffU)
            return {TranslationStatus::contract_violation, child.control,
                context.registers.program_counter};
        return child;
    }
    return {TranslationStatus::contract_violation, 0U, target};
}
} // namespace gain_ground
