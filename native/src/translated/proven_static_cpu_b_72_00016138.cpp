#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
void add_word(CpuRegisters &r, std::uint16_t left, std::uint16_t right,
    std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult proven_static_cpu_b_72_00016138(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};
    auto &r = context.registers;
    auto value = static_cast<std::uint16_t>(r.data[0]);
    auto result = static_cast<std::uint16_t>(value + 0x0030U);
    r.data[0] = (r.data[0] & 0xffff0000U) | result;
    add_word(r, value, 0x0030U, result);

    value = result;
    result = static_cast<std::uint16_t>(value << 1U);
    std::uint16_t flags{};
    if ((value & 0x8000U) != 0U) flags |= 0x0011U;
    if (((value ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    r.data[0] = (r.data[0] & 0xffff0000U) | result;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    r.program_counter = 0x00016142U;
    return context.host->call_function(589U, 1U, 0x72U, 1U,
        0x0001613eU, 0x00016142U, context);
}
} // namespace gain_ground::translated
