#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kMainRamRegion = 3U;
constexpr std::uint16_t kTilemapARegion = 5U;
constexpr std::uint16_t kExtend = 0x0010U;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, 0xffffU);
}

void set_logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_logic_long(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & kExtend;
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

FunctionResult cpu_a_fill_strided_video_memory(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.data[7] = (registers.data[7] & 0xffffff00U) | 0x80U;
    set_logic_byte(registers, 0x80U);
    prefetch(host, 0x00003872U);

    registers.data[4] = 0x17U;
    set_logic_long(registers, registers.data[4]);
    prefetch(host, 0x00003874U);

    std::uint16_t outer = 0x17U;
    for (;;) {
        registers.address[0] = registers.address[3];
        prefetch(host, 0x00003876U);
        registers.data[0] = 7U;
        set_logic_long(registers, registers.data[0]);

        std::uint16_t inner = 7U;
        for (;;) {
            prefetch(host, 0x00003878U);
            prefetch(host, 0x0000387aU);
            host.write_memory_word(kTilemapARegion, registers.address[0] & 0x0003ffffU,
                                   static_cast<std::uint16_t>(registers.data[7]), 0xffffU);
            registers.address[0] = static_cast<std::uint32_t>(
                registers.address[0] + static_cast<std::int16_t>(registers.data[6]));
            prefetch(host, 0x0000387cU);
            prefetch(host, 0x0000387eU);
            inner = static_cast<std::uint16_t>(inner - 1U);
            registers.data[0] = (registers.data[0] & 0xffff0000U) | inner;
            if (inner == 0xffffU) break;
        }

        prefetch(host, 0x00003878U);
        const auto old_d7 = static_cast<std::uint16_t>(registers.data[7]);
        const auto next_d7 = static_cast<std::uint16_t>(old_d7 + 1U);
        registers.data[7] = (registers.data[7] & 0xffff0000U) | next_d7;
        set_add_word(registers, old_d7, 1U, next_d7);
        prefetch(host, 0x00003880U);
        registers.address[3] += 2U;
        prefetch(host, 0x00003882U);
        prefetch(host, 0x00003884U);
        prefetch(host, 0x00003886U);
        outer = static_cast<std::uint16_t>(outer - 1U);
        registers.data[4] = (registers.data[4] & 0xffff0000U) | outer;
        if (outer == 0xffffU) break;
        prefetch(host, 0x00003874U);
    }

    prefetch(host, 0x00003874U);
    const auto old_d7 = static_cast<std::uint16_t>(registers.data[7]);
    const auto final_d7 = static_cast<std::uint16_t>(old_d7 + 0x1000U);
    registers.data[7] = (registers.data[7] & 0xffff0000U) | final_d7;
    set_add_word(registers, old_d7, 0x1000U, final_d7);
    prefetch(host, 0x00003888U);
    prefetch(host, 0x0000388aU);
    prefetch(host, 0x0000388cU);
    prefetch(host, 0x0000388eU);
    const auto return_address = pop_return(host, registers);
    prefetch(host, return_address);
    prefetch(host, return_address + 2U);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
