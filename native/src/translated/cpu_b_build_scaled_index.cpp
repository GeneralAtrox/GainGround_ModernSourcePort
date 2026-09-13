#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_add_long_flags(CpuRegisters &registers,
    std::uint32_t left, std::uint32_t right, std::uint32_t result)
{
    std::uint16_t flags{};
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x80000000U) != 0U)
        flags |= 0x0002U;
    if (static_cast<std::uint64_t>(left) + right > 0xffffffffULL)
        flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_b_build_scaled_index(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &registers = context.registers;
    std::uint32_t accumulator{};
    std::uint32_t rotated = registers.data[0];
    std::uint32_t digit{};

    for (unsigned iteration = 0; iteration != 8U; ++iteration) {
        const auto doubled = accumulator + accumulator;
        const auto saved = doubled;
        const auto quadrupled = doubled + doubled;
        const auto octupled = quadrupled + quadrupled;
        accumulator = octupled + saved;

        rotated = (rotated << 4U) | (rotated >> 28U);
        digit = rotated & 0x0000000fU;
        const auto before_add = accumulator;
        accumulator += digit;
        set_add_long_flags(registers, before_add, digit, accumulator);
    }

    registers.data[0] = rotated;
    registers.data[1] = accumulator;
    registers.data[2] = 0x0000ffffU;
    registers.data[3] = digit;
    const auto return_address = read_long(*context.host, registers.address[7]);
    registers.address[7] += 4U;
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
