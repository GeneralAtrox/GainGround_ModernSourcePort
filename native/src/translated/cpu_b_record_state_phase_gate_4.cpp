#include "gain_ground/contract_types.h"
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t R=2U,M=0xffffU;
std::uint16_t rw(ExecutionHost&h,std::uint32_t a){return h.read_memory_word(R,a&0x3ffffU,M);}void ww(ExecutionHost&h,std::uint32_t a,std::uint16_t v){h.write_memory_word(R,a&0x3ffffU,v,M);}
std::uint8_t rb(ExecutionHost&h,std::uint32_t a){auto p=a&0x3ffffU;bool o=p&1U;auto v=h.read_memory_word(R,p&~1U,o?0xffU:0xff00U);return static_cast<std::uint8_t>(o?v:v>>8U);}void wb(ExecutionHost&h,std::uint32_t a,std::uint8_t v){auto p=a&0x3ffffU;bool o=p&1U;h.write_memory_word(R,p&~1U,static_cast<std::uint16_t>(v)<<(o?0U:8U),o?0xffU:0xff00U);}
void logicw(CpuRegisters&r,std::uint16_t v){std::uint16_t f=r.status&0x10U;if(v&0x8000U)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}void logicb(CpuRegisters&r,std::uint8_t v){std::uint16_t f=r.status&0x10U;if(v&0x80U)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void bitz(CpuRegisters&r,bool set){if(set)r.status=static_cast<std::uint16_t>(r.status&~4U);else r.status=static_cast<std::uint16_t>(r.status|4U);}bool btst(ExecutionHost&h,CpuRegisters&r,std::uint32_t a,unsigned b){auto v=rb(h,a);bool s=(v&(1U<<b))!=0U;bitz(r,s);return s;}void bset(ExecutionHost&h,CpuRegisters&r,std::uint32_t a,unsigned b){auto v=rb(h,a);bool s=(v&(1U<<b))!=0U;wb(h,a,static_cast<std::uint8_t>(v|(1U<<b)));bitz(r,s);}void bclr(ExecutionHost&h,CpuRegisters&r,std::uint32_t a,unsigned b){auto v=rb(h,a);bool s=(v&(1U<<b))!=0U;wb(h,a,static_cast<std::uint8_t>(v&~(1U<<b)));bitz(r,s);}
void addw(CpuRegisters&r,std::uint16_t d,std::uint16_t s,std::uint16_t v){std::uint16_t f=0;if(std::uint32_t(d)+s>0xffffU)f|=0x11U;if((~(d^s)&(d^v)&0x8000U)!=0)f|=2U;if(v&0x8000U)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}void cmpw(CpuRegisters&r,std::uint16_t d,std::uint16_t s){auto v=static_cast<std::uint16_t>(d-s);std::uint16_t f=r.status&0x10U;if(s>d)f|=1U;if(((d^s)&(d^v)&0x8000U)!=0)f|=2U;if(v&0x8000U)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void push(ExecutionHost&h,CpuRegisters&r,std::uint32_t v){r.address[7]-=4U;ww(h,r.address[7],static_cast<std::uint16_t>(v>>16U));ww(h,r.address[7]+2U,static_cast<std::uint16_t>(v));}std::uint32_t pop(ExecutionHost&h,CpuRegisters&r){auto v=(static_cast<std::uint32_t>(rw(h,r.address[7]))<<16U)|rw(h,r.address[7]+2U);r.address[7]+=4U;return v;}
FunctionResult child(FunctionContext&c,std::uint32_t id,std::uint32_t site,std::uint32_t target,std::uint32_t ret){push(*c.host,c.registers,ret);c.registers.program_counter=target;return c.host->call_function(id,1U,0x72U,2U,site,target,c);}FunctionResult done(FunctionContext&c){auto t=pop(*c.host,c.registers);c.registers.program_counter=t;return FunctionResult::complete(1U,t);}FunctionResult tail(FunctionContext&c,std::uint32_t id,std::uint32_t site,std::uint32_t target){c.registers.program_counter=target;return c.host->call_function(id,1U,0x72U,1U,site,target,c);}
}

FunctionResult cpu_b_record_state_phase_gate_4(FunctionContext&c) noexcept{
 if(!c.host)return{TranslationStatus::contract_violation,0U,c.registers.program_counter};
 auto&h=*c.host;auto&r=c.registers;auto a=r.address[5];
 auto v=rb(h,a+0x3fU);logicb(r,v);if(v!=0U)return tail(c,550U,0x1c384U,0x1d9daU);v=rb(h,a+0x3eU);logicb(r,v);if(v!=0U)return tail(c,551U,0x1c38eU,0x1d9eaU);
 auto old=rw(h,a+0x54U);auto nv=static_cast<std::uint16_t>(old+1U);ww(h,a+0x54U,nv);addw(r,old,1U,nv);
 if(btst(h,r,a+0x41U,1U)){
  auto z=child(c,350U,0x1c39eU,0x1dad8U,0x1c3a2U);if(z.status!=TranslationStatus::complete||z.control!=1U)return z;
  v=rb(h,a+0x60U);logicb(r,v);if(v!=0U){z=child(c,347U,0x1c3a8U,0x1da58U,0x1c3acU);if(z.status!=TranslationStatus::complete||z.control!=1U)return z;}
  z=child(c,364U,0x1c3acU,0x1ec00U,0x1c3b0U);if(z.status!=TranslationStatus::complete||z.control!=1U)return z;
  auto d1=static_cast<std::uint16_t>(r.data[1]);ww(h,a+0x5cU,d1);logicw(r,d1);bset(h,r,a+0x40U,1U);return done(c);
 }
 if(btst(h,r,a+0x40U,7U))goto clear4;
 {auto z=child(c,337U,0x1c3c4U,0x1d402U,0x1c3c8U);if(z.status!=TranslationStatus::complete||z.control!=1U)return z;}
 if(btst(h,r,a+0x41U,5U)){r.address[0]=(static_cast<std::uint32_t>(rw(h,a+0x66U))<<16U)|rw(h,a+0x68U);auto w=rw(h,r.address[0]+0x10U);ww(h,a+0x74U,w);logicw(r,w);bclr(h,r,a+0x41U,1U);bset(h,r,a+0x41U,0U);bset(h,r,a+0x41U,2U);goto clear1;}
 {auto d0=rw(h,a+0x54U);r.data[0]=(r.data[0]&0xffff0000U)|d0;logicw(r,d0);d0=static_cast<std::uint16_t>(d0&0x1fU);r.data[0]=(r.data[0]&0xffff0000U)|d0;logicw(r,d0);if(d0==0U){auto z=child(c,351U,0x1c3daU,0x1db5cU,0x1c3deU);if(z.status!=TranslationStatus::complete||z.control!=1U)return z;goto common;}}
 if(btst(h,r,a+0x40U,5U)){wb(h,a+0x60U,0U);logicb(r,0U);auto z=child(c,347U,0x1c46cU,0x1da58U,0x1c470U);if(z.status!=TranslationStatus::complete||z.control!=1U)return z;goto clear1;}
 if(btst(h,r,a+0x40U,6U))goto clear1;
 bclr(h,r,a+0x40U,7U);if(btst(h,r,a+0x41U,4U))goto update;return done(c);
clear1:bclr(h,r,a+0x40U,1U);
clear4:bclr(h,r,a+0x41U,4U);
common:bclr(h,r,a+0x40U,0U);bset(h,r,a+0x40U,3U);bset(h,r,a+0x40U,2U);return done(c);
update:bclr(h,r,a+0x40U,1U);{auto d0=rw(h,a+0x5cU);r.data[0]=(r.data[0]&0xffff0000U)|d0;logicw(r,d0);auto other=rw(h,a+0x3aU);cmpw(r,d0,other);if(d0==other){bclr(h,r,a+0x41U,4U);return done(c);}auto d1b=rb(h,a+0x39U);r.data[1]=(r.data[1]&0xffffff00U)|d1b;logicb(r,d1b);auto d1=static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(d1b)));r.data[1]=(r.data[1]&0xffff0000U)|d1;logicw(r,d1);auto sum=static_cast<std::uint16_t>(d0+d1);r.data[0]=(r.data[0]&0xffff0000U)|sum;addw(r,d0,d1,sum);sum=static_cast<std::uint16_t>(sum&0x7ffU);r.data[0]=(r.data[0]&0xffff0000U)|sum;logicw(r,sum);ww(h,a+0x5cU,sum);logicw(r,sum);goto common;}
}
} // namespace gain_ground::translated
