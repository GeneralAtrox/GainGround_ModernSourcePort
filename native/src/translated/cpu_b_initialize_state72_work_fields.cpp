#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_move_flags(CpuRegisters &registers, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto mask = static_cast<std::uint16_t>((address & 1U) ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(kRegion, address & ~1U, mask);
    return static_cast<std::uint8_t>((address & 1U) ? word : (word >> 8U));
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto odd = (address & 1U) != 0U;
    host.write_memory_word(kRegion, address & ~1U,
        odd ? value : static_cast<std::uint16_t>(value << 8U),
        odd ? 0x00ffU : 0xff00U);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack = registers.address[7];
    const auto high = host.read_memory_word(kRegion, stack, kWordMask);
    const auto low = host.read_memory_word(kRegion, stack + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_initialize_state72_work_fields(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    const auto entry_pc = registers.program_counter;
    if (entry_pc != 0x00009c2cU) {
        host.write_memory_word(kRegion, 0x00000834U, 1U, kWordMask);
        set_move_flags(registers, 1U, 0x8000U);
        host.write_memory_word(kRegion, 0x00000c16U, 0U, kWordMask);
        set_move_flags(registers, 0U, 0x8000U);
        (void)host.read_memory_word(kRegion, 0x00000410U, 0x00ffU);
        host.write_memory_word(kRegion, 0x00000410U, 0x00ffU, 0x00ffU);
        registers.address[1] = 0x00024836U;

        registers.address[7] -= 4U;
        host.write_memory_word(kRegion, registers.address[7], 0U, kWordMask);
        host.write_memory_word(kRegion, registers.address[7] + 2U, 0x9c2cU, kWordMask);
        registers.program_counter = 0x00008872U;
        const auto child = host.call_function(126U, 1U, 0x72U, 2U,
            0x00009c26U, 0x00008872U, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }

    registers.address[1] = 0x00009c76U;
    const auto index = host.read_memory_word(kRegion, 0x00000c02U, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | index;
    set_move_flags(registers, index, 0x8000U);
    const auto shifted = static_cast<std::uint16_t>(index << 2U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | shifted;
    const bool carry = (index & 0x4000U) != 0U;
    const auto once = static_cast<std::uint16_t>(index << 1U);
    const bool overflow = (((index ^ once) | (once ^ shifted)) & 0x8000U) != 0U;
    std::uint16_t flags = registers.status & 0x0010U;
    if ((shifted & 0x8000U) != 0U) flags |= 0x0008U;
    if (shifted == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    if (carry) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
    registers.address[1] += static_cast<std::int16_t>(shifted);
    registers.address[0] = 0x00001480U;
    for (std::uint32_t index_byte = 0; index_byte != 3U; ++index_byte) {
        const auto value = read_byte(host, registers.address[1]++);
        write_byte(host, registers.address[0] + 0x4bU, value);
        set_move_flags(registers, value, 0x80U);
        if (index_byte != 2U) registers.address[0] += 0x80U;
    }
    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
