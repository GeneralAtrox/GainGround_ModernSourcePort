#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <commdlg.h>
#include <shellapi.h>
#include <shlobj.h>
#include "gain_ground/runtime_startup.h"
#include <array>
#include <chrono>
#include <fstream>

namespace gain_ground {
int prepare_runtime_paths(RuntimePaths &paths, RomHashCheck hash) {
    PWSTR local{};
    if (FAILED(SHGetKnownFolderPath(FOLDERID_LocalAppData,0,nullptr,&local))) {
        MessageBoxW(nullptr,L"Cannot find the local application data directory.",L"Gain Ground",MB_OK|MB_ICONERROR);
        return 2;
    }
    paths.user_data=std::filesystem::path(local)/L"GainGround";
    CoTaskMemFree(local);
    int count{};
    auto **args=CommandLineToArgvW(GetCommandLineW(),&count);
    if (!args) return 2;
    if (count!=1) {
        const bool valid=(count==5 || count==7) && std::wstring_view(args[1])==L"--bios" &&
            std::wstring_view(args[3])==L"--assets" && (count!=7 || std::wstring_view(args[5])==L"--characters");
        if (valid) {
            paths.bios=args[2]; paths.assets=args[4];
            if(count==7) paths.characters=args[6];
        }
        LocalFree(args);
        if(valid) return 1;
        MessageBoxW(nullptr,L"Start without arguments to select your ROM set.\n\nAdvanced usage:\ngain_ground_runtime.exe --bios <cpu_a_program.bin> --assets <extracted directory> [--characters <directory>]",
            L"Gain Ground",MB_OK|MB_ICONINFORMATION);
        return 2;
    }
    LocalFree(args);
    try {
        std::filesystem::create_directories(paths.user_data);
        const auto pointer=paths.user_data/"rom-cache.txt";
        std::string generation;
        std::ifstream(pointer) >> generation;
        // The marker contains only an application-generated directory name.
        if (generation.starts_with("rom-v1-") && generation.find_first_not_of("romv-0123456789")==generation.npos) {
            const auto cache=paths.user_data/generation;
            if(valid_rom_cache(cache,hash)) { paths.assets=cache; paths.bios=cache/"cpu_a_program.bin"; return 1; }
        }
        if (MessageBoxW(nullptr,L"Find your ROM to extract\n\nSelect your Gain Ground arcade ZIP, or one file in an extracted ROM folder. The set must contain:\n\nepr-12187.ic2\nepr-12186.ic1\nds3-5000-03d-rev-a.img\n\nThe application supplies no game ROMs. Extracted data will be stored in your local application data folder.",
            L"Gain Ground - First run",MB_OKCANCEL|MB_ICONINFORMATION)!=IDOK) return 0;
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
                return 1;
            }
            const std::wstring message(error.begin(),error.end());
            if(MessageBoxW(nullptr,message.c_str(),L"Gain Ground - ROM import failed",MB_RETRYCANCEL|MB_ICONERROR)!=IDRETRY) return 0;
        }
    } catch (const std::exception &exception) {
        const std::string error=exception.what();
        const std::wstring message(error.begin(),error.end());
        MessageBoxW(nullptr,message.c_str(),L"Gain Ground",MB_OK|MB_ICONERROR);
        return 2;
    }
}
}
