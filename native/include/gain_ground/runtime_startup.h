#pragma once
#include "gain_ground/rom_import.h"
namespace gain_ground {
struct RuntimePaths {
    std::filesystem::path bios, assets, characters, user_data;
};
// 1: ready; 0: cancelled; 2: invalid command line/startup failure.
int prepare_runtime_paths(RuntimePaths &paths, RomHashCheck hash);
}
