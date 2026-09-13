#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kMainRamRegion = 3U;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, 0xffffU);
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_logic_long(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_add_word(CpuRegisters &registers, std::uint16_t lhs, std::uint16_t rhs,
                  std::uint16_t result)
{
    const auto wide = static_cast<std::uint32_t>(lhs) + rhs;
    const bool overflow = ((~(lhs ^ rhs) & (lhs ^ result)) & 0x8000U) != 0U;
    std::uint16_t flags = 0U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    if (wide > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack = registers.address[7] & 0x0003ffffU;
    const auto high = host.read_memory_word(kMainRamRegion, stack, 0xffffU);
    const auto low = host.read_memory_word(kMainRamRegion, (stack + 2U) & 0x0003ffffU, 0xffffU);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_a_fdc_offset_to_track(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.data[6] = registers.data[0];

    prefetch(host, 0x0000179aU);
    const auto dividend = registers.data[6];
    const auto quotient = dividend / 0x2d00U;
    if (quotient <= 0xffffU) {
        const auto remainder = dividend % 0x2d00U;
        registers.data[6] = (remainder << 16U) | quotient;
        set_logic_word(registers, static_cast<std::uint16_t>(quotient));
    } else {
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x000eU) | 0x0002U);
    }

    registers.data[7] = (registers.data[7] & 0xffff0000U) |
                        static_cast<std::uint16_t>(registers.data[6]);
    set_logic_word(registers, static_cast<std::uint16_t>(registers.data[7]));
    prefetch(host, 0x0000179cU);

    const auto old_d7 = static_cast<std::uint16_t>(registers.data[7]);
    const auto next_d7 = static_cast<std::uint16_t>(old_d7 + 2U);
    registers.data[7] = (registers.data[7] & 0xffff0000U) | next_d7;
    set_add_word(registers, old_d7, 2U, next_d7);
    prefetch(host, 0x0000179eU);

    registers.data[6] = (registers.data[6] << 16U) | (registers.data[6] >> 16U);
    set_logic_long(registers, registers.data[6]);
    prefetch(host, 0x000017a0U);
    prefetch(host, 0x000017a2U);
    prefetch(host, 0x000017a4U);

    const auto return_address = pop_return(host, registers);
    prefetch(host, return_address);
    prefetch(host, return_address + 2U);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
