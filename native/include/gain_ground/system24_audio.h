#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <span>
#include <vector>

namespace gain_ground {
class System24Audio {
public:
    System24Audio();
    ~System24Audio();
    void advance_to_ns(std::uint64_t time_ns);
    void reset();
    void write(unsigned port, std::uint8_t data);
    std::uint8_t read(unsigned port);
    void dac(std::uint8_t value);
    bool irq() const noexcept;
    std::uint64_t next_event_ns() const noexcept;
    std::vector<std::int16_t> take_samples();
    // Optional presentation replacement: stereo s16le at the board's 62,500 Hz.
    // The original YM voices, registers and timers continue to execute.
    bool set_title_music(std::span<const std::uint8_t> pcm);
    void notify_sound_command(bool original_title_effect);
    // Accepted credit cue reuses the title voices. Release each voice only
    // when its new key-on arrives, never while the old title sound is running.
    void notify_coin_credit_accepted();
private:
    struct Core;
    std::unique_ptr<Core> core_;
};
} // namespace gain_ground
