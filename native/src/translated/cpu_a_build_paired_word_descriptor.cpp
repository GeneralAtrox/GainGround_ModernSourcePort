#include "gain_ground/contract_types.h"
#include "gground_functions.h"
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t P=1U,S=3U,T=5U,W=0xffffU;
constexpr std::uint32_t SM=0x0003ffffU;
void pf(ExecutionHost&h,std::uint32_t a){(void)h.read_memory_word(P,a,W);}
std::uint16_t rs(ExecutionHost&h,std::uint32_t a){return h.read_memory_word(S,a&SM,W);}
void ws(ExecutionHost&h,std::uint32_t a,std::uint16_t v){h.write_memory_word(S,a&SM,v,W);}
std::uint8_t rb(ExecutionHost&h,std::uint32_t a){const auto v=h.read_memory_word(P,a&~1U,(a&1U)?0x00ffU:0xff00U);return static_cast<std::uint8_t>((a&1U)?v:v>>8U);}
void logic(CpuRegisters&r,std::uint32_t v,std::uint32_t sign){std::uint16_t f=r.status&0x10U;if(v&sign)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void addb(CpuRegisters&r,std::uint8_t a,std::uint8_t b,std::uint8_t v){const bool c=static_cast<unsigned>(a)+b>0xffU;const bool o=((~(a^b)&(a^v))&0x80U)!=0U;std::uint16_t f=0;if(c)f|=0x11U;if(v&0x80U)f|=8U;if(!v)f|=4U;if(o)f|=2U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void irq4(FunctionContext&c){auto&h=*c.host;auto&r=c.registers;const auto p=h.consume_pending_interrupt(0U,0xffU,0x384aU);const auto m=static_cast<std::uint8_t>((r.status>>8U)&7U);if(!p.asserted||p.level<=m)return;const auto sr=r.status;r.address[7]-=4U;ws(h,r.address[7]+2U,0x384eU);r.address[7]-=2U;ws(h,r.address[7],sr);ws(h,r.address[7]+2U,0U);r.status=static_cast<std::uint16_t>((sr&0x38ffU)|0x2000U|(static_cast<std::uint16_t>(p.level)<<8U));pf(h,0x70U);pf(h,0x72U);(void)rs(h,0x48U);(void)rs(h,0x4aU);r.program_counter=0x80048U;(void)h.call_function(47U,0U,0xffU,0U,0x384aU,0x80048U,c);(void)cpu_a_irq4_vector_trampoline(c);r.program_counter=0x384eU;}
}

FunctionResult cpu_a_build_paired_word_descriptor(FunctionContext&c) noexcept
{
    if(!c.host)return{TranslationStatus::contract_violation,0U,c.registers.program_counter};
    auto&h=*c.host;auto&r=c.registers;
    const auto src=rb(h,r.address[4]++);r.data[0]=(r.data[0]&0xffffff00U)|src;logic(r,src,0x80U);
    pf(h,0x3844U);const auto twice=static_cast<std::uint8_t>(src+src);r.data[0]=(r.data[0]&0xffffff00U)|twice;addb(r,src,src,twice);
    pf(h,0x3846U);const auto even=static_cast<std::uint8_t>(twice+2U);r.data[0]=(r.data[0]&0xffffff00U)|even;addb(r,twice,2U,even);
    pf(h,0x3848U);h.write_memory_word(T,r.address[0]-0x200000U,static_cast<std::uint16_t>(r.data[0]),W);r.address[0]+=2U;logic(r,static_cast<std::uint16_t>(r.data[0]),0x8000U);
    pf(h,0x384aU);const auto odd=static_cast<std::uint8_t>(even+1U);r.data[0]=(r.data[0]&0xffffff00U)|odd;addb(r,even,1U,odd);
    pf(h,0x384cU);pf(h,0x384eU);h.write_memory_word(T,r.address[0]-0x200000U+0x7eU,static_cast<std::uint16_t>(r.data[0]),W);logic(r,static_cast<std::uint16_t>(r.data[0]),0x8000U);pf(h,0x3850U);irq4(c);
    const auto sp=r.address[7]&SM;const auto target=(static_cast<std::uint32_t>(rs(h,sp))<<16U)|rs(h,sp+2U);r.address[7]+=4U;pf(h,target);pf(h,target+2U);r.program_counter=target;return FunctionResult::complete(1U,target);
}
} // namespace gain_ground::translated
