#include "gain_ground/runtime_input.h"
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <vector>
using namespace gain_ground;
namespace {
void check(bool ok,const char *why){if(!ok)throw std::runtime_error(why);}
unsigned port(System24Devices &d,unsigned p){return *d.read(0x800000+p*2,0xff);}
}
int main(){try{
    System24Devices d;RuntimeKeyboard input;std::array<RuntimePad,3> pads;
    std::vector<std::tuple<unsigned,bool>> journal;
    const auto sample=[&](unsigned p,RuntimePadState s,bool focus=true,bool enabled=true){
        return pads[p].update(s,focus,enabled,[&](unsigned bit,bool pressed){
            const auto key=RuntimeKeyboard::gamepad_key(p,bit);input.key(d,key,pressed);journal.emplace_back(key,pressed);
        });
    };
    for(unsigned p=0;p<3;++p)sample(p,{true,0x1020,20000,0});
    for(unsigned p=0;p<3;++p)check((port(d,p)&0x43)==0,"Controller did not drive its player slot");
    check((port(d,4)&0x43)==0,"Three independent coin inputs missing");
    sample(1,{});check(port(d,1)==255 && (port(d,0)&2)==0 && (port(d,2)&2)==0,"Disconnect reassigned or released another player");
    input.key(d,'Z',true);sample(0,{});check(!(port(d,0)&2),"Disconnect released held keyboard action");
    input.key(d,RuntimeKeyboard::mouse_primary,true);input.key(d,'Z',false);check(!(port(d,0)&2),"Keyboard release cancelled mouse");
    sample(0,{true,0x1000,0,0});input.key(d,RuntimeKeyboard::mouse_primary,false);check(!(port(d,0)&2),"Mouse release cancelled controller");
    sample(0,{true,0,20000,0});input.key(d,0x25,true);check((port(d,0)&0xc0)==0xc0,"Opposite devices produced invalid direction");
    input.key(d,0x25,false);check(!(port(d,0)&0x40),"Direction did not resume after opposite release");
    sample(0,{true,0,8000,0});check(!(port(d,0)&0x40),"Stick hysteresis lost held direction");
    sample(0,{true,0,6000,0});check((port(d,0)&0xf0)==0xf0,"Stick drift was not released");
    sample(0,{true,0,8000,0});check((port(d,0)&0xf0)==0xf0,"Stick drift entered movement");
    sample(0,{true,0x4005,0,0});check((port(d,0)&0xa4)==0,"D-pad diagonal or X attack missing");
    check(sample(0,{true,0x10,0,0}),"Menu rising edge did not pause");
    check(!sample(0,{true,0x10,0,0},true,false),"Held Menu toggled pause repeatedly");
    sample(0,{true,0x1020,0,0},true,false);
    sample(0,{true,0x1020,0,0});check(port(d,0)==255,"Paused button leaked or added another credit on resume");
    sample(0,{true,0,0,0});sample(0,{true,0x1020,0,0});check((port(d,0)&3)==0,"Button did not rearm after release");
    sample(0,{true,0x1020,0,0},false);check(port(d,0)==255,"Unfocused input leaked");
    sample(0,{true,0x1020,0,0});check(port(d,0)==255,"Held input leaked after focus returned");
    input.release_all(d);for(auto &pad:pads)pad.release();
    for(unsigned p=0;p<3;++p)check(port(d,p)==255,"Release-all left a held player input");
    check(!input.key(d,RuntimeKeyboard::gamepad_key(3,0),true),"Fourth controller was accepted");
    // The existing key-based crash journal must reproduce controller edges.
    System24Devices replay;RuntimeKeyboard replay_input;
    for(auto [key,pressed]:journal)replay_input.key(replay,key,pressed);
    check(!(port(replay,2)&2),"Controller 3 journal did not replay independently");
    std::cout<<"PASS: three slots, mixed sources, dead zones, disconnects, pause/focus and replay\n";
}catch(const std::exception &e){std::cerr<<e.what()<<'\n';return 1;}}
