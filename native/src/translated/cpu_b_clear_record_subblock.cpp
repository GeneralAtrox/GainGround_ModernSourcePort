#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_flags(CpuRegisters &registers, std::uint32_t value, std::uint32_t sign_mask)
{
    const auto extend = static_cast<std::uint16_t>(registers.status & 0x0010U);
    std::uint16_t flags = extend;
    if (value == 0U) flags |= 0x0004U;
    if ((value & sign_mask) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(kRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(kRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_clear_record_subblock(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.data[1] = 0U;
    set_logic_flags(registers, 0U, 0x80000000U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x001cU;
    set_logic_flags(registers, 0x001cU, 0x8000U);
    registers.address[1] = registers.address[6] + 0x0cU;

    for (std::uint32_t count = 0; count != 29U; ++count) {
        host.write_memory_word(kRegion, registers.address[1], 0U, kWordMask);
        host.write_memory_word(kRegion, registers.address[1] + 2U, 0U, kWordMask);
        registers.address[1] += 4U;
        set_logic_flags(registers, 0U, 0x80000000U);
        const auto counter = static_cast<std::uint16_t>(registers.data[0] - 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
