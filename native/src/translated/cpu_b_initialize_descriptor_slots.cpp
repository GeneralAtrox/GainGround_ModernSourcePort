#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivate=2U,kMask=0xffffU;
struct B {std::uint32_t o;std::uint16_t m;unsigned s;};
B bl(std::uint32_t a){const bool odd=a&1U;return{a&~1U,static_cast<std::uint16_t>(odd?0xffU:0xff00U),odd?0U:8U};}
std::uint8_t rb(ExecutionHost&h,std::uint32_t a){const auto b=bl(a);return static_cast<std::uint8_t>(h.read_memory_word(kPrivate,b.o,b.m)>>b.s);}
void wb(ExecutionHost&h,std::uint32_t a,std::uint8_t v){const auto b=bl(a);h.write_memory_word(kPrivate,b.o,static_cast<std::uint16_t>(v)<<b.s,b.m);}
void logic(CpuRegisters&r,std::uint32_t v,std::uint32_t s,std::uint32_t m){std::uint16_t f=r.status&0x10U;if(v&s)f|=8U;if((v&m)==0U)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
std::uint32_t rl(ExecutionHost&h,std::uint32_t a){const auto hi=h.read_memory_word(kPrivate,a,kMask),lo=h.read_memory_word(kPrivate,a+2U,kMask);return(static_cast<std::uint32_t>(hi)<<16U)|lo;}
void wl(ExecutionHost&h,std::uint32_t a,std::uint32_t v){h.write_memory_word(kPrivate,a,static_cast<std::uint16_t>(v>>16U),kMask);h.write_memory_word(kPrivate,a+2U,static_cast<std::uint16_t>(v),kMask);}
void push(ExecutionHost&h,CpuRegisters&r,std::uint32_t v){r.address[7]-=4U;wl(h,r.address[7],v);}
std::uint32_t pop(ExecutionHost&h,CpuRegisters&r){const auto v=rl(h,r.address[7]);r.address[7]+=4U;return v;}
void clr_word(ExecutionHost&h,CpuRegisters&r,std::uint32_t a){(void)h.read_memory_word(kPrivate,a,kMask);h.write_memory_word(kPrivate,a,0U,kMask);logic(r,0U,0x8000U,0xffffU);}
void clr_byte(ExecutionHost&h,CpuRegisters&r,std::uint32_t a){(void)rb(h,a);wb(h,a,0U);logic(r,0U,0x80U,0xffU);}
void asl_one(CpuRegisters&r,std::uint16_t old,std::uint16_t v){std::uint16_t f{};if(v&0x8000U)f|=8U;if(v==0U)f|=4U;if(((old^v)&0x8000U)!=0U)f|=2U;if(old&0x8000U)f|=0x11U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);}
} // namespace

FunctionResult cpu_b_initialize_descriptor_slots(FunctionContext&context) noexcept
{
    if(context.host==nullptr)return{TranslationStatus::contract_violation,0U,context.registers.program_counter};
    auto&h=*context.host;auto&r=context.registers;
    auto d0=h.read_memory_word(kPrivate,0x0c02U,kMask);r.data[0]=(r.data[0]&0xffff0000U)|d0;logic(r,d0,0x8000U,0xffffU);
    for(unsigned i=0;i<2U;++i){const auto v=static_cast<std::uint16_t>(d0<<1U);asl_one(r,d0,v);d0=v;r.data[0]=(r.data[0]&0xffff0000U)|d0;}
    r.address[0]=0x00014452U;
    r.address[0]=static_cast<std::uint32_t>(static_cast<std::int64_t>(r.address[0])+static_cast<std::int16_t>(d0));
    r.address[0]=rl(h,r.address[0]);
    auto d7=h.read_memory_word(kPrivate,r.address[0],kMask);r.address[0]+=2U;r.data[7]=(r.data[7]&0xffff0000U)|d7;logic(r,d7,0x8000U,0xffffU);
    while((d7&0x8000U)==0U){
        wb(h,r.address[6],0x80U);logic(r,0x80U,0x80U,0xffU);
        clr_byte(h,r,r.address[6]+1U);
        wl(h,r.address[6]+2U,0x00015df2U);logic(r,0x00015df2U,0x80000000U,0xffffffffU);
        clr_word(h,r,r.address[6]+0x1eU);clr_word(h,r,r.address[6]+0x20U);clr_word(h,r,r.address[6]+0x22U);
        clr_byte(h,r,r.address[6]+0x3fU);clr_word(h,r,r.address[6]+0x42U);clr_word(h,r,r.address[6]+0x44U);
        h.write_memory_word(kPrivate,r.address[6]+0x46U,0x8000U,kMask);logic(r,0x8000U,0x8000U,0xffffU);
        r.address[1]=rl(h,r.address[0]);r.address[0]+=4U;
        if(r.address[1]!=0x00013eb2U)return{TranslationStatus::contract_violation,0U,r.address[1]};
        push(h,r,0x00013e82U);r.program_counter=r.address[1];
        const auto child=h.call_function(278U,1U,0x72U,2U,0x00013e80U,r.address[1],context);
        if(child.status!=TranslationStatus::complete||child.control!=1U)return child;
        r.address[6]=static_cast<std::uint32_t>(static_cast<std::int64_t>(r.address[6])+0x80);
        d7=static_cast<std::uint16_t>(d7-1U);r.data[7]=(r.data[7]&0xffff0000U)|d7;
        if(d7==0xffffU)break;
    }
    const auto target=pop(h,r);r.program_counter=target;return FunctionResult::complete(1U,target);
}

} // namespace gain_ground::translated
