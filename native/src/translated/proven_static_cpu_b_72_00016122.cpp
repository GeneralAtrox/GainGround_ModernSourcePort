#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
void logic_word(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void logic_long(CpuRegisters &r, std::uint32_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void rotate_left_long_four(CpuRegisters &r)
{
    const auto original = r.data[2];
    const auto result = (original << 4U) | (original >> 28U);
    std::uint16_t flags = r.status & 0x0010U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if ((original & 0x10000000U) != 0U) flags |= 0x0001U;
    r.data[2] = result;
    r.status = static_cast<std::uint16_t>((r.status & ~0x000fU) | flags);
}

FunctionResult transfer(FunctionContext &c, std::uint32_t id,
    std::uint32_t site, std::uint32_t target)
{
    c.registers.program_counter = target;
    return c.host->call_function(id, 1U, 0x72U, 1U, site, target, c);
}
} // namespace

FunctionResult proven_static_cpu_b_72_00016122(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &r = context.registers;
    rotate_left_long_four(r);
    const auto moved = static_cast<std::uint16_t>(r.data[2]);
    r.data[0] = (r.data[0] & 0xffff0000U) | moved;
    logic_word(r, moved);
    const auto masked = static_cast<std::uint16_t>(moved & 0x000fU);
    r.data[0] = (r.data[0] & 0xffff0000U) | masked;
    logic_word(r, masked);
    if (masked != 0U)
        return transfer(context, 586U, 0x0001612aU, 0x00016134U);

    logic_word(r, static_cast<std::uint16_t>(r.data[3]));
    if (static_cast<std::uint16_t>(r.data[3]) == 0U)
        return transfer(context, 587U, 0x0001612eU, 0x00016138U);

    logic_long(r, r.data[3]);
    if ((r.status & 0x0008U) != 0U)
        return transfer(context, 586U, 0x00016132U, 0x00016134U);
    return transfer(context, 588U, 0x00016132U, 0x00016140U);
}

} // namespace gain_ground::translated
