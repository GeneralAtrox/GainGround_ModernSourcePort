#include "gain_ground/runtime_host.h"
#include <cstdio>
#include <cstdlib>
#include <optional>
#include "gain_ground/direct_asset_loader.h"
#include "gain_ground/system24_devices.h"
#include "gain_ground/native_function_registry.h"
#include "gground_fixture_contract.h"
#include "gground_functions.h"
#include "gground_memory_map.h"
#include "translated/cpu_b_irq_timing.h"
#include "gameplay/legacy_gameplay_bridge.h"
#include "gameplay/legacy_enemy_bridge.h"
#include <algorithm>


namespace gain_ground {
void RuntimeHost::select_cpu(unsigned cpu) noexcept
{
    suspended_[selected_cpu_] = {active_, invocation_, depth_, waiting_};
    selected_cpu_ = cpu;
    active_ = suspended_[cpu].context;
    invocation_ = suspended_[cpu].invocation;
    depth_ = suspended_[cpu].depth;
    waiting_ = suspended_[cpu].waiting;
}

void RuntimeHost::prepare_sega_logo()
{
    // Original BIOS writes at 0x5d2..0x688, isolated from disk timing.
    for (unsigned id = 5; id <= 11; ++id) {
        std::fill(regions_[id].bytes.begin(), regions_[id].bytes.end(), 0U);
        std::fill(regions_[id].known_bits.begin(), regions_[id].known_bits.end(), 0xffU);
    }
    const auto rom = region_bytes(1);
    load_region(8U, 0x20U, rom.subspan(0x7380U, 0xdc0U * 4U));
    for (unsigned group = 0; group < 3; ++group)
        for (unsigned repeat = 0; repeat < 32; ++repeat)
            load_region(9U, (group * 32 + repeat) * 32, rom.subspan(0x76eU + group * 32U, 32));
    std::uint32_t source = 0xaa80U;
    const auto rectangle = [&](unsigned destination, unsigned columns, unsigned rows, unsigned bias) {
        for (unsigned y = 0; y < rows; ++y) for (unsigned x = 0; x < columns; ++x) {
            const auto value = static_cast<std::uint16_t>(((rom[source] << 8U) | rom[source + 1]) + 1U + bias);
            source += 2;
            write_memory_word(5, destination + y * 128 + x * 2, value, 0xffff);
        }
    };
    rectangle(0x441a, 36, 13, 0);
    rectangle(0x4d8a, 51, 6, 0x1000);
    rectangle(0x539a, 37, 2, 0x2000);
}
namespace {
struct LoadTarget { std::uint16_t region{}; std::uint32_t offset{}, available{}; };
LoadTarget load_target(std::uint32_t address) noexcept
{
    address &= 0xffffffU;
    for (const auto &window : generated::kMemoryWindows) {
        if (window.address_space != "program" || !(window.cpu_mask & 1U)) continue;
        const auto normalized = address & ~window.mirror;
        if (normalized < window.start || normalized > window.end) continue;
        auto offset = normalized - window.start;
        const auto available = window.end - normalized + 1U;
        const auto store = window.backing_store;
        if (store == "subcpu") return {2U, offset, available};
        if (store == "share1") return {3U, offset, available};
        if (store == "character_ram") return {8U, offset, available};
        if (store == "palette_ram") return {9U, offset, available};
        if (store == "sprite_ram") return {11U, offset, available};
        if (store == "tile_ram") {
            if (offset < 0x8000U) return {5U, offset, 0x8000U - offset};
            if (offset < 0xc000U) return {6U, offset - 0x8000U, 0xc000U - offset};
            return {7U, offset - 0xc000U, 0x10000U - offset};
        }
        return {}; // ROM and device registers are not bulk-load destinations.
    }
    return {};
}
} // namespace

bool RuntimeHost::can_load_bus(std::uint32_t address, std::uint32_t bytes) const noexcept
{
    const auto target = load_target(address);
    return target.region != 0U && bytes <= target.available &&
        !regions_[target.region].read_only && target.offset <= regions_[target.region].bytes.size() &&
        bytes <= regions_[target.region].bytes.size() - target.offset;
}

bool RuntimeHost::load_bus(std::uint32_t address, std::span<const std::uint8_t> bytes)
{
    if (bytes.size() > 0x1000000U || !can_load_bus(address, static_cast<std::uint32_t>(bytes.size()))) return false;
    const auto target = load_target(address);
    return load_region(target.region, target.offset, bytes);
}

bool RuntimeHost::prepare_direct_boot(DirectAssetLoader &assets, FunctionContext &context)
{
    if (active_ || faulted() || !assets.ready()) return false;
    // This is a new loading policy, not a claim about physical power-on RAM.
    // Keep BIOS ROM identity/initialization supplied by the executable intact.
    for (auto &region : regions_) {
        if (region.read_only) continue;
        std::fill(region.bytes.begin(), region.bytes.end(), 0U);
        std::fill(region.known_bits.begin(), region.known_bits.end(), 0xffU);
    }
    if (!assets.load_boot(*this)) return false;
    assets_ = &assets;
    // Original reset tail 0x6cc..0x6f0 clears D0-D7/A0-A7, sets SR=2700
    // and jumps to 0x800c0. No captured CPU context is injected.
    context = {};
    context.host = this;
    context.cpu = 0U;
    context.state = 0xffU;
    context.registers.status = 0x2700U;
    context.registers.program_counter = 0x800c0U;
    return true;
}

FunctionResult RuntimeHost::direct_load_trap(std::uint32_t id, FunctionContext &c)
{
    auto &r = c.registers;
    // Approved direct transfers are instantaneous emulated operations. Keep
    // their memory-copy checkpoints from consuming scheduler/device time.
    // The real TRAP entry is timed by its caller; RTE below owns its 20 clocks.
    M68000Timing direct_transfer(*this);
    // TRAP #10 ABI at the original initial/dynamic load-table callers:
    // D0 = logical disk offset, D1 = byte count, A1 = destination.
    // TRAP #8 only operated the drive. Its replacement has no device effects.
    // TRAP #6/#7 read/write the saved settings record, independently of the
    // immutable game-asset load tables. Missing session data takes the original
    // carry-set fallback at 0x80258, which initializes defaults at 0x80268.
    // New adapters preserve general registers and return through the original
    // six-byte exception frame. Disk scratch/clobbers are omitted.
    const bool settings_trap = id == 50U || id == 51U;
    bool settings_available = false;
    if (settings_trap) {
        // Original callers 0x80256 and 0x8038c use this exact record and size.
        // Do not mistake D0 (a drive selector here) for a logical asset offset.
        if ((r.address[1] & 0xffffffU) != 0xf07b20U || r.data[1] != saved_settings_.size()) {
            fail("Direct settings transfer has no address/length mapping", r.address[1]);
            return {TranslationStatus::contract_violation, 0U, r.program_counter};
        }
        if (id == 51U) {
            auto pending = saved_settings_;
            for (std::size_t i = 0; i < pending.size(); i += 2U) {
                const auto value = read_memory_word(2U, 0x7b20U + static_cast<std::uint32_t>(i), 0xffffU);
                pending[i] = static_cast<std::uint8_t>(value >> 8U);
                pending[i + 1U] = static_cast<std::uint8_t>(value);
            }
            if (faulted()) return {TranslationStatus::contract_violation, 0U, r.program_counter};
            saved_settings_ = pending;
            has_saved_settings_ = true;
        } else if (has_saved_settings_ && !load_bus(r.address[1], saved_settings_)) {
            fail("Direct settings destination is not writable native memory", r.address[1]);
            return {TranslationStatus::contract_violation, 0U, r.program_counter};
        }
        settings_available = has_saved_settings_;
    }
    if (id == 53U && !assets_->transfer(*this, r.data[0], r.address[1], r.data[1])) {
        fail("Direct asset load failed; inspect loader error", r.address[1]);
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    }
    const auto sp = r.address[7];
    if (id == 50U)
        write_memory_word(3U, sp & 0x3ffffU, settings_available ? 0U : 1U, 0x00ffU);
    if (faulted()) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    translated::unverified::Machine machine{*this, r, c.cpu, c.state};
    translated::CpuBIrqTiming timing(c, machine);
    return timing.rte();
}

} // namespace gain_ground
