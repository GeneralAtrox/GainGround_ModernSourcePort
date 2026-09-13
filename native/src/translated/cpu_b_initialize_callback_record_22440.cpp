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

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

void set_logic_long_flags(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], value);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto value = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return value;
}
} // namespace

FunctionResult cpu_b_initialize_callback_record_22440(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    push_return(host, registers, 0x000223d8U);
    registers.program_counter = 0x000238c8U;
    const auto clear = host.call_function(416U, 1U, 0x72U, 2U,
        0x000223d4U, 0x000238c8U, context);
    if (clear.status != TranslationStatus::complete || clear.control != 1U)
        return clear;

    write_long(host, registers.address[5] + 2U, 0x00022440U);
    set_logic_long_flags(registers, 0x00022440U);
    registers.address[6] = registers.address[5] + 0x22U;
    registers.address[0] = 0x00023c38U;
    registers.data[0] = 7U;
    set_logic_long_flags(registers, 7U);

    for (;;) {
        const auto value = read_long(host, registers.address[0]);
        registers.address[0] += 4U;
        write_long(host, registers.address[6], value);
        registers.address[6] += 4U;
        set_logic_long_flags(registers, value);

        const auto counter = static_cast<std::uint16_t>(registers.data[0]);
        registers.data[0] = (registers.data[0] & 0xffff0000U)
            | static_cast<std::uint16_t>(counter - 1U);
        if (counter == 0U) break;
    }

    push_return(host, registers, 0x000223f6U);
    registers.program_counter = 0x0002382cU;
    const auto minimum = host.call_function(413U, 1U, 0x72U, 2U,
        0x000223f2U, 0x0002382cU, context);
    if (minimum.status != TranslationStatus::complete
        || minimum.control != 1U)
        return minimum;

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated

