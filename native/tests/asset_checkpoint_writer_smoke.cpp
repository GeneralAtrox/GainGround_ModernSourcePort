#include "gain_ground/asset_checkpoint_writer.h"

#include "gground_assets.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void require(bool condition, const std::string &message)
{
    if (!condition)
        throw std::runtime_error(message);
}

std::vector<std::uint8_t> read_file(const std::filesystem::path &path)
{
    std::ifstream stream(path, std::ios::binary);
    require(static_cast<bool>(stream), "cannot read " + path.string());
    return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
}

std::string set_directory(std::uint32_t asset_set)
{
    std::ostringstream result;
    result << std::setw(2) << std::setfill('0') << asset_set;
    return result.str();
}

void write_all(
    const std::filesystem::path &source_root,
    const std::filesystem::path &output_root)
{
    gain_ground::AssetCheckpointWriter writer(output_root);
    for (std::uint32_t asset_set = 1; asset_set <= 42; ++asset_set) {
        const auto source = source_root / "asset-sets" / set_directory(asset_set);
        const auto primary = read_file(source / "primary-workspace.bin");
        const auto metatile = read_file(source / "metatile-workspace.bin");
        const auto attribute = read_file(source / "attribute-workspace.bin");
        require(primary.size() >= gain_ground::generated::kPrimaryRetainedBytes,
            "primary source is shorter than its retained output");
        require(metatile.size() >= gain_ground::generated::kMetatileRetainedBytes,
            "metatile source is shorter than its retained output");
        writer.write_asset_set(
            asset_set,
            std::span(primary).first(gain_ground::generated::kPrimaryRetainedBytes),
            std::span(metatile).first(gain_ground::generated::kMetatileRetainedBytes),
            attribute);
    }
    require(writer.asset_set_count() == 42, "writer asset-set count differs");
    writer.finish();
}

void compare_outputs(
    const std::filesystem::path &first,
    const std::filesystem::path &second)
{
    for (std::uint32_t asset_set = 1; asset_set <= 42; ++asset_set) {
        std::ostringstream prefix;
        prefix << "asset-set-" << std::setw(2) << std::setfill('0') << asset_set << '-';
        for (const char *suffix : {"primary-retained.bin", "metatile-retained.bin", "attribute-workspace.bin"}) {
            const auto name = prefix.str() + suffix;
            require(read_file(first / name) == read_file(second / name),
                "repeated asset checkpoint differs: " + name);
        }
    }
}

} // namespace

int main(int argc, char **argv)
{
    try {
        require(argc == 4,
            "usage: gain_ground_asset_checkpoint_writer_smoke <analysis/assets> <first-output> <second-output>");
        const std::filesystem::path source(argv[1]);
        const std::filesystem::path first(argv[2]);
        const std::filesystem::path second(argv[3]);
        write_all(source, first);
        write_all(source, second);
        compare_outputs(first, second);
        std::cout << "Gain Ground asset checkpoint writer smoke passed\n"
                  << "  asset sets: 42\n"
                  << "  checkpoint outputs: 126\n"
                  << "  repeated outputs: byte-identical\n";
        return 0;
    } catch (const std::exception &error) {
        std::cerr << "ERROR: " << error.what() << '\n';
        return 1;
    }
}
