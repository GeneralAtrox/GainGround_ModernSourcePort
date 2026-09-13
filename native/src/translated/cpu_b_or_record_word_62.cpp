#include "gain_ground/contract_types.h"
#include "cpu_b_or_record_word_62_common.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kTileBase = 0x00200000U;

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    const auto extend = static_cast<std::uint16_t>(registers.status & 0x0010U);
    std::uint16_t flags = extend;
    if (value == 0U) flags |= 0x0004U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void addq_word_one(CpuRegisters &registers)
{
    const auto before = static_cast<std::uint16_t>(registers.data[0]);
    const auto result = static_cast<std::uint16_t>(before + 1U);
    const bool carry = result == 0U;
    const bool overflow = before == 0x7fffU;
    std::uint16_t flags = 0U;
    if (carry) flags |= 0x0011U;
    if (overflow) flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
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

FunctionResult cpu_b_or_record_word_62_common(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto record_word = host.read_memory_word(
        kPrivateRegion, registers.address[5] + 0x62U, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U)
        | static_cast<std::uint16_t>(registers.data[0] | record_word);
    set_move_word_flags(registers, static_cast<std::uint16_t>(registers.data[0]));

    registers.data[4] = 6U;
    set_move_word_flags(registers, 6U);
    const auto selector = host.read_memory_word(
        kPrivateRegion, registers.address[5] + 0x7aU, kWordMask);
    registers.address[0] += static_cast<std::uint32_t>(static_cast<std::int32_t>(
        static_cast<std::int16_t>(selector)));

    for (std::uint32_t row = 0; row != 7U; ++row) {
        registers.address[1] = registers.address[0];
        registers.data[3] = 6U;
        set_move_word_flags(registers, 6U);
        for (std::uint32_t column = 0; column != 7U; ++column) {
            const auto value = static_cast<std::uint16_t>(registers.data[0]);
            host.write_memory_word(kTileRegion,
                registers.address[1] - kTileBase, value, kWordMask);
            registers.address[1] += 2U;
            set_move_word_flags(registers, value);
            addq_word_one(registers);
            registers.data[3] = (registers.data[3] & 0xffff0000U)
                | static_cast<std::uint16_t>(registers.data[3] - 1U);
        }
        registers.address[0] += 0x80U;
        registers.data[4] = (registers.data[4] & 0xffff0000U)
            | static_cast<std::uint16_t>(registers.data[4] - 1U);
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

FunctionResult cpu_b_or_record_word_62(FunctionContext &context) noexcept
{
    return cpu_b_or_record_word_62_common(context);
}

} // namespace gain_ground::translated
