#include "gain_ground/gameplay/enemy_navigation.h"
#include "gain_ground/gameplay/game_definitions.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace gain_ground::gameplay {
namespace {
// Opt-in diagnostics: GAIN_GROUND_NAV_LOG=<path> appends one line whenever the
// adapter stands aside or holds an actor, sampled every 30 frames per actor.
std::FILE *nav_log(){
    static std::FILE *file=[]{const char *path=std::getenv("GAIN_GROUND_NAV_LOG");return path && *path ? std::fopen(path,"a"):nullptr;}();
    return file;
}
void log_actor(const char *why,std::span<const std::uint8_t> r,unsigned a,std::uint64_t frame,
               double goal_x,double goal_y,const char *extra=""){
    auto *file=nav_log();if(!file || frame%30)return;
    const auto n=[&](unsigned o){return (unsigned(r[a+o])<<24)|(unsigned(r[a+o+1])<<16)|(unsigned(r[a+o+2])<<8)|r[a+o+3];};
    const auto fx=[&](unsigned o){return static_cast<std::int32_t>(n(o))/65536.0;};
    std::fprintf(file,"frame %llu %s record %05x callback %06x descriptor %06x pos %.1f,%.1f intent %.2f,%.2f target %u goal %.0f,%.0f +40 %02x +41 %02x +59 %02x +5c %04x%s\n",
        static_cast<unsigned long long>(frame),why,a,n(2),n(0x6e),fx(0x12),fx(0x1a),fx(0x1e),fx(0x26),unsigned(r[a+0x60]),goal_x,goal_y,
        unsigned(r[a+0x40]),unsigned(r[a+0x41]),unsigned(r[a+0x59]),(unsigned(r[a+0x5c])<<8)|r[a+0x5d],extra);
    std::fflush(file);
}
unsigned word(std::span<const std::uint8_t> r,unsigned a){return a+1<r.size() ? (unsigned(r[a])<<8)|r[a+1]:0;}
std::uint32_t number(std::span<const std::uint8_t> r,unsigned a){return (word(r,a)<<16)|word(r,a+2);}
double fixed(std::span<const std::uint8_t> r,unsigned a){return static_cast<std::int32_t>(number(r,a))/65536.0;}
void put(std::span<std::uint8_t> r,unsigned a,double value){
    auto n=static_cast<std::uint32_t>(static_cast<std::int32_t>(value*65536));
    for(unsigned i=0;i<4;++i)r[a+i]=static_cast<std::uint8_t>(n>>(24-i*8));
}
bool actor(std::span<const std::uint8_t> r,unsigned a){
    return a>=0x3480 && a<0x6c00 && (a&127)==0 && a+127<r.size() &&
        (r[a]&128) && (r[a+0xb]==8 || r[a+0xb]==9) && r[a+0x3f]==0;
}
NavigationBody body(std::span<const std::uint8_t> r,unsigned a){return {a,{fixed(r,a+0x12),fixed(r,a+0x1a)},10,9};}
class World final : public NavigationWorld {
public:
    World(std::span<const std::uint8_t> r,std::span<const std::uint8_t> s,unsigned a,
          std::span<const NavigationReservation> reservations):shared(s),claims(reservations){
        // F355 uses the descriptor-selected movement rectangle and F287's
        // column-major 8-pixel attribute lookup (solid/water mask 0x09).
        const auto descriptor=number(r,a+0x6e);
        const auto index=word(r,descriptor+8);
        const auto table=0x32dec+index*4;
        if(descriptor+9>=r.size() || table+3>=r.size())return;
        bounds={int(static_cast<std::int8_t>(r[table]))*8,int(static_cast<std::int8_t>(r[table+1]))*8,
                int(static_cast<std::int8_t>(r[table+2]))*8,int(static_cast<std::int8_t>(r[table+3]))*8};
        valid=bounds[0]<bounds[1] && bounds[2]<bounds[3] && shared.size()>=0x3bb20;
        for(unsigned other=0x3480;other<0x6c00;other+=128){
            if(actor(r,other))obstacles.push_back(body(r,other));
            else if((r[other]&128) && r[other+0xb]==4){
                // Original F353's solid record bounds, unlike pickups/player contacts.
                const auto left=static_cast<std::int16_t>(word(r,other+0x2a));
                const auto right=static_cast<std::int16_t>(word(r,other+0x2c));
                const auto bottom=static_cast<std::int16_t>(word(r,other+0x32));
                const auto top=static_cast<std::int16_t>(word(r,other+0x34));
                if(right>=left && top>=bottom){
                    const NavigationBody solid{other,{(left+right)/2.0,(bottom+top)/2.0},(right-left)/2.0,(top-bottom)/2.0};
                    obstacles.push_back(solid);solids.push_back(solid);
                }
            }
        }
    }
    NavigationPoint constrain_goal(NavigationPoint goal) const override {
        // Match the integer footprint checks, including the exclusive right
        // edge and F355's positive-offset screen probes. Use an integer anchor
        // so fixed-point rounding cannot submit an out-of-bounds next step.
        const double left=std::max(10,bounds[0]+10),right=std::min(363,bounds[1]-11);
        const double bottom=std::max(9,bounds[2]+9),top=std::min(477,bounds[3]-9);
        if(left>right || bottom>top)return goal;
        return {std::clamp(goal.x,left,right),std::clamp(goal.y,bottom,top)};
    }
    // The mover's footprint lies within its movement rectangle, and F355's
    // four attribute probes at (x,y), (x,y+18), (x+20,y+18), (x+20,y) all
    // fall on screen. Terrain content and scenery are judged separately.
    bool inside(NavigationPoint p) const {
        const int x=int(std::floor(p.x)),y=int(std::floor(p.y));
        return valid && x-10>=bounds[0] && x+10<bounds[1] && y-9>=bounds[2] && y+9<=bounds[3] &&
            x>=10 && x+20<=383 && y>=9 && y+18<=495;
    }
    bool walkable(NavigationPoint p) const override {
        if(!inside(p))return false;
        // F355's bounds are centered, but its four attribute probes begin at
        // (x,y), then add (0,18), (20,18), (20,0). F287 preserves those inputs.
        // Using centered corners here approves steps that F355 then rejects.
        for(int x:{int(std::floor(p.x)),int(std::floor(p.x))+20})
            for(int y:{int(std::floor(p.y)),int(std::floor(p.y))+18})
                if(shared[0x3af22+(x/8)*64+(495-y)/8]&9)return false;
        // Scenery never yields. Exclude its footprint from A* and line-of-sight
        // queries as well as local steps, using the same contact clearance.
        for(const auto &solid:solids)
            if(std::abs(p.x-solid.position.x)<10+solid.half_width+1 &&
               std::abs(p.y-solid.position.y)<9+solid.half_height+1)return false;
        return true;
    }
    std::span<const NavigationBody> bodies() const override{return obstacles;}
    std::span<const NavigationReservation> reservations() const override{return claims;}
    bool valid{};
private:
    std::span<const std::uint8_t> shared;
    std::array<int,4> bounds{};
    std::vector<NavigationBody> obstacles;
    std::vector<NavigationBody> solids;
    std::span<const NavigationReservation> claims;
};
}
void LegacyEnemyNavigation::reset(){for(auto &slot:slots_)slot=Slot{};}
void LegacyEnemyNavigation::prepare(std::span<std::uint8_t> r,std::span<const std::uint8_t> shared,
        std::uint32_t record,std::uint64_t frame){
    if(record<0x3400 || record>=0x7400 || (record&127))return;
    auto &slot=slots_[(record-0x3400)/128];
    if(!actor(r,record)){slot=Slot{};return;}
    if(slot.frame==frame)return;
    const auto callback=number(r,record+2),descriptor=number(r,record+0x6e);
    if(slot.callback!=callback || slot.descriptor!=descriptor || (slot.frame!=UINT64_MAX && frame>slot.frame+2))slot=Slot{};
    slot.callback=callback;slot.descriptor=descriptor;slot.frame=frame;slot.prepared=false;slot.presentation=false;
    const auto *definition=enemy_definition(callback);
    if(!actor(r,record) || !definition || definition->kind==EnemyKind::boss ||
        (!definition->calls.empty() && std::none_of(definition->calls.begin(),definition->calls.end(),[](const auto &call){return call.target==0x1e124;})) ||
        r[record+0x3e]!=0 ||
        (r[record+0x59]!=0 && r[record+0x59]!=0x1e) || number(r,record+0x22)!=0){slot.contact_blocked=false;slot.facing_initialized=false;slot.facing_samples=0;slot.state.waiting_assigned=false;return;}
    // Preserve a valid waiting claim during an attack, but never recover its
    // stop or substitute presentation/movement into the original attack path.
    if(r[record+0x41]&2){slot.contact_blocked=false;slot.facing_samples=0;return;}
    if(slot.contact_blocked && (frame<slot.contact_frame || frame-slot.contact_frame>2 ||
       !actor(r,slot.contact_other) || number(r,slot.contact_other+2)!=slot.contact_callback ||
       number(r,slot.contact_other+0x6e)!=slot.contact_descriptor))slot.contact_blocked=false;
    if((r[record+0x40]&2) && !slot.contact_blocked){slot.facing_samples=0;return;}
    if(!(r[record+0x40]&2))slot.contact_blocked=false;
    const NavigationPoint intended{fixed(r,record+0x1e),fixed(r,record+0x26)};
    const double speed=std::hypot(intended.x,intended.y);
    if(speed<0.00001 || speed>6){slot.facing_samples=0;return;} // No walking intent, or a scripted displacement.
    auto mover=body(r,record);
    NavigationPoint goal{double(static_cast<std::int16_t>(word(r,record+0x62))),double(static_cast<std::int16_t>(word(r,record+0x64)))};
    const unsigned target=r[record+0x60];
    if(target>3){slot.facing_samples=0;return;}
    if(target){
        if(!r[0x3409+target]){slot.facing_samples=0;return;}
        goal={double(static_cast<std::int16_t>(word(r,0x340c + target*4))),double(static_cast<std::int16_t>(word(r,0x340e + target*4)))};
    }
    if(goal.x<0 || goal.x>383 || goal.y<0 || goal.y>495){slot.facing_samples=0;return;}
    // Preserve the original patrol-arrival stop even if contact preceded it.
    // The step dispatcher accepts arrival within 10 pixels on each axis.
    if(!target && std::abs(goal.x-mover.position.x)<=10 && std::abs(goal.y-mover.position.y)<=10){slot.contact_blocked=false;slot.facing_samples=0;return;}
    std::vector<NavigationReservation> claims;
    for(unsigned i=0;i<slots_.size();++i){
        const unsigned other=0x3400+i*128;const auto &owner=slots_[i];
        if(other!=record && owner.state.waiting_assigned && owner.frame<=frame && frame-owner.frame<=2 &&
           actor(r,other) && number(r,other+2)==owner.callback && number(r,other+0x6e)==owner.descriptor)
            claims.push_back({other,owner.state.waiting_position});
    }
    World world(r,shared,record,claims);if(!world.valid){slot.facing_samples=0;return;}
    // An actor entering from outside the screen or its movement rectangle has
    // no legal navigation step until its footprint is inside. Leave the
    // original walking intent for F355/F357, which admit it as before.
    if(!world.inside(mover.position)){slot.facing_samples=0;log_actor("outside-rectangle",r,record,frame,goal.x,goal.y);return;}
    // A scripted route point outside the permitted rectangle is not a player
    // to wait for at the edge. Waiting there would zero the intent forever and
    // the original step dispatcher would never see arrival. Leave it alone.
    if(!target && !world.inside(goal)){slot.facing_samples=0;slot.state.waiting_assigned=false;log_actor("edge-route-point",r,record,frame,goal.x,goal.y);return;}
    const auto delta=EnemyNavigation{}.steer(slot.state,world,mover,goal,intended);
    // Preserve intent across a one-pass wait; animation follows committed
    // displacement, not the retained speed used to resume on the next update.
    if(delta.x==0 && delta.y==0){
        slot.facing_samples=0;
        log_actor(slot.state.waiting ? "boundary-wait":"no-legal-step",r,record,frame,goal.x,goal.y,
                  slot.state.route.empty() ? " route 0":" route >0");
        {
            // Suppress only this movement pass. Restore intent after F357 so
            // waiting does not turn into a permanent zero-speed enemy.
            slot.held_intent=intended;slot.restore_intent=true;slot.prepared=true;
            put(r,record+0x1e,0);put(r,record+0x26,0);
        }
        if(slot.contact_blocked && std::abs(mover.position.x-body(r,slot.contact_other).position.x)<=20 &&
           std::abs(mover.position.y-body(r,slot.contact_other).position.y)<=18)slot.contact_frame=frame;
        return;
    }
    // Retry only a block observed coming from NPC contact, and only after a
    // new swept terrain/contact check found a legal step. Other stop owners
    // (terrain, arrival, hit/death, scripts) must never be cleared here.
    if(slot.contact_blocked){r[record+0x40]&=static_cast<std::uint8_t>(~2U);slot.contact_blocked=false;}
    if(slot.state.boundary_goal && std::hypot(delta.x,delta.y)<speed-0.00001){
        slot.held_intent=intended;slot.restore_intent=true;
    }
    put(r,record+0x1e,delta.x);put(r,record+0x26,delta.y);slot.prepared=true;
}
void LegacyEnemyNavigation::record_contact_block(std::span<const std::uint8_t> r,std::uint32_t record,
        std::uint32_t other,std::uint64_t frame){
    if(!actor(r,record) || !actor(r,other) || record==other || !(r[record+0x40]&2) || (r[record+0x41]&2))return;
    auto &slot=slots_[(record-0x3400)/128];
    if(slot.frame==frame){
        slot.contact_blocked=true;slot.contact_frame=frame;slot.contact_other=other;
        slot.contact_callback=number(r,other+2);slot.contact_descriptor=number(r,other+0x6e);
    }
}
void LegacyEnemyNavigation::finish_move(std::span<std::uint8_t> r,std::uint32_t record,std::uint64_t frame){
    if(!actor(r,record))return;
    auto &slot=slots_[(record-0x3400)/128];
    if(!slot.prepared || slot.frame!=frame)return;
    slot.prepared=false;
    if(slot.restore_intent){
        put(r,record+0x1e,slot.held_intent.x);put(r,record+0x26,slot.held_intent.y);
        slot.restore_intent=false;
    }
    const double dx=fixed(r,record+0x12)-slot.state.previous.x;
    const double dy=fixed(r,record+0x1a)-slot.state.previous.y;
    slot.presentation=true;slot.moving=std::hypot(dx,dy)>=0.00001;
    if(!slot.moving || std::hypot(dx,dy)>6){slot.facing_samples=0;return;}
    // F306's vector table uses 0=east, 0x200=+Y, with a 0x800 turn.
    // F596 selects the walking sprite from this heading; bit 3 requests reload.
    const auto heading=static_cast<unsigned>(static_cast<int>(std::lround(std::atan2(dy,dx)*2048/6.283185307179586)))&0x7ffU;
    const auto old=slot.facing_initialized ? slot.facing:word(r,record+0x5c);
    if(!slot.facing_initialized){slot.facing=heading;slot.facing_initialized=true;slot.facing_samples=0;}
    else{
        const auto current_sector=(slot.facing+0x80)&0x700;
        const auto next_sector=(heading+0x80)&0x700;
        const auto difference=static_cast<int>((heading-current_sector+0x400)&0x7ff)-0x400;
        // Retain the current sprite through an extra 5.625 degrees at its
        // boundary. Brief avoidance reversals must persist for three moves.
        if(next_sector==current_sector){slot.facing=heading;slot.facing_samples=0;}
        else if(std::abs(difference)<=0xa0)slot.facing_samples=0;
        else{
            if(slot.candidate_facing!=next_sector){slot.candidate_facing=next_sector;slot.facing_samples=0;}
            if(++slot.facing_samples>=3){slot.facing=heading;slot.facing_samples=0;}
        }
    }
    if(((old+0x80)&0x700)!=((slot.facing+0x80)&0x700))r[record+0x40]|=8;
}
std::uint16_t LegacyEnemyNavigation::walking_heading(std::span<const std::uint8_t> r,
        std::uint32_t record,std::uint64_t frame,std::uint16_t original) const {
    if(!actor(r,record) || (r[record+0x41]&2))return original;
    const auto &slot=slots_[(record-0x3400)/128];
    return slot.presentation && slot.facing_initialized && slot.frame==frame ? slot.facing:original;
}
std::uint16_t LegacyEnemyNavigation::walking_advance(std::span<const std::uint8_t> r,
        std::uint32_t record,std::uint64_t frame,std::uint16_t original) const {
    if(!actor(r,record) || (r[record+0x41]&2))return original;
    const auto &slot=slots_[(record-0x3400)/128];
    return slot.presentation && !slot.moving && slot.frame==frame ? 0:original;
}
bool LegacyEnemyNavigation::separating(std::span<const std::uint8_t> r,std::uint32_t record,
        std::uint32_t other,std::uint64_t frame) const {
    if(!actor(r,record) || !actor(r,other) || record==other)return false;
    const auto &slot=slots_[(record-0x3400)/128];
    if(!slot.prepared || slot.frame!=frame)return false;
    return EnemyNavigation::separates(body(r,record),{fixed(r,record+0x1e),fixed(r,record+0x26)},body(r,other));
}
} // namespace gain_ground::gameplay
