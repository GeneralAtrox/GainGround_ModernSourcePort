#include "gain_ground/system24_audio.h"
#include "ymfm_opm.h"
#include "ymfm_fm.ipp"
#include <algorithm>
#include <limits>

namespace gain_ground {
namespace {
struct PresentationYm : ymfm::ym2151 {
    using ymfm::ym2151::ym2151;
    void generate_audible(output_data *output, std::uint32_t mask) {
        // Output also advances operator feedback. Synthesize muted voices into
        // a discarded mix so a later credit cue inherits the original state.
        m_fm.clock(fm_engine::ALL_CHANNELS);
        const auto muted = fm_engine::ALL_CHANNELS & ~mask;
        if (muted) {
            output_data discarded{};
            m_fm.output(discarded.clear(), 0, 32767, muted);
        }
        m_fm.output(output->clear(), 0, 32767, mask);
        output->roundtrip_fp();
    }
};
}
struct System24Audio::Core : ymfm::ymfm_interface {
    PresentationYm chip{*this};
    std::uint64_t clock{}, next_sample{64}, time_ns{}, busy_until_ns{};
    std::array<std::uint64_t,2> timers_ns{UINT64_MAX, UINT64_MAX};
    bool asserted{};
    int dac_value{};
    std::vector<std::int16_t> samples;
    std::vector<std::int16_t> title_music;
    std::size_t title_cursor{};
    std::uint8_t selected_register{};
    bool title_armed{}, title_playing{};
    std::uint8_t title_voice_mask{}, release_on_key_on{};
    void ymfm_set_timer(std::uint32_t number, std::int32_t duration) override {
        // ymfm_mame schedules from the exact machine/bus time, including
        // its phase within a chip clock. Samples keep their own clock grid.
        timers_ns[number] = duration < 0 ? UINT64_MAX : time_ns + std::uint64_t(duration) * 250U;
    }
    // MAME starts busy time at the exact bus timestamp, not a rounded chip tick.
    void ymfm_set_busy_end(std::uint32_t duration) override { busy_until_ns = time_ns + std::uint64_t(duration) * 250U; }
    bool ymfm_is_busy() override { return time_ns < busy_until_ns; }
    void ymfm_update_irq(bool value) override { asserted = value; }
    void advance(std::uint64_t target_ns) {
        for (;;) {
            const auto next_ns = std::min({next_sample * 250U, timers_ns[0], timers_ns[1]});
            if (next_ns > target_ns) break;
            clock = next_ns / 250U;
            time_ns = next_ns;
            for (unsigned i = 0; i < 2; ++i) if (timers_ns[i] == next_ns) {
                timers_ns[i] = UINT64_MAX;
                m_engine->engine_timer_expired(i);
            }
            if (next_sample * 250U == next_ns) {
                ymfm::ym2151::output_data out{};
                // Command 0x52's captured title effect uses YM channels 2/3.
                // Other channels (including unrelated effects) remain audible.
                chip.generate_audible(&out, 0xffU & ~title_voice_mask);
                for (unsigned channel = 0; channel < 2; ++channel)
                    samples.push_back(static_cast<std::int16_t>(std::clamp(
                        out.data[channel] / 2 + dac_value / 2 +
                        (title_playing ? title_music[title_cursor + channel] : 0), -32768, 32767)));
                if (title_playing) {
                    title_cursor += 2;
                    if (title_cursor == title_music.size()) title_playing = false;
                }
                next_sample += 64; // 4 MHz / 64 = 62,500 stereo sample frames/s.
            }
        }
        clock = target_ns / 250U;
        time_ns = target_ns;
    }
};
System24Audio::System24Audio() : core_(std::make_unique<Core>()) { core_->chip.reset(); }
System24Audio::~System24Audio() = default;
void System24Audio::advance_to_ns(std::uint64_t time_ns) { core_->advance(time_ns); }
void System24Audio::reset() { core_->chip.reset(); }
void System24Audio::write(unsigned port, std::uint8_t value) {
    if ((port & 1U) == 0U) core_->selected_register = value;
    else if (core_->title_armed && core_->selected_register == 8U &&
             (value & 0x78U) && ((value & 7U) == 2U || (value & 7U) == 3U)) {
        // Start on the actual first key-on, after the original command queue
        // and instrument setup. Both streams share the next 16-us sample edge.
        core_->title_armed = false;
        core_->title_playing = true;
        core_->title_cursor = 0;
        core_->title_voice_mask = 0x0cU;
        core_->release_on_key_on = 0U;
    }
    if ((port & 1U) && core_->selected_register == 8U && (value & 0x78U)) {
        const auto voice = static_cast<std::uint8_t>(1U << (value & 7U));
        if (core_->release_on_key_on & voice) {
            core_->title_voice_mask &= static_cast<std::uint8_t>(~voice);
            core_->release_on_key_on &= static_cast<std::uint8_t>(~voice);
        }
    }
    core_->chip.write(port, value);
}
std::uint8_t System24Audio::read(unsigned port) { return core_->chip.read(port); }
void System24Audio::dac(std::uint8_t value) { core_->dac_value = int(value) * 256 - 32768; }
bool System24Audio::irq() const noexcept { return core_->asserted; }
std::uint64_t System24Audio::next_event_ns() const noexcept {
    const auto timer = std::min(core_->timers_ns[0], core_->timers_ns[1]);
    return std::min(timer,
                    core_->busy_until_ns > core_->time_ns ? core_->busy_until_ns : UINT64_MAX);
}
std::vector<std::int16_t> System24Audio::take_samples() { std::vector<std::int16_t> out; out.swap(core_->samples); return out; }
bool System24Audio::set_title_music(std::span<const std::uint8_t> pcm) {
    if (pcm.empty() || pcm.size() % 4U) return false;
    core_->title_music.resize(pcm.size() / 2U);
    for (std::size_t i = 0; i < core_->title_music.size(); ++i)
        core_->title_music[i] = static_cast<std::int16_t>(std::uint16_t(pcm[2*i]) | (std::uint16_t(pcm[2*i+1]) << 8U));
    core_->title_armed = core_->title_playing = false;
    core_->title_voice_mask = core_->release_on_key_on = 0U;
    core_->title_cursor = 0;
    return true;
}
void System24Audio::notify_sound_command(bool title) {
    if (core_->title_music.empty()) return;
    core_->title_armed = title;
    if (!title) {
        core_->title_playing = false;
        core_->title_voice_mask = core_->release_on_key_on = 0U;
    }
}
void System24Audio::notify_coin_credit_accepted() {
    // F85 has accepted credit command 0x36; channel setup/key-off comes next.
    // The replacement PCM keeps its cursor and its single-play lifecycle.
    core_->release_on_key_on |= core_->title_voice_mask;
}
} // namespace gain_ground
