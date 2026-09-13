#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;
constexpr std::uint16_t kX = 0x10U, kN = 0x08U, kZ = 0x04U, kV = 0x02U, kC = 0x01U;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };
[[nodiscard]] constexpr ByteLocation locate(std::uint32_t address) noexcept
{ const bool odd=(address&1U)!=0U;return {address&~1U,static_cast<std::uint16_t>(odd?0x00ffU:0xff00U),odd?0U:8U}; }
[[nodiscard]] std::uint8_t rb(ExecutionHost &h,std::uint32_t address)
{ const auto l=locate(address);return static_cast<std::uint8_t>(h.read_memory_word(kRegion,l.offset,l.mask)>>l.shift); }
void wb(ExecutionHost &h,std::uint32_t address,std::uint8_t value)
{ const auto l=locate(address);h.write_memory_word(kRegion,l.offset,static_cast<std::uint16_t>(value)<<l.shift,l.mask); }
[[nodiscard]] std::uint16_t rw(ExecutionHost &h,std::uint32_t address)
{ return h.read_memory_word(kRegion,address,kMask); }
void ww(ExecutionHost &h,std::uint32_t address,std::uint16_t value)
{ h.write_memory_word(kRegion,address,value,kMask); }
[[nodiscard]] std::uint32_t rl(ExecutionHost &h,std::uint32_t address)
{ return (static_cast<std::uint32_t>(rw(h,address))<<16U)|rw(h,address+2U); }

void logic(CpuRegisters &r,std::uint32_t value,std::uint32_t sign,std::uint32_t mask)
{ std::uint16_t f=r.status&kX;if(value&sign)f|=kN;if((value&mask)==0U)f|=kZ;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f); }
void zero_only(CpuRegisters &r,bool zero)
{ r.status=static_cast<std::uint16_t>(zero?(r.status|kZ):(r.status&~kZ)); }
void add_flags(CpuRegisters &r,std::uint32_t left,std::uint32_t right,std::uint32_t result)
{ std::uint16_t f{};if(result&0x8000U)f|=kN;if((result&0xffffU)==0U)f|=kZ;if(((~(left^right))&(left^result)&0x8000U)!=0U)f|=kV;if(left+right>0xffffU)f|=kX|kC;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f); }
void sub_flags(CpuRegisters &r,std::uint32_t left,std::uint32_t right,std::uint32_t result)
{ std::uint16_t f{};if(result&0x8000U)f|=kN;if((result&0xffffU)==0U)f|=kZ;if(((left^right)&(left^result)&0x8000U)!=0U)f|=kV;if((right&0xffffU)>(left&0xffffU))f|=kC;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f); }
[[nodiscard]] std::uint16_t addw(CpuRegisters &r,std::uint16_t left,std::uint16_t right)
{ const auto value=static_cast<std::uint16_t>(left+right);add_flags(r,left,right,value);return value; }
[[nodiscard]] std::uint16_t subw(CpuRegisters &r,std::uint16_t left,std::uint16_t right)
{ const auto value=static_cast<std::uint16_t>(left-right);sub_flags(r,left,right,value);return value; }
void comparew(CpuRegisters &r,std::uint16_t left,std::uint16_t right)
{ sub_flags(r,left,right,static_cast<std::uint16_t>(left-right)); }
[[nodiscard]] std::uint16_t lsr7(CpuRegisters &r,std::uint16_t value)
{ const bool carry=(value&0x0040U)!=0U;const auto result=static_cast<std::uint16_t>(value>>7U);std::uint16_t f{};if(result&0x8000U)f|=kN;if(result==0U)f|=kZ;if(carry)f|=kX|kC;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);return result; }
[[nodiscard]] std::uint16_t lsl3(CpuRegisters &r,std::uint16_t value)
{ const bool carry=(value&0x2000U)!=0U;const auto result=static_cast<std::uint16_t>(value<<3U);std::uint16_t f{};if(result&0x8000U)f|=kN;if(result==0U)f|=kZ;if(carry)f|=kX|kC;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);return result; }
[[nodiscard]] bool btst(CpuRegisters &r,std::uint8_t value,unsigned bit)
{ const bool set=(value&(1U<<bit))!=0U;zero_only(r,!set);return set; }
void bset(ExecutionHost &h,CpuRegisters &r,std::uint32_t address,unsigned bit)
{ const auto old=rb(h,address);zero_only(r,(old&(1U<<bit))==0U);wb(h,address,static_cast<std::uint8_t>(old|(1U<<bit))); }
void bclr(ExecutionHost &h,CpuRegisters &r,std::uint32_t address,unsigned bit)
{ const auto old=rb(h,address);zero_only(r,(old&(1U<<bit))==0U);wb(h,address,static_cast<std::uint8_t>(old&~(1U<<bit))); }
[[nodiscard]] FunctionResult finish(FunctionContext &c)
{ auto &h=*c.host;auto &r=c.registers;const auto target=rl(h,r.address[7]);r.address[7]+=4U;r.program_counter=target;return FunctionResult::complete(1U,target); }
[[nodiscard]] bool signed_le(std::uint16_t left,std::uint16_t right)
{ return static_cast<std::int16_t>(left)<=static_cast<std::int16_t>(right); }
[[nodiscard]] bool signed_ge(std::uint16_t left,std::uint16_t right)
{ return static_cast<std::int16_t>(left)>=static_cast<std::int16_t>(right); }
} // namespace

FunctionResult proven_static_cpu_b_72_0001d614(FunctionContext &context) noexcept
{
    if(context.host==nullptr)return {TranslationStatus::contract_violation,0U,context.registers.program_counter};
    auto &h=*context.host;auto &r=context.registers;const auto base=r.address[5];

    if(!btst(r,rb(h,base+0x41U),6U)){
        r.address[0]=rl(h,base+0x66U);
        r.address[0]=rl(h,r.address[0]);
        r.address[0]+=1U;

        auto selector=rw(h,base+0x5cU);
        r.data[0]=(r.data[0]&0xffff0000U)|selector;
        logic(r,selector,0x8000U,0xffffU);
        selector=static_cast<std::uint16_t>(selector&0x0600U);
        r.data[0]=(r.data[0]&0xffff0000U)|selector;
        logic(r,selector,0x8000U,0xffffU);
        selector=lsr7(r,selector);
        r.data[0]=(r.data[0]&0xffff0000U)|selector;

        std::uint16_t value{};
        std::uint16_t threshold{};
        bool accept{};
        switch(selector){
        case 0U:
            r.data[0]=0U;logic(r,0U,0x80000000U,0xffffffffU);
            value=rb(h,r.address[0]+1U);r.data[0]=value;logic(r,value,0x80U,0xffU);
            value=lsl3(r,value);r.data[0]=(r.data[0]&0xffff0000U)|value;
            threshold=rw(h,base+0x12U);comparew(r,value,threshold);accept=signed_le(value,threshold);break;
        case 4U:
            r.data[0]=0U;logic(r,0U,0x80000000U,0xffffffffU);
            value=rb(h,r.address[0]+3U);r.data[0]=value;logic(r,value,0x80U,0xffU);
            value=lsl3(r,value);r.data[0]=(r.data[0]&0xffff0000U)|value;
            threshold=rw(h,base+0x1aU);comparew(r,value,threshold);accept=signed_le(value,threshold);break;
        case 8U:
            r.data[0]=0U;logic(r,0U,0x80000000U,0xffffffffU);
            value=rb(h,r.address[0]);r.data[0]=value;logic(r,value,0x80U,0xffU);
            value=lsl3(r,value);r.data[0]=(r.data[0]&0xffff0000U)|value;
            threshold=rw(h,base+0x12U);comparew(r,value,threshold);accept=signed_ge(value,threshold);break;
        case 12U:
            r.data[0]=0U;logic(r,0U,0x80000000U,0xffffffffU);
            value=rb(h,r.address[0]+2U);r.data[0]=value;logic(r,value,0x80U,0xffU);
            value=lsl3(r,value);r.data[0]=(r.data[0]&0xffff0000U)|value;
            threshold=rw(h,base+0x1aU);comparew(r,value,threshold);accept=signed_ge(value,threshold);break;
        default:return {TranslationStatus::contract_violation,0U,r.program_counter};
        }
        if(!accept)return finish(context);

        bset(h,r,base+0x41U,6U);
        wb(h,base+0x60U,0U);logic(r,0U,0x80U,0xffU);
        auto offset=rw(h,base+0x5cU);r.data[0]=(r.data[0]&0xffff0000U)|offset;logic(r,offset,0x8000U,0xffffU);
        offset=addw(r,offset,0x0400U);r.data[0]=(r.data[0]&0xffff0000U)|offset;
        offset=static_cast<std::uint16_t>(offset&0x07ffU);r.data[0]=(r.data[0]&0xffff0000U)|offset;logic(r,offset,0x8000U,0xffffU);
        ww(h,base+0x5cU,offset);logic(r,offset,0x8000U,0xffffU);
        return finish(context);
    }

    auto value=rw(h,base+0x12U);r.data[0]=(r.data[0]&0xffff0000U)|value;logic(r,value,0x8000U,0xffffU);
    value=subw(r,value,rw(h,base+0x62U));r.data[0]=(r.data[0]&0xffff0000U)|value;
    value=addw(r,value,0x000aU);r.data[0]=(r.data[0]&0xffff0000U)|value;
    comparew(r,value,0x0014U);if(!signed_le(value,0x0014U))return finish(context);
    value=rw(h,base+0x1aU);r.data[0]=(r.data[0]&0xffff0000U)|value;logic(r,value,0x8000U,0xffffU);
    value=subw(r,value,rw(h,base+0x64U));r.data[0]=(r.data[0]&0xffff0000U)|value;
    value=addw(r,value,0x000aU);r.data[0]=(r.data[0]&0xffff0000U)|value;
    comparew(r,value,0x0014U);if(!signed_le(value,0x0014U))return finish(context);
    bclr(h,r,base+0x41U,5U);
    bclr(h,r,base+0x41U,6U);
    return finish(context);
}

} // namespace gain_ground::translated
