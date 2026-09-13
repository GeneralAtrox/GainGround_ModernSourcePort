#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWindowRegion = 7U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kHighByteMask = 0xff00U;

void set_move_flags(CpuRegisters &registers, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack = registers.address[7];
    const auto high = host.read_memory_word(kPrivateRegion, stack, kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion, stack + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_set_window_mask_entry_bits(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    registers.address[0] = 0x0020c1c6U;
    registers.address[1] = 0x0000a0eeU;
    registers.data[1] = 4U;
    set_move_flags(registers, registers.data[1], 0x80000000U);
    for (;;) {
        const auto count = host.read_memory_word(
            kPrivateRegion, registers.address[1], kWordMask);
        registers.address[1] += 2U;
        registers.data[0] = (registers.data[0] & 0xffff0000U) | count;
        set_move_flags(registers, count, 0x8000U);
        for (;;) {
            const auto offset = registers.address[0] & 0x00003fffU;
            const auto original = host.read_memory_word(kWindowRegion, offset, kHighByteMask);
            host.write_memory_word(kWindowRegion, offset,
                static_cast<std::uint16_t>(original | 0x0800U), kHighByteMask);
            if ((original & 0x0800U) == 0U)
                registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
            else
                registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);
            registers.address[0] = (registers.address[0] & 0xffff0000U)
                | static_cast<std::uint16_t>(registers.address[0] + 8U);
            const auto d0_counter = static_cast<std::uint16_t>(registers.data[0] - 1U);
            registers.data[0] = (registers.data[0] & 0xffff0000U) | d0_counter;
            if (d0_counter == 0xffffU) break;
        }
        const auto displacement = host.read_memory_word(
            kPrivateRegion, registers.address[1], kWordMask);
        registers.address[1] += 2U;
        registers.address[0] += static_cast<std::int16_t>(displacement);
        const auto d1_counter = static_cast<std::uint16_t>(registers.data[1] - 1U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | d1_counter;
        if (d1_counter == 0xffffU) break;
    }

    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
