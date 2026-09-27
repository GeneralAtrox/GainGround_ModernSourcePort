// Bounded gameplay smoke test, seeded by a private runtime RAM snapshot.
// Exercises real native movement, attacks and spawned actor callbacks; this is
// not a full-game replay or an original-machine parity comparison.
#include "gain_ground/runtime_host.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

using namespace gain_ground;
namespace {
constexpr unsigned player = 0x1480, controls = 0xe00;
unsigned word(std::span<const std::uint8_t> b, unsigned a) { return (b[a]<<8)|b[a+1]; }
unsigned lng(std::span<const std::uint8_t> b, unsigned a) { return (word(b,a)<<16)|word(b,a+2); }
void byte(RuntimeHost &h,unsigned a,unsigned v) {
    h.write_memory_word(2,a&~1U,static_cast<std::uint16_t>(v << (a&1 ? 0:8)),a&1 ? 0xff:0xff00);
}
void put(RuntimeHost &h,unsigned a,unsigned v) { h.write_memory_word(2,a,static_cast<std::uint16_t>(v),0xffff); }
bool invoke(RuntimeHost &h,unsigned pc,unsigned actor,unsigned frame) {
    FunctionContext c{}; c.host=&h; c.cpu=1; c.state=0x72;
    c.registers.status=0x2700; c.registers.program_counter=pc;
    c.registers.address[4]=controls; c.registers.address[5]=actor;
    c.registers.address[7]=0x7ffa;
    put(h,0x7ffa,0); put(h,0x7ffc,0x8594);
    const auto r=h.run(c);
    if(h.faulted() || r.status!=TranslationStatus::complete || r.control!=1 ||
       c.registers.program_counter!=0x8594 || c.registers.address[7]!=0x7ffe) {
        std::cout<<"FAIL frame="<<frame<<" actor=0x"<<std::hex<<actor
                 <<" entry=0x"<<pc<<" stopped=0x"<<c.registers.program_counter<<std::dec
                 <<" reason="<<h.fault().message<<'\n';
        return false;
    }
    return true;
}
}
int main(int argc,char **argv) { try {
    if(argc!=4) throw std::runtime_error("Usage: gain_ground_character_smoke SNAPSHOT_DIR ID movement|primary|secondary");
    const auto id=std::stoul(argv[2]); const std::string mode=argv[3];
    if(id>=20 || (mode!="movement" && mode!="primary" && mode!="secondary"))
        throw std::runtime_error("Invalid character or mode");
    RuntimeHost h; h.select_cpu(1);
    for(unsigned region:{2U,3U}) {
        std::ifstream f(std::filesystem::path(argv[1])/(region==2 ? "cpu-b-ram.bin":"cpu-a-shared-ram.bin"),std::ios::binary);
        std::vector<std::uint8_t> b{std::istreambuf_iterator<char>(f),{}};
        if(b.size()!=0x40000 || !h.load_region(region,0,b)) throw std::runtime_error("Missing/invalid RAM snapshot");
    }
    auto b=h.region_bytes(2);
    if(!(b[player]&0x80) || word(b,player+0x60)!=controls || b[player+0x4b]>=20)
        throw std::runtime_error("Snapshot needs an initialized player 1 record");
    const unsigned table=0x26f1c+id*16;
    if(word(b,table+6)>3 || word(b,table+8)>10 || word(b,table+10)!=id)
        throw std::runtime_error("Snapshot does not contain the expected character definitions");
    // Use the live player record layout and original per-character selectors.
    // Isolate its projectiles from actors already active in the saved game.
    for(unsigned a=0x1600;a<0x3400;a+=0x80) byte(h,a,b[a]&0x7f);
    byte(h,player+0x4b,id); put(h,player+0x4c,table>>16); put(h,player+0x4e,table);
    for(unsigned offset:{6U,8U,10U}) put(h,player+0x30+offset,word(b,table+offset));
    for(unsigned a=0x3c;a<=0x41;++a) byte(h,player+a,0);
    put(h,player+0x58,0); // Face up into the stage.
    put(h,player+0x12,192); put(h,player+0x14,0);
    put(h,player+0x1a,384); put(h,player+0x1c,0);
    std::cout<<"character="<<id<<" mode="<<mode<<'\n';
    if(mode=="movement") {
        for(unsigned direction:{0x10U,0x20U,0x40U,0x80U}) {
            byte(h,controls+0x8b,direction);
            if(!invoke(h,0xfe54,player,0)) return 1;
            if(lng(b,player+0x1e)==0 && lng(b,player+0x26)==0)
                throw std::runtime_error("Movement input produced no velocity");
        }
        std::cout<<"PASS four directions\n"; return 0;
    }
    const bool secondary=mode=="secondary";
    const unsigned button=secondary?4:2;
    byte(h,player+0x3e,secondary?2:1);
    std::set<unsigned> callbacks;
    for(unsigned frame=0;frame<180;++frame) {
        byte(h,controls+0x8b,frame<120 ? button:0);
        byte(h,controls+0x8c,frame%30==0 && frame<120 ? button:0);
        if(!invoke(h,secondary?0x10cc0:0x10a2e,player,frame)) return 1;
        for(unsigned a=0x1600;a<0x3400;a+=0x80) if(b[a]&0x80) {
            const auto pc=lng(b,a+2);
            callbacks.insert(pc);
            if(!invoke(h,pc,a,frame)) return 1;
        }
    }
    if(callbacks.empty()) throw std::runtime_error("No spawned actor was exercised");
    std::cout<<"PASS 180 updates; callbacks";
    for(auto pc:callbacks) std::cout<<" 0x"<<std::hex<<pc;
    std::cout<<'\n'; return 0;
} catch(const std::exception &e) { std::cerr<<"ERROR "<<e.what()<<'\n'; return 2; } }
