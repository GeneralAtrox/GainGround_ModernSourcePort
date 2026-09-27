#include "gain_ground/gameplay/enemy_navigation.h"
#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_devices.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace gain_ground::gameplay;
namespace {
void check(bool ok,const char *message){if(!ok)throw std::runtime_error(message);}
struct Rect {double l,r,b,t;};
struct World : NavigationWorld {
    std::vector<Rect> walls;
    std::vector<NavigationBody> actors;
    std::vector<NavigationReservation> claims;
    bool walkable(NavigationPoint p) const override {
        if(p.x<10||p.x>374||p.y<9||p.y>487)return false;
        for(auto r:walls)if(p.x+10>=r.l && p.x-10<=r.r && p.y+9>=r.b && p.y-9<=r.t)return false;
        return true;
    }
    std::span<const NavigationBody> bodies() const override{return actors;}
    std::span<const NavigationReservation> reservations() const override{return claims;}
};
double distance(NavigationPoint a,NavigationPoint b){return std::hypot(a.x-b.x,a.y-b.y);}
void simulate(World &world,std::span<const NavigationPoint> goals,unsigned ticks){
    std::vector<NavigationState> states(goals.size());EnemyNavigation navigation;
    for(unsigned tick=0;tick<ticks;++tick)for(unsigned i=0;i<goals.size();++i){
        auto &actor=world.actors[i];auto p=actor.position;const auto d=distance(p,goals[i]);if(d<2)continue;
        const NavigationPoint intent{(goals[i].x-p.x)/d,(goals[i].y-p.y)/d};
        const auto step=navigation.steer(states[i],world,actor,goals[i],intent);
        check(std::hypot(step.x,step.y)<=1.000001,"Navigation increased speed");
        actor.position={p.x+step.x,p.y+step.y};check(world.walkable(actor.position),"Crossed terrain");
        for(unsigned j=0;j<world.actors.size();++j)if(i!=j){
            const auto &other=world.actors[j];
            check(std::abs(actor.position.x-other.position.x)>=20 || std::abs(actor.position.y-other.position.y)>=18,"NPC overlap introduced");
        }
    }
    for(unsigned i=0;i<goals.size();++i){
        if(distance(world.actors[i].position,goals[i])>=3)std::cerr<<"actor "<<i<<" at "<<world.actors[i].position.x<<','<<world.actors[i].position.y<<'\n';
        check(distance(world.actors[i].position,goals[i])<3,"NPC failed to reach goal");
    }
}
void put(std::vector<std::uint8_t>&r,unsigned a,unsigned n,unsigned bytes){for(unsigned i=0;i<bytes;++i)r[a+i]=static_cast<std::uint8_t>(n>>((bytes-i-1)*8));}
void actor(std::vector<std::uint8_t>&r,unsigned a,int x,int y){
    r[a]=0x80;r[a+0xa]=static_cast<std::uint8_t>((a-0x3400)/128);r[a+0xb]=8;
    put(r,a+2,0x20918,4);put(r,a+0x12,x*65536,4);put(r,a+0x1a,y*65536,4);
    put(r,a+0x1e,65536,4);put(r,a+0x6e,0x20000,4);put(r,a+0x62,200,2);put(r,a+0x64,y,2);
}
}
int main(){try{
    { // The NPC-only enclosure reproduced in the review has an open left exit.
        World w;w.actors={{1,{120,120}}};unsigned id=2;
        for(double y:{80,100,120,140,160})w.actors.push_back({id++,{160,y}});
        for(double x:{100,120,140}){w.actors.push_back({id++,{x,80}});w.actors.push_back({id++,{x,160}});}
        NavigationState state;unsigned routed=0;
        for(unsigned tick=0;tick<1000;++tick){
            auto &b=w.actors[0];const auto d=distance(b.position,{220,120});if(d<2)break;
            const auto step=EnemyNavigation{}.steer(state,w,b,{220,120},{(220-b.position.x)/d,(120-b.position.y)/d});
            routed+=!state.route.empty();b.position.x+=step.x;b.position.y+=step.y;
            for(unsigned j=1;j<w.actors.size();++j)check(std::abs(b.position.x-w.actors[j].position.x)>=20 || std::abs(b.position.y-w.actors[j].position.y)>=18,"Enclosure escape crossed an NPC");
        }
        if(distance(w.actors[0].position,{220,120})>=3)std::cerr<<"enclosure at "<<w.actors[0].position.x<<','<<w.actors[0].position.y<<'\n';
        check(routed && distance(w.actors[0].position,{220,120})<3,"NPC enclosure never escaped");
    }
    for(bool reverse:{false,true}){
        struct EdgeWorld:World{
            NavigationPoint constrain_goal(NavigationPoint p)const override{return {std::clamp(p.x,10.0,363.0),std::clamp(p.y,200.0,477.0)};}
            bool walkable(NavigationPoint p)const override{return p.y>=200 && World::walkable(p);}
        }w;
        w.actors={{1,{100,200}},{2,{122,200}},{3,{144,200}},{4,{166,200}}};
        std::array<NavigationState,4> states;std::array<double,4> travel{};
        for(unsigned tick=0;tick<1000;++tick)for(unsigned index=0;index<4;++index){
            const auto i=reverse ? 3-index:index;w.claims.clear();
            for(unsigned j=0;j<4;++j)if(states[j].waiting_assigned)w.claims.push_back({w.actors[j].id,states[j].waiting_position});
            auto &b=w.actors[i];const auto d=distance(b.position,{100,100});
            const auto step=EnemyNavigation{}.steer(states[i],w,b,{100,100},{(100-b.position.x)/d,(100-b.position.y)/d});
            b.position.x+=step.x;b.position.y+=step.y;
            if(tick>=900)travel[i]+=std::hypot(step.x,step.y);
            check(w.walkable(b.position),"Boundary crowd crossed terrain");
            for(unsigned j=0;j<4;++j)if(i!=j)check(std::abs(b.position.x-w.actors[j].position.x)>=20 || std::abs(b.position.y-w.actors[j].position.y)>=18,"Boundary crowd introduced overlap");
        }
        for(unsigned i=0;i<4;++i){
            if(travel[i]>0.01)std::cerr<<"boundary actor "<<i<<" pos "<<w.actors[i].position.x<<','<<w.actors[i].position.y<<" travel "<<travel[i]<<" goal "<<states[i].goal.x<<','<<states[i].goal.y<<" route "<<states[i].route.size()<<'\n';
        }
        for(unsigned i=0;i<4;++i)check(states[i].waiting && travel[i]<0.01,"Boundary crowd never settled");
    }
    { // Every edge and a corner: no oscillation, overshoot, or permanent wait.
        struct BoundedWorld : World {
            NavigationPoint constrain_goal(NavigationPoint p) const override {
                return {std::clamp(p.x,40.0,160.0),std::clamp(p.y,40.0,160.0)};
            }
            bool walkable(NavigationPoint p) const override {
                return p.x>=40 && p.x<=160 && p.y>=40 && p.y<=160 && World::walkable(p);
            }
        };
        for(NavigationPoint goal: {NavigationPoint{200,100},{0,100},{100,0},{100,200},{200,200}}){
            BoundedWorld world;world.actors={{1,{100,100}}};NavigationState state;
            const auto edge=world.constrain_goal(goal);
            for(unsigned tick=0;tick<200;++tick){
                auto &body=world.actors[0];const auto d=distance(body.position,goal);
                const auto step=EnemyNavigation{}.steer(state,world,body,goal,{1.3*(goal.x-body.position.x)/d,1.3*(goal.y-body.position.y)/d});
                body.position.x+=step.x;body.position.y+=step.y;
                check(world.walkable(body.position),"Boundary step left permitted area");
                if(tick>100)check(distance(body.position,edge)<=0.25 && std::hypot(step.x,step.y)==0,"Boundary arrival kept bouncing");
            }
            auto &body=world.actors[0];const auto before=distance(body.position,{100,100});
            const auto step=EnemyNavigation{}.steer(state,world,body,{100,100},{(100-body.position.x)/before,(100-body.position.y)/before});
            check(distance({body.position.x+step.x,body.position.y+step.y},{100,100})<before,"Boundary hold prevented resumed pursuit");
        }
        BoundedWorld world;world.actors={{1,{160,100}},{2,{145,100}}};NavigationState state;
        const auto delta=EnemyNavigation{}.steer(state,world,world.actors[0],{200,100},{1,0});
        check(EnemyNavigation::separates(world.actors[0],delta,world.actors[1]),"Boundary hold prevented required NPC separation");
    }
    { // Pursuit must settle at a descriptor boundary, through the real gates.
        gain_ground::RuntimeHost host;gain_ground::System24Devices devices;host.attach_devices(devices);host.select_cpu(1);
        std::vector<std::uint8_t> ram(0x40000),shared(0x40000);actor(ram,0x3500,140,100);
        put(ram,0x351e,111411,4); // 1.7 pixels/update: arrival needs a shortened final step.
        ram[0x32ded]=20;ram[0x32def]=62;ram[0x355e]=7;ram[0x3560]=1;ram[0x340a]=1;
        put(ram,0x3410,200,2);put(ram,0x3412,100,2);
        put(ram,0x6c00,0x8000,2);put(ram,0x7ff0,0x20934,4);
        host.load_region(2,0,ram);host.load_region(3,0,shared);
        gain_ground::FunctionContext c{};c.host=&host;c.cpu=1;c.state=0x72;c.registers.address[5]=0x3500;c.registers.status=0x2700;
        const auto fixed=[&](unsigned a){const auto r=host.region_bytes(2);std::uint32_t n=0;for(unsigned i=0;i<4;++i)n=(n<<8)|r[a+i];return static_cast<std::int32_t>(n)/65536.0;};
        const auto tick=[&](unsigned frame){
            devices.advance(frame*17500000ULL);
            for(auto [id,pc]:{std::pair{353U,0x1dbf0U},std::pair{355U,0x1de30U},std::pair{357U,0x1e124U}}){
                c.registers.address[7]=0x7ff0;c.registers.program_counter=pc;
                auto result=host.call_function(id,1,0x72,2,0x20930,pc,c);
                check(!host.faulted() && result.control==1,"Boundary dispatch failed");
                check(!(host.region_bytes(2)[0x3540]&2),"Boundary steering submitted a rejected step");
            }
        };
        for(unsigned frame=1;frame<=100;++frame){
            tick(frame);
            check(fixed(0x3512)<=149.0001,"Enemy left its descriptor boundary");
            if(frame>=20)check(std::hypot(fixed(0x3512)-149,fixed(0x351a)-100)<0.3,"Enemy bounced instead of holding the boundary");
            if(frame>=20)check(host.enemy_walking_advance(0x3500,96)==0,"Boundary wait kept walking animation active");
        }
        check(std::hypot(fixed(0x351e),fixed(0x3526))>1.69,"Boundary wait erased or slowed walking intent");
        host.write_memory_word(2,0x3410,100,0xffff);
        for(unsigned frame=101;frame<=115;++frame)tick(frame);
        check(fixed(0x3512)<140,"Enemy failed to resume pursuit when player returned inside");
    }
    for(double side:{1.0,-1.0}){ // Include the direction-table wraparound edge.
        std::vector<std::uint8_t> ram(0x40000),shared(0x40000);actor(ram,0x3500,200,200);
        ram[0x32ded]=48;ram[0x32def]=62;put(ram,0x3562,320,2);
        const auto fixed=[&](unsigned a){std::uint32_t n=0;for(unsigned i=0;i<4;++i)n=(n<<8)|ram[a+i];return static_cast<std::int32_t>(n)/65536.0;};
        LegacyEnemyNavigation adapter;unsigned changes=0;
        const auto step=[&](unsigned frame,double degrees){
            const double angle=degrees*3.141592653589793/180;
            put(ram,0x351e,static_cast<std::uint32_t>(static_cast<std::int32_t>(std::cos(angle)*65536)),4);
            put(ram,0x3526,static_cast<std::uint32_t>(static_cast<std::int32_t>(std::sin(angle)*65536)),4);
            ram[0x3540]&=static_cast<std::uint8_t>(~8U);adapter.prepare(ram,shared,0x3500,frame);
            const auto dx=fixed(0x351e),dy=fixed(0x3526);
            put(ram,0x3512,static_cast<unsigned>((fixed(0x3512)+dx)*65536),4);
            put(ram,0x351a,static_cast<unsigned>((fixed(0x351a)+dy)*65536),4);
            adapter.finish_move(ram,0x3500,frame);
            check(ram[0x355c]==0 && ram[0x355d]==0,"Walking appearance changed original aim");
            check(std::abs(fixed(0x351e)-dx)<0.00002 && std::abs(fixed(0x3526)-dy)<0.00002,"Facing filter changed movement");
            if(ram[0x3540]&8)++changes;
        };
        for(unsigned frame=1;frame<=20;++frame)step(frame,side*(frame%2 ? 21:24));
        check(changes<=1,"Walking facing flickers at direction boundary");
        const auto steady=changes;
        for(unsigned frame=21;frame<=40;++frame)step(frame,side*(frame%2 ? 100:21));
        check(changes==steady,"Single-update sidesteps cause facing flicker");
        for(unsigned frame=41;frame<=43;++frame)step(frame,180);
        const auto heading=adapter.walking_heading(ram,0x3500,43,0);
        check(((heading+0x80)&0x700)==0x400,"Sustained reversal never updated facing");
    }
    { // Live reservation leases, waiting animation, and same-type slot reuse.
        gain_ground::RuntimeHost host;gain_ground::System24Devices devices;host.attach_devices(devices);host.select_cpu(1);
        std::vector<std::uint8_t> ram(0x40000),shared(0x40000);
        const std::array<unsigned,4> records{0x3500,0x3580,0x3600,0x3680};
        for(unsigned i=0;i<4;++i){actor(ram,records[i],100+i*22,201);ram[records[i]+0x60]=1;}
        ram[0x340a]=1;put(ram,0x3410,100,2);put(ram,0x3412,100,2);
        ram[0x32ded]=48;ram[0x32dee]=24;ram[0x32def]=62;put(ram,0x6c00,0x8000,2);put(ram,0x7ff0,0x20934,4);
        host.load_region(2,0,ram);host.load_region(3,0,shared);
        gain_ground::FunctionContext c{};c.host=&host;c.cpu=1;c.state=0x72;c.registers.status=0x2700;
        const auto tick=[&](unsigned frame){devices.advance(frame*17500000ULL);for(auto a:records){
            if(!(host.region_bytes(2)[a]&128))continue;c.registers.address[5]=a;
            for(auto [id,pc]:{std::pair{353U,0x1dbf0U},std::pair{355U,0x1de30U},std::pair{357U,0x1e124U}}){
                c.registers.address[7]=0x7ff0;c.registers.program_counter=pc;
                const auto result=host.call_function(id,1,0x72,2,0x20930,pc,c);
                check(!host.faulted() && result.control==1,"Live crowd callback failed");
            }
        }};
        for(unsigned frame=1;frame<=800;++frame)tick(frame);
        for(auto a:records)check(host.enemy_walking_advance(a,96)==0,"Live boundary crowd did not stop animating");
        host.write_memory_word(2,0x3500,0,0xffff);
        for(unsigned frame=801;frame<=805;++frame)tick(frame);
        actor(ram,0x3500,210,260);ram[0x3560]=1;host.load_region(2,0x3500,std::span(ram).subspan(0x3500,128));
        for(unsigned frame=806;frame<=1400;++frame)tick(frame);
        for(auto a:records)if(host.enemy_walking_advance(a,96)!=0){const auto r=host.region_bytes(2);
            std::cerr<<"reused actor "<<std::hex<<a<<std::dec<<" pos "<<((r[a+0x12]<<8)|r[a+0x13])<<','<<((r[a+0x1a]<<8)|r[a+0x1b])<<" flags "<<unsigned(r[a+0x40])<<'\n';}
        for(auto a:records)check(host.enemy_walking_advance(a,96)==0,"Reused record retained a stale waiting claim");
    }
    { // The route must be accepted by the actual F355 terrain probes.
        gain_ground::RuntimeHost host;gain_ground::System24Devices devices;host.attach_devices(devices);host.select_cpu(1);
        std::vector<std::uint8_t> ram(0x40000),shared(0x40000);actor(ram,0x3500,60,100);
        ram[0x32ded]=48;ram[0x32def]=62;ram[0x355e]=7;
        put(ram,0x6c00,0x8000,2);put(ram,0x7ff0,0x20934,4);
        for(unsigned x=112;x<152;x+=8)for(unsigned y=80;y<160;y+=8)shared[0x3af22+(x/8)*64+(495-y)/8]=1;
        host.load_region(2,0,ram);host.load_region(3,0,shared);
        gain_ground::FunctionContext c{};c.host=&host;c.cpu=1;c.state=0x72;c.registers.address[5]=0x3500;c.registers.status=0x2700;
        const auto fixed=[&](unsigned a){const auto r=host.region_bytes(2);std::uint32_t n=0;for(unsigned i=0;i<4;++i)n=(n<<8)|r[a+i];return static_cast<std::int32_t>(n)/65536.0;};
        for(unsigned frame=1;frame<=700;++frame){
            if(std::hypot(fixed(0x3512)-200,fixed(0x351a)-100)<9)break;
            devices.advance(frame*17500000ULL);
            for(auto [id,pc]:{std::pair{353U,0x1dbf0U},std::pair{355U,0x1de30U},std::pair{357U,0x1e124U}}){
                c.registers.address[7]=0x7ff0;c.registers.program_counter=pc;
                auto result=host.call_function(id,1,0x72,2,0x20930,pc,c);
                check(!host.faulted() && result.control==1,"Terrain-route dispatch failed");
                check(!(host.region_bytes(2)[0x3540]&2),"Route selected a step rejected by original terrain probes");
            }
        }
        check(std::hypot(fixed(0x3512)-200,fixed(0x351a)-100)<9,"Terrain route did not reach destination");
    }
    { // Solid type-4 scenery must participate in routing, not just contact avoidance.
        std::vector<std::uint8_t> ram(0x40000),shared(0x40000);actor(ram,0x3500,60,100);
        ram[0x32ded]=48;ram[0x32def]=62;
        ram[0x3580]=0x80;ram[0x358b]=4;
        put(ram,0x35aa,110,2);put(ram,0x35ac,150,2);put(ram,0x35b2,80,2);put(ram,0x35b4,160,2);
        const auto fixed=[&](unsigned a){std::uint32_t n=0;for(unsigned i=0;i<4;++i)n=(n<<8)|ram[a+i];return static_cast<std::int32_t>(n)/65536.0;};
        LegacyEnemyNavigation adapter;
        for(unsigned frame=1;frame<=700;++frame){
            const auto x=fixed(0x3512),y=fixed(0x351a);if(std::hypot(x-200,y-100)<9)break;
            put(ram,0x351e,65536,4);put(ram,0x3526,0,4);
            adapter.prepare(ram,shared,0x3500,frame);
            const auto dx=fixed(0x351e),dy=fixed(0x3526);
            check(std::hypot(dx,dy)<=1.00001,"Rock routing increased speed");
            const auto nx=x+dx,ny=y+dy;
            check(nx+10<110 || nx-10>150 || ny+9<80 || ny-9>160,"Rock routing crossed solid scenery");
            put(ram,0x3512,static_cast<unsigned>(nx*65536),4);put(ram,0x351a,static_cast<unsigned>(ny*65536),4);
        }
        if(std::hypot(fixed(0x3512)-200,fixed(0x351a)-100)>=9)std::cerr<<"rock actor at "<<fixed(0x3512)<<','<<fixed(0x351a)<<'\n';
        check(std::hypot(fixed(0x3512)-200,fixed(0x351a)-100)<9,"NPC remained stuck at solid scenery");
    }
    { // Entering from off screen or outside the movement rectangle keeps original intent.
        for(auto [x,y,dx,dy]:{std::array<int,4>{-40,100,1,0},{200,530,0,-1},{420,300,-1,0},{100,-30,0,1},{30,300,1,0}}){
            std::vector<std::uint8_t> ram(0x40000),shared(0x40000);actor(ram,0x3500,x,y);
            ram[0x32dec]=static_cast<std::uint8_t>(x==30 ? 8:0);ram[0x32ded]=48;ram[0x32def]=62;
            put(ram,0x3562,200,2);put(ram,0x3564,250,2);
            const auto fixed=[&](unsigned a){std::uint32_t n=0;for(unsigned i=0;i<4;++i)n=(n<<8)|ram[a+i];return static_cast<std::int32_t>(n)/65536.0;};
            LegacyEnemyNavigation adapter;bool entered=false;
            const int left=x==30 ? 64:0;
            for(unsigned frame=1;frame<=120 && !entered;++frame){
                put(ram,0x351e,static_cast<std::uint32_t>(dx*65536),4);put(ram,0x3526,static_cast<std::uint32_t>(dy*65536),4);
                adapter.prepare(ram,shared,0x3500,frame);
                const auto sx=fixed(0x351e),sy=fixed(0x3526);
                check(std::hypot(sx,sy)>0.99,"Off-screen entry intent was erased");
                const auto nx=fixed(0x3512)+sx,ny=fixed(0x351a)+sy;
                put(ram,0x3512,static_cast<std::uint32_t>(static_cast<std::int32_t>(nx*65536)),4);
                put(ram,0x351a,static_cast<std::uint32_t>(static_cast<std::int32_t>(ny*65536)),4);
                adapter.finish_move(ram,0x3500,frame);
                const int fx=int(std::floor(nx)),fy=int(std::floor(ny));
                entered=fx-10>=left && fx>=10 && fx<=363 && fy>=9 && fy<=477;
            }
            if(!entered)std::cerr<<"entry actor from "<<x<<','<<y<<" at "<<fixed(0x3512)<<','<<fixed(0x351a)<<'\n';
            check(entered,"Off-screen enemy never entered the level");
        }
    }
    { // A scripted route point at the screen edge is reached by the original step, not waited for.
        for(auto [gx,gy,dx,dy]:{std::array<int,4>{200,490,0,1},{378,300,1,0},{200,3,0,-1},{4,300,-1,0}}){
            std::vector<std::uint8_t> ram(0x40000),shared(0x40000);actor(ram,0x3500,200,300);
            ram[0x32ded]=48;ram[0x32def]=62;put(ram,0x3562,static_cast<unsigned>(gx)&0xffff,2);put(ram,0x3564,static_cast<unsigned>(gy)&0xffff,2);
            const auto fixed=[&](unsigned a){std::uint32_t n=0;for(unsigned i=0;i<4;++i)n=(n<<8)|ram[a+i];return static_cast<std::int32_t>(n)/65536.0;};
            LegacyEnemyNavigation adapter;bool arrived=false;
            for(unsigned frame=1;frame<=400 && !arrived;++frame){
                put(ram,0x351e,static_cast<std::uint32_t>(dx*65536),4);put(ram,0x3526,static_cast<std::uint32_t>(dy*65536),4);
                adapter.prepare(ram,shared,0x3500,frame);
                const auto sx=fixed(0x351e),sy=fixed(0x3526);
                check(std::hypot(sx,sy)>0.99,"Edge route point put the runner into a boundary wait");
                const auto nx=fixed(0x3512)+sx,ny=fixed(0x351a)+sy;
                put(ram,0x3512,static_cast<std::uint32_t>(static_cast<std::int32_t>(nx*65536)),4);
                put(ram,0x351a,static_cast<std::uint32_t>(static_cast<std::int32_t>(ny*65536)),4);
                adapter.finish_move(ram,0x3500,frame);
                arrived=std::abs(nx-gx)<=10 && std::abs(ny-gy)<=10;
            }
            check(arrived,"Runner never reached its edge route point");
        }
    }
    { // Through the real gates: a descriptor rectangle reaching off screen admits the walk in.
        gain_ground::RuntimeHost host;gain_ground::System24Devices devices;host.attach_devices(devices);host.select_cpu(1);
        std::vector<std::uint8_t> ram(0x40000),shared(0x40000);actor(ram,0x3500,-40,100);
        ram[0x32dec]=0xf0;ram[0x32ded]=64;ram[0x32dee]=0xf8;ram[0x32def]=80;put(ram,0x3564,100,2);
        put(ram,0x6c00,0x8000,2);put(ram,0x7ff0,0x20934,4);
        host.load_region(2,0,ram);host.load_region(3,0,shared);
        gain_ground::FunctionContext c{};c.host=&host;c.cpu=1;c.state=0x72;c.registers.address[5]=0x3500;c.registers.status=0x2700;
        const auto fixed=[&](unsigned a){const auto r=host.region_bytes(2);std::uint32_t n=0;for(unsigned i=0;i<4;++i)n=(n<<8)|r[a+i];return static_cast<std::int32_t>(n)/65536.0;};
        for(unsigned frame=1;frame<=120;++frame){
            devices.advance(frame*17500000ULL);
            host.write_memory_word(2,0x351e,1,0xffff);host.write_memory_word(2,0x3520,0,0xffff);
            host.write_memory_word(2,0x3526,0,0xffff);host.write_memory_word(2,0x3528,0,0xffff);
            for(auto [id,pc]:{std::pair{353U,0x1dbf0U},std::pair{355U,0x1de30U},std::pair{357U,0x1e124U}}){
                c.registers.address[7]=0x7ff0;c.registers.program_counter=pc;
                auto result=host.call_function(id,1,0x72,2,0x20930,pc,c);
                check(!host.faulted() && result.control==1,"Off-screen entry dispatch failed");
            }
            check(!(host.region_bytes(2)[0x3540]&2),"Off-screen entry was stopped by the movement gate");
        }
        if(fixed(0x3512)<70)std::cerr<<"entry actor at "<<fixed(0x3512)<<','<<fixed(0x351a)<<'\n';
        check(fixed(0x3512)>=70,"Off-screen enemy did not walk onto the field through the original gates");
    }
    {World w;w.walls={{110,150,20,170}};w.actors={{1,{70,100}}};const NavigationPoint goals[]{{200,100}};simulate(w,goals,650);}
    {World w;w.actors={{1,{80,100}},{2,{180,100}}};const NavigationPoint goals[]{{220,100},{40,100}};simulate(w,goals,600);}
    {World w;w.walls={{80,150,0,80},{80,150,120,496}};w.actors={{1,{60,100}},{2,{170,100}}};const NavigationPoint goals[]{{220,100},{30,100}};simulate(w,goals,1000);}
    {World w;w.walls={{120,170,80,160}};w.actors={{1,{70,90}},{2,{70,120}},{3,{70,150}}};const NavigationPoint goals[]{{230,90},{230,120},{230,150}};simulate(w,goals,1000);}
    { // A compact triangle pursues one target, rather than three separate goals.
        World w;w.actors={{1,{100,120}},{2,{115,100}},{3,{134,112}}};
        std::array<NavigationState,3> states;const NavigationPoint goal{220,220};
        for(unsigned tick=0;tick<500;++tick)for(unsigned i=0;i<3;++i){
            auto &actor=w.actors[i];const auto p=actor.position;const auto d=distance(p,goal);if(d<24)continue;
            const auto step=EnemyNavigation{}.steer(states[i],w,actor,goal,{(goal.x-p.x)/d,(goal.y-p.y)/d});
            actor.position={p.x+step.x,p.y+step.y};
        }
        for(const auto &actor:w.actors){
            if(distance(actor.position,goal)>=45)std::cerr<<"cluster actor "<<actor.id<<" at "<<actor.position.x<<','<<actor.position.y<<'\n';
            check(distance(actor.position,goal)<45,"Three-NPC pursuit cluster stayed stuck");
        }
    }
    {World w;w.actors={{1,{80,100}},{2,{98,100}}};NavigationState s;
        const auto delta=EnemyNavigation{}.steer(s,w,w.actors[0],{150,100},{1,0});
        check(EnemyNavigation::separates(w.actors[0],delta,w.actors[1]),"Overlapping NPC failed to separate");}
    {World w;w.walls={{100,120,0,496}};w.actors={{1,{80,100}}};NavigationState s;
        for(int i=0;i<100;++i){auto d=EnemyNavigation{}.steer(s,w,w.actors[0],{180,100},{1,0});
            w.actors[0].position.x+=d.x;w.actors[0].position.y+=d.y;check(w.actors[0].position.x<90,"Crossed unreachable wall");}}
    {std::vector<std::uint8_t> ram(0x40000),shared(0x40000);actor(ram,0x3500,60,100);actor(ram,0x3580,78,100);
        ram[0x32ded]=48;ram[0x32def]=62;
        LegacyEnemyNavigation adapter;adapter.prepare(ram,shared,0x3500,1);
        check(adapter.separating(ram,0x3500,0x3580,1),"Live adapter did not enable separation");
        check(!adapter.separating(ram,0x3500,0x3580,2),"Stale separation escaped frame");
        for(unsigned gate:{0x3eU,0x3fU,0x59U}){auto stopped=ram;stopped[0x3500+gate]=1;auto before=stopped;adapter.reset();adapter.prepare(stopped,shared,0x3500,3);check(stopped==before,"Non-walking state changed");}
        auto stationary=ram;put(stationary,0x351e,0,4);put(stationary,0x3526,0,4);auto before=stationary;adapter.reset();adapter.prepare(stationary,shared,0x3500,4);check(stationary==before,"Stationary actor moved");
        for(unsigned kind:{0U,1U}){auto excluded=ram;if(kind==0)put(excluded,0x3502,0x184ea,4);else put(excluded,0x3522,65536,4);
            auto saved=excluded;adapter.reset();adapter.prepare(excluded,shared,0x3500,5);check(excluded==saved,"Boss or airborne motion changed");}
        {auto blocked=ram;blocked[0x3540]|=2;auto saved=blocked;adapter.reset();adapter.prepare(blocked,shared,0x3500,6);check(blocked==saved,"Unowned terrain/script stop was cleared");}
        for(unsigned gate:{0x3eU,0x3fU,0x59U}){
            auto blocked=ram;adapter.reset();adapter.prepare(blocked,shared,0x3500,7);blocked[0x3540]|=2;
            adapter.record_contact_block(blocked,0x3500,0x3580,7);blocked[0x3500+gate]=1;
            auto saved=blocked;adapter.prepare(blocked,shared,0x3500,8);check(blocked==saved,"Contact recovery overrode non-walking state");
            blocked[0x3500+gate]=0;saved=blocked;adapter.prepare(blocked,shared,0x3500,9);check(blocked==saved,"Old contact ownership survived a behavior change");
        }
        adapter.reset();check(!adapter.separating(ram,0x3500,0x3580,1),"Stage reset retained navigation");
    }
    { // Real F353 -> F354 -> RTS, followed by the original terrain and move gates.
        gain_ground::RuntimeHost host;gain_ground::System24Devices devices;host.attach_devices(devices);host.select_cpu(1);
        std::vector<std::uint8_t> ram(0x40000),shared(0x40000);actor(ram,0x3500,60,100);actor(ram,0x3580,78,100);
        ram[0x32ded]=48;ram[0x32def]=62;put(ram,0x6c00,0x8000,2);put(ram,0x6c02+(100-32)*2,0x3580,2);put(ram,0x7ff0,0x20924,4);
        host.load_region(2,0,ram);host.load_region(3,0,shared);
        gain_ground::FunctionContext c{};c.host=&host;c.cpu=1;c.state=0x72;c.registers.address[5]=0x3500;c.registers.address[7]=0x7ff0;c.registers.program_counter=0x1dbf0;c.registers.status=0x2700;
        auto result=host.call_function(353,1,0x72,2,0x20924,0x1dbf0,c);
        check(!host.faulted() && result.control==1 && c.registers.address[7]==0x7ff4,"Real contact dispatch failed");
        check(!(host.region_bytes(2)[0x3540]&2),"Original contact gate blocked separating motion");
        for(auto [id,pc]:{std::pair{355U,0x1de30U},std::pair{357U,0x1e124U}}){
            c.registers.address[7]=0x7ff0;c.registers.program_counter=pc;result=host.call_function(id,1,0x72,2,0x20928,pc,c);
            check(!host.faulted() && result.control==1,"Original terrain/movement gate failed");
        }
        const auto final=host.region_bytes(2);check(!std::equal(ram.begin()+0x3512,ram.begin()+0x351e,final.begin()+0x3512),"Real actor did not move");
        const auto moved=[&](unsigned a){std::uint32_t n=0;for(unsigned i=0;i<4;++i)n=(n<<8)|final[a+i];return static_cast<std::int32_t>(n)/65536.0;};
        check(final[0x355c]==0 && final[0x355d]==0,"Navigation changed original heading");
        const auto heading=host.enemy_walking_heading(0x3500,0);
        const auto radians=heading*6.283185307179586/2048;
        const auto dx=moved(0x3512)-60,dy=moved(0x351a)-100;
        check((dx*std::cos(radians)+dy*std::sin(radians))/std::hypot(dx,dy)>0.99,"Enemy facing disagrees with committed navigation step");
        check(final[0x3540]&8,"Changed facing did not request sprite refresh");
    }
    { // NPC contact blocks one update; a later legal route must be retried.
        gain_ground::RuntimeHost host;gain_ground::System24Devices devices;host.attach_devices(devices);host.select_cpu(1);
        std::vector<std::uint8_t> ram(0x40000),shared(0x40000,1);actor(ram,0x3500,60,100);actor(ram,0x3580,78,100);actor(ram,0x3600,68,116);
        ram[0x32ded]=48;ram[0x32def]=62;put(ram,0x6c00,0x8000,2);put(ram,0x6c02+(100-32)*2,0x3580,2);put(ram,0x6c02+(100-31)*2,0x3600,2);put(ram,0x7ff0,0x20924,4);
        host.load_region(2,0,ram);host.load_region(3,0,shared);
        gain_ground::FunctionContext c{};c.host=&host;c.cpu=1;c.state=0x72;c.registers.address[5]=0x3500;c.registers.status=0x2700;
        const auto contact=[&]{c.registers.address[7]=0x7ff0;c.registers.program_counter=0x1dbf0;
            auto result=host.call_function(353,1,0x72,2,0x20924,0x1dbf0,c);check(!host.faulted() && result.control==1,"Repeated contact failed");};
        const auto commit=[&]{c.registers.address[7]=0x7ff0;c.registers.program_counter=0x1e124;
            const auto result=host.call_function(357,1,0x72,2,0x20930,0x1e124,c);check(!host.faulted() && result.control==1,"Blocked movement completion failed");};
        contact();check(host.region_bytes(2)[0x3540]&2,"Test did not establish NPC blocking flag");commit();
        devices.advance(17500000);contact();check(host.region_bytes(2)[0x3540]&2,"Recovery crossed blocked terrain");commit();
        std::fill(shared.begin(),shared.end(),0);host.load_region(3,0,shared);
        devices.advance(35000000);contact();
        check(!(host.region_bytes(2)[0x3540]&2),"NPC blocking flag prevented next-update recovery");
    }
    std::cout<<"PASS: obstacle route, reciprocal avoidance, narrow passage, crowd, overlap recovery, unreachable goal and live behavior gates\n";
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
