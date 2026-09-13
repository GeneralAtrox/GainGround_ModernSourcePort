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
#include <array>
#include <cstdio>
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
    std::uint64_t next_yield{4096U};
    std::array<bool,2> finished{};
    bool title_started{}, cpu_b_started{};
    std::uint64_t last_frame{UINT64_MAX};
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
        SetWindowTextW(window, paused ? L"Gain Ground - Paused" : L"Gain Ground");
        ModifyMenuW(GetMenu(window), kPauseCommand, MF_BYCOMMAND | MF_STRING,
            kPauseCommand, paused ? L"&Resume (P)" : L"&Pause (P)");
        DrawMenuBar(window);
    }

    ~RuntimeWindow() { for (auto fiber : cpu_fiber) if (fiber) DeleteFiber(fiber); }

    static void checkpoint(void *argument)
    {
        auto &app = *static_cast<RuntimeWindow *>(argument);
        if (app.host.faulted() || app.host.waiting_for_device() ||
            (!app.host.timed_execution() && app.host.execution_checkpoints() >= app.next_yield)) {
            app.next_yield = app.host.execution_checkpoints() + 4096U;
            if (GetCurrentFiber() != app.ui_fiber) SwitchToFiber(app.ui_fiber);
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
        const auto target_ns = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - epoch).count());
        // Allow enough work to catch up between UI timer messages (~16 ms).
        // This is a work ceiling, not a delay: stop at the wall-clock target.
        const auto host_deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(20);
        do {
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
                next_yield = host.execution_checkpoints() + 4096U;
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
            video.render(host);
            last_frame = devices.frame();
        }
        const auto next = std::min(devices.next_event_ns(), host.next_cpu_deadline_ns());
        if (next <= emulated_ns) { error = L"Device clock did not advance"; return; }
        if (emulated_ns >= target_ns) break;
        emulated_ns = std::min(next, target_ns);
        } while (!host.faulted() && std::chrono::steady_clock::now() < host_deadline);
        if (!output.submit(devices.audio.take_samples())) { error = L"Audio output failed"; return; }
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
        if (app) { app->poll_controllers(window); app->advance(); app->save_failure(); InvalidateRect(window, nullptr, FALSE); }
        return 0;
    case WM_COMMAND:
        if (app && LOWORD(wparam) == kPauseCommand) app->toggle_pause(window);
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
        const auto buffer_dc = CreateCompatibleDC(window_dc);
        const auto buffer = CreateCompatibleBitmap(window_dc,
            std::max<LONG>(1, rect.right), std::max<LONG>(1, rect.bottom));
        const bool buffered = buffer_dc && buffer;
        const auto dc = buffered ? buffer_dc : window_dc;
        const auto previous_bitmap = buffered ? SelectObject(dc, buffer) : nullptr;
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
        if (buffered) {
            RECT client{};
            GetClientRect(window, &client);
            BitBlt(window_dc, 0, 0, client.right, client.bottom, dc, 0, 0, SRCCOPY);
            SelectObject(dc, previous_bitmap);
        }
        if (buffer) DeleteObject(buffer);
        if (buffer_dc) DeleteDC(buffer_dc);
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
    if (!menu || !AppendMenuW(menu, MF_STRING, kPauseCommand, L"&Pause (P)")) {
        if (menu) DestroyMenu(menu);
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
