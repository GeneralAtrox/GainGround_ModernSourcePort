#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(host.read_memory_word(
                kPrivateRegion, address, kWordMask)) << 16U)
        | host.read_memory_word(kPrivateRegion, address + 2U, kWordMask);
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kPrivateRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

void set_logic_flags(CpuRegisters &registers,
    std::uint32_t value, std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_sub_word_flags(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (destination < source) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], value);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return target;
}
} // namespace

FunctionResult cpu_b_reload_periodic_counter(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.address[0] = 0x00000d00U;
    const auto counter = host.read_memory_word(
        kPrivateRegion, registers.address[0], kWordMask);
    const auto decremented = static_cast<std::uint16_t>(counter - 1U);
    host.write_memory_word(kPrivateRegion, registers.address[0],
        decremented, kWordMask);
    set_sub_word_flags(registers, counter, 1U, decremented);

    const bool greater_than_zero = (registers.status & 0x0004U) == 0U
        && (((registers.status & 0x0008U) != 0U)
            == ((registers.status & 0x0002U) != 0U));
    if (!greater_than_zero) {
        host.write_memory_word(kPrivateRegion, registers.address[0],
            0x000fU, kWordMask);
        set_logic_flags(registers, 0x000fU, 0x8000U, 0xffffU);
        registers.address[0] += 2U;

        registers.data[1] = read_long(host, registers.address[0]);
        set_logic_flags(registers, registers.data[1],
            0x80000000U, 0xffffffffU);
        registers.data[0] = 0x00000010U;
        set_logic_flags(registers, registers.data[0],
            0x80000000U, 0xffffffffU);

        push_return(host, registers, 0x0000de62U);
        registers.program_counter = 0x00015ea0U;
        const auto child = host.call_function(285U, 1U, 0x72U, 2U,
            0x0000de5cU, 0x00015ea0U, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;

        write_long(host, 0x00000d02U, registers.data[0]);
        set_logic_flags(registers, registers.data[0],
            0x80000000U, 0xffffffffU);
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
