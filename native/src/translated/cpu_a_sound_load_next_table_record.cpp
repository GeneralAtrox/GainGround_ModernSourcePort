#include "gain_ground/contract_types.h"
#include "gain_ground/sound_caller_timing.h"

#include <cstdint>

namespace gain_ground::translated {
FunctionResult cpu_a_sound_reset_and_build_channel_records(FunctionContext&) noexcept;
namespace {
constexpr std::uint16_t kRegion=3U;
constexpr std::uint32_t kAddressMask=0x0003ffffU;
std::uint16_t rw(SoundCallerTiming& h,std::uint32_t a){return h.read_memory_word(kRegion,a&kAddressMask,0xffffU);}
void ww(SoundCallerTiming& h,std::uint32_t a,std::uint16_t v){h.write_memory_word(kRegion,a&kAddressMask,v,0xffffU);}
std::uint32_t rl(SoundCallerTiming& h,std::uint32_t a){return (static_cast<std::uint32_t>(rw(h,a))<<16U)|rw(h,a+2U);}
void wl(SoundCallerTiming& h,std::uint32_t a,std::uint32_t v){ww(h,a,static_cast<std::uint16_t>(v>>16U));ww(h,a+2U,static_cast<std::uint16_t>(v));}
void pf(SoundCallerTiming& h,std::uint32_t a){(void)rw(h,a);}
void logicw(CpuRegisters&r,std::uint16_t v){r.status=static_cast<std::uint16_t>((r.status&~0x000fU)|(v==0U?4U:0U)|(v&0x8000U?8U:0U));}
void addw(CpuRegisters&r,std::uint16_t l,std::uint16_t q,std::uint16_t v){std::uint16_t f=0U;if(v==0U)f|=4U;if(v&0x8000U)f|=8U;if(((~(l^q))&(l^v)&0x8000U)!=0U)f|=2U;if(static_cast<std::uint32_t>(l)+q>0xffffU)f|=0x11U;r.status=static_cast<std::uint16_t>((r.status&~0x001fU)|f);}
void subw(CpuRegisters&r,std::uint16_t l,std::uint16_t q,std::uint16_t v){std::uint16_t f=0U;if(v==0U)f|=4U;if(v&0x8000U)f|=8U;if(((l^q)&(l^v)&0x8000U)!=0U)f|=2U;if(q>l)f|=0x11U;r.status=static_cast<std::uint16_t>((r.status&~0x001fU)|f);}
void push(SoundCallerTiming&h,CpuRegisters&r,std::uint32_t v){r.address[7]-=4U;wl(h,r.address[7],v);}
FunctionResult call(SoundCallerTiming&h,FunctionContext&c,std::uint32_t id,std::uint32_t site,std::uint32_t target,std::uint32_t ret){auto&r=c.registers;h.clocks(2U);push(h,r,ret);pf(h,target);pf(h,target+2U);r.program_counter=target;return h.call_function(id,0U,0xffU,2U,site,target,c);}
FunctionResult finish(SoundCallerTiming&h,CpuRegisters&r){const auto t=rl(h,r.address[7]);r.address[7]+=4U;pf(h,t);pf(h,t+2U);r.program_counter=t;return FunctionResult::complete(1U,t);}
}

FunctionResult cpu_a_sound_load_next_table_record(FunctionContext& context) noexcept
{
    if(context.host==nullptr)return {TranslationStatus::contract_violation,0U,context.registers.program_counter};
    SoundCallerTiming h(context);auto&r=context.registers;
    h.begin(0x83cccU);
    r.data[0]=0U;logicw(r,0U);
    r.data[1]=0U;logicw(r,0U);pf(h,0x00083cd0U);pf(h,0x00083cd2U);
    r.address[0]=r.address[6]+0x60U;

    pf(h,0x00083cd4U);pf(h,0x00083cd6U);pf(h,0x00083cd8U);
    const auto timer=rw(h,r.address[0]+0x20U);const auto next_timer=static_cast<std::uint16_t>(timer-1U);
    pf(h,0x00083cdaU);
    ww(h,r.address[0]+0x20U,next_timer);subw(r,timer,1U,next_timer);
    pf(h,0x00083cdcU);
    const auto cursor=rw(h,r.address[0]+0x24U);
    r.data[0]=(r.data[0]&0xffff0000U)|cursor;logicw(r,cursor);
    pf(h,0x00083cdeU);
    const auto masked=static_cast<std::uint16_t>(cursor&0x001eU);r.data[0]=(r.data[0]&0xffff0000U)|masked;logicw(r,masked);
    pf(h,0x00083ce0U);pf(h,0x00083ce2U);h.clocks(2U);pf(h,0x00083ce4U);
    const auto record_index=rw(h,r.address[0]+static_cast<std::int16_t>(masked));r.data[0]=(r.data[0]&0xffff0000U)|record_index;logicw(r,record_index);
    pf(h,0x00083ce6U);pf(h,0x00083ce8U);const auto cursor2=rw(h,r.address[0]+0x24U);const auto next_cursor=static_cast<std::uint16_t>(cursor2+2U);
    pf(h,0x00083ceaU);ww(h,r.address[0]+0x24U,next_cursor);addw(r,cursor2,2U,next_cursor);
    pf(h,0x00083cecU);const auto table_base=rw(h,r.address[5]+4U);r.data[1]=(r.data[1]&0xffff0000U)|table_base;logicw(r,table_base);pf(h,0x00083ceeU);
    h.clocks(2U);pf(h,0x00083cf0U);h.clocks(2U);pf(h,0x00083cf2U);r.address[0]=r.address[5]+r.data[1];
    h.clocks(2U);pf(h,0x00083cf4U);
    const auto value=rw(h,r.address[0]+static_cast<std::int16_t>(record_index));r.data[1]=(r.data[1]&0xffff0000U)|value;logicw(r,value);
    pf(h,0x00083cf6U);
    if(value==0U){h.clocks(2U);pf(h,0x00083de6U);pf(h,0x00083de8U);return finish(h,r);}

    h.clocks(4U);pf(h,0x00083cf8U);pf(h,0x00083cfaU);
    r.data[2]=(r.data[2]&0xffff0000U)|record_index;logicw(r,record_index);pf(h,0x00083cfcU);pf(h,0x00083cfeU);
    const auto plus100=static_cast<std::uint16_t>(record_index+0x100U);r.data[0]=(r.data[0]&0xffff0000U)|plus100;addw(r,record_index,0x100U,plus100);
    pf(h,0x00083d00U);h.clocks(2U);pf(h,0x00083d02U);const auto pending=rw(h,r.address[0]+static_cast<std::int16_t>(plus100));r.data[0]=(r.data[0]&0xffff0000U)|pending;logicw(r,pending);pf(h,0x00083d04U);
    pf(h,0x00083d06U);ww(h,r.address[6]+0x86U,pending);logicw(r,pending);pf(h,0x00083d08U);
    auto child=call(h,context,84U,0x00083d06U,0x00083de8U,0x00083d0aU);if(child.status!=TranslationStatus::complete)return child;
    h.begin(0x83d0aU);

    r.data[0]=(r.data[0]&0xffff0000U)|static_cast<std::uint16_t>(r.data[2]);logicw(r,static_cast<std::uint16_t>(r.data[0]));pf(h,0x00083d0eU);
    const auto plus200=static_cast<std::uint16_t>(static_cast<std::uint16_t>(r.data[0])+0x200U);r.data[0]=(r.data[0]&0xffff0000U)|plus200;addw(r,static_cast<std::uint16_t>(r.data[2]),0x200U,plus200);
    pf(h,0x00083d10U);pf(h,0x00083d12U);h.clocks(2U);pf(h,0x00083d14U);const auto packed=rw(h,r.address[0]+static_cast<std::int16_t>(plus200));r.data[0]=(r.data[0]&0xffff0000U)|packed;logicw(r,packed);
    pf(h,0x00083d16U);pf(h,0x00083d18U);ww(h,r.address[6]+0x30U,packed);logicw(r,packed);
    pf(h,0x00083d1aU);r.data[3]=(r.data[3]&0xffff0000U)|packed;logicw(r,packed);
    pf(h,0x00083d1cU);child=call(h,context,85U,0x00083d1aU,0x00083e0cU,0x00083d1eU);if(child.status!=TranslationStatus::complete)return child;
    // 83ede LEA 4(SP),SP / RTS discards this loader's continuation and
    // returns directly to its caller. The child already popped both frames;
    // do not build channel records or execute another RTS on this path.
    if(child.control==8U)return FunctionResult::complete(1U,child.exit_program_counter);
    h.begin(0x83d1eU);
    r.address[0]+=r.data[1];
    pf(h,0x00083d22U);h.clocks(4U);h.clocks(2U);pf(h,0x00083d80U);pf(h,0x00083d82U);
    r.program_counter=0x00083d80U;
    h.stop(); // Tail target owns the following setup clocks.
    return cpu_a_sound_reset_and_build_channel_records(context);
}
}
