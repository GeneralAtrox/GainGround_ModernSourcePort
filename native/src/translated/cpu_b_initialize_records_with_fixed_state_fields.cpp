#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t R=2U,M=0xffffU;
std::uint16_t rw(ExecutionHost&h,std::uint32_t a){return h.read_memory_word(R,a,M);} std::uint32_t rl(ExecutionHost&h,std::uint32_t a){return(static_cast<std::uint32_t>(rw(h,a))<<16)|rw(h,a+2);} std::uint8_t rb(ExecutionHost&h,std::uint32_t a){bool o=a&1;auto v=h.read_memory_word(R,a&~1U,o?0xffU:0xff00U);return static_cast<std::uint8_t>(o?v:v>>8);} void ww(ExecutionHost&h,std::uint32_t a,std::uint16_t v){h.write_memory_word(R,a,v,M);} void wb(ExecutionHost&h,std::uint32_t a,std::uint8_t v){bool o=a&1;h.write_memory_word(R,a&~1U,static_cast<std::uint16_t>(v)<<(o?0:8),o?0xffU:0xff00U);}
void lw(CpuRegisters&r,std::uint16_t v){std::uint16_t f=r.status&0x10;if(v&0x8000)f|=8;if(!v)f|=4;r.status=static_cast<std::uint16_t>((r.status&~0x1f)|f);} void lb(CpuRegisters&r,std::uint8_t v){std::uint16_t f=r.status&0x10;if(v&0x80)f|=8;if(!v)f|=4;r.status=static_cast<std::uint16_t>((r.status&~0x1f)|f);} void ll(CpuRegisters&r,std::uint32_t v){std::uint16_t f=r.status&0x10;if(v&0x80000000)f|=8;if(!v)f|=4;r.status=static_cast<std::uint16_t>((r.status&~0x1f)|f);} void aw(CpuRegisters&r,std::uint16_t a,std::uint16_t b,std::uint16_t v){std::uint16_t f=0;if(v&0x8000)f|=8;if(!v)f|=4;if(((~(a^b))&(a^v)&0x8000)!=0)f|=2;if(static_cast<std::uint32_t>(a)+b>0xffff)f|=0x11;r.status=static_cast<std::uint16_t>((r.status&~0x1f)|f);} void clr_b(ExecutionHost&h,CpuRegisters&r,std::uint32_t a){(void)rb(h,a);wb(h,a,0);lb(r,0);} std::uint32_t pop(ExecutionHost&h,CpuRegisters&r){auto v=rl(h,r.address[7]);r.address[7]+=4;return v;}
} // namespace
FunctionResult cpu_b_initialize_records_with_fixed_state_fields(FunctionContext&c) noexcept{
 if(!c.host) {
  return {TranslationStatus::contract_violation,0,c.registers.program_counter};
 }
 auto&h=*c.host;auto&r=c.registers;
 r.address[7]-=2;ww(h,r.address[7],static_cast<std::uint16_t>(r.address[6]));lw(r,static_cast<std::uint16_t>(r.address[6]));auto n=rw(h,r.address[3]+6);r.data[1]=(r.data[1]&0xffff0000)|n;lw(r,n);r.address[1]=rl(h,r.address[3]+0x10)+0x0e;
 for(;;){r.address[0]=rl(h,r.address[3]+0x10);auto hi=rw(h,r.address[0]),lo=rw(h,r.address[0]+2);ww(h,r.address[6]+2,hi);ww(h,r.address[6]+4,lo);r.address[0]+=4;ll(r,(static_cast<std::uint32_t>(hi)<<16)|lo);auto a=rw(h,r.address[5]+0x66),b=rw(h,r.address[0]);r.address[0]+=2;auto v=static_cast<std::uint16_t>(a+b);r.data[0]=(r.data[0]&0xffff0000)|v;aw(r,a,b,v);ww(h,r.address[6]+8,v);lw(r,v);auto q=rb(h,r.address[0]++);wb(h,r.address[6]+0x3c,q);lb(r,q);q=rb(h,r.address[0]++);wb(h,r.address[6]+0xb,q);lb(r,q);v=rw(h,r.address[0]);r.address[0]+=2;ww(h,r.address[6]+0x3a,v);lw(r,v);v=rw(h,r.address[0]);r.address[0]+=2;ww(h,r.address[6]+6,v);lw(r,v);v=rw(h,r.address[0]);r.address[0]+=2;ww(h,r.address[6],v);lw(r,v);ww(h,r.address[6]+0x36,static_cast<std::uint16_t>(r.address[5]));lw(r,static_cast<std::uint16_t>(r.address[5]));clr_b(h,r,r.address[6]+0x3f);clr_b(h,r,r.address[6]+0x3d);ww(h,r.address[6]+0x10,0x3f3f);lw(r,0x3f3f);ww(h,r.address[6]+0x58,0x0600);lw(r,0x0600);v=rw(h,r.address[1]);r.address[1]+=2;ww(h,r.address[6]+0x46,v);lw(r,v);v=rw(h,r.address[1]);r.address[1]+=2;ww(h,r.address[6]+0x48,v);lw(r,v);do{r.address[6]+=0x80;q=rb(h,r.address[6]);lb(r,q);}while(q&0x80);n=static_cast<std::uint16_t>(r.data[1]-1);r.data[1]=(r.data[1]&0xffff0000)|n;if(n==0xffff)break;}
 auto s=rw(h,r.address[7]);r.address[7]+=2;r.address[6]=static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(s)));auto t=pop(h,r);r.program_counter=t;return FunctionResult::complete(1,t);
}
} // namespace gain_ground::translated
