#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    const auto extend = static_cast<std::uint16_t>(registers.status & 0x0010U);
    std::uint16_t flags = extend;
    if (value == 0U) flags |= 0x0004U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void asl_word_two(CpuRegisters &registers)
{
    auto value = static_cast<std::uint16_t>(registers.data[0]);
    bool carry = false;
    bool overflow = false;
    for (std::uint32_t shift = 0; shift != 2U; ++shift) {
        const bool sign_before = (value & 0x8000U) != 0U;
        carry = sign_before;
        value = static_cast<std::uint16_t>(value << 1U);
        overflow = overflow || (sign_before != ((value & 0x8000U) != 0U));
    }
    std::uint16_t flags = 0U;
    if (carry) flags |= 0x0011U;
    if (overflow) flags |= 0x0002U;
    if (value == 0U) flags |= 0x0004U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
}

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kPrivateRegion, address, kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return target;
}
} // namespace

FunctionResult cpu_b_load_descriptor_pointer_pair(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto index = host.read_memory_word(
        kPrivateRegion, registers.address[5] + 0x58U, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | index;
    set_move_word_flags(registers, index);
    asl_word_two(registers);
    registers.address[1] = read_long(host, registers.address[3] + 0x20U);

    push_return(host, registers, 0x00010c98U);
    registers.program_counter = 0x00010c9aU;
    const auto child = host.call_function(223U, 1U, 0x72U, 2U,
        0x00010c94U, 0x00010c9aU, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
