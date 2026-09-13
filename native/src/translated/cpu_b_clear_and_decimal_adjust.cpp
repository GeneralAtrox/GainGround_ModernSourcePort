#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_flags(CpuRegisters &registers, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(kRegion, address & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? word : (word >> 8U));
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    host.write_memory_word(kRegion, address & ~1U,
        odd ? value : static_cast<std::uint16_t>(value << 8U),
        odd ? 0x00ffU : 0xff00U);
}

void clear_long(ExecutionHost &host, CpuRegisters &registers, std::uint32_t address)
{
    (void)host.read_memory_word(kRegion, address, kWordMask);
    (void)host.read_memory_word(kRegion, address + 2U, kWordMask);
    host.write_memory_word(kRegion, address + 2U, 0U, kWordMask);
    host.write_memory_word(kRegion, address, 0U, kWordMask);
    set_logic_flags(registers, 0U, 0x80000000U);
}

void add_d0_word(CpuRegisters &registers)
{
    const auto left = static_cast<std::uint16_t>(registers.data[0]);
    const auto wide = static_cast<std::uint32_t>(left) * 2U;
    const auto result = static_cast<std::uint16_t>(wide);
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ left)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (wide > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
}

void abcd_predecrement(ExecutionHost &host, CpuRegisters &registers)
{
    --registers.address[1];
    const auto source = read_byte(host, registers.address[1]);
    --registers.address[0];
    const auto destination = read_byte(host, registers.address[0]);
    const auto extend = static_cast<std::uint16_t>((registers.status & 0x0010U) != 0U);
    const auto binary = static_cast<std::uint16_t>(source + destination + extend);
    auto adjusted = binary;
    if (static_cast<std::uint16_t>((source & 0x0fU) + (destination & 0x0fU) + extend) > 9U)
        adjusted = static_cast<std::uint16_t>(adjusted + 6U);
    const bool carry = adjusted > 0x99U;
    if (carry) adjusted = static_cast<std::uint16_t>(adjusted + 0x60U);
    const auto result = static_cast<std::uint8_t>(adjusted);
    write_byte(host, registers.address[0], result);
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if ((registers.status & 0x0004U) != 0U && result == 0U) flags |= 0x0004U;
    if (((~binary) & adjusted & 0x80U) != 0U) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void addq_four_address_word(CpuRegisters &registers)
{
    const auto left = static_cast<std::uint16_t>(registers.address[0]);
    const auto wide = static_cast<std::uint32_t>(left) + 4U;
    const auto result = static_cast<std::uint16_t>(wide);
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (left <= 0x7fffU && result > 0x7fffU) flags |= 0x0002U;
    if (wide > 0xffffU) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x000fU) | flags);
    registers.address[0] = (registers.address[0] & 0xffff0000U) | result;
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_clear_and_decimal_adjust(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    registers.data[1] = 0x0000000fU;
    set_logic_flags(registers, registers.data[1], 0x80000000U);
    registers.address[0] = 0x00000826U;
    clear_long(host, registers, registers.address[0]);
    registers.address[0] += 4U;
    for (;;) {
        registers.address[1] = registers.address[0];
        add_d0_word(registers);
        for (std::uint32_t index = 0; index != 4U; ++index)
            abcd_predecrement(host, registers);
        addq_four_address_word(registers);
        const auto counter = static_cast<std::uint16_t>(registers.data[1] - 1U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }
    registers.address[0] -= 4U;
    registers.data[0] = read_long(host, registers.address[0]);
    set_logic_flags(registers, registers.data[0], 0x80000000U);
    const auto return_address = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
