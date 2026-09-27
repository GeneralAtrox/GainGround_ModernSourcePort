#pragma once
#include <cstdint>
#include <string>

namespace gain_ground {
// Opt-in statistical profiler for the runtime's own thread. A helper thread
// suspends the sampled thread about a thousand times a second, records the
// instruction pointer relative to the executable's load address, and on stop
// writes "offset count" lines sorted by count. Offsets resolve against
// `nm -n` of a build with symbols (subtract the image's preferred base).
// Fibers share the thread, so guest execution is sampled like any other code.
// Never active unless started; intended for GAIN_GROUND_SAMPLE=<path>.
class HostSampler {
public:
    struct State; // sampling thread state, shared with the platform source
    HostSampler() = default;
    HostSampler(const HostSampler &) = delete;
    HostSampler &operator=(const HostSampler &) = delete;
    ~HostSampler();
    // Samples the calling thread until stop(); returns false if it cannot start.
    bool start(std::string output_path);
    void stop();
    bool running() const noexcept { return thread_ != nullptr; }

private:
    State *state_{};
    void *thread_{};
    std::string output_path_;
};
} // namespace gain_ground
