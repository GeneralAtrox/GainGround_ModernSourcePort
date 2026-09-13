#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

void set_neg_word(CpuRegisters &registers,
    std::uint16_t source, std::uint16_t result)
{
    std::uint16_t flags = 0U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (source == 0x8000U) flags |= 0x0002U;
    if (source != 0U) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion,
        registers.address[7] & kAddressMask,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion,
        (registers.address[7] + 2U) & kAddressMask,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(kPrivateRegion,
        registers.address[7] & kAddressMask, kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion,
        (registers.address[7] + 2U) & kAddressMask, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001639a(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    const bool bit_set = (registers.data[7] & 0x00000200U) != 0U;
    registers.status = static_cast<std::uint16_t>(
        bit_set ? (registers.status & ~0x0004U)
                : (registers.status | 0x0004U));
    if (bit_set) {
        registers.program_counter = 0x000163aaU;
        return host.call_function(591U, 1U, 0x72U, 1U,
            0x0001639eU, 0x000163aaU, context);
    }

    push_return(host, registers, 0x000163a4U);
    registers.program_counter = 0x000162f2U;
    const auto child = host.call_function(302U, 1U, 0x72U, 2U,
        0x000163a0U, 0x000162f2U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    auto source = static_cast<std::uint16_t>(registers.data[0]);
    auto result = static_cast<std::uint16_t>(0U - source);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
    set_neg_word(registers, source, result);

    source = static_cast<std::uint16_t>(registers.data[1]);
    result = static_cast<std::uint16_t>(0U - source);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | result;
    set_neg_word(registers, source, result);

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
