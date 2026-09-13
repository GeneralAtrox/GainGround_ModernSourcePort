#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & 0x00ffffffU, kWordMask);
}

void set_word(CpuRegisters &registers, std::uint16_t value)
{
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
}

void set_logic_flags(CpuRegisters &registers,
    std::uint32_t value, std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void compare_word(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    std::uint16_t flags = registers.status & 0x0010U;
    if (destination < source) flags |= 0x0001U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = read_word(host, registers.address[7]);
    const auto low = read_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_clamp_and_publish_mode_value(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;
    if (registers.program_counter != 0x0000dcdeU)
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};

    auto value = static_cast<std::uint16_t>(registers.data[0]);
    compare_word(registers, value, 0x0063U);
    if ((registers.status & 0x0001U) == 0U) {
        registers.data[0] = 0U;
        value = 0U;
        set_logic_flags(registers, 0U, 0x80000000U, 0xffffffffU);
    } else {
        compare_word(registers, value, 0x0059U);
        const bool less_or_equal = (registers.status & 0x0004U) != 0U
            || (((registers.status & 0x0008U) != 0U)
                != ((registers.status & 0x0002U) != 0U));
        if (!less_or_equal) {
            value = 0x0059U;
            set_word(registers, value);
            set_logic_flags(registers, value, 0x8000U, 0xffffU);
        }
    }

    host.write_memory_word(kRegion, 0x00000838U, value, kWordMask);
    set_logic_flags(registers, value, 0x8000U, 0xffffU);
    host.write_hardware(2U, 1U, 0x72U, 0x0000dcf6U,
        0x00d00034U, 0x1f1fU, 0x00ffU);
    set_logic_flags(registers, 0x1fU, 0x80U, 0xffU);

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
