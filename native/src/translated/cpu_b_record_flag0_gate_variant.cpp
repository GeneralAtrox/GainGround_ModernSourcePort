#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
FunctionResult cpu_b_record_flag0_gate(FunctionContext&) noexcept;
namespace {
constexpr std::uint16_t R=2U,W=0xffffU;
struct B{std::uint32_t a;std::uint16_t m;unsigned s;};
B bl(std::uint32_t a){bool o=a&1U;return{a&~1U,static_cast<std::uint16_t>(o?0x00ffU:0xff00U),o?0U:8U};}
std::uint16_t rw(ExecutionHost&h,std::uint32_t a){return h.read_memory_word(R,a,W);}void ww(ExecutionHost&h,std::uint32_t a,std::uint16_t v){h.write_memory_word(R,a,v,W);}
std::uint8_t rb(ExecutionHost&h,std::uint32_t a){auto x=bl(a);return static_cast<std::uint8_t>(h.read_memory_word(R,x.a,x.m)>>x.s);}
void push(ExecutionHost&h,CpuRegisters&r,std::uint32_t v){r.address[7]-=4U;ww(h,r.address[7],static_cast<std::uint16_t>(v>>16U));ww(h,r.address[7]+2U,static_cast<std::uint16_t>(v));}
void logic(CpuRegisters&r,std::uint32_t v,std::uint32_t sign,std::uint32_t mask){std::uint16_t f=r.status&0x10U;if(v&sign)f|=8U;if((v&mask)==0)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}void bitz(CpuRegisters&r,bool z){r.status=static_cast<std::uint16_t>((r.status&~4U)|(z?4U:0U));}
void wordf(CpuRegisters&r,std::uint16_t v,bool c=false,bool ov=false){std::uint16_t f=c?0x11U:0U;if(ov)f|=2U;if(v&0x8000U)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void cmpf(CpuRegisters&r,std::uint16_t l,std::uint16_t q,std::uint16_t v){std::uint16_t f=r.status&0x10U;if(l<q)f|=1U;if(((l^q)&(l^v)&0x8000U)!=0)f|=2U;if(v&0x8000U)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
FunctionResult call(FunctionContext&c,std::uint32_t site,std::uint32_t target,std::uint32_t ret,std::uint32_t id){auto&h=*c.host;auto&r=c.registers;push(h,r,ret);r.program_counter=target;return h.call_function(id,1U,0x72U,2U,site,target,c);}
}

FunctionResult cpu_b_record_flag0_gate_variant(FunctionContext&c) noexcept
{
 if(!c.host)return{TranslationStatus::contract_violation,0U,c.registers.program_counter};
 auto&h=*c.host;auto&r=c.registers;const auto a=r.address[5];
 const auto tail=[&](std::uint32_t id,std::uint32_t site,std::uint32_t target)->FunctionResult{r.program_counter=target;return h.call_function(id,1U,0x72U,1U,site,target,c);};
 const auto add_coordinate=[&](unsigned shift){auto d0=rw(h,a+0x5cU);r.data[0]=(r.data[0]&0xffff0000U)|d0;logic(r,d0,0x8000U,0xffffU);d0=static_cast<std::uint16_t>(d0+0x80U);r.data[0]=(r.data[0]&0xffff0000U)|d0;wordf(r,d0);d0&=0x0700U;r.data[0]=(r.data[0]&0xffff0000U)|d0;logic(r,d0,0x8000U,0xffffU);d0=static_cast<std::uint16_t>(d0>>8U);r.data[0]=(r.data[0]&0xffff0000U)|d0;wordf(r,d0);const auto before_subq=d0;d0=static_cast<std::uint16_t>(d0-1U);r.data[0]=(r.data[0]&0xffff0000U)|d0;wordf(r,d0,before_subq<1U,before_subq==0x8000U);if(static_cast<std::int16_t>(d0)>=0){auto d2=static_cast<std::uint16_t>(r.data[7]);r.data[2]=(r.data[2]&0xffff0000U)|d2;logic(r,d2,0x8000U,0xffffU);d2=static_cast<std::uint16_t>(d2<<shift);r.data[2]=(r.data[2]&0xffff0000U)|d2;wordf(r,d2);do{auto d1=static_cast<std::uint16_t>(r.data[1]);auto wide=static_cast<std::uint32_t>(d1)+d2;auto out=static_cast<std::uint16_t>(wide);r.data[1]=(r.data[1]&0xffff0000U)|out;wordf(r,out,wide>0xffffU,((~(d1^d2))&(d1^out)&0x8000U)!=0);d0=static_cast<std::uint16_t>(d0-1U);}while(d0!=0xffffU);r.data[0]=(r.data[0]&0xffff0000U)|d0;}};
 if(r.program_counter==0x0001e3a6U){
  auto x=call(c,0x1e3a6U,0x1ebf0U,0x1e3aaU,363U);if(x.status!=TranslationStatus::complete||x.control!=1U)return x;
  auto shared_flags=rb(h,a+0x41U);bitz(r,(shared_flags&2U)==0);bool first=(shared_flags&2U)==0;
  if(!first){shared_flags=rb(h,a+0x41U);bitz(r,(shared_flags&1U)==0);first=(shared_flags&1U)!=0;}
  if(first){r.data[1]=0;logic(r,0,0x8000U,0xffffU);add_coordinate(2U);auto phase=rw(h,a+0x56U);auto next=static_cast<std::uint16_t>(phase+1U);ww(h,a+0x56U,next);wordf(r,next);auto d0=rw(h,a+0x56U);r.data[0]=(r.data[0]&0xffff0000U)|d0;logic(r,d0,0x8000U,0xffffU);d0&=8U;r.data[0]=(r.data[0]&0xffff0000U)|d0;logic(r,d0,0x8000U,0xffffU);if(d0==0U)return tail(554U,0x1e3e4U,0x1e1f2U);auto d1=static_cast<std::uint16_t>((r.data[7]&0xffffU)<<5U);r.data[1]=(r.data[1]&0xffff0000U)|d1;wordf(r,d1);add_coordinate(1U);return tail(554U,0x1e408U,0x1e1f2U);}
  auto d1=static_cast<std::uint16_t>((r.data[7]&0xffffU)<<5U);r.data[1]=(r.data[1]&0xffff0000U)|d1;wordf(r,d1);add_coordinate(1U);const auto state=rw(h,a+0x74U);const auto diff=static_cast<std::uint16_t>(state-3U);cmpf(r,state,3U,diff);if(static_cast<std::int16_t>(state)>3)return tail(554U,0x1e432U,0x1e1f2U);d1=static_cast<std::uint16_t>((r.data[1]&0xffffU)+(r.data[7]&0xffffU));r.data[1]=(r.data[1]&0xffff0000U)|d1;wordf(r,d1);return tail(554U,0x1e438U,0x1e1f2U);
 }
 auto f=rb(h,a+0x40U);bitz(r,(f&1U)==0);if(f&1U)return tail(555U,0x1e30aU,0x1e22aU);
 f=rb(h,a+0x41U);bitz(r,(f&2U)==0);if((f&2U)==0U){r.program_counter=0x1e18aU;return h.call_function(553U,1U,0x72U,1U,0x1e314U,0x1e18aU,c);}
 auto x=call(c,0x1e318U,0x1ebf0U,0x1e31cU,363U);if(x.status!=TranslationStatus::complete||x.control!=1U)return x;
 f=rb(h,a+0x41U);bitz(r,(f&4U)==0);
 if((f&4U)==0U){r.data[1]=0;logic(r,0,0x8000U,0xffffU);add_coordinate(2U);auto phase=rw(h,a+0x56U);auto next=static_cast<std::uint16_t>(phase+1U);ww(h,a+0x56U,next);wordf(r,next);auto d0=rw(h,a+0x56U);r.data[0]=(r.data[0]&0xffff0000U)|d0;logic(r,d0,0x8000U,0xffffU);d0&=8U;r.data[0]=(r.data[0]&0xffff0000U)|d0;logic(r,d0,0x8000U,0xffffU);if(d0==0U)return tail(554U,0x1e34eU,0x1e1f2U);auto d1=static_cast<std::uint16_t>((r.data[7]&0xffffU)<<5U);r.data[1]=(r.data[1]&0xffff0000U)|d1;wordf(r,d1);add_coordinate(1U);return tail(554U,0x1e372U,0x1e1f2U);}
 auto d1=static_cast<std::uint16_t>((r.data[7]&0xffffU)<<5U);r.data[1]=(r.data[1]&0xffff0000U)|d1;wordf(r,d1);add_coordinate(1U);const auto state=rw(h,a+0x74U);const auto diff=static_cast<std::uint16_t>(state-3U);cmpf(r,state,3U,diff);if(static_cast<std::int16_t>(state)<=3){d1=static_cast<std::uint16_t>((r.data[1]&0xffffU)+(r.data[7]&0xffffU));r.data[1]=(r.data[1]&0xffff0000U)|d1;wordf(r,d1);return tail(554U,0x1e3a2U,0x1e1f2U);}return tail(554U,0x1e39cU,0x1e1f2U);
}
}
