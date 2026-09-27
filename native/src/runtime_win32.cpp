#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <shellapi.h>
#include <bcrypt.h>
#include <mmsystem.h>
#include <xinput.h>
#include <cstring>
#include "gain_ground/runtime_host.h"
#include "gain_ground/runtime_startup.h"
#include "gain_ground/direct_asset_loader.h"
#include "gain_ground/direct_boot_audio.h"
#include "gain_ground/system24_devices.h"
#include "gain_ground/runtime_input.h"
#include "gain_ground/system24_video.h"
#include "gain_ground/timing_trace_session.h"
#include "gain_ground/host_sampler.h"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <deque>

namespace {
constexpr wchar_t kWindowClass[] = L"GainGroundNativeRuntime";
constexpr UINT kPauseCommand = 1001U;
// Stage menu: 4 rounds of 10 stages map to original stage index round*10 + stage.
constexpr UINT kStageCommandBase = 2000U;
constexpr UINT kStageCount = 40U;
constexpr UINT kStageContinueCommand = kStageCommandBase + kStageCount;
// Test-only commands (no menu entries): sweep player 1 across every clear
// grid cell of the current stage, or stop such a sweep.
constexpr UINT kSweepStartCommand = 3001U;
constexpr UINT kSweepStopCommand = 3002U;
constexpr char kBiosSha256[] = "a42dc284615f58ec035652f178e1bae9b1443e7468e436c578e328dd75dc93ed";

using gain_ground::matches_hash;

struct AudioOutput {
    struct Block { std::vector<std::int16_t> samples; WAVEHDR header{}; };
    HWAVEOUT output{};
    std::deque<std::unique_ptr<Block>> blocks;
    bool open() {
        WAVEFORMATEX format{WAVE_FORMAT_PCM, 2, 62500, 250000, 4, 16, 0};
        return waveOutOpen(&output, WAVE_MAPPER, &format, 0, 0, CALLBACK_NULL) == MMSYSERR_NOERROR;
    }
    bool submit(std::vector<std::int16_t> samples) {
        while (!blocks.empty() && (blocks.front()->header.dwFlags & WHDR_DONE)) {
            waveOutUnprepareHeader(output, &blocks.front()->header, sizeof(WAVEHDR));
            blocks.pop_front();
        }
        if (samples.empty()) return true;
        auto block = std::make_unique<Block>();
        block->samples = std::move(samples);
        block->header.lpData = reinterpret_cast<LPSTR>(block->samples.data());
        block->header.dwBufferLength = static_cast<DWORD>(block->samples.size() * sizeof(std::int16_t));
        if (waveOutPrepareHeader(output, &block->header, sizeof(WAVEHDR)) != MMSYSERR_NOERROR) return false;
        if (waveOutWrite(output, &block->header, sizeof(WAVEHDR)) != MMSYSERR_NOERROR) {
            waveOutUnprepareHeader(output, &block->header, sizeof(WAVEHDR));
            return false;
        }
        blocks.push_back(std::move(block));
        return true;
    }
    ~AudioOutput() {
        if (!output) return;
        waveOutReset(output);
        for (auto &block : blocks) waveOutUnprepareHeader(output, &block->header, sizeof(WAVEHDR));
        waveOutClose(output);
    }
};

struct RuntimeWindow {
    struct Controllers {
        HMODULE library{LoadLibraryExW(L"xinput1_4.dll",nullptr,LOAD_LIBRARY_SEARCH_SYSTEM32)};
        using GetState=DWORD(WINAPI *)(DWORD,XINPUT_STATE *);
        GetState get{};
        std::array<gain_ground::RuntimePad,3> pads;
        std::array<ULONGLONG,3> retry_at{};
        Controllers(){
            if(!library)library=LoadLibraryExW(L"xinput9_1_0.dll",nullptr,LOAD_LIBRARY_SEARCH_SYSTEM32);
            if(library){const auto symbol=GetProcAddress(library,"XInputGetState");static_assert(sizeof(get)==sizeof(symbol));std::memcpy(&get,&symbol,sizeof(get));}
        }
        ~Controllers(){if(library)FreeLibrary(library);}
    } controllers;
    gain_ground::DirectAssetLoader assets;
    gain_ground::RuntimeHost host;
    gain_ground::TimingTraceSession timing_trace;
    gain_ground::System24Devices devices;
    gain_ground::System24Video video;
    AudioOutput output;
    std::array<gain_ground::FunctionContext,2> cpu;
    std::array<gain_ground::FunctionResult,2> result;
    void *ui_fiber{};
    std::array<void *,2> cpu_fiber{};
    struct FiberArgument { RuntimeWindow *app{}; unsigned cpu{}; };
    std::array<FiberArgument,2> arguments;
    std::uint64_t fiber_switches{}; // diagnostics for the progress line
    std::array<bool,2> finished{};
    bool title_started{}, cpu_b_started{};
    std::uint64_t last_frame{UINT64_MAX};
    // Frames are rendered at display rate: every frame at or below real speed,
    // otherwise at most about sixty a second. Painting waits for a new render.
    std::chrono::steady_clock::time_point last_render{};
    bool frame_dirty{};
    std::uint64_t emulated_ns{};
    std::chrono::steady_clock::time_point epoch{std::chrono::steady_clock::now()};
    std::wstring error;
    bool paused{};
    gain_ground::RuntimeKeyboard keyboard;
    struct InputObservation { std::uint64_t ns, frame; unsigned key; bool pressed; };
    std::vector<InputObservation> input_history;
    bool failure_saved{};
    std::chrono::steady_clock::time_point paused_at{};

    void record_input(unsigned key, bool pressed) {
        input_history.push_back({emulated_ns, devices.frame(), key, pressed});
    }

    void release_input(){
        keyboard.release_all(devices);
        for(auto &pad:controllers.pads)pad.release();
        record_input(0U,false);
    }
    void poll_controllers(HWND window){
        const bool focused=GetForegroundWindow()==window;
        const auto now=GetTickCount64();bool pause=false;
        for(unsigned p=0;p<3;++p){
            gain_ground::RuntimePadState sample;
            if(controllers.get && now>=controllers.retry_at[p]){
                XINPUT_STATE state{};
                if(controllers.get(p,&state)==ERROR_SUCCESS)
                    sample={true,state.Gamepad.wButtons,state.Gamepad.sThumbLX,state.Gamepad.sThumbLY};
                else controllers.retry_at[p]=now+1000;
            }
            pause|=controllers.pads[p].update(sample,focused,!paused,[&](unsigned bit,bool pressed){
                const auto key=gain_ground::RuntimeKeyboard::gamepad_key(p,bit);
                keyboard.key(devices,key,pressed);record_input(key,pressed);
            });
        }
        if(pause)toggle_pause(window);
    }

    void save_failure() {
        if (failure_saved || (!host.faulted() && error.empty())) return;
        failure_saved = true;
        // Host-side snapshot after the stopped fiber yielded: no guest reads,
        // calls or debugger expressions execute in the failed context.
        std::error_code ec;
        const auto directory = std::filesystem::path(L"run") / L"last-native-failure";
        std::filesystem::create_directories(directory, ec);
        if (ec) return;
        std::ofstream report(directory / L"status.txt");
        const auto message = status();
        report << std::string(message.begin(), message.end()) << '\n'
               << "emulated_ns=" << emulated_ns << " frame=" << devices.frame() << '\n';
        for (unsigned i = 0; i < cpu.size(); ++i) {
            const auto &c = cpu[i];
            report << "cpu=" << i << " state=" << unsigned(c.state) << " pc=" << c.registers.program_counter
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

    void toggle_pause(HWND window)
    {
        const auto now = std::chrono::steady_clock::now();
        const auto result = paused ? waveOutRestart(output.output) : waveOutPause(output.output);
        if (result != MMSYSERR_NOERROR) {
            error = L"Cannot change audio pause state";
            return;
        }
        if (paused) epoch += now - paused_at;
        else { paused_at = now; release_input(); }
        paused = !paused;
        update_title(window);
        ModifyMenuW(GetMenu(window), kPauseCommand, MF_BYCOMMAND | MF_STRING,
            kPauseCommand, paused ? L"&Resume (P)" : L"&Pause (P)");
        DrawMenuBar(window);
    }

    int selected_stage{-1};

    // Test aids, configured from the environment at startup:
    //   GAIN_GROUND_TEST_SPEED   emulated seconds per wall second (audio is dropped above 1)
    //   GAIN_GROUND_INVULNERABLE player characters ignore hits
    //   GAIN_GROUND_NAV_LOG      diagnostic log shared with the navigation adapter
    double speed{1.0};
    std::FILE *diag{};
    struct Sweep {
        bool active{};
        int cell{-1};
        unsigned frames{}, teleports{}, skipped{};
        std::uint16_t stage{};
        static constexpr int width = 48, height = 62, step = 8;
        unsigned frames_per_cell{8U};
        // The top strip holds the exit zone and the time banner; stepping into the
        // exit with an empty roster ends the player by the game's own rule.
        int min_x{0}, max_x{383}, min_y{0}, max_y{424};
    } sweep;

    void diag_log(const char *format, auto... args)
    {
        if (!diag) return;
        std::fprintf(diag, format, args...);
        std::fputc('\n', diag);
        std::fflush(diag);
    }

    std::uint16_t stage_index() const
    {
        const auto bytes = host.region_bytes(2U);
        return bytes.size() > 0xc03U ? static_cast<std::uint16_t>((bytes[0xc02U] << 8U) | bytes[0xc03U]) : 0U;
    }

    void start_sweep()
    {
        const auto keep = sweep;
        sweep = Sweep{};
        sweep.frames_per_cell = keep.frames_per_cell;
        sweep.min_x = keep.min_x; sweep.max_x = keep.max_x; sweep.min_y = keep.min_y; sweep.max_y = keep.max_y;
        sweep.active = true;
        sweep.stage = stage_index();
        diag_log("sweep start stage %u record %05x", unsigned(sweep.stage), unsigned(host.player_record(0)));
    }

    void stop_sweep(const char *why)
    {
        if (!sweep.active) return;
        sweep.active = false;
        diag_log("sweep %s stage %u teleports %u skipped %u", why, unsigned(sweep.stage), sweep.teleports, sweep.skipped);
    }

    // F355's terrain probes at (x,y), (x,y+18), (x+20,y+18), (x+20,y) against
    // the column-major attribute map, plus the 20x18 footprint on screen.
    bool cell_clear(int x, int y) const
    {
        const auto shared = host.region_bytes(3U);
        if (shared.size() < 0x3bb20U || x - 10 < 0 || x + 20 > 383 || y - 9 < 0 || y + 18 > 495) return false;
        for (int px : {x, x + 20})
            for (int py : {y, y + 18})
                if (shared[0x3af22U + static_cast<std::size_t>(px / 8) * 64U + static_cast<std::size_t>((495 - py) / 8)] != 0U) return false; // any attribute: solid, exit, hazard
        return true;
    }

    // Once per rendered frame while a sweep runs: every few frames move
    // player 1 to the next clear cell, so its update, contacts and the enemies'
    // reactions run from every position the stage offers.
    // The stage phase machine clears the stage on time-up when the byte at
    // 0xd2d reaches 2 (raised by the clock object as its countdown expires).
    // Ten-times speed would expire the clock mid-sweep, so hold that byte.
    void hold_stage_clock()
    {
        host.write_memory_word(2U, 0xd2cU, 0U, 0x00ffU);
    }

    void sweep_step()
    {
        if (host.faulted()) return;
        // After a sweep the clock has long expired underneath; keep holding the
        // time-up until the harness moves the game to another stage.
        if (!sweep.active) { if (sweep.teleports && stage_index() == sweep.stage) hold_stage_clock(); return; }
        if (stage_index() != sweep.stage) { stop_sweep("left stage during"); return; }
        hold_stage_clock();
        if (++sweep.frames < sweep.frames_per_cell) return;
        sweep.frames = 0;
        for (;;) {
            if (++sweep.cell >= Sweep::width * Sweep::height) { stop_sweep("done"); return; }
            const int x = (sweep.cell % Sweep::width) * Sweep::step + 4;
            const int y = (sweep.cell / Sweep::width) * Sweep::step + 4;
            if (x < sweep.min_x || x > sweep.max_x || y < sweep.min_y || y > sweep.max_y || !cell_clear(x, y)) { ++sweep.skipped; continue; }
            {
                // Where the game left the player since the previous placement, and its lifecycle state.
                const auto bytes = host.region_bytes(2U); const auto rec = host.player_record(0U);
                if (rec && rec + 0x70U < bytes.size())
                    diag_log("sweep before frame %llu at %d,%d active %02x mode %04x +3f %02x +44 %04x +46 %04x",
                             static_cast<unsigned long long>(devices.frame()),
                             int(std::int16_t((bytes[rec + 0x12U] << 8) | bytes[rec + 0x13U])), int(std::int16_t((bytes[rec + 0x1aU] << 8) | bytes[rec + 0x1bU])),
                             unsigned(bytes[rec]), (unsigned(bytes[rec + 0x44U]) << 8) | bytes[rec + 0x45U], unsigned(bytes[rec + 0x3fU]),
                             (unsigned(bytes[rec + 0x44U]) << 8) | bytes[rec + 0x45U], (unsigned(bytes[rec + 0x46U]) << 8) | bytes[rec + 0x47U]);
            }
            if (!host.teleport_player(0U, x, y)) { stop_sweep("no player record during"); return; }
            ++sweep.teleports;
            diag_log("sweep teleport frame %llu cell %d x %d y %d", static_cast<unsigned long long>(devices.frame()), sweep.cell, x, y);
            return;
        }
    }

    void update_title(HWND window) const
    {
        std::wstring title = L"Gain Ground";
        if (paused) title += L" - Paused";
        if (selected_stage >= 0) {
            wchar_t next[64]{};
            std::swprintf(next, std::size(next), L" - Jumping to Round %d Stage %d",
                selected_stage / 10 + 1, selected_stage % 10 + 1);
            title += next;
        }
        SetWindowTextW(window, title.c_str());
    }

    // A choice ends the current stage through the original stage-clear
    // sequence and loads the chosen stage, once. Made at the title, it applies
    // on the first frame of the next game; the attract demo never consumes it.
    void select_stage(HWND window, int stage)
    {
        const auto menu = GetMenu(window);
        if (selected_stage >= 0)
            CheckMenuItem(menu, kStageCommandBase + static_cast<UINT>(selected_stage), MF_BYCOMMAND | MF_UNCHECKED);
        selected_stage = stage;
        if (selected_stage >= 0)
            CheckMenuItem(menu, kStageCommandBase + static_cast<UINT>(selected_stage), MF_BYCOMMAND | MF_CHECKED);
        host.set_start_stage(selected_stage);
        update_title(window);
    }

    // Off-screen paint buffer, kept across paints and recreated on resize.
    HDC buffer_dc{}; HBITMAP buffer{}; LONG buffer_width{}, buffer_height{};
    bool ensure_buffer(HDC window_dc, LONG width, LONG height)
    {
        if (buffer_dc && buffer && buffer_width == width && buffer_height == height) return true;
        if (buffer) DeleteObject(buffer);
        if (buffer_dc) DeleteDC(buffer_dc);
        buffer_dc = CreateCompatibleDC(window_dc);
        buffer = buffer_dc ? CreateCompatibleBitmap(window_dc, std::max<LONG>(1, width), std::max<LONG>(1, height)) : nullptr;
        buffer_width = width; buffer_height = height;
        if (buffer_dc && buffer) SelectObject(buffer_dc, buffer);
        return buffer_dc && buffer;
    }
    ~RuntimeWindow() {
        if (buffer) DeleteObject(buffer);
        if (buffer_dc) DeleteDC(buffer_dc);
        for (auto fiber : cpu_fiber) if (fiber) DeleteFiber(fiber);
    }

    static void checkpoint(void *argument)
    {
        auto &app = *static_cast<RuntimeWindow *>(argument);
        if (app.host.faulted() || app.host.waiting_for_device() ||
            (!app.host.timed_execution() && app.host.operations_since_switch() >= 4096U)) {
            app.host.reset_yield_window();
            if (GetCurrentFiber() != app.ui_fiber) { ++app.fiber_switches; SwitchToFiber(app.ui_fiber); }
        }
    }

    static void WINAPI execute(void *argument)
    {
        auto &arg = *static_cast<FiberArgument *>(argument);
        auto &app = *arg.app;
        if (arg.cpu == 0U) {
            // Fast loading omits the BIOS, but its YM/DAC setup is required.
            // Keep the original reset-tail register state for game startup.
            auto boot = app.cpu[0];
            app.result[0] = gain_ground::run_direct_boot_audio(boot);
            if (app.result[0].status != gain_ground::TranslationStatus::complete ||
                app.result[0].control != 1U || app.host.faulted()) {
                app.error = L"BIOS sound initialization failed";
                app.finished[0] = true;
                SwitchToFiber(app.ui_fiber);
                for (;;) SwitchToFiber(app.ui_fiber);
            }
        }
        app.result[arg.cpu] = app.host.run(app.cpu[arg.cpu]);
        app.finished[arg.cpu] = true;
        SwitchToFiber(app.ui_fiber);
        // A completed fiber is never scheduled again.
        for (;;) SwitchToFiber(app.ui_fiber);
    }

    void advance()
    {
        if (paused || !error.empty() || host.faulted()) return;
        if (!title_started) {
            // Approved replacement loading sequence: display the original logo
            // resources for two seconds, then enter direct-loaded game startup.
            if (std::chrono::steady_clock::now() - epoch < std::chrono::seconds(2)) return;
            if (!host.prepare_direct_boot(assets, cpu[0])) { error = L"Direct startup failed"; return; }
            title_started = true;
            epoch = std::chrono::steady_clock::now();
            host.attach_devices(devices);
            gain_ground::prepare_direct_boot_devices(devices);
            // CPU A first runs the original BIOS sound calls on its fiber.
            // Their device clocks remain part of this same emulated timeline.
        }
        const auto target_ns = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - epoch).count() * speed);
        // Allow enough work to catch up between UI timer messages (~16 ms).
        // This is a work ceiling, not a delay: stop at the wall-clock target.
        // Above real speed the same ceiling scales with the speed, so a tick may
        // run several emulated frames; the window only needs to stay responsive.
        const auto host_deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(static_cast<long long>(20 * std::max(1.0, speed)));
        unsigned work_iterations = 0U;
        do {
        // A CPU fiber may have stepped the device clock itself while waiting;
        // the device timer must never be asked to move backwards.
        emulated_ns = std::max(emulated_ns, devices.time_ns());
        devices.advance(emulated_ns);
        for (unsigned i = 0; i < 2; ++i) {
            if (i == 1 && !devices.cpu_b_enabled()) continue;
            if (!host.cpu_ready(i)) continue;
            if (i == 1 && !cpu_b_started) {
                // Approved fast boot also removes CPU-B floppy-speed and CPU
                // calibration loops (843a..84d8). Retain their load request
                // and interrupt setup, then run the original boot-mode selector.
                cpu[1] = {};
                cpu[1].cpu = 1;
                cpu[1].state = 0xdd;
                cpu[1].registers.status = 0x2000;
                cpu[1].registers.address[7] = 0x7ffe;
                cpu[1].registers.program_counter = 0x84da;
                host.write_memory_word(3, 0x38002, 1, 0xffff);
                devices.write(0xa00004, 0x18, 0xffff);
                cpu_b_started = true;
            }
            if (!finished[i] && !host.faulted()) {
                host.select_cpu(i);
                host.reset_yield_window();
                ++fiber_switches;
                SwitchToFiber(cpu_fiber[i]);
            }
            if (finished[i]) {
                wchar_t stopped[256]{};
                std::swprintf(stopped, std::size(stopped),
                    L"Native CPU %c stopped before the scene loop\nPC 0x%06X  State 0x%02X\nResult status %u  Control %u  Exit PC 0x%06X",
                    i ? L'B' : L'A', static_cast<unsigned>(cpu[i].registers.program_counter),
                    static_cast<unsigned>(cpu[i].state), static_cast<unsigned>(result[i].status),
                    static_cast<unsigned>(result[i].control), static_cast<unsigned>(result[i].exit_program_counter));
                error = stopped;
                return;
            }
        }
        if (last_frame != devices.frame()) {
            const auto now = std::chrono::steady_clock::now();
            if (speed <= 1.0 || now - last_render >= std::chrono::milliseconds(16)) {
                video.render(host);
                last_render = now;
                frame_dirty = true;
            }
            last_frame = devices.frame();
            sweep_step();
            // Determinism probe for performance work: the execution checkpoint
            // count at fixed frames must not move when only host code changes.
            if (diag && last_frame % 600U == 0U)
                diag_log("progress frame %llu checkpoints %llu switches %llu", static_cast<unsigned long long>(last_frame),
                         static_cast<unsigned long long>(host.execution_checkpoints()), static_cast<unsigned long long>(fiber_switches));
        }
        const auto next = std::min(devices.next_event_ns(), host.next_cpu_deadline_ns());
        if (next <= emulated_ns) { error = L"Device clock did not advance"; return; }
        if (emulated_ns >= target_ns) break;
        emulated_ns = std::min(next, target_ns);
        // The wall-clock read is comparatively costly on this toolchain; checking
        // the work ceiling every few iterations changes only how long a tick works.
        } while (!host.faulted() && ((++work_iterations & 31U) != 0U || std::chrono::steady_clock::now() < host_deadline));
        auto samples = devices.audio.take_samples();
        if (speed > 1.0) return; // Faster than real time: keep the mixer drained, play nothing.
        if (!output.submit(std::move(samples))) { error = L"Audio output failed"; return; }
    }

    std::wstring status() const
    {
        wchar_t line[640]{};
        if (!error.empty()) return error;
        if (host.faulted()) {
            const auto &f = host.fault();
            const std::wstring message(f.message.begin(), f.message.end());
            const auto &asset_error = assets.error();
            const std::wstring detail(asset_error.begin(), asset_error.end());
            std::swprintf(line, std::size(line),
                L"Runtime stopped: %ls\n\nCPU %c   PC 0x%06X\nAddress 0x%08X   Region %u   Mask 0x%04X\nExecution checkpoints: %llu\n%ls",
                message.c_str(), f.cpu == 0U ? L'A' : L'B',
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
            std::swprintf(line, std::size(line), L"Executing CPU-A after direct asset loading\nPC 0x%06X\nExecution checkpoints: %llu",
                static_cast<unsigned>(cpu[0].registers.program_counter),
                static_cast<unsigned long long>(host.execution_checkpoints()));
        }
        return line;
    }
};

LRESULT CALLBACK window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    auto *app = reinterpret_cast<RuntimeWindow *>(GetWindowLongPtrW(window, GWLP_USERDATA));
    switch (message) {
    case WM_NCCREATE: {
        const auto *create = reinterpret_cast<const CREATESTRUCTW *>(lparam);
        SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create->lpCreateParams));
        return DefWindowProcW(window, message, wparam, lparam);
    }
    case WM_TIMER:
        if (app) {
            app->poll_controllers(window); app->advance(); app->save_failure();
            if (app->host.take_start_stage_applied()) app->select_stage(window, -1);
            if (app->frame_dirty || app->host.faulted() || !app->error.empty()) { app->frame_dirty = false; InvalidateRect(window, nullptr, FALSE); }
        }
        return 0;
    case WM_COMMAND:
        if (app) {
            const UINT command = LOWORD(wparam);
            if (command == kPauseCommand) app->toggle_pause(window);
            else if (command == kStageContinueCommand) app->select_stage(window, -1);
            else if (command == kSweepStartCommand) app->start_sweep();
            else if (command == kSweepStopCommand) app->stop_sweep("stopped");
            else if (command >= kStageCommandBase && command < kStageCommandBase + kStageCount)
                app->select_stage(window, static_cast<int>(command - kStageCommandBase));
        }
        return 0;
    case WM_KEYDOWN: case WM_KEYUP:
        if (app) {
            const bool pressed = message == WM_KEYDOWN;
            if (wparam == VK_ESCAPE && pressed) { DestroyWindow(window); return 0; }
            if (wparam == 'P' || wparam == VK_PAUSE) {
                if (pressed && !(lparam & (1LL << 30))) app->toggle_pause(window);
                return 0;
            }
            // Release held controls while paused, but do not queue new presses.
            if (app->paused && pressed) return 0;
            if (app->keyboard.key(app->devices, static_cast<unsigned>(wparam), pressed) &&
                (!pressed || !(lparam & (1LL << 30))))
                app->record_input(static_cast<unsigned>(wparam), pressed);
        }
        return 0;
    case WM_KILLFOCUS:
        if (app) app->release_input();
        return 0;
    case WM_LBUTTONDOWN: case WM_LBUTTONUP: case WM_RBUTTONDOWN: case WM_RBUTTONUP:
        if(app){
            const bool pressed=message==WM_LBUTTONDOWN || message==WM_RBUTTONDOWN;
            if(app->paused && pressed)return 0;
            const auto key=(message==WM_LBUTTONDOWN || message==WM_LBUTTONUP)
                ? gain_ground::RuntimeKeyboard::mouse_primary:gain_ground::RuntimeKeyboard::mouse_secondary;
            if(pressed)SetCapture(window);
            app->keyboard.key(app->devices,key,pressed);app->record_input(key,pressed);
            if(!pressed && !(wparam&(MK_LBUTTON|MK_RBUTTON)))ReleaseCapture();
        }
        return 0;
    case WM_CAPTURECHANGED:
        if(app)for(auto key:{gain_ground::RuntimeKeyboard::mouse_primary,gain_ground::RuntimeKeyboard::mouse_secondary}){
            app->keyboard.key(app->devices,key,false);app->record_input(key,false);
        }
        return 0;
    case WM_ERASEBKGND:
        // WM_PAINT composes the complete client area, including its margins.
        return 1;
    case WM_PAINT: {
        PAINTSTRUCT paint{};
        const auto window_dc = BeginPaint(window, &paint);
        RECT rect{};
        GetClientRect(window, &rect);
        const bool buffered = app && app->ensure_buffer(window_dc, rect.right, rect.bottom);
        const auto dc = buffered ? app->buffer_dc : window_dc;
        // Clear and compose off-screen so a black intermediate paint cannot
        // become visible between FillRect and StretchDIBits.
        FillRect(dc, &rect, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
        if (app) {
            const auto scale = std::min(double(rect.right) / 384.0, double(rect.bottom) / 496.0);
            const int width = int(384 * scale), height = int(496 * scale);
            BITMAPINFO info{};
            info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            info.bmiHeader.biWidth = 384;
            info.bmiHeader.biHeight = -496;
            info.bmiHeader.biPlanes = 1;
            info.bmiHeader.biBitCount = 32;
            info.bmiHeader.biCompression = BI_RGB;
            SetStretchBltMode(dc, COLORONCOLOR);
            StretchDIBits(dc, (rect.right - width) / 2, (rect.bottom - height) / 2,
                width, height, 0, 0, 384, 496, app->video.pixels().data(), &info, DIB_RGB_COLORS, SRCCOPY);
        }
        SetBkMode(dc, TRANSPARENT);
        SetTextColor(dc, RGB(230, 230, 230));
        rect.left += 20; rect.right -= 20; rect.top += 20;
        if (app && (app->host.faulted() || !app->error.empty())) {
            SetBkMode(dc, OPAQUE);
            SetBkColor(dc, RGB(0,0,0));
            const auto text = app->status();
            DrawTextW(dc, text.c_str(), -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);
        }
        if (buffered) BitBlt(window_dc, 0, 0, rect.right, rect.bottom, dc, 0, 0, SRCCOPY);
        EndPaint(window, &paint);
        return 0;
    }
    case WM_DESTROY:
        KillTimer(window, 1U);
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(window, message, wparam, lparam);
    }
}
} // namespace

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show)
{
    gain_ground::RuntimePaths paths;
    const int startup = gain_ground::prepare_runtime_paths(paths, &matches_hash);
    if (startup != 1) return startup;
    const auto &bios_path = paths.bios;
    const auto &asset_path = paths.assets;
    const auto &character_path = paths.characters;
    std::ifstream input(bios_path, std::ios::binary | std::ios::ate);
    if (!input || input.tellg() != std::streampos(0x40000)) {
        MessageBoxW(nullptr, L"Cannot read the 256 KiB CPU-A BIOS image.", L"Gain Ground", MB_OK | MB_ICONERROR);
        return 2;
    }
    input.seekg(0);
    std::vector<std::uint8_t> bios(0x40000U);
    input.read(reinterpret_cast<char *>(bios.data()), static_cast<std::streamsize>(bios.size()));
    if (!input || !matches_hash(bios, kBiosSha256)) {
        MessageBoxW(nullptr, L"CPU-A ROM identity differs from the original image used by the native translation.", L"Gain Ground", MB_OK | MB_ICONERROR);
        return 2;
    }
    auto app = std::make_unique<RuntimeWindow>();
    // Test aids from the environment; absent variables leave normal play untouched.
    if (const char *value = std::getenv("GAIN_GROUND_TEST_SPEED"); value && *value) {
        const auto requested = std::strtod(value, nullptr);
        if (requested >= 0.1 && requested <= 100.0) app->speed = requested;
    }
    if (const char *value = std::getenv("GAIN_GROUND_INVULNERABLE"); value && *value && *value != '0')
        app->host.set_player_invulnerable(true);
    if (const char *value = std::getenv("GAIN_GROUND_NAV_LOG"); value && *value)
        app->diag = std::fopen(value, "a");
    // Statistical sampling of this thread for performance work (see host_sampler.h).
    gain_ground::HostSampler sampler;
    if (const char *value = std::getenv("GAIN_GROUND_SAMPLE"); value && *value) sampler.start(value);
    if (const char *value = std::getenv("GAIN_GROUND_SWEEP_BOUNDS"); value && *value) {
        int x0, x1, y0, y1;
        if (std::sscanf(value, "%d,%d,%d,%d", &x0, &x1, &y0, &y1) == 4) { app->sweep.min_x = x0; app->sweep.max_x = x1; app->sweep.min_y = y0; app->sweep.max_y = y1; }
    }
    if (const char *value = std::getenv("GAIN_GROUND_SWEEP_FRAMES"); value && *value) {
        const auto frames = std::strtoul(value, nullptr, 10);
        if (frames >= 1U && frames <= 600U) app->sweep.frames_per_cell = static_cast<unsigned>(frames);
    }
    if (!character_path.empty()) {
        std::string error;
        if (!app->host.load_character_definitions(character_path, error)) {
            const std::wstring message(error.begin(), error.end());
            MessageBoxW(nullptr, message.c_str(), L"Gain Ground characters", MB_OK | MB_ICONERROR);
            return 2;
        }
    }
    // Optional user-provided replacement, never embedded in the source build.
    std::ifstream music(paths.user_data / "title_music.pcm", std::ios::binary | std::ios::ate);
    if (music && music.tellg() > 0 && music.tellg() <= 64 * 1024 * 1024) {
        std::vector<std::uint8_t> samples(static_cast<std::size_t>(music.tellg()));
        music.seekg(0);
        music.read(reinterpret_cast<char *>(samples.data()), static_cast<std::streamsize>(samples.size()));
        if (music) app->devices.audio.set_title_music(samples);
    }
    try {
        app->timing_trace.configure("native");
        if (app->timing_trace.enabled())
            app->host.set_timing_observer([](void *session, const gain_ground::TimingTraceEvent &event) {
                static_cast<gain_ground::TimingTraceSession *>(session)->observe(event);
            }, &app->timing_trace);
    } catch (const std::exception &e) {
        const std::string text = e.what();
        const std::wstring message(text.begin(), text.end());
        MessageBoxW(nullptr, message.c_str(), L"Gain Ground timing trace", MB_OK | MB_ICONERROR);
        return 2;
    }
    if (!app->assets.open(asset_path, &matches_hash)) {
        const auto &error = app->assets.error();
        const std::wstring message(error.begin(), error.end());
        MessageBoxW(nullptr, message.c_str(), L"Gain Ground assets", MB_OK | MB_ICONERROR);
        return 2;
    }
    if (!app->host.load_region(1U, 0U, bios)) return 2;
    if (!app->host.prepare_direct_boot(app->assets, app->cpu[0])) {
        MessageBoxW(nullptr, L"Cannot initialize direct asset loading.", L"Gain Ground", MB_OK | MB_ICONERROR);
        return 2;
    }
    app->host.prepare_sega_logo();
    app->video.render(app->host);
    if (!app->output.open()) {
        MessageBoxW(nullptr, L"Cannot open stereo audio output.", L"Gain Ground", MB_OK | MB_ICONERROR);
        return 2;
    }
    if (app->host.faulted()) return 2;
    const bool own_fiber = !IsThreadAFiber();
    app->ui_fiber = own_fiber ? ConvertThreadToFiberEx(nullptr, FIBER_FLAG_FLOAT_SWITCH) : GetCurrentFiber();
    if (!app->ui_fiber) return 2;
    for (unsigned i = 0; i < 2; ++i) {
        app->arguments[i] = {app.get(), i};
        app->cpu_fiber[i] = CreateFiberEx(0, 8U * 1024U * 1024U, FIBER_FLAG_FLOAT_SWITCH,
                                         &RuntimeWindow::execute, &app->arguments[i]);
        if (!app->cpu_fiber[i]) { app.reset(); if (own_fiber) ConvertFiberToThread(); return 2; }
    }
    app->host.set_checkpoint(&RuntimeWindow::checkpoint, app.get());
    // In-place waits reproduce this scheduler's stepping exactly; GAIN_GROUND_DIRECT_WAIT=0 compares without them.
    if (const char *value = std::getenv("GAIN_GROUND_DIRECT_WAIT"); !(value && *value == '0')) app->host.set_direct_wait(true);
    WNDCLASSW klass{};
    klass.lpfnWndProc = &window_proc;
    klass.hInstance = instance;
    klass.hIcon = static_cast<HICON>(LoadImageW(nullptr, (paths.user_data / "game.ico").c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE));
    if (!klass.hIcon) klass.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
    klass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    klass.lpszClassName = kWindowClass;
    if (!RegisterClassW(&klass)) {
        app.reset();
        if (own_fiber) ConvertFiberToThread();
        return 2;
    }
    const auto menu = CreateMenu();
    const auto stage_menu = CreatePopupMenu();
    bool menu_ok = menu && stage_menu && AppendMenuW(menu, MF_STRING, kPauseCommand, L"&Pause (P)");
    for (UINT round = 0; menu_ok && round < kStageCount / 10U; ++round) {
        const auto round_menu = CreatePopupMenu();
        menu_ok = round_menu != nullptr;
        for (UINT stage = 0; menu_ok && stage < 10U; ++stage) {
            wchar_t label[32]{};
            std::swprintf(label, std::size(label), L"Stage &%u", stage + 1U);
            menu_ok = AppendMenuW(round_menu, MF_STRING, kStageCommandBase + round * 10U + stage, label);
        }
        wchar_t label[32]{};
        std::swprintf(label, std::size(label), L"Round &%u", round + 1U);
        menu_ok = menu_ok && AppendMenuW(stage_menu, MF_POPUP, reinterpret_cast<UINT_PTR>(round_menu), label);
    }
    menu_ok = menu_ok && AppendMenuW(stage_menu, MF_SEPARATOR, 0, nullptr) &&
        AppendMenuW(stage_menu, MF_STRING, kStageContinueCommand, L"&Continue normally") &&
        AppendMenuW(menu, MF_POPUP, reinterpret_cast<UINT_PTR>(stage_menu), L"&Stage");
    if (!menu_ok) {
        if (menu) DestroyMenu(menu); // Attached popups are destroyed with it.
        else if (stage_menu) DestroyMenu(stage_menu);
        app.reset();
        if (own_fiber) ConvertFiberToThread();
        return 2;
    }
    const auto window = CreateWindowExW(0, kWindowClass, L"Gain Ground",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 520, 620,
        nullptr, menu, instance, app.get());
    if (!window) { DestroyMenu(menu); app.reset(); if (own_fiber) ConvertFiberToThread(); return 2; }
    ShowWindow(window, show);
    // UI scheduling only; device clocks use their own source frequencies.
    app->epoch = std::chrono::steady_clock::now();
    if (!SetTimer(window, 1U, 1U, nullptr)) {
        DestroyWindow(window);
        app.reset();
        if (own_fiber) ConvertFiberToThread();
        return 2;
    }
    MSG message{};
    BOOL received{};
    while ((received = GetMessageW(&message, nullptr, 0, 0)) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    if (received < 0) DestroyWindow(window);
    app.reset();
    if (own_fiber) ConvertFiberToThread();
    return received < 0 ? 2 : static_cast<int>(message.wParam);
}
