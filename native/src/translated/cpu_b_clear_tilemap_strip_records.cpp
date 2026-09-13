#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kTileBase = 0x00200000U;

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(
        kPrivateRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_clear_tilemap_strip_records(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto selector = host.read_memory_word(
        kPrivateRegion, registers.address[5] + 0x7aU, kWordMask);
    registers.address[0] = 0x00200002U
        + static_cast<std::uint32_t>(static_cast<std::int32_t>(
            static_cast<std::int16_t>(selector)));
    registers.data[0] = 0x0000000fU;
    registers.data[1] = 0U;

    const auto extend = static_cast<std::uint16_t>(registers.status & 0x0010U);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | extend | 0x0004U);

    for (std::uint32_t row = 0; row != 16U; ++row) {
        const auto offset = registers.address[0] - kTileBase;
        host.write_memory_word(kTileRegion, offset, 0U, kWordMask);
        host.write_memory_word(kTileRegion, offset + 2U, 0U, kWordMask);
        host.write_memory_word(kTileRegion, offset + 4U, 0U, kWordMask);
        host.write_memory_word(kTileRegion, offset + 6U, 0U, kWordMask);
        registers.address[0] += 0x80U;
        registers.data[0] = (registers.data[0] & 0xffff0000U)
            | static_cast<std::uint16_t>(registers.data[0] - 1U);
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
