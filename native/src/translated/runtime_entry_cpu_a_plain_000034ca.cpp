#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
FunctionResult cpu_a_ym2151_write_register(FunctionContext &context) noexcept;

namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kMainRamRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kMainRamMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto offset = address & kMainRamMask;
    const auto mask = static_cast<std::uint16_t>(
        (offset & 1U) == 0U ? 0xff00U : 0x00ffU);
    const auto shift = (offset & 1U) == 0U ? 8U : 0U;
    return static_cast<std::uint8_t>(host.read_memory_word(
        kProgramRegion, offset & ~1U, mask) >> shift);
}

void set_logic_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_logic_long_flags(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_byte_flags(CpuRegisters &registers,
    std::uint8_t left, std::uint8_t right, std::uint8_t result)
{
    const auto sum = static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(left) + right);
    std::uint16_t flags{};
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if (sum > 0xffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    const auto offset = registers.address[7] & kMainRamMask;
    host.write_memory_word(kMainRamRegion, offset,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kMainRamRegion, offset + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto offset = registers.address[7] & kMainRamMask;
    const auto result = (static_cast<std::uint32_t>(host.read_memory_word(
        kMainRamRegion, offset, kWordMask)) << 16U)
        | host.read_memory_word(kMainRamRegion, offset + 2U, kWordMask);
    registers.address[7] += 4U;
    return result;
}

void call_ym2151_writer(FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    push_return(host, registers, 0x000034deU);
    prefetch(host, 0x000034e6U);
    prefetch(host, 0x000034e8U);
    registers.program_counter = 0x000034e6U;
    auto result = host.call_function(40U, 0U, 0xffU, 2U,
        0x000034daU, 0x000034e6U, context);
    while (result.status == TranslationStatus::complete
        && result.control == 4U) {
        result = cpu_a_ym2151_write_register(context);
    }
}
} // namespace

FunctionResult runtime_entry_cpu_a_plain_000034ca(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    const auto source = read_byte(host, registers.address[0]++);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | source;
    set_logic_byte_flags(registers, source);

    const auto delta = static_cast<std::uint8_t>(registers.data[6]);
    const auto adjusted = static_cast<std::uint8_t>(source + delta);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | adjusted;
    set_add_byte_flags(registers, source, delta, adjusted);
    prefetch(host, 0x000034ceU);
    prefetch(host, 0x000034d0U);

    if ((adjusted & 0x80U) != 0U) {
        set_logic_byte_flags(registers, delta);
        prefetch(host, 0x000034d2U);
        prefetch(host, 0x000034d4U);
        const std::uint32_t clamped = (delta & 0x80U) != 0U ? 0U : 0x7fU;
        registers.data[1] = clamped;
        set_logic_long_flags(registers, clamped);
        if ((delta & 0x80U) != 0U)
            prefetch(host, 0x000034d6U);
        prefetch(host, 0x000034d8U);
    }

    prefetch(host, 0x000034daU);
    prefetch(host, 0x000034dcU);
    call_ym2151_writer(context);

    const auto old_register = static_cast<std::uint8_t>(registers.data[0]);
    const auto next_register = static_cast<std::uint8_t>(old_register + 8U);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | next_register;
    set_add_byte_flags(registers, old_register, 8U, next_register);
    prefetch(host, 0x000034e2U);

    const auto count = static_cast<std::uint16_t>(registers.data[2]);
    registers.data[2] = (registers.data[2] & 0xffff0000U)
        | static_cast<std::uint16_t>(count - 1U);
    prefetch(host, 0x000034caU);
    if (count != 0U) {
        prefetch(host, 0x000034ccU);
        registers.program_counter = 0x000034caU;
        const auto continuation = host.call_function(
            514U, 0U, 0xffU, 1U, 0x000034e0U, 0x000034caU, context);
        if (continuation.status == TranslationStatus::complete
            && continuation.control == 3U)
            return FunctionResult::complete(4U, 0x000034caU);
        return continuation;
    }

    prefetch(host, 0x000034e4U);
    prefetch(host, 0x000034e6U);
    const auto return_address = pop_return(host, registers);
    prefetch(host, return_address);
    prefetch(host, return_address + 2U);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
