#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kMainRamRegion = 3U;
constexpr std::uint16_t kWindowRegion = 7U;
constexpr std::uint16_t kExtend = 0x0010U;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, 0xffffU);
}

void set_logic_long(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
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

FunctionResult cpu_a_clear_window_mask_ram(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.data[1] = 0U;
    registers.address[0] = 0x0020c000U;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x07ffU;

    prefetch(host, 0x00000bd8U);
    prefetch(host, 0x00000bdaU);
    prefetch(host, 0x00000bdcU);
    prefetch(host, 0x00000bdeU);

    std::uint16_t counter = 0x07ffU;
    for (;;) {
        prefetch(host, 0x00000be0U);
        prefetch(host, 0x00000be2U);
        const auto offset = registers.address[0] - 0x0020c000U;
        host.write_memory_word(kWindowRegion, offset, 0U, 0xffffU);
        host.write_memory_word(kWindowRegion, offset + 2U, 0U, 0xffffU);
        prefetch(host, 0x00000be4U);
        registers.address[0] += 4U;
        set_logic_long(registers, 0U);
        counter = static_cast<std::uint16_t>(counter - 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }

    prefetch(host, 0x00000be0U);
    prefetch(host, 0x00000be6U);
    prefetch(host, 0x00000be8U);
    const auto return_address = pop_return(host, registers);
    prefetch(host, return_address);
    prefetch(host, return_address + 2U);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
