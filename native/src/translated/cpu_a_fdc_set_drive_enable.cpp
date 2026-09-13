#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgram=1U,kShared=3U,kWordMask=0xffffU,kCc=0x001fU;
void pf(ExecutionHost&h,std::uint32_t a){(void)h.read_memory_word(kProgram,a,kWordMask);}
void logicw(CpuRegisters&r,std::uint16_t v){std::uint16_t f=r.status&0x10U;if(v&0x8000U)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~kCc)|f);}
void logicb(CpuRegisters&r,std::uint8_t v){std::uint16_t f=r.status&0x10U;if(v&0x80U)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~kCc)|f);}
std::uint8_t rh(ExecutionHost&h,std::uint32_t pc,std::uint32_t a){return static_cast<std::uint8_t>(h.read_hardware(1U,0U,0xffU,pc,a&~1U,0x00ffU));}
void wh(ExecutionHost&h,std::uint32_t pc,std::uint32_t a,std::uint8_t v){h.write_hardware(2U,0U,0xffU,pc,a&~1U,static_cast<std::uint16_t>(v)*0x0101U,0x00ffU);}
void ws(ExecutionHost&h,std::uint32_t a,std::uint16_t v){h.write_memory_word(kShared,a&0x3ffffU,v,kWordMask);}
std::uint16_t rs(ExecutionHost&h,std::uint32_t a){return h.read_memory_word(kShared,a&0x3ffffU,kWordMask);}
void pushl(ExecutionHost&h,CpuRegisters&r,std::uint32_t v){r.address[7]-=4U;ws(h,r.address[7],static_cast<std::uint16_t>(v>>16U));ws(h,r.address[7]+2U,static_cast<std::uint16_t>(v));}
std::uint32_t popl(ExecutionHost&h,CpuRegisters&r){auto v=(static_cast<std::uint32_t>(rs(h,r.address[7]))<<16U)|rs(h,r.address[7]+2U);r.address[7]+=4U;return v;}
FunctionResult call(FunctionContext&c,std::uint32_t id,std::uint32_t site,std::uint32_t target,std::uint32_t ret){auto&h=*c.host;auto&r=c.registers;pushl(h,r,ret);pf(h,target);pf(h,target+2U);r.program_counter=target;return h.call_function(id,0U,0xffU,2U,site,target,c);}
bool complete(FunctionResult const&r){return r.status==TranslationStatus::complete;}
}

namespace {
FunctionResult drive_error_entry(FunctionContext &c)
{
 auto &h=*c.host;auto &r=c.registers;FunctionResult z{};
 r.address[7]=0U;pf(h,0x1994U);r.status=0x2600U;pf(h,0x1998U);r.address[7]-=2U;ws(h,r.address[7],static_cast<std::uint16_t>(r.data[0]));pf(h,0x199aU);pf(h,0x199cU);
 z=call(c,446U,0x199aU,0x1a1aU,0x199eU);if(!complete(z))return z;
 r.data[0]=(r.data[0]&0xffff0000U)|rs(h,r.address[7]);r.address[7]+=2U;logicw(r,static_cast<std::uint16_t>(r.data[0]));
 r.address[0]=0x661eU;auto doubled=static_cast<std::uint16_t>(r.data[0]+r.data[0]);r.data[0]=(r.data[0]&0xffff0000U)|doubled;logicw(r,doubled);r.address[0]=static_cast<std::uint32_t>(r.address[0]+static_cast<std::int16_t>(doubled));r.address[0]=static_cast<std::uint32_t>(r.address[0]+static_cast<std::int16_t>(h.read_memory_word(kProgram,r.address[0],kWordMask)));
 r.address[2]=0x280080U;r.data[7]=4U;r.address[3]=0x200c3cU;z=call(c,27U,0x19bcU,0x1a82U,0x19c0U);if(!complete(z))return z;r.address[3]=0x200dbcU;z=call(c,27U,0x19c6U,0x1a82U,0x19caU);if(!complete(z))return z;
 // Trap handlers are already native; their exact vector/stack behavior is delegated.
 z=call(c,0U,0x19caU,0x402U,0x19ccU);if(!complete(z))return z;r.data[0]=0U;z=call(c,52U,0x19ceU,0x8008aU,0x19d0U);if(!complete(z))return z;
 auto flag=rs(h,0xa008U);ws(h,0xa008U,static_cast<std::uint16_t>(flag&0x7fffU));r.data[2]=1U;r.data[3]=(r.data[3]&0xffff0000U)|0x80U;
 do{z=call(c,445U,0x19deU,0x1a04U,0x19e2U);if(!complete(z))return z;r.data[2]=(r.data[2]&0xffff0000U)|static_cast<std::uint16_t>(r.data[2]+8U);r.data[3]=(r.data[3]&0xffff0000U)|static_cast<std::uint16_t>(r.data[3]-1U);}while(static_cast<std::uint16_t>(r.data[3])!=0xffffU);
 ws(h,0xa008U,0x0080U);r.data[2]=0U;r.data[3]=(r.data[3]&0xffff0000U)|0x40U;
 do{z=call(c,445U,0x19f6U,0x1a04U,0x19faU);if(!complete(z))return z;r.data[3]=(r.data[3]&0xffff0000U)|static_cast<std::uint16_t>(r.data[3]-1U);}while(static_cast<std::uint16_t>(r.data[3])!=0xffffU);
 r.program_counter=0x19d0U;return FunctionResult::complete(3U,0x19d0U);
}
}

FunctionResult cpu_a_fdc_set_drive_enable(FunctionContext&c) noexcept
{
 if(!c.host||(c.registers.program_counter!=0x18a4U && c.registers.program_counter!=0x1990U))return{TranslationStatus::contract_violation,0U,c.registers.program_counter};
 if(c.registers.program_counter==0x1990U)return drive_error_entry(c);
 auto&h=*c.host;auto&r=c.registers;
 logicw(r,static_cast<std::uint16_t>(r.data[0]));pf(h,0x18a8U);
 if(static_cast<std::uint16_t>(r.data[0])==0U){
  pf(h,0x18aaU);pf(h,0x18acU);pf(h,0x18aeU);pf(h,0x18b0U);wh(h,0x18a8U,0xb0000bU,0U);logicb(r,0U);pf(h,0x18b2U);
  auto t=popl(h,r);pf(h,t);pf(h,t+2U);r.program_counter=t;return FunctionResult::complete(1U,t);
 }
 pf(h,0x18b2U);pf(h,0x18b4U);pf(h,0x18b6U);pf(h,0x18b8U);pf(h,0x18baU);wh(h,0x18b2U,0xb0000bU,10U);logicb(r,10U);pf(h,0x18bcU);
 auto z=call(c,19U,0x18baU,0x17ceU,0x18beU);if(!complete(z))return z;
 pf(h,0x18c2U);pf(h,0x18c4U);pf(h,0x18c6U);auto ready=rh(h,0x18beU,0xb00009U);r.status=static_cast<std::uint16_t>((r.status&~4U)|((ready&0x80U)==0U?4U:0U));pf(h,0x18c8U);
 if((r.status&4U)==0U){pf(h,0x1920U);logicw(r,static_cast<std::uint16_t>(r.data[0]));pf(h,0x1922U);pf(h,0x1924U);auto t=popl(h,r);pf(h,t);pf(h,t+2U);r.program_counter=t;return FunctionResult::complete(1U,t);}

 pf(h,0x18caU);pf(h,0x18ccU);pf(h,0x18ceU);r.address[5]=0xb00001U;
 for(;;){pf(h,0x18d0U);pf(h,0x18d2U);pf(h,0x18d4U);auto s=rh(h,0x18ceU,r.address[5]);r.status=static_cast<std::uint16_t>((r.status&~4U)|((s&1U)==0U?4U:0U));pf(h,0x18d6U);if(r.status&4U)break;pf(h,0x18ceU);}
 pf(h,0x18d8U);pf(h,0x18daU);pf(h,0x18dcU);wh(h,0x18d6U,r.address[5],0xd0U);logicb(r,0xd0U);
 for(;;){pf(h,0x18deU);pf(h,0x18e0U);pf(h,0x18e2U);auto s=rh(h,0x18dcU,r.address[5]);r.status=static_cast<std::uint16_t>((r.status&~4U)|((s&1U)==0U?4U:0U));pf(h,0x18e4U);if(r.status&4U)break;pf(h,0x18dcU);}
 pf(h,0x18e6U);pf(h,0x18e8U);pf(h,0x18eaU);wh(h,0x18e4U,r.address[5]+6U,0xc0U);logicb(r,0xc0U);
 pf(h,0x18ecU);pf(h,0x18eeU);pf(h,0x18f0U);wh(h,0x18eaU,r.address[5],0xfeU);logicb(r,0xfeU);
 for(;;){pf(h,0x18f2U);pf(h,0x18f4U);pf(h,0x18f6U);auto s=rh(h,0x18f0U,r.address[5]);r.status=static_cast<std::uint16_t>((r.status&~4U)|((s&1U)==0U?4U:0U));pf(h,0x18f8U);if(r.status&4U)break;pf(h,0x18f0U);}
 pf(h,0x18faU);pf(h,0x18fcU);pf(h,0x18feU);wh(h,0x18f8U,r.address[5]+6U,0x8aU);logicb(r,0x8aU);
 pf(h,0x1900U);pf(h,0x1902U);pf(h,0x1904U);wh(h,0x18feU,r.address[5],0xfdU);logicb(r,0xfdU);
 pf(h,0x1906U);pf(h,0x1908U);pf(h,0x190aU);wh(h,0x1904U,r.address[5]+10U,0x1aU);logicb(r,0x1aU);pf(h,0x190cU);
 z=call(c,21U,0x190aU,0x17e4U,0x190eU);if(!complete(z))return z;
 r.data[7]=(r.data[7]&0xffff0000U)|2U;logicw(r,2U);pf(h,0x1912U);pf(h,0x1914U);
 z=call(c,444U,0x1912U,0x1862U,0x1916U);if(!complete(z))return z;
 pf(h,0x1918U);pf(h,0x191aU);pf(h,0x191cU);auto result=rh(h,0x1916U,r.address[5]+8U);r.status=static_cast<std::uint16_t>((r.status&~4U)|((result&0x80U)==0U?4U:0U));pf(h,0x191eU);
 if((r.status&4U)==0U){pf(h,0x1920U);logicw(r,static_cast<std::uint16_t>(r.data[0]));pf(h,0x1922U);pf(h,0x1924U);auto t=popl(h,r);pf(h,t);pf(h,t+2U);r.program_counter=t;return FunctionResult::complete(1U,t);}

 // Original non-returning recovery continuation, proven by the terminal fixture.
 pf(h,0x1954U);r.data[0]=(r.data[0]&0xffff0000U);logicw(r,0U);pf(h,0x1958U);pf(h,0x195aU);pf(h,0x1990U);
 return drive_error_entry(c);
}
} // namespace gain_ground::translated
