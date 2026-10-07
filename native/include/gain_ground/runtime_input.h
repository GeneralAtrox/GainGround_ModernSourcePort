#pragma once
#include "gain_ground/system24_devices.h"
#include <array>
#include <istream>
#include <ostream>
#include <string>

namespace gain_ground {
// Remappable player controls, in the order the Controls dialog lists them.
enum RuntimeAction : unsigned { action_up, action_down, action_left, action_right,
    action_attack, action_big_attack, action_credit, action_pause, action_count };
// Player port bit each action drives; pause is handled by the window instead.
inline constexpr std::array<unsigned,action_count> action_bits{0x20,0x10,0x80,0x40,2,4,1,0};
inline constexpr std::array<const char *,action_count> action_names{
    "up","down","left","right","attack","big_attack","credit","pause"};

// Keyboard codes are Win32 virtual keys (player 1); controller codes are XInput
// button masks shared by all three slots. 0 leaves an action unbound. The left
// stick always moves in addition to the bound direction buttons.
struct RuntimeBindings {
    std::array<unsigned,action_count> keys{'W','S','A','D','Q','E','F','P'};
    std::array<unsigned,action_count> pad{1,2,4,8,0x1000,0x4000,0x20,0x10};
    // Escape exits and Pause always pauses; Windows keys belong to the shell.
    static bool reserved_key(unsigned key){return key==0 || key>0xff || key==0x1b || key==0x13 || key==0x5b || key==0x5c;}
    // A code already used by another action moves there in exchange, so one
    // press can never drive two actions.
    static void assign(std::array<unsigned,action_count> &codes,unsigned action,unsigned code){
        for(auto &other:codes)if(other==code)other=codes[action];
        codes[action]=code;
    }
    // "keyboard.<action>=<code>" / "controller.<action>=<code>" lines; unknown
    // or malformed entries are ignored and leave the defaults.
    void read(std::istream &in){
        std::string line;
        while(std::getline(in,line)){
            const auto dot=line.find('.'),equals=line.find('=');
            if(dot==std::string::npos || equals==std::string::npos || equals<dot)continue;
            const auto device=line.substr(0,dot),name=line.substr(dot+1,equals-dot-1);
            unsigned long code=0;
            try{code=std::stoul(line.substr(equals+1),nullptr,0);}catch(...){continue;}
            for(unsigned a=0;a<action_count;++a)if(name==action_names[a]){
                if(device=="keyboard" && !reserved_key(unsigned(code)))assign(keys,a,unsigned(code));
                else if(device=="controller" && code>0 && code<=0xffff && !(code&(code-1)))assign(pad,a,unsigned(code));
            }
        }
    }
    void write(std::ostream &out) const {
        for(unsigned a=0;a<action_count;++a)out<<"keyboard."<<action_names[a]<<'='<<keys[a]<<'\n';
        for(unsigned a=0;a<action_count;++a)out<<"controller."<<action_names[a]<<"=0x"<<std::hex<<pad[a]<<std::dec<<'\n';
    }
};

// Gain Ground's activation code at FA9E..FAAA tests player button bits 0x06,
// not the generic SERVICE-port START bits. Enter aliases player 1's small attack.
inline void set_start_input(System24Devices &devices, unsigned player, bool pressed)
{
    if (player < 3U) devices.input(player, 0x02U, pressed);
}

// Physical sources remain independent, including mouse and controller journal
// codes. Releasing one source must not release another held alias. Enter joins
// as player 1's attack unless it is bound to another action.
class RuntimeKeyboard {
public:
    // Callers release held input before changing bindings.
    void set_bindings(const std::array<unsigned,action_count> &keys){keys_=keys;}
    static constexpr unsigned mouse_primary=0x100, mouse_secondary=0x101;
    static constexpr unsigned gamepad_key(unsigned player,unsigned bit){return 0x110+player*8+bit;}
    bool key(System24Devices &devices, unsigned key, bool pressed) {
        if (key >= held_.size() || !mapped(key)) return false;
        held_[key] = pressed;
        publish(devices);
        return true;
    }
    void release_all(System24Devices &devices) {
        held_.fill(false);
        publish(devices);
    }
private:
    std::array<bool, 0x128> held_{};
    std::array<unsigned,action_count> keys_{RuntimeBindings{}.keys};
    bool enter_joins() const {
        for(const auto key:keys_)if(key==0x0d)return false;
        return true;
    }
    bool mapped(unsigned key) const {
        if(key==mouse_primary || key==mouse_secondary || (key>=0x110 && key<0x128))return true;
        for(unsigned a=0;a<action_pause;++a)if(keys_[a]==key)return true;
        return key==0x0d && enter_joins();
    }
    void publish(System24Devices &devices) const {
        std::array<unsigned,3> bits{};
        for(unsigned p=0;p<3;++p)for(unsigned bit=0;bit<8;++bit)
            if(held_[gamepad_key(p,bit)])bits[p]|=1U<<bit;
        for(unsigned a=0;a<action_pause;++a)if(keys_[a]<0x100 && held_[keys_[a]])bits[0]|=action_bits[a];
        if((held_[0x0d] && enter_joins()) || held_[mouse_primary])bits[0]|=2;
        if(held_[mouse_secondary])bits[0]|=4;
        constexpr unsigned coins[]{1,2,0x40};
        for(unsigned p=0;p<3;++p){
            // Opposing directions from two devices cancel, rather than feeding
            // an invalid direction combination into the original lookup table.
            if((bits[p]&0x30)==0x30)bits[p]&=~0x30U;
            if((bits[p]&0xc0)==0xc0)bits[p]&=~0xc0U;
            devices.input(p,0xf7,false);devices.input(p,static_cast<std::uint8_t>(bits[p]&0xf7),true);
            devices.input(4,static_cast<std::uint8_t>(coins[p]),(bits[p]&1)!=0);
        }
    }
};

// Portable XInput-state decoding; the Windows poller supplies raw fields.
struct RuntimePadState { bool connected{}; unsigned buttons{}; int x{},y{}; };
class RuntimePad {
public:
    // Callers release held input before changing bindings.
    void set_bindings(const std::array<unsigned,action_count> &buttons){buttons_=buttons;}
    template<class Emit> bool update(RuntimePadState state,bool focused,bool enabled,Emit emit){
        const bool menu=state.connected && (state.buttons&buttons_[action_pause])!=0;
        const bool pause=focused && menu && !menu_;
        menu_=menu;
        unsigned bits=0;
        if(state.connected){
            for(unsigned a=0;a<action_pause;++a)if(state.buttons&buttons_[a])bits|=action_bits[a];
            // Axis thresholds with hysteresis prevent drift near the dead zone.
            horizontal_=axis(state.x,horizontal_);vertical_=axis(state.y,vertical_);
            if(vertical_<0)bits|=0x10;
            if(vertical_>0)bits|=0x20;
            if(horizontal_>0)bits|=0x40;
            if(horizontal_<0)bits|=0x80;
        }else horizontal_=vertical_=0;
        if(!focused || !enabled || pause)suppressed_=bits;
        else suppressed_&=bits;
        const auto next=focused && enabled && !pause ? bits&~suppressed_:0U;
        for(unsigned bit=0;bit<8;++bit)if((next^published_)&(1U<<bit))emit(bit,(next&(1U<<bit))!=0);
        published_=next;
        return pause;
    }
    void release(){published_=0;}
private:
    static int axis(int value,int previous){
        if(value>9000)return 1;
        if(value< -9000)return -1;
        if(previous>0 && value>6500)return 1;
        if(previous<0 && value< -6500)return -1;
        return 0;
    }
    std::array<unsigned,action_count> buttons_{RuntimeBindings{}.pad};
    int horizontal_{},vertical_{};
    unsigned published_{},suppressed_{};
    bool menu_{};
};
} // namespace gain_ground
