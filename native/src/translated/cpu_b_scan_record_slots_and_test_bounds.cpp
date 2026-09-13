// Gameplay implementation pass, unverified: restores original contact types
// 10/11 and child continuations using the retained state-72 opcode source.
#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t R = 2U, W = 0xffffU, F = 0x001fU;
constexpr std::uint16_t X = 0x10U, N = 8U, Z = 4U, V = 2U, C = 1U;
std::uint16_t rw(ExecutionHost&h,std::uint32_t a){return h.read_memory_word(R,a&0xffffffU,W);}
void ww(ExecutionHost&h,std::uint32_t a,std::uint16_t v){h.write_memory_word(R,a&0xffffffU,v,W);}
std::uint8_t rb(ExecutionHost&h,std::uint32_t a){bool o=a&1U;auto v=h.read_memory_word(R,(a&0xffffffU)&~1U,o?0xffU:0xff00U);return static_cast<std::uint8_t>(o?v:v>>8U);}
void logicw(CpuRegisters&r,std::uint16_t v){std::uint16_t f=r.status&X;if(v&0x8000U)f|=N;if(!v)f|=Z;r.status=static_cast<std::uint16_t>((r.status&~F)|f);}
void logicb(CpuRegisters&r,std::uint8_t v){std::uint16_t f=r.status&X;if(v&0x80U)f|=N;if(!v)f|=Z;r.status=static_cast<std::uint16_t>((r.status&~F)|f);}
void logicl(CpuRegisters&r,std::uint32_t v){std::uint16_t f=r.status&X;if(v&0x80000000U)f|=N;if(!v)f|=Z;r.status=static_cast<std::uint16_t>((r.status&~F)|f);}
void subw(CpuRegisters&r,std::uint16_t d,std::uint16_t s,std::uint16_t q,bool ux){bool c=d<s,ov=((d^s)&(d^q)&0x8000U)!=0;std::uint16_t f=ux?(c?X:0U):(r.status&X);if(q&0x8000U)f|=N;if(!q)f|=Z;if(ov)f|=V;if(c)f|=C;r.status=static_cast<std::uint16_t>((r.status&~F)|f);}
void addw(CpuRegisters&r,std::uint16_t d,std::uint16_t s,std::uint16_t q){bool c=static_cast<std::uint32_t>(d)+s>0xffffU,ov=((~(d^s))&(d^q)&0x8000U)!=0;std::uint16_t f=c?(X|C):0U;if(q&0x8000U)f|=N;if(!q)f|=Z;if(ov)f|=V;r.status=static_cast<std::uint16_t>((r.status&~F)|f);}
void cmpw(CpuRegisters&r,std::uint16_t d,std::uint16_t s){subw(r,d,s,static_cast<std::uint16_t>(d-s),false);}
void cmpb(CpuRegisters&r,std::uint8_t d,std::uint8_t s){auto q=static_cast<std::uint8_t>(d-s);bool c=d<s,ov=((d^s)&(d^q)&0x80U)!=0;std::uint16_t f=r.status&X;if(q&0x80U)f|=N;if(!q)f|=Z;if(ov)f|=V;if(c)f|=C;r.status=static_cast<std::uint16_t>((r.status&~F)|f);}
bool bmi(const CpuRegisters&r){return r.status&N;} bool bne(const CpuRegisters&r){return !(r.status&Z);} bool beq(const CpuRegisters&r){return r.status&Z;} bool bcs(const CpuRegisters&r){return r.status&C;} bool bcc(const CpuRegisters&r){return !(r.status&C);}
bool bgt(const CpuRegisters&r){return !(r.status&Z)&&bool(r.status&N)==bool(r.status&V);} bool ble(const CpuRegisters&r){return (r.status&Z)||bool(r.status&N)!=bool(r.status&V);}
void mdw(CpuRegisters&r,unsigned n,std::uint16_t v){r.data[n]=(r.data[n]&0xffff0000U)|v;logicw(r,v);} void mwa(CpuRegisters&r,unsigned n,std::uint16_t v){r.address[n]=static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(v)));}
void pushl(ExecutionHost&h,CpuRegisters&r,std::uint32_t v){r.address[7]-=4U;ww(h,r.address[7],static_cast<std::uint16_t>(v>>16U));ww(h,r.address[7]+2U,static_cast<std::uint16_t>(v));}
std::uint32_t popl(ExecutionHost&h,CpuRegisters&r){auto v=(static_cast<std::uint32_t>(rw(h,r.address[7]))<<16U)|rw(h,r.address[7]+2U);r.address[7]+=4U;return v;}
FunctionResult child(FunctionContext&c,std::uint32_t id,std::uint32_t site,std::uint32_t target,std::uint32_t ret){pushl(*c.host,c.registers,ret);c.registers.program_counter=target;return c.host->call_function(id,1U,0x72U,2U,site,target,c);}
}

FunctionResult cpu_b_scan_record_slots_and_test_bounds(FunctionContext&c) noexcept
{
 if(!c.host)
  return{TranslationStatus::contract_violation,0U,c.registers.program_counter};
 auto&h=*c.host;auto&r=c.registers;auto&a3=r.address[3];auto&a6=r.address[6];const auto a5=r.address[5];std::uint16_t q,s;FunctionResult z;
 // Resume after an existing child/IRQ without repeating contact scans or sound.
 switch (r.program_counter) {
 case 0xfed6U: break;
 case 0xff86U: case 0xff9eU: case 0x100e8U: case 0x100f0U:
 case 0x10180U: case 0x10186U: goto next;
 case 0x10004U: goto after_rescue_sound;
 case 0x1005eU: case 0x100aeU: case 0x1015aU: case 0x101daU: goto after_damage;
 default: return {TranslationStatus::contract_violation, 0U, r.program_counter};
 }
 a3=0x6c00U;q=rw(h,a3);a3+=2U;logicw(r,q);if(!bmi(r))a3=0x7002U;
 q=rw(h,a5+0x1aU);mdw(r,0,q);cmpw(r,q,0x200U);if(bcc(r))goto success;mdw(r,7,q);s=static_cast<std::uint16_t>(r.data[7]);q=static_cast<std::uint16_t>(s+0x20U);r.data[7]=(r.data[7]&0xffff0000U)|q;addw(r,s,0x20U,q);s=static_cast<std::uint16_t>(r.data[0]);q=static_cast<std::uint16_t>(s-0x20U);r.data[0]=(r.data[0]&0xffff0000U)|q;subw(r,s,0x20U,q,true);if(!bmi(r)){cmpw(r,static_cast<std::uint16_t>(r.data[7]),0x1ffU);if(!ble(r))mdw(r,7,0x1ffU);s=static_cast<std::uint16_t>(r.data[7]);q=static_cast<std::uint16_t>(s-r.data[0]);r.data[7]=(r.data[7]&0xffff0000U)|q;subw(r,s,static_cast<std::uint16_t>(r.data[0]),q,true);s=static_cast<std::uint16_t>(r.data[0]);q=static_cast<std::uint16_t>(s+s);r.data[0]=(r.data[0]&0xffff0000U)|q;addw(r,s,s,q);a3=static_cast<std::uint32_t>(a3+static_cast<std::int16_t>(q));}
scan: q=rw(h,a3);a3+=2U;mdw(r,0,q);if(beq(r))goto next;mwa(r,6,q);r.data[0]=0;logicl(r,0);r.data[0]=(r.data[0]&0xffffff00U)|rb(h,a6+0xbU);logicb(r,static_cast<std::uint8_t>(r.data[0]));q=static_cast<std::uint16_t>(r.data[0]<<2U);r.data[0]=(r.data[0]&0xffff0000U)|q;logicw(r,q);r.status=static_cast<std::uint16_t>(r.status&~X);
 switch(q>>2U){case 0:goto next;case 1:goto type1;case 2:case 3:goto next;case 4:goto type4;case 5:goto type5;case 6:goto type6;case 7:goto type7;case 8:goto type8;case 9:goto type9;case 10:goto type10;case 11:goto type11;default:r.program_counter=(0xff1eU+static_cast<std::int16_t>(q))&0xffffffU;return{TranslationStatus::contract_violation,0U,r.program_counter};}
type1:{auto v=rb(h,a5+0xaU);r.data[0]=(r.data[0]&0xffffff00U)|v;logicb(r,v);cmpb(r,v,rb(h,a6+0xaU));if(beq(r))goto next;v=rb(h,a6+0x3fU);logicb(r,v);if(bne(r))goto next;mdw(r,1,0xfff9U);mdw(r,2,7U);mdw(r,3,0xfffaU);mdw(r,4,6U);q=rw(h,a6+0x12U);mdw(r,0,q);s=static_cast<std::uint16_t>(r.data[1]);r.data[1]=(r.data[1]&0xffff0000U)|static_cast<std::uint16_t>(s+q);addw(r,s,q,static_cast<std::uint16_t>(s+q));s=static_cast<std::uint16_t>(r.data[2]);r.data[2]=(r.data[2]&0xffff0000U)|static_cast<std::uint16_t>(s+q);addw(r,s,q,static_cast<std::uint16_t>(s+q));q=rw(h,a6+0x1aU);mdw(r,0,q);s=static_cast<std::uint16_t>(r.data[3]);r.data[3]=(r.data[3]&0xffff0000U)|static_cast<std::uint16_t>(s+q);addw(r,s,q,static_cast<std::uint16_t>(s+q));s=static_cast<std::uint16_t>(r.data[4]);r.data[4]=(r.data[4]&0xffff0000U)|static_cast<std::uint16_t>(s+q);addw(r,s,q,static_cast<std::uint16_t>(s+q));z=child(c,202,0xff82U,0x101e2U,0xff86U);if(z.status!=TranslationStatus::complete||z.control!=1U)return z;goto next;}
type4:mdw(r,1,rw(h,a6+0x2aU));mdw(r,2,rw(h,a6+0x2cU));mdw(r,3,rw(h,a6+0x32U));mdw(r,4,rw(h,a6+0x34U));z=child(c,202,0xff9aU,0x101e2U,0xff9eU);if(z.status!=TranslationStatus::complete||z.control!=1U)return z;goto next;
type5:{q=rw(h,a5+0x5cU);logicw(r,q);if(bne(r))goto next;bool bit=rb(h,a6+0x43U)&1U;r.status=static_cast<std::uint16_t>(bit?(r.status&~Z):(r.status|Z));if(bit)goto next;mdw(r,1,0xfffbU);mdw(r,2,5U);mdw(r,3,0xfffcU);mdw(r,4,4U);q=rw(h,a6+0x12U);mdw(r,0,q);for(unsigned n=1;n<=2;n++){s=static_cast<std::uint16_t>(r.data[n]);r.data[n]=(r.data[n]&0xffff0000U)|static_cast<std::uint16_t>(s+q);addw(r,s,q,static_cast<std::uint16_t>(s+q));}q=rw(h,a6+0x1aU);mdw(r,0,q);for(unsigned n=3;n<=4;n++){s=static_cast<std::uint16_t>(r.data[n]);r.data[n]=(r.data[n]&0xffff0000U)|static_cast<std::uint16_t>(s+q);addw(r,s,q,static_cast<std::uint16_t>(s+q));}q=rw(h,a5+0x12U);mdw(r,0,q);s=q;q=static_cast<std::uint16_t>(q+7U);r.data[0]=(r.data[0]&0xffff0000U)|q;addw(r,s,7U,q);cmpw(r,static_cast<std::uint16_t>(r.data[1]),q);if(bgt(r))goto next;s=q;q=static_cast<std::uint16_t>(q-14U);r.data[0]=(r.data[0]&0xffff0000U)|q;subw(r,s,14U,q,true);cmpw(r,static_cast<std::uint16_t>(r.data[2]),q);if(bmi(r))goto next;q=rw(h,a5+0x1aU);mdw(r,0,q);s=q;q=static_cast<std::uint16_t>(q+6U);r.data[0]=(r.data[0]&0xffff0000U)|q;addw(r,s,6U,q);cmpw(r,static_cast<std::uint16_t>(r.data[3]),q);if(bgt(r))goto next;s=q;q=static_cast<std::uint16_t>(q-12U);r.data[0]=(r.data[0]&0xffff0000U)|q;subw(r,s,12U,q,true);cmpw(r,static_cast<std::uint16_t>(r.data[4]),q);if(bmi(r))goto next;r.address[7]-=4U;ww(h,r.address[7]+2U,static_cast<std::uint16_t>(r.address[4]));ww(h,r.address[7],static_cast<std::uint16_t>(r.address[4]>>16U));mdw(r,0,0x48U);z=child(c,308,0xfffeU,0x16ff8U,0x10004U);if(z.status!=TranslationStatus::complete||z.control!=1U)return z;goto after_rescue_sound;}
after_rescue_sound:r.address[4]=popl(h,r);ww(h,a5+0x5cU,static_cast<std::uint16_t>(a6));logicw(r,static_cast<std::uint16_t>(a6));ww(h,a6+0x44U,1U);logicw(r,1U);ww(h,a6+0x60U,static_cast<std::uint16_t>(a5));logicw(r,static_cast<std::uint16_t>(a5));goto next;
type6:{auto v=rb(h,a6+0x3fU);logicb(r,v);if(bne(r))goto next;mdw(r,1,rw(h,a6+0x12U));q=rw(h,a5+0x12U);mdw(r,0,q);s=q;q=static_cast<std::uint16_t>(q-7U);mdw(r,0,q);subw(r,s,7U,q,true);cmpw(r,static_cast<std::uint16_t>(r.data[1]),q);if(bmi(r))goto next;s=q;q=static_cast<std::uint16_t>(q+14U);mdw(r,0,q);addw(r,s,14U,q);cmpw(r,static_cast<std::uint16_t>(r.data[1]),q);if(bgt(r))goto next;mdw(r,1,rw(h,a6+0x16U));if(bmi(r))goto next;cmpw(r,static_cast<std::uint16_t>(r.data[1]),20U);if(bgt(r))goto next;mdw(r,1,rw(h,a6+0x1aU));q=rw(h,a5+0x1aU);mdw(r,0,q);s=q;q=static_cast<std::uint16_t>(q-6U);mdw(r,0,q);subw(r,s,6U,q,true);cmpw(r,static_cast<std::uint16_t>(r.data[1]),q);if(bmi(r))goto next;s=q;q=static_cast<std::uint16_t>(q+12U);mdw(r,0,q);addw(r,s,12U,q);cmpw(r,static_cast<std::uint16_t>(r.data[1]),q);if(bgt(r))goto next;z=child(c,203,0x1005aU,0x10272U,0x1005eU);if(z.status!=TranslationStatus::complete||z.control!=1U)return z;if(bcs(r))goto success;goto next;}
type7:{auto v=rb(h,a6+0x3fU);logicb(r,v);if(bne(r))goto next;q=rw(h,a5+0x12U);mdw(r,0,q);s=q;q=static_cast<std::uint16_t>(q-7U);mdw(r,0,q);subw(r,s,7U,q,true);cmpw(r,q,rw(h,a6+0x2cU));if(bgt(r))goto next;s=q;q=static_cast<std::uint16_t>(q+14U);mdw(r,0,q);addw(r,s,14U,q);cmpw(r,q,rw(h,a6+0x2aU));if(bmi(r))goto next;q=rw(h,a6+0x30U);logicw(r,q);if(bmi(r))goto next;cmpw(r,rw(h,a6+0x2eU),20U);if(bgt(r))goto next;q=rw(h,a5+0x1aU);mdw(r,0,q);s=q;q=static_cast<std::uint16_t>(q-6U);mdw(r,0,q);subw(r,s,6U,q,true);cmpw(r,q,rw(h,a6+0x34U));if(bgt(r))goto next;s=q;q=static_cast<std::uint16_t>(q+12U);mdw(r,0,q);addw(r,s,12U,q);cmpw(r,q,rw(h,a6+0x32U));if(bmi(r))goto next;z=child(c,203,0x100aaU,0x10272U,0x100aeU);if(z.status!=TranslationStatus::complete||z.control!=1U)return z;if(bcs(r))goto success;goto next;}
type8:{auto v=rb(h,a6+0x3fU);logicb(r,v);if(bne(r))goto next;}
type8_body:mdw(r,1,0xfff6U);mdw(r,2,10U);mdw(r,3,0xfff7U);mdw(r,4,9U);q=rw(h,a6+0x12U);mdw(r,0,q);for(unsigned n=1;n<=2;n++){s=static_cast<std::uint16_t>(r.data[n]);r.data[n]=(r.data[n]&0xffff0000U)|static_cast<std::uint16_t>(s+q);addw(r,s,q,static_cast<std::uint16_t>(s+q));}q=rw(h,a6+0x1aU);mdw(r,0,q);for(unsigned n=3;n<=4;n++){s=static_cast<std::uint16_t>(r.data[n]);r.data[n]=(r.data[n]&0xffff0000U)|static_cast<std::uint16_t>(s+q);addw(r,s,q,static_cast<std::uint16_t>(s+q));}q=rw(h,a5+0x48U);logicw(r,q);if(ble(r)){z=child(c,202,0x100ecU,0x101e2U,0x100f0U);}else{z=child(c,471,0x100e4U,0x10244U,0x100e8U);}if(z.status!=TranslationStatus::complete||z.control!=1U)return z;goto next;
type9:{auto v=rb(h,a6+0x3fU);logicb(r,v);if(bne(r))goto next;v=rb(h,a6+0x3eU);logicb(r,v);if(bne(r))goto type8_body;mdw(r,1,0xfff6U);mdw(r,2,10U);mdw(r,3,0xfff7U);mdw(r,4,9U);q=rw(h,a6+0x12U);mdw(r,0,q);for(unsigned n=1;n<=2;n++){s=static_cast<std::uint16_t>(r.data[n]);r.data[n]=(r.data[n]&0xffff0000U)|static_cast<std::uint16_t>(s+q);addw(r,s,q,static_cast<std::uint16_t>(s+q));}q=rw(h,a6+0x1aU);mdw(r,0,q);for(unsigned n=3;n<=4;n++){s=static_cast<std::uint16_t>(r.data[n]);r.data[n]=(r.data[n]&0xffff0000U)|static_cast<std::uint16_t>(s+q);addw(r,s,q,static_cast<std::uint16_t>(s+q));}q=rw(h,a5+0x12U);mdw(r,0,q);s=q;q=static_cast<std::uint16_t>(q-7U);r.data[0]=(r.data[0]&0xffff0000U)|q;subw(r,s,7U,q,true);cmpw(r,q,static_cast<std::uint16_t>(r.data[2]));if(bgt(r))goto next;s=q;q=static_cast<std::uint16_t>(q+14U);r.data[0]=(r.data[0]&0xffff0000U)|q;addw(r,s,14U,q);cmpw(r,q,static_cast<std::uint16_t>(r.data[1]));if(bmi(r))goto next;q=rw(h,a6+0x30U);logicw(r,q);if(bmi(r))goto next;cmpw(r,rw(h,a6+0x2eU),20U);if(bgt(r))goto next;q=rw(h,a5+0x1aU);mdw(r,0,q);s=q;q=static_cast<std::uint16_t>(q-6U);r.data[0]=(r.data[0]&0xffff0000U)|q;subw(r,s,6U,q,true);cmpw(r,q,static_cast<std::uint16_t>(r.data[4]));if(bgt(r))goto next;s=q;q=static_cast<std::uint16_t>(q+12U);r.data[0]=(r.data[0]&0xffff0000U)|q;addw(r,s,12U,q);cmpw(r,q,static_cast<std::uint16_t>(r.data[3]));if(bmi(r))goto next;z=child(c,472,0x10156U,0x1029aU,0x1015aU);if(z.status!=TranslationStatus::complete||z.control!=1U)return z;if(bcs(r))goto success;goto next;}
// Retained state-72 bytes FF46/FF4A and 10160..101DA: two further
// object categories use stored bounds. Their mark/protection rules differ.
type10: {
 const auto mark = rb(h, a6 + 0x3fU); logicb(r, mark);
 if (bne(r)) goto next;
 mdw(r, 1U, rw(h, a6 + 0x2aU));
 mdw(r, 2U, rw(h, a6 + 0x2cU));
 mdw(r, 3U, rw(h, a6 + 0x32U));
 mdw(r, 4U, rw(h, a6 + 0x34U));
 logicw(r, rw(h, a5 + 0x48U));
 if (ble(r)) z = child(c, 202U, 0x10182U, 0x101e2U, 0x10186U);
 else z = child(c, 471U, 0x1017cU, 0x10244U, 0x10180U);
 if (z.status != TranslationStatus::complete || z.control != 1U) return z;
 goto next;
}
type11: {
 const auto mark = rb(h, a6 + 0x3fU); logicb(r, mark);
 if (bne(r)) goto next;
 mdw(r, 1U, rw(h, a6 + 0x2aU));
 mdw(r, 2U, rw(h, a6 + 0x2cU));
 mdw(r, 3U, rw(h, a6 + 0x32U));
 mdw(r, 4U, rw(h, a6 + 0x34U));
 q = rw(h, a5 + 0x12U); mdw(r, 0U, q);
 s = q; q = static_cast<std::uint16_t>(q - 7U);
 r.data[0] = (r.data[0] & 0xffff0000U) | q; subw(r, s, 7U, q, true);
 cmpw(r, q, static_cast<std::uint16_t>(r.data[2])); if (bgt(r)) goto next;
 s = q; q = static_cast<std::uint16_t>(q + 14U);
 r.data[0] = (r.data[0] & 0xffff0000U) | q; addw(r, s, 14U, q);
 cmpw(r, q, static_cast<std::uint16_t>(r.data[1])); if (bmi(r)) goto next;
 logicw(r, rw(h, a6 + 0x30U)); if (bmi(r)) goto next;
 cmpw(r, rw(h, a6 + 0x2eU), 20U); if (bgt(r)) goto next;
 q = rw(h, a5 + 0x1aU); mdw(r, 0U, q);
 s = q; q = static_cast<std::uint16_t>(q - 6U);
 r.data[0] = (r.data[0] & 0xffff0000U) | q; subw(r, s, 6U, q, true);
 cmpw(r, q, static_cast<std::uint16_t>(r.data[4])); if (bgt(r)) goto next;
 s = q; q = static_cast<std::uint16_t>(q + 12U);
 r.data[0] = (r.data[0] & 0xffff0000U) | q; addw(r, s, 12U, q);
 cmpw(r, q, static_cast<std::uint16_t>(r.data[3])); if (bmi(r)) goto next;
 // This source call skips F203's TAS; do not consume/mark the contacted object.
 z = child(c, 203U, 0x101d6U, 0x10276U, 0x101daU);
 if (z.status != TranslationStatus::complete || z.control != 1U) return z;
 if (bcs(r)) goto success;
 goto next;
}
after_damage:if(bcs(r))goto success;goto next;
next:q=static_cast<std::uint16_t>(r.data[7]-1U);r.data[7]=(r.data[7]&0xffff0000U)|q;if(q!=0xffffU)goto scan;
success:{auto t=popl(h,r);r.program_counter=t;return FunctionResult::complete(1U,t);}
}
}
