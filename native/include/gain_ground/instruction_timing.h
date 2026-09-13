#pragma once

#include "gain_ground/m68000_timing.h"
#include "gain_ground/timing_trace.h"
#include <limits>
#include <utility>

namespace gain_ground {
struct TimingInstructionIdentity {
    std::uint8_t cpu{}, state{};
    std::uint32_t pc{};
    std::uint16_t opcode{};
};

// Adapter for emitted native operations, not a second CPU interpreter. The
// supplied bus callable performs the real access once and returns its real data.
// The initial emitter supports the existing 10 MHz live mode and non-retrying
// aligned accesses; exceptional/waiting accesses must use a dedicated mapping.
class TimingInstruction {
public:
    TimingInstruction(ExecutionHost &host, TimingCpuPosition &position,
                      TimingInstructionIdentity identity,
                      TimingTraceWriter *trace = nullptr,
                      std::uint64_t clock_hz = 10000000U)
        : host_(host), timing_(host), position_(position), identity_(identity), trace_(trace) {
        if (clock_hz != 10000000U || position_.instruction_open || identity.cpu > 1U ||
            position_.instruction == std::numeric_limits<std::uint64_t>::max()) {
            good_ = false;
            return;
        }
        position_.instruction_open = true;
        ++position_.instruction;
        emit("instruction-begin", "");
    }
    TimingInstruction(const TimingInstruction &) = delete;
    TimingInstruction &operator=(const TimingInstruction &) = delete;
    // Incomplete instruction remains open in the interval, so the trace cannot
    // accidentally become complete when an exception unwinds this object.
    bool good() const noexcept { return good_; }
    bool clocks(std::uint32_t count) {
        if (!good_ || complete_) return false;
        if (position_.clocks > std::numeric_limits<std::uint64_t>::max() - count)
            return unsupported("cycle counter overflow");
        position_.clocks += count;
        timing_.clocks(count);
        return true;
    }
    bool unsupported(std::string_view reason) {
        if (good_) emit("unsupported", "", reason);
        good_ = false;
        return false;
    }
    // A missing observation invalidates a trace, but must not replace an
    // unavailable opcode fetch with a data read or stop otherwise translated
    // game execution. The synthetic zero return is never consumed as an opcode.
    void unobserved(std::string_view reason) {
        if (observable_) emit("unsupported", "", reason);
        observable_ = false;
    }
    template<class Access>
    std::uint16_t read(std::string_view location, std::string_view space,
                       std::uint8_t width, std::uint32_t address,
                       std::uint16_t mask, Access &&access) {
        if (!access_valid(width, address, mask)) return 0U;
        const auto time = TimingTimestamp::from_ns(host_.execution_time_ns());
        const auto value = static_cast<std::uint16_t>(std::forward<Access>(access)());
        bus("bus-read", location, space, width, address, mask, value, time);
        clocks(4U);
        return value;
    }
    template<class Access>
    void write(std::string_view location, std::string_view space,
               std::uint8_t width, std::uint32_t address,
               std::uint16_t mask, std::uint16_t value, Access &&access) {
        if (!access_valid(width, address, mask)) return;
        const auto time = TimingTimestamp::from_ns(host_.execution_time_ns());
        std::forward<Access>(access)();
        bus("bus-write", location, space, width, address, mask, value, time);
        clocks(4U);
    }
    bool complete(std::uint32_t next_pc, std::uint8_t state, std::string_view detail) {
        if (!good_ || complete_) return false;
        auto event = base("instruction-complete", "");
        event.next_pc = next_pc;
        event.detail = detail;
        if (observable_) {
            if (trace_) trace_->event(event);
            host_.observe_instruction_timing(event);
        }
        if (state != identity_.state) {
            identity_.state = state;
            emit("state", "");
        }
        position_.instruction_open = false;
        complete_ = true;
        return true;
    }

private:
    bool access_valid(std::uint8_t width, std::uint32_t address, std::uint16_t mask) {
        if (!good_ || complete_) return false;
        if (width != 8U && width != 16U) return unsupported("unsupported bus width");
        if (width == 16U && ((address & 1U) || mask != 0xffffU))
            return unsupported("word exception/mask mapping required");
        if (width == 8U && mask != ((address & 1U) ? 0x00ffU : 0xff00U))
            return unsupported("byte lane/address mismatch");
        return true;
    }
    TimingTraceEvent base(std::string_view kind, std::string_view location) {
        TimingTraceEvent event;
        event.kind = kind; event.cpu = identity_.cpu; event.state = identity_.state;
        event.pc = identity_.pc; event.opcode = identity_.opcode;
        event.instruction = position_.instruction; event.phase = phase_++;
        event.clocks = position_.clocks;
        event.time = TimingTimestamp::from_ns(host_.execution_time_ns());
        event.location = location;
        return event;
    }
    void emit(std::string_view kind, std::string_view location, std::string_view detail = {}) {
        auto event = base(kind, location); event.detail = detail;
        if (observable_) {
            if (trace_) trace_->event(event);
            host_.observe_instruction_timing(event);
        }
    }
    void bus(std::string_view kind, std::string_view location, std::string_view space,
             std::uint8_t width, std::uint32_t address, std::uint16_t mask,
             std::uint16_t value, TimingTimestamp time) {
        auto event = base(kind, location);
        event.space = space; event.width = width; event.address = address;
        event.mask = mask; event.value = value & mask; event.time = time;
        if (observable_) {
            if (trace_) trace_->event(event);
            host_.observe_instruction_timing(event);
        }
    }
    ExecutionHost &host_;
    M68000Timing timing_;
    TimingCpuPosition &position_;
    TimingInstructionIdentity identity_;
    TimingTraceWriter *trace_;
    std::uint64_t phase_{};
    bool good_{true}, complete_{}, observable_{true};
};
} // namespace gain_ground
