#pragma once

#include "gain_ground/contract_types.h"
#include "gain_ground/gameplay/character.h"

namespace gain_ground::gameplay {

// True means the result replaces the translated owner. False leaves an
// unconverted continuation to the owner with its current CPU state intact.
bool run_character_update(const Character &, FunctionContext &, FunctionResult &);
bool run_character_attacks(const Character &, FunctionContext &, FunctionResult &);
bool run_character_movement(const Character &, FunctionContext &, FunctionResult &);
bool run_character_attack_phase(const Character &, FunctionContext &, FunctionResult &);
bool run_character_collision(const Character &, FunctionContext &, FunctionResult &);
bool run_character_exit(const Character &, FunctionContext &, FunctionResult &);
bool run_projectile_update(FunctionContext &, FunctionResult &);
bool run_character_damage(const Character &, FunctionContext &, FunctionResult &);
bool is_character_update_entry(std::uint32_t pc) noexcept;

} // namespace gain_ground::gameplay
