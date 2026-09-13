#include "gain_ground/system24_audio.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <stdexcept>

using gain_ground::System24Audio;
void check(bool value, const char *message) { if (!value) throw std::runtime_error(message); }
void write(System24Audio &audio, unsigned reg, unsigned value) { audio.write(0, reg); audio.write(1, value); }
void tone(System24Audio &audio, unsigned channel) {
    write(audio, 0x20+channel, 0xc7); write(audio, 0x28+channel, 0x4a);
    for (unsigned op=0; op<32; op+=8) {
        write(audio,0x40+op+channel,1); write(audio,0x60+op+channel,0);
        write(audio,0x80+op+channel,31); write(audio,0xa0+op+channel,0);
        write(audio,0xc0+op+channel,0); write(audio,0xe0+op+channel,15);
    }
}
int main() { try {
    const std::array<std::uint8_t,12> pcm{1,0,2,0,3,0,4,0,5,0,6,0};
    System24Audio audio;
    check(!audio.set_title_music({}) && !audio.set_title_music({pcm.data(),3}), "Invalid PCM accepted");
    check(audio.set_title_music(pcm), "PCM rejected");
    audio.notify_sound_command(true);
    audio.advance_to_ns(16000); check(audio.take_samples()==std::vector<std::int16_t>({0,0}), "Command started music before key-on");
    write(audio,8,2); // key off must not trigger
    audio.advance_to_ns(32000); check(audio.take_samples()==std::vector<std::int16_t>({0,0}), "Key-off triggered music");
    audio.advance_to_ns(33000); write(audio,8,0x7a);
    audio.advance_to_ns(47999); check(audio.take_samples().empty(), "Music started before board sample edge");
    audio.advance_to_ns(48000); check(audio.take_samples()==std::vector<std::int16_t>({1,2}), "First music frame not aligned to key-on");
    write(audio,8,0x7b); // second title voice must not restart the clip
    audio.advance_to_ns(96000);
    check(audio.take_samples()==std::vector<std::int16_t>({3,4,5,6,0,0}), "Playback restarted, looped or failed to stop at EOF");
    audio.advance_to_ns(160000); auto silence=audio.take_samples();
    check(std::all_of(silence.begin(),silence.end(),[](auto s){return s==0;}), "Clip looped after EOF");
    audio.notify_sound_command(true); write(audio,8,0x7a); audio.advance_to_ns(176000);
    check(audio.take_samples()==std::vector<std::int16_t>({1,2}), "New title event did not start a fresh single play");
    audio.notify_sound_command(false); audio.advance_to_ns(192000);
    check(audio.take_samples()==std::vector<std::int16_t>({0,0}), "Next sound command did not end title override");

    // A real sounding title voice is suppressed, but unrelated channel 5 is
    // identical to the baseline; busy/IRQ/timer state also remains identical.
    for (unsigned channel : {2U,5U}) {
        System24Audio baseline, replacement;
        std::array<std::uint8_t,4> zero{}; replacement.set_title_music(zero);
        tone(baseline,channel); tone(replacement,channel);
        replacement.notify_sound_command(true);
        write(baseline,8,0x7a); write(replacement,8,0x7a);
        write(baseline,8,0x78|channel); write(replacement,8,0x78|channel);
        for(auto *a : {&baseline,&replacement}) { write(*a,0x10,255); write(*a,0x11,3); write(*a,0x14,5); a->advance_to_ns(10000000); }
        check(baseline.read(1)==replacement.read(1) && baseline.irq()==replacement.irq() && baseline.next_event_ns()==replacement.next_event_ns(), "Presentation changed YM timers/status");
        const auto original=baseline.take_samples(), changed=replacement.take_samples();
        check(std::any_of(original.begin(),original.end(),[](auto s){return s!=0;}), "Test voice did not sound");
        if(channel==2) check(std::all_of(changed.begin(),changed.end(),[](auto s){return s==0;}), "Original title voice leaked through");
        else check(original==changed,"Unrelated sound channel changed");
    }
    // YM output also updates operator feedback. Muting a channel must discard
    // its output, not skip synthesis: the later coin reuses that exact state.
    for(unsigned channel:{2U,3U}) {
        System24Audio baseline, replacement;
        const std::array<std::uint8_t,4> zero{};replacement.set_title_music(zero);
        for(auto *a:{&baseline,&replacement}) {
            tone(*a,channel);write(*a,0x20+channel,0xff); // Maximum self-feedback.
        }
        replacement.notify_sound_command(true);
        for(auto *a:{&baseline,&replacement}) {
            write(*a,8,0x7a);write(*a,8,0x78|channel);
            a->advance_to_ns(10000000);a->take_samples();
        }
        replacement.notify_coin_credit_accepted();
        for(auto *a:{&baseline,&replacement}) {
            write(*a,8,channel);write(*a,8,0x78|channel);
            a->advance_to_ns(12000000);
        }
        const auto expected=baseline.take_samples(),actual=replacement.take_samples();
        check(std::any_of(expected.begin(),expected.end(),[](auto v){return v!=0;}),"Feedback test voice is silent");
        check(expected==actual,"Title mute changed FM feedback before credit handover");
    }
    std::cout << "PASS: key-on alignment, no restart/loop, EOF, new title, next cue, voice masking, preserved FM feedback and unchanged YM timers\n";
} catch(const std::exception &e) { std::cerr<<e.what()<<'\n'; return 1; } }
