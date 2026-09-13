#pragma once
#include "gain_ground/direct_asset_loader.h"
#include <filesystem>
#include <string>
#include <vector>

namespace gain_ground {
inline constexpr std::string_view kBiosSha256 =
    "a42dc284615f58ec035652f178e1bae9b1443e7468e436c578e328dd75dc93ed";
bool matches_hash(std::span<const std::uint8_t> bytes, std::string_view expected);
using RomHashCheck = DirectAssetLoader::CheckHash;
// Accept an extracted set directory, a member of that directory, or a ZIP.
std::vector<std::uint8_t> read_rom_member(const std::filesystem::path &source,
    std::string_view name, std::size_t expected_size);
bool valid_rom_cache(const std::filesystem::path &cache, RomHashCheck hash);
bool import_rom_set(const std::filesystem::path &source,
    const std::filesystem::path &cache, RomHashCheck hash, std::string &error);
}
