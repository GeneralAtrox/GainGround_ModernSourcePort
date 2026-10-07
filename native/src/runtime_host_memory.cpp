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
RuntimeHost::Region *RuntimeHost::region_at(std::uint16_t id, std::uint32_t offset)
{
    if (id == 4U) id = 1U;
    if (id == 0U || id >= regions_.size() || (offset & 1U) != 0U ||
        regions_[id].bytes.size() < 2U || offset >= regions_[id].bytes.size() - 1U) {
        fail("Unmapped or unaligned native memory access", offset, id);
        return nullptr;
    }
    return &regions_[id];
}

bool RuntimeHost::load_region(std::uint16_t id, std::uint32_t offset,
                              std::span<const std::uint8_t> bytes)
{
    if (id == 4U) id = 1U;
    if (id == 0U || id >= regions_.size()) return false;
    auto &r = regions_[id];
    if (offset > r.bytes.size() || bytes.size() > r.bytes.size() - offset) return false;
    std::copy(bytes.begin(), bytes.end(), r.bytes.begin() + offset);
    std::fill_n(r.known_bits.begin() + offset, bytes.size(), 0xffU);
    return true;
}

std::uint16_t RuntimeHost::read_memory_word(std::uint16_t id, std::uint32_t offset,
                                           std::uint16_t mask)
{
    checkpoint();
    auto *r = region_at(id, offset);
    if (!r || faulted()) return 0U;
    if (slice_end_ && shared_page(id, offset)) sync_shared();
    const auto known = (static_cast<std::uint16_t>(r->known_bits[offset]) << 8U)
        | r->known_bits[offset + 1U];
    if ((known & mask) != mask) {
        fail("Read requires uninitialized memory; no captured value substituted", offset, id, mask);
        return 0U;
    }
    auto original = static_cast<std::uint16_t>((r->bytes[offset] << 8U) | r->bytes[offset + 1U]);
    // The player start/continue paths OR byte $404 into credit eligibility.
    // Overlay its high bit for the user option; preserve stored settings.
    if (unlimited_credits_ && id == 2U && offset == 0x404U) original |= 0x8000U;
    return static_cast<std::uint16_t>(definition_read_word(id, offset, original) & mask);
}

void RuntimeHost::write_memory_word(std::uint16_t id, std::uint32_t offset,
                                    std::uint16_t value, std::uint16_t mask)
{
    checkpoint();
    auto *r = region_at(id, offset);
    if (!r || faulted()) return;
    if (slice_end_ && shared_page(id, offset)) sync_shared();
    if (r->read_only) { fail("Native write to ROM", offset, id, mask); return; }
    const bool cpu_b_write = selected_cpu_ == 1U && active_ && active_->cpu == 1U && mask == 0xffffU;
    const auto pc = active_ ? active_->registers.program_counter : 0U;
    if (cpu_b_write && (start_stage_ >= 0 || start_stage_publish_)) {
        const auto redirected = stage_select_write(id, offset, value, pc);
        if (redirected != value) {
            value = redirected;
            if (id == 3U) active_->registers.data[0] = (active_->registers.data[0] & 0xffff0000U) | value;
        }
    }
    const bool stage_handshake = cpu_b_write && id == 3U && offset == 0x38006U &&
        (pc == 0xd390U || pc == 0xd888U || pc == 0xd896U) && regions_[2].bytes.size() > 0x840U;
    // Opt-in diagnostics share the navigation log: GAIN_GROUND_NAV_LOG=<path>.
    static std::FILE *log = [] { const char *p = std::getenv("GAIN_GROUND_NAV_LOG"); return p && *p ? std::fopen(p, "a") : nullptr; }();
    if (log && devices_ && ((id == 2U && (offset == 0xc02U || offset == 0xc04U || offset == 0xc06U || offset == 0xc14U || offset == 0xc16U || offset == 0x820U || offset == 0x834U || offset == 0x836U)) ||
                            (id == 3U && (offset == 0x38002U || offset == 0x38006U)))) {
        const auto before = (static_cast<std::uint16_t>(r->bytes[offset]) << 8U) | r->bytes[offset + 1U];
        const auto after = static_cast<std::uint16_t>((before & ~mask) | (value & mask));
        if (after != before)
            std::fprintf(log, "write frame %llu cpu %u pc %06x region %u offset %05x %04x -> %04x\n",
                static_cast<unsigned long long>(devices_->frame()), active_ ? unsigned(active_->cpu) : 9U,
                active_ ? unsigned(active_->registers.program_counter) : 0U, unsigned(id), unsigned(offset), unsigned(before), unsigned(after));
        std::fflush(log);
    }
    if (stage_handshake) {
        if (log) {
            const auto &b = regions_[2].bytes;
            std::fprintf(log, "stage-start selector %u c02 %02x%02x c04 %02x c05 %02x c06 %02x c07 %02x 820 %02x 821 %02x 834 %02x%02x 836 %02x pending %d\n",
                unsigned(value), b[0xc02], b[0xc03], b[0xc04], b[0xc05], b[0xc06], b[0xc07], b[0x820], b[0x821], b[0x834], b[0x835], b[0x836], start_stage_);
            std::fflush(log);
        }
    }
    const auto before = (static_cast<std::uint16_t>(r->bytes[offset]) << 8U)
        | r->bytes[offset + 1U];
    const auto after = static_cast<std::uint16_t>((before & ~mask) | (value & mask));
    r->bytes[offset] = static_cast<std::uint8_t>(after >> 8U);
    r->bytes[offset + 1U] = static_cast<std::uint8_t>(after);
    r->known_bits[offset] |= static_cast<std::uint8_t>(mask >> 8U);
    r->known_bits[offset + 1U] |= static_cast<std::uint8_t>(mask);
}

// Stage select. The original stage-clear routine advances the index at
// 0xd862, then publishes it to CPU A: a round's first stage takes the
// new-round branch (bank request at $8002, selector at $8006 from 0xd888),
// any other stage the same-round branch (0xd896, no bank request). A chosen
// stage may lie in another round, so route the clear through the new-round
// branch: land on that round's first stage at 0xd862, with the bank one below
// so the original increment reaches it, then publish the chosen stage itself
// at 0xd888. Boot and the attract demo publish from other paths.
std::uint16_t RuntimeHost::stage_select_write(std::uint16_t region, std::uint32_t offset,
                                              std::uint16_t value, std::uint32_t pc)
{
    if (start_stage_ >= 0 && region == 2U && offset == 0xc02U && pc == 0xd862U) {
        const auto round = static_cast<std::uint16_t>(start_stage_ / 10);
        write_memory_word(2U, 0xc00U, static_cast<std::uint16_t>(round - 1U), 0xffffU);
        start_stage_publish_ = true;
        return static_cast<std::uint16_t>(round * 10U);
    }
    if (start_stage_publish_ && region == 3U && offset == 0x38006U && pc == 0xd888U) {
        const auto stage = static_cast<std::uint16_t>(start_stage_);
        start_stage_ = -1;
        start_stage_publish_ = false;
        start_stage_applied_ = true;
        write_memory_word(2U, 0xc02U, stage, 0xffffU);
        return static_cast<std::uint16_t>(stage + 1U);
    }
    if (start_stage_publish_ && region == 3U && offset == 0x38006U && pc == 0xd896U)
        start_stage_publish_ = false; // Not reachable after the redirect; never leave it armed.
    return value;
}

// The stage phase machine (0xd734) runs each frame of a stage. In its play
// phase it clears the stage itself once the remaining-enemy count at 0xc14
// reaches zero while players are on the field (0xc10) and none is dying
// (0xc06). That is the clear that carries every player into the next stage's
// roster; writing the clear phase directly instead behaves like the time-up
// clear, which loses the players still on the field. So a menu request zeroes
// the enemy count and lets the original trigger run, during play only: the
// attract demo marks its players in 0xc06. A clear already under way simply
// carries the pending stage.
bool RuntimeHost::stage_select_frame(std::uint32_t)
{
    const auto &b = regions_[2].bytes;
    if (!start_stage_force_ || b.size() <= 0xc17U || b[0xc06U] != 0U) return false;
    start_stage_force_ = false;
    if (b[0xc16U] == 0U && b[0xc17U] == 0U) write_memory_word(2U, 0xc14U, 0U, 0xffffU);
    return true;
}

std::uint16_t RuntimeHost::read_hardware(std::uint8_t kind, std::uint8_t cpu, std::uint8_t,
                                        std::uint32_t, std::uint32_t address, std::uint16_t mask)
{
    checkpoint();
    sync_shared();
    sync_devices();
    if (devices_ && (kind == 1U || kind == 5U))
        if (const auto value = devices_->read(address, mask)) {
            if (!timed_execution() && (address & 0xe001feU) == 0x800102U && (*value & mask & 0x80U)) {
                waiting_ = true;
                if (checkpoint_) checkpoint_(checkpoint_argument_);
                waiting_ = false;
            }
            return *value;
        }
    bool mapped = false;
    for (const auto &window : generated::kMemoryWindows) {
        const auto normalized = (address & 0xffffffU) & ~window.mirror;
        if (window.address_space == "program" && (window.cpu_mask & (1U << cpu)) &&
            normalized >= (window.start & ~1U) && normalized <= window.end) {
            mapped = true;
            if (window.access == "write-no-op") return mask;
        }
    }
    if (!mapped) return mask; // MAME unmapped 16-bit bus: all ones.
    fail("Device read has no live model", address, 0U, mask);
    return 0U;
}

void RuntimeHost::write_hardware(std::uint8_t kind, std::uint8_t cpu, std::uint8_t,
                                 std::uint32_t, std::uint32_t address,
                                 std::uint16_t value, std::uint16_t mask)
{
    checkpoint();
    sync_shared();
    sync_devices();
    if (devices_ && kind == 2U && devices_->write(address, value, mask)) {
        // A write may arm an earlier timer: end the slice at its edge.
        if (slice_end_) slice_end_ = std::min(slice_end_, devices_->next_event_ns());
        return;
    }
    // Translation observer markers duplicate the preceding device bus write.
    if (devices_ && (kind == 3U || kind == 4U)) return;
    // Only source-declared nopw mappings can be ignored. Device reset/audio
    // observation kinds require their own model and must not be swallowed here.
    if (kind == 2U && cpu < 2U) {
        for (const auto &window : generated::kMemoryWindows) {
            const auto normalized = (address & 0xffffffU) & ~window.mirror;
            if ((window.cpu_mask & (1U << cpu)) && window.address_space == "program" &&
                normalized >= window.start && normalized <= window.end &&
                window.access == "write-no-op") return;
        }
    }
    bool mapped = false;
    for (const auto &window : generated::kMemoryWindows) {
        const auto normalized = (address & 0xffffffU) & ~window.mirror;
        if (window.address_space == "program" && (window.cpu_mask & (1U << cpu)) &&
            normalized >= (window.start & ~1U) && normalized <= window.end) mapped = true;
    }
    if (!mapped && kind == 2U) return;
    fail("Device write has no live model", address, 0U, mask);
}


} // namespace gain_ground
