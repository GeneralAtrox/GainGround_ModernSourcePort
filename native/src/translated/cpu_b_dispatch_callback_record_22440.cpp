#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_asl_word_two_flags(CpuRegisters &registers, std::uint16_t original,
                            std::uint16_t result)
{
    const bool carry = (original & 0x4000U) != 0U;
    const bool overflow = ((original ^ (original << 1U)) & 0x8000U) != 0U
        || (((original << 1U) ^ (original << 2U)) & 0x8000U) != 0U;
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
                 std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(
        kRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_dispatch_callback_record_22440(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    push_return(host, registers, 0x00022444U);
    registers.program_counter = 0x000237e2U;
    const auto snapshot = host.call_function(412U, 1U, 0x72U, 2U,
        0x00022440U, 0x000237e2U, context);
    if (snapshot.status != TranslationStatus::complete
        || snapshot.control != 1U)
        return snapshot;

    const auto selector = host.read_memory_word(
        kRegion, registers.address[5] + 0x1cU, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | selector;
    set_logic_word_flags(registers, selector);

    const auto table_offset = static_cast<std::uint16_t>(selector << 2U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | table_offset;
    set_asl_word_two_flags(registers, selector, table_offset);
    if (table_offset != 0U)
        return {TranslationStatus::contract_violation, 0U, 0x0002244aU};

    registers.address[2] = 0x00023c68U;
    push_return(host, registers, 0x0002245eU);
    registers.program_counter = 0x0002392aU;
    const auto test = host.call_function(419U, 1U, 0x72U, 2U,
        0x0002245aU, 0x0002392aU, context);
    if (test.status != TranslationStatus::complete || test.control != 1U)
        return test;

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
