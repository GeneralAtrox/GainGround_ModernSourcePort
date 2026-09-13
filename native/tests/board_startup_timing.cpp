#define NOMINMAX
#include <windows.h>
#include <bcrypt.h>
#include "gain_ground/direct_asset_loader.h"
#include "gain_ground/direct_boot_audio.h"
#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_devices.h"
#include "gground_functions.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <limits>
#include <stdexcept>

using namespace gain_ground;
namespace {
void require(bool ok, const char *why) { if (!ok) throw std::runtime_error(why); }
bool matches_hash(std::span<const std::uint8_t> bytes, std::string_view expected) {
    if (expected.size() != 64 || bytes.size() > std::numeric_limits<ULONG>::max()) return false;
    BCRYPT_ALG_HANDLE algorithm{};
    if (BCryptOpenAlgorithmProvider(&algorithm, BCRYPT_SHA256_ALGORITHM, nullptr, 0) < 0) return false;
    std::array<UCHAR, 32> digest{};
    const auto result = BCryptHash(algorithm, nullptr, 0, const_cast<PUCHAR>(bytes.data()),
        static_cast<ULONG>(bytes.size()), digest.data(), static_cast<ULONG>(digest.size()));
    BCryptCloseAlgorithmProvider(algorithm, 0);
    if (result < 0) return false;
    constexpr char hex[] = "0123456789abcdef";
    for (unsigned i = 0; i < digest.size(); ++i)
        if (hex[digest[i] >> 4] != expected[2 * i] || hex[digest[i] & 15] != expected[2 * i + 1]) return false;
    return true;
}
struct Probe {
    RuntimeHost host;
    System24Devices devices;
    unsigned budget{}, untimed{}, yields{};
    std::uint64_t slice{};
    static void checkpoint(void *argument) {
        auto &p = *static_cast<Probe *>(argument);
        const auto deadline = p.host.next_cpu_deadline_ns();
        if (deadline != UINT64_MAX) {
            // The window scheduler starts a fresh cooperative budget whenever
            // it resumes the CPU after a timed wait.
            p.untimed = 0;
            p.devices.advance(std::min(deadline, p.devices.time_ns() + p.slice));
        } else if (!p.host.timed_execution() && ++p.untimed >= p.budget) {
            // Reproduce the runtime's old cooperative-yield clock advancement.
            p.untimed = 0; ++p.yields;
            p.devices.advance(p.devices.next_event_ns());
        }
    }
};
}
int main(int argc, char **argv) { try {
    require(argc == 3, "Expected BIOS and extracted-assets paths");
    std::ifstream input(argv[1], std::ios::binary);
    const std::vector<std::uint8_t> bios{std::istreambuf_iterator<char>(input), {}};
    require(matches_hash(bios, "a42dc284615f58ec035652f178e1bae9b1443e7468e436c578e328dd75dc93ed"), "BIOS identity mismatch");
    DirectAssetLoader assets;
    require(assets.open(argv[2], matches_hash), "Asset identity mismatch");
    std::uint64_t first_time{};
    CpuRegisters first_registers{};
    std::vector<std::uint8_t> first_memory;
    unsigned cases{};
    for (auto budget : {1024U, 4096U, 8192U}) for (auto slice : {173ULL, 41000ULL}) {
        auto p = std::make_unique<Probe>(); p->budget = budget; p->slice = slice;
        require(p->host.load_region(1, 0, bios), "BIOS load failed");
        FunctionContext c{};
        require(p->host.prepare_direct_boot(assets, c), "Direct boot preparation failed");
        p->host.attach_devices(p->devices);
        prepare_direct_boot_devices(p->devices);
        p->host.set_checkpoint(Probe::checkpoint, p.get());
        auto boot = c;
        const auto audio = run_direct_boot_audio(boot);
        require(audio.status == TranslationStatus::complete && audio.control == 1, "BIOS audio startup failed");
        // Execute the real startup and its real children, stopping at the
        // sequential handoff to owner 55 rather than entering the game loop.
        const auto result = translated::cpu_a_runtime_init(c);
        require(!p->host.faulted(), "Startup device/memory fault");
        require(result.status == TranslationStatus::complete && result.control == 3 &&
            result.exit_program_counter == 0x80104 && c.registers.program_counter == 0x80104,
            "Startup did not reach the main-loop handoff");
        require(!p->host.timed_execution() && p->yields == 0, "Untimed startup consumed cooperative scheduler time");
        require(p->devices.cpu_b_enabled() && c.registers.address[7] == 0 &&
            c.registers.status == 0x2000 && p->devices.read(0xa00000, 0xffff) == 0,
            "Wrong CPU/timer startup state");
        std::vector<std::uint8_t> memory;
        for (unsigned region : {2U, 3U, 5U, 6U, 7U, 8U, 9U, 10U, 11U}) {
            const auto bytes = p->host.region_bytes(region);
            memory.insert(memory.end(), bytes.begin(), bytes.end());
        }
        if (cases == 0) { first_time = p->devices.time_ns(); first_registers = c.registers; first_memory = memory; }
        else require(first_time == p->devices.time_ns() && first_registers.data == c.registers.data &&
            first_registers.address == c.registers.address && first_memory == memory,
            "Yield budget or scheduler slice changed startup state/time");
        ++cases;
    }
    std::cout << "PASS " << cases << " startup schedules: identical RAM/registers, zero untimed yields; handoff ns=" << first_time << '\n';
    return 0;
} catch (const std::exception &e) { std::cerr << e.what() << '\n'; return 1; } }
