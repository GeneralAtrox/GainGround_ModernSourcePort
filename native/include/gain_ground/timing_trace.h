#pragma once

#include <array>
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string_view>

namespace gain_ground {
struct TimingCpuPosition {
    std::uint64_t clocks{}, instruction{};
    bool instruction_open{};
};
// Shared wire format for bounded reference/native timing intervals. A caller
// supplies the exact emulated timestamp, never wall-clock/debugger elapsed time.
// Used by both the native schedule and the reference CPU hooks.
struct TimingTimestamp {
    std::uint64_t seconds{};
    std::uint64_t attoseconds{};
    static TimingTimestamp from_ns(std::uint64_t value) noexcept {
        return {value / 1000000000ULL, (value % 1000000000ULL) * 1000000000ULL};
    }
};

struct TimingTraceHeader {
    std::string_view producer; // "native" or "reference"
    std::string_view rom_sha256;
    std::string_view initial_state_sha256; // RAM/devices/IRQs/pipeline; empty for diagnostics, never parity evidence.
    std::string_view interval;
    std::string_view build_sha256;
    std::string_view source_sha256;
    std::array<std::uint64_t, 2> clock_hz{10000000U, 10000000U};
    TimingTimestamp origin{};
};

struct TimingTraceEvent {
    std::string_view kind;
    std::uint8_t cpu{}, state{};
    std::uint32_t pc{};
    std::uint16_t opcode{};
    std::uint64_t instruction{}, phase{}, clocks{};
    TimingTimestamp time{};
    std::string_view location; // Diagnostic only; not used to align events.
    std::string_view space;
    std::uint32_t address{}, next_pc{};
    std::uint16_t mask{}, value{};
    std::uint8_t width{};
    std::string_view detail;
};

class TimingTraceWriter {
public:
    TimingTraceWriter(std::ostream &stream, const TimingTraceHeader &header) : out_(stream) {
        out_ << "{\"kind\":\"header\",\"schema\":\"gground-timing-trace-v1\",\"producer\":";
        string(header.producer);
        field("romSha256", header.rom_sha256);
        field("initialStateSha256", header.initial_state_sha256);
        field("interval", header.interval);
        field("buildSha256", header.build_sha256);
        field("sourceSha256", header.source_sha256);
        out_ << ",\"clockHz\":[" << header.clock_hz[0] << ',' << header.clock_hz[1] << ']';
        out_ << ",\"origin\":"; timestamp(header.origin);
        out_ << "}\n";
        check();
    }
    TimingTraceWriter(const TimingTraceWriter &) = delete;
    TimingTraceWriter &operator=(const TimingTraceWriter &) = delete;

    void event(const TimingTraceEvent &e) {
        if (finished_) throw std::logic_error("Timing trace already finished");
        out_ << "{\"kind\":"; string(e.kind);
        out_ << ",\"sequence\":" << count_ << ",\"cpu\":" << unsigned(e.cpu)
             << ",\"state\":" << unsigned(e.state) << ",\"pc\":" << e.pc
             << ",\"opcode\":" << e.opcode << ",\"instruction\":" << e.instruction
             << ",\"phase\":" << e.phase << ",\"clocks\":" << e.clocks << ",\"time\":";
        timestamp(e.time);
        field("location", e.location);
        if (e.kind == "bus-read" || e.kind == "bus-write") {
            field("space", e.space);
            out_ << ",\"address\":" << e.address << ",\"width\":" << unsigned(e.width)
                 << ",\"mask\":" << e.mask << ",\"value\":" << e.value;
        }
        if (e.kind == "instruction-complete") out_ << ",\"nextPc\":" << e.next_pc;
        if (!e.detail.empty()) field("detail", e.detail);
        out_ << "}\n";
        check();
        ++count_;
    }

    // A missing footer is intentionally incomplete. Destruction cannot certify
    // a normal stop; bounded-limit, unsupported and fault exits pass false.
    void finish(bool complete) {
        if (finished_) throw std::logic_error("Timing trace already finished");
        out_ << "{\"kind\":\"footer\",\"eventCount\":" << count_
             << ",\"complete\":" << (complete ? "true" : "false") << "}\n";
        out_.flush();
        check();
        finished_ = true;
    }

private:
    void check() { if (!out_) throw std::runtime_error("Cannot write timing trace"); }
    void field(std::string_view name, std::string_view value) {
        out_ << ','; string(name); out_ << ':'; string(value);
    }
    void timestamp(TimingTimestamp value) {
        if (value.attoseconds >= 1000000000000000000ULL)
            throw std::invalid_argument("Non-normalized timing timestamp");
        out_ << "{\"seconds\":" << value.seconds << ",\"attoseconds\":" << value.attoseconds << '}';
    }
    void string(std::string_view value) {
        constexpr char hex[] = "0123456789abcdef";
        out_ << '"';
        for (unsigned char ch : value) {
            if (ch == '"' || ch == '\\') { out_ << '\\' << char(ch); }
            else if (ch < 32U || ch >= 127U) {
                out_ << "\\u00" << hex[ch >> 4U] << hex[ch & 15U];
            } else out_ << char(ch);
        }
        out_ << '"';
    }
    std::ostream &out_;
    std::uint64_t count_{};
    bool finished_{};
};
} // namespace gain_ground
