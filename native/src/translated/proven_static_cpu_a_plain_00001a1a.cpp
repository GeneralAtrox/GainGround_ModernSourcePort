#include "gain_ground/contract_types.h"

#include <array>
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgram = 1U, kShared = 3U;
constexpr std::uint16_t kPalette = 9U, kPaletteMirror = 10U, kSprite = 11U, kWordMask = 0xffffU;
constexpr std::uint16_t kConditionMask = 0x001fU;
void logic_word(CpuRegisters&r,std::uint16_t v){std::uint16_t f=r.status&0x10U;if(v&0x8000U)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~kConditionMask)|f);}
void logic_long(CpuRegisters&r,std::uint32_t v){std::uint16_t f=r.status&0x10U;if(v&0x80000000U)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~kConditionMask)|f);}
void logic_byte(CpuRegisters&r,std::uint8_t v){std::uint16_t f=r.status&0x10U;if(v&0x80U)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~kConditionMask)|f);}
void prefetch(ExecutionHost&h,std::uint32_t a){(void)h.read_memory_word(kProgram,a,kWordMask);}
void push(ExecutionHost&h,CpuRegisters&r,std::uint32_t v){r.address[7]-=4U;auto o=r.address[7]&0x3ffffU;h.write_memory_word(kShared,o,static_cast<std::uint16_t>(v>>16U),kWordMask);h.write_memory_word(kShared,o+2U,static_cast<std::uint16_t>(v),kWordMask);}
std::uint32_t pop(ExecutionHost&h,CpuRegisters&r){auto o=r.address[7]&0x3ffffU;auto v=(static_cast<std::uint32_t>(h.read_memory_word(kShared,o,kWordMask))<<16U)|h.read_memory_word(kShared,o+2U,kWordMask);r.address[7]+=4U;return v;}
FunctionResult call(FunctionContext&c,std::uint32_t id,std::uint32_t site,std::uint32_t target,std::uint32_t ret){push(*c.host,c.registers,ret);prefetch(*c.host,target);prefetch(*c.host,target+2U);c.registers.program_counter=target;return c.host->call_function(id,0U,0xffU,2U,site,target,c);}
}

FunctionResult proven_static_cpu_a_plain_00001a1a(FunctionContext&c) noexcept
{
 if(!c.host||c.registers.program_counter!=0x1a1aU)
  return{TranslationStatus::contract_violation,0U,c.registers.program_counter};
 auto&h=*c.host;auto&r=c.registers;
 prefetch(h,0x1a1eU);prefetch(h,0x1a20U);prefetch(h,0x1a22U);
 h.write_hardware(2U,0U,0xffU,0x1a1aU,0x80001cU,0x0404U,0x00ffU);logic_byte(r,4U);
 prefetch(h,0x1a24U);
 for(const auto item:{std::array<std::uint32_t,4>{5U,0x1a22U,0xb98U,0x1a26U},std::array<std::uint32_t,4>{6U,0x1a26U,0xbacU,0x1a2aU},std::array<std::uint32_t,4>{7U,0x1a2aU,0xbc0U,0x1a2eU},std::array<std::uint32_t,4>{8U,0x1a2eU,0xbd4U,0x1a32U},std::array<std::uint32_t,4>{9U,0x1a32U,0xbe8U,0x1a36U}}){auto z=call(c,item[0],item[1],item[2],item[3]);if(z.status!=TranslationStatus::complete)return z;}
 prefetch(h,0x1a3aU);prefetch(h,0x1a3cU);prefetch(h,0x1a3eU);
 h.write_memory_word(kSprite,0U,0xffffU,kWordMask);logic_word(r,0xffffU);
 prefetch(h,0x1a40U);prefetch(h,0x1a42U);prefetch(h,0x1a44U);
 r.address[1]=0x400000U;prefetch(h,0x1a46U);prefetch(h,0x1a48U);
 r.data[2]=(r.data[2]&0xffff0000U)|0xffU;logic_word(r,0xffU);
 prefetch(h,0x1a4aU);prefetch(h,0x1a4cU);r.address[3]=0xb58U;
 prefetch(h,0x1a4eU);prefetch(h,0x1a50U);prefetch(h,0x1a52U);
 for(;;){r.address[2]=r.address[3];r.data[1]=7U;logic_long(r,7U);for(;;){auto hi=h.read_memory_word(kProgram,r.address[2],kWordMask);auto lo=h.read_memory_word(kProgram,r.address[2]+2U,kWordMask);r.address[2]+=4U;h.write_memory_word(kPalette,r.address[1]-0x400000U,hi,kWordMask);h.write_memory_word(kPalette,r.address[1]-0x400000U+2U,lo,kWordMask);r.address[1]+=4U;logic_long(r,(static_cast<std::uint32_t>(hi)<<16U)|lo);prefetch(h,0x1a54U);auto d1=static_cast<std::uint16_t>(r.data[1]-1U);r.data[1]=(r.data[1]&0xffff0000U)|d1;prefetch(h,0x1a50U);if(d1==0xffffU)break;prefetch(h,0x1a52U);}prefetch(h,0x1a56U);prefetch(h,0x1a58U);auto d2=static_cast<std::uint16_t>(r.data[2]-1U);r.data[2]=(r.data[2]&0xffff0000U)|d2;prefetch(h,0x1a4cU);if(d2==0xffffU)break;prefetch(h,0x1a4eU);prefetch(h,0x1a50U);prefetch(h,0x1a52U);}
 prefetch(h,0x1a5aU);prefetch(h,0x1a5cU);prefetch(h,0x1a5eU);prefetch(h,0x1a60U);prefetch(h,0x1a62U);
 h.write_memory_word(kPaletteMirror,0x1aU,0U,0x00ffU);logic_byte(r,0U);prefetch(h,0x1a64U);auto t=pop(h,r);prefetch(h,t);prefetch(h,t+2U);r.program_counter=t;return FunctionResult::complete(1U,t);
}
}
