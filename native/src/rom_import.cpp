#include "gain_ground/rom_import.h"
#include "gground_runtime_assets.h"
#include <chrono>
#include <fstream>
#include <stdexcept>

namespace gain_ground { namespace {
void write_file(const std::filesystem::path &path, std::span<const std::uint8_t> bytes) {
    std::filesystem::create_directories(path.parent_path());
    std::ofstream file(path,std::ios::binary | std::ios::trunc);
    file.write(reinterpret_cast<const char *>(bytes.data()),static_cast<std::streamsize>(bytes.size()));
    file.close();
    if (!file) throw std::runtime_error("Cannot save extracted ROM data");
}
}
bool valid_rom_cache(const std::filesystem::path &cache, RomHashCheck hash) {
    if (!hash) return false;
    try {
        const auto bios=read_rom_member(cache,"cpu_a_program.bin",0x40000);
        DirectAssetLoader assets;
        return hash(bios,kBiosSha256) && assets.open(cache,hash);
    } catch (const std::exception &) { return false; }
}
bool import_rom_set(const std::filesystem::path &source, const std::filesystem::path &cache,
                    RomHashCheck hash, std::string &error) {
    std::filesystem::path staging;
    bool staging_owned{};
    try {
        if (!hash) throw std::runtime_error("ROM hash validation is unavailable");
        const auto even=read_rom_member(source,"epr-12187.ic2",0x20000);
        const auto odd=read_rom_member(source,"epr-12186.ic1",0x20000);
        const auto disk=read_rom_member(source,"ds3-5000-03d-rev-a.img",1843200);
        std::vector<std::uint8_t> bios(0x40000);
        for (std::size_t i=0;i<even.size();++i) { bios[2*i]=even[i]; bios[2*i+1]=odd[i]; }
        if (!hash(bios,kBiosSha256) || !hash(disk,"0612e2c2cb1f3a6fcf9e4a301dde787f443d87d428ba24a70bf8845e5d52f29b"))
            throw std::runtime_error("This is not the supported Gain Ground arcade ROM revision (gground, DS3-5000-03D Rev A)");
        for (const auto &asset : generated::kRuntimeAssets) {
            const std::size_t offset=0x5a00U+asset.logical_offset;
            if (offset>disk.size() || asset.bytes>disk.size()-offset ||
                !hash(std::span(disk).subspan(offset,asset.bytes),asset.sha256))
                throw std::runtime_error("Extracted asset identity failed validation");
        }
        if (valid_rom_cache(cache,hash)) return true;
        // This destination belongs to the application. Never replace an existing
        // directory implicitly: the launcher allocates a fresh cache generation.
        if (std::filesystem::exists(cache)) throw std::runtime_error("Import destination already exists");
        staging=cache;
        staging += ".import-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count());
        if (!std::filesystem::create_directories(staging)) throw std::runtime_error("Cannot create import staging directory");
        staging_owned=true;
        write_file(staging/"cpu_a_program.bin",bios);
        for (const auto &asset : generated::kRuntimeAssets)
            write_file(staging/asset.path,std::span(disk).subspan(0x5a00U+asset.logical_offset,asset.bytes));
        if (!valid_rom_cache(staging,hash)) throw std::runtime_error("Cannot verify saved ROM data");
        std::filesystem::rename(staging,cache);
        staging.clear();
        error.clear();
        return true;
    } catch (const std::exception &exception) {
        error=exception.what();
        if (staging_owned && !staging.empty()) { std::error_code ignored; std::filesystem::remove_all(staging,ignored); }
        return false;
    }
}
}
