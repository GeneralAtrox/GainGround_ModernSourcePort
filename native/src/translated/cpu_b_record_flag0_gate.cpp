#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto shift = odd ? 0U : 8U;
    return static_cast<std::uint8_t>(
        host.read_memory_word(kRegion, address & ~1U, mask) >> shift);
}

void set_bit_zero(CpuRegisters &registers, bool bit_was_set)
{
    if (bit_was_set)
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);
    else
        registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
}

FunctionResult transfer(FunctionContext &context, std::uint32_t site,
    std::uint32_t target, std::uint32_t function_id)
{
    context.registers.program_counter = target;
    return context.host->call_function(function_id, 1U, 0x72U, 1U,
        site, target, context);
}
} // namespace

FunctionResult cpu_b_record_flag0_gate(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x0001e180U)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    const auto flags = read_byte(*context.host, context.registers.address[5] + 0x40U);
    const bool bit_set = (flags & 0x01U) != 0U;
    set_bit_zero(context.registers, bit_set);
    if (bit_set)
        return transfer(context, 0x0001e186U, 0x0001e22aU, 555U);
    return transfer(context, 0x0001e186U, 0x0001e18aU, 553U);
}

} // namespace gain_ground::translated
