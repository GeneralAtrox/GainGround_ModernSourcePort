#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kByteMask = 0x00ffU;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kSharedOffsetMask = 0x0003ffffU;

void prefetch_word(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

[[nodiscard]] std::uint8_t read_hardware_byte(
    ExecutionHost &host, std::uint32_t pc, std::uint32_t address)
{
    return static_cast<std::uint8_t>(
        host.read_hardware(1U, 0U, 0xffU, pc, address & ~1U, kByteMask));
}

void set_bit_test_flags(CpuRegisters &registers, bool set)
{
    if (set)
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);
    else
        registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
}

void set_move_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags{};
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x000fU) | flags);
}
} // namespace

FunctionResult cpu_a_fdc_force_interrupt(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto command = registers.address[5];
    const auto status_address = command + 8U;

    for (;;) {
        prefetch_word(host, 0x000017e8U);
        prefetch_word(host, 0x000017eaU);
        const auto value = read_hardware_byte(host, 0x000017e4U, command);
        set_bit_test_flags(registers, (value & 0x01U) != 0U);
        if ((value & 0x01U) == 0U)
            break;
        prefetch_word(host, 0x000017e4U);
        prefetch_word(host, 0x000017e6U);
        registers.program_counter = 0x000017e4U;
        const auto loop = host.call_function(
            21U, 0U, 0xffU, 1U, 0x000017eaU, 0x000017e4U, context);
        if (loop.status == TranslationStatus::complete && loop.control == 3U)
            return FunctionResult::complete(4U, 0x000017e4U);
        return loop;
    }

    prefetch_word(host, 0x000017ecU);
    prefetch_word(host, 0x000017eeU);
    prefetch_word(host, 0x000017f0U);
    prefetch_word(host, 0x000017f2U);
    host.write_hardware(2U, 0U, 0xffU, 0x000017ecU,
        command & ~1U, 0U, kByteMask);
    set_move_byte_flags(registers, 0U);

    bool first_drq_test = true;
    for (;;) {
        if (first_drq_test) {
            prefetch_word(host, 0x000017f4U);
        }
        prefetch_word(host, 0x000017f6U);
        prefetch_word(host, 0x000017f8U);
        const auto value = read_hardware_byte(host, 0x000017f2U, status_address);
        set_bit_test_flags(registers, (value & 0x02U) != 0U);
        if ((value & 0x02U) != 0U)
            break;
        prefetch_word(host, 0x000017f2U);
        prefetch_word(host, 0x000017f4U);
        first_drq_test = false;
    }

    prefetch_word(host, 0x000017faU);
    prefetch_word(host, 0x000017fcU);
    prefetch_word(host, 0x000017feU);
    prefetch_word(host, 0x00001800U);
    const auto status = read_hardware_byte(host, 0x000017faU, status_address);
    set_bit_test_flags(registers, (status & 0x40U) != 0U);
    prefetch_word(host, 0x00001802U);
    registers.status = host.apply_controlled_status(
        0U, 0xffU, 0x00001800U, status_address & ~1U, registers.status);
    if ((registers.status & 0x0004U) == 0U) {
        prefetch_word(host, 0x00001964U);
        prefetch_word(host, 0x00001966U);
        registers.program_counter = 0x00001964U;
        return FunctionResult::complete(3U, 0x00001964U);
    }

    prefetch_word(host, 0x00001804U);
    prefetch_word(host, 0x00001806U);
    const auto stack_offset = registers.address[7] & kSharedOffsetMask;
    const auto return_address = (static_cast<std::uint32_t>(
        host.read_memory_word(kSharedRegion, stack_offset, kWordMask)) << 16U)
        | host.read_memory_word(kSharedRegion, stack_offset + 2U, kWordMask);
    registers.address[7] += 4U;
    prefetch_word(host, return_address);
    prefetch_word(host, return_address + 2U);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
