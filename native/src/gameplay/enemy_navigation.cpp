#include "gain_ground/gameplay/enemy_navigation.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>

namespace gain_ground::gameplay {
namespace {
NavigationPoint add(NavigationPoint a,NavigationPoint b){return {a.x+b.x,a.y+b.y};}
NavigationPoint sub(NavigationPoint a,NavigationPoint b){return {a.x-b.x,a.y-b.y};}
NavigationPoint scale(NavigationPoint a,double n){return {a.x*n,a.y*n};}
double length(NavigationPoint a){return std::hypot(a.x,a.y);}
NavigationPoint unit(NavigationPoint a){auto n=length(a);return n>0 ? scale(a,1/n):NavigationPoint{};}
double dot(NavigationPoint a,NavigationPoint b){return a.x*b.x+a.y*b.y;}
double overlap(const NavigationBody &a,const NavigationBody &b){
    return std::max(0.0,a.half_width+b.half_width+1-std::abs(a.position.x-b.position.x))*
        std::max(0.0,a.half_height+b.half_height+1-std::abs(a.position.y-b.position.y));
}
double penetration(const NavigationBody &a,const NavigationBody &b){
    return std::max(0.0,a.half_width+b.half_width-std::abs(a.position.x-b.position.x))*
        std::max(0.0,a.half_height+b.half_height-std::abs(a.position.y-b.position.y));
}
bool occupied(const NavigationWorld &world,const NavigationBody &body,NavigationPoint point,bool claims=false){
    auto moved=body;moved.position=point;
    for(const auto &other:world.bodies())if(other.id!=body.id && overlap(moved,other)>0)return true;
    if(claims)for(const auto &claim:world.reservations())if(claim.id!=body.id){
        auto other=body;other.id=claim.id;other.position=claim.position;
        if(overlap(moved,other)>0)return true;
    }
    return false;
}
bool terrain_clear(const NavigationWorld &world,NavigationPoint a,NavigationPoint b){
    const auto d=sub(b,a);const auto steps=std::max(1,int(std::ceil(length(d)/2)));
    for(int i=1;i<=steps;++i)if(!world.walkable(add(a,scale(d,double(i)/steps))))return false;
    return true;
}
bool free_step(const NavigationWorld &world,const NavigationBody &body,NavigationPoint delta){
    if(!terrain_clear(world,body.position,add(body.position,delta)))return false;
    for(const auto &other:world.bodies())if(other.id!=body.id){
        double previous=overlap(body,other),physical=penetration(body,other);
        const auto steps=std::max(1,int(std::ceil(length(delta)*2)));
        for(int i=1;i<=steps;++i){
            auto moved=body;moved.position=add(body.position,scale(delta,double(i)/steps));
            const auto after=overlap(moved,other);
            const auto next_physical=penetration(moved,other);
            if((after>0 && previous==0) || after>previous+0.0001 || next_physical>physical+0.0001)return false;
            previous=after;physical=next_physical;
        }
    }
    return true;
}
std::vector<NavigationPoint> route(const NavigationWorld &world,const NavigationBody &body,NavigationPoint goal,bool dynamic){
    constexpr int width=48,height=62,count=width*height;
    const auto point=[](int id){return NavigationPoint{double(id%width*8+4),double(id/width*8+4)};};
    std::array<double,count> cost;cost.fill(std::numeric_limits<double>::infinity());
    std::array<int,count> parent;parent.fill(-1);
    std::array<signed char,count> usable;usable.fill(-1);
    auto valid=[&](int id){if(usable[id]<0)usable[id]=world.walkable(point(id)) ? 1:0;return usable[id]!=0;};
    int start=-1;double nearest=16;
    for(int y=std::max(0,int(body.position.y/8)-1);y<=std::min(height-1,int(body.position.y/8)+1);++y)
        for(int x=std::max(0,int(body.position.x/8)-1);x<=std::min(width-1,int(body.position.x/8)+1);++x){
            int id=y*width+x;double distance=length(sub(point(id),body.position));
            if(distance<nearest && valid(id) && terrain_clear(world,body.position,point(id)) &&
               (!dynamic || (!occupied(world,body,point(id)) && free_step(world,body,sub(point(id),body.position))))){start=id;nearest=distance;}
        }
    if(start<0)return {};
    using Entry=std::pair<double,int>;std::priority_queue<Entry,std::vector<Entry>,std::greater<Entry>> open;
    cost[start]=0;open.push({length(sub(point(start),goal)),start});
    int best=start;double best_distance=length(sub(point(start),goal));
    std::array<bool,count> closed{};
    while(!open.empty()){
        const auto id=open.top().second;open.pop();if(closed[id])continue;closed[id]=true;
        const auto here=point(id);const auto distance=length(sub(here,goal));
        if(distance<best_distance){best=id;best_distance=distance;}
        auto here_body=body;here_body.position=here;
        if(distance<=8 && terrain_clear(world,here,goal) &&
           (!dynamic || free_step(world,here_body,sub(goal,here)))){best=id;break;}
        for(int dy=-1;dy<=1;++dy)for(int dx=-1;dx<=1;++dx){
            if(dx==0 && dy==0)continue;
            int x=id%width+dx,y=id/width+dy;if(x<0||x>=width||y<0||y>=height)continue;
            int next=y*width+x;if(closed[next]||!valid(next)||!terrain_clear(world,here,point(next)))continue;
            // Dynamic bodies are checked separately from cached terrain. A
            // temporary NPC wall must not be cheaper than its open detour.
            if(dynamic && (occupied(world,body,point(next)) || !free_step(world,here_body,sub(point(next),here))))continue;
            const auto candidate=cost[id]+(dx && dy ? 11.3137085:8);
            if(candidate<cost[next]){cost[next]=candidate;parent[next]=id;open.push({candidate+length(sub(point(next),goal)),next});}
        }
    }
    std::vector<NavigationPoint> result;
    auto best_body=body;best_body.position=point(best);
    if(length(sub(point(best),goal))<=8 && terrain_clear(world,point(best),goal) &&
       (!dynamic || free_step(world,best_body,sub(goal,point(best)))))result.push_back(goal);
    else if(best==start)return {};
    for(int id=best;id!=start;id=parent[id])result.push_back(point(id));
    result.push_back(point(start));std::reverse(result.begin(),result.end());return result;
}
NavigationPoint waiting_position(NavigationState &state,const NavigationWorld &world,
                                 const NavigationBody &body,NavigationPoint anchor){
    if(state.waiting_assigned && length(sub(anchor,state.boundary_anchor))<=8 &&
       world.walkable(state.waiting_position) && !occupied(world,body,state.waiting_position,true))return state.waiting_position;
    state.waiting_assigned=false;state.boundary_anchor=anchor;
    // Prefer the nearest free waiting space, breaking ties by travel distance.
    // Keep the claim while valid, rather than chasing newly vacant positions.
    std::vector<std::pair<double,NavigationPoint>> candidates;
    for(int y=-12;y<=12;++y)for(int x=-18;x<=18;++x){
        NavigationPoint p{anchor.x+x*(body.half_width*2+2),anchor.y+y*(body.half_height*2+2)};
        if(!world.walkable(p) || occupied(world,body,p,true))continue;
        candidates.push_back({length(sub(p,anchor))+length(sub(p,body.position))*0.1,p});
    }
    std::stable_sort(candidates.begin(),candidates.end(),[](const auto &a,const auto &b){return a.first<b.first;});
    for(const auto &[cost,p]:candidates){
        if(free_step(world,body,sub(p,body.position))){
            state.waiting_position=p;state.waiting_assigned=true;break;
        }
        const auto path=route(world,body,p,true);
        if(!path.empty() && length(sub(path.back(),p))<0.01){state.waiting_position=p;state.waiting_assigned=true;break;}
    }
    // No free anchor: remain on legal ground and retry as occupancy changes.
    return state.waiting_assigned ? state.waiting_position:body.position;
}
}
bool EnemyNavigation::separates(const NavigationBody &body,NavigationPoint delta,const NavigationBody &other){
    auto moved=body;moved.position=add(body.position,delta);
    return length(delta)>0.00001 && overlap(body,other)>0 &&
        overlap(moved,other)<=overlap(body,other)+0.0001 &&
        penetration(moved,other)<=penetration(body,other)+0.0001;
}
NavigationPoint EnemyNavigation::steer(NavigationState &state,const NavigationWorld &world,
        const NavigationBody &body,NavigationPoint goal,NavigationPoint intended) const {
    const auto speed=length(intended);if(speed<0.00001)return {};
    const auto requested=goal;goal=world.constrain_goal(goal);
    state.boundary_goal=length(sub(requested,goal))>0.00001;
    state.waiting=false;
    if(state.boundary_goal)goal=waiting_position(state,world,body,goal);
    else state.waiting_assigned=false;
    if(state.initialized && length(sub(body.position,state.previous))<speed*0.1)++state.stalled;
    else state.stalled=0;
    state.previous=body.position;state.initialized=true;++state.age;
    // A player outside the permitted area is not an obstacle to orbit. Wait
    // at the reachable edge, unless an existing overlap requires separation.
    const auto goal_distance=length(sub(goal,body.position));
    if(state.boundary_goal && goal_distance<=0.25 && !occupied(world,body,body.position)){
        state.route.clear();state.stalled=0;state.waiting=true;return {};
    }
    if(length(sub(goal,state.goal))>16){state.route.clear();state.age=12;state.no_progress=0;state.best_remaining=1e30;}
    state.goal=goal;
    const bool clear_goal=terrain_clear(world,body.position,goal);
    const bool clear_step=free_step(world,body,scale(unit(sub(goal,body.position)),std::min(goal_distance,std::max(6.0,speed*12))));
    state.blocked_updates=clear_step ? 0:state.blocked_updates+1;
    if(!clear_goal || !clear_step || !free_step(world,body,scale(unit(intended),std::max(6.0,speed*12))))state.navigating=true;
    while(!state.route.empty() && length(sub(state.route.front(),body.position))<=std::max(0.25,speed))state.route.erase(state.route.begin());
    const auto progress=state.route.empty() ? goal:state.route.front();
    const auto remaining=length(sub(progress,body.position));
    if(length(sub(progress,state.progress_point))>1 || remaining<state.best_remaining-std::max(0.25,speed)){
        state.progress_point=progress;state.best_remaining=remaining;state.no_progress=0;
    }else ++state.no_progress;
    const bool npc_detour=state.blocked_updates>=6 || state.no_progress>=16;
    if((!clear_goal || npc_detour) && (state.route.empty() || state.stalled>=12 || state.no_progress>=24) && state.age>=12){
        state.dynamic_route=npc_detour;
        state.route=route(world,body,goal,state.dynamic_route);state.age=0;state.no_progress=0;state.best_remaining=1e30;
        if(state.boundary_goal && state.dynamic_route &&
           (state.route.empty() || length(sub(state.route.back(),goal))>0.25))state.waiting_assigned=false;
    }
    if(clear_goal && !state.dynamic_route)state.route.clear();
    while(!state.route.empty() && length(sub(state.route.front(),body.position))<=std::max(0.25,speed))state.route.erase(state.route.begin());
    auto desired=state.route.empty() ? ((state.navigating || state.boundary_goal) ? scale(unit(sub(goal,body.position)),speed):intended)
                                    :scale(unit(sub(state.route.front(),body.position)),speed);
    if(state.boundary_goal && goal_distance<=0.25)desired=intended; // Escape an overlap at the destination.
    else if(state.boundary_goal && state.route.empty())desired=scale(unit(desired),std::min(speed,goal_distance));
    // Arrival must not avoid a neighbor *beyond* the reserved destination.
    // The ordinary lookahead otherwise scores a legal final step as a collision
    // and makes the actor reverse repeatedly one pixel from its waiting place.
    if(state.boundary_goal && goal_distance>0.25 && goal_distance<=std::max(6.0,speed*12) &&
       free_step(world,body,sub(goal,body.position))){
        state.route.clear();state.yield_updates=0;
        return scale(unit(sub(goal,body.position)),std::min(speed,goal_distance));
    }
    if(!state.route.empty() && !terrain_clear(world,body.position,add(body.position,desired))){state.route.clear();state.age=12;}
    const auto forward=unit(desired),right=NavigationPoint{-forward.y,forward.x};
    // Follow a valid detour instead of repeatedly steering back toward a crowd.
    if(!state.route.empty() && free_step(world,body,desired)){state.yield_updates=0;return desired;}
    if(state.yield_updates){
        --state.yield_updates;auto delta=scale(state.yield_direction,speed);
        if(free_step(world,body,delta))return delta;
        state.yield_updates=0;
    }
    NavigationPoint chosen{};double best=-1e30;
    // Stable right-hand preference prevents reciprocal left/right oscillation.
    // Backward choices let the yielding actor retreat toward a passing space.
    for(int angle:{0,1,-1,2,-2,3,-3,4}){
        const double radians=angle*0.7853981633974483;
        auto direction=add(scale(forward,std::cos(radians)),scale(right,std::sin(radians)));
        auto delta=scale(direction,length(desired));if(!free_step(world,body,delta))continue;
        double score=dot(direction,forward)*4+(angle>0 ? 0.12:0);
        auto ahead=body;ahead.position=add(body.position,scale(direction,std::max(6.0,speed*12)));
        for(const auto &other:world.bodies())if(other.id!=body.id){
            if(overlap(ahead,other)>0)score-=8;
            if(overlap(body,other)>0){auto moved=body;moved.position=add(body.position,delta);score+=(overlap(body,other)-overlap(moved,other))*0.05;}
            if(body.id>other.id && overlap(ahead,other)>0 && state.stalled>=12)score-=4;
        }
        if(score>best){best=score;chosen=delta;}
    }
    if(length(chosen)>0.00001 && state.blocked_updates>=6){
        for(const auto &other:world.bodies())if(body.id>other.id &&
            length(sub(body.position,other.position))<36 && dot(unit(chosen),forward)<0.5){
            state.yield_direction=unit(chosen);state.yield_updates=6;break;
        }
    }
    return chosen;
}
} // namespace gain_ground::gameplay
