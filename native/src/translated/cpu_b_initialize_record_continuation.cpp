#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;
std::uint16_t rw(ExecutionHost &h, std::uint32_t a)
{ return h.read_memory_word(kRegion, a & 0x3ffffU, kMask); }
void ww(ExecutionHost &h, std::uint32_t a, std::uint16_t v)
{ h.write_memory_word(kRegion, a & 0x3ffffU, v, kMask); }
std::uint8_t rb(ExecutionHost &h, std::uint32_t a)
{
    const auto p = a & 0x3ffffU; const bool odd = (p & 1U) != 0U;
    const auto v = h.read_memory_word(kRegion, p & ~1U,
        odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? v : v >> 8U);
}
void wb(ExecutionHost &h, std::uint32_t a, std::uint8_t v)
{
    const auto p = a & 0x3ffffU; const bool odd = (p & 1U) != 0U;
    h.write_memory_word(kRegion, p & ~1U,
        static_cast<std::uint16_t>(v) << (odd ? 0U : 8U),
        odd ? 0x00ffU : 0xff00U);
}
void logicw(CpuRegisters &r, std::uint16_t v)
{ std::uint16_t f=r.status&0x10U;if(v&0x8000U)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f); }
void logicb(CpuRegisters &r, std::uint8_t v)
{ std::uint16_t f=r.status&0x10U;if(v&0x80U)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f); }
void addw(CpuRegisters &r,std::uint16_t d,std::uint16_t s,std::uint16_t v)
{ std::uint16_t f=0;if(std::uint32_t(d)+s>0xffffU)f|=0x11U;if((~(d^s)&(d^v)&0x8000U)!=0)f|=2U;if(v&0x8000U)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f); }
void subw(CpuRegisters &r,std::uint16_t d,std::uint16_t s,std::uint16_t v)
{ std::uint16_t f=0;if(s>d)f|=0x11U;if(((d^s)&(d^v)&0x8000U)!=0)f|=2U;if(v&0x8000U)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f); }
void asr1(CpuRegisters &r, std::uint16_t old, std::uint16_t v)
{ std::uint16_t f=0;if(old&1U)f|=0x11U;if(v&0x8000U)f|=8U;if(!v)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f); }
void push(ExecutionHost &h,CpuRegisters &r,std::uint32_t v)
{ r.address[7]-=4U;ww(h,r.address[7],static_cast<std::uint16_t>(v>>16U));ww(h,r.address[7]+2U,static_cast<std::uint16_t>(v)); }
std::uint32_t pop(ExecutionHost &h,CpuRegisters &r)
{ auto v=(static_cast<std::uint32_t>(rw(h,r.address[7]))<<16U)|rw(h,r.address[7]+2U);r.address[7]+=4U;return v; }
FunctionResult child(FunctionContext &c,std::uint32_t id,std::uint32_t site,std::uint32_t target,std::uint32_t ret)
{ push(*c.host,c.registers,ret);c.registers.program_counter=target;return c.host->call_function(id,1U,0x72U,2U,site,target,c); }
} // namespace

FunctionResult cpu_b_initialize_record_continuation(FunctionContext &c) noexcept
{
    if (!c.host) return {TranslationStatus::contract_violation,0U,c.registers.program_counter};
    auto &h=*c.host; auto &r=c.registers; const auto base=r.address[5];

    ww(h,base+2U,0x0001U); ww(h,base+4U,0x2daeU); logicw(r,0x2daeU);
    (void)rb(h,base+0x3dU); wb(h,base+0x3dU,0U); logicb(r,0U);

    auto d0=rw(h,base+0x16U); r.data[0]=(r.data[0]&0xffff0000U)|d0; logicw(r,d0);
    auto next=static_cast<std::uint16_t>((static_cast<std::int16_t>(d0))>>1U);
    r.data[0]=(r.data[0]&0xffff0000U)|next; asr1(r,d0,next); d0=next;
    next=static_cast<std::uint16_t>(d0+0x3fU); r.data[0]=(r.data[0]&0xffff0000U)|next; addw(r,d0,0x3fU,next); d0=next;
    wb(h,base+0x10U,static_cast<std::uint8_t>(d0)); logicb(r,static_cast<std::uint8_t>(d0));
    wb(h,base+0x11U,static_cast<std::uint8_t>(d0)); logicb(r,static_cast<std::uint8_t>(d0));

    const auto bounds=[&](std::uint32_t source,std::uint32_t lower,std::uint32_t upper){
        auto value=rw(h,base+source);r.data[0]=(r.data[0]&0xffff0000U)|value;logicw(r,value);
        auto v=static_cast<std::uint16_t>(value-0x10U);r.data[0]=(r.data[0]&0xffff0000U)|v;subw(r,value,0x10U,v);ww(h,base+lower,v);logicw(r,v);
        value=v;v=static_cast<std::uint16_t>(value+0x20U);r.data[0]=(r.data[0]&0xffff0000U)|v;addw(r,value,0x20U,v);ww(h,base+upper,v);logicw(r,v);
    };
    bounds(0x12U,0x2aU,0x2cU); bounds(0x16U,0x2eU,0x30U); bounds(0x1aU,0x32U,0x34U);

    auto z=child(c,253U,0x12da8U,0x1283cU,0x12dacU);
    if(z.status!=TranslationStatus::complete||z.control!=1U)return z;

    r.data[0]=0U; logicw(r,0U);
    const auto phase=rb(h,base+0x3dU);r.data[0]=(r.data[0]&0xffffff00U)|phase;logicb(r,phase);
    r.address[0]=0x1360aU+static_cast<std::uint32_t>(static_cast<std::int16_t>(static_cast<std::uint16_t>(r.data[0])));
    auto word=rw(h,r.address[0]);r.address[0]+=2U;ww(h,base+6U,word);logicw(r,word);
    auto byte=rb(h,r.address[0]++);wb(h,base+1U,byte);logicb(r,byte);
    byte=rb(h,r.address[0]++);wb(h,base+9U,byte);logicb(r,byte);
    z=child(c,280U,0x12de2U,0x15d24U,0x12de8U);if(z.status!=TranslationStatus::complete||z.control!=1U)return z;
    z=child(c,282U,0x12de8U,0x15df2U,0x12deeU);if(z.status!=TranslationStatus::complete||z.control!=1U)return z;
    const auto target=pop(h,r);r.program_counter=target;return FunctionResult::complete(1U,target);
}
} // namespace gain_ground::translated
