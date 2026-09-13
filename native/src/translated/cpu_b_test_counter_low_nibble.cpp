#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address, kMask);
}

void set_word(CpuRegisters &registers, std::uint16_t value)
{
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
}

void set_logic_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

FunctionResult return_from_function(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = read_word(host, registers.address[7]);
    const auto low = read_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_test_counter_low_nibble(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    auto d0 = read_word(host, registers.address[5] + 8U);
    set_word(registers, d0);
    set_logic_flags(registers, d0);
    d0 = static_cast<std::uint16_t>(d0 & 0x000fU);
    set_word(registers, d0);
    set_logic_flags(registers, d0);
    if (d0 != 0U)
        return return_from_function(host, registers);

    registers.program_counter = 0x00023936U;
    return host.call_function(420U, 1U, 0x72U, 1U,
        0x00023932U, 0x00023936U, context);
}

} // namespace gain_ground::translated
