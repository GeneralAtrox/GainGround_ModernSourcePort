#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include "phangrade_session.hpp"
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
#include <algorithm>
#include <array>
#include <cstdio>
#include <cwctype>
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
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

#include "runtime_win32_internal.h"
using namespace runtime_win32_detail;
namespace {
LRESULT CALLBACK window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    auto *app = reinterpret_cast<RuntimeWindow *>(GetWindowLongPtrW(window, GWLP_USERDATA));
    switch (message) {
    case WM_NCCREATE: {
        const auto *create = reinterpret_cast<const CREATESTRUCTW *>(lparam);
        SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create->lpCreateParams));
        return DefWindowProcW(window, message, wparam, lparam);
    }
    case kStateMessage:
        if (app) app->show_state();
        return 0;
    // Everything below that changes the game is posted to the emulation thread.
    case WM_COMMAND:
        if (app) {
            const UINT command = LOWORD(wparam);
            if (command == kPauseCommand) app->post([app] { app->toggle_pause(); });
            else if (command == kUnlimitedCreditsCommand)
                app->post([app] { app->host.set_unlimited_credits(!app->host.unlimited_credits()); app->publish_state(); });
            else if (command == kControlsCommand) app->edit_controls();
            else if (command == kStageContinueCommand) app->post([app] { app->select_stage(-1); });
            else if (command == kSweepStartCommand) app->post([app] { app->start_sweep(); });
            else if (command == kSweepStopCommand) app->post([app] { app->stop_sweep("stopped"); });
            else if (command >= kStageCommandBase && command < kStageCommandBase + kStageCount) {
                const int stage = static_cast<int>(command - kStageCommandBase);
                app->post([app, stage] { app->select_stage(stage); });
            }
        }
        return 0;
    case WM_KEYDOWN: case WM_KEYUP:
        if (app) {
            const bool pressed = message == WM_KEYDOWN;
            if (wparam == VK_ESCAPE && pressed) { DestroyWindow(window); return 0; }
            const bool repeat = (lparam & (1LL << 30)) != 0;
            if (wparam == app->bindings.keys[gain_ground::action_pause] || wparam == VK_PAUSE) {
                if (pressed && !repeat) app->post([app] { app->toggle_pause(); });
                return 0;
            }
            const auto key = static_cast<unsigned>(wparam);
            app->post([app, key, pressed, repeat] {
                // Release held controls while paused, but do not queue new presses.
                if (app->paused && pressed) return;
                if (app->keyboard.key(app->devices, key, pressed) && (!pressed || !repeat))
                    app->record_input(key, pressed);
            });
        }
        return 0;
    case WM_KILLFOCUS:
        if (app) app->post([app] { app->release_input(); });
        return 0;
    case WM_LBUTTONDOWN: case WM_LBUTTONUP: case WM_RBUTTONDOWN: case WM_RBUTTONUP:
        if(app){
            const bool pressed=message==WM_LBUTTONDOWN || message==WM_RBUTTONDOWN;
            const auto key=(message==WM_LBUTTONDOWN || message==WM_LBUTTONUP)
                ? gain_ground::RuntimeKeyboard::mouse_primary:gain_ground::RuntimeKeyboard::mouse_secondary;
            if(pressed)SetCapture(window);
            app->post([app,key,pressed]{
                if(app->paused && pressed)return;
                app->keyboard.key(app->devices,key,pressed);app->record_input(key,pressed);
            });
            if(!pressed && !(wparam&(MK_LBUTTON|MK_RBUTTON)))ReleaseCapture();
        }
        return 0;
    case WM_CAPTURECHANGED:
        if(app)app->post([app]{
            for(auto key:{gain_ground::RuntimeKeyboard::mouse_primary,gain_ground::RuntimeKeyboard::mouse_secondary}){
                app->keyboard.key(app->devices,key,false);app->record_input(key,false);
            }
        });
        return 0;
    case WM_ERASEBKGND:
        // WM_PAINT composes the complete client area, including its margins.
        return 1;
    case WM_PAINT: {
        PAINTSTRUCT paint{};
        const auto window_dc = BeginPaint(window, &paint);
        RECT client{};
        GetClientRect(window, &client);
        auto rect = client;
        const bool buffered = app && app->ensure_buffer(window_dc, rect.right, rect.bottom);
        const auto dc = buffered ? app->buffer_dc : window_dc;
        // Clear and compose off-screen so a black intermediate paint cannot
        // become visible between FillRect and StretchDIBits.
        FillRect(dc, &rect, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
        std::unique_lock<std::mutex> shared;
        if (app) shared = std::unique_lock(app->shared_lock);
        if (app && app->shared.pixels.size() >= 384U * 496U) {
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
                width, height, 0, 0, 384, 496, app->shared.pixels.data(), &info, DIB_RGB_COLORS, SRCCOPY);
        }
        SetBkMode(dc, TRANSPARENT);
        SetTextColor(dc, RGB(230, 230, 230));
        rect.left += 20; rect.right -= 20; rect.top += 20;
        if (app && !app->shared.status.empty()) {
            SetBkMode(dc, OPAQUE);
            SetBkColor(dc, RGB(0,0,0));
            DrawTextW(dc, app->shared.status.c_str(), -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);
        }
        if (shared) shared.unlock();
        if (buffered) BitBlt(window_dc, 0, 0, client.right, client.bottom, dc, 0, 0, SRCCOPY);
        EndPaint(window, &paint);
        return 0;
    }
    case WM_DESTROY:
        if (app) app->stop_emulation();
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(window, message, wparam, lparam);
    }
}
} // namespace

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show)
{
    phangrade::Session session;
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
    if (const char *value = std::getenv("GAIN_GROUND_SAMPLE"); value && *value) app->sample_path = value;
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
    // GAIN_GROUND_DEFAULT_CONTROLS: scripted runs use the default bindings and
    // neither read nor overwrite the player's saved ones.
    if (const char *value = std::getenv("GAIN_GROUND_DEFAULT_CONTROLS"); !(value && *value && *value != '0')) {
        app->controls_path = paths.user_data / "controls.ini";
        if (std::ifstream controls(app->controls_path); controls) app->bindings.read(controls);
    }
    app->apply_bindings(app->bindings);
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
    gain_ground::FunctionContext loading_context;
    if (!app->host.prepare_direct_boot(app->assets, loading_context)) {
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
    if (!app->wake) return 2;
    WNDCLASSW klass{};
    klass.lpfnWndProc = &window_proc;
    klass.hInstance = instance;
    klass.hIcon = static_cast<HICON>(LoadImageW(nullptr, (paths.user_data / "game.ico").c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE));
    if (!klass.hIcon) klass.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
    klass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    klass.lpszClassName = kWindowClass;
    if (!RegisterClassW(&klass)) return 2;
    const auto menu = CreateMenu();
    const auto settings_menu = CreatePopupMenu();
    const auto stage_menu = CreatePopupMenu();
    // Menu-bar items cannot show a check mark, so toggles live in a popup.
    bool menu_ok = menu && settings_menu && stage_menu && AppendMenuW(menu, MF_STRING, kPauseCommand, L"&Pause (P)") &&
        AppendMenuW(settings_menu, MF_STRING | MF_UNCHECKED, kUnlimitedCreditsCommand, L"&Unlimited credits") &&
        AppendMenuW(settings_menu, MF_STRING, kControlsCommand, L"&Controls...") &&
        AppendMenuW(menu, MF_POPUP, reinterpret_cast<UINT_PTR>(settings_menu), L"S&ettings");
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
        else {
            if (settings_menu) DestroyMenu(settings_menu);
            if (stage_menu) DestroyMenu(stage_menu);
        }
        return 2;
    }
    const auto window = CreateWindowExW(0, kWindowClass, L"Gain Ground",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 520, 620,
        nullptr, menu, instance, app.get());
    if (!window) { DestroyMenu(menu); return 2; }
    app->window = window;
    { std::lock_guard lock(app->shared_lock); app->shared.pixels = app->video.pixels(); }
    ShowWindow(window, show);
    session.attach(window);
    try {
        app->emulation = std::thread([runtime = app.get()] { runtime->emulation_main(); });
    } catch (const std::system_error &) {
        DestroyWindow(window);
        return 2;
    }
    MSG message{};
    BOOL received{};
    while ((received = GetMessageW(&message, nullptr, 0, 0)) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    if (received < 0) DestroyWindow(window);
    app->stop_emulation();
    return received < 0 ? 2 : static_cast<int>(message.wParam);
}
