#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <span>
#include <string_view>
#include <vector>

namespace gain_ground {

struct CheckpointStreamIdentity {
    std::array<std::uint8_t, 20> base_mame_commit{};
    std::array<std::uint8_t, 32> identity_sha256{};
    std::array<std::uint8_t, 32> rom_manifest_sha256{};
    std::array<std::uint8_t, 32> executable_sha256{};
    std::array<std::uint8_t, 32> configuration_sha256{};
    std::array<std::uint8_t, 32> replay_sha256{};
    std::array<std::uint8_t, 32> source_manifest_sha256{};
};

class CheckpointPayload {
public:
    CheckpointPayload &u8(std::uint8_t value);
    CheckpointPayload &u16(std::uint16_t value);
    CheckpointPayload &u32(std::uint32_t value);
    CheckpointPayload &s32(std::int32_t value);
    CheckpointPayload &u64(std::uint64_t value);
    CheckpointPayload &s64(std::int64_t value);
    CheckpointPayload &bytes(std::span<const std::uint8_t> value);

    [[nodiscard]] std::span<const std::uint8_t> view() const noexcept { return bytes_; }
    [[nodiscard]] std::size_t size() const noexcept { return bytes_.size(); }

private:
    std::vector<std::uint8_t> bytes_;
};

class CheckpointProjectionWriter {
public:
    CheckpointProjectionWriter(
        std::filesystem::path path,
        const CheckpointStreamIdentity &identity);
    ~CheckpointProjectionWriter();

    CheckpointProjectionWriter(const CheckpointProjectionWriter &) = delete;
    CheckpointProjectionWriter &operator=(const CheckpointProjectionWriter &) = delete;
    CheckpointProjectionWriter(CheckpointProjectionWriter &&) = delete;
    CheckpointProjectionWriter &operator=(CheckpointProjectionWriter &&) = delete;

    void write_event(
        std::uint16_t event_type,
        std::uint8_t event_specific_flags,
        std::uint8_t producer_domain,
        std::span<const std::uint8_t> payload,
        std::int64_t emulated_seconds = 0,
        std::int64_t emulated_attoseconds = 0);
    void finish();

    [[nodiscard]] std::uint64_t event_count() const noexcept;
    [[nodiscard]] const std::filesystem::path &path() const noexcept;

private:
    class Implementation;
    std::unique_ptr<Implementation> implementation_;
};

[[nodiscard]] std::array<std::uint8_t, 20> checkpoint_sha1(std::string_view hexadecimal);
[[nodiscard]] std::array<std::uint8_t, 32> checkpoint_sha256(std::string_view hexadecimal);

} // namespace gain_ground
