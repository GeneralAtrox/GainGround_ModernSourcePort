#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <span>

namespace gain_ground {

class AssetCheckpointWriter {
public:
    explicit AssetCheckpointWriter(std::filesystem::path output_root);

    void write_asset_set(
        std::uint32_t asset_set,
        std::span<const std::uint8_t> primary_retained,
        std::span<const std::uint8_t> metatile_retained,
        std::span<const std::uint8_t> attribute_workspace);
    void finish();

    [[nodiscard]] const std::filesystem::path &output_root() const noexcept { return output_root_; }
    [[nodiscard]] std::uint32_t asset_set_count() const noexcept { return asset_set_count_; }

private:
    std::filesystem::path output_root_;
    std::array<bool, 42> written_{};
    std::uint32_t asset_set_count_{};
    bool finished_{};
};

} // namespace gain_ground
