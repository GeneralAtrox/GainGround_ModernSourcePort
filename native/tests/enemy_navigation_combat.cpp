#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_devices.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
using namespace gain_ground;
namespace {
void check(bool v,const char *s){if(!v)throw std::runtime_error(s);}
void put(std::vector<std::uint8_t>&r,unsigned a,unsigned v,unsigned n){for(unsigned i=0;i<n;++i)r[a+i]=v>>((n-i-1)*8);}
unsigned word(std::span<const std::uint8_t>r,unsigned a){return (unsigned(r[a])<<8)|r[a+1];}
void call(RuntimeHost &h,unsigned id,unsigned pc){
    h.write_memory_word(2,0x7ff0,2,0xffff);h.write_memory_word(2,0x7ff2,0x0934,0xffff);
    FunctionContext c{};c.host=&h;c.cpu=1;c.state=0x72;c.registers.status=0x2700;
    c.registers.address[5]=0x3500;c.registers.address[7]=0x7ff0;c.registers.program_counter=pc;
    const auto result=h.call_function(id,1,0x72,2,0x20930,pc,c);
    if(h.faulted() || result.control!=1)std::cerr<<"call "<<id<<" pc "<<std::hex<<pc<<" result "<<unsigned(result.status)<<'/'<<unsigned(result.control)<<" exit "<<result.exit_program_counter<<std::dec<<'\n';
    check(!h.faulted() && result.status==TranslationStatus::complete && result.control==1,"Combat callback failed");
}
}
int main(){try{
    for(unsigned family:{331U,332U})for(bool prior_contact:{false,true}){
        auto reference=std::make_unique<RuntimeHost>();auto live=std::make_unique<RuntimeHost>();
        System24Devices devices;live->attach_devices(devices);reference->select_cpu(1);live->select_cpu(1);
        std::vector<std::uint8_t> ram(0x40000),shared(0x40000);
        for(unsigned a:{0x3500U,0x3580U}){
            ram[a]=0x80;ram[a+0xa]=(a-0x3400)/128;ram[a+0xb]=8;
            put(ram,a+2,family==331 ? 0x20918:0x2093a,4);put(ram,a+0x12,(a==0x3500 ? 100:118)*65536,4);
            put(ram,a+0x1a,100*65536,4);put(ram,a+0x1e,65536,4);ram[a+0x5e]=7;
            put(ram,a+0x6e,0x20000,4);put(ram,a+0x66,0x24000,4);ram[a+0x60]=1;
        }
        ram[0x32ded]=48;ram[0x32def]=62;ram[0x340a]=1;put(ram,0x3410,100,2);put(ram,0x3412,220,2);
        put(ram,0x6c00,0x8000,2);if(prior_contact)put(ram,0x6c02+(100-32)*2,0x3580,2);
        // Controlled attack descriptor: one burst, three differently aimed shots.
        put(ram,0x24004,0x213d2,4);ram[0x24009]=16;ram[0x2400b]=10;put(ram,0x2400c,0x25000,4);
        ram[0x24012]=1;ram[0x24014]=3;ram[0x24015]=8;
        for(unsigned i=0;i<8;++i)put(ram,0x25002+i*4,0x8000,2);
        if(prior_contact)std::fill(shared.begin(),shared.end(),1);
        for(auto *h:{reference.get(),live.get()}){h->load_region(2,0,ram);h->load_region(3,0,shared);}
        if(prior_contact){
            for(auto *h:{reference.get(),live.get()}){call(*h,353,0x1dbf0);check(h->region_bytes(2)[0x3540]&2,"No prior contact established");call(*h,357,0x1e124);}
            std::fill(shared.begin(),shared.end(),0);for(auto *h:{reference.get(),live.get()})h->load_region(3,0,shared);
        }
        for(unsigned tick=0;tick<=4;++tick){
            devices.advance((tick+1)*17500000ULL);
            for(auto *h:{reference.get(),live.get()}){
                // Keep the controlled target exactly above the moving actor.
                h->write_memory_word(2,0x3410,word(h->region_bytes(2),0x3512),0xffff);
                if(tick)call(*h,family,family==331 ? 0x1c37e:0x1cc66);
                call(*h,368,0x1efdc); // Actual aim selection / wind-up / F608 fan.
                if(family==331)call(*h,356,0x1e0ee);
                call(*h,352,0x1dbc4);call(*h,353,0x1dbf0);call(*h,355,0x1de30);
                if(family==332)call(*h,356,0x1e0ee);
                call(*h,357,0x1e124);
            }
            const auto a=reference->region_bytes(2),b=live->region_bytes(2);
            check(std::equal(a.begin()+0x3500,a.begin()+0x3580,b.begin()+0x3500),"Navigation changed attack actor state");
            check(std::equal(a.begin()+0x5400,a.begin()+0x6400,b.begin()+0x5400),"Navigation changed original projectiles");
        }
        const auto r=live->region_bytes(2);unsigned projectiles=0;
        for(unsigned a=0x5400;a<0x6400;a+=128)if(r[a]&128)++projectiles;
        check(projectiles==3,"Original three-shot fan did not execute");
        check(word(r,0x545c)!=word(r,0x54dc),"Fan directions were not distinct");
    }
    std::cout<<"PASS: both callback families, initial attack entry, prior contact, wind-up and complete original three-projectile fan\n";
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
