// Statistical sampler for the runtime's own thread (Windows). See the header.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <mmsystem.h>
#include <psapi.h>
#include <dbghelp.h>
#include "gain_ground/host_sampler.h"
#include <algorithm>
#include <atomic>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace gain_ground {
struct HostSampler::State {
    HANDLE target{};
    std::uintptr_t base{}, size{};
    std::atomic<bool> run{true};
    std::unordered_map<std::uint32_t, std::uint32_t> histogram;
    std::unordered_map<std::uintptr_t, std::uint32_t> outside; // absolute address, 64-byte granules
    std::uint64_t samples{}, failures{};
};

namespace {
DWORD WINAPI sample_loop(void *argument)
{
    auto &state = *static_cast<HostSampler::State *>(argument);
    // 1 ms scheduler granularity for Sleep(1); the histogram is offsets only.
    timeBeginPeriod(1U);
    CONTEXT context{};
    while (state.run.load(std::memory_order_relaxed)) {
        Sleep(1);
        if (SuspendThread(state.target) == static_cast<DWORD>(-1)) { ++state.failures; continue; }
        context.ContextFlags = CONTEXT_CONTROL;
        const bool ok = GetThreadContext(state.target, &context) != 0;
        ResumeThread(state.target);
        if (!ok) { ++state.failures; continue; }
        ++state.samples;
        const auto rip = static_cast<std::uintptr_t>(context.Rip);
        // Addresses outside the executable (system DLLs) collapse into one bin.
        const auto offset = rip >= state.base && rip - state.base < state.size
            ? static_cast<std::uint32_t>(rip - state.base) : 0xffffffffU;
        ++state.histogram[offset];
        if (offset == 0xffffffffU) ++state.outside[rip & ~std::uintptr_t{63}];
    }
    timeEndPeriod(1U);
    return 0;
}
} // namespace

bool HostSampler::start(std::string output_path)
{
    if (thread_) return false;
    auto *state = new State{};
    state->target = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION,
                               FALSE, GetCurrentThreadId());
    state->base = reinterpret_cast<std::uintptr_t>(GetModuleHandleW(nullptr));
    MODULEINFO image{};
    if (GetModuleInformation(GetCurrentProcess(), GetModuleHandleW(nullptr), &image, sizeof(image))) state->size = image.SizeOfImage;
    if (!state->size) state->size = 0x4000000U;
    if (!state->target) { delete state; return false; }
    output_path_ = std::move(output_path);
    thread_ = CreateThread(nullptr, 0, &sample_loop, state, 0, nullptr);
    if (!thread_) { CloseHandle(state->target); delete state; return false; }
    state_ = state;
    return true;
}

void HostSampler::stop()
{
    if (!thread_) return;
    state_->run.store(false);
    WaitForSingleObject(static_cast<HANDLE>(thread_), 5000);
    CloseHandle(static_cast<HANDLE>(thread_));
    thread_ = nullptr;
    std::vector<std::pair<std::uint32_t, std::uint32_t>> rows(state_->histogram.begin(), state_->histogram.end());
    std::sort(rows.begin(), rows.end(), [](const auto &a, const auto &b) { return a.second > b.second; });
    if (auto *file = std::fopen(output_path_.c_str(), "w")) {
        std::fprintf(file, "# samples %llu failures %llu base %llx (offsets relative to load address; 0xffffffff = outside the executable)\n",
                     static_cast<unsigned long long>(state_->samples), static_cast<unsigned long long>(state_->failures),
                     static_cast<unsigned long long>(state_->base));
        for (const auto &[offset, count] : rows) std::fprintf(file, "%08x %u\n", offset, count);
        // Attribute samples outside the executable to the module that owns them.
        HMODULE modules[256]; DWORD needed = 0;
        if (EnumProcessModules(GetCurrentProcess(), modules, sizeof(modules), &needed)) {
            std::unordered_map<std::string, std::uint32_t> per_module;
            for (const auto &[address, count] : state_->outside) {
                std::string owner = "?";
                for (DWORD i = 0; i < needed / sizeof(HMODULE) && i < 256U; ++i) {
                    MODULEINFO info{};
                    if (!GetModuleInformation(GetCurrentProcess(), modules[i], &info, sizeof(info))) continue;
                    const auto lo = reinterpret_cast<std::uintptr_t>(info.lpBaseOfDll);
                    if (address >= lo && address < lo + info.SizeOfImage) {
                        char name[MAX_PATH]{};
                        GetModuleBaseNameA(GetCurrentProcess(), modules[i], name, sizeof(name));
                        owner = name;
                        break;
                    }
                }
                per_module[owner] += count;
            }
            for (const auto &[name, count] : per_module) std::fprintf(file, "# outside %s %u\n", name.c_str(), count);
            // Name the hottest addresses in those modules from their export tables.
            std::vector<std::pair<std::uintptr_t, std::uint32_t>> away(state_->outside.begin(), state_->outside.end());
            std::sort(away.begin(), away.end(), [](const auto &a, const auto &b) { return a.second > b.second; });
            if (SymInitialize(GetCurrentProcess(), nullptr, TRUE)) {
                std::unordered_map<std::string, std::uint32_t> per_symbol;
                alignas(SYMBOL_INFO) char storage[sizeof(SYMBOL_INFO) + 256]{};
                for (std::size_t i = 0; i < away.size() && i < 400U; ++i) {
                    auto *symbol = reinterpret_cast<SYMBOL_INFO *>(storage);
                    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
                    symbol->MaxNameLen = 255;
                    DWORD64 displacement = 0;
                    std::string name = "?";
                    if (SymFromAddr(GetCurrentProcess(), away[i].first, &displacement, symbol)) {
                        char text[320];
                        std::snprintf(text, sizeof(text), "%s+0x%llx", symbol->Name, static_cast<unsigned long long>(displacement & ~0x3fULL));
                        name = text;
                    }
                    per_symbol[name] += away[i].second;
                }
                std::vector<std::pair<std::string, std::uint32_t>> named(per_symbol.begin(), per_symbol.end());
                std::sort(named.begin(), named.end(), [](const auto &a, const auto &b) { return a.second > b.second; });
                for (std::size_t i = 0; i < named.size() && i < 16U; ++i)
                    std::fprintf(file, "# symbol %s %u\n", named[i].first.c_str(), named[i].second);
                SymCleanup(GetCurrentProcess());
            }
        } else {
            std::fprintf(file, "# outside attribution unavailable (error %lu)\n", GetLastError());
        }
        std::fclose(file);
    }
    CloseHandle(state_->target);
    delete state_;
    state_ = nullptr;
}

HostSampler::~HostSampler() { stop(); }
} // namespace gain_ground
