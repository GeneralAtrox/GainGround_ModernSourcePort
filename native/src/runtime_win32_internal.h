#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <shellapi.h>
#include <bcrypt.h>
#include <mmsystem.h>
#include <xinput.h>
#include <cstring>
#include "gain_ground/runtime_host.h"
#include "gain_ground/native_game_loop.h"
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

namespace runtime_win32_detail {
constexpr wchar_t kWindowClass[] = L"GainGroundNativeRuntime";
constexpr UINT kPauseCommand = 1001U;
constexpr UINT kUnlimitedCreditsCommand = 1002U;
constexpr UINT kControlsCommand = 1003U;
// Posted by the emulation thread when the pause, credit or stage state changed.
constexpr UINT kStateMessage = WM_APP + 1U;
// Emulation slice period. Each slice runs to the wall clock and submits its audio.
constexpr LONG kSliceMs = 4;
// Audio queue bounds, in 62500 Hz frames: the cushion restored whenever the
// device runs dry, and the queue beyond which new samples are dropped.
constexpr std::size_t kAudioPrerollFrames = 62500U * 40U / 1000U;
constexpr std::size_t kAudioMaxQueueFrames = 62500U * 150U / 1000U;
// At real speed, a host that falls further behind than this lets emulated
// time slip instead of fast-forwarding, which would queue the whole backlog
// as late audio.
constexpr std::uint64_t kMaxLagNs = 60'000'000U;
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

namespace controls {
using GetState = DWORD(WINAPI *)(DWORD, XINPUT_STATE *);
bool edit(HWND owner, GetState get, gain_ground::RuntimeBindings &bindings);
}

struct AudioOutput {
    struct Block { std::vector<std::int16_t> samples; WAVEHDR header{}; };
    HWAVEOUT output{};
    std::deque<std::unique_ptr<Block>> blocks;
    bool open() {
        WAVEFORMATEX format{WAVE_FORMAT_PCM, 2, 62500, 250000, 4, 16, 0};
        return waveOutOpen(&output, WAVE_MAPPER, &format, 0, 0, CALLBACK_NULL) == MMSYSERR_NOERROR;
    }
    // Diagnostics for the progress line: frames written, submissions that found
    // the device had played everything (an audible gap), and the queue range.
    std::uint64_t submitted{};
    unsigned starved{};
    unsigned dropped{};
    double min_queued_ms{-1.0};
    double max_queued_ms{-1.0};
    bool submit(std::vector<std::int16_t> samples) {
        while (!blocks.empty() && (blocks.front()->header.dwFlags & WHDR_DONE)) {
            waveOutUnprepareHeader(output, &blocks.front()->header, sizeof(WAVEHDR));
            blocks.pop_front();
        }
        if (samples.empty()) return true;
        if (blocks.empty() && submitted) ++starved;
        std::size_t pending = 0U;
        for (const auto &queued : blocks) pending += queued->samples.size() / 2U;
        const double queued = double(pending) * 1000.0 / 62500.0;
        if (submitted && (min_queued_ms < 0.0 || queued < min_queued_ms)) min_queued_ms = queued;
        if (queued > max_queued_ms) max_queued_ms = queued;
        if (pending == 0U) samples.insert(samples.begin(), kAudioPrerollFrames * 2U, std::int16_t{});
        else if (pending > kAudioMaxQueueFrames) { ++dropped; return true; }
        submitted += samples.size() / 2U;
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
    gain_ground::NativeGameLoop game{host, devices};
    // One resumable native call stack, solely to yield to presentation/input.
    // There are no independently scheduled CPU workers or per-CPU deadlines.
    void *scheduler_fiber{}, *game_fiber{};
    std::uint64_t yield_at_ns{};
    bool title_started{};
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
    gain_ground::RuntimeBindings bindings;
    std::filesystem::path controls_path; // Saved bindings; empty disables saving.
    struct InputObservation { std::uint64_t ns, frame; unsigned key; bool pressed; };
    std::vector<InputObservation> input_history;
    bool failure_saved{};
    std::chrono::steady_clock::time_point paused_at{};

    // Threads. The game thread owns the native loop, services, audio
    // and rendering, so painting, menus and dialogs on the window thread cannot
    // stall the sound. The window thread reaches that state only through post()
    // (commands run between emulation slices) and the published Shared copy.
    HWND window{};
    std::thread emulation;
    std::atomic<bool> stopping{};
    HANDLE wake{CreateEventW(nullptr, FALSE, FALSE, nullptr)};
    std::mutex commands_lock;
    std::vector<std::function<void()>> commands;
    std::string sample_path; // GAIN_GROUND_SAMPLE profiles the emulation thread.
    struct Shared {
        std::vector<std::uint32_t> pixels;
        std::wstring status; // Non-empty once the runtime stopped.
        bool paused{}, unlimited_credits{};
        int selected_stage{-1};
    };
    std::mutex shared_lock;
    Shared shared;
    std::wstring published_status; // emulation thread
    int checked_stage{-1};          // window thread: stage menu item shown checked

void post(std::function<void()> command);

    // Window thread: runs `command` on the emulation thread and waits for its result.
    template<class F> auto call(F command) -> decltype(command())
    {
        std::mutex done_lock;
        std::condition_variable done;
        bool complete = false;
        decltype(command()) value{};
        post([&] {
            auto result = command();
            std::lock_guard lock(done_lock);
            value = result;
            complete = true;
            done.notify_one();
        });
        std::unique_lock lock(done_lock);
        done.wait(lock, [&] { return complete; });
        return value;
    }
void run_commands();

void stop_emulation();


    // Emulation thread: slices of emulated time paced by a high-resolution
    // timer, with the window thread's commands applied between slices.
    // Diagnostics: the longest wake-to-wake gap, the furthest emulation fell
    // behind wall-clock time, and how often it let emulated time slip.
    double max_gap_ms{};
    double max_lag_ms{};
    // Share of wall-clock time this thread spent running since the last call.
    std::uint64_t busy_cpu_100ns{};
    std::chrono::steady_clock::time_point busy_wall{std::chrono::steady_clock::now()};
double busy_percent();

    unsigned slipped{};
void emulation_main();

    // Emulation thread: hand a new frame or stop report to the window.
void publish_frame();

void publish_state();


void record_input(unsigned key, bool pressed);


void apply_bindings(const gain_ground::RuntimeBindings &chosen);

    // Window thread; `bindings` is its copy. The game pauses while the
    // Controls dialog is open, resuming only if it was running.
void edit_controls();

void release_input();

void poll_controllers();


void save_failure();


void toggle_pause();


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

template<class... Args>
    void diag_log(const char *format, Args... args)
    {
        if (!diag) return;
        std::fprintf(diag, format, args...);
        std::fputc('\n', diag);
        std::fflush(diag);
    }


std::uint16_t stage_index() const;


void start_sweep();


void stop_sweep(const char *why);


    // F355's terrain probes at (x,y), (x,y+18), (x+20,y+18), (x+20,y) against
    // the column-major attribute map, plus the 20x18 footprint on screen.
bool cell_clear(int x, int y) const;


    // Once per rendered frame while a sweep runs: every few frames move
    // player 1 to the next clear cell, so its update, contacts and the enemies'
    // reactions run from every position the stage offers.
    // The stage phase machine clears the stage on time-up when the byte at
    // 0xd2d reaches 2 (raised by the clock object as its countdown expires).
    // Ten-times speed would expire the clock mid-sweep, so hold that byte.
void hold_stage_clock();


void sweep_step();


    // Window thread: title and menu marks from the published state.
void show_state();


    // A choice ends the current stage through the original stage-clear
    // sequence and loads the chosen stage, once. Made at the title, it applies
    // on the first frame of the next game; the attract demo never consumes it.
void select_stage(int stage);


    // Off-screen paint buffer, kept across paints and recreated on resize.
    HDC buffer_dc{}; HBITMAP buffer{}; LONG buffer_width{}, buffer_height{};
bool ensure_buffer(HDC window_dc, LONG width, LONG height);

    ~RuntimeWindow() {
        if (buffer) DeleteObject(buffer);
        if (buffer_dc) DeleteDC(buffer_dc);
        if (wake) CloseHandle(wake);
    }

static void checkpoint(void *argument);
static void WINAPI execute(void *argument);




void advance();


std::wstring status() const;

};
}
