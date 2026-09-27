#pragma once
#include "gain_ground/system24_devices.h"
#include <array>

namespace gain_ground {
// Gain Ground's activation code at FA9E..FAAA tests player button bits 0x06,
// not the generic SERVICE-port START bits. Enter aliases player 1's small attack.
inline void set_start_input(System24Devices &devices, unsigned player, bool pressed)
{
    if (player < 3U) devices.input(player, 0x02U, pressed);
}

// Physical sources remain independent, including mouse and controller journal
// codes. Releasing one source must not release another held alias.
class RuntimeKeyboard {
public:
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
    static bool mapped(unsigned key) {
        if(key==mouse_primary || key==mouse_secondary || (key>=0x110 && key<0x128))return true;
        switch (key) {
        case 'W': case 'A': case 'S': case 'D':
        case 'Q': case 'E': case 'F': case 0x0d: return true;
        default: return false;
        }
    }
    void publish(System24Devices &devices) const {
        std::array<unsigned,3> bits{};
        for(unsigned p=0;p<3;++p)for(unsigned bit=0;bit<8;++bit)
            if(held_[gamepad_key(p,bit)])bits[p]|=1U<<bit;
        if(held_['F'])bits[0]|=1;
        if(held_['Q'] || held_[0x0d] || held_[mouse_primary])bits[0]|=2;
        if(held_['E'] || held_[mouse_secondary])bits[0]|=4;
        if(held_['S'])bits[0]|=0x10;
        if(held_['W'])bits[0]|=0x20;
        if(held_['D'])bits[0]|=0x40;
        if(held_['A'])bits[0]|=0x80;
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
    template<class Emit> bool update(RuntimePadState state,bool focused,bool enabled,Emit emit){
        const bool menu=state.connected && (state.buttons&0x10)!=0;
        const bool pause=focused && menu && !menu_;
        menu_=menu;
        unsigned bits=0;
        if(state.connected){
            if(state.buttons&0x20)bits|=1; // View: credit.
            if(state.buttons&0x1000)bits|=2; // A: primary / join.
            if(state.buttons&0x4000)bits|=4; // X: secondary.
            // Axis thresholds with hysteresis prevent drift near the dead zone.
            horizontal_=axis(state.x,horizontal_);vertical_=axis(state.y,vertical_);
            if((state.buttons&2)||vertical_<0)bits|=0x10;
            if((state.buttons&1)||vertical_>0)bits|=0x20;
            if((state.buttons&8)||horizontal_>0)bits|=0x40;
            if((state.buttons&4)||horizontal_<0)bits|=0x80;
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
    int horizontal_{},vertical_{};
    unsigned published_{},suppressed_{};
    bool menu_{};
};
} // namespace gain_ground
