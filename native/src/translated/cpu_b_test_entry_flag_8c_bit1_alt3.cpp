#include "gain_ground/contract_types.h"
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion=2U,kMask=0xffffU;
struct ByteLocation{std::uint32_t offset;std::uint16_t mask;unsigned shift;};
[[nodiscard]] constexpr ByteLocation locate(std::uint32_t a) noexcept
{const bool o=(a&1U)!=0U;return {a&~1U,static_cast<std::uint16_t>(o?0x00ffU:0xff00U),o?0U:8U};}
[[nodiscard]] std::uint8_t read_byte(ExecutionHost&h,std::uint32_t a)
{const auto l=locate(a);return static_cast<std::uint8_t>(h.read_memory_word(kRegion,l.offset,l.mask)>>l.shift);}
void write_byte(ExecutionHost&h,std::uint32_t a,std::uint8_t v)
{const auto l=locate(a);h.write_memory_word(kRegion,l.offset,static_cast<std::uint16_t>(v)<<l.shift,l.mask);}
[[nodiscard]] std::uint32_t read_long(ExecutionHost&h,std::uint32_t a)
{return (static_cast<std::uint32_t>(h.read_memory_word(kRegion,a,kMask))<<16U)|h.read_memory_word(kRegion,a+2U,kMask);}
void logic(CpuRegisters&r,std::uint32_t v,std::uint32_t sign,std::uint32_t mask)
{std::uint16_t f=r.status&0x10U;if(v&sign)f|=8U;if((v&mask)==0U)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
[[nodiscard]] bool test_bit(CpuRegisters&r,std::uint8_t v,std::uint8_t b)
{const bool s=(v&(1U<<b))!=0U;if(s)r.status=static_cast<std::uint16_t>(r.status&~4U);else r.status=static_cast<std::uint16_t>(r.status|4U);return s;}
void push_return(ExecutionHost&h,CpuRegisters&r,std::uint32_t v)
{r.address[7]-=4U;h.write_memory_word(kRegion,r.address[7],static_cast<std::uint16_t>(v>>16U),kMask);h.write_memory_word(kRegion,r.address[7]+2U,static_cast<std::uint16_t>(v),kMask);}
[[nodiscard]] std::uint32_t pop_return(ExecutionHost&h,CpuRegisters&r)
{const auto hi=h.read_memory_word(kRegion,r.address[7],kMask),lo=h.read_memory_word(kRegion,r.address[7]+2U,kMask);r.address[7]+=4U;return(static_cast<std::uint32_t>(hi)<<16U)|lo;}
[[nodiscard]] FunctionResult finish(FunctionContext&c)
{const auto t=pop_return(*c.host,c.registers);c.registers.program_counter=t;return FunctionResult::complete(1U,t);}
[[nodiscard]] std::uint32_t function_id(std::uint32_t target)
{
    switch(target){case 0x00010b58U:return 217U;case 0x00010b92U:return 218U;
    case 0x00010bd2U:return 219U;case 0x00010c0eU:return 220U;
    case 0x00010c3eU:return 221U;case 0x00010c8aU:return 222U;default:return 0xffffffffU;}
}
[[nodiscard]] FunctionResult call_indirect(FunctionContext&c,std::uint32_t callsite,
    std::uint32_t continuation,std::uint32_t target)
{
    const auto id=function_id(target);if(id==0xffffffffU)return {TranslationStatus::contract_violation,0U,target};
    push_return(*c.host,c.registers,continuation);c.registers.program_counter=target;
    return c.host->call_function(id,1U,0x72U,2U,callsite,target,c);
}
} // namespace

FunctionResult cpu_b_test_entry_flag_8c_bit1_alt3_resume_10b02(FunctionContext &context) noexcept
{
    if(context.host==nullptr)return {TranslationStatus::contract_violation,0U,context.registers.program_counter};
    auto&h=*context.host;auto&r=context.registers;
    std::uint32_t target=read_long(h,r.address[3]+0x0cU);r.address[0]=target;
    auto child=call_indirect(context,0x00010b06U,0x00010b08U,target);
    if(child.status!=TranslationStatus::complete||child.control!=1U)return child;
    target=read_long(h,r.address[3]+0x14U);r.address[0]=target;
    child=call_indirect(context,0x00010b0cU,0x00010b0eU,target);
    if(child.status!=TranslationStatus::complete||child.control!=1U)return child;
    target=read_long(h,r.address[3]+0x1cU);r.address[0]=target;
    child=call_indirect(context,0x00010b12U,0x00010b14U,target);
    if(child.status!=TranslationStatus::complete||child.control!=1U)return child;

    const auto command=h.read_memory_word(kRegion,r.address[3]+0x24U,kMask);
    r.data[0]=(r.data[0]&0xffff0000U)|command;logic(r,command,0x8000U,0xffffU);
    r.address[7]-=2U;h.write_memory_word(kRegion,r.address[7],static_cast<std::uint16_t>(r.data[7]),kMask);
    r.address[7]-=4U;h.write_memory_word(kRegion,r.address[7]+2U,static_cast<std::uint16_t>(r.address[4]),kMask);
    h.write_memory_word(kRegion,r.address[7],static_cast<std::uint16_t>(r.address[4]>>16U),kMask);
    push_return(h,r,0x00010b22U);r.program_counter=0x00016ff8U;
    child=h.call_function(308U,1U,0x72U,2U,0x00010b1cU,0x00016ff8U,context);
    if(child.status!=TranslationStatus::complete||child.control!=1U)return child;
    r.address[4]=read_long(h,r.address[7]);r.address[7]+=4U;
    const auto d7=h.read_memory_word(kRegion,r.address[7],kMask);r.address[7]+=2U;
    r.data[7]=(r.data[7]&0xffff0000U)|d7;logic(r,d7,0x8000U,0xffffU);
    return finish(context);
}

FunctionResult cpu_b_test_entry_flag_8c_bit1_alt3(FunctionContext &context) noexcept
{
    if(context.host==nullptr)return {TranslationStatus::contract_violation,0U,context.registers.program_counter};
    auto&h=*context.host;auto&r=context.registers;
    const bool force=test_bit(r,read_byte(h,r.address[4]+0x8cU),1U);
    if(!force){
        const auto active=read_byte(h,r.address[5]+0x40U);logic(r,active,0x80U,0xffU);
        if(active!=0U)return finish(context);
        if(!test_bit(r,read_byte(h,r.address[4]+0x8bU),1U))return finish(context);
    }
    r.address[0]=0x00011582U+static_cast<std::int16_t>(static_cast<std::uint16_t>(r.data[7]));
    const auto mode=read_byte(h,r.address[0]);write_byte(h,r.address[5]+0x40U,mode);logic(r,mode,0x80U,0xffU);
    r.program_counter=0x00010b02U;
    return cpu_b_test_entry_flag_8c_bit1_alt3_resume_10b02(context);
}
} // namespace gain_ground::translated
