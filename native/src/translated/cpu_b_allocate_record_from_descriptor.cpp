#include "gain_ground/contract_types.h"

#include <cstddef>
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;

std::uint16_t read_word(ExecutionHost &h, std::uint32_t a)
{ return h.read_memory_word(kRegion, a, kMask); }
void write_word(ExecutionHost &h, std::uint32_t a, std::uint16_t v)
{ h.write_memory_word(kRegion, a, v, kMask); }
std::uint8_t read_byte(ExecutionHost &h, std::uint32_t a)
{
    const bool odd=(a&1U)!=0U;
    const auto v=h.read_memory_word(kRegion,a&~1U,odd?0x00ffU:0xff00U);
    return static_cast<std::uint8_t>(odd?v:v>>8U);
}
void write_byte(ExecutionHost &h,std::uint32_t a,std::uint8_t v)
{
    const bool odd=(a&1U)!=0U;
    h.write_memory_word(kRegion,a&~1U,
        static_cast<std::uint16_t>(v)<<(odd?0U:8U),odd?0x00ffU:0xff00U);
}
std::uint32_t read_long(ExecutionHost &h,std::uint32_t a)
{ return (static_cast<std::uint32_t>(read_word(h,a))<<16U)|read_word(h,a+2U); }
void write_long(ExecutionHost &h,std::uint32_t a,std::uint32_t v)
{ write_word(h,a,static_cast<std::uint16_t>(v>>16U)); write_word(h,a+2U,static_cast<std::uint16_t>(v)); }
void set_word(CpuRegisters &r,std::size_t i,std::uint16_t v)
{ r.data[i]=(r.data[i]&0xffff0000U)|v; }
void set_byte(CpuRegisters &r,std::size_t i,std::uint8_t v)
{ r.data[i]=(r.data[i]&0xffffff00U)|v; }
void logic(CpuRegisters &r,std::uint32_t v,std::uint32_t sign,std::uint32_t mask)
{
    std::uint16_t f=r.status&0x10U;
    if(v&sign) f|=8U;
    if((v&mask)==0U) f|=4U;
    r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);
}
void sub_word_flags(CpuRegisters &r,std::uint16_t l,std::uint16_t q,std::uint16_t v)
{
    std::uint16_t f{};
    if(l<q) f|=0x11U;
    if(((l^q)&(l^v)&0x8000U)!=0U) f|=2U;
    if(v==0U) f|=4U;
    if(v&0x8000U) f|=8U;
    r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);
}
void sub_byte_flags(CpuRegisters &r,std::uint8_t l,std::uint8_t q,std::uint8_t v)
{
    std::uint16_t f{};
    if(l<q) f|=0x11U;
    if(((l^q)&(l^v)&0x80U)!=0U) f|=2U;
    if(v==0U) f|=4U;
    if(v&0x80U) f|=8U;
    r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);
}
void add_word_flags(CpuRegisters &r,std::uint16_t l,std::uint16_t q,std::uint16_t v)
{
    std::uint16_t f{};
    if(static_cast<std::uint32_t>(l)+q>0xffffU) f|=0x11U;
    if(((~(l^q))&(l^v)&0x8000U)!=0U) f|=2U;
    if(v==0U) f|=4U;
    if(v&0x8000U) f|=8U;
    r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);
}
void bit_zero(CpuRegisters &r,bool old_set)
{ r.status=static_cast<std::uint16_t>(old_set?r.status&~4U:r.status|4U); }
void push_return(ExecutionHost &h,CpuRegisters &r,std::uint32_t v)
{ r.address[7]-=4U; write_long(h,r.address[7],v); }
FunctionResult pop_return(ExecutionHost &h,CpuRegisters &r)
{
    const auto target=read_long(h,r.address[7]); r.address[7]+=4U;
    r.program_counter=target; return FunctionResult::complete(1U,target);
}
FunctionResult call(ExecutionHost &h,FunctionContext &c,std::uint32_t id,
    std::uint32_t site,std::uint32_t target,std::uint32_t continuation)
{
    push_return(h,c.registers,continuation); c.registers.program_counter=target;
    return h.call_function(id,1U,0x72U,2U,site,target,c);
}
bool signed_less_equal(const CpuRegisters &r)
{ return (r.status&4U)!=0U || ((r.status&8U)!=0U)!=((r.status&2U)!=0U); }
} // namespace

FunctionResult cpu_b_allocate_record_from_descriptor(FunctionContext &context) noexcept
{
    if(context.host==nullptr) return {TranslationStatus::contract_violation,0U,context.registers.program_counter};
    auto &h=*context.host; auto &r=context.registers;

    r.data[0]=7U; logic(r,7U,0x80000000U,0xffffffffU);
    r.address[4]=r.address[5]+0x22U;
    for(;;){
        const auto first=read_byte(h,r.address[4]); logic(r,first,0x80U,0xffU);
        if((first&0x80U)!=0U) return pop_return(h,r);
        if(first!=0U){
            const auto second=read_byte(h,r.address[4]+1U); logic(r,second,0x80U,0xffU);
            if(!signed_less_equal(r)){
                r.address[0]=read_long(h,r.address[2]); r.address[2]+=4U;
                r.address[2]=read_long(h,r.address[2]);
                break;
            }
        }
        r.address[2]+=8U; r.address[4]+=2U;
        auto d0=static_cast<std::uint16_t>(r.data[0]); d0=static_cast<std::uint16_t>(d0-1U); set_word(r,0U,d0);
        if(d0==0xffffU) return pop_return(h,r);
    }

    auto d5=read_word(h,r.address[2]); r.address[2]+=2U; set_word(r,5U,d5); logic(r,d5,0x8000U,0xffffU);
    for(;;){
        auto child=call(h,context,421U,0x00023960U,0x00023970U,0x00023962U);
        if(child.status!=TranslationStatus::complete||child.control!=1U) return child;
        if((r.status&1U)==0U) break;
        r.address[2]+=6U;
        d5=static_cast<std::uint16_t>(static_cast<std::uint16_t>(r.data[5])-1U); set_word(r,5U,d5);
        if(d5==0xffffU) return pop_return(h,r);
    }

    r.address[6]=read_long(h,r.address[0]);
    r.data[0]=0U; logic(r,0U,0x80000000U,0xffffffffU);
    std::uint16_t d0=read_byte(h,r.address[6]+8U); set_byte(r,0U,static_cast<std::uint8_t>(d0)); logic(r,d0,0x80U,0xffU);
    set_word(r,1U,0x003fU); logic(r,0x003fU,0x8000U,0xffffU);
    r.address[6]=0x00005400U;
    for(;;){
        r.address[6]-=0x80U;
        const auto occupied=read_byte(h,r.address[6]); logic(r,occupied,0x80U,0xffU);
        if((occupied&0x80U)==0U){
            const auto before=static_cast<std::uint16_t>(r.data[0]);
            d0=static_cast<std::uint16_t>(before-1U); set_word(r,0U,d0); sub_word_flags(r,before,1U,d0);
            if(d0==0U) break;
        }
        auto d1=static_cast<std::uint16_t>(r.data[1]); d1=static_cast<std::uint16_t>(d1-1U); set_word(r,1U,d1);
        if(d1==0xffffU) return pop_return(h,r);
    }

    auto child=call(h,context,417U,0x00023a48U,0x000238daU,0x00023a4cU);
    if(child.status!=TranslationStatus::complete||child.control!=1U) return child;
    write_word(h,r.address[6],0x8000U); logic(r,0x8000U,0x8000U,0xffffU);
    r.address[1]=read_long(h,r.address[0]); r.address[0]+=4U;
    auto value_long=read_long(h,r.address[0]); r.address[0]+=4U; write_long(h,r.address[6]+2U,value_long); logic(r,value_long,0x80000000U,0xffffffffU);
    d0=read_word(h,r.address[2]); set_word(r,0U,d0); logic(r,d0,0x8000U,0xffffU);
    for(const auto off:{0x12U,0x2aU,0x2cU}){write_word(h,r.address[6]+off,d0); logic(r,d0,0x8000U,0xffffU);}
    auto v=read_word(h,r.address[0]); r.address[0]+=2U; write_word(h,r.address[6]+0x62U,v); logic(r,v,0x8000U,0xffffU);
    d0=read_word(h,r.address[0]); r.address[0]+=2U; set_word(r,0U,d0); logic(r,d0,0x8000U,0xffffU);
    for(const auto off:{0x16U,0x2eU,0x30U}){write_word(h,r.address[6]+off,d0); logic(r,d0,0x8000U,0xffffU);}
    d0=read_word(h,r.address[2]+2U); set_word(r,0U,d0); logic(r,d0,0x8000U,0xffffU);
    for(const auto off:{0x1aU,0x32U,0x34U}){write_word(h,r.address[6]+off,d0); logic(r,d0,0x8000U,0xffffU);}
    v=read_word(h,r.address[0]); r.address[0]+=2U; write_word(h,r.address[6]+0x64U,v); logic(r,v,0x8000U,0xffffU);
    d0=read_word(h,r.address[0]); r.address[0]+=2U; set_word(r,0U,d0); logic(r,d0,0x8000U,0xffffU);
    write_byte(h,r.address[6]+0x36U,static_cast<std::uint8_t>(d0)); logic(r,static_cast<std::uint8_t>(d0),0x80U,0xffU);
    write_byte(h,r.address[6]+0x37U,static_cast<std::uint8_t>(d0)); logic(r,static_cast<std::uint8_t>(d0),0x80U,0xffU);
    for(const auto off:{0x5eU,0x60U}){v=read_word(h,r.address[0]);r.address[0]+=2U;write_word(h,r.address[6]+off,v);logic(r,v,0x8000U,0xffffU);}
    for(const auto off:{0x66U,0x6eU}){value_long=read_long(h,r.address[0]);r.address[0]+=4U;write_long(h,r.address[6]+off,value_long);logic(r,value_long,0x80000000U,0xffffffffU);}
    v=read_word(h,r.address[2]+4U); write_word(h,r.address[6]+0x5cU,v); logic(r,v,0x8000U,0xffffU);
    v=read_word(h,r.address[2]+4U); write_word(h,r.address[6]+0x3aU,v); logic(r,v,0x8000U,0xffffU);
    auto b=read_byte(h,r.address[6]+0x41U); write_byte(h,r.address[6]+0x41U,static_cast<std::uint8_t>(b|8U)); bit_zero(r,(b&8U)!=0U);
    write_byte(h,r.address[6]+0x0bU,8U); logic(r,8U,0x80U,0xffU);
    write_word(h,r.address[6]+0x74U,0xffffU); logic(r,0xffffU,0x8000U,0xffffU);
    for(const auto off:{0x48U,0x50U}){value_long=read_long(h,r.address[1]);r.address[1]+=4U;write_long(h,r.address[6]+off,value_long);logic(r,value_long,0x80000000U,0xffffffffU);}
    for(const auto off:{0x38U,0x3cU}){b=read_byte(h,r.address[1]++);write_byte(h,r.address[6]+off,b);logic(r,b,0x80U,0xffU);}
    for(const auto off:{0x42U,0x44U}){v=read_word(h,r.address[1]);r.address[1]+=2U;write_word(h,r.address[6]+off,v);logic(r,v,0x8000U,0xffffU);}
    r.data[1]=0U; logic(r,0U,0x80000000U,0xffffffffU);
    b=read_byte(h,r.address[1]++);set_byte(r,1U,b);logic(r,b,0x80U,0xffU);write_word(h,r.address[6]+0x46U,static_cast<std::uint16_t>(r.data[1]));logic(r,static_cast<std::uint16_t>(r.data[1]),0x8000U,0xffffU);
    child=call(h,context,418U,0x00023ae2U,0x000238ecU,0x00023ae6U);
    if(child.status!=TranslationStatus::complete||child.control!=1U) return child;
    write_word(h,r.address[6]+0x54U,static_cast<std::uint16_t>(r.data[6]));logic(r,static_cast<std::uint16_t>(r.data[6]),0x8000U,0xffffU);
    write_word(h,r.address[6]+0x56U,0U);logic(r,0U,0x8000U,0xffffU);
    b=read_byte(h,r.address[6]+0x40U);write_byte(h,r.address[6]+0x40U,static_cast<std::uint8_t>(b|4U));bit_zero(r,(b&4U)!=0U);
    b=read_byte(h,r.address[6]+0x40U);write_byte(h,r.address[6]+0x40U,static_cast<std::uint8_t>(b|8U));bit_zero(r,(b&8U)!=0U);
    b=read_byte(h,r.address[6]+0x40U);write_byte(h,r.address[6]+0x40U,static_cast<std::uint8_t>(b&~2U));bit_zero(r,(b&2U)!=0U);
    set_word(r,1U,static_cast<std::uint16_t>(r.address[6]));logic(r,static_cast<std::uint16_t>(r.address[6]),0x8000U,0xffffU);
    r.data[0]=0U;logic(r,0U,0x80000000U,0xffffffffU);
    b=read_byte(h,r.address[6]+0x38U);set_byte(r,0U,b);logic(r,b,0x80U,0xffU);
    auto db=static_cast<std::uint8_t>(b-2U);set_byte(r,0U,db);sub_byte_flags(r,b,2U,db);
    if((r.status&8U)==0U){
        r.data[2]=6U;logic(r,6U,0x80000000U,0xffffffffU);
        for(;;){
            r.address[6]+=0x80U;
            const auto used=read_byte(h,r.address[6]);logic(r,used,0x80U,0xffU);
            if((used&0x80U)!=0U) continue;
            write_word(h,r.address[6],0x8000U);logic(r,0x8000U,0x8000U,0xffffU);
            write_word(h,r.address[6]+0x36U,static_cast<std::uint16_t>(r.data[1]));logic(r,static_cast<std::uint16_t>(r.data[1]),0x8000U,0xffffU);
            write_byte(h,r.address[6]+0x0bU,0U);logic(r,0U,0x80U,0xffU);
            write_long(h,r.address[6]+2U,0x0001ef62U);logic(r,0x0001ef62U,0x80000000U,0xffffffffU);
            write_word(h,r.address[6]+0x38U,static_cast<std::uint16_t>(r.data[2]));logic(r,static_cast<std::uint16_t>(r.data[2]),0x8000U,0xffffU);
            const auto d2=static_cast<std::uint16_t>(r.data[2]);const auto next=static_cast<std::uint16_t>(d2+6U);set_word(r,2U,next);add_word_flags(r,d2,6U,next);
            auto count=static_cast<std::uint16_t>(r.data[0]);count=static_cast<std::uint16_t>(count-1U);set_word(r,0U,count);
            if(count==0xffffU) break;
        }
    }
    const auto old=read_word(h,r.address[4]);const auto result=static_cast<std::uint16_t>(old-0x0101U);write_word(h,r.address[4],result);sub_word_flags(r,old,0x0101U,result);
    return pop_return(h,r);
}

} // namespace gain_ground::translated
