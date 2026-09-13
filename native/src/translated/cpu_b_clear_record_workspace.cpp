#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kTileBase = 0x00200000U;

void set_zero_flags(CpuRegisters &registers)
{
    const auto extend = static_cast<std::uint16_t>(registers.status & 0x0010U);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | extend | 0x0004U);
}

void clear_long(ExecutionHost &host, CpuRegisters &registers)
{
    const auto offset = registers.address[0] - kTileBase;
    (void)host.read_memory_word(kTileRegion, offset, kWordMask);
    (void)host.read_memory_word(kTileRegion, offset + 2U, kWordMask);
    host.write_memory_word(kTileRegion, offset + 2U, 0U, kWordMask);
    host.write_memory_word(kTileRegion, offset, 0U, kWordMask);
    registers.address[0] += 4U;
    set_zero_flags(registers);
}

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

FunctionResult cpu_b_clear_record_workspace(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[0] += 0x0cU;
    clear_long(host, registers);
    clear_long(host, registers);

    const auto word_offset = registers.address[0] - kTileBase;
    (void)host.read_memory_word(kTileRegion, word_offset, kWordMask);
    host.write_memory_word(kTileRegion, word_offset, 0U, kWordMask);
    registers.address[0] += 2U;
    set_zero_flags(registers);

    registers.address[0] += 0x0eU;
    registers.data[0] = 0x00000015U;
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);
    for (std::uint32_t index = 0; index != 22U; ++index) {
        clear_long(host, registers);
        registers.data[0] = (registers.data[0] & 0xffff0000U)
            | static_cast<std::uint16_t>(registers.data[0] - 1U);
    }
    registers.address[0] += 4U;

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
