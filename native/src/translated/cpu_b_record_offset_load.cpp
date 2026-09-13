#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
std::uint32_t read_long(ExecutionHost &h, std::uint32_t a) { const auto hi=h.read_memory_word(kRegion,a,kWordMask); const auto lo=h.read_memory_word(kRegion,a+2U,kWordMask); return (static_cast<std::uint32_t>(hi)<<16U)|lo; }
void logic(CpuRegisters&r,std::uint32_t v,std::uint32_t s){std::uint16_t f=r.status&0x10U;if(v==0)f|=4;if(v&s)f|=8;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void subw(CpuRegisters&r,std::uint16_t a,std::uint16_t b,std::uint16_t v,bool preserve_x=false){auto x=r.status&0x10U;std::uint16_t f=0;if(a<b)f|=0x11;if(((a^b)&(a^v)&0x8000U)!=0)f|=2;if(v==0)f|=4;if(v&0x8000U)f|=8;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);if(preserve_x)r.status=static_cast<std::uint16_t>((r.status&~0x10U)|x);}
void addw(CpuRegisters&r,std::uint16_t a,std::uint16_t b,std::uint16_t v){std::uint16_t f=0;if(static_cast<std::uint32_t>(a)+b>0xffffU)f|=0x11;if(((~(a^b))&(a^v)&0x8000U)!=0)f|=2;if(v==0)f|=4;if(v&0x8000U)f|=8;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void addl(CpuRegisters&r,std::uint32_t a,std::uint32_t b,std::uint32_t v){std::uint16_t f=0;if(static_cast<std::uint64_t>(a)+b>0xffffffffULL)f|=0x11;if(((~(a^b))&(a^v)&0x80000000U)!=0)f|=2;if(v==0)f|=4;if(v&0x80000000U)f|=8;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
bool gt(const CpuRegisters&r){return !(r.status&4U)&&((bool(r.status&8U))==(bool(r.status&2U)));} bool mi(const CpuRegisters&r){return(r.status&8U)!=0;}
std::uint32_t pop(ExecutionHost&h,CpuRegisters&r){auto hi=h.read_memory_word(kRegion,r.address[7],kWordMask);auto lo=h.read_memory_word(kRegion,r.address[7]+2U,kWordMask);r.address[7]+=4U;return(static_cast<std::uint32_t>(hi)<<16U)|lo;}
} // namespace
FunctionResult cpu_b_record_offset_load(FunctionContext &context) noexcept
{
    if (!context.host)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;
    auto d0=read_long(h,r.address[5]+0x1eU);r.data[0]=d0;logic(r,d0,0x80000000U);auto src=read_long(h,r.address[5]+0x12U);auto sum=d0+src;addl(r,d0,src,sum);r.data[0]=sum;
    d0=(sum<<16U)|(sum>>16U);r.data[0]=d0;logic(r,d0,0x80000000U);auto w=static_cast<std::uint16_t>(d0);auto v=static_cast<std::uint16_t>(w-0x000aU);subw(r,w,0x000aU,v);r.data[0]=(r.data[0]&0xffff0000U)|v;
    subw(r,v,static_cast<std::uint16_t>(r.data[2]),static_cast<std::uint16_t>(v-static_cast<std::uint16_t>(r.data[2])),true);if(gt(r))goto done;
    w=v;v=static_cast<std::uint16_t>(w+0x0014U);addw(r,w,0x0014U,v);r.data[0]=(r.data[0]&0xffff0000U)|v;subw(r,v,static_cast<std::uint16_t>(r.data[1]),static_cast<std::uint16_t>(v-static_cast<std::uint16_t>(r.data[1])),true);if(mi(r))goto done;
    d0=read_long(h,r.address[5]+0x26U);r.data[0]=d0;logic(r,d0,0x80000000U);src=read_long(h,r.address[5]+0x1aU);sum=d0+src;addl(r,d0,src,sum);r.data[0]=sum;d0=(sum<<16U)|(sum>>16U);r.data[0]=d0;logic(r,d0,0x80000000U);w=static_cast<std::uint16_t>(d0);v=static_cast<std::uint16_t>(w-9U);subw(r,w,9U,v);r.data[0]=(r.data[0]&0xffff0000U)|v;
    subw(r,v,static_cast<std::uint16_t>(r.data[4]),static_cast<std::uint16_t>(v-static_cast<std::uint16_t>(r.data[4])),true);if(gt(r))goto done;w=v;v=static_cast<std::uint16_t>(w+0x12U);addw(r,w,0x12U,v);r.data[0]=(r.data[0]&0xffff0000U)|v;subw(r,v,static_cast<std::uint16_t>(r.data[3]),static_cast<std::uint16_t>(v-static_cast<std::uint16_t>(r.data[3])),true);if(mi(r))goto done;
    {const auto a=r.address[5]+0x40U;const auto old=static_cast<std::uint8_t>(h.read_memory_word(kRegion,a&~1U,(a&1U)?0x00ffU:0xff00U)>>((a&1U)?0U:8U));h.write_memory_word(kRegion,a&~1U,static_cast<std::uint16_t>(old|2U)<<((a&1U)?0U:8U),(a&1U)?0x00ffU:0xff00U);if(old&2U)r.status=static_cast<std::uint16_t>(r.status&~4U);else r.status=static_cast<std::uint16_t>(r.status|4U);}
done: const auto target=pop(h,r);r.program_counter=target;return FunctionResult::complete(1U,target);
}
} // namespace gain_ground::translated
