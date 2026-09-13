#include "gain_ground/contract_types.h"

#include <cstdint>
#include <limits>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgram = 1U;
constexpr std::uint16_t kShared = 3U;
constexpr std::uint16_t kMask = 0xffffU;

void pf(ExecutionHost &h, std::uint32_t a)
{
    (void)h.read_memory_word(kProgram, a, kMask);
}
void ws(ExecutionHost &h, std::uint32_t a, std::uint16_t v)
{
    h.write_memory_word(kShared, a & 0x3ffffU, v, kMask);
}
std::uint16_t rs(ExecutionHost &h, std::uint32_t a)
{
    return h.read_memory_word(kShared, a & 0x3ffffU, kMask);
}
void logic(CpuRegisters &r, std::uint32_t v, std::uint32_t sign)
{
    std::uint16_t f = r.status & 0x10U;
    if ((v & sign) != 0U) f |= 8U;
    if (v == 0U) f |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void arithmetic_word(CpuRegisters &r, std::uint16_t left,
    std::uint16_t right, std::uint16_t result, bool subtract)
{
    const bool carry = subtract ? right > left
        : static_cast<std::uint32_t>(left) + right > 0xffffU;
    const bool overflow = subtract
        ? (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        : (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U);
    std::uint16_t f{};
    if (carry) f |= 0x11U;
    if (overflow) f |= 2U;
    if ((result & 0x8000U) != 0U) f |= 8U;
    if (result == 0U) f |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void compare_word(CpuRegisters &r, std::uint16_t left, std::uint16_t right)
{
    const auto x = r.status & 0x10U;
    const auto result = static_cast<std::uint16_t>(left - right);
    arithmetic_word(r, left, right, result, true);
    r.status = static_cast<std::uint16_t>((r.status & ~0x10U) | x);
}
std::uint8_t hardware_byte(FunctionContext &c, std::uint32_t pc,
    std::uint32_t a)
{
    const bool odd = (a & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto value = c.host->read_hardware(1U, 0U, 0xffU, pc, a & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? value : value >> 8U);
}
FunctionResult call_catalogued(FunctionContext &c, std::uint32_t id,
    std::uint32_t site, std::uint32_t target, std::uint32_t resume)
{
    auto &h = *c.host; auto &r = c.registers;
    r.address[7] -= 4U;
    ws(h, r.address[7], static_cast<std::uint16_t>(resume >> 16U));
    ws(h, r.address[7] + 2U, static_cast<std::uint16_t>(resume));
    pf(h, target);
    pf(h, target + 2U);
    r.program_counter = target;
    return h.call_function(id, 0U, 0xffU, 2U, site, target, c);
}
FunctionResult return_subroutine(FunctionContext &c)
{
    auto &h = *c.host; auto &r = c.registers;
    const auto high = rs(h, r.address[7]);
    const auto low = rs(h, r.address[7] + 2U);
    r.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    pf(h, target); pf(h, target + 2U);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
FunctionResult crc_continuation(FunctionContext &c, std::uint32_t site,
    std::uint32_t target, std::uint32_t resume)
{
    auto &h = *c.host; auto &r = c.registers;
    r.address[7] -= 4U;
    ws(h, r.address[7], static_cast<std::uint16_t>(resume >> 16U));
    ws(h, r.address[7] + 2U, static_cast<std::uint16_t>(resume));
    pf(h, target);
    pf(h, target + 2U);
    r.program_counter = target;
    (void)h.call_function(std::numeric_limits<std::uint32_t>::max(),
        0U, 0xffU, 2U, site, target, c);
    if (target == 0x00001d32U) {
        r.data[0] = 0U;
        logic(r, 0U, 0x80000000U);
    }
    bool pf1d36_prefetched = target == 0x00001d34U;
    for (;;) {
        if (!pf1d36_prefetched) pf(h, 0x00001d36U);
        pf1d36_prefetched = false;
        pf(h, 0x00001d38U);
        const auto high = hardware_byte(c, 0x00001d34U, r.address[0]);
        const auto low = hardware_byte(c, 0x00001d34U, r.address[0] + 2U);
        const auto moved = static_cast<std::uint16_t>(
            (static_cast<std::uint16_t>(high) << 8U) | low);
        r.data[2] = (r.data[2] & 0xffff0000U) | moved;
        r.address[0] += 4U;
        pf(h, 0x00001d3aU);
        auto crc = static_cast<std::uint16_t>(r.data[0]) ^ moved;
        r.data[0] = (r.data[0] & 0xffff0000U) | crc;
        logic(r, crc, 0x8000U);
        pf(h, 0x00001d3cU);
        const bool carry = (crc & 1U) != 0U;
        crc = static_cast<std::uint16_t>(crc >> 1U);
        r.data[0] = (r.data[0] & 0xffff0000U) | crc;
        std::uint16_t flags = carry ? 0x11U : 0U;
        if (crc == 0U) flags |= 4U;
        r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
        pf(h, 0x00001d3eU);
        pf(h, 0x00001d40U);
        if (carry) {
            pf(h, 0x00001d42U);
            crc = static_cast<std::uint16_t>(crc ^ 0x8810U);
            r.data[0] = (r.data[0] & 0xffff0000U) | crc;
            logic(r, crc, 0x8000U);
        }
        pf(h, 0x00001d44U); pf(h, 0x00001d46U);
        const auto count = static_cast<std::uint16_t>(r.data[1] - 1U);
        r.data[1] = (r.data[1] & 0xffff0000U) | count;
        pf(h, 0x00001d34U);
        if (count != 0xffffU) continue;
        pf(h, 0x00001d48U); pf(h, 0x00001d4aU);
        return return_subroutine(c);
    }
}
} // namespace

FunctionResult proven_static_cpu_a_plain_00001c68(FunctionContext &c) noexcept
{
    if (c.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                c.registers.program_counter};
    auto &h = *c.host; auto &r = c.registers;

    pf(h, 0x00001c6cU);
    r.address[0] = 0x00001d18U;
    r.data[1] = 4U; logic(r, 4U, 0x80000000U);
    pf(h, 0x00001c6eU); pf(h, 0x00001c70U);
    auto child = call_catalogued(c, 449U, 0x00001c6eU, 0x00001cd8U, 0x00001c72U);
    if (child.status != TranslationStatus::complete) return child;

    r.address[0] = r.address[6];
    r.data[1] = 3U; logic(r, 3U, 0x80000000U);
    pf(h, 0x00001c76U); pf(h, 0x00001c78U);
    child = call_catalogued(c, 449U, 0x00001c76U, 0x00001cd8U, 0x00001c7aU);
    if (child.status != TranslationStatus::complete) return child;

    r.address[6] += 4U;
    const auto d3 = static_cast<std::uint16_t>(r.data[6]);
    r.data[3] = (r.data[3] & 0xffff0000U) | d3; logic(r, d3, 0x8000U);
    pf(h, 0x00001c7eU); r.address[0] = r.address[4];
    pf(h, 0x00001c80U);
    const auto d1 = static_cast<std::uint16_t>(r.data[4]);
    r.data[1] = (r.data[1] & 0xffff0000U) | d1; logic(r, d1, 0x8000U);
    pf(h, 0x00001c82U);
    pf(h, 0x00001c84U);
    h.write_hardware(2U, 0U, 0xffU, 0x00001c82U, r.address[5],
        static_cast<std::uint16_t>(r.data[5]), kMask);
    logic(r, static_cast<std::uint16_t>(r.data[5]), 0x8000U);
    pf(h, 0x00001c86U);
    child = crc_continuation(c, 0x00001c84U, 0x00001d32U, 0x00001c88U);
    if (child.status != TranslationStatus::complete) return child;

    bool pf1c8a_prefetched = true;
    for (;;) {
        const auto left = static_cast<std::uint16_t>(r.data[3]);
        const auto result = static_cast<std::uint16_t>(left - 1U);
        r.data[3] = (r.data[3] & 0xffff0000U) | result;
        arithmetic_word(r, left, 1U, result, true);
        if (!pf1c8a_prefetched) pf(h, 0x00001c8aU);
        pf1c8a_prefetched = false;
        pf(h, 0x00001c8cU);
        if (result == 0U) { pf(h, 0x00001c9eU); break; }
        const auto old5 = static_cast<std::uint16_t>(r.data[5]);
        const auto new5 = static_cast<std::uint16_t>(old5 + 1U);
        r.data[5] = (r.data[5] & 0xffff0000U) | new5;
        arithmetic_word(r, old5, 1U, new5, false);
        pf(h, 0x00001c8eU);
        pf(h, 0x00001c90U);
        h.write_hardware(2U, 0U, 0xffU, 0x00001c8eU, r.address[5], new5, kMask);
        logic(r, new5, 0x8000U);
        pf(h, 0x00001c92U); pf(h, 0x00001c94U);
        r.address[0] -= 0x40000U;
        pf(h, 0x00001c96U);
        r.data[1] = 0xffffffffU; logic(r, r.data[1], 0x80000000U);
        pf(h, 0x00001c98U); pf(h, 0x00001c9aU);
        child = crc_continuation(c, 0x00001c98U, 0x00001d34U, 0x00001c9cU);
        if (child.status != TranslationStatus::complete) return child;
        pf(h, 0x00001c88U);
    }

    pf(h, 0x00001ca0U);
    pf(h, 0x00001ca2U);
    h.write_hardware(2U, 0U, 0xffU, 0x00001c9eU, r.address[5], 0U, kMask);
    logic(r, 0U, 0x8000U);
    pf(h, 0x00001ca4U);
    r.address[0] = 0x00001d22U;
    pf(h, 0x00001ca6U);
    pf(h, 0x00001ca8U);
    const auto source = h.read_hardware(1U, 0U, 0xffU, 0x00001ca6U,
        r.address[6], kMask);
    r.address[6] += 2U;
    compare_word(r, static_cast<std::uint16_t>(r.data[0]), source);
    pf(h, 0x00001caaU);
    bool pf1cb4_prefetched = false;
    if (static_cast<std::uint16_t>(r.data[0]) != source) {
        pf(h, 0x00001cacU);
        r.address[0] = 0x00001d28U;
        pf(h, 0x00001caeU); pf(h, 0x00001cb0U); pf(h, 0x00001cb2U);
        pf(h, 0x00001cb4U); pf1cb4_prefetched = true;
        ws(h, 0x0000ffc000U, 0xffffU); logic(r, 0xffffU, 0x8000U);
    }
    if (!pf1cb4_prefetched) pf(h, 0x00001cb4U);
    r.data[1] = 5U; logic(r, 5U, 0x80000000U);
    pf(h, 0x00001cb6U); pf(h, 0x00001cb8U);
    child = call_catalogued(c, 449U, 0x00001cb6U, 0x00001cd8U, 0x00001cbaU);
    if (child.status != TranslationStatus::complete) return child;

    r.data[0] = r.address[3]; logic(r, r.data[0], 0x80000000U);
    auto word = static_cast<std::uint16_t>(r.data[0]);
    r.data[1] = (r.data[1] & 0xffff0000U) | word; logic(r, word, 0x8000U);
    pf(h, 0x00001cbeU); pf(h, 0x00001cc0U);
    word = static_cast<std::uint16_t>(word & 0x007fU);
    r.data[1] = (r.data[1] & 0xffff0000U) | word; logic(r, word, 0x8000U);
    pf(h, 0x00001cc2U); pf(h, 0x00001cc4U);
    compare_word(r, word, 0x0078U); pf(h, 0x00001cc6U);
    pf(h, 0x00001cc8U);
    if (word >= 0x0078U) {
        pf(h, 0x00001ccaU);
        auto d0 = static_cast<std::uint16_t>(r.data[0] & 0xff80U);
        r.data[0] = (r.data[0] & 0xffff0000U) | d0; logic(r, d0, 0x8000U);
        pf(h, 0x00001cccU); pf(h, 0x00001cceU);
        const auto added = static_cast<std::uint16_t>(d0 + 0x0180U);
        r.data[0] = (r.data[0] & 0xffff0000U) | added;
        arithmetic_word(r, d0, 0x0180U, added, false);
        pf(h, 0x00001cd0U); r.address[3] = r.data[0];
    }
    pf(h, 0x00001cd2U);
    const auto old5 = static_cast<std::uint16_t>(r.data[5]);
    const auto sub5 = static_cast<std::uint16_t>(old5 - static_cast<std::uint16_t>(r.data[6]));
    r.data[5] = (r.data[5] & 0xffff0000U) | sub5;
    arithmetic_word(r, old5, static_cast<std::uint16_t>(r.data[6]), sub5, true);
    pf(h, 0x00001cd4U);
    const auto final5 = static_cast<std::uint16_t>(sub5 + 1U);
    r.data[5] = (r.data[5] & 0xffff0000U) | final5;
    arithmetic_word(r, sub5, 1U, final5, false);
    pf(h, 0x00001cd6U); pf(h, 0x00001cd8U);
    return return_subroutine(c);
}
} // namespace gain_ground::translated
