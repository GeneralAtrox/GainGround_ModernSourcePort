#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kCpuBMainMemoryRegion = 2U;
constexpr std::uint16_t kFullWordMask = 0xffffU;
constexpr std::uint32_t kMainMemoryMask = 0x0003ffffU;

void set_logic_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    constexpr std::uint16_t kConditionCodeMaskWithoutExtend = 0x000fU;
    std::uint16_t flags{};
    if ((value & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (value == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMaskWithoutExtend) | flags);
}

[[nodiscard]] std::uint16_t add_word(
    CpuRegisters &registers, std::uint16_t left, std::uint16_t right)
{
    const auto wide = static_cast<std::uint32_t>(left) + right;
    const auto result = static_cast<std::uint16_t>(wide);
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (result == 0U)
        flags |= 0x0004U;
    if ((((~(left ^ right)) & (left ^ result)) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (wide > 0xffffU)
        flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    return result;
}

[[nodiscard]] std::uint16_t subtract_word(
    CpuRegisters &registers, std::uint16_t left, std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left - right);
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (result == 0U)
        flags |= 0x0004U;
    if ((((left ^ right) & (left ^ result)) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (left < right)
        flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    return result;
}

void load_d0_word(CpuRegisters &registers, std::uint16_t value)
{
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    set_logic_word_flags(registers, value);
}

void write_move_word(
    ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(
        kCpuBMainMemoryRegion, address & kMainMemoryMask, value, kFullWordMask);
    set_logic_word_flags(registers, value);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack = registers.address[7] & kMainMemoryMask;
    const auto high = host.read_memory_word(
        kCpuBMainMemoryRegion, stack, kFullWordMask);
    const auto low = host.read_memory_word(
        kCpuBMainMemoryRegion, (stack + 2U) & kMainMemoryMask, kFullWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

} // namespace

FunctionResult cpu_b_compute_object_bounds(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];

    auto value = host.read_memory_word(
        kCpuBMainMemoryRegion, (base + 0x12U) & kMainMemoryMask, kFullWordMask);
    load_d0_word(registers, value);
    value = subtract_word(registers, value, 5U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    write_move_word(host, registers, base + 0x2aU, value);
    value = add_word(registers, value, 10U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    write_move_word(host, registers, base + 0x2cU, value);

    value = host.read_memory_word(
        kCpuBMainMemoryRegion, (base + 0x16U) & kMainMemoryMask, kFullWordMask);
    load_d0_word(registers, value);
    const auto clear_address = (base + 0x2eU) & kMainMemoryMask;
    (void)host.read_memory_word(
        kCpuBMainMemoryRegion, clear_address, kFullWordMask);
    host.write_memory_word(
        kCpuBMainMemoryRegion, clear_address, 0U, kFullWordMask);
    set_logic_word_flags(registers, 0U);
    value = add_word(registers, value, 12U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    write_move_word(host, registers, base + 0x30U, value);

    value = host.read_memory_word(
        kCpuBMainMemoryRegion, (base + 0x1aU) & kMainMemoryMask, kFullWordMask);
    load_d0_word(registers, value);
    value = subtract_word(registers, value, 4U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    write_move_word(host, registers, base + 0x32U, value);
    value = add_word(registers, value, 8U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    write_move_word(host, registers, base + 0x34U, value);

    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
