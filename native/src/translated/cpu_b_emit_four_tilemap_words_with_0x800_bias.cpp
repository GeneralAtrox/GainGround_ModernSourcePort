#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto high = host.read_memory_word(kRegion, r.address[7], kMask);
    const auto low = host.read_memory_word(kRegion, r.address[7] + 2U, kMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_emit_four_tilemap_words_with_0x800_bias(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    r.data[3] = 3U;
    for (;;) {
        const auto d2 = static_cast<std::uint16_t>(r.data[2]);
        const auto rotated = static_cast<std::uint16_t>((d2 << 4U) | (d2 >> 12U));
        r.data[2] = (r.data[2] & 0xffff0000U) | rotated;
        const auto nibble = static_cast<std::uint16_t>(rotated & 0x000fU);
        r.data[0] = (r.data[0] & 0xffff0000U) | nibble;

        std::uint16_t output{};
        const auto counter = static_cast<std::uint16_t>(r.data[3]);
        if (nibble != 0U) {
            r.data[3] |= 0x80000000U;
            output = static_cast<std::uint16_t>(((nibble + 0x0030U) << 1U) + 0x0800U);
        } else if (counter == 0U) {
            output = 0x0860U;
        } else if ((r.data[3] & 0x80000000U) != 0U) {
            r.data[3] |= 0x80000000U;
            output = 0x0860U;
        } else {
            r.data[0] = 0U;
            output = 0x0800U;
        }
        r.data[0] = (r.data[0] & 0xffff0000U) | output;
        host.write_memory_word(kRegion, r.address[0], output, kMask);
        r.address[0] += 2U;
        r.status = static_cast<std::uint16_t>(r.status & ~0x001fU);

        const auto decremented = static_cast<std::uint16_t>(counter - 1U);
        r.data[3] = (r.data[3] & 0xffff0000U) | decremented;
        if (decremented == 0xffffU)
            break;
    }
    const auto return_address = pop_return(host, r);
    r.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}
} // namespace gain_ground::translated
