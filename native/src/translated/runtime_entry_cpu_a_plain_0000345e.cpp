#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion=1U,kWordMask=0xffffU;
void prefetch(ExecutionHost &h,std::uint32_t a){(void)h.read_memory_word(kProgramRegion,a,kWordMask);}
void set_moveq_flags(CpuRegisters &r,std::uint32_t v){std::uint16_t f=r.status&0x10U;if(v&0x80000000U)f|=8U;if(v==0U)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
} // namespace
FunctionResult runtime_entry_cpu_a_plain_0000345e(FunctionContext &context) noexcept
{
    if(context.host==nullptr)return {TranslationStatus::contract_violation,0U,context.registers.program_counter};
    auto &h=*context.host;auto &r=context.registers;
    r.data[2]=3U;set_moveq_flags(r,3U);prefetch(h,0x00003462U);prefetch(h,0x000034caU);prefetch(h,0x000034ccU);r.program_counter=0x000034caU;
    return h.call_function(514U,0U,0xffU,1U,0x00003460U,0x000034caU,context);
}
} // namespace gain_ground::translated
