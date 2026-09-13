#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t R=2U,M=0xffffU;
struct B{std::uint32_t o;std::uint16_t m;unsigned s;};
B bl(std::uint32_t a){const bool odd=a&1U;return{a&~1U,static_cast<std::uint16_t>(odd?0xffU:0xff00U),odd?0U:8U};}
std::uint8_t rb(ExecutionHost&h,std::uint32_t a){const auto b=bl(a);return static_cast<std::uint8_t>(h.read_memory_word(R,b.o,b.m)>>b.s);}
void wb(ExecutionHost&h,std::uint32_t a,std::uint8_t v){const auto b=bl(a);h.write_memory_word(R,b.o,static_cast<std::uint16_t>(v)<<b.s,b.m);}
void logic(CpuRegisters&r,std::uint32_t v,std::uint32_t s,std::uint32_t m){std::uint16_t f=r.status&0x10U;if(v&s)f|=8U;if((v&m)==0U)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void addw(CpuRegisters&r,std::uint16_t l,std::uint16_t q,std::uint16_t v){const auto wide=static_cast<std::uint32_t>(l)+q;std::uint16_t f{};if(v&0x8000U)f|=8U;if(v==0U)f|=4U;if(((~(l^q))&(l^v)&0x8000U)!=0U)f|=2U;if(wide>0xffffU)f|=0x11U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void subb(CpuRegisters&r,std::uint8_t l,std::uint8_t q,std::uint8_t v){std::uint16_t f{};if(v&0x80U)f|=8U;if(v==0U)f|=4U;if(((l^q)&(l^v)&0x80U)!=0U)f|=2U;if(q>l)f|=0x11U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void cmpw(CpuRegisters&r,std::uint16_t l,std::uint16_t q){const auto v=static_cast<std::uint16_t>(l-q);std::uint16_t f=r.status&0x10U;if(v&0x8000U)f|=8U;if(v==0U)f|=4U;if(((l^q)&(l^v)&0x8000U)!=0U)f|=2U;if(q>l)f|=1U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
std::uint32_t rl(ExecutionHost&h,std::uint32_t a){const auto hi=h.read_memory_word(R,a,M),lo=h.read_memory_word(R,a+2U,M);return(static_cast<std::uint32_t>(hi)<<16U)|lo;}
void wl(ExecutionHost&h,std::uint32_t a,std::uint32_t v){h.write_memory_word(R,a,static_cast<std::uint16_t>(v>>16U),M);h.write_memory_word(R,a+2U,static_cast<std::uint16_t>(v),M);}
void push(ExecutionHost&h,CpuRegisters&r,std::uint32_t v){r.address[7]-=4U;wl(h,r.address[7],v);}
std::uint32_t pop(ExecutionHost&h,CpuRegisters&r){const auto v=rl(h,r.address[7]);r.address[7]+=4U;return v;}
void mw(ExecutionHost&h,CpuRegisters&r,std::uint32_t a,std::uint16_t v){h.write_memory_word(R,a,v,M);logic(r,v,0x8000U,0xffffU);}
void mb(ExecutionHost&h,CpuRegisters&r,std::uint32_t a,std::uint8_t v){wb(h,a,v);logic(r,v,0x80U,0xffU);}
void ml(ExecutionHost&h,CpuRegisters&r,std::uint32_t a,std::uint32_t v){wl(h,a,v);logic(r,v,0x80000000U,0xffffffffU);}
void cw(ExecutionHost&h,CpuRegisters&r,std::uint32_t a){(void)h.read_memory_word(R,a,M);h.write_memory_word(R,a,0U,M);logic(r,0U,0x8000U,0xffffU);}
} // namespace

FunctionResult cpu_b_initialize_object_descriptor_tables(FunctionContext&context) noexcept
{
 if(context.host==nullptr)return{TranslationStatus::contract_violation,0U,context.registers.program_counter};
 auto&h=*context.host;auto&r=context.registers;r.address[1]=0x2680U;r.address[0]=0x2700U;r.data[2]=5U;logic(r,5U,0x80000000U,0xffffffffU);
 auto d2=std::uint16_t{5};
 for(;;){const auto a1=r.address[1];
  mb(h,r,a1+1U,4U);ml(h,r,a1+2U,0x00013756U);mb(h,r,a1+0xbU,5U);mw(h,r,a1+0x10U,0x2f2fU);
  cw(h,r,a1+0x42U);cw(h,r,a1+0x44U);cw(h,r,a1+0x46U);mw(h,r,a1+0x48U,0x96U);mw(h,r,a1+0x6aU,0x900U);
  r.data[1]=0U;logic(r,0U,0x80000000U,0xffffffffU);auto d1=std::uint16_t{0};
  r.data[0]=(r.data[0]&0xffff0000U)|2U;logic(r,2U,0x80000000U,0xffffffffU);auto d0=std::uint16_t{2};
  for(;;){const auto a0=r.address[0];mb(h,r,a0+1U,4U);ml(h,r,a0+2U,0x00013a1eU);mb(h,r,a0+0xbU,0U);mw(h,r,a0+0x10U,0x2f2fU);mw(h,r,a0+0x36U,static_cast<std::uint16_t>(r.address[1]));mw(h,r,a0+0x38U,d1);r.address[0]+=0x80U;const auto nd1=static_cast<std::uint16_t>(d1+6U);addw(r,d1,6U,nd1);d1=nd1;r.data[1]=(r.data[1]&0xffff0000U)|d1;d0=static_cast<std::uint16_t>(d0-1U);r.data[0]=(r.data[0]&0xffff0000U)|d0;if(d0==0xffffU)break;}
  r.address[1]+=0x200U;r.address[0]=r.address[1]+0x80U;cmpw(r,d2,1U);if(d2==1U){r.address[1]+=0x180U;r.address[0]=r.address[1]+0x80U;}d2=static_cast<std::uint16_t>(d2-1U);r.data[2]=(r.data[2]&0xffff0000U)|d2;if(d2==0xffffU)break;
 }
 r.address[6]=0x2680U;auto d0=h.read_memory_word(R,0x0c02U,M);r.data[0]=(r.data[0]&0xffff0000U)|d0;logic(r,d0,0x8000U,0xffffU);cmpw(r,d0,0x1eU);
 if(static_cast<std::int16_t>(d0-0x1eU)<0){auto v=static_cast<std::uint16_t>(d0+d0);addw(r,d0,d0,v);d0=v;r.data[0]=(r.data[0]&0xffff0000U)|d0;auto d1=d0;r.data[1]=(r.data[1]&0xffff0000U)|d1;logic(r,d1,0x8000U,0xffffU);v=static_cast<std::uint16_t>(d0+d0);addw(r,d0,d0,v);d0=v;r.data[0]=(r.data[0]&0xffff0000U)|d0;v=static_cast<std::uint16_t>(d0+d1);addw(r,d0,d1,v);d0=v;r.data[0]=(r.data[0]&0xffff0000U)|d0;
  auto b=rb(h,0x821U);r.data[1]=(r.data[1]&0xffffff00U)|b;logic(r,b,0x80U,0xffU);auto nb=static_cast<std::uint8_t>(b-1U);subb(r,b,1U,nb);b=nb;r.data[1]=(r.data[1]&0xffffff00U)|b;if(b!=0U){v=static_cast<std::uint16_t>(d0+2U);addw(r,d0,2U,v);d0=v;r.data[0]=(r.data[0]&0xffff0000U)|d0;nb=static_cast<std::uint8_t>(b-1U);subb(r,b,1U,nb);b=nb;r.data[1]=(r.data[1]&0xffffff00U)|b;if(b!=0U){v=static_cast<std::uint16_t>(d0+2U);addw(r,d0,2U,v);d0=v;r.data[0]=(r.data[0]&0xffff0000U)|d0;}}
  r.address[0]=0x13aa8U;const auto disp=h.read_memory_word(R,static_cast<std::uint32_t>(static_cast<std::int64_t>(r.address[0])+static_cast<std::int16_t>(d0)),M);r.address[0]=static_cast<std::uint32_t>(static_cast<std::int64_t>(r.address[0])+static_cast<std::int16_t>(disp));auto d7=h.read_memory_word(R,r.address[0],M);r.address[0]+=2U;r.data[7]=(r.data[7]&0xffff0000U)|d7;logic(r,d7,0x8000U,0xffffU);
  while((d7&0x8000U)==0U){push(h,r,0x136eaU);r.program_counter=0x136f4U;const auto child=h.call_function(267U,1U,0x72U,2U,0x136e6U,0x136f4U,context);if(child.status!=TranslationStatus::complete||child.control!=1U)return child;r.address[6]+=0x200U;d7=static_cast<std::uint16_t>(d7-1U);r.data[7]=(r.data[7]&0xffff0000U)|d7;if(d7==0xffffU)break;}
 }
 const auto target=pop(h,r);r.program_counter=target;return FunctionResult::complete(1U,target);
}
} // namespace gain_ground::translated
