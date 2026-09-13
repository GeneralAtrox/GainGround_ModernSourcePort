#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kPrivateMask = 0x0003ffffU;

[[nodiscard]] std::uint8_t read_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto shift = odd ? 0U : 8U;
    return static_cast<std::uint8_t>(host.read_memory_word(
        kPrivateRegion, address & ~1U, mask) >> shift);
}

void set_logic_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_compare_word_flags(
    CpuRegisters &registers, std::uint16_t destination,
    std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (destination < source) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] bool test_bit_one(
    CpuRegisters &registers, std::uint8_t value)
{
    const bool set = (value & 0x02U) != 0U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x0004U) | (set ? 0U : 0x0004U));
    return set;
}

void push_return(
    ExecutionHost &host, CpuRegisters &registers, std::uint32_t target)
{
    registers.address[7] -= 4U;
    const auto stack = registers.address[7] & kPrivateMask;
    host.write_memory_word(kPrivateRegion, stack,
        static_cast<std::uint16_t>(target >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, (stack + 2U) & kPrivateMask,
        static_cast<std::uint16_t>(target), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack = registers.address[7] & kPrivateMask;
    const auto high = host.read_memory_word(
        kPrivateRegion, stack, kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, (stack + 2U) & kPrivateMask, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_load_pc_table_a1_alt2(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    if (registers.program_counter != 0x0000fb72U)
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};

    registers.address[1] = 0x000104daU;
    const auto a6_word = host.read_memory_word(kPrivateRegion,
        (registers.address[5] + 0x68U) & kPrivateMask, kWordMask);
    registers.address[6] = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(a6_word)));

    auto value = read_byte(host, registers.address[6] & kPrivateMask);
    registers.data[7] = (registers.data[7] & 0xffffff00U) | value;
    set_logic_byte_flags(registers, value);
    value = static_cast<std::uint8_t>(
        value | read_byte(host, 0x00000404U));
    registers.data[7] = (registers.data[7] & 0xffffff00U) | value;
    set_logic_byte_flags(registers, value);

    if (value != 0U) {
        push_return(host, registers, 0x0000fb86U);
        registers.program_counter = 0x0000fa30U;
        const auto child = host.call_function(182U, 1U, 0x72U, 2U,
            0x0000fb82U, 0x0000fa30U, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
        registers.address[1] = 0x000104f6U;
    }

    const bool enabled = test_bit_one(
        registers, read_byte(host, 0x00000833U));
    if (enabled) {
        const auto mode = host.read_memory_word(
            kPrivateRegion, 0x00000c00U, kWordMask);
        set_compare_word_flags(registers, mode, 3U);
        if (mode < 3U) {
            constexpr std::uint32_t kTarget = 0x00015fd4U;
            registers.program_counter = kTarget;
            (void)host.call_function(290U, 1U, 0x72U, 1U,
                0x0000fb9aU, kTarget, context);
            return FunctionResult::complete(3U, kTarget);
        }
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
