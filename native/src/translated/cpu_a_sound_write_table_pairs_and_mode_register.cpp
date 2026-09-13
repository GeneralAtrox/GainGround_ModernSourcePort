#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 3U;
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kRegion, address & kAddressMask, kWordMask);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto offset = address & kAddressMask;
    const auto mask = static_cast<std::uint16_t>(
        (offset & 1U) == 0U ? 0xff00U : 0x00ffU);
    const auto shift = (offset & 1U) == 0U ? 8U : 0U;
    const auto region = address <= kAddressMask ? kProgramRegion : kRegion;
    return static_cast<std::uint8_t>(host.read_memory_word(
        region, offset & ~1U, mask) >> shift);
}

void set_logic_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
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
    const auto offset = registers.address[7] & kAddressMask;
    host.write_memory_word(kRegion, offset,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, offset + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto offset = registers.address[7] & kAddressMask;
    const auto result = (static_cast<std::uint32_t>(
        host.read_memory_word(kRegion, offset, kWordMask)) << 16U)
        | host.read_memory_word(kRegion, offset + 2U, kWordMask);
    registers.address[7] += 4U;
    return result;
}

void call_writer(FunctionContext &context, std::uint32_t callsite,
    std::uint32_t return_address)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    push_return(host, registers, return_address);
    prefetch(host, 0x00084126U);
    prefetch(host, 0x00084128U);
    registers.program_counter = 0x00084126U;
    (void)host.call_function(88U, 0U, 0xffU, 2U,
        callsite, 0x00084126U, context);
}
} // namespace

FunctionResult cpu_a_sound_write_table_pairs_and_mode_register(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    prefetch(host, 0x000842e8U);
    prefetch(host, 0x000842eaU);
    registers.address[0] = 0x0008430cU;
    registers.data[2] = (registers.data[2] & 0xffff0000U) | 2U;
    set_logic_byte_flags(registers, 2U);

    for (;;) {
        prefetch(host, 0x000842ecU);
        prefetch(host, 0x000842eeU);
        const auto address_byte = read_byte(host, registers.address[0]++);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | address_byte;
        set_logic_byte_flags(registers, address_byte);
        prefetch(host, 0x000842f0U);
        const auto data_byte = read_byte(host, registers.address[0]++);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | data_byte;
        set_logic_byte_flags(registers, data_byte);
        prefetch(host, 0x000842f2U);
        call_writer(context, 0x000842f0U, 0x000842f4U);
        const auto count = static_cast<std::uint16_t>(registers.data[2]);
        registers.data[2] = (registers.data[2] & 0xffff0000U)
            | static_cast<std::uint16_t>(count - 1U);
        if (count == 0U) break;
    }

    prefetch(host, 0x000842ecU);
    prefetch(host, 0x000842f8U);
    prefetch(host, 0x000842faU);
    prefetch(host, 0x000842fcU);
    const auto mode = read_byte(host, registers.address[3] + 2U);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | mode;
    set_logic_byte_flags(registers, mode);
    prefetch(host, 0x000842feU);
    const auto masked = static_cast<std::uint8_t>(mode & 7U);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | masked;
    set_logic_byte_flags(registers, masked);
    prefetch(host, 0x00084300U);
    prefetch(host, 0x00084302U);
    const auto ym_register = static_cast<std::uint8_t>(masked + 0x38U);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | ym_register;
    set_add_byte_flags(registers, masked, 0x38U, ym_register);
    prefetch(host, 0x00084304U);
    registers.data[1] = 0U;
    set_logic_byte_flags(registers, 0U);
    prefetch(host, 0x00084306U);
    prefetch(host, 0x00084308U);
    call_writer(context, 0x00084306U, 0x0008430aU);

    const auto return_address = pop_return(host, registers);
    prefetch(host, return_address);
    prefetch(host, return_address + 2U);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
