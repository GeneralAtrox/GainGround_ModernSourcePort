#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_devices.h"
#include "gain_ground/native_function_registry.h"
#include <iostream>
#include <vector>

using namespace gain_ground;
static void word(std::vector<std::uint8_t> &m, unsigned a, unsigned v) {
    m[a] = static_cast<std::uint8_t>(v >> 8); m[a+1] = static_cast<std::uint8_t>(v);
}
static bool movement(unsigned mode, unsigned actor) {
    RuntimeHost h;
    std::vector<std::uint8_t> ram(h.region_bytes(2).size());
    word(ram, 0x3478, mode); word(ram, 0x7ff2, 0x9000);
    word(ram, 0x7fe8, 0xa55a); word(ram, 0x7ff4, 0x5aa5);
    h.load_region(2, 0, ram);
    FunctionContext c{}; c.host=&h; c.cpu=1; c.state=0x72;
    auto &r=c.registers; r.program_counter=0x2043c; r.address[5]=actor;
    r.address[7]=0x7ff0; r.data[0]=1024; r.data[1]=static_cast<unsigned>(-512); r.data[4]=1;
    auto result=h.call_function(383,1,0x72,2,0,0x2043c,c);
    // Original 204E4 replaces D0.w with the selector before the helper.
    // Integer ASR truncation matters: 20+5+1=26; 40+10+5+2=57.
    const int x=mode==40?57:actor<0x5400?26:20;
    const int y=mode==40?-184:actor<0x5400?-168:-128;
    const auto m=h.region_bytes(2);
    bool ok=!h.faulted() && result.status==TranslationStatus::complete && result.control==1 &&
        r.program_counter==0x9000 && r.address[7]==0x7ff4 &&
        r.data[0]==static_cast<unsigned>(x) && r.data[1]==static_cast<unsigned>(y) &&
        m[0x7fe8]==0xa5 && m[0x7fe9]==0x5a && m[0x7ff4]==0x5a && m[0x7ff5]==0xa5;
    std::cout<<"movement mode="<<mode<<" actor="<<std::hex<<actor<<std::dec<<" x="<<r.data[0]<<" pass="<<ok<<'\n';
    return ok;
}

struct SoundProbe {
    RuntimeHost host; System24Devices devices;
    bool trap_seen=false, trap_ok=false;
    static void checkpoint(void *arg) {
        auto &p=*static_cast<SoundProbe*>(arg);
        auto c=p.host.active_context();
        if (c && c->registers.program_counter==0x80078 && !p.trap_seen) {
            p.trap_seen=true; const auto &r=c->registers;
            const auto ram=p.host.region_bytes(3); const auto sp=r.address[7]&0x3ffff;
            p.trap_ok=r.data[0]==0x1234 && r.data[1]==0x34 && (r.data[6]&0xff)==0x56 &&
                ram[sp+2]==0 && ram[sp+3]==8 && ram[sp+4]==0x39 && ram[sp+5]==0x58;
        }
        auto deadline=p.host.next_cpu_deadline_ns();
        if(deadline!=UINT64_MAX)p.devices.advance(deadline);
    }
};
static bool sound() {
    SoundProbe p; p.host.attach_devices(p.devices);
    std::vector<std::uint8_t> ram(p.host.region_bytes(3).size());
    ram[0x31000]=0x12; ram[0x31001]=0x34; ram[0x31002]=0x56;
    word(ram,0x37ff0,8); word(ram,0x37ff2,0x1234); word(ram,0x37ff4,0xa55a);
    p.host.load_region(3,0,ram);
    std::vector<std::uint8_t> rom(p.host.region_bytes(1).size()); p.host.load_region(1,0,rom);
    p.host.set_checkpoint(&SoundProbe::checkpoint,&p);
    FunctionContext c{}; c.host=&p.host; c.cpu=0; c.state=0xff;
    auto &r=c.registers; r.program_counter=0x83644; r.status=0x2300;
    r.address[4]=0xff1000; r.address[7]=0xffff7ff0;
    for(unsigned i=1;i<7;++i)r.data[i]=0x12340000+i;
    r.data[0]=27; r.data[7]=0; auto saved=r.data; saved[0]=27*4;
    auto result=p.host.call_function(81,0,0xff,2,0,0x83644,c);
    const auto after=p.host.region_bytes(3);
    bool ok=!p.host.faulted() && p.trap_seen && p.trap_ok &&
        result.status==TranslationStatus::complete && result.control==1 &&
        r.program_counter==0x81234 && r.address[7]==0xffff7ff4 && r.address[4]==0xff1003 &&
        r.data==saved && after[0x37ff4]==0xa5 && after[0x37ff5]==0x5a;
    std::cout<<"sound event27 pass="<<ok<<" trap="<<p.trap_ok<<" pc="<<std::hex<<r.program_counter
        <<" sp="<<r.address[7]<<std::dec<<" fault="<<p.host.fault().message<<'\n';
    return ok;
}

// Bound the non-returning floppy recovery at its first child. Validate real
// registry dispatch and original stack reset without running the error screen.
struct RecoveryProbe : ExecutionHost {
    PendingInterrupt consume_pending_interrupt(std::uint8_t,std::uint8_t,std::uint32_t) override {return {};}
    std::vector<std::uint8_t> ram=std::vector<std::uint8_t>(0x40000);
    bool entered=false, valid=true; unsigned latch=0;
    std::uint16_t read_memory_word(std::uint16_t region,std::uint32_t a,std::uint16_t mask) override {
        if(region!=3)return 0; a&=0x3ffff; return ((ram[a]<<8)|ram[a+1])&mask;
    }
    void write_memory_word(std::uint16_t region,std::uint32_t a,std::uint16_t v,std::uint16_t mask) override {
        if(region==3){a&=0x3ffff; word(ram,a,(read_memory_word(region,a,0xffff)&~mask)|(v&mask));}
    }
    std::uint16_t read_hardware(std::uint8_t,std::uint8_t,std::uint8_t,std::uint32_t,std::uint32_t a,std::uint16_t) override {return a==0xb00004?latch:0;}
    void write_hardware(std::uint8_t,std::uint8_t,std::uint8_t,std::uint32_t,std::uint32_t a,std::uint16_t v,std::uint16_t) override {if(a==0xb00004)latch=v&0xff;}
    FunctionResult call_function(std::uint32_t id,std::uint8_t cpu,std::uint8_t state,std::uint8_t,
        std::uint32_t site,std::uint32_t target,FunctionContext &c) override {
        auto f=native_registry::find(cpu,state,target);
        if(!f || f->id!=id){valid=false;return {TranslationStatus::contract_violation,0,target};}
        if(id==446){
            entered=true;
            valid &= site==0x199a && c.registers.status==0x2600 && c.registers.address[7]==0xfffffffa &&
                read_memory_word(3,0x3fffe,0xffff)==1 && read_memory_word(3,0x3fffa,0xffff)==0 &&
                read_memory_word(3,0x3fffc,0xffff)==0x199e;
            return FunctionResult::unimplemented(); // Deliberate bounded stop.
        }
        return f->entry(c);
    }
};
static bool recovery() {
    RecoveryProbe h; FunctionContext c{}; c.host=&h; c.cpu=0; c.state=0xff;
    c.registers.program_counter=0x113e; c.registers.address[7]=0xffff7000;
    auto result=h.call_function(13,0,0xff,2,0,0x113e,c);
    bool ok=h.entered && h.valid && result.status==TranslationStatus::unimplemented;
    std::cout<<"drive-absent recovery entry pass="<<ok<<'\n';return ok;
}
int main(){bool ok=true; for(auto mode:{20U,40U})for(auto actor:{0x5380U,0x5400U})ok=movement(mode,actor)&&ok;
    ok=sound()&&ok;ok=recovery()&&ok;return ok?0:1;}
