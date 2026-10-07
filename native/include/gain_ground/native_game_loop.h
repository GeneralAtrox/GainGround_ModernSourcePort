#pragma once
#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_devices.h"

namespace gain_ground {
// A single serial game loop. Register contexts are call adapters for existing
// translations; they are never independently clocked or resumed CPU workers.
class NativeGameLoop {
public:
    NativeGameLoop(RuntimeHost &host, System24Devices &devices) : host_(host), devices_(devices) {}
    bool start(DirectAssetLoader &assets);
    bool frame();
    const FunctionContext &context() const noexcept { return game_; }
    std::uint64_t updates() const noexcept { return updates_; }
    std::uint64_t sound_services() const noexcept { return sound_services_; }
private:
    RuntimeHost &host_;
    System24Devices &devices_;
    FunctionContext game_{};
    bool ready_{}, servicing_{};
    std::uint64_t updates_{}, sound_services_{};
    static void service(void *self);
    void services();
    bool service_interrupt(unsigned level);
    FunctionResult invoke(FunctionContext &c, std::uint32_t target, std::uint32_t resume);
    bool returned(const FunctionResult &result, const FunctionContext &c,
                  std::uint32_t resume, std::uint32_t sp);
};
}
