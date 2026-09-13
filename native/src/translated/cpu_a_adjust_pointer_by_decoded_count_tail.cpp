#include "gain_ground/contract_types.h"
#include "gground_functions.h"
#include <cstdint>

namespace gain_ground::translated { namespace {
constexpr std::uint16_t P=1,S=3,W=0xffff; constexpr std::uint32_t M=0x3ffff;
void pf(ExecutionHost&h,std::uint32_t a){(void)h.read_memory_word(P,a,W);} void ws(ExecutionHost&h,std::uint32_t a,std::uint16_t v){h.write_memory_word(S,a&M,v,W);}
void logicw(CpuRegisters&r,std::uint16_t v){std::uint16_t f=r.status&0x10;if(v&0x8000)f|=8;if(!v)f|=4;r.status=static_cast<std::uint16_t>((r.status&~0x1f)|f);}
}
FunctionResult cpu_a_adjust_pointer_by_decoded_count_tail(FunctionContext&c) noexcept {if(!c.host)return{TranslationStatus::contract_violation,0,c.registers.program_counter};auto&h=*c.host;auto&r=c.registers;r.address[7]-=4;ws(h,r.address[7],0);ws(h,r.address[7]+2,0x1a86);pf(h,0x1a64);pf(h,0x1a66);auto z=h.call_function(26,0,0xff,2,0x1a82,0x1a64,c);if(z.status!=TranslationStatus::complete)return z;auto v=static_cast<std::uint16_t>(r.data[0])&0xfffe;r.data[0]=(r.data[0]&0xffff0000)|v;logicw(r,v);pf(h,0x1a8a);r.address[3]-=static_cast<std::int16_t>(v);pf(h,0x1a8c);pf(h,0x1a8e);r.program_counter=0x1a8c;return h.call_function(28,0,0xff,0,0x1a8a,0x1a8c,c);}
} // namespace gain_ground::translated
