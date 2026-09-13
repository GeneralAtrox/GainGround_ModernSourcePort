#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t R=2U,W=0xffffU; constexpr std::uint32_t M=0x0003ffffU;
std::uint16_t rw(ExecutionHost&h,std::uint32_t a){return h.read_memory_word(R,a&M,W);} void ww(ExecutionHost&h,std::uint32_t a,std::uint16_t v){h.write_memory_word(R,a&M,v,W);}
std::uint32_t rl(ExecutionHost&h,std::uint32_t a){return(static_cast<std::uint32_t>(rw(h,a))<<16U)|rw(h,a+2U);}
std::uint8_t rb(ExecutionHost&h,std::uint32_t a){auto v=h.read_memory_word(R,(a&M)&~1U,(a&1U)?0x00ffU:0xff00U);return static_cast<std::uint8_t>((a&1U)?v:v>>8U);} void wb(ExecutionHost&h,std::uint32_t a,std::uint8_t v){bool o=(a&1U)!=0U;h.write_memory_word(R,(a&M)&~1U,static_cast<std::uint16_t>(v)<<(o?0U:8U),o?0x00ffU:0xff00U);}
void mf(CpuRegisters&r,std::uint16_t v){std::uint16_t f=r.status&0x10U;if(v&0x8000U)f|=8U;if(v==0U)f|=4U;r.status=static_cast<std::uint16_t>((r.status&~0x1fU)|f);} std::uint32_t pop(ExecutionHost&h,CpuRegisters&r){auto v=rl(h,r.address[7]);r.address[7]+=4U;return v;}
}
FunctionResult cpu_b_initialize_record_with_descriptor_aux_word(FunctionContext &c) noexcept
{
    if (c.host == nullptr || c.registers.program_counter != 0x00011034U)
        return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    auto &h = *c.host;
    auto &r = c.registers;
    const auto a5 = r.address[5];
    const auto a6 = r.address[6];
    r.address[0] = rl(h, r.address[3] + 0x10U);
    auto high = rw(h, r.address[0]);
    auto low = rw(h, r.address[0] + 2U);
    ww(h, a6 + 2U, high); ww(h, a6 + 4U, low);
    r.address[0] += 4U;
    const auto sum = static_cast<std::uint16_t>(
        rw(h, a5 + 0x66U) + rw(h, r.address[0]));
    r.address[0] += 2U;
    r.data[0] = (r.data[0] & 0xffff0000U) | sum;
    ww(h, a6 + 8U, sum);
    wb(h, a6 + 0x3cU, rb(h, r.address[0]++));
    wb(h, a6 + 0x0bU, rb(h, r.address[0]++));
    ww(h, a6 + 0x46U, rw(h, r.address[0])); r.address[0] += 2U;
    ww(h, a6 + 0x3aU, rw(h, r.address[0])); r.address[0] += 2U;
    ww(h, a6 + 0x36U, static_cast<std::uint16_t>(a5));
    (void)rb(h, a6 + 0x3fU); wb(h, a6 + 0x3fU, 0U);
    (void)rb(h, a6 + 0x3dU); wb(h, a6 + 0x3dU, 0U);
    ww(h, a6 + 6U, rw(h, r.address[0])); r.address[0] += 2U;
    auto value = rw(h, r.address[0]); r.address[0] += 2U; ww(h, a6, value);
    value = rw(h, r.address[0]); r.address[0] += 2U; ww(h, a6 + 0x10U, value);
    mf(r, value);
    const auto target = pop(h, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
}
