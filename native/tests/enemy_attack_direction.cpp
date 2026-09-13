#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_devices.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
using namespace gain_ground;
namespace {
constexpr unsigned actor=0x3500;
void check(bool ok,const char *why){if(!ok)throw std::runtime_error(why);}
void put(std::vector<std::uint8_t>&r,unsigned a,unsigned v,unsigned n){
    for(unsigned i=0;i<n;++i)r[a+i]=static_cast<std::uint8_t>(v>>((n-i-1)*8));
}
unsigned word(std::span<const std::uint8_t>r,unsigned a){return (unsigned(r[a])<<8)|r[a+1];}
void byte(RuntimeHost &h,unsigned a,unsigned v){h.write_memory_word(2,a&~1U,static_cast<std::uint16_t>(v<<(a&1 ? 0:8)),a&1 ? 0xff:0xff00);}
FunctionContext call(RuntimeHost &h,unsigned id,unsigned pc,unsigned site=0x20930){
    h.write_memory_word(2,0x7ff0,2,0xffff);h.write_memory_word(2,0x7ff2,0x0934,0xffff);
    FunctionContext c{};c.host=&h;c.cpu=1;c.state=0x72;c.registers.status=0x2700;
    c.registers.address[5]=actor;c.registers.address[7]=0x7ff0;c.registers.program_counter=pc;
    const auto result=h.call_function(id,1,0x72,2,site,pc,c);
    check(!h.faulted() && result.status==TranslationStatus::complete && result.control==1,"Attack callback failed");
    check(c.registers.address[7]==0x7ff4,"Attack call unbalanced the stack");
    return c;
}
std::vector<std::uint8_t> setup(const std::vector<std::uint8_t>&image,unsigned callback,unsigned normal,unsigned current,unsigned shots){
    auto r=image;
    std::fill(r.begin(),r.begin()+0x8000,0);
    std::fill(r.begin()+0x24000,r.begin()+0x24020,0);
    std::fill(r.begin()+0x25000,r.begin()+0x25022,0);
    r[actor]=0x80;r[actor+0xb]=8;
    put(r,actor+2,callback,4);put(r,actor+0x12,172*65536,4);put(r,actor+0x1a,199*65536,4);
    put(r,actor+0x1e,65536,4);put(r,actor+0x5c,1024,2);
    r[actor+0x5e]=static_cast<std::uint8_t>(current);r[actor+0x5f]=static_cast<std::uint8_t>(normal);
    r[actor+0x60]=1;put(r,actor+0x66,0x24000,4);put(r,actor+0x6e,0x20000,4);
    r[0x340a]=1;put(r,0x3410,119,2);put(r,0x3412,405,2);
    put(r,0x24004,0x213d2,4);r[0x24009]=16;r[0x2400b]=10;put(r,0x2400c,0x25000,4);
    r[0x24012]=1;r[0x24014]=static_cast<std::uint8_t>(shots);r[0x24015]=8;
    for(unsigned i=0;i<8;++i)put(r,0x25002+i*4,0x8000,2);
    return r;
}
void load(RuntimeHost &h,const std::vector<std::uint8_t>&r){h.select_cpu(1);check(h.load_region(2,0,r),"RAM load failed");}
void compare(RuntimeHost &a,RuntimeHost &b,bool mode_difference){
    const auto x=a.region_bytes(2),y=b.region_bytes(2);
    for(unsigned i=actor;i<actor+128;++i)if(!mode_difference || i!=actor+0x5e)
        check(x[i]==y[i],"Attack changed more than the selected direction mode");
    check(std::equal(x.begin()+0x5400,x.begin()+0x6400,y.begin()+0x5400),"Projectile state differs from authored-mode attack");
}
}
int main(int argc,char **argv){try{
    check(argc==2,"Supply the retained CPU-B state72 data image");
    std::ifstream input(argv[1],std::ios::binary|std::ios::ate);
    check(input && input.tellg()==0x40000,"Missing CPU-B data image");
    std::vector<std::uint8_t> image(0x40000);input.seekg(0);input.read(reinterpret_cast<char*>(image.data()),image.size());
    check(bool(input),"Short CPU-B data image");
    unsigned cases=0;
    for(unsigned family:{331U,332U})for(unsigned normal:{7U,8U,9U})
    for(unsigned restriction:{1U,2U,3U,4U,5U,6U,7U})for(unsigned shots:{1U,3U}){
        auto reference=std::make_unique<RuntimeHost>(),live=std::make_unique<RuntimeHost>();
        System24Devices devices;live->attach_devices(devices);
        const auto callback=family==331 ? 0x20918U:0x2093aU;
        load(*reference,setup(image,callback,normal,normal,shots));
        load(*live,setup(image,callback,normal,restriction,shots));
        for(unsigned tick=0;tick<=4;++tick){
            for(auto *h:{reference.get(),live.get()}){
                // Change the target during wind-up: F331 tracks; F332 locks its initial aim.
                if(tick==1){h->write_memory_word(2,0x3410,300,0xffff);h->write_memory_word(2,0x3412,100,0xffff);}
                if(tick)call(*h,family,family==331 ? 0x1c37e:0x1cc66);
                call(*h,368,0x1efdc);
            }
            compare(*reference,*live,true);
            check(live->region_bytes(2)[actor+0x5e]==restriction,"Attack erased a movement restriction");
            check(live->region_bytes(2)[actor+0x5f]==normal,"Attack overwrote the authored mode");
            if(tick==0 && normal==8)check(word(live->region_bytes(2),actor+0x5c)==640,"Reported shot must aim down-left (640), not left (1024)");
        }
        unsigned count=0;for(unsigned p=0x5400;p<0x6400;p+=128)if(live->region_bytes(2)[p]&128)++count;
        check(count==shots,"Attack changed shot count or timing");
        if(shots==3)check(word(live->region_bytes(2),0x545c)!=word(live->region_bytes(2),0x54dc),"Attack flattened the original fan");
        // An unrelated decoder call, even with a forged attack callsite, must use movement mode.
        byte(*reference,actor+0x5e,restriction);
        reference->write_memory_word(2,actor+0x5c,578,0xffff);live->write_memory_word(2,actor+0x5c,578,0xffff);
        const auto a=call(*reference,364,0x1ec00,0x1c3ac),b=call(*live,364,0x1ec00,0x1c3ac);
        check(a.registers.data==b.registers.data && a.registers.status==b.registers.status,"Attack scope leaked into a standalone movement call");
        compare(*reference,*live,false);++cases;
    }
    // Shared attack helpers do not authorize changing deliberately fixed/scripted families.
    for(unsigned callback:{0x208f6U,0x20d5cU,0x21cd2U}){
        auto reference=std::make_unique<RuntimeHost>(),live=std::make_unique<RuntimeHost>();System24Devices devices;live->attach_devices(devices);
        auto ram=setup(image,callback,8,1,1);load(*reference,ram);load(*live,ram);
        for(auto *h:{reference.get(),live.get()}){call(*h,368,0x1efdc);if(callback==0x208f6)call(*h,333,0x1ce0a);}
        compare(*reference,*live,false);
        check(word(live->region_bytes(2),actor+0x5c)==(callback==0x208f6 ? 512U:1024U),"Original fixed attack changed");++cases;
    }
    for(unsigned mode:{0U,10U,255U}){
        auto reference=std::make_unique<RuntimeHost>(),live=std::make_unique<RuntimeHost>();System24Devices devices;live->attach_devices(devices);
        auto ram=setup(image,0x20918,mode,1,1);load(*reference,ram);load(*live,ram);
        call(*reference,368,0x1efdc);call(*live,368,0x1efdc);compare(*reference,*live,false);++cases;
    }
    std::cout<<"PASS: "<<cases<<" attack direction cases; authored aim, both wind-up policies, original fans, retained movement modes and scope exclusions\n";
}catch(const std::exception &e){std::cerr<<e.what()<<'\n';return 1;}}
