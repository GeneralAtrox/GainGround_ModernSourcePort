#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kMainRamRegion = 3U;
constexpr std::uint16_t kTilemapARegion = 5U;

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

FunctionResult cpu_a_copy_tilemap_with_index_bias(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.data[2] = (registers.data[2] & 0xffff0000U) |
                        static_cast<std::uint16_t>(registers.data[7]);
    set_logic_word(registers, static_cast<std::uint16_t>(registers.data[2]));

    std::uint16_t counter = static_cast<std::uint16_t>(registers.data[2]);
    for (;;) {
        prefetch(host, 0x0000075cU);
        auto value = host.read_memory_word(kProgramRegion, registers.address[0], 0xffffU);
        registers.address[0] += 2U;
        set_logic_word(registers, value);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | value;

        prefetch(host, 0x0000075eU);
        const auto incremented = static_cast<std::uint16_t>(value + 1U);
        set_add_word(registers, value, 1U, incremented);
        value = incremented;
        registers.data[0] = (registers.data[0] & 0xffff0000U) | value;

        prefetch(host, 0x00000760U);
        const auto bias = static_cast<std::uint16_t>(registers.data[4]);
        const auto biased = static_cast<std::uint16_t>(value + bias);
        set_add_word(registers, value, bias, biased);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | biased;

        prefetch(host, 0x00000762U);
        host.write_memory_word(kTilemapARegion, registers.address[1] & 0x0003ffffU,
                               biased, 0xffffU);
        registers.address[1] += 2U;
        prefetch(host, 0x00000764U);

        counter = static_cast<std::uint16_t>(counter - 1U);
        registers.data[2] = (registers.data[2] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
        prefetch(host, 0x0000075aU);
    }

    prefetch(host, 0x0000075aU);
    registers.address[1] = static_cast<std::uint32_t>(
        registers.address[1] + static_cast<std::int16_t>(registers.data[5]));
    prefetch(host, 0x00000766U);
    prefetch(host, 0x00000768U);
    prefetch(host, 0x0000076aU);

    const auto outer = static_cast<std::uint16_t>(registers.data[6] - 1U);
    registers.data[6] = (registers.data[6] & 0xffff0000U) | outer;
    prefetch(host, 0x00000758U);
    if (outer != 0xffffU) {
        prefetch(host, 0x0000075aU);
        registers.program_counter = 0x00000758U;
        const auto loop = host.call_function(
            3U, 0U, 0xffU, 1U, 0x00000768U, 0x00000758U, context);
        if (loop.status == TranslationStatus::complete && loop.control == 3U)
            return FunctionResult::complete(4U, 0x00000758U);
        return loop;
    }

    prefetch(host, 0x0000076cU);
    prefetch(host, 0x0000076eU);
    const auto return_address = pop_return(host, registers);
    prefetch(host, return_address);
    prefetch(host, return_address + 2U);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
