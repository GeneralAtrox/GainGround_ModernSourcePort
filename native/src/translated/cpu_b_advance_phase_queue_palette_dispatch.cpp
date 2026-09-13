#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivate = 2U, kTile = 5U, kWindow = 7U, kIo = 10U;
constexpr std::uint16_t kMask = 0xffffU;
struct Loc { std::uint16_t region; std::uint32_t offset; };
Loc locate(std::uint32_t a) {
    if (a >= 0x20c000U && a < 0x210000U) return {kWindow, a & 0x3fffU};
    if (a >= 0x200000U && a < 0x20c000U) return {kTile, a - 0x200000U};
    if (a >= 0x404000U && a < 0x405000U) return {kIo, a - 0x404000U};
    return {kPrivate, a & 0x3ffffU};
}
std::uint16_t rw(ExecutionHost& h, std::uint32_t a) { auto p=locate(a); return h.read_memory_word(p.region,p.offset,kMask); }
void ww(ExecutionHost& h, std::uint32_t a, std::uint16_t v) { auto p=locate(a); h.write_memory_word(p.region,p.offset,v,kMask); }
std::uint8_t rb(ExecutionHost& h, std::uint32_t a) { auto p=locate(a); bool odd=p.offset&1U; auto m=static_cast<std::uint16_t>(odd?0xffU:0xff00U); return static_cast<std::uint8_t>(h.read_memory_word(p.region,p.offset&~1U,m)>>(odd?0:8)); }
void wb(ExecutionHost& h, std::uint32_t a, std::uint8_t v) { auto p=locate(a); bool odd=p.offset&1U; auto m=static_cast<std::uint16_t>(odd?0xffU:0xff00U); h.write_memory_word(p.region,p.offset&~1U,static_cast<std::uint16_t>(v)<<(odd?0:8),m); }
std::uint32_t rl(ExecutionHost& h, std::uint32_t a) { return (static_cast<std::uint32_t>(rw(h,a))<<16U)|rw(h,a+2U); }
void logic(CpuRegisters&r,std::uint32_t v,std::uint32_t sign){std::uint16_t f=r.status&0x10U;if(v&sign)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void addw(CpuRegisters&r,std::uint16_t a,std::uint16_t b,std::uint16_t v){std::uint16_t f=0;if(v&0x8000)f|=8;if(!v)f|=4;if((~(a^b)&(a^v)&0x8000)!=0)f|=2;if(std::uint32_t(a)+b>0xffff)f|=0x11;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void subw(CpuRegisters&r,std::uint16_t a,std::uint16_t b,std::uint16_t v,bool x=true){std::uint16_t f=x?0:(r.status&0x10U);if(v&0x8000)f|=8;if(!v)f|=4;if(((a^b)&(a^v)&0x8000)!=0)f|=2;if(b>a)f|=x?0x11:1;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void push(ExecutionHost&h,CpuRegisters&r,std::uint32_t v){r.address[7]-=4;ww(h,r.address[7],static_cast<std::uint16_t>(v>>16));ww(h,r.address[7]+2,static_cast<std::uint16_t>(v));}
std::uint32_t pop(ExecutionHost&h,CpuRegisters&r){auto v=rl(h,r.address[7]);r.address[7]+=4;return v;}
FunctionResult child(FunctionContext&c,std::uint32_t id,std::uint32_t site,std::uint32_t target,std::uint32_t ret){auto&r=c.registers;push(*c.host,r,ret);r.program_counter=target;return c.host->call_function(id,1U,0x72U,2U,site,target,c);}
std::uint32_t read_predecrement_long(ExecutionHost&h,std::uint32_t a){auto low=rw(h,a+2U);auto high=rw(h,a);return (static_cast<std::uint32_t>(high)<<16U)|low;}
void addx_long(ExecutionHost&h,CpuRegisters&r){r.address[1]-=4;auto s=read_predecrement_long(h,r.address[1]);r.address[0]-=4;auto d=read_predecrement_long(h,r.address[0]);auto sum=std::uint64_t(d)+s+((r.status&0x10U)?1U:0U);auto v=static_cast<std::uint32_t>(sum);ww(h,r.address[0]+2,static_cast<std::uint16_t>(v));ww(h,r.address[0],static_cast<std::uint16_t>(v>>16));std::uint16_t f=0;if(v&0x80000000U)f|=8;if(!v&&(r.status&4U))f|=4;if((~(d^s)&(d^v)&0x80000000U)!=0)f|=2;if(sum>0xffffffffULL)f|=0x11;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
}

FunctionResult cpu_b_advance_phase_queue_palette_dispatch(FunctionContext& c) noexcept {
 if(!c.host) return {TranslationStatus::contract_violation,0U,c.registers.program_counter};
 auto&h=*c.host;auto&r=c.registers;
 auto cr=child(c,154U,0xd5faU,0xdba0U,0xd5feU);if(cr.status!=TranslationStatus::complete||cr.control!=1U)return cr;
 cr=child(c,155U,0xd5feU,0xdbe2U,0xd602U);if(cr.status!=TranslationStatus::complete||cr.control!=1U)return cr;
 auto sel=rw(h,r.address[5]+0xc);r.data[0]=(r.data[0]&0xffff0000U)|sel;logic(r,sel,0x8000U);
 {auto v=static_cast<std::uint16_t>(sel<<2);bool cbit=sel&0x4000U;bool ov=((sel^(sel<<1))&0x8000U)||(((sel<<1)^(sel<<2))&0x8000U);r.data[0]=(r.data[0]&0xffff0000U)|v;std::uint16_t f=(v&0x8000?8:0)|(!v?4:0)|(ov?2:0)|(cbit?0x11:0);r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
 if(sel>1U)return {TranslationStatus::contract_violation,0U,0xd608U};
 if(sel==0U){
  auto a=r.address[5]+0x18;auto delay=rw(h,a);logic(r,delay,0x8000U);if(static_cast<std::int16_t>(delay)>0){delay=rw(h,a);auto v=static_cast<std::uint16_t>(delay-1);ww(h,a,v);subw(r,delay,1,v);goto done;}
  auto d2=rw(h,r.address[5]+0x10);r.data[2]=(r.data[2]&0xffff0000U)|d2;logic(r,d2,0x8000U);r.address[0]=0x202000U;r.data[0]=(r.data[0]&0xffff0000U)|1U;logic(r,1,0x8000U);auto d0=static_cast<std::uint16_t>(d2+1);r.data[0]=(r.data[0]&0xffff0000U)|d0;addw(r,1,d2,d0);auto prod=std::uint32_t(d0)*0x62U;r.data[0]=prod;logic(r,prod,0x80000000U);r.address[1]=0x8960U+static_cast<std::int16_t>(static_cast<std::uint16_t>(prod));
  auto d1=static_cast<std::uint16_t>(0x16-d2);r.data[1]=(r.data[1]&0xffff0000U)|d1;subw(r,0x16,d2,d1);if(static_cast<std::int16_t>(d1)>=0)for(;;){cr=child(c,157U,0xd642U,0xdc5eU,0xd646U);if(cr.status!=TranslationStatus::complete||cr.control!=1U)return cr;auto n=static_cast<std::uint16_t>(r.data[1]-1);r.data[1]=(r.data[1]&0xffff0000U)|n;if(n==0xffff)break;}
  cr=child(c,158U,0xd64aU,0xdc78U,0xd64eU);if(cr.status!=TranslationStatus::complete||cr.control!=1U)return cr;
  r.data[0]=(r.data[0]&0xffff0000U)|d2;logic(r,d2,0x8000U);auto shifted=static_cast<std::uint16_t>(d2<<7);r.data[0]=(r.data[0]&0xffff0000U)|shifted;logic(r,shifted,0x8000U);r.address[0]=0x202c00U+static_cast<std::int16_t>(shifted);cr=child(c,158U,0xd65aU,0xdc78U,0xd65eU);if(cr.status!=TranslationStatus::complete||cr.control!=1U)return cr;
  r.address[1]=0x9290U;d1=static_cast<std::uint16_t>(0x16-d2);r.data[1]=(r.data[1]&0xffff0000U)|d1;subw(r,0x16,d2,d1);if(static_cast<std::int16_t>(d1)>=0)for(;;){cr=child(c,157U,0xd66cU,0xdc5eU,0xd670U);if(cr.status!=TranslationStatus::complete||cr.control!=1U)return cr;auto n=static_cast<std::uint16_t>(r.data[1]-1);r.data[1]=(r.data[1]&0xffff0000U)|n;if(n==0xffff)break;}
  a=r.address[5]+0x10;auto old=rw(h,a);auto nv=static_cast<std::uint16_t>(old+1);ww(h,a,nv);addw(r,old,1,nv);auto chk=rw(h,a);subw(r,chk,0x18,static_cast<std::uint16_t>(chk-0x18),false);if(static_cast<std::int16_t>(chk-0x18)>=0){a=r.address[5]+0xc;ww(h,a,1);logic(r,1,0x8000U);a=r.address[5]+0x10;(void)rw(h,a);ww(h,a,0);logic(r,0,0x8000U);r.address[0]=0x20c000U;r.data[0]=(r.data[0]&0xffff0000U)|0x2ffU;logic(r,0x2ff,0x8000U);for(;;){(void)rw(h,r.address[0]);(void)rw(h,r.address[0]+2);ww(h,r.address[0]+2,0);ww(h,r.address[0],0);r.address[0]+=4;logic(r,0,0x80000000U);auto n=static_cast<std::uint16_t>(r.data[0]-1);r.data[0]=(r.data[0]&0xffff0000U)|n;if(n==0xffff)break;}}
 } else {
  r.address[0]=0x20d007U;r.data[3]=0xbU;logic(r,0xb,0x80000000U);auto d2=rw(h,r.address[5]+0x10);r.data[2]=(r.data[2]&0xffff0000U)|d2;logic(r,d2,0x8000U);
  for(;;){subw(r,d2,10,static_cast<std::uint16_t>(d2-10),false);if(static_cast<std::int16_t>(d2-10)<=0){auto d0=static_cast<std::uint16_t>(d2*6);r.data[0]=(r.data[0]&0xffff0000U)|d0;r.address[1]=0xdffcU+static_cast<std::int16_t>(d0);r.data[0]=0;logic(r,0,0x80000000U);r.data[1]=(r.data[1]&0xffff0000U)|5;logic(r,5,0x8000U);for(;;){auto b=rb(h,r.address[1]++);r.data[0]=(r.data[0]&0xffffff00U)|b;logic(r,b,0x80U);auto addr=r.address[0]+static_cast<std::int16_t>(static_cast<std::uint16_t>(r.data[0]));auto ov=rb(h,addr);auto bit=static_cast<std::uint8_t>(ov&~1U);wb(h,addr,bit);r.status=static_cast<std::uint16_t>((r.status&~4U)|((ov&1U)?0U:4U));auto n=static_cast<std::uint16_t>(r.data[1]-1);r.data[1]=(r.data[1]&0xffff0000U)|n;if(n==0xffff)break;}}
   d2=static_cast<std::uint16_t>(d2-2);r.data[2]=(r.data[2]&0xffff0000U)|d2;subw(r,static_cast<std::uint16_t>(d2+2),2,d2);if(static_cast<std::int16_t>(d2)<0)break;r.address[0]+=0x100;auto n=static_cast<std::uint16_t>(r.data[3]-1);r.data[3]=(r.data[3]&0xffff0000U)|n;if(n==0xffff)break;}
  auto flag=rb(h,r.address[5]+0x11);r.status=static_cast<std::uint16_t>((r.status&~4U)|((flag&1U)?0U:4U));if(flag&1U){r.address[0]=0x20d007U;r.data[0]=(r.data[0]&0xffff0000U)|0x17f;logic(r,0x17f,0x8000U);for(;;){auto b=rb(h,r.address[0]++);r.status=static_cast<std::uint16_t>((r.status&~4U)|((b&1U)?0U:4U));if(!(b&1U)){r.address[1]=r.address[0];r.status=static_cast<std::uint16_t>(r.status&~0x1fU);addx_long(h,r);addx_long(h,r);r.address[0]+=8;r.address[1]+=8;r.status=static_cast<std::uint16_t>(r.status&~0x1fU);addx_long(h,r);addx_long(h,r);r.address[0]+=8;}r.address[0]+=7;auto n=static_cast<std::uint16_t>(r.data[0]-1);r.data[0]=(r.data[0]&0xffff0000U)|n;if(n==0xffff)break;}}
  auto a=r.address[5]+0x10;auto old=rw(h,a);auto nv=static_cast<std::uint16_t>(old+1);ww(h,a,nv);addw(r,old,1,nv);auto chk=rw(h,a);subw(r,chk,0x60,static_cast<std::uint16_t>(chk-0x60),false);if(static_cast<std::int16_t>(chk-0x60)>=0){(void)rb(h,0xc18);wb(h,0xc18,0);logic(r,0,0x80U);ww(h,0x404018,2);logic(r,2,0x8000U);auto dest=r.address[5]+2;ww(h,dest,0);ww(h,dest+2,0xd734);logic(r,0xd734,0x80000000U);}
 }
done: auto target=pop(h,r);r.program_counter=target;return FunctionResult::complete(1U,target);
}
} // namespace gain_ground::translated
