#include "gain_ground/contract_types.h"
#include <cstdint>

namespace gain_ground::translated {
FunctionResult cpu_b_test_entry_flag_8c_bit1_alt3_resume_10b02(
    FunctionContext &context) noexcept;

namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;
struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };
[[nodiscard]] constexpr ByteLocation locate(std::uint32_t a) noexcept
{
    const bool odd=(a&1U)!=0U;
    return {a&~1U,static_cast<std::uint16_t>(odd?0x00ffU:0xff00U),odd?0U:8U};
}
[[nodiscard]] std::uint8_t read_byte(ExecutionHost &h,std::uint32_t a)
{
    const auto l=locate(a); return static_cast<std::uint8_t>(h.read_memory_word(kRegion,l.offset,l.mask)>>l.shift);
}
void write_byte(ExecutionHost &h,std::uint32_t a,std::uint8_t v)
{
    const auto l=locate(a); h.write_memory_word(kRegion,l.offset,static_cast<std::uint16_t>(v)<<l.shift,l.mask);
}
void logic_byte(CpuRegisters &r,std::uint8_t v)
{
    std::uint16_t f=r.status&0x0010U; if(v&0x80U)f|=8U; if(v==0U)f|=4U;
    r.status=static_cast<std::uint16_t>((r.status&~0x001fU)|f);
}
[[nodiscard]] bool test_bit(CpuRegisters &r,std::uint8_t v,std::uint8_t bit)
{
    const bool set=(v&(1U<<bit))!=0U;
    if(set)r.status=static_cast<std::uint16_t>(r.status&~4U); else r.status=static_cast<std::uint16_t>(r.status|4U);
    return set;
}
void push_return(ExecutionHost &h,CpuRegisters &r,std::uint32_t v)
{
    r.address[7]-=4U; h.write_memory_word(kRegion,r.address[7],static_cast<std::uint16_t>(v>>16U),kMask);
    h.write_memory_word(kRegion,r.address[7]+2U,static_cast<std::uint16_t>(v),kMask);
}
[[nodiscard]] std::uint32_t pop_return(ExecutionHost &h,CpuRegisters &r)
{
    const auto hi=h.read_memory_word(kRegion,r.address[7],kMask),lo=h.read_memory_word(kRegion,r.address[7]+2U,kMask);
    r.address[7]+=4U; return (static_cast<std::uint32_t>(hi)<<16U)|lo;
}
[[nodiscard]] FunctionResult finish(FunctionContext &c)
{
    const auto target=pop_return(*c.host,c.registers); c.registers.program_counter=target;
    return FunctionResult::complete(1U,target);
}
} // namespace

FunctionResult cpu_b_test_entry_flag_8c_bit1_alt_resume_10ad0(
    FunctionContext &context) noexcept
{
    if(context.host==nullptr)return {TranslationStatus::contract_violation,0U,context.registers.program_counter};
    auto &h=*context.host; auto &r=context.registers;
    push_return(h,r,0x00010ad4U); r.program_counter=0x00010b28U;
    const auto child=h.call_function(215U,1U,0x72U,2U,0x00010ad0U,0x00010b28U,context);
    if(child.status!=TranslationStatus::complete||child.control!=1U)return child;
    r.program_counter=0x00010b02U;
    return cpu_b_test_entry_flag_8c_bit1_alt3_resume_10b02(context);
}

FunctionResult cpu_b_test_entry_flag_8c_bit1_alt(FunctionContext &context) noexcept
{
    if(context.host==nullptr)return {TranslationStatus::contract_violation,0U,context.registers.program_counter};
    auto &h=*context.host; auto &r=context.registers;
    const bool force=test_bit(r,read_byte(h,r.address[4]+0x8cU),1U);
    if(!force){
        const auto active=read_byte(h,r.address[5]+0x40U); logic_byte(r,active);
        if(active!=0U)return finish(context);
        if(!test_bit(r,read_byte(h,r.address[4]+0x8bU),1U))return finish(context);
    }
    r.address[0]=0x00011582U+static_cast<std::int16_t>(static_cast<std::uint16_t>(r.data[7]));
    const auto mode=read_byte(h,r.address[0]);
    write_byte(h,r.address[5]+0x40U,mode); logic_byte(r,mode);
    r.program_counter=0x00010ad0U;
    return cpu_b_test_entry_flag_8c_bit1_alt_resume_10ad0(context);
}
} // namespace gain_ground::translated
