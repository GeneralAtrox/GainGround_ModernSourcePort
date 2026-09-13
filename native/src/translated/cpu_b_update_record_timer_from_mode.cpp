#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void compare_word(CpuRegisters &registers, std::uint16_t lhs, std::uint16_t rhs)
{
    const auto result = static_cast<std::uint16_t>(lhs - rhs);
    const bool overflow = (((lhs ^ rhs) & (lhs ^ result)) & 0x8000U) != 0U;
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    if (lhs < rhs) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void add_word(CpuRegisters &registers, std::uint16_t lhs, std::uint16_t rhs,
              std::uint16_t result)
{
    const auto wide = static_cast<std::uint32_t>(lhs) + rhs;
    const bool overflow = ((~(lhs ^ rhs) & (lhs ^ result)) & 0x8000U) != 0U;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    if (wide > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(kPrivateRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_update_record_timer_from_mode(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    auto timer = host.read_memory_word(
        kPrivateRegion, registers.address[0] + 0x10U, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | timer;
    set_logic_word(registers, timer);

    const auto global_word = host.read_memory_word(kPrivateRegion, 0x00000402U, 0x00ffU);
    const auto global_byte = static_cast<std::uint8_t>(global_word);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | global_byte;
    set_logic_byte(registers, global_byte);

    const auto mode = static_cast<std::uint8_t>(global_byte & 0x1cU);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | mode;
    set_logic_byte(registers, mode);
    compare_word(registers, static_cast<std::uint16_t>(registers.data[1]), 0x001cU);

    if (static_cast<std::uint16_t>(registers.data[1]) == 0x001cU) {
        registers.data[1] = (registers.data[1] & 0xffff0000U) | timer;
        set_logic_word(registers, timer);
        const bool carry = (timer & 0x0002U) != 0U;
        const auto quarter = static_cast<std::uint16_t>(timer >> 2U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | quarter;
        std::uint16_t shift_flags{};
        if (quarter == 0U) shift_flags |= 0x0004U;
        if (carry) shift_flags |= 0x0011U;
        registers.status = static_cast<std::uint16_t>(
            (registers.status & ~0x001fU) | shift_flags);
        const auto adjusted = static_cast<std::uint16_t>(timer + quarter);
        add_word(registers, timer, quarter, adjusted);
        timer = adjusted;
        registers.data[0] = (registers.data[0] & 0xffff0000U) | timer;
    }

    host.write_memory_word(
        kPrivateRegion, registers.address[5] + 0x74U, timer, kWordMask);
    set_logic_word(registers, timer);
    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
