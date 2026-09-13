#include "gain_ground/direct_asset_loader.h"
#include "gain_ground/runtime_host.h"
#include "gground_runtime_assets.h"
#include <fstream>

namespace gain_ground {
bool DirectAssetLoader::open(const std::filesystem::path &root, CheckHash check_hash)
{
    payloads_.clear();
    error_.clear();
    bytes_loaded_ = 0;
    if (!check_hash) { error_ = "Asset identity checker is missing"; return false; }
    std::vector<std::vector<std::uint8_t>> loaded;
    loaded.reserve(generated::kRuntimeAssets.size());
    for (const auto &record : generated::kRuntimeAssets) {
        const auto path = root / std::filesystem::path(record.path);
        std::ifstream input(path, std::ios::binary | std::ios::ate);
        if (!input || input.tellg() != std::streampos(record.bytes)) {
            error_ = "Missing or wrong-sized payload: " + path.string();
            return false;
        }
        input.seekg(0);
        std::vector<std::uint8_t> bytes(record.bytes);
        input.read(reinterpret_cast<char *>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
        if (!input || !check_hash(bytes, record.sha256)) {
            error_ = "Payload identity mismatch: " + path.string();
            return false;
        }
        loaded.push_back(std::move(bytes));
    }
    // Publish only a complete, identified set. No host memory changes on failure.
    payloads_ = std::move(loaded);
    return true;
}

bool DirectAssetLoader::load_boot(RuntimeHost &host)
{
    if (!ready()) { error_ = "Extracted payloads are not loaded"; return false; }
    for (const auto &record : generated::kRuntimeAssets)
        if (record.group == "bios" && !host.can_load_bus(record.destination, record.bytes)) {
            error_ = "BIOS payload destination is not writable native memory";
            return false;
        }
    for (std::size_t i = 0; i < generated::kRuntimeAssets.size(); ++i) {
        const auto &record = generated::kRuntimeAssets[i];
        if (record.group != "bios") continue;
        if (!host.load_bus(record.destination, payloads_[i])) {
            error_ = "BIOS payload write failed";
            return false;
        }
        bytes_loaded_ += record.bytes;
    }
    return true;
}

bool DirectAssetLoader::transfer(RuntimeHost &host, std::uint32_t logical_offset,
                                 std::uint32_t destination, std::uint32_t bytes)
{
    if (!ready()) { error_ = "Extracted payloads are not loaded"; return false; }
    destination &= 0xffffffU;
    for (std::size_t i = 0; i < generated::kRuntimeAssets.size(); ++i) {
        const auto &record = generated::kRuntimeAssets[i];
        if (record.logical_offset != logical_offset || record.destination != destination || record.bytes != bytes)
            continue;
        if (!host.load_bus(destination, payloads_[i])) {
            error_ = "Payload destination is not writable native memory";
            return false;
        }
        bytes_loaded_ += bytes;
        return true;
    }
    error_ = "Load request has no extracted offset/destination/length mapping";
    return false;
}
} // namespace gain_ground
