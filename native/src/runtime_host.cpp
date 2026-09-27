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
std::uint16_t RuntimeHost::enemy_walking_heading(std::uint32_t record,std::uint16_t original) const {
    return devices_ && selected_cpu_==1 ? enemy_navigation_.walking_heading(regions_[2].bytes,record,devices_->frame(),original):original;
}
std::uint16_t RuntimeHost::enemy_walking_advance(std::uint32_t record,std::uint16_t original) const {
    return devices_ && selected_cpu_==1 ? enemy_navigation_.walking_advance(regions_[2].bytes,record,devices_->frame(),original):original;
}
std::uint8_t RuntimeHost::enemy_direction_mode(std::uint32_t record,std::uint8_t original) const {
    // Match actual invocation owners, not just an active-attack flag or a caller
    // address. F347/F364 also serve patrol, contact and terrain responses.
    if(!devices_ || selected_cpu_!=1 || !active_ || active_->cpu!=1 || active_->state!=0x72)
        return original;
    const auto same_actor=[&](const Invocation *f,unsigned id){
        return f && f->cpu==1 && f->executing_state==0x72 && f->actor==record && f->function_id==id;
    };
    const auto *decode=invocation_;
    if(!same_actor(decode,364))return original;
    const auto *caller=decode->parent;
    bool attack=decode->callsite==0x1c3ac && same_actor(caller,331);
    if(decode->callsite==0x1da88 && same_actor(caller,347)){
        attack=(caller->callsite==0x1f024 && same_actor(caller->parent,368)) ||
               (caller->callsite==0x1c3a8 && same_actor(caller->parent,331));
    }
    if(!attack || record<0x3480 || record>=0x6c00 || (record&127))return original;
    const auto &r=regions_[2].bytes;
    if(record+127>=r.size() || !(r[record]&128) || (r[record+0xb]!=8 && r[record+0xb]!=9) ||
       r[record+0x3e] || r[record+0x3f] || !(r[record+0x41]&2) ||
       r[record+0x22] || r[record+0x23] || r[record+0x24] || r[record+0x25])return original;
    const auto callback=(std::uint32_t(r[record+2])<<24)|(std::uint32_t(r[record+3])<<16)|
                        (std::uint32_t(r[record+4])<<8)|r[record+5];
    if(callback!=0x20918 && callback!=0x2093a)return original;
    const auto normal=r[record+0x5f];
    return regions_[2].known_bits[record+0x5f]==0xff && normal>=1 && normal<=9 ? normal:original;
}
namespace {
template<class Run>
bool with_original_character(RuntimeHost &host, FunctionContext &context,
                             FunctionResult &result, Run run)
{
    if (context.cpu != 1U || context.state != 0x72U) return false;
    const auto record = context.registers.address[5];
    const auto bytes = host.region_bytes(2U);
    if (record >= bytes.size() || bytes.size() - record <= 0x4bU) return false;
    // Type dispatch is host bookkeeping: it must not add guest bus accesses.
    const gameplay::CharacterState state{record};
    switch (bytes[record + 0x4bU]) {
    case 0U: return run(gameplay::OriginalCharacter00(state), context, result);
    case 1U: return run(gameplay::OriginalCharacter<1U>(state), context, result);
    case 2U: return run(gameplay::OriginalCharacter<2U>(state), context, result);
    case 3U: return run(gameplay::OriginalCharacter<3U>(state), context, result);
    case 4U: return run(gameplay::OriginalCharacter<4U>(state), context, result);
    case 5U: return run(gameplay::OriginalCharacter<5U>(state), context, result);
    case 6U: return run(gameplay::OriginalCharacter<6U>(state), context, result);
    case 7U: return run(gameplay::OriginalCharacter<7U>(state), context, result);
    case 8U: return run(gameplay::OriginalCharacter08(state), context, result);
    case 9U: return run(gameplay::OriginalCharacter<9U>(state), context, result);
    case 10U: return run(gameplay::OriginalCharacter<10U>(state), context, result);
    case 11U: return run(gameplay::OriginalCharacter<11U>(state), context, result);
    case 12U: return run(gameplay::OriginalCharacter<12U>(state), context, result);
    case 13U: return run(gameplay::OriginalCharacter<13U>(state), context, result);
    case 14U: return run(gameplay::OriginalCharacter<14U>(state), context, result);
    case 15U: return run(gameplay::OriginalCharacter<15U>(state), context, result);
    case 16U: return run(gameplay::OriginalCharacter<16U>(state), context, result);
    case 17U: return run(gameplay::OriginalCharacter<17U>(state), context, result);
    case 18U: return run(gameplay::OriginalCharacter<18U>(state), context, result);
    case 19U: return run(gameplay::OriginalCharacter<19U>(state), context, result);
    default: return false;
    }
}
} // namespace

bool RuntimeHost::run_character_update(FunctionContext &context, FunctionResult &result)
{
    if (!gameplay::is_character_update_entry(context.registers.program_counter)) return false;
    // Remember which record each player character occupies (player index at +6D).
    const auto record = context.registers.address[5];
    const auto bytes = region_bytes(2U);
    if (record + 0x6dU < bytes.size()) player_records_[bytes[record + 0x6dU] & 3U] = record;
    return with_original_character(*this, context, result, gameplay::run_character_update);
}

bool RuntimeHost::teleport_player(unsigned player, int x, int y)
{
    const auto record = player_record(player);
    if (record == 0U || regions_[2].bytes.size() <= record + 0x1dU || !(regions_[2].bytes[record] & 0x80U)) return false;
    const auto fx = static_cast<std::uint32_t>(x) << 16U, fy = static_cast<std::uint32_t>(y) << 16U;
    write_memory_word(2U, record + 0x12U, static_cast<std::uint16_t>(fx >> 16U), 0xffffU);
    write_memory_word(2U, record + 0x14U, static_cast<std::uint16_t>(fx), 0xffffU);
    write_memory_word(2U, record + 0x1aU, static_cast<std::uint16_t>(fy >> 16U), 0xffffU);
    write_memory_word(2U, record + 0x1cU, static_cast<std::uint16_t>(fy), 0xffffU);
    return true;
}

bool RuntimeHost::run_character_attacks(FunctionContext &context, FunctionResult &result)
{
    const auto pc = context.registers.program_counter;
    if (pc != 0x10a24U && pc != 0x10a28U && pc != 0x10a2cU) return false;
    return with_original_character(*this, context, result, gameplay::run_character_attacks);
}

bool RuntimeHost::run_character_movement(FunctionContext &context, FunctionResult &result)
{
    if (context.registers.program_counter != 0xfe54U) return false;
    return with_original_character(*this, context, result, gameplay::run_character_movement);
}

bool RuntimeHost::run_character_attack_phase(FunctionContext &context, FunctionResult &result)
{
    const auto pc = context.registers.program_counter;
    if (pc != 0x10a2eU && pc != 0x10a4aU && pc != 0x10cc0U &&
        pc != 0x10cdcU && pc != 0x10d1aU) return false;
    return with_original_character(*this, context, result, gameplay::run_character_attack_phase);
}

bool RuntimeHost::run_character_collision(FunctionContext &context, FunctionResult &result)
{
    if (context.registers.program_counter != 0x102aaU) return false;
    return with_original_character(*this, context, result, gameplay::run_character_collision);
}

bool RuntimeHost::run_character_exit(FunctionContext &context, FunctionResult &result)
{
    if (context.registers.program_counter != 0x10374U) return false;
    return with_original_character(*this, context, result, gameplay::run_character_exit);
}

bool RuntimeHost::run_projectile_update(FunctionContext &context, FunctionResult &result)
{
    if (context.cpu != 1U || context.state != 0x72U) return false;
    return gameplay::run_projectile_update(context, result);
}

bool RuntimeHost::run_character_damage(FunctionContext &context, FunctionResult &result)
{
    const auto pc = context.registers.program_counter;
    if (pc != 0x10272U && pc != 0x10276U && pc != 0x1029aU) return false;
    return with_original_character(*this, context, result, gameplay::run_character_damage);
}

std::uint16_t RuntimeHost::character_profile(std::uint32_t record,
    unsigned definition_offset, std::uint16_t original) const noexcept
{
    const auto bytes = region_bytes(2U);
    if (record >= bytes.size() || bytes.size() - record <= 0x4bU) return original;
    gameplay::CharacterProfile field;
    switch (definition_offset) {
    case 6U: field = gameplay::CharacterProfile::movement; break;
    case 8U: field = gameplay::CharacterProfile::primary_attack; break;
    case 10U: field = gameplay::CharacterProfile::secondary_attack; break;
    default: return original;
    }
    return character_definitions_.profile(bytes[record + 0x4bU], field, original);
}

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

RuntimeHost::RuntimeHost()
{
    // Region IDs describe the native bus ABI; only their geometry is reused.
    // Unknown power-on RAM is not silently promoted to authoritative zeroes.
    for (const auto &spec : generated::kFixtureRegions) {
        if (spec.id == 4U) continue; // BIOS mirrors the same physical ROM as 1.
        auto &r = regions_[spec.id];
        r.bytes.resize(spec.bytes);
        r.known_bits.resize(spec.bytes);
        r.read_only = spec.immutable;
    }
}

void RuntimeHost::set_checkpoint(Checkpoint callback, void *argument) noexcept
{
    checkpoint_ = callback;
    checkpoint_argument_ = argument;
}

std::uint64_t RuntimeHost::execution_time_ns() const noexcept
{
    return devices_ ? devices_->time_ns() : 0U;
}

void RuntimeHost::wait_until_time(std::uint64_t deadline_ns)
{
    if (!devices_ || !checkpoint_ || deadline_ns <= devices_->time_ns()) return;
    const auto cpu = selected_cpu_;
    const auto previous_deadline = cpu_deadlines_[cpu];
    cpu_deadlines_[cpu] = deadline_ns;
    const auto previous_waiting = waiting_;
    waiting_ = true;
    const auto other = 1U - cpu;
    while (!faulted() && devices_->time_ns() < deadline_ns) {
        // When the other CPU cannot run before this deadline, the scheduler
        // would only step the device clock to the next device event or this
        // deadline and switch straight back. Take exactly that step here and
        // save the fiber round trip; the device sees the same sequence of
        // advances. Otherwise yield so the other CPU runs in its turn.
        // A CPU without a deadline is runnable, not idle: the scheduler would
        // switch to it before stepping time. Only a later deadline, or CPU B
        // still disabled, leaves this CPU alone until its own deadline.
        const auto other_deadline = cpu_deadlines_[other];
        const bool other_idle = (other == 1U && !devices_->cpu_b_enabled()) ||
                                (other_deadline != UINT64_MAX && other_deadline > deadline_ns);
        if (other_idle && direct_wait_) {
            const auto step = std::min(devices_->next_event_ns(), deadline_ns);
            if (step > devices_->time_ns()) { devices_->advance(step); operations_since_switch_ = 0U; continue; }
        }
        checkpoint_(checkpoint_argument_);
    }
    waiting_ = previous_waiting;
    cpu_deadlines_[cpu] = previous_deadline;
}

bool RuntimeHost::cpu_ready(unsigned cpu) const noexcept
{
    return cpu_deadlines_[cpu] == UINT64_MAX || cpu_deadlines_[cpu] <= execution_time_ns();
}

std::uint64_t RuntimeHost::next_cpu_deadline_ns() const noexcept
{
    auto next = UINT64_MAX;
    for (const auto deadline : cpu_deadlines_)
        if (deadline > execution_time_ns()) next = std::min(next, deadline);
    return next;
}

void RuntimeHost::checkpoint()
{
    ++operations_;
    ++operations_since_switch_;
    if (checkpoint_) checkpoint_(checkpoint_argument_);
}

void RuntimeHost::fail(std::string_view message, std::uint32_t address,
                        std::uint16_t region, std::uint16_t mask)
{
    if (!faulted()) fault_ = {message, active_ ? active_->registers.program_counter : 0U,
        address, region, mask, active_ ? active_->cpu : std::uint8_t{0}};
    checkpoint();
}

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
    const auto known = (static_cast<std::uint16_t>(r->known_bits[offset]) << 8U)
        | r->known_bits[offset + 1U];
    if ((known & mask) != mask) {
        fail("Read requires uninitialized memory; no captured value substituted", offset, id, mask);
        return 0U;
    }
    const auto original = static_cast<std::uint16_t>((r->bytes[offset] << 8U) | r->bytes[offset + 1U]);
    return static_cast<std::uint16_t>(definition_read_word(id, offset, original) & mask);
}

void RuntimeHost::write_memory_word(std::uint16_t id, std::uint32_t offset,
                                    std::uint16_t value, std::uint16_t mask)
{
    checkpoint();
    auto *r = region_at(id, offset);
    if (!r || faulted()) return;
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
    if (devices_ && kind == 2U && devices_->write(address, value, mask)) return;
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

void RuntimeHost::set_irq_line(std::uint8_t cpu, std::uint8_t level, bool asserted) noexcept
{
    if (cpu >= 2U || level == 0U || level > 7U) return;
    const auto bit = static_cast<std::uint8_t>(1U << level);
    if (asserted) irq_lines_[cpu] |= bit;
    else irq_lines_[cpu] &= static_cast<std::uint8_t>(~bit);
}

PendingInterrupt RuntimeHost::consume_pending_interrupt(std::uint8_t cpu, std::uint8_t,
                                                       std::uint32_t completed_pc)
{
    checkpoint();
    if (cpu >= 2U) { fail("Invalid interrupt CPU"); return {}; }
    if (devices_) {
        // Suspend original idle/backedge loops without changing registers.
        // The scheduler resumes here after advancing to a device event, so
        // the pending interrupt is sampled after the suspension returns.
        const auto status = active_ ? active_->registers.status : 0x2700U;
        const auto pending = devices_->irq_level(cpu);
        if (pending <= ((status >> 8U) & 7U) &&
            ((cpu == 0U && completed_pc == 0x80118U) ||
             (cpu == 1U && completed_pc == 0x85b4U && !(status & 8U)))) {
            waiting_ = true;
            if (checkpoint_) checkpoint_(checkpoint_argument_);
            waiting_ = false;
        }
        const auto level = devices_->irq_level(cpu);
        if (level) return {true, level};
    }
    for (std::uint8_t level = 7U; level != 0U; --level)
        if (irq_lines_[cpu] & (1U << level)) return {true, level};
    return {};
}

const FunctionContract *RuntimeHost::entry_at(std::uint8_t cpu, std::uint8_t state,
                                              std::uint32_t pc) const noexcept
{
    return native_registry::find(cpu, state, pc);
}

FunctionContext RuntimeHost::cpu_a_reset_context()
{
    FunctionContext c{};
    c.host = this;
    c.cpu = 0U;
    c.state = 0xffU;
    c.registers.status = 0x2700U;
    // Original big-endian initial SSP and PC; no hard-coded boot entry override.
    c.registers.address[7] = (static_cast<std::uint32_t>(read_memory_word(1U, 0U, 0xffffU)) << 16U)
        | read_memory_word(1U, 2U, 0xffffU);
    c.registers.program_counter = (static_cast<std::uint32_t>(read_memory_word(1U, 4U, 0xffffU)) << 16U)
        | read_memory_word(1U, 6U, 0xffffU);
    return c;
}

FunctionResult RuntimeHost::execute(const FunctionContract &first, FunctionContext &c, std::uint32_t callsite)
{
    if (depth_ >= 512U) {
        // Opt-in diagnostics: the innermost frames of the runaway chain.
        static std::FILE *log = [] { const char *p = std::getenv("GAIN_GROUND_NAV_LOG"); return p && *p ? std::fopen(p, "a") : nullptr; }();
        if (log) {
            std::fprintf(log, "call depth exceeded entering %06x (function %u); innermost frames:", unsigned(first.address), unsigned(first.id));
            unsigned shown = 0U;
            for (const auto *frame = invocation_; frame && shown < 24U; frame = frame->parent, ++shown)
                std::fprintf(log, " %u@%06x", unsigned(frame->function_id), unsigned(frame->callsite));
            std::fputc('\n', log);
            std::fflush(log);
        }
        fail("Native call depth exceeds runtime capacity");
        return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    }
    auto *previous_context = active_;
    Invocation frame{invocation_, {}, 0U, false};
    frame.callsite = callsite;
    invocation_ = &frame;
    active_ = &c;
    ++depth_;
    const FunctionContract *f = &first;
    FunctionResult result{};
    // Class selection is host bookkeeping and must not add guest bus reads.
    // Retain the decision across IRQ resumption even if a callback changes the
    // actor's next-frame callback while completing this invocation.
    const auto actor = c.registers.address[5];
    const auto actor_bytes = region_bytes(2U);
    bool enemy_invocation = false;
    if (c.cpu == 1U && c.state == 0x72U && actor >= 0x3400U && actor < 0x7400U &&
        (actor & 0x7fU) == 0U && actor_bytes.size() > actor + 5U) {
        const auto callback = (std::uint32_t(actor_bytes[actor+2U]) << 24U) |
            (std::uint32_t(actor_bytes[actor+3U]) << 16U) |
            (std::uint32_t(actor_bytes[actor+4U]) << 8U) | actor_bytes[actor+5U];
        enemy_invocation = callback == first.address;
    }
    for (;;) {
        checkpoint();
        if (faulted()) { result = {TranslationStatus::contract_violation, 0U, c.registers.program_counter}; break; }
        if (!f->implemented || !f->entry) {
            fail("Native function has no body");
            result = {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
            break;
        }
        frame.executed_child = false;
        frame.executing_state = f->state;
        frame.cpu = c.cpu;
        frame.function_id = f->id;
        frame.actor = c.registers.address[5];
        if (start_stage_force_ && f->id == 153U && c.cpu == 1U && c.state == 0x72U &&
            c.registers.program_counter == 0xd734U)
            (void)stage_select_frame(c.registers.address[5]);
        if (f->id == 140U && c.cpu == 1U && c.state == 0x72U && c.registers.program_counter == 0xa618U)
            enemy_navigation_.reset();
        if (f->id == 140U && c.cpu == 1U && c.state == 0x72U &&
            c.registers.program_counter == 0xa618U && !apply_level_definition()) {
            result = {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
            break;
        }
        if (devices_ && f->id == 353U && c.cpu == 1U && c.state == 0x72U && c.registers.program_counter == 0x1dbf0U)
            enemy_navigation_.prepare(regions_[2].bytes,regions_[3].bytes,c.registers.address[5],devices_->frame());
        const auto moving_actor=c.registers.address[5];
        const auto contacted_actor=c.registers.address[6];
        const bool new_contact_test=devices_ && f->id==354U && c.cpu==1U && c.state==0x72U &&
            moving_actor<regions_[2].bytes.size()-0x40U && (regions_[2].bytes[moving_actor+0x40]&2U)==0;
        const bool separating_contact=devices_ && f->id==354U && c.cpu==1U && c.state==0x72U &&
            enemy_navigation_.separating(regions_[2].bytes,moving_actor,c.registers.address[6],devices_->frame()) &&
            (regions_[2].bytes[moving_actor+0x40]&2U)==0;
        if (!enemy_invocation || !gameplay::run_enemy(*f, c, result)) result = f->entry(c);
        // Only waive the NPC/NPC contact just tested, when our validated step
        // reduces an existing overlap. Terrain and other contacts still run.
        if(separating_contact && result.status==TranslationStatus::complete && result.control==1U)
            regions_[2].bytes[moving_actor+0x40]&=static_cast<std::uint8_t>(~2U);
        if(new_contact_test && result.status==TranslationStatus::complete && result.control==1U)
            enemy_navigation_.record_contact_block(regions_[2].bytes,moving_actor,contacted_actor,devices_->frame());
        if(devices_ && f->id==357U && c.cpu==1U && c.state==0x72U &&
           result.status==TranslationStatus::complete && result.control==1U)
            enemy_navigation_.finish_move(regions_[2].bytes,moving_actor,devices_->frame());
        if (result.status != TranslationStatus::complete) {
            // Stop at the first rejected native operation, before an older
            // wrapper can discard its result and report a misleading transfer.
            if (!faulted()) {
                fault_ = {"Native function rejected execution", c.registers.program_counter,
                    result.exit_program_counter, 0U, 0U, c.cpu,
                    f->id, f->address, c.state, static_cast<std::uint8_t>(result.status), result.control};
            }
            checkpoint();
            break;
        }
        if (result.control == 5U && frame.executed_child && frame.child.control == 2U &&
            c.registers.program_counter >= f->body_min && c.registers.program_counter <= f->body_max) {
            // A translated instruction suspended for an ISR. Resume its owner
            // at the architectural return PC after the native ISR completed.
            continue;
        }
        if (result.control == 5U && frame.executed_child) { result = frame.child; break; }
        // Retained trampolines and self-loop bodies can report a transfer or
        // loop boundary after the live child has already run through RTS/RTE.
        // Preserve that actual return for both control-3 and control-4 wrappers.
        if ((result.control == 3U || result.control == 4U) && frame.executed_child &&
            result.exit_program_counter == frame.child_target &&
            frame.child.status == TranslationStatus::complete &&
            frame.child.exit_program_counter == c.registers.program_counter &&
            (frame.child.control == 1U || frame.child.control == 2U)) {
            result = frame.child;
            break;
        }
        if (result.control != 3U) break;
        if (c.registers.program_counter != result.exit_program_counter) {
            fail("Transfer result and architectural PC disagree");
            result = {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
            break;
        }
        f = entry_at(c.cpu, c.state, c.registers.program_counter);
        if (!f) {
            fail("No exact native entry for transfer target", c.registers.program_counter);
            result = {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
            break;
        }
    }
    --depth_;
    active_ = previous_context;
    invocation_ = frame.parent;
    return result;
}

FunctionResult RuntimeHost::run(FunctionContext &c)
{
    c.host = this;
    const auto *f = entry_at(c.cpu, c.state, c.registers.program_counter);
    if (!f) {
        active_ = &c;
        fail("No exact native entry for CPU start", c.registers.program_counter);
        active_ = nullptr;
        return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    }
    return execute(*f, c);
}

FunctionResult RuntimeHost::call_function(std::uint32_t id, std::uint8_t cpu,
    std::uint8_t state, std::uint8_t kind, std::uint32_t site, std::uint32_t target, FunctionContext &c)
{
    checkpoint();
    const auto *f = entry_at(cpu, state, target);
    if (faulted() || !f || f->id != id || c.cpu != cpu) {
        fail("Native call target/CPU/state mapping is unresolved", target);
        return {TranslationStatus::contract_violation, 0U, target};
    }
    if (devices_ && cpu == 1U && state == 0x72U && (id == 309U || id == 310U)) {
        // Presentation-only override for the original title command. Never
        // suppress or replace the translated call, queue writes or YM timing.
        const auto command = c.registers.data[0] & 0xffffU;
        // DBCC checks the credit-change flag; DBD2/DBD6 submit cue 0x36.
        // It overlays the title, so preserve both replacement playback and
        // the original title-voice mask, including before key-on and after EOF.
        const bool coin_credit_cue = id == 309U && site == 0xdbd6U && command == 0x36U;
        if (!coin_credit_cue)
            devices_->audio.notify_sound_command(id == 309U && site == 0xd430U && command == 0x52U);
    }
    if (assets_ && cpu == 0U && state == 0xffU && kind == 4U &&
        (id == 50U || id == 51U || id == 52U || id == 53U)) {
        if (c.state != state || c.registers.program_counter != target) {
            fail("Direct load trap architectural state disagrees with target", target);
            return {TranslationStatus::contract_violation, 0U, target};
        }
        return direct_load_trap(id, c);
    }
    if (kind == 0U) {
        // A sequential partition marker: let its enclosing body finish, then
        // the outer dispatcher executes the returned control-3 continuation.
        return FunctionResult::complete(3U, target);
    }
    if (kind != 1U && kind != 2U && kind != 3U && kind != 4U && kind != 6U) {
        fail("Exception/FD1094 transition requires a live device model", target);
        return {TranslationStatus::contract_violation, 0U, target};
    }
    // Older bodies assign the IRQ state before calling the host. The active
    // owner's state still identifies the state to restore after its ISR.
    const auto source_state = invocation_ ? invocation_->executing_state : c.state;
    if (kind == 6U) c.state = state; // explicit native FD1094/IRQ target mapping
    if (c.state != state || c.registers.program_counter != target) {
        fail("Native call architectural state disagrees with target", target);
        return {TranslationStatus::contract_violation, 0U, target};
    }
    // IRQ5's prologue and frame-commit body now advance their own bus and
    // internal clocks. No aggregate minimum-duration delay owns that time.
    // D2 still contains the request ID on entry to the priority selector.
    // An enqueued coin is insufficient: rejected requests must not unmute
    // the original title voices. The accepted cue's own key-ons release them.
    const bool coin_request = devices_ && cpu == 0U && state == 0xffU &&
        id == 85U && site == 0x83d1aU && (c.registers.data[2] & 0xffffU) == 0x36U;
    const auto stack_before = c.registers.address[7];
    const auto caller_id = invocation_ ? invocation_->function_id : 0U;
    // CPU B stack words as the caller left them: [a7] is the caller's own
    // continuation (just pushed), [a7+4] is the caller's return address.
    const auto stack_long = [&](std::uint32_t a) -> std::optional<std::uint32_t> {
        const auto &b = regions_[2].bytes;
        if (cpu != 1U || a + 8U > b.size()) return std::nullopt;
        return (std::uint32_t(b[a]) << 24U) | (std::uint32_t(b[a + 1U]) << 16U) | (std::uint32_t(b[a + 2U]) << 8U) | b[a + 3U];
    };
    const auto caller_next = stack_long(stack_before);
    const auto caller_return = stack_long(stack_before + 4U);
    auto child = execute(*f, c, site);
    // Nonlocal returns. Some originals drop their caller's frame and return
    // to the caller's caller. Hand translations report that as control 8;
    // a literal translation reports control 1 with the stack popped twice.
    // Normalise both here so every caller, generated or hand-written, sees a
    // control-8 result pass through it, and the frame whose continuation the
    // return lands on sees an ordinary return.
    if (cpu == 1U && child.status == TranslationStatus::complete) {
        const auto pc = c.registers.program_counter;
        if (child.control == 1U && caller_return && c.registers.address[7] == stack_before + 8U && pc == *caller_return)
            child = FunctionResult::complete(8U, pc);
        else if (child.control == 8U && caller_next && pc == *caller_next)
            child = FunctionResult::complete(1U, pc);
    }
    // Opt-in diagnostics (GAIN_GROUND_NAV_LOG): every child result that is not a
    // plain return to its caller, with the stack movement it left behind.
    if (devices_ && (child.status != TranslationStatus::complete || child.control != 1U ||
                     c.registers.address[7] != stack_before + 4U)) {
        static std::FILE *log = [] { const char *p = std::getenv("GAIN_GROUND_NAV_LOG"); return p && *p ? std::fopen(p, "a") : nullptr; }();
        if (log) {
            std::fprintf(log, "call frame %llu caller %u site %06x callee %u target %06x -> status %u control %u exit %06x a7 %05x -> %05x\n",
                static_cast<unsigned long long>(devices_->frame()), unsigned(caller_id), unsigned(site), unsigned(id), unsigned(target),
                unsigned(child.status), unsigned(child.control), unsigned(child.exit_program_counter),
                unsigned(stack_before), unsigned(c.registers.address[7]));
            std::fflush(log);
        }
    }
    if (coin_request && child.status == TranslationStatus::complete && child.control == 1U)
        devices_->audio.notify_coin_credit_accepted();
    if (child.status == TranslationStatus::complete && child.control == 2U &&
        (kind == 3U || kind == 4U || kind == 6U)) c.state = source_state;
    if (invocation_) {
        invocation_->child = child;
        invocation_->child_target = target;
        invocation_->executed_child = true;
    }
    return child;
}
} // namespace gain_ground
