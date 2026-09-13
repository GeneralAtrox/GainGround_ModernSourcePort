#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion=2U;constexpr std::uint16_t kWordMask=0xffffU;struct B{std::uint32_t o;std::uint16_t m;unsigned s;};B loc(std::uint32_t a){const bool x=a&1U;return{a&~1U,static_cast<std::uint16_t>(x?0x00ffU:0xff00U),x?0U:8U};}std::uint8_t rb(ExecutionHost&h,B l){return static_cast<std::uint8_t>(h.read_memory_word(kRegion,l.o,l.m)>>l.s);}void wb(ExecutionHost&h,B l,std::uint8_t v){h.write_memory_word(kRegion,l.o,static_cast<std::uint16_t>(v)<<l.s,l.m);}void logic(CpuRegisters&r,std::uint32_t v,std::uint32_t s){std::uint16_t f=r.status&0x10U;if(v==0)f|=4U;if(v&s)f|=8U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}void cmp(CpuRegisters&r,std::uint16_t a,std::uint16_t b){auto x=r.status&0x10U;auto v=static_cast<std::uint16_t>(a-b);std::uint16_t f=0;if(a<b)f|=1U;if(((a^b)&(a^v)&0x8000U)!=0)f|=2U;if(v==0)f|=4U;if(v&0x8000U)f|=8U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f|x);}std::uint32_t rl(ExecutionHost&h,std::uint32_t a){auto hi=h.read_memory_word(kRegion,a,kWordMask);auto lo=h.read_memory_word(kRegion,a+2U,kWordMask);return(static_cast<std::uint32_t>(hi)<<16U)|lo;}std::uint32_t pop(ExecutionHost&h,CpuRegisters&r){auto v=rl(h,r.address[7]);r.address[7]+=4U;return v;}
} // namespace
FunctionResult cpu_b_refresh_record_index_flag(FunctionContext &context) noexcept
{
    if (!context.host)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;
    const auto base=r.address[5];const auto flag=loc(base+0x78U);auto old=rb(h,flag);wb(h,flag,static_cast<std::uint8_t>(old&~1U));if(old&1U)r.status=static_cast<std::uint16_t>(r.status&~4U);else r.status=static_cast<std::uint16_t>(r.status|4U);
    const auto value=h.read_memory_word(kRegion,base+0x46U,kWordMask);r.data[0]=(r.data[0]&0xffff0000U)|value;logic(r,value,0x8000U);const auto prior=h.read_memory_word(kRegion,base+0x76U,kWordMask);cmp(r,value,prior);if(value!=prior){h.write_memory_word(kRegion,base+0x76U,value,kWordMask);logic(r,value,0x8000U);r.address[0]=rl(h,base+0x50U);const auto source=rb(h,loc(r.address[0]+1U));wb(h,loc(base+9U),source);logic(r,source,0x80U);old=rb(h,flag);wb(h,flag,static_cast<std::uint8_t>(old|1U));if(old&1U)r.status=static_cast<std::uint16_t>(r.status&~4U);else r.status=static_cast<std::uint16_t>(r.status|4U);}const auto target=pop(h,r);r.program_counter=target;return FunctionResult::complete(1U,target);
}
} // namespace gain_ground::translated
