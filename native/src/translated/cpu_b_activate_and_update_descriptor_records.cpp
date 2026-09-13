#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;

std::uint16_t read_word(ExecutionHost &h, std::uint32_t a)
{ return h.read_memory_word(kRegion, a, kMask); }
std::uint32_t read_long(ExecutionHost &h, std::uint32_t a)
{ return (static_cast<std::uint32_t>(read_word(h, a)) << 16U) | read_word(h, a + 2U); }
std::uint8_t read_byte(ExecutionHost &h, std::uint32_t a)
{ const bool o = (a & 1U) != 0U; const auto v = h.read_memory_word(kRegion, a & ~1U, o ? 0x00ffU : 0xff00U); return static_cast<std::uint8_t>(o ? v : v >> 8U); }
void write_word(ExecutionHost &h, std::uint32_t a, std::uint16_t v)
{ h.write_memory_word(kRegion, a, v, kMask); }
void write_byte(ExecutionHost &h, std::uint32_t a, std::uint8_t v)
{ const bool o = (a & 1U) != 0U; h.write_memory_word(kRegion, a & ~1U, static_cast<std::uint16_t>(v) << (o ? 0U : 8U), o ? 0x00ffU : 0xff00U); }
void logicw(CpuRegisters &r, std::uint16_t v)
{ std::uint16_t f = r.status & 0x10U; if (v & 0x8000U) f |= 8U; if (!v) f |= 4U; r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f); }
void logicb(CpuRegisters &r, std::uint8_t v)
{ std::uint16_t f = r.status & 0x10U; if (v & 0x80U) f |= 8U; if (!v) f |= 4U; r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f); }
void push_return(ExecutionHost &h, CpuRegisters &r, std::uint32_t v)
{ r.address[7] -= 4U; write_word(h, r.address[7], static_cast<std::uint16_t>(v >> 16U)); write_word(h, r.address[7] + 2U, static_cast<std::uint16_t>(v)); }
std::uint32_t pop_return(ExecutionHost &h, CpuRegisters &r)
{ const auto v = read_long(h, r.address[7]); r.address[7] += 4U; return v; }
} // namespace

FunctionResult cpu_b_activate_and_update_descriptor_records(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;
    auto count = read_word(h, r.address[3] + 6U);
    r.data[1] = (r.data[1] & 0xffff0000U) | count;
    logicw(r, count);
    r.address[1] = read_long(h, r.address[3] + 0x20U);

    for (;;) {
        const auto flags = read_byte(h, r.address[6]);
        write_byte(h, r.address[6], static_cast<std::uint8_t>(flags | 0x80U));
        if ((flags & 0x80U) == 0U) r.status = static_cast<std::uint16_t>(r.status | 4U);
        else r.status = static_cast<std::uint16_t>(r.status & ~4U);
        const auto value = read_word(h, r.address[6] + 0x48U);
        r.data[0] = (r.data[0] & 0xffff0000U) | value;
        logicw(r, value);
        push_return(h, r, 0x00011380U);
        r.program_counter = 0x00010c9aU;
        const auto child = h.call_function(223U, 1U, 0x72U, 2U,
            0x0001137cU, 0x00010c9aU, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;

        std::uint8_t next{};
        do {
            r.address[6] += 0x80U;
            next = read_byte(h, r.address[6]);
            logicb(r, next);
        } while ((next & 0x80U) != 0U);
        count = static_cast<std::uint16_t>(r.data[1] - 1U);
        r.data[1] = (r.data[1] & 0xffff0000U) | count;
        if (count == 0xffffU) break;
    }

    const auto target = pop_return(h, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
