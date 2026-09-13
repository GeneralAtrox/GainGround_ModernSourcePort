#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(kPrivateRegion, address & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? word : (word >> 8U));
}

void set_test_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    const auto extend = static_cast<std::uint16_t>(registers.status & 0x0010U);
    std::uint16_t flags = extend;
    if (value == 0U) flags |= 0x0004U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(
        kPrivateRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_find_free_descriptor_slot(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto descriptor_base = host.read_memory_word(
        kPrivateRegion, registers.address[5] + 0x72U, kWordMask);
    registers.address[6] = static_cast<std::uint32_t>(static_cast<std::int32_t>(
        static_cast<std::int16_t>(descriptor_base)));
    registers.address[6] += 0x180U;

    const auto count = host.read_memory_word(
        kPrivateRegion, registers.address[3] + 4U, kWordMask);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | count;

    for (;;) {
        const auto state = read_byte(host, registers.address[6]);
        set_test_byte_flags(registers, state);
        if ((state & 0x80U) == 0U) break;

        registers.address[6] += 0x80U;
        const auto remaining = static_cast<std::uint16_t>(registers.data[1]);
        registers.data[1] = (registers.data[1] & 0xffff0000U)
            | static_cast<std::uint16_t>(remaining - 1U);
        if (remaining == 0U) break;
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
