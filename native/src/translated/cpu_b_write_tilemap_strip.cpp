#include "gain_ground/contract_types.h"
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t P=2U,T=5U,M=0xffffU;
void logic(CpuRegisters&r,std::uint32_t v,std::uint32_t s){std::uint16_t f=r.status&0x10U;if(v&s)f|=8U;if(v==0)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void addw(CpuRegisters&r,std::uint16_t a,std::uint16_t b,std::uint16_t z){const auto w=static_cast<std::uint32_t>(a)+b;std::uint16_t f=0;if(z&0x8000U)f|=8U;if(z==0)f|=4U;if(((~(a^b))&(a^z)&0x8000U)!=0)f|=2U;if(w>0xffffU)f|=0x11U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void cmpb(CpuRegisters&r,std::uint8_t a,std::uint8_t b){const auto z=static_cast<std::uint8_t>(a-b);std::uint16_t f=r.status&0x10U;if(z&0x80U)f|=8U;if(z==0)f|=4U;if(((a^b)&(a^z)&0x80U)!=0)f|=2U;if(a<b)f|=1U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void clear_long(ExecutionHost&h,CpuRegisters&r,std::uint16_t region,std::uint32_t a){(void)h.read_memory_word(region,a,M);(void)h.read_memory_word(region,a+2U,M);h.write_memory_word(region,a+2U,0U,M);h.write_memory_word(region,a,0U,M);logic(r,0U,0x80000000U);}
void push(ExecutionHost&h,CpuRegisters&r,std::uint32_t v){r.address[7]-=4U;h.write_memory_word(P,r.address[7],static_cast<std::uint16_t>(v>>16U),M);h.write_memory_word(P,r.address[7]+2U,static_cast<std::uint16_t>(v),M);}
std::uint32_t pop(ExecutionHost&h,CpuRegisters&r){const auto hi=h.read_memory_word(P,r.address[7],M),lo=h.read_memory_word(P,r.address[7]+2U,M);r.address[7]+=4U;return(static_cast<std::uint32_t>(hi)<<16U)|lo;}
void tile(ExecutionHost&h,std::uint32_t a,std::uint16_t v){h.write_memory_word(T,a-0x00200000U,v,M);}
}

FunctionResult cpu_b_write_tilemap_strip(FunctionContext&c) noexcept
{
 if(c.host==nullptr)
  return{TranslationStatus::contract_violation,0U,c.registers.program_counter};
 auto&h=*c.host;
 auto&r=c.registers;
 auto d1=static_cast<std::uint16_t>(r.data[1]);
 const auto d7=static_cast<std::uint8_t>(r.data[7]);
 if(r.program_counter!=0x0000fa58U){
 r.address[0]=0x00200182U;d1=h.read_memory_word(P,r.address[5]+0x62U,M);r.data[1]=(r.data[1]&0xffff0000U)|d1;logic(r,d1,0x8000U);
 const auto disp=static_cast<std::int16_t>(h.read_memory_word(P,r.address[5]+0x7aU,M));r.address[0]=static_cast<std::uint32_t>(r.address[0]+disp);
 logic(r,d7,0x80U);
 if(d7==0U){r.data[2]=8U;logic(r,8U,0x80000000U);r.data[0]=0U;logic(r,0U,0x80000000U);for(unsigned n=0;n!=9U;++n){const auto offset=r.address[0]-0x00200000U;h.write_memory_word(T,offset,0U,M);h.write_memory_word(T,offset+2U,0U,M);logic(r,0U,0x80000000U);r.address[0]+=0x80U;const auto q=static_cast<std::uint16_t>(r.data[2]-1U);r.data[2]=(r.data[2]&0xffff0000U)|q;}const auto target=pop(h,r);r.program_counter=target;return FunctionResult::complete(1U,target);}
 if((d7&0x80U)!=0U){r.address[1]=0x00010520U;r.data[2]=8U;logic(r,8U,0x80000000U);r.program_counter=0x00015fe6U;(void)h.call_function(291U,1U,0x72U,1U,0x0000fa8cU,0x00015fe6U,c);return FunctionResult::complete(3U,0x00015fe6U);}
 r.address[1]=0x00010512U;r.data[2]=5U;logic(r,5U,0x80000000U);cmpb(r,d7,2U);if(static_cast<std::int8_t>(static_cast<std::uint8_t>(d7-2U))>=0){r.data[2]=6U;logic(r,6U,0x80000000U);}
 push(h,r,0x0000fa58U);r.program_counter=0x00015fe6U;const auto child=h.call_function(291U,1U,0x72U,2U,0x0000fa52U,0x00015fe6U,c);if(child.status!=TranslationStatus::complete)return child;if(r.program_counter!=0x0000fa58U)return FunctionResult::complete(5U,r.program_counter);
 }
 clear_long(h,r,T,r.address[0]-0x00200000U);r.data[0]=0U;logic(r,0U,0x80000000U);r.data[0]=(r.data[0]&0xffffff00U)|d7;logic(r,d7,0x80U);
 auto d0=static_cast<std::uint16_t>(r.data[0]);auto z=static_cast<std::uint16_t>(d0+0x30U);addw(r,d0,0x30U,z);d0=z;z=static_cast<std::uint16_t>(d0<<1U);addw(r,d0,d0,z);d0=static_cast<std::uint16_t>(z|d1);logic(r,d0,0x8000U);r.data[0]=(r.data[0]&0xffff0000U)|d0;
 tile(h,r.address[0]+0x80U,d0);logic(r,d0,0x8000U);z=static_cast<std::uint16_t>(d0+1U);addw(r,d0,1U,z);r.data[0]=(r.data[0]&0xffff0000U)|z;tile(h,r.address[0]+0x82U,z);logic(r,z,0x8000U);clear_long(h,r,T,r.address[0]+0x100U-0x00200000U);
 const auto target=pop(h,r);r.program_counter=target;return FunctionResult::complete(1U,target);
}
} // namespace gain_ground::translated
