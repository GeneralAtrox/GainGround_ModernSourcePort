#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;

std::uint16_t rw(ExecutionHost &h, std::uint32_t a)
{ return h.read_memory_word(kRegion, a, kMask); }
std::uint32_t rl(ExecutionHost &h, std::uint32_t a)
{ return (static_cast<std::uint32_t>(rw(h, a)) << 16U) | rw(h, a + 2U); }
std::uint8_t rb(ExecutionHost &h, std::uint32_t a)
{ const bool o = (a & 1U) != 0U; const auto v = h.read_memory_word(kRegion, a & ~1U, o ? 0x00ffU : 0xff00U); return static_cast<std::uint8_t>(o ? v : v >> 8U); }
void ww(ExecutionHost &h, std::uint32_t a, std::uint16_t v)
{ h.write_memory_word(kRegion, a, v, kMask); }
void wb(ExecutionHost &h, std::uint32_t a, std::uint8_t v)
{ const bool o = (a & 1U) != 0U; h.write_memory_word(kRegion, a & ~1U, static_cast<std::uint16_t>(v) << (o ? 0U : 8U), o ? 0x00ffU : 0xff00U); }

void logicw(CpuRegisters &r, std::uint16_t v)
{ std::uint16_t f = r.status & 0x10U; if (v & 0x8000U) f |= 8U; if (!v) f |= 4U; r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f); }
void logicb(CpuRegisters &r, std::uint8_t v)
{ std::uint16_t f = r.status & 0x10U; if (v & 0x80U) f |= 8U; if (!v) f |= 4U; r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f); }
void logicl(CpuRegisters &r, std::uint32_t v)
{ std::uint16_t f = r.status & 0x10U; if (v & 0x80000000U) f |= 8U; if (!v) f |= 4U; r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f); }
void addw(CpuRegisters &r, std::uint16_t l, std::uint16_t q, std::uint16_t v)
{ std::uint16_t f{}; if (v & 0x8000U) f |= 8U; if (!v) f |= 4U; if (((~(l ^ q)) & (l ^ v) & 0x8000U) != 0U) f |= 2U; if (static_cast<std::uint32_t>(l) + q > 0xffffU) f |= 0x11U; r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f); }

std::uint32_t pop_return(ExecutionHost &h, CpuRegisters &r)
{ const auto v = rl(h, r.address[7]); r.address[7] += 4U; return v; }
} // namespace

FunctionResult cpu_b_initialize_records_from_descriptor_sequence(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;

    r.address[7] -= 2U;
    ww(h, r.address[7], static_cast<std::uint16_t>(r.address[6]));
    logicw(r, static_cast<std::uint16_t>(r.address[6]));
    auto count = rw(h, r.address[3] + 6U);
    r.data[1] = (r.data[1] & 0xffff0000U) | count;
    logicw(r, count);
    r.address[1] = rl(h, r.address[3] + 0x10U) + 0x0cU;

    for (;;) {
        r.address[0] = rl(h, r.address[3] + 0x10U);
        auto high = rw(h, r.address[0]);
        auto low = rw(h, r.address[0] + 2U);
        ww(h, r.address[6] + 2U, high);
        ww(h, r.address[6] + 4U, low);
        r.address[0] += 4U;
        logicl(r, (static_cast<std::uint32_t>(high) << 16U) | low);

        const auto left = rw(h, r.address[5] + 0x66U);
        const auto right = rw(h, r.address[0]);
        r.address[0] += 2U;
        auto value = static_cast<std::uint16_t>(left + right);
        r.data[0] = (r.data[0] & 0xffff0000U) | value;
        addw(r, left, right, value);
        ww(h, r.address[6] + 8U, value);
        logicw(r, value);

        auto byte = rb(h, r.address[0]++);
        wb(h, r.address[6] + 0x3cU, byte);
        logicb(r, byte);
        byte = rb(h, r.address[0]++);
        wb(h, r.address[6] + 0x0bU, byte);
        logicb(r, byte);
        value = rw(h, r.address[0]);
        r.address[0] += 2U;
        ww(h, r.address[6] + 0x3aU, value);
        logicw(r, value);
        ww(h, r.address[6] + 0x36U, static_cast<std::uint16_t>(r.address[5]));
        logicw(r, static_cast<std::uint16_t>(r.address[5]));
        (void)rb(h, r.address[6] + 0x3fU);
        wb(h, r.address[6] + 0x3fU, 0U);
        logicb(r, 0U);
        value = rw(h, r.address[0]);
        r.address[0] += 2U;
        ww(h, r.address[6] + 6U, value);
        logicw(r, value);

        value = rw(h, r.address[1]);
        r.address[1] += 2U;
        r.data[0] = (r.data[0] & 0xffff0000U) | value;
        logicw(r, value);
        wb(h, r.address[6] + 0x3dU, static_cast<std::uint8_t>(value));
        logicb(r, static_cast<std::uint8_t>(value));
        value = static_cast<std::uint16_t>(value & 0xff00U);
        r.data[0] = (r.data[0] & 0xffff0000U) | value;
        logicw(r, value);
        ww(h, r.address[6] + 0x58U, value);
        logicw(r, value);
        value = rw(h, r.address[1]);
        r.address[1] += 2U;
        ww(h, r.address[6], value);
        logicw(r, value);

        do {
            r.address[6] += 0x80U;
            byte = rb(h, r.address[6]);
            logicb(r, byte);
        } while ((byte & 0x80U) != 0U);

        count = static_cast<std::uint16_t>(r.data[1] - 1U);
        r.data[1] = (r.data[1] & 0xffff0000U) | count;
        if (count == 0xffffU) break;
    }

    const auto saved_a6 = rw(h, r.address[7]);
    r.address[7] += 2U;
    r.address[6] = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(saved_a6)));
    const auto target = pop_return(h, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
