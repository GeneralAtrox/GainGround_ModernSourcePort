#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kMainRamRegion = 3U;
constexpr std::uint32_t kMainRamMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, 0xffffU);
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_logic_long(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto offset = registers.address[7] & kMainRamMask;
    const auto high = host.read_memory_word(
        kMainRamRegion, offset, 0xffffU);
    const auto low = host.read_memory_word(
        kMainRamRegion, (offset + 2U) & kMainRamMask, 0xffffU);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult proven_static_cpu_a_plain_000012b6(FunctionContext &context) noexcept
{
    if (context.host == nullptr
            || context.registers.program_counter != 0x000012b6U)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.address[0] = 0x00b80000U;
    prefetch(host, 0x000012baU);
    registers.address[1] = 0x00b82000U;
    prefetch(host, 0x000012bcU);
    prefetch(host, 0x000012beU);
    prefetch(host, 0x000012c0U);

    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x07ffU;
    set_logic_word(registers, 0x07ffU);
    prefetch(host, 0x000012c2U);
    prefetch(host, 0x000012c4U);

    std::uint16_t counter = 0x07ffU;
    for (;;) {
        prefetch(host, 0x000012c6U);
        prefetch(host, 0x000012c8U);

        const auto source = registers.address[0];
        const auto destination = registers.address[2];
        const auto high = host.read_hardware(
            1U, 0U, 0xffU, 0x000012c6U, source, 0xffffU);
        const auto low = host.read_hardware(
            1U, 0U, 0xffU, 0x000012c6U, source + 2U, 0xffffU);
        host.write_hardware(
            2U, 0U, 0xffU, 0x000012c6U, destination, high, 0xffffU);
        host.write_hardware(
            2U, 0U, 0xffU, 0x000012c6U, destination + 2U, low, 0xffffU);
        registers.address[0] += 4U;
        registers.address[2] += 4U;
        set_logic_long(registers,
            (static_cast<std::uint32_t>(high) << 16U) | low);

        prefetch(host, 0x000012caU);
        counter = static_cast<std::uint16_t>(counter - 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }

    prefetch(host, 0x000012c6U);
    prefetch(host, 0x000012ccU);
    prefetch(host, 0x000012ceU);
    const auto return_address = pop_return(host, registers);
    prefetch(host, return_address);
    prefetch(host, return_address + 2U);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
