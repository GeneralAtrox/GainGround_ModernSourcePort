#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
void set_clear_word_flags(CpuRegisters &registers)
{
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | (registers.status & 0x0010U) | 0x0004U);
}

void compare_long(CpuRegisters &registers, std::uint32_t source)
{
    const auto destination = registers.data[0];
    const auto result = destination - source;
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result) & 0x80000000U) != 0U)
        flags |= 0x0002U;
    if (destination < source) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_b_reset_state00_entry(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[7] = 0x00007ffeU;

    (void)host.read_hardware(1U, 1U, 0x00U, 0x0000843aU,
                             0x00a00004U, 0xffffU);
    host.write_hardware(2U, 1U, 0x00U, 0x0000843aU,
                        0x00a00004U, 0U, 0xffffU);
    set_clear_word_flags(registers);

    (void)host.read_hardware(1U, 1U, 0x00U, 0x00008440U,
                             0x00a00000U, 0xffffU);
    host.write_hardware(2U, 1U, 0x00U, 0x00008440U,
                        0x00a00000U, 0U, 0xffffU);
    set_clear_word_flags(registers);

    registers.address[0] = 0x00b00009U;
    host.write_hardware(2U, 1U, 0x00U, 0x0000844cU,
                        0x00b00008U, 0x0a0aU, 0x00ffU);
    compare_long(registers, 0x0091ffffU);

    registers.program_counter = 0x00008456U;
    (void)host.call_function(422U, 1U, 0x91U, 6U,
                             0x00008450U, 0x00008456U, context);
    return FunctionResult::complete(5U, 0x00008456U);
}

} // namespace gain_ground::translated
