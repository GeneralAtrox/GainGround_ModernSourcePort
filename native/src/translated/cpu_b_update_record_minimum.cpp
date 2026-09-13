#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
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

FunctionResult cpu_b_update_record_minimum(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    push_return(host, registers, 0x00023830U);
    registers.program_counter = 0x00023848U;
    const auto child = host.call_function(415U, 1U, 0x72U, 2U,
        0x0002382cU, 0x00023848U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    const auto minimum_class = static_cast<std::uint16_t>(registers.data[1]);
    host.write_memory_word(kRegion, registers.address[5] + 0x78U,
        minimum_class, kWordMask);
    set_move_word_flags(registers, minimum_class);

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
