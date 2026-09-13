#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion=1U,kMainRamRegion=3U,kWordMask=0xffffU;
constexpr std::uint32_t kMainRamMask=0x0003ffffU;
void prefetch(ExecutionHost &h,std::uint32_t a){(void)h.read_memory_word(kProgramRegion,a,kWordMask);}
void set_moveq_flags(CpuRegisters &r,std::uint32_t v){std::uint16_t f=r.status&0x10U;if(v&0x80000000U)f|=8U;if(v==0U)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
void push_return(ExecutionHost &h,CpuRegisters &r,std::uint32_t v){r.address[7]-=4U;const auto o=r.address[7]&kMainRamMask;h.write_memory_word(kMainRamRegion,o,static_cast<std::uint16_t>(v>>16U),kWordMask);h.write_memory_word(kMainRamRegion,o+2U,static_cast<std::uint16_t>(v),kWordMask);}
} // namespace
FunctionResult runtime_entry_cpu_a_plain_00003452(FunctionContext &context) noexcept
{
    if(context.host==nullptr)return {TranslationStatus::contract_violation,0U,context.registers.program_counter};
    auto &h=*context.host;auto &r=context.registers;
    r.data[2]=0U;set_moveq_flags(r,0U);prefetch(h,0x00003456U);push_return(h,r,0x00003458U);
    prefetch(h,0x000034bcU);prefetch(h,0x000034beU);r.program_counter=0x000034bcU;
    (void)h.call_function(513U,0U,0xffU,2U,0x00003454U,0x000034bcU,context);
    r.data[2]=2U;set_moveq_flags(r,2U);prefetch(h,0x0000345cU);prefetch(h,0x000034caU);prefetch(h,0x000034ccU);r.program_counter=0x000034caU;
    return h.call_function(514U,0U,0xffU,1U,0x0000345aU,0x000034caU,context);
}
} // namespace gain_ground::translated
