#include "gain_ground/contract_types.h"

#include <cstdint>
#include <limits>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

std::uint16_t rw(ExecutionHost &h, std::uint32_t a)
{ return h.read_memory_word(kRegion, a & kAddressMask, kWordMask); }
std::uint32_t rl(ExecutionHost &h, std::uint32_t a)
{ return (static_cast<std::uint32_t>(rw(h, a)) << 16U) | rw(h, a + 2U); }
std::uint8_t rb(ExecutionHost &h, std::uint32_t a)
{
    const auto o = a & kAddressMask;
    const auto m = static_cast<std::uint16_t>((o & 1U) ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(h.read_memory_word(kRegion, o & ~1U, m)
        >> ((o & 1U) ? 0U : 8U));
}
void ww(ExecutionHost &h, std::uint32_t a, std::uint16_t v)
{ h.write_memory_word(kRegion, a & kAddressMask, v, kWordMask); }
void wl(ExecutionHost &h, std::uint32_t a, std::uint32_t v)
{ ww(h, a, static_cast<std::uint16_t>(v >> 16U)); ww(h, a + 2U, static_cast<std::uint16_t>(v)); }
void wb(ExecutionHost &h, std::uint32_t a, std::uint8_t v)
{
    const auto o = a & kAddressMask;
    const bool odd = (o & 1U) != 0U;
    h.write_memory_word(kRegion, o & ~1U,
        static_cast<std::uint16_t>(odd ? v : static_cast<unsigned>(v) << 8U),
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U));
}
void pf(ExecutionHost &h, std::uint32_t a) { (void)rw(h, a); }
void logic(CpuRegisters &r, std::uint32_t v, std::uint32_t sign)
{
    std::uint16_t f = r.status & 0x0010U;
    if (v & sign) f |= 8U;
    if (v == 0U) f |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}
void cmpb(CpuRegisters &r, std::uint8_t left, std::uint8_t right)
{
    const auto v = static_cast<std::uint8_t>(left - right);
    std::uint16_t f = r.status & 0x0010U;
    if (v & 0x80U) f |= 8U;
    if (v == 0U) f |= 4U;
    if (((left ^ right) & (left ^ v) & 0x80U) != 0U) f |= 2U;
    if (right > left) f |= 1U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}
void subb(CpuRegisters &r, std::uint8_t left, std::uint8_t right, std::uint8_t v)
{
    std::uint16_t f{};
    if (v & 0x80U) f |= 8U;
    if (v == 0U) f |= 4U;
    if (((left ^ right) & (left ^ v) & 0x80U) != 0U) f |= 2U;
    if (right > left) f |= 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}
void bit(CpuRegisters &r, std::uint8_t v, unsigned b)
{ r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((v & (1U << b)) ? 0U : 4U)); }
void push(ExecutionHost &h, CpuRegisters &r, std::uint32_t v)
{ r.address[7] -= 4U; wl(h, r.address[7], v); }
FunctionResult ret(ExecutionHost &h, CpuRegisters &r)
{
    const auto target = rl(h, r.address[7]); r.address[7] += 4U;
    pf(h, target); pf(h, target + 2U); r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
FunctionResult call(FunctionContext &c, std::uint32_t id, std::uint32_t site,
    std::uint32_t target, std::uint32_t resume)
{
    push(*c.host, c.registers, resume); pf(*c.host, target); pf(*c.host, target + 2U);
    c.registers.program_counter = target;
    return c.host->call_function(id, 0U, 0xffU, 2U, site, target, c);
}
FunctionResult outer_return(ExecutionHost &h, CpuRegisters &r)
{ pf(h, 0x834aeU); pf(h, 0x834b0U); return ret(h, r); }

FunctionResult inline_record_copy_tail(FunctionContext &c)
{
    auto &h = *c.host; auto &r = c.registers;
    push(h, r, 0x835faU); pf(h, 0x83612U); pf(h, 0x83614U);
    r.program_counter = 0x83612U;
    (void)h.call_function(std::numeric_limits<std::uint32_t>::max(), 0U, 0xffU,
        2U, 0x835f6U, 0x83612U, c);
    auto v = rw(h, r.address[0]); r.address[0] += 2U; ww(h, r.address[1], v);
    r.address[1] += 2U; logic(r, v, 0x8000U);
    pf(h, 0x83616U); auto b = rb(h, r.address[0]++); r.data[1] = (r.data[1] & 0xffffff00U) | b;
    logic(r, b, 0x80U);
    r.data[1] = (r.data[1] & 0xffffff00U) | static_cast<std::uint8_t>(r.data[7]);
    logic(r, static_cast<std::uint8_t>(r.data[1]), 0x80U); pf(h, 0x83618U); pf(h, 0x8361aU);
    b = static_cast<std::uint8_t>(r.data[1] | 0x90U); r.data[1] = (r.data[1] & 0xffffff00U) | b;
    logic(r, b, 0x80U); pf(h, 0x8361cU); pf(h, 0x8361eU); wb(h, r.address[1]++, b);
    pf(h, 0x83620U); b = rb(h, r.address[0]++); wb(h, r.address[1]++, b); logic(r, b, 0x80U);
    r.data[1] = r.address[0]; pf(h, 0x83622U); pf(h, 0x83624U);
    r.data[1] += rl(h, r.address[0]); r.address[0] += 4U; r.data[1] -= r.address[5];
    pf(h, 0x83626U); pf(h, 0x83628U); wl(h, r.address[1], r.data[1]); r.address[1] += 4U;
    logic(r, r.data[1], 0x80000000U); pf(h, 0x8362aU);
    const auto lv = rl(h, r.address[0]); r.address[0] += 4U; wl(h, r.address[1], lv);
    r.address[1] += 4U; logic(r, lv, 0x80000000U); pf(h, 0x8362cU); pf(h, 0x8362eU);
    ww(h, r.address[1], 0x5001U); r.address[1] += 2U; logic(r, 0x5001U, 0x8000U);
    pf(h, 0x83630U); pf(h, 0x83632U); b = rb(h, r.address[0] - 9U);
    r.data[1] = (r.data[1] & 0xffffff00U) | b; const auto d = static_cast<std::uint8_t>(b - 1U);
    r.data[1] = (r.data[1] & 0xffffff00U) | d; subb(r, b, 1U, d);
    pf(h, 0x83634U); pf(h, 0x83636U); wb(h, r.address[1]++, d);
    pf(h, 0x83638U); r.data[1] = 0U; logic(r, 0U, 0x80000000U);
    pf(h, 0x8363aU); wb(h, r.address[1]++, 0U); logic(r, 0U, 0x80U);
    r.data[2] = 15U; logic(r, 15U, 0x80000000U);
    for (;;) {
        pf(h, 0x8363cU); pf(h, 0x8363eU); wl(h, r.address[1], r.data[1]); r.address[1] += 4U;
        logic(r, r.data[1], 0x80000000U);
        pf(h, 0x83640U); const auto n = static_cast<std::uint16_t>(r.data[2]);
        r.data[2] = (r.data[2] & 0xffff0000U) | static_cast<std::uint16_t>(n - 1U);
        if (n == 0U) break;
    }
    pf(h, 0x8363cU); pf(h, 0x83642U); pf(h, 0x83644U);
    (void)rl(h, r.address[7]); r.address[7] += 4U; r.program_counter = 0x835faU;
    pf(h, 0x835faU); pf(h, 0x835fcU);
    return ret(h, r);
}
} // namespace

FunctionResult cpu_a_sound_channel_decode_stream(FunctionContext &c) noexcept
{
    if (!c.host) return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    auto &h = *c.host; auto &r = c.registers;
    pf(h, 0x834b4U); r.address[4] = rl(h, r.address[3] + 4U);
    r.address[4] += r.address[5]; pf(h, 0x834b6U); pf(h, 0x834b8U); pf(h, 0x834baU);
    pf(h, 0x834bcU); auto mode = rb(h, r.address[3] + 1U); bit(r, mode, 3U); pf(h, 0x834beU);

    if ((mode & 8U) == 0U) {
        pf(h, 0x834c0U);
        for (;;) {
            pf(h, 0x834c2U);
            auto d0 = rb(h, r.address[4]++); r.data[0] = (r.data[0] & 0xffffff00U) | d0;
            logic(r, d0, 0x80U);
            if (d0 == 0U) { pf(h, 0x834c4U); pf(h, 0x834b6U); pf(h, 0x834b8U); pf(h, 0x834baU); pf(h, 0x834bcU); mode = rb(h, r.address[3] + 1U); bit(r, mode, 3U); pf(h, 0x834beU); if(mode&8U)goto mode1;pf(h, 0x834c0U); continue; }
            pf(h, 0x834c4U); pf(h, 0x834c6U);
            if ((d0 & 0x80U) == 0U) {
                pf(h, 0x8353eU); pf(h, 0x83540U); pf(h, 0x83542U); wb(h, r.address[3] + 13U, d0); logic(r, d0, 0x80U);
                r.address[4] -= r.address[5]; pf(h, 0x83544U); pf(h, 0x83546U);
                pf(h, 0x83548U); wl(h, r.address[3] + 4U, r.address[4]); logic(r, r.address[4], 0x80000000U); pf(h, 0x8354aU); return ret(h, r);
            }
            pf(h, 0x834c8U); pf(h, 0x834caU); cmpb(r, d0, 0xe0U); pf(h, 0x834ccU);
            if (d0 >= 0xe0U) {
                pf(h, 0x834ceU); pf(h, 0x834d0U); auto z = call(c, 81U, 0x834ceU, 0x83644U, 0x834d2U);
                if (z.status != TranslationStatus::complete) return z;
                pf(h, 0x834d6U); const auto f = rb(h, r.address[3] + 15U); logic(r, f, 0x80U);
                if (f == 0U) { pf(h, 0x834d8U); return outer_return(h, r); }
                pf(h, 0x834d8U); pf(h, 0x834daU); pf(h, 0x834b6U);
                pf(h, 0x834b8U); pf(h, 0x834baU);
                pf(h, 0x834bcU); mode = rb(h, r.address[3] + 1U); bit(r, mode, 3U);
                pf(h, 0x834beU); if(mode&8U)goto mode1;pf(h, 0x834c0U); continue;
            }
            pf(h, 0x834ceU); pf(h, 0x834daU); pf(h, 0x834dcU); cmpb(r, d0, 0x80U); pf(h, 0x834deU);
            pf(h, 0x834e0U);
            bool scan_opcode_prefetched = false;
            if (d0 == 0x80U) {
                pf(h, 0x834e2U); pf(h, 0x834e4U); pf(h, 0x834e6U); const auto a = r.address[3] + 16U;
                auto v = rb(h, a); bit(r, v, 7U); pf(h, 0x834e8U); wb(h, a, static_cast<std::uint8_t>(v | 0x80U)); pf(h, 0x8351aU);
            } else {
                pf(h, 0x834e8U); pf(h, 0x834eaU); const auto x = static_cast<std::uint8_t>(d0 - 0x81U); subb(r, d0, 0x81U, x);
                r.data[0] = (r.data[0] & 0xffffff00U) | x; pf(h, 0x834ecU); pf(h, 0x834eeU); pf(h, 0x834f0U);
                const auto base = rb(h, r.address[3] + 8U); const auto sum = static_cast<std::uint8_t>(x + base);
                r.data[0] = (r.data[0] & 0xffffff00U) | sum; /* ADD flags */
                { const auto wide=static_cast<unsigned>(x)+base; std::uint16_t f{}; if(sum&0x80)f|=8;if(sum==0)f|=4;if(((~(x^base))&(x^sum)&0x80)!=0)f|=2;if(wide>0xff)f|=0x11;r.status=static_cast<std::uint16_t>((r.status&~0x1f)|f); }
                pf(h, 0x834f2U); pf(h, 0x834f4U); wb(h, r.address[3] + 16U, sum); logic(r, sum, 0x80U);
                pf(h, 0x834f6U); pf(h, 0x834f8U); pf(h, 0x834faU); wb(h, r.address[3] + 17U, 0U); logic(r, 0U, 0x80U);
                pf(h, 0x834fcU); pf(h, 0x834feU); pf(h,0x83500U); mode=rb(h,r.address[3]+1U);bit(r,mode,4U);
                if (mode&0x10U) { pf(h,0x83502U);pf(h,0x83504U);auto q=rb(h,r.address[4]++);pf(h,0x83506U);wb(h,r.address[3]+18U,q);logic(r,q,0x80U);pf(h,0x83508U);q=rb(h,r.address[4]++);pf(h,0x8350aU);wb(h,r.address[3]+19U,q);logic(r,q,0x80U); } else { pf(h,0x83502U);pf(h,0x8350aU); }
                pf(h,0x8350cU);pf(h,0x8350eU);pf(h,0x83510U);const auto ch=rb(h,r.address[3]);bit(r,ch,2U);
                if ((ch&4U)==0U) { pf(h,0x83512U);pf(h,0x83514U);pf(h,0x83516U);r.data[1]=(r.data[1]&0xffff0000U)|rw(h,r.address[3]+16U);logic(r,static_cast<std::uint16_t>(r.data[1]),0x8000U);pf(h,0x83518U);auto z=call(c,93U,0x83516U,0x8419cU,0x8351aU);if(z.status!=TranslationStatus::complete)return z;scan_opcode_prefetched=true; }
                else { pf(h,0x83512U); pf(h,0x8351aU); }
            }
            for (;;) {
                if(!scan_opcode_prefetched) pf(h,0x8351cU);
                scan_opcode_prefetched=false;d0=rb(h,r.address[4]++);r.data[0]=(r.data[0]&0xffffff00U)|d0;logic(r,d0,0x80U);
                if(d0==0U){pf(h,0x8351eU);pf(h,0x8351aU);continue;} pf(h,0x8351eU);pf(h,0x83520U);if((d0&0x80U)==0U){pf(h,0x8353eU);pf(h,0x83540U);pf(h,0x83542U);wb(h,r.address[3]+13U,d0);logic(r,d0,0x80U);r.address[4]-=r.address[5];pf(h,0x83544U);pf(h,0x83546U);pf(h,0x83548U);wl(h,r.address[3]+4U,r.address[4]);logic(r,r.address[4],0x80000000U);pf(h,0x8354aU);return ret(h,r);}pf(h,0x83522U);cmpb(r,d0,0xe0U);pf(h,0x83524U);
                if(d0<0xe0U){pf(h,0x83526U);pf(h,0x83534U);r.address[4]-=1U;pf(h,0x83536U);r.address[4]-=r.address[5];pf(h,0x83538U);pf(h,0x8353aU);pf(h,0x8353cU);wl(h,r.address[3]+4U,r.address[4]);logic(r,r.address[4],0x80000000U);pf(h,0x8353eU);return ret(h,r);} pf(h,0x83526U);pf(h,0x83528U);auto z=call(c,81U,0x83526U,0x83644U,0x8352aU);if(z.status!=TranslationStatus::complete)return z;pf(h,0x8352eU);const auto f=rb(h,r.address[3]+15U);logic(r,f,0x80U);pf(h,0x83530U);if(f==0U)return outer_return(h,r);pf(h,0x83532U);pf(h,0x83534U);pf(h,0x8351aU);
            }
        }
    }

mode1:
    pf(h, 0x8354aU);
    for (;;) {
        pf(h,0x8354cU);auto d0=rb(h,r.address[4]++);r.data[0]=(r.data[0]&0xffffff00U)|d0;logic(r,d0,0x80U);
        if(d0==0U){pf(h,0x8354eU);pf(h,0x8354aU);continue;} pf(h,0x8354eU);pf(h,0x83550U);
        if((d0&0x80U)==0U){pf(h,0x835d0U);pf(h,0x835d2U);pf(h,0x835d4U);wb(h,r.address[3]+13U,d0);logic(r,d0,0x80U);r.address[4]-=r.address[5];pf(h,0x835d6U);pf(h,0x835d8U);pf(h,0x835daU);wl(h,r.address[3]+4U,r.address[4]);logic(r,r.address[4],0x80000000U);pf(h,0x835dcU);pf(h,0x835deU);pf(h,0x835e0U);const auto peer=rb(h,r.address[3]+0x320U);bit(r,peer,7U);pf(h,0x835e2U);if(peer&0x80U)return outer_return(h,r);pf(h,0x835e4U);r.data[0]=0U;logic(r,0U,0x80000000U);pf(h,0x835e6U);pf(h,0x835e8U);pf(h,0x835eaU);const auto vv=rb(h,r.address[3]+22U);r.data[0]=(r.data[0]&0xffffff00U)|vv;logic(r,vv,0x80U);r.data[1]=2U;logic(r,2U,0x80000000U);pf(h,0x835ecU);pf(h,0x835eeU);auto z=call(c,89U,0x835ecU,0x8413eU,0x835f0U);if(z.status!=TranslationStatus::complete)return z;const auto tw=rw(h,r.address[0]);r.address[0]+=2U;logic(r,tw,0x8000U);pf(h,0x835f4U);r.address[1]=r.address[3]+0xa0U;pf(h,0x835f6U);pf(h,0x835f8U);return inline_record_copy_tail(c);}
        pf(h,0x83552U);pf(h,0x83554U);cmpb(r,d0,0xe0U);pf(h,0x83556U);
        if(d0>=0xe0U){pf(h,0x83558U);pf(h,0x8355aU);auto z=call(c,81U,0x83558U,0x83644U,0x8355cU);if(z.status!=TranslationStatus::complete)return z;pf(h,0x83560U);const auto f=rb(h,r.address[3]+15U);logic(r,f,0x80U);pf(h,0x83562U);if(f==0U)return outer_return(h,r);pf(h,0x83564U);pf(h,0x83566U);pf(h,0x8354aU);continue;}
        pf(h,0x83558U);pf(h,0x83566U);pf(h,0x83568U);cmpb(r,d0,0x80U);pf(h,0x8356aU);pf(h,0x8356cU);
        bool mode1_second_prefetched=false;
        if(d0==0x80U){pf(h,0x8356eU);pf(h,0x83570U);pf(h,0x83572U);pf(h,0x83574U);const auto a=r.address[3]+16U;auto v=rb(h,a);bit(r,v,7U);pf(h,0x83576U);wb(h,a,static_cast<std::uint8_t>(v|0x80U));pf(h,0x8359eU);}else{pf(h,0x83576U);pf(h,0x83578U);pf(h,0x8357aU);pf(h,0x8357cU);auto v=rb(h,r.address[3]);bit(r,v,4U);pf(h,0x8357eU);if(v&0x10U){pf(h,0x8359eU);}else{pf(h,0x83580U);pf(h,0x83582U);pf(h,0x83584U);pf(h,0x83586U);v=rb(h,r.address[3]+0x320U);bit(r,v,7U);if(v&0x80U){pf(h,0x83588U);pf(h,0x8359eU);}else{pf(h,0x83588U);pf(h,0x8358aU);const auto x=static_cast<std::uint8_t>(d0-0x82U);subb(r,d0,0x82U,x);r.data[0]=(r.data[0]&0xffffff00U)|x;pf(h,0x8358cU);pf(h,0x8358eU);const auto aw=static_cast<std::uint16_t>(r.data[0])&0x003eU;r.data[0]=(r.data[0]&0xffff0000U)|aw;logic(r,aw,0x8000U);pf(h,0x83590U);pf(h,0x83592U);pf(h,0x83594U);wb(h,r.address[3]+22U,static_cast<std::uint8_t>(aw));logic(r,static_cast<std::uint8_t>(aw),0x80U);r.data[1]=2U;logic(r,2U,0x80000000U);pf(h,0x83596U);pf(h,0x83598U);auto z=call(c,89U,0x83596U,0x8413eU,0x8359aU);if(z.status!=TranslationStatus::complete)return z;z=call(c,80U,0x8359aU,0x835fcU,0x8359eU);if(z.status!=TranslationStatus::complete)return z;mode1_second_prefetched=true;}}}
        for(;;){if(!mode1_second_prefetched)pf(h,0x835a0U);mode1_second_prefetched=false;d0=rb(h,r.address[4]++);r.data[0]=(r.data[0]&0xffffff00U)|d0;logic(r,d0,0x80U);if(d0==0U){pf(h,0x835a2U);pf(h,0x8359eU);continue;}pf(h,0x835a2U);pf(h,0x835a4U);if((d0&0x80U)==0U){pf(h,0x835c4U);pf(h,0x835c6U);pf(h,0x835c8U);wb(h,r.address[3]+13U,d0);logic(r,d0,0x80U);r.address[4]-=r.address[5];pf(h,0x835caU);pf(h,0x835ccU);pf(h,0x835ceU);wl(h,r.address[3]+4U,r.address[4]);logic(r,r.address[4],0x80000000U);pf(h,0x835d0U);return ret(h,r);}pf(h,0x835a6U);pf(h,0x835a8U);cmpb(r,d0,0xe0U);pf(h,0x835aaU);if(d0<0xe0U){pf(h,0x835acU);pf(h,0x835baU);r.address[4]-=1U;pf(h,0x835bcU);r.address[4]-=r.address[5];pf(h,0x835beU);pf(h,0x835c0U);pf(h,0x835c2U);wl(h,r.address[3]+4U,r.address[4]);logic(r,r.address[4],0x80000000U);pf(h,0x835c4U);return ret(h,r);}pf(h,0x835acU);pf(h,0x835aeU);auto z=call(c,81U,0x835acU,0x83644U,0x835b0U);if(z.status!=TranslationStatus::complete)return z;pf(h,0x835b4U);const auto f=rb(h,r.address[3]+15U);logic(r,f,0x80U);pf(h,0x835b6U);if(f==0U)return outer_return(h,r);pf(h,0x835b8U);pf(h,0x835baU);pf(h,0x8359eU);}
    }
}
} // namespace gain_ground::translated
