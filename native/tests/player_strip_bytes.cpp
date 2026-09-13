#include "gain_ground/runtime_host.h"
#include <iostream>
#include <vector>
using namespace gain_ground;
int main(){
    for(unsigned address:{0x408U,0x409U,0x40aU})for(unsigned flag:{0U,1U,2U,0x80U}){
        RuntimeHost host;host.select_cpu(1);
        std::vector<std::uint8_t> ram(0x40000);
        const auto word=[&](unsigned a,unsigned v){ram[a]=static_cast<std::uint8_t>(v>>8);ram[a+1]=static_cast<std::uint8_t>(v);};
        word(0x1468,address);word(0x1462,0x4000);word(0x7ffc,0x12);word(0x7ffe,0x3456);
        ram[address^1]=0x55;ram[address]=static_cast<std::uint8_t>(flag);
        host.load_region(2,0,ram);
        std::vector<std::uint8_t> video(host.region_bytes(5).size());host.load_region(5,0,video);
        FunctionContext c{};c.host=&host;c.cpu=1;c.state=0x72;
        c.registers.address[5]=0x1400;c.registers.address[7]=0x7ffc;
        c.registers.program_counter=0xfa26;c.registers.data[7]=0x12345600;c.registers.status=0x2710;
        const auto result=host.call_function(181,1,0x72,2,0xee7a,0xfa26,c);
        if(host.faulted() || result.status!=TranslationStatus::complete || result.control!=1 ||
           c.registers.program_counter!=0x123456 || c.registers.address[7]!=0x8000 ||
           c.registers.address[6]!=address || c.registers.data[7]!=(0x12345600|flag)){
            std::cerr<<"Player strip failed address="<<std::hex<<address<<" flag="<<flag<<" fault="<<host.fault().message<<'\n';return 1;
        }
    }
    std::cout<<"PASS: three player flag bytes, even/odd lanes, full HUD child chain and stack return\n";
}
