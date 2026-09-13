// Register behavior from pinned segas24.cpp and 315_5296.cpp (BSD-3-Clause).
#include "gain_ground/system24_devices.h"
#include <algorithm>

namespace gain_ground {
namespace {
std::uint64_t ticks(std::uint64_t ns, std::uint64_t hz)
{
    return (ns / 1000000000ULL) * hz + (ns % 1000000000ULL) * hz / 1000000000ULL;
}
}
void System24Devices::sync_timer(std::uint64_t ns)
{
    if (timer_mode_ == 1 || timer_mode_ == 3) {
        const auto tick = timer_mode_ == 1 ? ns / 41000U : ticks(ns, 8000000);
        const auto previous = timer_mode_ == 1 ? timer_sync_ns_ / 41000U : ticks(timer_sync_ns_, 8000000);
        timer_value_ = static_cast<std::uint16_t>(timer_value_ + tick - previous);
    }
    timer_sync_ns_ = ns;
}
void System24Devices::start_timer(std::uint8_t old_mode)
{
    // segas24_state::irq_timer_start: even unchanged data/mode writes re-arm
    // relative to the current bus time, not the next global scanline edge.
    if (timer_mode_ == 0) {
        if (old_mode) {
            timer_deadline_ns_ = UINT64_MAX;
            if (++timer_value_ == 4096) {
                timer_value_ = timer_data_;
                timer_pending_.fill(true); // Reference schedules a zero-delay expiry.
            }
        }
        return;
    }
    if (timer_value_ == 4096) {
        timer_value_ = timer_data_;
        timer_pending_.fill(true);
    }
    const auto period = timer_mode_ == 1 ? 41000ULL : 125ULL;
    timer_deadline_ns_ = now_ + (4096U - timer_value_) * period;
}
void System24Devices::advance(std::uint64_t ns)
{
    while (timer_deadline_ns_ <= ns) {
        now_ = timer_deadline_ns_;
        sync_timer(now_);
        timer_deadline_ns_ = UINT64_MAX;
        timer_value_ = timer_data_;
        start_timer(timer_mode_);
        timer_pending_.fill(true);
    }
    sync_timer(ns);
    const auto line = ticks(ns, 16000000) / 656;
    if (line != scanline_) {
        // MAME screen_device starts at VBLANK (beam line 384), not line 0.
        // The scanline timer starts at its next line 0: 40 * 41 us after reset.
        const auto beam = (line + 384U) % 424U;
        vblank_ = beam == 384U;
        sprite_ = beam == 0U;
        scanline_ = line;
    }
    now_ = ns;
    audio.advance_to_ns(ns);
}
std::uint64_t System24Devices::next_event_ns() const noexcept
{
    auto next = (scanline_ + 1U) * 41000ULL; // 656 clocks / 16 MHz
    next = std::min(next, timer_deadline_ns_);
    const auto ym = audio.next_event_ns();
    if (ym != UINT64_MAX) next = std::min(next, ym);
    return next;
}
void System24Devices::input(unsigned port, std::uint8_t bits, bool pressed)
{
    if (port < ports_.size()) ports_[port] = pressed ? ports_[port] & ~bits : ports_[port] | bits;
}
std::uint8_t System24Devices::irq_level(unsigned cpu) const
{
    if (sprite_ && (enables_[cpu] & 16)) return 5;
    if (vblank_ && (enables_[cpu] & 8)) return 4;
    if (timer_pending_[cpu] && (enables_[cpu] & 4)) return 3;
    if (audio.irq() && (enables_[cpu] & 2)) return 2;
    return 0;
}
std::optional<std::uint16_t> System24Devices::read(std::uint32_t address, std::uint16_t mask)
{
    const auto io = address & 0xe001ffU;
    if (io <= 0x80003fU && io >= 0x800000U) {
        const auto reg = (io >> 1) & 31;
        std::uint8_t value = 255;
        if (reg < 8) value = (direction_ & (1U << reg)) ? outputs_[reg] : ports_[reg];
        else if (reg < 12) value = static_cast<std::uint8_t>("SEGA"[reg - 8]);
        else if (reg == 12 || reg == 14) value = cnt_;
        else if (reg == 13 || reg == 15) value = direction_;
        return (0xff00U | value) & mask;
    }
    if (io >= 0x800100U && io <= 0x800103U) return (0xff00U | audio.read((io >> 1) & 1)) & mask;
    if (io >= 0x800040U && io <= 0x80007fU) return 0xffffU & mask; // iod_r
    const auto irq = address & 0xf00007U;
    if (irq >= 0xa00000U && irq <= 0xa00007U) {
        const auto reg = (irq >> 1) & 3;
        if (reg >= 2) timer_pending_[reg - 2] = false;
        return timer_value_ & mask;
    }
    const auto frc = address & 0xfc0007U;
    if ((frc & ~7U) == 0xbc0000U || (frc & ~7U) == 0xcc0000U) {
        if ((frc & 6U) == 4U) {
            const auto counter = ticks(now_ - frc_reset_, 10000000) / (frc_mode_ ? 1536 : 24);
            return static_cast<std::uint16_t>((0xff00U | (counter % (frc_mode_ ? 0x67 : 0x100))) & mask);
        }
    }
    return {};
}
bool System24Devices::write(std::uint32_t address, std::uint16_t value, std::uint16_t mask)
{
    const auto fdc_board = address & 0xf8000fU;
    if (fdc_board >= 0xb00008U && fdc_board <= 0xb0000fU) return true; // original fdc_ctrl_w logs only
    const auto io = address & 0xe001ffU;
    if (io >= 0x800000U && io <= 0x80003fU) {
        if (!(mask & 255)) return true;
        const auto reg = (io >> 1) & 31;
        const auto byte = static_cast<std::uint8_t>(value);
        if (reg < 8) {
            outputs_[reg] = byte;
            if (reg == 7 && (direction_ & 128)) audio.dac(byte);
        } else if (reg == 14) {
            // ym2151_device::reset_w resets the core on release (0 -> 1).
            if (!(cnt_ & 4U) && (byte & 4U)) audio.reset();
            cnt_ = byte;
        } else if (reg == 15) {
            if ((direction_ ^ byte) & 128) audio.dac((byte & 128) ? outputs_[7] : 0);
            direction_ = byte;
        }
        return true;
    }
    if (io >= 0x800100U && io <= 0x800103U) {
        if ((mask & 255) && (cnt_ & 4U)) audio.write((io >> 1) & 1, static_cast<std::uint8_t>(value));
        return true;
    }
    if (io >= 0x800040U && io <= 0x80007fU) return true;
    const auto irq = address & 0xf00007U;
    if (irq >= 0xa00000U && irq <= 0xa00007U) {
        const auto reg = (irq >> 1) & 3;
        if (reg == 0) {
            sync_timer(now_);
            timer_data_ = static_cast<std::uint16_t>(((timer_data_ & ~mask) | (value & mask)) & 4095);
            start_timer(timer_mode_);
        }
        else if (reg == 1 && (mask & 255)) {
            if ((value & 3) == 2) return false; // pinned source explicitly rejects mode 2
            sync_timer(now_);
            const auto old_mode = timer_mode_;
            timer_mode_ = value & 3;
            start_timer(old_mode);
        } else if (reg >= 2) { enables_[reg - 2] = value & 63; timer_pending_[reg - 2] = false; }
        return true;
    }
    const auto frc = address & 0xfc0007U;
    if ((frc & ~7U) == 0xbc0000U || (frc & ~7U) == 0xcc0000U) {
        if ((frc & 6U) == 2 && (mask & 255)) { frc_mode_ = value & 1; frc_reset_ = now_; return true; }
        if ((frc & 6U) == 4) return true;
    }
    return false;
}
} // namespace gain_ground
