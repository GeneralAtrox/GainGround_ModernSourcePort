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

} // namespace gain_ground
