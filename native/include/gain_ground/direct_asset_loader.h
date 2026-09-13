#pragma once
#include <cstdint>
#include <filesystem>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace gain_ground {
class RuntimeHost;

// User-approved replacement for floppy transport, not a parity implementation
// of its mechanical timing, scratch buffer contents or diagnostic registers.
class DirectAssetLoader {
public:
    using CheckHash = bool (*)(std::span<const std::uint8_t>, std::string_view);
    bool open(const std::filesystem::path &root, CheckHash check_hash);
    bool load_boot(RuntimeHost &host);
    bool transfer(RuntimeHost &host, std::uint32_t logical_offset,
                  std::uint32_t destination, std::uint32_t bytes);
    [[nodiscard]] bool ready() const noexcept { return !payloads_.empty(); }
    [[nodiscard]] const std::string &error() const noexcept { return error_; }
    [[nodiscard]] std::uint64_t bytes_loaded() const noexcept { return bytes_loaded_; }
private:
    std::vector<std::vector<std::uint8_t>> payloads_;
    std::string error_;
    std::uint64_t bytes_loaded_{};
};
} // namespace gain_ground
