#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kCpuBMainMemoryRegion = 2U;
constexpr std::uint16_t kFullWordMask = 0xffffU;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kCpuBMainMemoryRegion, address, kFullWordMask);
    const auto low = host.read_memory_word(kCpuBMainMemoryRegion, address + 2U, kFullWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

} // namespace

FunctionResult cpu_b_update_record_position_fields(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    const std::uint32_t base = context.registers.address[5];
    const auto source = host.read_memory_word(kCpuBMainMemoryRegion, base + 0x12U, kFullWordMask);
    host.write_memory_word(kCpuBMainMemoryRegion, base + 0x0eU, source, kFullWordMask);

    const auto vertical = host.read_memory_word(kCpuBMainMemoryRegion, base + 0x1aU, kFullWordMask);
    const auto adjusted = static_cast<std::uint16_t>(0x01efU - vertical);
    const auto delta = host.read_memory_word(kCpuBMainMemoryRegion, base + 0x16U, kFullWordMask);
    const std::uint32_t wide = static_cast<std::uint32_t>(adjusted) + delta;
    const auto result = static_cast<std::uint16_t>(wide);
    context.registers.data[0] = (context.registers.data[0] & 0xffff0000U) | result;

    std::uint16_t flags{};
    if (wide > 0xffffU)
        flags |= kExtend;
    if ((result & 0x8000U) != 0U)
        flags |= kNegative;
    if (result == 0U)
        flags |= kZero;
    context.registers.status = static_cast<std::uint16_t>((context.registers.status & ~0x001fU) | flags);
    host.write_memory_word(kCpuBMainMemoryRegion, base + 0x0cU, result, kFullWordMask);

    const std::uint32_t return_address = read_long(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
