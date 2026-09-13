#include "gain_ground/runtime_host.h"
#include "gain_ground/native_function_registry.h"
#include <iostream>
#include <vector>

using namespace gain_ground;
struct Case { unsigned id, entry, start; };
static bool check(Case test, int x, int y) {
    RuntimeHost host;
    std::vector<std::uint8_t> ram(host.region_bytes(2).size());
    constexpr unsigned actor=0x5400, sp=0x7ffa;
    auto word=[&](unsigned a,unsigned v){ram[a]=static_cast<std::uint8_t>(v>>8);ram[a+1]=static_cast<std::uint8_t>(v);};
    ram[actor]=0x81; word(actor+0x12,x); word(actor+0x1a,y); word(actor+0x74,3);
    word(sp,0); word(sp+2,0x8594); word(sp+4,0xa55a); word(sp-8,0x5aa5);
    host.load_region(2,0,ram);
    FunctionContext c{}; c.cpu=1; c.state=0x72; c.host=&host;
    c.registers.program_counter=test.start; c.registers.address[5]=actor;
    c.registers.address[7]=sp; c.registers.status=0x2010;
    const auto *owner=native_registry::find(1,0x72,test.entry);
    if(!owner || owner->id!=test.id)return false;
    host.select_cpu(1);
    // PC-switch callers also admit a precise original call-site start. This
    // isolates return ownership from unrelated earlier actor-state gates.
    const auto result=test.start==test.entry ? host.call_function(test.id,1,0x72,2,0,test.entry,c) : owner->entry(c);
    const auto after=host.region_bytes(2);
    const unsigned removed=test.id==380?1:0;
    bool ok=!host.faulted() && result.status==TranslationStatus::complete && result.control==1 &&
        result.exit_program_counter==0x8594 && c.registers.program_counter==0x8594 &&
        c.registers.address[7]==sp+4 && after[actor]==removed &&
        after[sp+4]==0xa5 && after[sp+5]==0x5a && after[sp-8]==0x5a && after[sp-7]==0xa5 &&
        after[actor+0x12]==ram[actor+0x12] && after[actor+0x13]==ram[actor+0x13] &&
        after[actor+0x1a]==ram[actor+0x1a] && after[actor+0x1b]==ram[actor+0x1b];
    std::cout<<"F"<<test.id<<" x="<<x<<" y="<<y<<" pass="<<ok
        <<" control="<<unsigned(result.control)<<" pc="<<std::hex<<c.registers.program_counter
        <<" sp="<<c.registers.address[7]<<std::dec<<" fault="<<host.fault().message<<'\n';
    return ok;
}
int main(){
    bool ok=true;
    for(auto c:{Case{380,0x1fe60,0x1fe60},Case{251,0x1250a,0x1250a},Case{249,0x120ae,0x120ae},
                Case{534,0x121f4,0x12218},Case{954,0x123ea,0x123f2},
                Case{919,0x19b62,0x19b62},Case{939,0x1a664,0x1a674}}){
        ok=check(c,-33,64)&&ok; ok=check(c,417,64)&&ok;
        ok=check(c,64,-33)&&ok; ok=check(c,64,528)&&ok;
    }
    return ok?0:1;
}
