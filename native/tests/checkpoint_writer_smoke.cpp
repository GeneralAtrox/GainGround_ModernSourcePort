#include "gain_ground/checkpoint_writer.h"

#include "gground_checkpoints.h"
#include "gground_memory_map.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void require(bool condition, const std::string &message)
{
    if (!condition)
        throw std::runtime_error(message);
}

std::vector<char> read_file(const std::filesystem::path &path)
{
    std::ifstream stream(path, std::ios::binary);
    require(static_cast<bool>(stream), "cannot read " + path.string());
    return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
}

gain_ground::CheckpointStreamIdentity identity()
{
    gain_ground::CheckpointStreamIdentity result;
    result.base_mame_commit = gain_ground::checkpoint_sha1(gain_ground::generated::kMemoryMapMameCommit);
    result.identity_sha256 = gain_ground::checkpoint_sha256(
        "1111111111111111111111111111111111111111111111111111111111111111");
    result.rom_manifest_sha256 = gain_ground::checkpoint_sha256(
        "2222222222222222222222222222222222222222222222222222222222222222");
    result.executable_sha256 = gain_ground::checkpoint_sha256(
        "3333333333333333333333333333333333333333333333333333333333333333");
    result.configuration_sha256 = gain_ground::checkpoint_sha256(
        "4444444444444444444444444444444444444444444444444444444444444444");
    result.replay_sha256 = gain_ground::checkpoint_sha256(
        "5555555555555555555555555555555555555555555555555555555555555555");
    result.source_manifest_sha256 = gain_ground::checkpoint_sha256(
        "6666666666666666666666666666666666666666666666666666666666666666");
    return result;
}

void write_projection(const std::filesystem::path &path)
{
    gain_ground::CheckpointProjectionWriter writer(path, identity());
    gain_ground::CheckpointPayload reset;
    reset.u64(0);
    writer.write_event(2, 0, 0, reset.view());

    gain_ground::CheckpointPayload snapshot;
    snapshot.u16(1).u16(1).u32(0x00040000U).u32(4).u32(0).u64(0xffffffffffffffffULL);
    const std::vector<std::uint8_t> words{0x12, 0x34, 0xab, 0xcd};
    snapshot.bytes(words);
    writer.write_event(3, 0, 0, snapshot.view());
    require(writer.event_count() == 2, "writer event ordinal/count differs");
    writer.finish();
}

} // namespace

int main(int argc, char **argv)
{
    try {
        require(argc == 3, "usage: gain_ground_checkpoint_writer_smoke <first.ggcap> <second.ggcap>");
        const std::filesystem::path first(argv[1]);
        const std::filesystem::path second(argv[2]);
        write_projection(first);
        write_projection(second);
        const auto first_bytes = read_file(first);
        const auto second_bytes = read_file(second);
        require(first_bytes == second_bytes, "repeated checkpoint projections are not byte-identical");
        require(first_bytes.size() > 256, "checkpoint projection has no compressed event stream");
        require(gain_ground::generated::kCaptureCheckpointEvents.size() == 13,
            "generated selected-event inventory differs");
        std::cout << "Gain Ground checkpoint writer smoke passed\n"
                  << "  selected events written: 2\n"
                  << "  repeated streams: byte-identical\n";
        return 0;
    } catch (const std::exception &error) {
        std::cerr << "ERROR: " << error.what() << '\n';
        return 1;
    }
}
