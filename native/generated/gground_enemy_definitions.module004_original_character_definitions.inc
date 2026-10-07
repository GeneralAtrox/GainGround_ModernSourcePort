std::span<const CharacterDefinition> original_character_definitions() noexcept { return {}; }
const LevelDefinition *level_definition(std::uint16_t) noexcept { return nullptr; }
const EnemyDefinition *enemy_definition(std::uint32_t callback) noexcept {
    for (const auto &definition : enemies) if (definition.callback == callback) return &definition;
    return nullptr;
}
