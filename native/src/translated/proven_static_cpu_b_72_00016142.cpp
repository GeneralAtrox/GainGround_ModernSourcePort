#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;

void logic_word(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

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

FunctionResult proven_static_cpu_b_72_00016142(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    const auto left = static_cast<std::uint16_t>(r.data[0]);
    const auto combined = static_cast<std::uint16_t>(left | r.data[1]);
    r.data[0] = (r.data[0] & 0xffff0000U) | combined;
    logic_word(r, combined);

    host.write_memory_word(kTileRegion, r.address[0] - 0x00200000U,
        combined, kWordMask);
    r.address[0] += 2U;
    const auto incremented = static_cast<std::uint16_t>(combined + 1U);
    r.data[0] = (r.data[0] & 0xffff0000U) | incremented;
    add_word(r, combined, 1U, incremented);
    host.write_memory_word(kTileRegion, r.address[0] - 0x00200000U,
        incremented, kWordMask);
    r.address[0] += 0x007eU;

    const auto counter = static_cast<std::uint16_t>(r.data[3] - 1U);
    r.data[3] = (r.data[3] & 0xffff0000U) | counter;
    if (counter != 0xffffU) {
        r.program_counter = 0x00016122U;
        return host.call_function(585U, 1U, 0x72U, 1U,
            0x0001614eU, 0x00016122U, context);
    }

    const auto high = host.read_memory_word(
        kPrivateRegion, r.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, r.address[7] + 2U, kWordMask);
    r.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace gain_ground::translated
