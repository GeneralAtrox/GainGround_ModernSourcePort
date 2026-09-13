#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_devices.h"
#include <algorithm>
#include <iostream>
#include <vector>
using namespace gain_ground;
struct Probe {
    RuntimeHost host; System24Devices devices;
    Probe() {
        host.attach_devices(devices); host.select_cpu(1);
        for(unsigned region:{2U,3U}) {
            std::vector<std::uint8_t> ram(host.region_bytes(region).size());
            host.load_region(region,0,ram);
        }
        host.set_checkpoint([](void *arg){auto &p=*static_cast<Probe*>(arg);
            auto next=p.host.next_cpu_deadline_ns();if(next!=UINT64_MAX)p.devices.advance(next);},this);
    }
    bool cue(unsigned site,unsigned command) {
        host.select_cpu(1);
        const std::uint8_t ret[]{0,0,0x90,0};host.load_region(2,0x7ff0,ret);
        FunctionContext c{};c.host=&host;c.cpu=1;c.state=0x72;
        c.registers.program_counter=0x1700c;c.registers.address[7]=0x7ff0;
        c.registers.data[0]=command;c.registers.status=0x2700;
        auto result=host.call_function(309,1,0x72,2,site,0x1700c,c);
        return !host.faulted() && result.status==TranslationStatus::complete && result.control==1;
    }
    bool accept_coin(bool accept) {
        host.select_cpu(0);
        const std::uint8_t stack[]{0,8,0x3d,0x1e,0,8,0x39,0x58};
        const std::uint8_t priority[]{0x80,0};
        host.load_region(3,0x37000,stack);host.load_region(3,0x3c032,priority);
        FunctionContext c{};c.host=&host;c.cpu=0;c.state=0xff;
        c.registers.program_counter=0x83e0c;c.registers.address[6]=0xffffc000;
        c.registers.address[7]=0xffff7000;c.registers.status=0x2700;
        c.registers.data[2]=0x36;
        c.registers.data[0]=c.registers.data[3]=accept ? 0x8000 : 0x0800;
        const auto result=host.call_function(85,0,0xff,2,0x83d1a,0x83e0c,c);
        return !host.faulted() && result.status==TranslationStatus::complete &&
            result.control==(accept ? 1U : 8U);
    }
    void write(unsigned reg,unsigned value){devices.audio.write(0,reg);devices.audio.write(1,value);}
    bool samples(int value){devices.advance(devices.time_ns()+32000);auto s=devices.audio.take_samples();
        return !s.empty() && std::all_of(s.begin(),s.end(),[&](auto v){return v==value;});}
};
bool credit_voice(bool after_eof,bool accept,unsigned channel) {
    Probe p;auto &a=p.devices.audio;
    std::vector<std::uint8_t> pcm(4096*4);
    for(unsigned i=0;i<pcm.size();i+=2){pcm[i]=0xe8;pcm[i+1]=3;}
    a.set_title_music(pcm);
    p.write(0x20+channel,0xc7);p.write(0x28+channel,0x4a);
    for(unsigned op=0;op<32;op+=8){p.write(0x40+channel+op,1);p.write(0x60+channel+op,0);
        p.write(0x80+channel+op,31);p.write(0xa0+channel+op,0);
        p.write(0xc0+channel+op,0);p.write(0xe0+channel+op,15);}
    if(!p.cue(0xd430,0x52))return false;
    p.write(8,0x7a);p.write(8,0x7b);
    p.devices.advance(p.devices.time_ns()+(after_eof ? 100000000 : 1000000));a.take_samples();
    const int pcm_value=after_eof ? 0 : 1000;
    if(!p.accept_coin(accept) || !p.samples(pcm_value))return false;
    // Acceptance must not expose the old sound; key-off or another voice's
    // key-on must not release this voice either.
    p.write(8,channel);p.write(8,channel==2 ? 0x7b : 0x7a);
    if(!p.samples(pcm_value))return false;
    p.write(8,0x78|channel);
    p.devices.advance(p.devices.time_ns()+10000000);const auto samples=a.take_samples();
    const bool heard=std::any_of(samples.begin(),samples.end(),[&](auto v){return v!=pcm_value;});
    if(heard!=accept)return false;
    // A later title visit restores masking and still ends at EOF.
    if(accept) {a.notify_sound_command(true);p.write(8,0x78|channel);}
    p.devices.advance(p.devices.time_ns()+100000000);a.take_samples();
    return p.samples(0);
}
int main(){
    for(bool eof:{false,true})for(bool accepted:{false,true})for(unsigned channel:{2U,3U})
        if(!credit_voice(eof,accepted,channel)){
            std::cerr<<"Credit voice failure: EOF="<<eof<<" accepted="<<accepted<<" channel="<<channel<<'\n';return 1;}
    Probe p; auto &a=p.devices.audio;
    std::vector<std::uint8_t> pcm(4096*4);
    for(unsigned i=0;i<pcm.size();i+=2){pcm[i]=0xe8;pcm[i+1]=3;} // Constant 1000, stereo.
    a.set_title_music(pcm);
    // Sounding original title voice makes any accidental unmute observable.
    p.write(0x22,0xc7);p.write(0x2a,0x4a);
    for(unsigned op=0;op<32;op+=8){p.write(0x42+op,1);p.write(0x62+op,0);
        p.write(0x82+op,31);p.write(0xa2+op,0);p.write(0xc2+op,0);p.write(0xe2+op,15);}
    if(!p.cue(0xd430,0x52) || !p.cue(0xdbd6,0x36))return 1;
    a.take_samples();p.write(8,0x7a);
    if(!p.samples(1000)){std::cerr<<"Coin cancelled pending title music\n";return 1;}
    for(unsigned i=0;i<3;++i){if(!p.cue(0xdbd6,0x36) || !p.samples(1000)){
        std::cerr<<"Coin interrupted playback or unmuted title voice\n";return 1;}}
    p.devices.advance(p.devices.time_ns()+100000000);a.take_samples();
    if(!p.cue(0xdbd6,0x36) || !p.samples(0)){std::cerr<<"Coin after EOF exposed title voice\n";return 1;}
    // Other scene cues still release the original channels and stop replacement.
    if(!p.cue(0xa752,0))return 1;
    p.devices.advance(p.devices.time_ns()+10000000);const auto released=a.take_samples();
    if(!std::any_of(released.begin(),released.end(),[](auto v){return v!=0;}))return 1;
    std::cout<<"PASS: queued coins preserve music; accepted credits release each voice at key-on; rejected credits stay masked; playback/EOF and scene release preserved\n";
}
