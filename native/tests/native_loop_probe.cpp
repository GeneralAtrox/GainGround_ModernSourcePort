#include "gain_ground/native_game_loop.h"
#include "gain_ground/direct_asset_loader.h"
#include "gain_ground/rom_import.h"
#include "gain_ground/runtime_input.h"
#include "gain_ground/system24_video.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <thread>
#ifdef _WIN32
#include "../src/runtime_win32_internal.h"
#endif

using namespace gain_ground;
#ifdef _WIN32
struct PlaybackProbe {
    RuntimeHost &host;
    System24Devices &devices;
    runtime_win32_detail::AudioOutput output;
    std::chrono::steady_clock::time_point epoch{std::chrono::steady_clock::now()};
    std::uint64_t next_ns{4'000'000}, samples{}, nonzero{}, clipped{};
    int peak{};
    bool good{true};
    bool precision{};
    ~PlaybackProbe() { if (precision) timeEndPeriod(1); }
    static void checkpoint(void *argument) {
        auto &p = *static_cast<PlaybackProbe *>(argument);
        const auto now = p.devices.time_ns();
        if (!p.good || now < p.next_ns) return;
        auto pcm = p.devices.audio.take_samples();
        p.samples += pcm.size();
        for (const auto value : pcm) {
            p.nonzero += value != 0; p.clipped += value == 32767 || value == -32768;
            p.peak = std::max(p.peak, std::abs(int(value)));
        }
        p.good = p.output.submit(std::move(pcm));
        p.next_ns = now + 4'000'000;
        std::this_thread::sleep_until(p.epoch + std::chrono::nanoseconds(now));
    }
};
#endif
int main(int argc, char **argv) {
    if (argc < 3) return 2;
    RuntimeHost host;
    System24Devices devices;
    DirectAssetLoader assets;
    std::ifstream input(argv[1], std::ios::binary);
    const std::vector<std::uint8_t> bios{std::istreambuf_iterator<char>(input), {}};
    if (bios.size() != 0x40000 || !host.load_region(1, 0, bios) ||
        !assets.open(argv[2], &matches_hash)) { std::cerr << assets.error(); return 2; }
    const bool play = argc > 4 && std::string_view(argv[4]) == "--play";
#ifdef _WIN32
    PlaybackProbe playback{host, devices};
    if (play) {
        if (!playback.output.open()) { std::cerr << "WaveOut open failed\n"; return 1; }
        // Match the runtime's timing/priority and start the wall-clock epoch
        // after the potentially slow device open, as the real window does.
        timeBeginPeriod(1); playback.precision = true;
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
        playback.epoch = std::chrono::steady_clock::now();
        host.set_checkpoint(&PlaybackProbe::checkpoint, &playback);
    }
#endif
    if (play && argc > 5) {
        std::ifstream music(argv[5], std::ios::binary);
        const std::vector<std::uint8_t> pcm{std::istreambuf_iterator<char>(music), {}};
        if (!devices.audio.set_title_music(pcm)) return 2;
    }
    NativeGameLoop loop(host, devices);
    const bool started = loop.start(assets);
    std::cout << "started=" << started << " ns=" << devices.time_ns() << " pc=" << std::hex
        << loop.context().registers.program_counter << std::dec << " fault=" << host.fault().message << std::endl;
    std::uint64_t samples = 0, nonzero = 0, clipped = 0;
    int peak = 0;
    unsigned completed = 0;
    const unsigned count = argc > 3 ? std::strtoul(argv[3], nullptr, 10) : 300;
    RuntimeKeyboard keyboard;
    if (argc > 4 && std::string_view(argv[4]) == "--stages") {
        host.set_unlimited_credits(true);
        host.set_player_invulnerable(true);
        auto step = [&](unsigned frames) {
            for (unsigned i = 0; i < frames; ++i) {
                if (!loop.frame()) return false;
                (void)devices.audio.take_samples();
            }
            return true;
        };
        auto tap = [&](unsigned key) {
            keyboard.key(devices, key, true); const bool ok = step(5);
            keyboard.key(devices, key, false); return step(5) && ok;
        };
        auto value = [&](unsigned offset) { const auto m = host.region_bytes(2); return (m[offset] << 8) | m[offset + 1]; };
        bool ok = started && step(60) && tap('F') && tap('Q') && step(90) && tap('Q') && step(30);
        if (ok && argc > 5) {
            const std::filesystem::path directory(argv[5]);
            std::filesystem::create_directories(directory);
            for (unsigned region : {2U, 3U}) {
                const auto bytes = host.region_bytes(region);
                std::ofstream memory(directory / (region == 2 ? "cpu-b-ram.bin" : "cpu-a-shared-ram.bin"), std::ios::binary);
                memory.write(reinterpret_cast<const char *>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
                if (!memory) return 2;
            }
            System24Video video;
            video.render(host);
            std::ofstream picture(directory / "stage.ppm", std::ios::binary);
            picture << "P6\n384 496\n255\n";
            for (const auto pixel : video.pixels()) {
                const char rgb[]{static_cast<char>(pixel >> 16), static_cast<char>(pixel >> 8), static_cast<char>(pixel)};
                picture.write(rgb, 3);
            }
            if (!picture) return 2;
            if (argc > 6 && std::string_view(argv[6]) == "--snapshot-only") return 0;
        }
        unsigned passed = 0;
        for (unsigned stage = 0; ok && stage < 40; ++stage) {
            host.set_start_stage(static_cast<int>(stage));
            bool applied = false;
            for (unsigned i = 0; ok && !applied && i < 600; ++i) {
                ok = step(1); applied = host.take_start_stage_applied();
            }
            ok = ok && applied && step(100) && tap('Q') && step(10) && tap('Q') && step(40);
            for (unsigned i = 0; ok && i < 20; ++i) ok = tap(i & 1 ? 'Q' : 'E');
            const auto player = host.player_record(0);
            const bool playing = value(0xc16) == 0 && player && (host.region_bytes(2)[player] & 0x80);
            ok = ok && playing && value(0xc02) == int(stage);
            passed += ok;
            std::cout << "stage=" << stage << " pass=" << ok << " applied=" << applied
                << " index=" << value(0xc02) << " phase=" << value(0xc16)
                << " player=" << player << " fault=" << host.fault().message << " pc=" << std::hex
                << host.fault().pc << std::dec << std::endl;
        }
        std::cout << "stages passed=" << passed << "/40 updates=" << loop.updates()
            << " sound=" << loop.sound_services() << std::endl;
        return ok && passed == 40 ? 0 : 1;
    }
    if (!play) (void)devices.audio.take_samples();
    for (unsigned i = 0; started && !host.faulted() && i < count; ++i) {
        if (i == 100) keyboard.key(devices, 'F', true);
        if (i == 104) keyboard.key(devices, 'F', false);
        if (play) {
            for (const auto start : {180U, 270U, 550U}) {
                if (i == start) keyboard.key(devices, 'Q', true);
                if (i == start + 4) keyboard.key(devices, 'Q', false);
            }
            if (i == 350) keyboard.key(devices, 'W', true);
            if (i == 370) keyboard.key(devices, 'W', false);
            if (i == 450) keyboard.key(devices, 'E', true);
            if (i == 454) keyboard.key(devices, 'E', false);
        }
        if (!loop.frame()) { std::cerr << "frame failed " << i << std::endl; break; }
        ++completed;
        auto pcm = play ? std::vector<std::int16_t>{} : devices.audio.take_samples();
        samples += pcm.size();
        for (const auto value : pcm) {
            nonzero += value != 0; clipped += value == 32767 || value == -32768;
            peak = std::max(peak, std::abs(int(value)));
        }
        if (i % 100 == 0) std::cout << "update=" << i << " frame=" << devices.frame() << std::endl;
    }
#ifdef _WIN32
    if (play) {
        auto &output = playback.output;
        samples = playback.samples; nonzero = playback.nonzero; clipped = playback.clipped; peak = playback.peak;
        std::cout << "WaveOut frames=" << output.submitted << " starved=" << output.starved
            << " dropped=" << output.dropped << " queue_ms=" << output.min_queued_ms << ',' << output.max_queued_ms << '\n';
        if (!playback.good || output.starved || output.dropped) return 1;
    }
#endif
    std::cout << "updates=" << loop.updates() << " sound=" << loop.sound_services()
        << " ns=" << devices.time_ns() << " samples=" << samples << " nonzero=" << nonzero
        << " clipped=" << clipped << " peak=" << peak << " depth=" << host.max_call_depth(1)
        << " fault=" << host.fault().message << " pc=" << std::hex << host.fault().pc
        << " function=" << std::dec << host.fault().function_id << std::endl;
    return started && !host.faulted() && completed == count && nonzero && !clipped ? 0 : 1;
}
