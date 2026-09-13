#include "gain_ground/contract_types.h"
#include "gain_ground/sound_timing.h"
#include "gain_ground/sound_caller_timing.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

std::uint16_t rw(SoundCallerTiming &h, std::uint32_t a)
{ return h.read_memory_word(kRegion, a & kAddressMask, kWordMask); }
std::uint32_t rl(SoundCallerTiming &h, std::uint32_t a)
{ return (static_cast<std::uint32_t>(rw(h, a)) << 16U) | rw(h, a + 2U); }
std::uint8_t rb(SoundCallerTiming &h, std::uint32_t a)
{
    const auto o = a & kAddressMask;
    const bool odd = (o & 1U) != 0U;
    const auto m = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto v = h.read_memory_word(kRegion, o & ~1U, m);
    return static_cast<std::uint8_t>(odd ? v : v >> 8U);
}
void ww(SoundCallerTiming &h, std::uint32_t a, std::uint16_t v)
{ h.write_memory_word(kRegion, a & kAddressMask, v, kWordMask); }
void wl(SoundCallerTiming &h, std::uint32_t a, std::uint32_t v)
{ ww(h, a, static_cast<std::uint16_t>(v >> 16U)); ww(h, a + 2U, static_cast<std::uint16_t>(v)); }
void wb(SoundCallerTiming &h, std::uint32_t a, std::uint8_t v)
{
    const auto o = a & kAddressMask; const bool odd = (o & 1U) != 0U;
    h.write_memory_word(kRegion, o & ~1U,
        static_cast<std::uint16_t>(odd ? v : static_cast<unsigned>(v) << 8U),
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U));
}
void pf(SoundCallerTiming &h, std::uint32_t a) { (void)rw(h, a); }
void logic(CpuRegisters &r, std::uint32_t v, std::uint32_t sign)
{
    std::uint16_t f = r.status & 0x10U;
    if (v & sign) f |= 8U;
    if (v == 0U) f |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void bit(CpuRegisters &r, std::uint8_t v, unsigned b)
{ r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((v & (1U << b)) ? 0U : 4U)); }
void cmpb(CpuRegisters &r, std::uint8_t left, std::uint8_t right)
{
    const auto v = static_cast<std::uint8_t>(left - right); std::uint16_t f = r.status & 0x10U;
    if (v & 0x80U) f |= 8U;
    if (v == 0U) f |= 4U;
    if (((left ^ right) & (left ^ v) & 0x80U) != 0U) f |= 2U;
    if (right > left) f |= 1U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void addb(CpuRegisters &r, std::uint8_t left, std::uint8_t right, std::uint8_t v)
{
    const auto wide = static_cast<unsigned>(left) + right; std::uint16_t f{};
    if (v & 0x80U) f |= 8U;
    if (v == 0U) f |= 4U;
    if (((~(left ^ right)) & (left ^ v) & 0x80U) != 0U) f |= 2U;
    if (wide > 0xffU) f |= 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void addw(CpuRegisters &r, std::uint16_t left, std::uint16_t right, std::uint16_t v)
{
    const auto wide = static_cast<std::uint32_t>(left) + right; std::uint16_t f{};
    if (v & 0x8000U) f |= 8U;
    if (v == 0U) f |= 4U;
    if (((~(left ^ right)) & (left ^ v) & 0x8000U) != 0U) f |= 2U;
    if (wide > 0xffffU) f |= 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void subb(CpuRegisters &r, std::uint8_t left, std::uint8_t right, std::uint8_t v)
{
    std::uint16_t f{};
    if (v & 0x80U) f |= 8U;
    if (v == 0U) f |= 4U;
    if (((left ^ right) & (left ^ v) & 0x80U) != 0U) f |= 2U;
    if (right > left) f |= 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void negb(CpuRegisters &r, std::uint8_t source, std::uint8_t value)
{
    std::uint16_t f{};
    if (value & 0x80U) f |= 8U;
    if (value == 0U) f |= 4U;
    if (source == 0x80U) f |= 2U;
    if (source != 0U) f |= 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void push(SoundCallerTiming &h, CpuRegisters &r, std::uint32_t v)
{ r.address[7] -= 4U; wl(h, r.address[7], v); }
FunctionResult ret(SoundCallerTiming &h, CpuRegisters &r)
{
    const auto target = rl(h, r.address[7]); r.address[7] += 4U;
    pf(h, target); pf(h, target + 2U); r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
FunctionResult shared_ret(SoundCallerTiming &h, CpuRegisters &r)
{ h.clocks(2U); pf(h, 0x834aeU); pf(h, 0x834b0U); return ret(h, r); }
FunctionResult call(SoundCallerTiming &h, FunctionContext &c, std::uint32_t id, std::uint32_t site,
    std::uint32_t target, std::uint32_t resume)
{
    h.clocks(2U); // BSR.w internal prefix.
    push(h, c.registers, resume); pf(h, target); pf(h, target + 2U);
    c.registers.program_counter = target;
    const auto result = h.call_function(id, 0U, 0xffU, 2U, site, target, c);
    if (result.status == TranslationStatus::complete) h.begin(resume);
    return result;
}
FunctionResult branch(SoundCallerTiming &h, FunctionContext &c, std::uint32_t id, std::uint32_t site,
    std::uint32_t target)
{
    h.clocks(2U); // BRA.w internal prefix.
    pf(h, target); pf(h, target + 2U); c.registers.program_counter = target;
    return h.call_function(id, 0U, 0xffU, 1U, site, target, c);
}
} // namespace

FunctionResult cpu_a_sound_channel_stream_tick(FunctionContext &c) noexcept
{
    if (!c.host) return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    SoundCallerTiming h(c); auto &r = c.registers; const auto ch = r.address[3];
    h.begin(0x83268U);
    bool full_update = false;
    bool entry_83302_prefetched = false;
    bool entry_8330c_prefetched = false;
    std::uint8_t flags{};
    std::uint8_t selector9{};
    std::uint8_t token{};

    pf(h,0x8326cU); auto d0=rb(h,ch+0x0fU); r.data[0]=(r.data[0]&0xffffff00U)|d0; logic(r,d0,0x80U);
    pf(h,0x8326eU); d0=static_cast<std::uint8_t>(d0+1U); r.data[0]=(r.data[0]&0xffffff00U)|d0; addb(r,static_cast<std::uint8_t>(d0-1U),1U,d0);
    pf(h,0x83270U);pf(h,0x83272U);wb(h,ch+0x0fU,d0);logic(r,d0,0x80U);
    pf(h,0x83274U);pf(h,0x83276U);auto m=rb(h,ch+0x0dU);cmpb(r,d0,m);pf(h,0x83278U);
    if(d0==m){
        h.clocks(4U); // BNE.w not taken.
        pf(h,0x8327aU);pf(h,0x8327cU);pf(h,0x8327eU);d0=rb(h,ch+0x0eU);r.data[0]=(r.data[0]&0xffffff00U)|d0;logic(r,d0,0x80U);
        pf(h,0x83280U);d0=static_cast<std::uint8_t>(d0+1U);r.data[0]=(r.data[0]&0xffffff00U)|d0;addb(r,static_cast<std::uint8_t>(d0-1U),1U,d0);
        pf(h,0x83282U);pf(h,0x83284U);wb(h,ch+0x0eU,d0);logic(r,d0,0x80U);
        pf(h,0x83286U);pf(h,0x83288U);m=rb(h,ch+3U);cmpb(r,d0,m);pf(h,0x8328aU);
        if(d0==m){
            h.clocks(4U); // BNE.w not taken.
            pf(h,0x8328cU);pf(h,0x8328eU);auto z=call(h,c,79U,0x8328cU,0x834b0U,0x83290U);if(z.status!=TranslationStatus::complete)return z;
            pf(h,0x83294U);m=rb(h,ch+0x0fU);logic(r,m,0x80U);pf(h,0x83296U);if(m==0U)return shared_ret(h,r);
            h.clocks(4U); // BEQ.w not taken.
            pf(h,0x83298U);r.data[0]=0U;logic(r,0U,0x80000000U);
            pf(h,0x8329aU);pf(h,0x8329cU);pf(h,0x8329eU);ww(h,ch+0x14U,0U);logic(r,0U,0x8000U);
            pf(h,0x832a0U);pf(h,0x832a2U);ww(h,ch+0x0eU,0U);logic(r,0U,0x8000U);
            pf(h,0x832a4U);pf(h,0x832a6U);pf(h,0x832a8U);m=rb(h,ch+1U);bit(r,m,5U);pf(h,0x832aaU);
            if(m&0x20U){
                h.clocks(4U); // BEQ.b not taken.
                pf(h,0x832acU);pf(h,0x832aeU);auto q=rb(h,ch+0x33U);pf(h,0x832b0U);wb(h,ch+0x31U,q);logic(r,q,0x80U);pf(h,0x832b2U);
                pf(h,0x832b4U);auto d2=rb(h,ch+0x32U);r.data[2]=(r.data[2]&0xffffff00U)|d2;logic(r,d2,0x80U);pf(h,0x832b6U);
                if(d2&0x80U){h.clocks(4U);pf(h,0x832b8U);auto n=static_cast<std::uint8_t>(0U-d2);r.data[2]=(r.data[2]&0xffffff00U)|n;negb(r,d2,n);pf(h,0x832baU);pf(h,0x832bcU);wb(h,ch+0x32U,n);logic(r,n,0x80U);}else{h.clocks(2U);pf(h,0x832bcU);}
                pf(h,0x832beU);pf(h,0x832c0U);ww(h,ch+0x34U,0U);logic(r,0U,0x8000U);
            }else {h.clocks(2U);pf(h,0x832c0U);}
            full_update = true;
        }else{
            h.clocks(2U); // BNE.w taken.
            pf(h,0x832fcU);pf(h,0x832feU);pf(h,0x83300U);pf(h,0x83302U);wb(h,ch+0x0fU,0U);logic(r,0U,0x80U);pf(h,0x83304U);entry_83302_prefetched=true;
        }
    }else{h.clocks(2U);pf(h,0x83302U);pf(h,0x83304U);entry_83302_prefetched=true;}

    if(full_update){
        bool sound_tail_prefetched = false;
        pf(h,0x832c2U);pf(h,0x832c4U);pf(h,0x832c6U);flags=rb(h,ch);bit(r,flags,2U);pf(h,0x832c8U);if(flags&4U)return shared_ret(h,r);
        h.clocks(4U); // BNE.w not taken.
        pf(h,0x832caU);pf(h,0x832ccU);pf(h,0x832ceU);pf(h,0x832d0U);flags=rb(h,ch);bit(r,flags,4U);
        if((flags&0x10U)==0U){
            pf(h,0x832d2U);h.clocks(4U);r.data[1]=(r.data[1]&0xffffff00U)|static_cast<std::uint8_t>(r.data[7]);logic(r,static_cast<std::uint8_t>(r.data[1]),0x80U);
            pf(h,0x832d4U);r.data[0]=8U;logic(r,8U,0x80000000U);pf(h,0x832d6U);pf(h,0x832d8U);
            h.stop(); // The generated YM slice owns its accesses and clocks.
            TimingCpuPosition fallback;
            auto *timing_position = c.host->instruction_timing_position(c.cpu);
            if (!timing_position) timing_position = &fallback;
            for (;;) {
                if (!sound_timing::poll(c, *timing_position, 0x832d6U))
                    return {TranslationStatus::contract_violation,0U,r.program_counter};
                if (r.status & 4U) break;
                if (!c.host->resumes_interrupts_inline()) return {TranslationStatus::contract_violation,0U,0x832d6U};
            }
            if (!sound_timing::write_pair(c, *timing_position, 0x832e0U))
                return {TranslationStatus::contract_violation,0U,r.program_counter};
            h.begin(0x832ecU);
            sound_tail_prefetched = true;
        }else{pf(h,0x832d2U);h.clocks(2U);pf(h,0x832ecU);}
        if (!sound_tail_prefetched) pf(h,0x832eeU);
        pf(h,0x832f0U);pf(h,0x832f2U);flags=rb(h,ch+0x10U);bit(r,flags,7U);pf(h,0x832f4U);if(flags&0x80U)return shared_ret(h,r);
        h.clocks(4U); // BNE.w not taken.
        pf(h,0x832f6U);pf(h,0x832f8U);auto z=call(h,c,91U,0x832f6U,0x8415aU,0x832faU);if(z.status!=TranslationStatus::complete)return z;h.clocks(2U);pf(h,0x8330cU);pf(h,0x8330eU);entry_8330c_prefetched=true;
    }

    if(!full_update){if(!entry_83302_prefetched){pf(h,0x83302U);pf(h,0x83304U);}pf(h,0x83306U);pf(h,0x83308U);flags=rb(h,ch+0x10U);bit(r,flags,7U);pf(h,0x8330aU);if(flags&0x80U)return shared_ret(h,r);h.clocks(4U);}
    if(!entry_8330c_prefetched){pf(h,0x8330cU);pf(h,0x8330eU);}pf(h,0x83310U);pf(h,0x83312U);auto flags1=rb(h,ch+1U);bit(r,flags1,4U);
    const bool channel_update_called=(flags1&0x10U)!=0U;
    if(channel_update_called){pf(h,0x83314U);h.clocks(4U);pf(h,0x83316U);auto z=call(h,c,78U,0x83314U,0x8348cU,0x83318U);if(z.status!=TranslationStatus::complete)return z;}else{pf(h,0x83314U);h.clocks(2U);pf(h,0x83318U);}
    if(!channel_update_called){pf(h,0x8331aU);}pf(h,0x8331cU);pf(h,0x8331eU);flags1=rb(h,ch+1U);bit(r,flags1,5U);pf(h,0x83320U);
    if(flags1&0x20U){h.clocks(2U);goto modulation;}
    h.clocks(4U); // BNE.w not taken.
    pf(h,0x83322U);pf(h,0x83324U);pf(h,0x83326U);selector9=rb(h,ch+9U);logic(r,selector9,0x80U);pf(h,0x83328U);
    if(selector9!=0U){h.clocks(4U);pf(h,0x8332aU);pf(h,0x8332cU);auto z=call(h,c,496U,0x8332aU,0x833beU,0x8332eU);if(z.status!=TranslationStatus::complete)return z;}
    else {h.clocks(2U);pf(h,0x8332eU);pf(h,0x83330U);}

stream_selector:
    pf(h,0x83332U);d0=rb(h,ch+0x0aU);r.data[0]=(r.data[0]&0xffffff00U)|d0;logic(r,d0,0x80U);pf(h,0x83334U);
    if(d0==0U){h.clocks(4U);pf(h,0x83336U);pf(h,0x83338U);return ret(h,r);} h.clocks(2U);pf(h,0x83338U);pf(h,0x8333aU);
    {auto w=static_cast<std::uint16_t>(r.data[0])&0x00ffU;r.data[0]=(r.data[0]&0xffff0000U)|w;logic(r,w,0x8000U);pf(h,0x8333cU);
     auto b=static_cast<std::uint8_t>(w);auto n=static_cast<std::uint8_t>(b-1U);r.data[0]=(r.data[0]&0xffffff00U)|n;subb(r,b,1U,n);pf(h,0x8333eU);
     w=static_cast<std::uint16_t>(r.data[0]);auto sum=static_cast<std::uint16_t>(w+w);r.data[0]=(r.data[0]&0xffff0000U)|sum;addw(r,w,w,sum);}
    pf(h,0x83340U);r.data[1]=10U;logic(r,10U,0x80000000U);pf(h,0x83342U);pf(h,0x83344U);
    {auto z=call(h,c,89U,0x83342U,0x8413eU,0x83346U);if(z.status!=TranslationStatus::complete)return z;}
    r.address[1]=r.address[0];
stream_loop:
    r.data[0]=0U;logic(r,0U,0x80000000U);pf(h,0x8334aU);pf(h,0x8334cU);pf(h,0x8334eU);d0=rb(h,ch+0x15U);r.data[0]=(r.data[0]&0xffffff00U)|d0;logic(r,d0,0x80U);
    pf(h,0x83350U);pf(h,0x83352U);{auto old=rb(h,ch+0x15U);auto v=static_cast<std::uint8_t>(old+1U);pf(h,0x83354U);wb(h,ch+0x15U,v);addb(r,old,1U,v);}
    r.data[1]=0U;logic(r,0U,0x80000000U);pf(h,0x83356U);
    h.clocks(2U); // Indexed MOVE.B effective-address calculation.
    pf(h,0x83358U);{auto b=rb(h,r.address[1]+static_cast<std::int16_t>(static_cast<std::uint16_t>(r.data[0])));r.data[1]=(r.data[1]&0xffffff00U)|b;logic(r,b,0x80U);}
    pf(h,0x8335aU);cmpb(r,static_cast<std::uint8_t>(r.data[1]),0x80U);pf(h,0x8335cU);pf(h,0x8335eU);
    token=static_cast<std::uint8_t>(r.data[1]);
    if(token<0x80U){h.clocks(2U);goto ordinary_token;}
    h.clocks(4U); // BCS.w not taken.
    pf(h,0x83360U);pf(h,0x83362U);if(token==0x80U){h.clocks(2U);pf(h,0x8337eU);pf(h,0x83380U);pf(h,0x83382U);wb(h,ch+0x0aU,0U);logic(r,0U,0x80U);pf(h,0x83384U);pf(h,0x83386U);pf(h,0x83388U);pf(h,0x8338aU);wb(h,ch+0x15U,0U);logic(r,0U,0x80U);pf(h,0x8338cU);return ret(h,r);}
    h.clocks(4U); // BEQ.w not taken.
    pf(h,0x83364U);pf(h,0x83366U);cmpb(r,token,0x83U);pf(h,0x83368U);pf(h,0x8336aU);if(token>0x83U){h.clocks(2U);goto ordinary_token;}
    h.clocks(4U); // BHI.w not taken.
    pf(h,0x8336cU);pf(h,0x8336eU);if(token==0x83U){h.clocks(2U);pf(h,0x83384U);pf(h,0x83386U);pf(h,0x83388U);pf(h,0x8338aU);wb(h,ch+0x15U,0U);logic(r,0U,0x80U);pf(h,0x8338cU);return ret(h,r);}
    h.clocks(4U); // BEQ.w not taken.
    pf(h,0x83370U);pf(h,0x83372U);cmpb(r,token,0x81U);pf(h,0x83374U);pf(h,0x83376U);
    if(token!=0x81U){
        h.clocks(4U); // BEQ.w not taken.
        pf(h,0x83378U);pf(h,0x8337aU);
        // SUBQ.B displacement: extension prefetch, read, final prefetch, write.
        pf(h,0x8337cU);auto old=rb(h,ch+0x15U);
        auto v=static_cast<std::uint8_t>(old-1U);
        pf(h,0x8337eU);wb(h,ch+0x15U,v);subb(r,old,1U,v);
        return ret(h,r);
    }
    h.clocks(2U); // BEQ.w taken to the relative stream redirect.
    pf(h,0x8338cU);pf(h,0x8338eU);d0=rb(h,r.address[1]++);r.data[0]=(r.data[0]&0xffffff00U)|d0;logic(r,d0,0x80U);
    pf(h,0x83390U);{auto w=static_cast<std::uint16_t>(r.data[0]);auto v=static_cast<std::uint16_t>(w<<8U);r.data[0]=(r.data[0]&0xffff0000U)|v;logic(r,v,0x8000U);}
    pf(h,0x83392U);h.clocks(18U); // LSL.W #8: 8 shifts plus final internal step.
    d0=rb(h,r.address[1]++);r.data[0]=(r.data[0]&0xffffff00U)|d0;logic(r,d0,0x80U);pf(h,0x83394U);
    r.address[1]=static_cast<std::uint32_t>(r.address[1]+static_cast<std::int16_t>(static_cast<std::uint16_t>(r.data[0])));
    pf(h,0x83396U);h.clocks(4U); // ADDA.W register ALU tail.
    pf(h,0x83398U);pf(h,0x8339aU);wb(h,ch+0x15U,0U);logic(r,0U,0x80U);pf(h,0x8339cU);h.clocks(2U);
    pf(h,0x83348U); // BRA target; target+2 is fetched at stream_loop.
    goto stream_loop;

ordinary_token:
    pf(h,0x8339cU);pf(h,0x8339eU);pf(h,0x833a0U);pf(h,0x833a2U);flags=rb(h,ch);bit(r,flags,2U);pf(h,0x833a4U);if(flags&4U)return shared_ret(h,r);
    h.clocks(4U); // BNE.w not taken.
    pf(h,0x833a6U);logic(r,token,0x80U);pf(h,0x833a8U);pf(h,0x833aaU);
    if(token&0x80U){h.clocks(4U);pf(h,0x833acU);pf(h,0x833aeU);auto w=static_cast<std::uint16_t>(r.data[1])|0xffc0U;r.data[1]=(r.data[1]&0xffff0000U)|w;logic(r,w,0x8000U);pf(h,0x833b0U);pf(h,0x833b2U);h.clocks(2U);pf(h,0x833b6U);}else{h.clocks(2U);pf(h,0x833b2U);pf(h,0x833b4U);auto w=static_cast<std::uint16_t>(r.data[1])&0x003fU;r.data[1]=(r.data[1]&0xffff0000U)|w;logic(r,w,0x8000U);pf(h,0x833b6U);}
    {auto w=static_cast<std::uint16_t>(r.data[1]);auto v=static_cast<std::uint16_t>(w+w);r.data[1]=(r.data[1]&0xffff0000U)|v;addw(r,w,w,v);}pf(h,0x833b8U);r.data[2]=(r.data[2]&0xffffff00U)|static_cast<std::uint8_t>(r.data[1]);logic(r,static_cast<std::uint8_t>(r.data[2]),0x80U);pf(h,0x833baU);pf(h,0x833bcU);return branch(h,c,94U,0x833baU,0x8421aU);

modulation:
    pf(h,0x8344cU);r.data[1]=0U;logic(r,0U,0x80000000U);pf(h,0x8344eU);pf(h,0x83450U);pf(h,0x83452U);d0=rb(h,ch+0x31U);r.data[0]=(r.data[0]&0xffffff00U)|d0;logic(r,d0,0x80U);
    d0=static_cast<std::uint8_t>(d0+1U);r.data[0]=(r.data[0]&0xffffff00U)|d0;addb(r,static_cast<std::uint8_t>(d0-1U),1U,d0);pf(h,0x83454U);pf(h,0x83456U);pf(h,0x83458U);m=rb(h,ch+0x30U);cmpb(r,d0,m);pf(h,0x8345aU);
    if(d0==m){h.clocks(4U);pf(h,0x8345cU);pf(h,0x8345eU);pf(h,0x83460U);auto old=rb(h,ch+0x32U);auto v=static_cast<std::uint8_t>(0U-old);pf(h,0x83462U);wb(h,ch+0x32U,v);negb(r,old,v);d0=0U;r.data[0]=0U;logic(r,0U,0x80000000U);}else{h.clocks(2U);pf(h,0x83462U);}
    pf(h,0x83464U);pf(h,0x83466U);wb(h,ch+0x31U,d0);logic(r,d0,0x80U);pf(h,0x83468U);pf(h,0x8346aU);auto d1=rb(h,ch+0x32U);r.data[1]=(r.data[1]&0xffffff00U)|d1;logic(r,d1,0x80U);pf(h,0x8346cU);
    if(d1&0x80U){h.clocks(4U);pf(h,0x8346eU);pf(h,0x83470U);auto w=static_cast<std::uint16_t>(r.data[1])|0xff80U;r.data[1]=(r.data[1]&0xffff0000U)|w;logic(r,w,0x8000U);}else h.clocks(2U);pf(h,0x83472U);pf(h,0x83474U);
    pf(h,0x83476U);auto phase=rw(h,ch+0x34U);r.data[0]=(r.data[0]&0xffff0000U)|phase;logic(r,phase,0x8000U);{auto w=static_cast<std::uint16_t>(r.data[1]);auto v=static_cast<std::uint16_t>(w+phase);r.data[1]=(r.data[1]&0xffff0000U)|v;addw(r,w,phase,v);}pf(h,0x83478U);pf(h,0x8347aU);pf(h,0x8347cU);ww(h,ch+0x34U,static_cast<std::uint16_t>(r.data[1]));logic(r,static_cast<std::uint16_t>(r.data[1]),0x8000U);
    pf(h,0x8347eU);pf(h,0x83480U);{auto w=static_cast<std::uint16_t>(r.data[1]),x=rw(h,ch+0x10U),v=static_cast<std::uint16_t>(w+x);r.data[1]=(r.data[1]&0xffff0000U)|v;addw(r,w,x,v);}pf(h,0x83482U);{auto w=static_cast<std::uint16_t>(r.data[1])&0x7fffU;r.data[1]=(r.data[1]&0xffff0000U)|w;logic(r,w,0x8000U);}pf(h,0x83484U);pf(h,0x83486U);{auto z=call(h,c,93U,0x83484U,0x8419cU,0x83488U);if(z.status!=TranslationStatus::complete)return z;}h.clocks(2U);pf(h,0x8332eU);pf(h,0x83330U);goto stream_selector;
}
} // namespace gain_ground::translated
