#include "runtime_win32_internal.h"
namespace runtime_win32_detail {
void RuntimeWindow::save_failure(){
        if (failure_saved || (!host.faulted() && error.empty())) return;
        failure_saved = true;
        // Host-side snapshot after the native continuation yielded: no guest reads,
        // calls or debugger expressions execute in the failed context.
        std::error_code ec;
        const auto directory = std::filesystem::path(L"run") / L"last-native-failure";
        std::filesystem::create_directories(directory, ec);
        if (ec) return;
        std::ofstream report(directory / L"status.txt");
        const auto message = status();
        report << std::string(message.begin(), message.end()) << '\n'
               << "emulated_ns=" << emulated_ns << " frame=" << devices.frame() << '\n';
        {
            const auto &c = game.context();
            report << "native-game state=" << unsigned(c.state) << " pc=" << c.registers.program_counter
                   << " sr=" << c.registers.status << '\n';
            for (unsigned reg = 0; reg < 8U; ++reg)
                report << "D" << reg << '=' << c.registers.data[reg]
                       << " A" << reg << '=' << c.registers.address[reg] << '\n';
        }
        std::ofstream inputs(directory / L"inputs.csv");
        inputs << "emulated_ns,frame,key,pressed\n";
        for (const auto &event : input_history)
            inputs << event.ns << ',' << event.frame << ',' << event.key << ',' << event.pressed << '\n';
        for (const auto region : {2U, 3U}) {
            const auto bytes = host.region_bytes(region);
            std::ofstream memory(directory / (region == 2U ? L"cpu-b-ram.bin" : L"cpu-a-shared-ram.bin"), std::ios::binary);
            memory.write(reinterpret_cast<const char *>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
        }
    }

void RuntimeWindow::toggle_pause(){
        const auto now = std::chrono::steady_clock::now();
        const auto result = paused ? waveOutRestart(output.output) : waveOutPause(output.output);
        if (result != MMSYSERR_NOERROR) {
            error = L"Cannot change audio pause state";
            return;
        }
        if (paused) epoch += now - paused_at;
        else { paused_at = now; release_input(); }
        paused = !paused;
        publish_state();
    }

void RuntimeWindow::show_state(){
        bool is_paused, unlimited;
        int stage;
        {
            std::lock_guard lock(shared_lock);
            is_paused = shared.paused; unlimited = shared.unlimited_credits; stage = shared.selected_stage;
        }
        std::wstring title = L"Gain Ground";
        if (is_paused) title += L" - Paused";
        if (unlimited) title += L" - Unlimited credits";
        if (stage >= 0) {
            wchar_t next[64]{};
            std::swprintf(next, std::size(next), L" - Jumping to Round %d Stage %d", stage / 10 + 1, stage % 10 + 1);
            title += next;
        }
        SetWindowTextW(window, title.c_str());
        const auto menu = GetMenu(window);
        ModifyMenuW(menu, kPauseCommand, MF_BYCOMMAND | MF_STRING, kPauseCommand, is_paused ? L"&Resume (P)" : L"&Pause (P)");
        CheckMenuItem(menu, kUnlimitedCreditsCommand, MF_BYCOMMAND | (unlimited ? MF_CHECKED : MF_UNCHECKED));
        if (checked_stage >= 0)
            CheckMenuItem(menu, kStageCommandBase + static_cast<UINT>(checked_stage), MF_BYCOMMAND | MF_UNCHECKED);
        checked_stage = stage;
        if (checked_stage >= 0)
            CheckMenuItem(menu, kStageCommandBase + static_cast<UINT>(checked_stage), MF_BYCOMMAND | MF_CHECKED);
        DrawMenuBar(window);
    }

void RuntimeWindow::select_stage(int stage){
        selected_stage = stage;
        host.set_start_stage(selected_stage);
        publish_state();
    }

bool RuntimeWindow::ensure_buffer(HDC window_dc, LONG width, LONG height){
        if (buffer_dc && buffer && buffer_width == width && buffer_height == height) return true;
        if (buffer) DeleteObject(buffer);
        if (buffer_dc) DeleteDC(buffer_dc);
        buffer_dc = CreateCompatibleDC(window_dc);
        buffer = buffer_dc ? CreateCompatibleBitmap(window_dc, std::max<LONG>(1, width), std::max<LONG>(1, height)) : nullptr;
        buffer_width = width; buffer_height = height;
        if (buffer_dc && buffer) SelectObject(buffer_dc, buffer);
        return buffer_dc && buffer;
    }

std::wstring RuntimeWindow::status() const{
        wchar_t line[640]{};
        if (!error.empty()) return error;
        if (host.faulted()) {
            const auto &f = host.fault();
            const std::wstring message(f.message.begin(), f.message.end());
            const auto &asset_error = assets.error();
            const std::wstring detail(asset_error.begin(), asset_error.end());
            std::swprintf(line, std::size(line),
                L"Runtime stopped: %ls\n\n%ls service   Source PC 0x%06X\nAddress 0x%08X   Region %u   Mask 0x%04X\nExecution checkpoints: %llu\n%ls",
                message.c_str(), f.cpu == 0U ? L"System" : L"Gameplay",
                static_cast<unsigned>(f.pc), static_cast<unsigned>(f.address),
                static_cast<unsigned>(f.region), static_cast<unsigned>(f.mask),
                static_cast<unsigned long long>(host.execution_checkpoints()), detail.c_str());
            if (f.function_id != UINT32_MAX) {
                wchar_t function[160]{};
                std::swprintf(function, std::size(function),
                    L"\nFunction %u  Entry 0x%06X  State 0x%02X\nResult %u  Control %u",
                    f.function_id, f.function_entry, unsigned(f.state), unsigned(f.result_status), unsigned(f.result_control));
                return std::wstring(line) + function;
            }
        } else {
            std::swprintf(line, std::size(line), L"Executing native game loop\nSource PC 0x%06X\nExecution checkpoints: %llu",
                static_cast<unsigned>(game.context().registers.program_counter),
                static_cast<unsigned long long>(host.execution_checkpoints()));
        }
        return line;
    }
}
