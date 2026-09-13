#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void write_word_postincrement(ExecutionHost &host, CpuRegisters &registers,
    std::uint16_t value)
{
    host.write_memory_word(kRegion, registers.address[0], value, kWordMask);
    registers.address[0] += 2U;
    const auto extend = static_cast<std::uint16_t>(registers.status & 0x0010U);
    std::uint16_t flags = extend;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(kRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(kRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_prepare_palette_block_descriptor(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[0] = 0x00007452U;
    write_word_postincrement(host, registers, 0x0080U);
    write_word_postincrement(host, registers, 0x0030U);
    write_word_postincrement(host, registers, 0x0803U);
    host.write_memory_word(kRegion, registers.address[0], 0x0002U, kWordMask);
    host.write_memory_word(kRegion, registers.address[0] + 2U, 0x4e46U, kWordMask);
    registers.address[0] += 4U;
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
