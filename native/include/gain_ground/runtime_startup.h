#pragma once
#include "gain_ground/rom_import.h"
#include <string_view>
namespace gain_ground {
struct RuntimePaths {
    std::filesystem::path bios, assets, characters, user_data;
    // Set by --data-directory: every writable file then stays under user_data.
    bool chosen_data_directory{};
};
// 1: ready; 0: cancelled; 2: invalid command line/startup failure.
int prepare_runtime_paths(RuntimePaths &paths, RomHashCheck hash);
// Appends a timestamped line to user_data/logs/gain_ground.log when
// --data-directory was given; otherwise does nothing. Safe from any thread.
void runtime_log(std::string_view line);
void runtime_log(std::wstring_view line);
// Logs the message, then shows it in an error message box.
void report_error(const wchar_t *message, const wchar_t *title);
}
