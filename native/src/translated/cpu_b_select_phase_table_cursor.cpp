#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_logic_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    const auto extend = static_cast<std::uint16_t>(registers.status & 0x0010U);
    std::uint16_t flags = extend;
    if (value == 0U) flags |= 0x0004U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_asl_word_two_flags(CpuRegisters &registers, std::uint16_t value)
{
    bool carry = false;
    bool overflow = false;
    auto result = value;
    for (unsigned count = 0; count != 2U; ++count) {
        const bool old_sign = (result & 0x8000U) != 0U;
        carry = old_sign;
        result = static_cast<std::uint16_t>(result << 1U);
        overflow = overflow || (old_sign != ((result & 0x8000U) != 0U));
    }
    std::uint16_t flags = 0U;
    if (carry) flags |= 0x0011U;
    if (overflow) flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
}
} // namespace

FunctionResult cpu_b_select_phase_table_cursor(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[0] = read_long(host, registers.address[5] + 0x66U);
    registers.address[0] = read_long(host, registers.address[0]);

    const auto selector = host.read_memory_word(kRegion, registers.address[5] + 0x54U, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | selector;
    set_logic_word_flags(registers, selector);
    registers.data[1] = 0U;
    set_logic_word_flags(registers, 0U);

    const auto cursor = registers.address[0];
    const bool odd = (cursor & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(kRegion, cursor & ~1U, mask);
    const auto byte = static_cast<std::uint8_t>(odd ? word : (word >> 8U));
    registers.address[0] = cursor + 1U;
    registers.data[1] = byte;
    set_logic_word_flags(registers, byte);

    const auto and_result = static_cast<std::uint16_t>(selector & byte);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | and_result;
    set_logic_word_flags(registers, and_result);
    set_asl_word_two_flags(registers, and_result);
    registers.address[0] = static_cast<std::uint32_t>(registers.address[0]
        + static_cast<std::int32_t>(static_cast<std::int16_t>(registers.data[0])));

    const auto stack_pointer = registers.address[7];
    const auto target = read_long(host, stack_pointer);
    registers.address[7] = stack_pointer + 4U;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
