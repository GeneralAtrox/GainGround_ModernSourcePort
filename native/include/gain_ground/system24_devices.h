#pragma once
#include "gain_ground/system24_audio.h"
#include <array>
#include <cstdint>
#include <optional>

namespace gain_ground {
// Source-derived board I/O and clocks. CPU instruction timing is not certified.
class System24Devices {
public:
    void advance(std::uint64_t nanoseconds);
    std::uint64_t next_event_ns() const noexcept;
    std::optional<std::uint16_t> read(std::uint32_t address, std::uint16_t mask);
    bool write(std::uint32_t address, std::uint16_t value, std::uint16_t mask);
    std::uint8_t irq_level(unsigned cpu) const;
    bool cpu_b_enabled() const { return (cnt_ & 2U) != 0; }
    void input(unsigned port, std::uint8_t bits, bool pressed);
    System24Audio audio;
    std::uint64_t frame() const { return scanline_ / 424; }
    std::uint64_t time_ns() const noexcept { return now_; }
private:
    void sync_timer(std::uint64_t nanoseconds);
    void start_timer(std::uint8_t old_mode);
    std::array<std::uint8_t,8> ports_{255,255,255,255,255,255,253,255}, outputs_{};
    std::uint8_t cnt_{}, direction_{}, timer_mode_{}, frc_mode_{};
    std::array<std::uint8_t,2> enables_{};
    std::array<bool,2> timer_pending_{};
    std::uint16_t timer_data_{}, timer_value_{};
    std::uint64_t now_{}, scanline_{}, timer_sync_ns_{}, frc_reset_{};
    std::uint64_t timer_deadline_ns_{UINT64_MAX};
    bool vblank_{}, sprite_{};
};
} // namespace gain_ground
