#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kByteMask = 0x00ffU;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kConditionCodes = 0x001fU;
constexpr std::uint32_t kSharedOffsetMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

void prefetch_return_target(ExecutionHost &host, std::uint32_t address)
{
    if (address >= 0x00080000U && address <= 0x000bffffU) {
        (void)host.read_memory_word(
            kSharedRegion, address & kSharedOffsetMask, kWordMask);
        return;
    }
    prefetch(host, address);
}

std::uint16_t read_stack_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(
        kSharedRegion, address & kSharedOffsetMask, kWordMask);
}

std::uint32_t read_stack_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_stack_word(host, address)) << 16U)
        | read_stack_word(host, address + 2U);
}

void write_stack_word(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value, std::uint16_t mask = kWordMask)
{
    host.write_memory_word(
        kSharedRegion, (address & kSharedOffsetMask) & ~1U, value, mask);
}

void set_logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodes) | flags);
}

void set_logic_long(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodes) | flags);
}

void set_compare_byte(CpuRegisters &registers, std::uint8_t destination,
    std::uint8_t source)
{
    const auto result = static_cast<std::uint8_t>(destination - source);
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if ((((destination ^ source) & (destination ^ result)) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (source > destination) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodes) | flags);
}

std::uint8_t read_fdc_byte(ExecutionHost &host, std::uint32_t pc)
{
    return static_cast<std::uint8_t>(host.read_hardware(
        1U, 0U, 0xffU, pc, 0x00b00004U, kByteMask));
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t return_address)
{
    registers.address[7] -= 4U;
    write_stack_word(host, registers.address[7],
        static_cast<std::uint16_t>(return_address >> 16U));
    write_stack_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(return_address));
}

FunctionResult return_from_exception(ExecutionHost &host,
    CpuRegisters &registers)
{
    const auto restored_status = read_stack_word(host, registers.address[7]);
    registers.address[7] += 2U;
    const auto return_address = read_stack_long(host, registers.address[7]);
    registers.address[7] += 4U;
    prefetch_return_target(host, return_address);
    prefetch_return_target(host, return_address + 2U);
    registers.status = restored_status;
    registers.program_counter = return_address;
    return FunctionResult::complete(2U, return_address);
}
} // namespace

FunctionResult cpu_a_bios_fdc_drive_exception(FunctionContext &context) noexcept
{
    if (context.host == nullptr
        || context.registers.program_counter != 0x000021ccU) {
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    }

    auto &host = *context.host;
    auto &registers = context.registers;

    prefetch(host, 0x000021d0U);
    prefetch(host, 0x000021d2U);
    const auto original = read_fdc_byte(host, 0x000021ccU);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | original;
    set_logic_byte(registers, original);

    const auto inverted = static_cast<std::uint8_t>(~original);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | inverted;
    set_logic_byte(registers, inverted);

    prefetch(host, 0x000021d4U);
    prefetch(host, 0x000021d6U);
    prefetch(host, 0x000021d8U);
    prefetch(host, 0x000021daU);
    host.write_hardware(2U, 0U, 0xffU, 0x000021d4U,
        0x00b00004U, static_cast<std::uint16_t>(inverted) * 0x0101U,
        kByteMask);
    set_logic_byte(registers, inverted);

    prefetch(host, 0x000021dcU);
    registers.address[7] -= 4U;
    set_logic_long(registers, read_stack_long(host, registers.address[7]));

    prefetch(host, 0x000021deU);
    const auto stack_probe = read_stack_long(host, registers.address[7]);
    registers.address[7] += 4U;
    set_logic_long(registers, stack_probe);

    prefetch(host, 0x000021e0U);
    prefetch(host, 0x000021e2U);
    prefetch(host, 0x000021e4U);
    const auto observed = read_fdc_byte(host, 0x000021deU);
    set_compare_byte(registers, inverted, observed);

    prefetch(host, 0x000021e6U);
    if ((registers.status & 0x0004U) == 0U) {
        prefetch(host, 0x000021ecU);
    } else {
        prefetch(host, 0x000021e8U);
        push_return(host, registers, 0x000021eaU);
        prefetch(host, 0x000018a4U);
        prefetch(host, 0x000018a6U);
        registers.program_counter = 0x000018a4U;
        const auto child = host.call_function(23U, 0U, 0xffU, 2U,
            0x000021e6U, 0x000018a4U, context);
        if (child.status != TranslationStatus::complete
            || child.control != 1U
            || registers.program_counter != 0x000021eaU) {
            return child;
        }

        if ((registers.status & 0x0001U) != 0U) {
            prefetch(host, 0x000021f2U);
            prefetch(host, 0x000021f4U);
            prefetch(host, 0x000021f6U);
            prefetch(host, 0x000021f8U);
            write_stack_word(host, registers.address[7] + 1U,
                0x0101U, kByteMask);
            set_logic_byte(registers, 1U);
            prefetch(host, 0x000021faU);
            return return_from_exception(host, registers);
        }
    }

    prefetch(host, 0x000021eeU);
    prefetch(host, 0x000021f0U);
    (void)read_stack_word(host, registers.address[7]);
    prefetch(host, 0x000021f2U);
    write_stack_word(host, registers.address[7] + 1U, 0U, kByteMask);
    set_logic_byte(registers, 0U);
    return return_from_exception(host, registers);
}

} // namespace gain_ground::translated
