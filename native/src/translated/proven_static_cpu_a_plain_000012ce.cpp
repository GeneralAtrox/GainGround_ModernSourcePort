#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

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

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto offset = registers.address[7] & kSharedMask;
    const auto high = host.read_memory_word(kSharedRegion, offset, 0xffffU);
    const auto low = host.read_memory_word(
        kSharedRegion, (offset + 2U) & kSharedMask, 0xffffU);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult proven_static_cpu_a_plain_000012ce(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x000012ceU)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.address[0] = 0x00b80000U;
    prefetch(host, 0x000012d2U);
    registers.address[2] = 0x00b82000U;
    prefetch(host, 0x000012d4U);
    prefetch(host, 0x000012d6U);
    prefetch(host, 0x000012d8U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x0fffU;
    set_logic_word(registers, 0x0fffU);
    prefetch(host, 0x000012daU);
    prefetch(host, 0x000012dcU);

    for (;;) {
        prefetch(host, 0x000012deU);
        prefetch(host, 0x000012e0U);
        const auto left = host.read_hardware(
            1U, 0U, 0xffU, 0x000012deU, registers.address[0], 0xffffU);
        registers.address[0] += 2U;
        registers.data[1] = (registers.data[1] & 0xffff0000U) | left;
        set_logic_word(registers, left);

        prefetch(host, 0x000012e2U);
        const auto right = host.read_hardware(
            1U, 0U, 0xffU, 0x000012e0U, registers.address[2], 0xffffU);
        registers.address[2] += 2U;
        registers.data[2] = (registers.data[2] & 0xffff0000U) | right;
        set_logic_word(registers, right);

        prefetch(host, 0x000012e4U);
        const auto difference = static_cast<std::uint16_t>(left ^ right);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | difference;
        set_logic_word(registers, difference);

        prefetch(host, 0x000012e6U);
        if (difference != 0U) break;
        const auto counter = static_cast<std::uint16_t>(registers.data[0] - 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }

    prefetch(host, 0x000012deU);
    prefetch(host, 0x000012e8U);
    prefetch(host, 0x000012eaU);
    if ((registers.status & 0x0004U) == 0U) {
        prefetch(host, 0x000012ecU);
        registers.status = static_cast<std::uint16_t>(
            (registers.status & ~0x001fU) | 0x0001U);
    }

    prefetch(host, 0x000012eeU);
    prefetch(host, 0x000012f0U);
    const auto return_address = pop_return(host, registers);
    prefetch(host, return_address);
    if (return_address == 0x00001120U) {
        registers.status = host.apply_controlled_status(
            0U, 0xffU, return_address, 0x00b83ffeU, registers.status);
    }
    prefetch(host, return_address + 2U);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
