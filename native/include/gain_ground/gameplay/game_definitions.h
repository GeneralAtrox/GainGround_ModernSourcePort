#pragma once
#include "gain_ground/gameplay/character_definition.h"
#include "gain_ground/gameplay/enemy.h"
#include <span>

namespace gain_ground::gameplay {
struct DefinitionPatch {
    std::uint32_t address;
    std::span<const std::uint8_t> bytes;
};
struct LevelDefinition {
    std::uint16_t original_index;
    std::uint16_t layout_asset_set;
    std::span<const DefinitionPatch> data;
};

// Optional override interfaces. Default character and level tables stay in ROM.
std::span<const CharacterDefinition> original_character_definitions() noexcept;
const LevelDefinition *level_definition(std::uint16_t original_index) noexcept;
const EnemyDefinition *enemy_definition(std::uint32_t callback) noexcept;
} // namespace gain_ground::gameplay
