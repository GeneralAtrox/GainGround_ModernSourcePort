#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };
ByteLocation locate(std::uint32_t a){const bool o=a&1U;return{a&~1U,static_cast<std::uint16_t>(o?0x00ffU:0xff00U),o?0U:8U};}
std::uint8_t readb(ExecutionHost&h,ByteLocation l){return static_cast<std::uint8_t>(h.read_memory_word(kRegion,l.offset,l.mask)>>l.shift);}
void writeb(ExecutionHost&h,ByteLocation l,std::uint8_t v){h.write_memory_word(kRegion,l.offset,static_cast<std::uint16_t>(v)<<l.shift,l.mask);}
void logic(CpuRegisters&r,std::uint32_t v,std::uint32_t s){std::uint16_t f=r.status&0x10U;if(v==0)f|=4U;if(v&s)f|=8U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void subw(CpuRegisters&r,std::uint16_t a,std::uint16_t b,std::uint16_t v){std::uint16_t f=0;if(a<b)f|=0x11U;if(((a^b)&(a^v)&0x8000U)!=0)f|=2U;if(v==0)f|=4U;if(v&0x8000U)f|=8U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void addw(CpuRegisters&r,std::uint16_t a,std::uint16_t b,std::uint16_t v){std::uint16_t f=0;if(static_cast<std::uint32_t>(a)+b>0xffffU)f|=0x11U;if(((~(a^b))&(a^v)&0x8000U)!=0)f|=2U;if(v==0)f|=4U;if(v&0x8000U)f|=8U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
std::uint32_t pop(ExecutionHost&h,CpuRegisters&r){const auto hi=h.read_memory_word(kRegion,r.address[7],kWordMask);const auto lo=h.read_memory_word(kRegion,r.address[7]+2U,kWordMask);r.address[7]+=4U;return(static_cast<std::uint32_t>(hi)<<16U)|lo;}
FunctionResult finish(ExecutionHost&h,CpuRegisters&r){const auto t=pop(h,r);r.program_counter=t;return FunctionResult::complete(1U,t);}
} // namespace
FunctionResult cpu_b_align_record_position_and_mark(FunctionContext &context) noexcept
{
    if (!context.host)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    const auto active=readb(h,locate(base+0x60U));logic(r,active,0x80U);if(active!=0U)return finish(h,r);
    auto d0=h.read_memory_word(kRegion,base+0x62U,kWordMask);r.data[0]=(r.data[0]&0xffff0000U)|d0;logic(r,d0,0x8000U);auto source=h.read_memory_word(kRegion,base+0x12U,kWordMask);auto value=static_cast<std::uint16_t>(d0-source);subw(r,d0,source,value);r.data[0]=(r.data[0]&0xffff0000U)|value;
    auto d1=h.read_memory_word(kRegion,base+0x64U,kWordMask);r.data[1]=(r.data[1]&0xffff0000U)|d1;logic(r,d1,0x8000U);source=h.read_memory_word(kRegion,base+0x1aU,kWordMask);auto value1=static_cast<std::uint16_t>(d1-source);subw(r,d1,source,value1);r.data[1]=(r.data[1]&0xffff0000U)|value1;
    auto next=static_cast<std::uint16_t>(value+7U);addw(r,value,7U,next);r.data[0]=(r.data[0]&0xffff0000U)|next;value=static_cast<std::uint16_t>(next&0xfff0U);r.data[0]=(r.data[0]&0xffff0000U)|value;logic(r,value,0x8000U);if(value!=0U)return finish(h,r);
    next=static_cast<std::uint16_t>(value1+7U);addw(r,value1,7U,next);r.data[1]=(r.data[1]&0xffff0000U)|next;value1=static_cast<std::uint16_t>(next&0xfff0U);r.data[1]=(r.data[1]&0xffff0000U)|value1;logic(r,value1,0x8000U);if(value1!=0U)return finish(h,r);
    h.write_memory_word(kRegion,base+0x5cU,0x0200U,kWordMask);logic(r,0x0200U,0x8000U);const auto l=locate(base+0x40U);const auto old=readb(h,l);writeb(h,l,static_cast<std::uint8_t>(old|2U));if(old&2U)r.status=static_cast<std::uint16_t>(r.status&~4U);else r.status=static_cast<std::uint16_t>(r.status|4U);return finish(h,r);
}
} // namespace gain_ground::translated
