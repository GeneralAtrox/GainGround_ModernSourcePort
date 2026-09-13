#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;
constexpr std::uint16_t kOverflow = 0x0002U;
constexpr std::uint16_t kCarry = 0x0001U;

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    const auto word = host.read_memory_word(kPrivateRegion, address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U));
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
                     std::uint32_t sign) noexcept
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & sign) != 0U)
        flags |= kNegative;
    if (value == 0U)
        flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers, std::uint16_t left,
                        std::uint16_t right, std::uint16_t result) noexcept
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U)
        flags |= kNegative;
    if (result == 0U)
        flags |= kZero;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= kOverflow;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU)
        flags |= static_cast<std::uint16_t>(kExtend | kCarry);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_return(FunctionContext &context, std::uint32_t address) noexcept
{
    auto &registers = context.registers;
    registers.address[7] -= 4U;
    context.host->write_memory_word(kPrivateRegion, registers.address[7],
        static_cast<std::uint16_t>(address >> 16U), kWordMask);
    context.host->write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(address), kWordMask);
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

FunctionResult proven_static_cpu_b_72_0000f2b4(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &registers = context.registers;
    auto &host = *context.host;

    registers.address[0] = 0x00200202U;
    const auto tile_offset = static_cast<std::int16_t>(host.read_memory_word(
        kPrivateRegion, registers.address[5] + 0x7aU, kWordMask));
    registers.address[0] = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(registers.address[0]) + tile_offset);
    registers.address[1] = registers.address[4] + 0x98U;
    registers.address[2] = 0x00010676U;

    registers.data[1] = 3U;
    set_logic_flags(registers, registers.data[1], 0x80000000U);

    for (;;) {
        registers.data[0] = 0U;
        set_logic_flags(registers, registers.data[0], 0x80000000U);

        const auto index_byte = read_byte(host, registers.address[1]);
        ++registers.address[1];
        registers.data[0] = index_byte;
        set_logic_flags(registers, index_byte, 0x80U);

        const auto before_double = static_cast<std::uint16_t>(registers.data[0]);
        const auto doubled = static_cast<std::uint16_t>(before_double + before_double);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | doubled;
        set_add_word_flags(registers, before_double, before_double, doubled);

        const auto table_value = host.read_memory_word(kPrivateRegion,
            registers.address[2] + static_cast<std::int16_t>(doubled), kWordMask);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | table_value;
        set_logic_flags(registers, table_value, 0x8000U);

        const auto bias = host.read_memory_word(
            kPrivateRegion, registers.address[5] + 0x62U, kWordMask);
        const auto combined = static_cast<std::uint16_t>(table_value | bias);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | combined;
        set_logic_flags(registers, combined, 0x8000U);

        push_return(context, 0x0000f2dcU);
        registers.program_counter = 0x000161eaU;
        const auto child = host.call_function(
            298U, 1U, 0x72U, 2U, 0x0000f2d6U, 0x000161eaU, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;

        registers.address[0] += 0x7eU;
        const auto decremented = static_cast<std::uint16_t>(registers.data[1] - 1U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | decremented;
        if (decremented == 0xffffU)
            break;
    }

    const auto target = pop_return(context);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
