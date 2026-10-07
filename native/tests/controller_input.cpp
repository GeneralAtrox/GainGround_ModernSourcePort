#include "gain_ground/runtime_input.h"
#include <iostream>
#include <sstream>
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
    for(const auto [key,bit]:std::array<std::pair<unsigned,unsigned>,8>{{
        {'W',0x20},{'A',0x80},{'S',0x10},{'D',0x40},
        {'Q',2},{'E',4},{'F',1},{0x0d,2}}}) {
        check(input.key(d,key,true),"Default key was not mapped");
        check(port(d,0)==(255U&~bit),"Default key drove the wrong action");
        check(port(d,1)==255 && port(d,2)==255,"Keyboard controlled another player");
        check(port(d,4)==(bit==1 ? 254U:255U),"Keyboard drove the wrong coin slot");
        input.key(d,key,false);
        check(port(d,0)==255 && port(d,4)==255,"Key release left input held");
    }
    for(unsigned key:std::array<unsigned,11>{'1','2','5','6','7','Z','X',0x25,0x26,0x27,0x28})
        check(!input.key(d,key,true),"Legacy keyboard binding remains enabled");
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
    input.key(d,'Q',true);sample(0,{});check(!(port(d,0)&2),"Disconnect released held keyboard action");
    input.key(d,RuntimeKeyboard::mouse_primary,true);input.key(d,'Q',false);check(!(port(d,0)&2),"Keyboard release cancelled mouse");
    sample(0,{true,0x1000,0,0});input.key(d,RuntimeKeyboard::mouse_primary,false);check(!(port(d,0)&2),"Mouse release cancelled controller");
    sample(0,{true,0,20000,0});input.key(d,'A',true);check((port(d,0)&0xc0)==0xc0,"Opposite devices produced invalid direction");
    input.key(d,'A',false);check(!(port(d,0)&0x40),"Direction did not resume after opposite release");
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
    // Remapping: a used code swaps actions, Enter joins only while unbound,
    // and the saved form round-trips while rejecting reserved or malformed codes.
    RuntimeBindings custom;
    RuntimeBindings::assign(custom.keys,action_attack,' ');
    RuntimeBindings::assign(custom.keys,action_credit,0x0d);
    RuntimeBindings::assign(custom.keys,action_up,'E');
    check(custom.keys[action_big_attack]=='W',"Rebinding a used key did not swap it");
    System24Devices m;RuntimeKeyboard mapped;mapped.set_bindings(custom.keys);
    check(mapped.key(m,' ',true) && port(m,0)==(255U&~2U),"Rebound attack key missing");
    mapped.key(m,' ',false);
    check(!mapped.key(m,'Q',true),"Replaced key remained bound");
    check(mapped.key(m,0x0d,true) && port(m,0)==254 && port(m,4)==254,"Enter bound to credit also joined");
    mapped.key(m,0x0d,false);
    check(mapped.key(m,'E',true) && port(m,0)==(255U&~0x20U),"Swapped key drove the wrong action");
    mapped.key(m,'E',false);
    check(!mapped.key(m,'P',true),"Pause key reached the game");
    RuntimeBindings::assign(custom.pad,action_attack,0x2000);
    RuntimeBindings::assign(custom.pad,action_pause,0x8000);
    System24Devices pd;RuntimeKeyboard pad_input;RuntimePad pad;pad.set_bindings(custom.pad);
    const auto pad_sample=[&](unsigned buttons){
        return pad.update({true,buttons,0,0},true,true,[&](unsigned bit,bool pressed){pad_input.key(pd,RuntimeKeyboard::gamepad_key(0,bit),pressed);});
    };
    pad_sample(0x1000);check(port(pd,0)==255,"Unbound controller button still attacked");
    pad_sample(0x2000);check(port(pd,0)==(255U&~2U),"Rebound controller attack missing");
    check(!pad_sample(0x10) && pad_sample(0x8010),"Rebound controller pause did not move");
    std::stringstream saved;custom.write(saved);
    RuntimeBindings loaded;loaded.read(saved);
    check(loaded.keys==custom.keys && loaded.pad==custom.pad,"Saved bindings did not round-trip");
    std::stringstream bad("keyboard.attack=27\ncontroller.up=0x3\nkeyboard.nothing=65\ngarbage\nkeyboard.credit=x\n");
    RuntimeBindings defaults;defaults.read(bad);
    check(defaults.keys==RuntimeBindings{}.keys && defaults.pad==RuntimeBindings{}.pad,"Invalid saved bindings were accepted");
    std::cout<<"PASS: three slots, mixed sources, dead zones, disconnects, pause/focus, replay and remapping\n";
}catch(const std::exception &e){std::cerr<<e.what()<<'\n';return 1;}}
