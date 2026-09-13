#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kPrivateRegion, address, kWordMask);
}

void write_word(
    ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kPrivateRegion, address, value, kWordMask);
}

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = read_word(host, address);
    const auto low = read_word(host, address + 2U);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(
    ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

void set_nzvc_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void subtract_word(
    CpuRegisters &registers, std::uint16_t source, std::uint16_t result)
{
    const auto destination = static_cast<std::uint16_t>(result + source);
    const bool source_negative = (source & 0x8000U) != 0U;
    const bool destination_negative = (destination & 0x8000U) != 0U;
    const bool result_negative = (result & 0x8000U) != 0U;
    const bool carry = source > destination;
    const bool overflow =
        (source_negative != destination_negative) &&
        (result_negative != destination_negative);

    std::uint16_t flags = 0U;
    if (carry) flags |= 0x0011U;
    if (result_negative) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

std::uint16_t arithmetic_shift_left_two(
    CpuRegisters &registers, std::uint16_t value)
{
    const auto result = static_cast<std::uint16_t>(value << 2U);
    const bool carry = (value & 0x4000U) != 0U;
    const bool overflow = ((value ^ (value << 1U)) & 0x8000U) != 0U ||
        (((value << 1U) ^ (value << 2U)) & 0x8000U) != 0U;

    std::uint16_t flags = 0U;
    if (carry) flags |= 0x0011U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    return result;
}

FunctionResult call_function_631(FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], 0x0000b710U);
    registers.program_counter = 0x0000b71aU;
    return host.call_function(
        631U, 1U, 0x72U, 2U, 0x0000b70cU, 0x0000b71aU, context);
}

FunctionResult return_from_function(FunctionContext &context)
{
    auto &registers = context.registers;
    const auto target = read_long(*context.host, registers.address[7]);
    registers.address[7] += 4U;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0000b6e0(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    if (registers.program_counter != 0x0000b6e0U)
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};

    registers.address[0] = 0x00001600U;
    const auto initial_count = read_word(host, 0x00024836U);
    auto clear_counter = static_cast<std::uint16_t>(initial_count - 5U);
    registers.data[0] =
        (registers.data[0] & 0xffff0000U) | clear_counter;
    subtract_word(registers, 5U, clear_counter);

    do {
        (void)read_word(host, registers.address[0]);
        write_word(host, registers.address[0], 0U);
        set_nzvc_word(registers, 0U);
        registers.address[0] += 0x80U;
        clear_counter = static_cast<std::uint16_t>(clear_counter - 1U);
        registers.data[0] =
            (registers.data[0] & 0xffff0000U) | clear_counter;
    } while (clear_counter != 0xffffU);

    registers.address[6] = 0x00001600U;
    auto descriptor_index = read_word(host, registers.address[5] + 0x0cU);
    registers.data[0] =
        (registers.data[0] & 0xffff0000U) | descriptor_index;
    set_nzvc_word(registers, descriptor_index);
    descriptor_index = arithmetic_shift_left_two(registers, descriptor_index);
    registers.data[0] =
        (registers.data[0] & 0xffff0000U) | descriptor_index;

    registers.address[0] = 0x0000b93aU;
    const auto table_offset = static_cast<std::int16_t>(descriptor_index);
    registers.address[0] = read_long(host,
        static_cast<std::uint32_t>(registers.address[0] + table_offset));

    auto descriptor_count = read_word(host, registers.address[0]);
    registers.address[0] += 2U;
    registers.data[0] =
        (registers.data[0] & 0xffff0000U) | descriptor_count;
    set_nzvc_word(registers, descriptor_count);
    if ((descriptor_count & 0x8000U) != 0U)
        return return_from_function(context);

    do {
        const auto child = call_function_631(context);
        if (child.status != TranslationStatus::complete)
            return child;
        registers.address[6] += 0x80U;
        descriptor_count = static_cast<std::uint16_t>(descriptor_count - 1U);
        registers.data[0] =
            (registers.data[0] & 0xffff0000U) | descriptor_count;
    } while (descriptor_count != 0xffffU);

    return return_from_function(context);
}

} // namespace gain_ground::translated
