#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgram = 1U;
constexpr std::uint16_t kShared = 3U;
void pf(ExecutionHost &h, std::uint32_t a)
{
    (void)h.read_memory_word(kProgram, a, 0xffffU);
}
void ws(ExecutionHost &h, std::uint32_t a, std::uint16_t v)
{
    h.write_memory_word(kShared, a & 0x3ffffU, v, 0xffffU);
}
std::uint16_t rs(ExecutionHost &h, std::uint32_t a)
{
    return h.read_memory_word(kShared, a & 0x3ffffU, 0xffffU);
}
FunctionResult call_448(FunctionContext &c, std::uint32_t site,
    std::uint32_t resume)
{
    auto &h = *c.host; auto &r = c.registers;
    r.address[7] -= 4U;
    ws(h, r.address[7], static_cast<std::uint16_t>(resume >> 16U));
    ws(h, r.address[7] + 2U, static_cast<std::uint16_t>(resume));
    pf(h, 0x00001c68U); pf(h, 0x00001c6aU);
    r.program_counter = 0x00001c68U;
    return h.call_function(448U, 0U, 0xffU, 2U,
        site, 0x00001c68U, c);
}
void add_word(CpuRegisters &r, std::uint16_t source)
{
    const auto left = static_cast<std::uint16_t>(r.data[5]);
    const auto sum = static_cast<std::uint32_t>(left) + source;
    const auto result = static_cast<std::uint16_t>(sum);
    std::uint16_t flags{};
    if (sum > 0xffffU) flags |= 0x0011U;
    if (((~(left ^ source)) & (left ^ result) & 0x8000U) != 0U) flags |= 2U;
    if ((result & 0x8000U) != 0U) flags |= 8U;
    if (result == 0U) flags |= 4U;
    r.data[5] = (r.data[5] & 0xffff0000U) | result;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_00001c5a(FunctionContext &c) noexcept
{
    if (c.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                c.registers.program_counter};
    auto &h = *c.host; auto &r = c.registers;
    auto child = call_448(c, 0x00001c5aU, 0x00001c5eU);
    if (child.status != TranslationStatus::complete) return child;
    r.address[4] += 1U;
    pf(h, 0x00001c62U);
    child = call_448(c, 0x00001c60U, 0x00001c64U);
    if (child.status != TranslationStatus::complete) return child;
    add_word(r, static_cast<std::uint16_t>(r.data[6]));
    pf(h, 0x00001c68U);
    const auto high = rs(h, r.address[7]);
    const auto low = rs(h, r.address[7] + 2U);
    r.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    pf(h, target); pf(h, target + 2U);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace gain_ground::translated
