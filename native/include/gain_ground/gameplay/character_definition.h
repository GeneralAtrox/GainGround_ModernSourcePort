#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>

namespace gain_ground::gameplay {

enum class CharacterProfile : unsigned { movement, primary_attack, secondary_attack };

// Original table: 26F1C + character ID * 16, words at +6, +8 and +A.
// These are independent table selectors, not damage/health/speed scalars.
struct CharacterDefinition {
    std::uint8_t original_id{};
    std::uint16_t movement_profile{};
    std::uint16_t primary_attack_profile{};
    std::uint16_t secondary_attack_profile{};

    std::uint16_t profile(CharacterProfile field) const noexcept;
};

class CharacterDefinitions {
public:
    // Explicit content loading. Without overrides, the original loaded data
    // supplies the original defaults. Loading optional overrides is transactional.
    bool load(const std::filesystem::path &directory, std::string &error);
    std::uint16_t profile(std::uint8_t id, CharacterProfile field,
                          std::uint16_t original) const noexcept;
private:
    std::array<std::optional<CharacterDefinition>, 20> overrides_{};
};

} // namespace gain_ground::gameplay
