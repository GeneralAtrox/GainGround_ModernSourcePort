#include "gain_ground/gameplay/character_definition.h"
#include "gain_ground/gameplay/game_definitions.h"

#include <fstream>
#include <string_view>

namespace gain_ground::gameplay {

std::uint16_t CharacterDefinition::profile(CharacterProfile field) const noexcept
{
    switch (field) {
    case CharacterProfile::movement: return movement_profile;
    case CharacterProfile::primary_attack: return primary_attack_profile;
    case CharacterProfile::secondary_attack: return secondary_attack_profile;
    }
    return 0U;
}

std::uint16_t CharacterDefinitions::profile(std::uint8_t id, CharacterProfile field,
                                            std::uint16_t original) const noexcept
{
    for (const auto &definition : overrides_)
        if (definition && definition->original_id == id) return definition->profile(field);
    for (const auto &definition : original_character_definitions())
        if (definition.original_id == id) return definition.profile(field);
    return original;
}

namespace {
// Deliberately limited content schema: one flat JSON object with four named
// unsigned integer fields. Reject unsupported content rather than ignore it.
class DefinitionReader {
public:
    explicit DefinitionReader(std::string_view source) : source_(source) {
        if (source_.starts_with("\xef\xbb\xbf")) position_ = 3U;
    }

    bool read(std::uint8_t expected_id, CharacterDefinition &definition, std::string &error) {
        if (!consume('{')) return fail(error, "expected a JSON object");
        unsigned seen = 0U;
        for (;;) {
            std::string_view key;
            if (!name(key)) return fail(error, "expected a supported field name in quotes");
            unsigned field;
            if (key == "original_id") field = 0U;
            else if (key == "movement_profile") field = 1U;
            else if (key == "primary_attack_profile") field = 2U;
            else if (key == "secondary_attack_profile") field = 3U;
            else return fail(error, "unknown field: " + std::string(key));
            if ((seen & (1U << field)) != 0U)
                return fail(error, "duplicate field: " + std::string(key));
            if (!consume(':')) return fail(error, "expected ':' after " + std::string(key));
            std::uint16_t value;
            if (!number(value)) return fail(error, "expected an unsigned integer for " + std::string(key));
            if (field == 0U) {
                if (value != expected_id) return fail(error, "original_id must match the filename");
                definition.original_id = expected_id;
            } else if (field == 1U) {
                // 10824..108D3: eleven original 16-byte movement rows.
                if (value > 10U) return fail(error, "movement_profile must be in 0..10");
                definition.movement_profile = value;
            } else {
                // Original 38-byte rows: 11 primary and 20 secondary profiles.
                const auto maximum = field == 2U ? 10U : 19U;
                if (value > maximum)
                    return fail(error, std::string(key) + " must be in 0.." + std::to_string(maximum));
                if (field == 2U) definition.primary_attack_profile = value;
                else definition.secondary_attack_profile = value;
            }
            seen |= 1U << field;
            if (consume('}')) break;
            if (!consume(',')) return fail(error, "expected ',' or '}'");
        }
        whitespace();
        if (position_ != source_.size()) return fail(error, "unexpected content after the object");
        if (seen != 15U) return fail(error, "all four definition fields are required");
        return true;
    }

private:
    std::string_view source_;
    std::size_t position_{};

    void whitespace() {
        while (position_ < source_.size()) {
            const auto c = source_[position_];
            if (c != ' ' && c != '\t' && c != '\r' && c != '\n') break;
            ++position_;
        }
    }
    bool consume(char c) {
        whitespace();
        if (position_ == source_.size() || source_[position_] != c) return false;
        ++position_;
        return true;
    }
    bool name(std::string_view &key) {
        if (!consume('"')) return false;
        const auto start = position_;
        while (position_ < source_.size() && source_[position_] != '"') {
            const auto c = source_[position_++];
            if ((c < 'a' || c > 'z') && c != '_') return false;
        }
        if (position_ == source_.size()) return false;
        key = source_.substr(start, position_ - start);
        ++position_;
        return true;
    }
    bool number(std::uint16_t &result) {
        whitespace();
        const auto start = position_;
        unsigned value = 0U;
        while (position_ < source_.size()) {
            const auto c = source_[position_];
            if (c < '0' || c > '9') break;
            if (position_ > start && source_[start] == '0') return false;
            value = value * 10U + static_cast<unsigned>(c - '0');
            if (value > 65535U) return false;
            ++position_;
        }
        if (position_ == start) return false;
        result = static_cast<std::uint16_t>(value);
        return true;
    }
    bool fail(std::string &error, const std::string &message) const {
        error = message + " (byte " + std::to_string(position_) + ")";
        return false;
    }
};
} // namespace

bool CharacterDefinitions::load(const std::filesystem::path &directory, std::string &error)
{
    std::array<std::optional<CharacterDefinition>, 20> pending{};
    if (!std::filesystem::is_directory(directory)) {
        error = directory.string() + ": character definition directory does not exist";
        return false;
    }
    for (std::size_t index = 0U; index < pending.size(); ++index) {
        const auto filename = (index < 10U ? std::string("0") : std::string()) + std::to_string(index) + ".json";
        const auto path = directory / filename;
        if (!std::filesystem::exists(path)) continue;
        std::ifstream input(path, std::ios::binary | std::ios::ate);
        const auto length = input ? input.tellg() : std::streampos(-1);
        if (length <= std::streampos(0) || length > std::streampos(4096)) {
            error = path.string() + ": cannot read a definition of 1..4096 bytes";
            return false;
        }
        std::string source(static_cast<std::size_t>(length), '\0');
        input.seekg(0);
        if (!input.read(source.data(), static_cast<std::streamsize>(source.size()))) {
            error = path.string() + ": cannot read the complete definition";
            return false;
        }
        CharacterDefinition definition;
        if (!DefinitionReader(source).read(static_cast<std::uint8_t>(index), definition, error)) {
            error = path.string() + ": " + error;
            return false;
        }
        pending[index] = definition;
    }
    overrides_ = pending;
    error.clear();
    return true;
}

} // namespace gain_ground::gameplay
