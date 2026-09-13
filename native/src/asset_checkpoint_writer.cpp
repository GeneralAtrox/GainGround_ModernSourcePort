#include "gain_ground/asset_checkpoint_writer.h"

#include "gground_assets.h"
#include "gground_checkpoints.h"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

namespace gain_ground {
namespace {

[[noreturn]] void fail(const std::string &message)
{
    throw std::runtime_error("Gain Ground asset checkpoint writer: " + message);
}

std::string prefix(std::uint32_t asset_set)
{
    std::ostringstream result;
    result << "asset-set-" << std::setw(2) << std::setfill('0') << asset_set << '-';
    return result.str();
}

void write_file(
    const std::filesystem::path &path,
    std::span<const std::uint8_t> bytes)
{
    std::ofstream stream(path, std::ios::binary | std::ios::trunc);
    if (!stream)
        fail("cannot open " + path.string());
    if (!bytes.empty())
        stream.write(reinterpret_cast<const char *>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    stream.close();
    if (!stream)
        fail("cannot write " + path.string());
}

void require_size(std::span<const std::uint8_t> bytes, std::uint32_t expected, const char *name)
{
    if (bytes.size() != expected) {
        fail(std::string(name) + " has " + std::to_string(bytes.size()) +
            " bytes; contract requires " + std::to_string(expected));
    }
}

} // namespace

AssetCheckpointWriter::AssetCheckpointWriter(std::filesystem::path output_root) :
    output_root_(std::move(output_root))
{
    std::error_code error;
    std::filesystem::create_directories(output_root_, error);
    if (error)
        fail("cannot create " + output_root_.string() + ": " + error.message());
}

void AssetCheckpointWriter::write_asset_set(
    std::uint32_t asset_set,
    std::span<const std::uint8_t> primary_retained,
    std::span<const std::uint8_t> metatile_retained,
    std::span<const std::uint8_t> attribute_workspace)
{
    if (finished_)
        fail("cannot append an asset set after finish");
    if (asset_set < generated::kAssetSetCheckpointFirst || asset_set > generated::kAssetSetCheckpointLast)
        fail("asset-set number is outside the generated checkpoint contract");
    const auto index = static_cast<std::size_t>(asset_set - generated::kAssetSetCheckpointFirst);
    if (written_.at(index))
        fail("asset set " + std::to_string(asset_set) + " was submitted twice");

    require_size(primary_retained, generated::kPrimaryRetainedBytes, "primary retained output");
    require_size(metatile_retained, generated::kMetatileRetainedBytes, "metatile retained output");
    require_size(attribute_workspace, generated::kAttributeRetainedBytes, "attribute workspace");
    const auto file_prefix = prefix(asset_set);
    write_file(output_root_ / (file_prefix + "primary-retained.bin"), primary_retained);
    write_file(output_root_ / (file_prefix + "metatile-retained.bin"), metatile_retained);
    write_file(output_root_ / (file_prefix + "attribute-workspace.bin"), attribute_workspace);
    written_[index] = true;
    ++asset_set_count_;
}

void AssetCheckpointWriter::finish()
{
    if (finished_)
        return;
    if (asset_set_count_ != written_.size()) {
        for (std::size_t index = 0; index != written_.size(); ++index) {
            if (!written_[index])
                fail("asset set " + std::to_string(index + generated::kAssetSetCheckpointFirst) + " is missing");
        }
        fail("asset-set inventory differs");
    }
    finished_ = true;
}

} // namespace gain_ground
