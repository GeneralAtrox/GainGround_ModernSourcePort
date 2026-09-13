#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 3U, kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;
void pf(ExecutionHost&h,std::uint32_t a){(void)h.read_memory_word(kRegion,a&kAddressMask,kWordMask);}
void ww(ExecutionHost&h,std::uint32_t a,std::uint16_t v){h.write_memory_word(kRegion,a&kAddressMask,v,kWordMask);}
void wl(ExecutionHost&h,std::uint32_t a,std::uint32_t v){ww(h,a,static_cast<std::uint16_t>(v>>16U));ww(h,a+2U,static_cast<std::uint16_t>(v));}
void logicl(CpuRegisters&r,std::uint32_t v){std::uint16_t f=r.status&0x10U;if(v&0x80000000U)f|=8U;if(v==0)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void logicw(CpuRegisters&r,std::uint16_t v){std::uint16_t f=r.status&0x10U;if(v&0x8000U)f|=8U;if(v==0)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void logicb(CpuRegisters&r,std::uint8_t v){std::uint16_t f=r.status&0x10U;if(v&0x80U)f|=8U;if(v==0)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void addb(CpuRegisters&r,std::uint8_t a,std::uint8_t v){std::uint16_t f{};if(v&0x80U)f|=8U;if(v==0)f|=4U;if(((~(a^1U))&(a^v)&0x80U)!=0)f|=2U;if(a==0xffU)f|=0x11U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void addw(CpuRegisters&r,std::uint16_t a,std::uint16_t v){std::uint16_t f{};if(v&0x8000U)f|=8U;if(v==0)f|=4U;if(((~(a^4U))&(a^v)&0x8000U)!=0)f|=2U;if(static_cast<std::uint32_t>(a)+4U>0xffffU)f|=0x11U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
FunctionResult call(FunctionContext&c,std::uint32_t id,std::uint32_t site,std::uint32_t target,std::uint32_t ret){auto&h=*c.host;auto&r=c.registers;r.address[7]-=4U;wl(h,r.address[7],ret);pf(h,target);pf(h,target+2U);r.program_counter=target;return h.call_function(id,0U,0xffU,2U,site,target,c);}
std::uint32_t pop(ExecutionHost&h,CpuRegisters&r){auto a=r.address[7]&kAddressMask;auto v=(static_cast<std::uint32_t>(h.read_memory_word(kRegion,a,kWordMask))<<16U)|h.read_memory_word(kRegion,a+2U,kWordMask);r.address[7]+=4U;return v;}
} // namespace

FunctionResult proven_static_cpu_a_plain_00084090(FunctionContext&c) noexcept
{
 if(!c.host)
  return{TranslationStatus::contract_violation,0U,c.registers.program_counter};
 auto&h=*c.host;auto&r=c.registers;
 pf(h,0x84094U);r.address[4]=r.address[6]+0x180U;r.data[0]=0x11U;logicl(r,r.data[0]);pf(h,0x84096U);r.data[1]=0U;logicl(r,0);pf(h,0x84098U);r.data[2]=0x50U;logicl(r,r.data[2]);pf(h,0x8409aU);
 for(;;){pf(h,0x8409cU);ww(h,r.address[4],static_cast<std::uint16_t>(r.data[1]));logicw(r,static_cast<std::uint16_t>(r.data[1]));r.address[4]+=static_cast<std::int16_t>(static_cast<std::uint16_t>(r.data[2]));pf(h,0x8409eU);pf(h,0x840a0U);auto n=static_cast<std::uint16_t>(r.data[0]);r.data[0]=(r.data[0]&0xffff0000U)|static_cast<std::uint16_t>(n-1U);if(n==0)break;pf(h,0x8409aU);}
 pf(h,0x8409aU);pf(h,0x840a2U);r.data[2]=0x1fU;logicl(r,r.data[2]);pf(h,0x840a4U);r.data[0]=0x60U;logicl(r,r.data[0]);pf(h,0x840a6U);r.data[1]=0x7fU;logicl(r,r.data[1]);
 pf(h,0x840a8U);pf(h,0x840aaU);
 for(;;){auto z=call(c,87U,0x840a8U,0x8410eU,0x840acU);if(z.status!=TranslationStatus::complete)return z;auto b=static_cast<std::uint8_t>(r.data[0]),v=static_cast<std::uint8_t>(b+1U);r.data[0]=(r.data[0]&0xffffff00U)|v;addb(r,b,v);pf(h,0x840b0U);auto n=static_cast<std::uint16_t>(r.data[2]);r.data[2]=(r.data[2]&0xffff0000U)|static_cast<std::uint16_t>(n-1U);if(n==0)break;pf(h,0x840a8U);pf(h,0x840aaU);}
 pf(h,0x840a8U);pf(h,0x840b2U);r.data[2]=0x1fU;logicl(r,r.data[2]);pf(h,0x840b4U);pf(h,0x840b6U);r.data[0]=(r.data[0]&0xffffff00U)|0xe0U;logicb(r,0xe0U);
 pf(h,0x840b8U);pf(h,0x840baU);
 for(;;){auto z=call(c,87U,0x840b8U,0x8410eU,0x840bcU);if(z.status!=TranslationStatus::complete)return z;auto b=static_cast<std::uint8_t>(r.data[0]),v=static_cast<std::uint8_t>(b+1U);r.data[0]=(r.data[0]&0xffffff00U)|v;addb(r,b,v);pf(h,0x840c0U);auto n=static_cast<std::uint16_t>(r.data[2]);r.data[2]=(r.data[2]&0xffff0000U)|static_cast<std::uint16_t>(n-1U);if(n==0)break;pf(h,0x840b8U);pf(h,0x840baU);}
 pf(h,0x840b8U);pf(h,0x840c2U);pf(h,0x840c4U);auto z=call(c,97U,0x840c2U,0x842e4U,0x840c6U);if(z.status!=TranslationStatus::complete)return z;
 r.data[2]=3U;logicl(r,3U);r.data[0]=0U;logicl(r,0U);
 pf(h,0x840caU);pf(h,0x840ccU);
 for(;;){pf(h,0x840ceU);pf(h,0x840d0U);ww(h,r.address[6]+0x30U+static_cast<std::int16_t>(static_cast<std::uint16_t>(r.data[0])),0x80U);logicw(r,0x80U);auto b=static_cast<std::uint16_t>(r.data[0]),v=static_cast<std::uint16_t>(b+4U);r.data[0]=(r.data[0]&0xffff0000U)|v;addw(r,b,v);pf(h,0x840d2U);pf(h,0x840d4U);auto n=static_cast<std::uint16_t>(r.data[2]);r.data[2]=(r.data[2]&0xffff0000U)|static_cast<std::uint16_t>(n-1U);if(n==0)break;pf(h,0x840caU);pf(h,0x840ccU);}
 pf(h,0x840caU);pf(h,0x840d6U);pf(h,0x840d8U);r.address[0]=r.address[6];pf(h,0x840daU);r.data[2]=0x23U;logicl(r,r.data[2]);pf(h,0x840dcU);r.data[0]=0U;logicl(r,0U);pf(h,0x840deU);
 for(;;){pf(h,0x840e0U);wl(h,r.address[0],r.data[0]);r.address[0]+=4U;logicl(r,r.data[0]);pf(h,0x840e2U);auto n=static_cast<std::uint16_t>(r.data[2]);r.data[2]=(r.data[2]&0xffff0000U)|static_cast<std::uint16_t>(n-1U);if(n==0)break;pf(h,0x840deU);}
 pf(h,0x840deU);pf(h,0x840e4U);pf(h,0x840e6U);auto target=pop(h,r);pf(h,target);pf(h,target+2U);r.program_counter=target;return FunctionResult::complete(1U,target);
}
} // namespace gain_ground::translated
