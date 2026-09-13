#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };
[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{ const bool odd=(address&1U)!=0U; return {address&~1U,static_cast<std::uint16_t>(odd?0x00ffU:0xff00U),odd?0U:8U}; }
[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host,std::uint32_t address)
{ const auto l=locate_byte(address); return static_cast<std::uint8_t>(host.read_memory_word(kRegion,l.offset,l.mask)>>l.shift); }
void write_byte(ExecutionHost &host,std::uint32_t address,std::uint8_t value)
{ const auto l=locate_byte(address); host.write_memory_word(kRegion,l.offset,static_cast<std::uint16_t>(value)<<l.shift,l.mask); }
void set_zero_only(CpuRegisters &registers,bool zero)
{ if(zero)registers.status=static_cast<std::uint16_t>(registers.status|0x0004U);else registers.status=static_cast<std::uint16_t>(registers.status&~0x0004U); }
void set_logic_byte_flags(CpuRegisters &registers,std::uint8_t value)
{ std::uint16_t flags=registers.status&0x0010U;if(value==0U)flags|=0x0004U;if(value&0x80U)flags|=0x0008U;registers.status=static_cast<std::uint16_t>((registers.status&~0x001fU)|flags); }
void push_return(ExecutionHost &host,CpuRegisters &registers,std::uint32_t value)
{ registers.address[7]-=4U;host.write_memory_word(kRegion,registers.address[7],static_cast<std::uint16_t>(value>>16U),kWordMask);host.write_memory_word(kRegion,registers.address[7]+2U,static_cast<std::uint16_t>(value),kWordMask); }
[[nodiscard]] FunctionResult finish(FunctionContext &context)
{ auto &host=*context.host;auto &registers=context.registers;const auto high=host.read_memory_word(kRegion,registers.address[7],kWordMask);const auto low=host.read_memory_word(kRegion,registers.address[7]+2U,kWordMask);registers.address[7]+=4U;const auto target=(static_cast<std::uint32_t>(high)<<16U)|low;registers.program_counter=target;return FunctionResult::complete(1U,target); }
} // namespace

FunctionResult cpu_b_clear_record_flag5(FunctionContext &context) noexcept
{
    if(context.host==nullptr)return {TranslationStatus::contract_violation,0U,context.registers.program_counter};
    auto &host=*context.host;auto &registers=context.registers;
    const auto flag_address=registers.address[5]+0x41U;
    const auto old_flags=read_byte(host,flag_address);
    write_byte(host,flag_address,static_cast<std::uint8_t>(old_flags&~0x20U));
    set_zero_only(registers,(old_flags&0x20U)==0U);

    push_return(host,registers,0x0001d90cU);registers.program_counter=0x0001fae6U;
    auto child=host.call_function(374U,1U,0x72U,2U,0x0001d908U,0x0001fae6U,context);
    if(child.status!=TranslationStatus::complete||child.control!=1U)return child;
    const auto selected=read_byte(host,registers.address[5]+0x60U);set_logic_byte_flags(registers,selected);
    if(selected==0U)return finish(context);

    push_return(host,registers,0x0001d918U);registers.program_counter=0x0001da58U;
    child=host.call_function(347U,1U,0x72U,2U,0x0001d914U,0x0001da58U,context);
    if(child.status!=TranslationStatus::complete||child.control!=1U)return child;
    const auto current_flags=read_byte(host,flag_address);
    write_byte(host,flag_address,static_cast<std::uint8_t>(current_flags|0x20U));
    set_zero_only(registers,(current_flags&0x20U)==0U);
    return finish(context);
}
} // namespace gain_ground::translated
