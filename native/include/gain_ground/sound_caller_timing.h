#pragma once
#include "gain_ground/m68000_timing.h"
#include "gain_ground/timing_trace.h"
#include <optional>

namespace gain_ground {
// Source-backed ordinary memory-bus timing for sound callers and IRQ bodies.
// The caller retains the original ordered accesses and inserts internal clocks
// at their microcode positions. This is not an instruction-trace producer.
class SoundCallerTiming {
public:
    explicit SoundCallerTiming(FunctionContext &context) : context_(context) {}
    void begin(std::uint32_t pc, const char *detail =
        "sound caller bus timing has no complete instruction observations") {
        timing_.emplace(*context_.host);
        TimingTraceEvent event;
        event.kind = "unsupported";
        event.cpu = context_.cpu; event.state = context_.state; event.pc = pc;
        event.time = TimingTimestamp::from_ns(context_.host->execution_time_ns());
        if (const auto position = context_.host->instruction_timing_position(context_.cpu))
            event.clocks = position->clocks;
        event.detail = detail;
        // Do not let a trace spanning this partially instrumented caller claim
        // complete coverage merely because both sound writers were observed.
        context_.host->observe_instruction_timing(event);
    }
    void stop() { timing_.reset(); }
    void clocks(std::uint32_t count) {
        if (!timing_) return;
        if (auto position = context_.host->instruction_timing_position(context_.cpu))
            position->clocks += count;
        timing_->clocks(count);
    }
    std::uint16_t read_memory_word(std::uint16_t region, std::uint32_t offset,
                                   std::uint16_t mask) {
        const auto value = context_.host->read_memory_word(region, offset, mask);
        clocks(4U);
        return value;
    }
    void write_memory_word(std::uint16_t region, std::uint32_t offset,
                           std::uint16_t value, std::uint16_t mask) {
        context_.host->write_memory_word(region, offset, value, mask);
        clocks(4U);
    }
    FunctionResult call_function(std::uint32_t id, std::uint8_t cpu,
        std::uint8_t state, std::uint8_t kind, std::uint32_t site,
        std::uint32_t target, FunctionContext &context) {
        // A child owns its timing; neither its accesses nor its elapsed time
        // are charged again by the parent's deadline.
        stop();
        return context_.host->call_function(id, cpu, state, kind, site, target, context);
    }
private:
    FunctionContext &context_;
    std::optional<M68000Timing> timing_;
};
} // namespace gain_ground
