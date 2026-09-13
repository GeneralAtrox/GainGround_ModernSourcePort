#include "gain_ground/contract_types.h"
#include "gain_ground/sound_caller_timing.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 3U;
constexpr std::uint32_t kMask = 0x0003ffffU;

std::uint16_t rw(SoundCallerTiming &h, std::uint32_t address)
{ return h.read_memory_word(kRegion, address & kMask, 0xffffU); }
std::uint32_t rl(SoundCallerTiming &h, std::uint32_t address)
{ return (static_cast<std::uint32_t>(rw(h, address)) << 16U) | rw(h, address + 2U); }
std::uint8_t rb(SoundCallerTiming &h, std::uint32_t address)
{
    const auto offset = address & kMask;
    const bool odd = (offset & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto value = h.read_memory_word(kRegion, offset & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? value : value >> 8U);
}
void ww(SoundCallerTiming &h, std::uint32_t address, std::uint16_t value)
{ h.write_memory_word(kRegion, address & kMask, value, 0xffffU); }
void wl(SoundCallerTiming &h, std::uint32_t address, std::uint32_t value)
{ ww(h, address, static_cast<std::uint16_t>(value >> 16U)); ww(h, address + 2U, static_cast<std::uint16_t>(value)); }
void wb(SoundCallerTiming &h, std::uint32_t address, std::uint8_t value)
{
    const auto offset = address & kMask;
    const bool odd = (offset & 1U) != 0U;
    h.write_memory_word(kRegion, offset & ~1U,
        static_cast<std::uint16_t>(odd ? value : static_cast<unsigned>(value) << 8U),
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U));
}
void pf(SoundCallerTiming &h, std::uint32_t address) { (void)rw(h, address); }
void logic(CpuRegisters &r, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = r.status & 0x10U;
    if ((value & sign) != 0U) flags |= 8U;
    if (value == 0U) flags |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}
void subw(CpuRegisters &r, std::uint16_t left, std::uint16_t right, std::uint16_t value)
{
    std::uint16_t flags{};
    if ((value & 0x8000U) != 0U) flags |= 8U;
    if (value == 0U) flags |= 4U;
    if (((left ^ right) & (left ^ value) & 0x8000U) != 0U) flags |= 2U;
    if (right > left) flags |= 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}
void subb(CpuRegisters &r, std::uint8_t left, std::uint8_t right, std::uint8_t value)
{
    std::uint16_t flags{};
    if ((value & 0x80U) != 0U) flags |= 8U;
    if (value == 0U) flags |= 4U;
    if (((left ^ right) & (left ^ value) & 0x80U) != 0U) flags |= 2U;
    if (right > left) flags |= 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}
void push(SoundCallerTiming &h, CpuRegisters &r, std::uint32_t value)
{ r.address[7] -= 4U; wl(h, r.address[7], value); }
FunctionResult call(SoundCallerTiming &h, FunctionContext &c, std::uint32_t id, std::uint32_t site,
    std::uint32_t target, std::uint32_t resume)
{
    h.clocks(2U); push(h, c.registers, resume); pf(h, target); pf(h, target + 2U);
    c.registers.program_counter = target;
    return h.call_function(id, 0U, 0xffU, 2U, site, target, c);
}
FunctionResult ret(SoundCallerTiming &h, CpuRegisters &r)
{
    const auto target = rl(h, r.address[7]); r.address[7] += 4U;
    pf(h, target); pf(h, target + 2U); r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_a_sound_driver_tick(FunctionContext &c) noexcept
{
    if (!c.host) return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    SoundCallerTiming h(c);
    auto &r = c.registers;
    const auto a6 = r.address[6];

    h.begin(0x83208U);
    r.data[0] = (r.data[0] & 0xffff0000U) | 0x0014U; logic(r, 0x14U, 0x8000U);
    pf(h, 0x8320cU); pf(h, 0x8320eU);
    r.data[1] = (r.data[1] & 0xffff0000U) | 0x0015U; logic(r, 0x15U, 0x8000U);
    pf(h, 0x83210U); pf(h, 0x83212U);
    { const auto result = call(h, c, 88U, 0x83210U, 0x84126U, 0x83214U); if (result.status != TranslationStatus::complete) return result; }

    h.begin(0x83214U); // Resume timing after the YM writer returns.
    pf(h, 0x83218U); const auto timer8a = rw(h, a6 + 0x8aU); logic(r, timer8a, 0x8000U); pf(h, 0x8321aU);
    if (timer8a != 0U) {
        h.clocks(4U); // BEQ.b not taken.
        pf(h, 0x8321cU); pf(h, 0x8321eU); pf(h, 0x83220U);
        const auto old = rw(h, a6 + 0x8aU); const auto value = static_cast<std::uint16_t>(old - 1U);
        pf(h, 0x83222U); ww(h, a6 + 0x8aU, value); subw(r, old, 1U, value);
    } else { h.clocks(2U); pf(h, 0x83220U); pf(h, 0x83222U); }

    pf(h, 0x83224U); const auto timer86 = rw(h, a6 + 0x86U); logic(r, timer86, 0x8000U); pf(h, 0x83226U);
    if (timer86 != 0U) {
        h.clocks(4U); // BEQ.b not taken.
        pf(h, 0x83228U); pf(h, 0x8322aU); pf(h, 0x8322cU);
        const auto old = rw(h, a6 + 0x86U); const auto value = static_cast<std::uint16_t>(old - 1U);
        pf(h, 0x8322eU); ww(h, a6 + 0x86U, value); subw(r, old, 1U, value);
        h.clocks(2U); // BRA.b, before its target prefetches.
        pf(h, 0x83238U); pf(h, 0x8323aU);
    } else {
        h.clocks(2U); // BEQ.b taken.
        pf(h, 0x8322eU); pf(h, 0x83230U); pf(h, 0x83232U); const auto pending = rw(h, a6 + 0x80U); logic(r, pending, 0x8000U); pf(h, 0x83234U);
        if (pending != 0U) {
            h.clocks(4U); // BEQ.b not taken.
            pf(h, 0x83236U);
            const auto result = call(h, c, 82U, 0x83234U, 0x83cccU, 0x83238U); if (result.status != TranslationStatus::complete) return result;
            h.begin(0x83238U);
        } else { h.clocks(2U); pf(h, 0x83238U); pf(h, 0x8323aU); }
    }

    pf(h, 0x8323cU); const auto reset = rw(h, a6 + 0x20U); logic(r, reset, 0x8000U); pf(h, 0x8323eU);
    if (reset != 0U) {
        h.clocks(4U); // BEQ.b not taken.
        pf(h, 0x83240U);
        const auto result = call(h, c, 83U, 0x8323eU, 0x83d24U, 0x83242U); if (result.status != TranslationStatus::complete) return result;
        h.begin(0x83242U);
    } else { h.clocks(2U); pf(h, 0x83242U); pf(h, 0x83244U); }

    r.address[3] = a6 + 0x180U;
    pf(h, 0x83246U); pf(h, 0x83248U); pf(h, 0x8324aU); pf(h, 0x8324cU);
    wb(h, a6 + 0xf0U, 0x12U); logic(r, 0x12U, 0x80U);

    unsigned loop_prefetch_state = 1U;
    for (;;) {
        if (loop_prefetch_state == 0U) pf(h, 0x8324cU);
        if (loop_prefetch_state != 2U) pf(h, 0x8324eU);
        pf(h, 0x83250U);
        const auto state = rb(h, r.address[3]); logic(r, state, 0x80U);
        loop_prefetch_state = 0U;
        pf(h, 0x83252U);
        if ((state & 0x80U) != 0U) {
            h.clocks(4U); // BPL.b not taken.
            pf(h, 0x83254U); pf(h, 0x83256U); const auto channel = rb(h, r.address[3] + 2U);
            r.data[7] = (r.data[7] & 0xffffff00U) | channel; logic(r, channel, 0x80U);
            pf(h, 0x83258U); r.data[7] &= 0xffffff07U; logic(r, static_cast<std::uint8_t>(r.data[7]), 0x80U);
            pf(h, 0x8325aU); pf(h, 0x8325cU);
            const auto result = call(h, c, 77U, 0x8325aU, 0x83268U, 0x8325cU); if (result.status != TranslationStatus::complete) return result;
            h.begin(0x8325cU);
        } else { h.clocks(2U); pf(h, 0x8325cU); pf(h, 0x8325eU); }
        r.address[3] += 0x50U;
        pf(h, 0x83260U); pf(h, 0x83262U); pf(h, 0x83264U); const auto old = rb(h, a6 + 0xf0U);
        const auto value = static_cast<std::uint8_t>(old - 1U); pf(h, 0x83266U); wb(h, a6 + 0xf0U, value); subb(r, old, 1U, value);
        if (value != 0U) { h.clocks(2U); pf(h, 0x8324cU); pf(h, 0x8324eU); loop_prefetch_state = 2U; continue; }
        h.clocks(4U); // BNE.b not taken; RTS follows.
        pf(h, 0x83268U); return ret(h, r);
    }
}
} // namespace gain_ground::translated
