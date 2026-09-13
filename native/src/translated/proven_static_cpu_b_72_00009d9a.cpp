#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kSpriteRegion = 7U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value) noexcept
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (value == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

std::uint16_t read_postincrement_word(ExecutionHost &host,
                                      CpuRegisters &registers,
                                      unsigned address_register) noexcept
{
    const auto value = host.read_memory_word(kPrivateRegion,
        registers.address[address_register], kWordMask);
    registers.address[address_register] += 2U;
    return value;
}

std::uint32_t pop_return(FunctionContext &context) noexcept
{
    auto &registers = context.registers;
    const auto stack = registers.address[7];
    const auto high = context.host->read_memory_word(
        kPrivateRegion, stack, kWordMask);
    const auto low = context.host->read_memory_word(
        kPrivateRegion, stack + 2U, kWordMask);
    registers.address[7] = stack + 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult proven_static_cpu_b_72_00009d9a(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &registers = context.registers;
    auto &host = *context.host;

    const auto value = read_postincrement_word(host, registers, 2U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    set_move_word_flags(registers, value);

    registers.address[0] = 0x0020c000U;
    const auto initial_offset = static_cast<std::int16_t>(
        read_postincrement_word(host, registers, 2U));
    registers.address[0] = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(registers.address[0]) + initial_offset);

    registers.data[2] = 2U;
    set_move_word_flags(registers, 2U);
    registers.address[1] = 0x0000a0e4U;

    for (;;) {
        const auto count = read_postincrement_word(host, registers, 1U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | count;
        set_move_word_flags(registers, count);

        for (;;) {
            host.write_memory_word(kSpriteRegion,
                registers.address[0] - 0x0020c000U, value, kWordMask);
            set_move_word_flags(registers, value);
            registers.address[0] += 8U;

            const auto decremented = static_cast<std::uint16_t>(
                registers.data[1] - 1U);
            registers.data[1] = (registers.data[1] & 0xffff0000U) | decremented;
            if (decremented == 0xffffU)
                break;
        }

        const auto row_offset = static_cast<std::int16_t>(
            read_postincrement_word(host, registers, 1U));
        registers.address[0] = static_cast<std::uint32_t>(
            static_cast<std::int64_t>(registers.address[0]) + row_offset);

        const auto decremented = static_cast<std::uint16_t>(
            registers.data[2] - 1U);
        registers.data[2] = (registers.data[2] & 0xffff0000U) | decremented;
        if (decremented == 0xffffU)
            break;
    }

    const auto target = pop_return(context);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
