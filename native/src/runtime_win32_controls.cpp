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

namespace runtime_win32_detail::controls {

using gain_ground::action_count;
constexpr wchar_t kClass[] = L"GainGroundControls";
constexpr int kKeyBase = 100, kPadBase = 200, kDefaults = 300;
constexpr const wchar_t *kLabels[action_count]{
    L"Move up", L"Move down", L"Move left", L"Move right",
    L"Attack / join", L"Big attack", L"Insert credit", L"Pause"};

std::wstring key_name(unsigned key)
{
    if (key == 0) return L"(none)";
    bool extended = false;
    switch (key) {
    case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN: case VK_PRIOR: case VK_NEXT:
    case VK_END: case VK_HOME: case VK_INSERT: case VK_DELETE: case VK_DIVIDE: case VK_NUMLOCK:
        extended = true;
        break;
    default: break;
    }
    const auto scan = MapVirtualKeyW(key, MAPVK_VK_TO_VSC);
    wchar_t name[64]{};
    if (scan && GetKeyNameTextW(static_cast<LONG>((scan << 16) | (extended ? 1U << 24 : 0U)), name, int(std::size(name))) > 0) {
        // Keyboard layouts report some names in capitals ("SPACE"); show them as words.
        std::wstring text = name;
        if (std::none_of(text.begin(), text.end(), [](wchar_t c) { return std::iswlower(c) != 0; }))
            for (std::size_t i = 1; i < text.size(); ++i)
                if (std::iswalpha(text[i - 1])) text[i] = static_cast<wchar_t>(std::towlower(text[i]));
        return text;
    }
    std::swprintf(name, std::size(name), L"Key 0x%02X", key);
    return name;
}

std::wstring button_name(unsigned mask)
{
    constexpr const wchar_t *names[16]{
        L"D-pad up", L"D-pad down", L"D-pad left", L"D-pad right", L"Menu (Start)", L"View (Back)",
        L"Left stick press", L"Right stick press", L"LB", L"RB", nullptr, nullptr, L"A", L"B", L"X", L"Y"};
    for (unsigned bit = 0; bit < 16U; ++bit)
        if (mask == 1U << bit && names[bit]) return names[bit];
    return L"(none)";
}

struct Dialog {
    gain_ground::RuntimeBindings bindings;
    GetState get{};
    std::array<HWND, action_count> key_buttons{}, pad_buttons{};
    // Device being captured: 0 none, 1 keyboard, 2 controller.
    unsigned capture{}, capture_action{};
    std::array<WORD, 3> previous{};
    bool done{}, accepted{};

    void refresh()
    {
        for (unsigned a = 0; a < action_count; ++a) {
            const bool keys = capture == 1U && capture_action == a, pads = capture == 2U && capture_action == a;
            SetWindowTextW(key_buttons[a], keys ? L"Press a key..." : key_name(bindings.keys[a]).c_str());
            SetWindowTextW(pad_buttons[a], pads ? L"Press a button..." : button_name(bindings.pad[a]).c_str());
        }
    }
    void end_capture()
    {
        const auto button = capture == 1U ? key_buttons[capture_action] : pad_buttons[capture_action];
        capture = 0U;
        refresh();
        SetFocus(button);
    }
    void poll_pads()
    {
        for (DWORD p = 0; p < 3U; ++p) {
            XINPUT_STATE state{};
            const WORD buttons = get && get(p, &state) == ERROR_SUCCESS ? state.Gamepad.wButtons : 0;
            const WORD pressed = buttons & ~previous[p] & 0xf3ffU;
            previous[p] = buttons;
            if (capture == 2U && pressed) {
                gain_ground::RuntimeBindings::assign(bindings.pad, capture_action, pressed & (~pressed + 1U));
                end_capture();
            }
        }
    }
};

LRESULT CALLBACK proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    auto *dialog = reinterpret_cast<Dialog *>(GetWindowLongPtrW(window, GWLP_USERDATA));
    switch (message) {
    case WM_NCCREATE:
        SetWindowLongPtrW(window, GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(reinterpret_cast<const CREATESTRUCTW *>(lparam)->lpCreateParams));
        return DefWindowProcW(window, message, wparam, lparam);
    case WM_COMMAND: {
        if (!dialog) return 0;
        const int id = LOWORD(wparam);
        if (id == IDOK || id == IDCANCEL) {
            // Escape reaches here only outside a capture; inside, it cancels the capture.
            dialog->accepted = id == IDOK;
            dialog->done = true;
        } else if (id == kDefaults) {
            dialog->capture = 0U;
            dialog->bindings = {};
            dialog->refresh();
        } else if ((id >= kKeyBase && id < kKeyBase + int(action_count)) || (id >= kPadBase && id < kPadBase + int(action_count))) {
            const unsigned device = id >= kPadBase ? 2U : 1U;
            const unsigned action = unsigned(id - (device == 2U ? kPadBase : kKeyBase));
            if (dialog->capture == device && dialog->capture_action == action) { dialog->end_capture(); return 0; }
            dialog->capture = device;
            dialog->capture_action = action;
            // Buttons already held when the capture starts are not a choice.
            for (DWORD p = 0; p < 3U; ++p) {
                XINPUT_STATE state{};
                dialog->previous[p] = dialog->get && dialog->get(p, &state) == ERROR_SUCCESS ? state.Gamepad.wButtons : 0;
            }
            dialog->refresh();
            // Keys go to the dialog itself while capturing, not the focused button.
            SetFocus(window);
        }
        return 0;
    }
    case WM_KEYDOWN:
        if (dialog && dialog->capture != 0U) {
            const auto key = static_cast<unsigned>(wparam);
            if (key == VK_ESCAPE) dialog->end_capture();
            else if (dialog->capture == 1U) {
                if (gain_ground::RuntimeBindings::reserved_key(key)) { MessageBeep(MB_ICONWARNING); return 0; }
                gain_ground::RuntimeBindings::assign(dialog->bindings.keys, dialog->capture_action, key);
                dialog->end_capture();
            }
            return 0;
        }
        break;
    case WM_TIMER:
        if (dialog) dialog->poll_pads();
        return 0;
    case WM_CLOSE:
        if (dialog) dialog->done = true;
        return 0;
    case WM_DESTROY:
        KillTimer(window, 1U);
        if (dialog) dialog->done = true;
        return 0;
    default: break;
    }
    return DefWindowProcW(window, message, wparam, lparam);
}

// Modal: returns true with the accepted bindings in `bindings`.
bool edit(HWND owner, GetState get, gain_ground::RuntimeBindings &bindings)
{
    const auto instance = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(owner, GWLP_HINSTANCE));
    WNDCLASSW klass{};
    if (!GetClassInfoW(instance, kClass, &klass)) {
        klass = {};
        klass.lpfnWndProc = &proc;
        klass.hInstance = instance;
        klass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        klass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
        klass.lpszClassName = kClass;
        if (!RegisterClassW(&klass)) return false;
    }
    Dialog dialog;
    dialog.bindings = bindings;
    dialog.get = get;
    constexpr DWORD style = WS_POPUP | WS_CAPTION | WS_SYSMENU;
    constexpr DWORD ex_style = WS_EX_DLGMODALFRAME | WS_EX_CONTROLPARENT;
    RECT frame{0, 0, 460, 364};
    AdjustWindowRectEx(&frame, style, FALSE, ex_style);
    RECT owner_rect{};
    GetWindowRect(owner, &owner_rect);
    const int width = frame.right - frame.left, height = frame.bottom - frame.top;
    const auto window = CreateWindowExW(ex_style, kClass, L"Controls", style,
        owner_rect.left + (owner_rect.right - owner_rect.left - width) / 2,
        owner_rect.top + (owner_rect.bottom - owner_rect.top - height) / 2,
        width, height, owner, nullptr, instance, &dialog);
    if (!window) return false;

    NONCLIENTMETRICSW metrics{};
    metrics.cbSize = sizeof(metrics);
    const auto font = SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(metrics), &metrics, 0)
        ? CreateFontIndirectW(&metrics.lfMessageFont) : nullptr;
    const auto child = [&](const wchar_t *type, const wchar_t *text, DWORD extra, int x, int y, int w, int h, int id) {
        const auto control = CreateWindowExW(0, type, text, WS_CHILD | WS_VISIBLE | extra, x, y, w, h,
            window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)), instance, nullptr);
        if (control && font) SendMessageW(control, WM_SETFONT, reinterpret_cast<WPARAM>(font), FALSE);
        return control;
    };
    child(L"STATIC", L"Keyboard (player 1)", SS_LEFT, 150, 12, 140, 18, -1);
    child(L"STATIC", L"Controller (all players)", SS_LEFT, 302, 12, 146, 18, -1);
    for (unsigned a = 0; a < action_count; ++a) {
        const int y = 36 + int(a) * 28;
        child(L"STATIC", kLabels[a], SS_LEFT | SS_CENTERIMAGE, 12, y, 130, 24, -1);
        dialog.key_buttons[a] = child(L"BUTTON", L"", WS_TABSTOP | BS_PUSHBUTTON, 150, y, 140, 24, kKeyBase + int(a));
        dialog.pad_buttons[a] = child(L"BUTTON", L"", WS_TABSTOP | BS_PUSHBUTTON, 302, y, 146, 24, kPadBase + int(a));
    }
    child(L"STATIC",
        L"Click a binding, then press the new key or controller button (Esc cancels). "
        L"A key or button already in use swaps with it. The left stick always moves; "
        L"Enter and the mouse buttons also attack. Esc exits the game.",
        SS_LEFT, 12, 264, 436, 54, -1);
    child(L"BUTTON", L"Restore defaults", WS_TABSTOP | BS_PUSHBUTTON, 12, 328, 120, 26, kDefaults);
    child(L"BUTTON", L"OK", WS_TABSTOP | BS_DEFPUSHBUTTON, 292, 328, 75, 26, IDOK);
    child(L"BUTTON", L"Cancel", WS_TABSTOP | BS_PUSHBUTTON, 373, 328, 75, 26, IDCANCEL);
    dialog.refresh();
    SetTimer(window, 1U, 16U, nullptr);

    EnableWindow(owner, FALSE);
    ShowWindow(window, SW_SHOW);
    SetFocus(dialog.key_buttons[0]);
    MSG message{};
    while (!dialog.done) {
        const auto got = GetMessageW(&message, nullptr, 0, 0);
        if (got == 0) { PostQuitMessage(static_cast<int>(message.wParam)); break; }
        if (got < 0) break;
        // Dialog keyboard navigation is off while a key is being captured.
        if (dialog.capture == 0U && IsDialogMessageW(window, &message)) continue;
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    // Re-enable the owner first so activation returns to it.
    EnableWindow(owner, TRUE);
    if (IsWindow(window)) DestroyWindow(window);
    if (font) DeleteObject(font);
    SetActiveWindow(owner);
    if (dialog.accepted) bindings = dialog.bindings;
    return dialog.accepted;
}

}
