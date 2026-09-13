#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_move_long_flags(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(
    CpuRegisters &registers, std::uint16_t left, std::uint16_t right,
    std::uint16_t result)
{
    const auto wide = static_cast<std::uint32_t>(left) + right;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (wide > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack = registers.address[7];
    const auto high = host.read_memory_word(kPrivateRegion, stack, kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, stack + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult proven_static_cpu_b_72_0000ab32(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    if (registers.program_counter != 0x0000ab32U)
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};

    registers.address[0] = 0x00204000U;
    const auto offset = host.read_memory_word(
        kPrivateRegion, registers.address[2], kWordMask);
    registers.address[2] += 2U;
    registers.address[0] = static_cast<std::uint32_t>(
        registers.address[0] + static_cast<std::int16_t>(offset));

    registers.data[2] = 7U;
    set_move_long_flags(registers, registers.data[2]);
    for (;;) {
        registers.address[1] = registers.address[0];
        registers.data[1] = 7U;
        set_move_long_flags(registers, registers.data[1]);

        for (;;) {
            const auto value = static_cast<std::uint16_t>(registers.data[0]);
            host.write_memory_word(
                kTileRegion, registers.address[1] - 0x00200000U,
                value, kWordMask);
            registers.address[1] += 2U;
            set_move_word_flags(registers, value);

            const auto incremented = static_cast<std::uint16_t>(value + 1U);
            registers.data[0] = (registers.data[0] & 0xffff0000U) | incremented;
            set_add_word_flags(registers, value, 1U, incremented);

            const auto inner = static_cast<std::uint16_t>(
                registers.data[1] - 1U);
            registers.data[1] = (registers.data[1] & 0xffff0000U) | inner;
            if (inner == 0xffffU) break;
        }

        registers.address[0] += 0x80U;
        const auto outer = static_cast<std::uint16_t>(
            registers.data[2] - 1U);
        registers.data[2] = (registers.data[2] & 0xffff0000U) | outer;
        if (outer == 0xffffU) break;
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
