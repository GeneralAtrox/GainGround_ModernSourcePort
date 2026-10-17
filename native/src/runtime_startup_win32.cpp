#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <commdlg.h>
#include <shellapi.h>
#include <shlobj.h>
#include "gain_ground/runtime_startup.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <mutex>
#include <utility>

namespace gain_ground {
namespace {
std::mutex log_lock;
std::filesystem::path log_file; // Empty: logging is off.

std::string utf8(std::wstring_view text) {
    if (text.empty()) return {};
    const int size=WideCharToMultiByte(CP_UTF8,0,text.data(),static_cast<int>(text.size()),nullptr,0,nullptr,nullptr);
    std::string result(static_cast<std::size_t>(std::max(size,0)),'\0');
    if (size>0) WideCharToMultiByte(CP_UTF8,0,text.data(),static_cast<int>(text.size()),result.data(),size,nullptr,nullptr);
    return result;
}

// Options may appear in any order, each at most once. --bios and --assets go
// together; without them the ROM set is chosen interactively.
bool parse_arguments(int count, wchar_t **args, RuntimePaths &paths, std::filesystem::path &data) {
    const std::pair<std::wstring_view,std::filesystem::path *> options[]{{L"--bios",&paths.bios},
        {L"--assets",&paths.assets},{L"--characters",&paths.characters},{L"--data-directory",&data}};
    for (int index=1; index<count; index+=2) {
        const auto option=std::find_if(std::begin(options),std::end(options),
            [&](const auto &candidate){return candidate.first==args[index];});
        if (option==std::end(options) || index+1>=count || !*args[index+1] || !option->second->empty()) return false;
        *option->second=args[index+1];
    }
    const bool rom=!paths.bios.empty();
    return rom==!paths.assets.empty() && (rom || paths.characters.empty());
}

bool use_data_directory(const std::filesystem::path &data, RuntimePaths &paths) {
    std::error_code ec;
    paths.user_data=std::filesystem::absolute(data,ec);
    if (!ec) std::filesystem::create_directories(paths.user_data,ec);
    if (ec || !std::filesystem::is_directory(paths.user_data)) {
        const auto message=L"Cannot create the data directory:\n"+data.wstring();
        MessageBoxW(nullptr,message.c_str(),L"Gain Ground",MB_OK|MB_ICONERROR);
        return false;
    }
    paths.chosen_data_directory=true;
    const auto logs=paths.user_data/L"logs";
    std::filesystem::create_directories(logs,ec);
    // One log per run: the newest run is the one a crash report needs.
    if (!ec && std::ofstream(logs/L"gain_ground.log",std::ios::trunc)) {
        std::lock_guard lock(log_lock);
        log_file=logs/L"gain_ground.log";
    }
    runtime_log(L"Gain Ground started. Data directory: "+paths.user_data.wstring());
    return true;
}
}

void runtime_log(std::string_view line) {
    std::lock_guard lock(log_lock);
    if (log_file.empty()) return;
    std::ofstream output(log_file,std::ios::app);
    SYSTEMTIME now{};
    GetLocalTime(&now);
    char stamp[32]{};
    std::snprintf(stamp,sizeof(stamp),"%04u-%02u-%02u %02u:%02u:%02u.%03u ",now.wYear,now.wMonth,now.wDay,
        now.wHour,now.wMinute,now.wSecond,now.wMilliseconds);
    output << stamp << line << '\n';
}

void runtime_log(std::wstring_view line) { runtime_log(std::string_view(utf8(line))); }

void report_error(const wchar_t *message, const wchar_t *title) {
    runtime_log(std::wstring(title)+L": "+message);
    MessageBoxW(nullptr,message,title,MB_OK|MB_ICONERROR);
}

int prepare_runtime_paths(RuntimePaths &paths, RomHashCheck hash) {
    int count{};
    auto **args=CommandLineToArgvW(GetCommandLineW(),&count);
    if (!args) return 2;
    std::filesystem::path data;
    const bool valid=parse_arguments(count,args,paths,data);
    LocalFree(args);
    if (!valid) {
        MessageBoxW(nullptr,L"Start without arguments to select your ROM set.\n\nAdvanced usage:\ngain_ground_runtime.exe --bios <cpu_a_program.bin> --assets <extracted directory> [--characters <directory>] [--data-directory <directory>]",
            L"Gain Ground",MB_OK|MB_ICONINFORMATION);
        return 2;
    }
    if (!data.empty()) {
        if (!use_data_directory(data,paths)) return 2;
    } else {
        PWSTR local{};
        if (FAILED(SHGetKnownFolderPath(FOLDERID_LocalAppData,0,nullptr,&local))) {
            MessageBoxW(nullptr,L"Cannot find the local application data directory.",L"Gain Ground",MB_OK|MB_ICONERROR);
            return 2;
        }
        paths.user_data=std::filesystem::path(local)/L"GainGround";
        CoTaskMemFree(local);
    }
    if (!paths.bios.empty()) {
        runtime_log(L"ROM: --bios "+paths.bios.wstring()+L" --assets "+paths.assets.wstring());
        return 1;
    }
    try {
        std::filesystem::create_directories(paths.user_data);
        const auto pointer=paths.user_data/"rom-cache.txt";
        std::string generation;
        std::ifstream(pointer) >> generation;
        // The marker contains only an application-generated directory name.
        if (generation.starts_with("rom-v1-") && generation.find_first_not_of("romv-0123456789")==generation.npos) {
            const auto cache=paths.user_data/generation;
            if(valid_rom_cache(cache,hash)) {
                paths.assets=cache; paths.bios=cache/"cpu_a_program.bin";
                runtime_log("ROM: cached "+generation);
                return 1;
            }
        }
        const auto first_run=std::wstring(L"Find your ROM to extract\n\nSelect your Gain Ground arcade ZIP, or one file in an extracted ROM folder. The set must contain:\n\nepr-12187.ic2\nepr-12186.ic1\nds3-5000-03d-rev-a.img\n\nThe application supplies no game ROMs. Extracted data will be stored in ")+
            (paths.chosen_data_directory ? L"the game's data folder." : L"your local application data folder.");
        if (MessageBoxW(nullptr,first_run.c_str(),L"Gain Ground - First run",MB_OKCANCEL|MB_ICONINFORMATION)!=IDOK) return 0;
        for (;;) {
            std::array<wchar_t,32768> selected{};
            OPENFILENAMEW dialog{};
            dialog.lStructSize=sizeof(dialog);
            dialog.lpstrFilter=L"Gain Ground ROM set (*.zip;*.ic1;*.ic2;*.img)\0*.zip;*.ic1;*.ic2;*.img\0All files\0*.*\0";
            dialog.lpstrFile=selected.data(); dialog.nMaxFile=static_cast<DWORD>(selected.size());
            dialog.lpstrTitle=L"Find your Gain Ground ROM to extract";
            dialog.Flags=OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NOCHANGEDIR|OFN_EXPLORER;
            if(!GetOpenFileNameW(&dialog)) {
                if(!CommDlgExtendedError()) return 0;
                throw std::runtime_error("Cannot open the ROM selection dialog");
            }
            generation="rom-v1-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count());
            const auto cache=paths.user_data/generation;
            std::string error;
            if(import_rom_set(selected.data(),cache,hash,error)) {
                std::ofstream output(pointer,std::ios::trunc);
                output << generation << '\n'; output.close();
                if(!output) throw std::runtime_error("Cannot save the ROM cache location");
                paths.assets=cache; paths.bios=cache/"cpu_a_program.bin";
                runtime_log("ROM: imported "+generation);
                return 1;
            }
            const std::wstring message(error.begin(),error.end());
            runtime_log("ROM import failed: "+error);
            if(MessageBoxW(nullptr,message.c_str(),L"Gain Ground - ROM import failed",MB_RETRYCANCEL|MB_ICONERROR)!=IDRETRY) return 0;
        }
    } catch (const std::exception &exception) {
        const std::string error=exception.what();
        const std::wstring message(error.begin(),error.end());
        report_error(message.c_str(),L"Gain Ground");
        return 2;
    }
}
}
