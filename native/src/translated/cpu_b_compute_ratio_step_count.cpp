#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;
constexpr std::uint16_t kX = 0x10U, kN = 0x08U, kZ = 0x04U, kV = 0x02U, kC = 0x01U;
struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };
[[nodiscard]] constexpr ByteLocation locate(std::uint32_t address) noexcept
{ const bool odd=(address&1U)!=0U; return {address&~1U,static_cast<std::uint16_t>(odd?0x00ffU:0xff00U),odd?0U:8U}; }
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
{ if(zero)r.status=static_cast<std::uint16_t>(r.status|kZ);else r.status=static_cast<std::uint16_t>(r.status&~kZ); }
void add_flags(CpuRegisters &r,std::uint32_t left,std::uint32_t right,std::uint32_t result,std::uint32_t sign,std::uint32_t mask)
{ std::uint16_t f{};if(result&sign)f|=kN;if((result&mask)==0U)f|=kZ;if(((~(left^right))&(left^result)&sign)!=0U)f|=kV;if(left+right>mask)f|=kX|kC;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f); }
void sub_flags(CpuRegisters &r,std::uint32_t left,std::uint32_t right,std::uint32_t result,std::uint32_t sign,std::uint32_t mask)
{ std::uint16_t f{};if(result&sign)f|=kN;if((result&mask)==0U)f|=kZ;if(((left^right)&(left^result)&sign)!=0U)f|=kV;if((right&mask)>(left&mask))f|=kX|kC;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f); }
[[nodiscard]] std::uint8_t addb(CpuRegisters &r,std::uint8_t a,std::uint8_t b)
{ const auto v=static_cast<std::uint8_t>(a+b);add_flags(r,a,b,v,0x80U,0xffU);return v; }
[[nodiscard]] std::uint16_t addw(CpuRegisters &r,std::uint16_t a,std::uint16_t b)
{ const auto v=static_cast<std::uint16_t>(a+b);add_flags(r,a,b,v,0x8000U,0xffffU);return v; }
[[nodiscard]] std::uint8_t subb(CpuRegisters &r,std::uint8_t a,std::uint8_t b)
{ const auto v=static_cast<std::uint8_t>(a-b);sub_flags(r,a,b,v,0x80U,0xffU);return v; }
[[nodiscard]] std::uint16_t subw(CpuRegisters &r,std::uint16_t a,std::uint16_t b)
{ const auto v=static_cast<std::uint16_t>(a-b);sub_flags(r,a,b,v,0x8000U,0xffffU);return v; }
[[nodiscard]] std::uint16_t negw(CpuRegisters &r,std::uint16_t value)
{ const auto v=static_cast<std::uint16_t>(0U-value);sub_flags(r,0U,value,v,0x8000U,0xffffU);return v; }
[[nodiscard]] std::uint32_t negl(CpuRegisters &r,std::uint32_t value)
{ const auto v=0U-value;sub_flags(r,0U,value,v,0x80000000U,0xffffffffU);return v; }
[[nodiscard]] std::uint32_t lsrl(CpuRegisters &r,std::uint32_t value,unsigned count)
{ const bool carry=(value&(1U<<(count-1U)))!=0U;const auto v=value>>count;std::uint16_t f{};if(v&0x80000000U)f|=kN;if(v==0U)f|=kZ;if(carry)f|=kX|kC;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);return v; }
[[nodiscard]] bool btst(CpuRegisters &r,std::uint8_t value,unsigned bit)
{ const bool set=(value&(1U<<bit))!=0U;zero_only(r,!set);return set; }
void bclr(ExecutionHost &h,CpuRegisters &r,std::uint32_t address,unsigned bit)
{ const auto old=rb(h,address);zero_only(r,(old&(1U<<bit))==0U);wb(h,address,static_cast<std::uint8_t>(old&~(1U<<bit))); }
void bset(ExecutionHost &h,CpuRegisters &r,std::uint32_t address,unsigned bit)
{ const auto old=rb(h,address);zero_only(r,(old&(1U<<bit))==0U);wb(h,address,static_cast<std::uint8_t>(old|(1U<<bit))); }
void push(ExecutionHost &h,CpuRegisters &r,std::uint32_t value)
{ r.address[7]-=4U;ww(h,r.address[7],static_cast<std::uint16_t>(value>>16U));ww(h,r.address[7]+2U,static_cast<std::uint16_t>(value)); }
[[nodiscard]] FunctionResult finish(FunctionContext &c)
{ auto &h=*c.host;auto &r=c.registers;const auto target=rl(h,r.address[7]);r.address[7]+=4U;r.program_counter=target;return FunctionResult::complete(1U,target); }
[[nodiscard]] FunctionResult call(FunctionContext &c,std::uint32_t id,std::uint32_t site,std::uint32_t target,std::uint32_t continuation)
{ auto &h=*c.host;push(h,c.registers,continuation);c.registers.program_counter=target;return h.call_function(id,1U,0x72U,2U,site,target,c); }
[[nodiscard]] bool completed(const FunctionResult &v)
{ return v.status==TranslationStatus::complete&&v.control==1U; }
} // namespace

FunctionResult cpu_b_compute_ratio_step_count(FunctionContext &context) noexcept
{
    if(context.host==nullptr)return {TranslationStatus::contract_violation,0U,context.registers.program_counter};
    auto &h=*context.host;auto &r=context.registers;const auto base=r.address[5];
    const auto marker=rb(h,base+0x59U);logic(r,marker,0x80U,0xffU);if(marker!=0U)return finish(context);
    if(!btst(r,rb(h,base+0x41U),5U)){
        r.program_counter=0x0001d6d8U;
        return h.call_function(341U,1U,0x72U,1U,0x0001d154U,0x0001d6d8U,context);
    }
    const auto prior_count=rb(h,base+0x58U);logic(r,prior_count,0x80U,0xffU);
    if(prior_count!=0U){const auto value=subb(r,rb(h,base+0x58U),1U);wb(h,base+0x58U,value);if(value==0U)bclr(h,r,base+0x41U,5U);return finish(context);}

    r.data[0]=0U;logic(r,0U,0x80000000U,0xffffffffU);
    const auto source=rb(h,base+0x60U);r.data[0]=source;logic(r,source,0x80U,0xffU);
    const auto index=addb(r,source,9U);r.data[0]=index;r.address[4]=0x3400U;
    const auto table=rb(h,r.address[4]+static_cast<std::int16_t>(static_cast<std::uint16_t>(r.data[0])));logic(r,table,0x80U,0xffU);
    if(table==0U){bclr(h,r,base+0x41U,5U);return finish(context);}
    auto mode=rw(h,base+0x54U);r.data[0]=(r.data[0]&0xffff0000U)|mode;logic(r,mode,0x8000U,0xffffU);
    mode=static_cast<std::uint16_t>(mode&0x000fU);r.data[0]=(r.data[0]&0xffff0000U)|mode;logic(r,mode,0x8000U,0xffffU);if(mode!=0U)return finish(context);

    auto child=call(context,347U,0x0001d182U,0x0001da58U,0x0001d186U);if(!completed(child))return child;
    bset(h,r,base+0x40U,2U);r.address[0]=rl(h,base+0x6eU);
    const auto adjustment=rb(h,r.address[0]+3U);r.data[0]=(r.data[0]&0xffffff00U)|adjustment;logic(r,adjustment,0x80U,0xffU);
    const auto phase=rb(h,base+0x37U);wb(h,base+0x36U,phase);logic(r,phase,0x80U,0xffU);
    const auto sum=addb(r,rb(h,base+0x36U),adjustment);wb(h,base+0x36U,sum);
    child=call(context,352U,0x0001d19eU,0x0001dbc4U,0x0001d1a2U);if(!completed(child))return child;
    r.data[0]=0U;logic(r,0U,0x80000000U,0xffffffffU);const auto selector=rb(h,base+0x60U);r.data[0]=selector;logic(r,selector,0x80U,0xffU);
    child=call(context,348U,0x0001d1a8U,0x0001da92U,0x0001d1acU);if(!completed(child))return child;
    r.data[3]=0U;logic(r,0U,0x80000000U,0xffffffffU);r.address[0]=rl(h,base+0x6eU);const auto margin=rb(h,r.address[0]+4U);r.data[3]=margin;logic(r,margin,0x80U,0xffU);
    auto selector_word=rw(h,base+0x5cU);r.data[2]=(r.data[2]&0xffff0000U)|selector_word;logic(r,selector_word,0x8000U,0xffffU);
    selector_word=addw(r,selector_word,0x0100U);r.data[2]=(r.data[2]&0xffff0000U)|selector_word;selector_word=static_cast<std::uint16_t>(selector_word&0x0200U);r.data[2]=(r.data[2]&0xffff0000U)|selector_word;logic(r,selector_word,0x8000U,0xffffU);
    if(selector_word==0U){
        auto value=subw(r,static_cast<std::uint16_t>(r.data[0]),rw(h,base+0x12U));r.data[0]=(r.data[0]&0xffff0000U)|value;if(static_cast<std::int16_t>(value)<0){value=negw(r,value);r.data[0]=(r.data[0]&0xffff0000U)|value;}
        value=addw(r,value,static_cast<std::uint16_t>(r.data[3]));r.data[0]=(r.data[0]&0xffff0000U)|value;
        r.data[0]=static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(value)));logic(r,r.data[0],0x80000000U,0xffffffffU);
        r.data[0]=(r.data[0]<<16U)|(r.data[0]>>16U);logic(r,r.data[0],0x80000000U,0xffffffffU);
        r.data[1]=rl(h,base+0x1eU);logic(r,r.data[1],0x80000000U,0xffffffffU);if(static_cast<std::int32_t>(r.data[1])<0)r.data[1]=negl(r,r.data[1]);
    }else{
        auto value=subw(r,static_cast<std::uint16_t>(r.data[1]),rw(h,base+0x1aU));r.data[1]=(r.data[1]&0xffff0000U)|value;if(static_cast<std::int16_t>(value)<0){value=negw(r,value);r.data[1]=(r.data[1]&0xffff0000U)|value;}
        value=addw(r,value,static_cast<std::uint16_t>(r.data[3]));r.data[1]=(r.data[1]&0xffff0000U)|value;r.data[0]=(r.data[0]&0xffff0000U)|value;logic(r,value,0x8000U,0xffffU);
        r.data[0]=static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(value)));logic(r,r.data[0],0x80000000U,0xffffffffU);
        r.data[0]=(r.data[0]<<16U)|(r.data[0]>>16U);logic(r,r.data[0],0x80000000U,0xffffffffU);
        r.data[1]=rl(h,base+0x26U);logic(r,r.data[1],0x80000000U,0xffffffffU);if(static_cast<std::int32_t>(r.data[1])<0)r.data[1]=negl(r,r.data[1]);
    }
    r.data[0]=lsrl(r,r.data[0],4U);r.data[1]=lsrl(r,r.data[1],4U);const auto divisor=static_cast<std::uint16_t>(r.data[1]);if(divisor==0U)return {TranslationStatus::contract_violation,0U,r.program_counter};
    const auto quotient=r.data[0]/divisor;const auto remainder=r.data[0]%divisor;if(quotient>0xffffU)return {TranslationStatus::contract_violation,0U,r.program_counter};r.data[0]=(remainder<<16U)|quotient;logic(r,static_cast<std::uint16_t>(quotient),0x8000U,0xffffU);
    const auto quotient_word=static_cast<std::uint16_t>(r.data[0]);r.data[0]=(r.data[0]&0xffff0000U)|quotient_word;logic(r,quotient_word,0x8000U,0xffffU);
    const auto count=static_cast<std::uint8_t>(r.data[0]);wb(h,base+0x58U,count);logic(r,count,0x80U,0xffU);const auto decremented=subb(r,rb(h,base+0x58U),1U);wb(h,base+0x58U,decremented);if(decremented==0U)bclr(h,r,base+0x41U,5U);return finish(context);
}

} // namespace gain_ground::translated
