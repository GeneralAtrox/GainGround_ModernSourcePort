#pragma once
#include "timing_trace.h"
#include <cstdlib>
#include <fstream>
#include <memory>
#include <string>

namespace gain_ground {
// Same bounded, single-CPU interval selector for native and reference producers.
// Initial-state identity must come from the approved comparison setup; this
// selector does not manufacture a state-equivalence claim from a matching PC.
class TimingTraceSession {
public:
    void configure(std::string_view producer) {
        const auto path = std::getenv("GGR_TIMING_TRACE");
        if (!path || !*path) return;
        path_ = path;
        producer_ = producer;
        cpu_ = number("GGR_TIMING_CPU", 1U);
        state_ = number("GGR_TIMING_STATE", 255U);
        start_ = number("GGR_TIMING_START_PC", 0xffffffU);
        stop_ = number("GGR_TIMING_STOP_PC", 0xffffffU); // Completed instruction PC.
        limit_ = number("GGR_TIMING_MAX_INSTRUCTIONS", 1000000U);
        if (!limit_) throw std::invalid_argument("Timing instruction limit must be nonzero");
        rom_ = hash("GGR_TIMING_ROM_SHA256");
        // Diagnostic captures may precede a matched-state setup. Leave their
        // identity absent: the strict comparator must reject them as evidence.
        const auto diagnostic = std::getenv("GGR_TIMING_DIAGNOSTIC");
        initial_ = diagnostic && std::string_view(diagnostic) == "1"
            ? std::string{} : hash("GGR_TIMING_INITIAL_STATE_SHA256");
        build_ = hash("GGR_TIMING_BUILD_SHA256");
        source_ = hash("GGR_TIMING_SOURCE_SHA256");
        interval_ = "cpu=" + std::to_string(cpu_) + ",state=" + std::to_string(state_) +
                    ",start=" + std::to_string(start_) + ",stop=" + std::to_string(stop_);
        enabled_ = true;
    }
    bool enabled() const noexcept { return enabled_; }
    const std::string &error() const noexcept { return error_; }
    ~TimingTraceSession() { close(false); }

    void observe(const TimingTraceEvent &input) noexcept {
        if (!enabled_ || finished_ || input.cpu != cpu_) return;
        try {
            if (!writer_) {
                if (input.kind != "instruction-begin" || input.pc != start_ || input.state != state_) return;
                file_.open(path_, std::ios::out | std::ios::trunc);
                if (!file_) throw std::runtime_error("Cannot open timing trace output");
                TimingTraceHeader header{producer_, rom_, initial_, interval_, build_, source_};
                header.origin = input.time;
                writer_ = std::make_unique<TimingTraceWriter>(file_, header);
                origin_clocks_ = input.clocks;
            }
            auto event = input;
            if (event.clocks < origin_clocks_) throw std::runtime_error("Timing cycle origin moved backwards");
            event.clocks -= origin_clocks_;
            if (event.kind == "instruction-begin") {
                if (open_) throw std::runtime_error("Missing instruction completion in timing producer");
                if (ordinal_ >= limit_) { close(false); return; }
                ++ordinal_; phase_ = 0; open_ = true;
            }
            event.instruction = ordinal_;
            event.phase = phase_++;
            writer_->event(event);
            if (event.kind == "unsupported") { close(false); return; }
            if (event.kind == "instruction-complete") {
                open_ = false;
                if (event.pc == stop_) close(true);
            }
        } catch (const std::exception &e) {
            error_ = e.what();
            close(false);
        }
    }
    void close(bool complete) noexcept {
        if (!writer_ || finished_) return;
        finished_ = true;
        try { writer_->finish(complete && !open_); }
        catch (const std::exception &e) { error_ = e.what(); }
        file_.close();
    }
private:
    static std::string required(const char *key) {
        const auto value = std::getenv(key);
        if (!value || !*value) throw std::invalid_argument(std::string("Missing timing setting: ") + key);
        return value;
    }
    static unsigned number(const char *key, unsigned maximum) {
        const auto text = required(key);
        std::size_t end{};
        const auto value = std::stoul(text, &end, 0);
        if (end != text.size() || value > maximum)
            throw std::invalid_argument(std::string("Invalid timing setting: ") + key);
        return static_cast<unsigned>(value);
    }
    static std::string hash(const char *key) {
        auto value = required(key);
        if (value.size() != 64 || value.find_first_not_of("0123456789abcdef") != std::string::npos)
            throw std::invalid_argument(std::string("Invalid timing identity: ") + key);
        return value;
    }
    std::string path_, producer_, rom_, initial_, interval_, build_, source_, error_;
    std::ofstream file_;
    std::unique_ptr<TimingTraceWriter> writer_;
    std::uint64_t origin_clocks_{}, ordinal_{}, phase_{};
    unsigned cpu_{}, state_{}, start_{}, stop_{}, limit_{};
    bool enabled_{}, finished_{}, open_{};
};
} // namespace gain_ground
