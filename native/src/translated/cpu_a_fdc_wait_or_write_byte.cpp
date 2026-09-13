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

FunctionResult return_from_subroutine(FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
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
} // namespace

FunctionResult cpu_a_fdc_wait_or_write_byte(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto fdc_base = registers.address[5];

    prefetch_word(host, 0x00001940U);
    prefetch_word(host, 0x00001942U);
    const auto write_ready = read_hardware_byte(host, 0x0000193cU, fdc_base + 8U);
    set_bit_test_flags(registers, (write_ready & 0x01U) != 0U);
    prefetch_word(host, 0x00001944U);
    registers.status = host.apply_controlled_status(0U, 0xffU,
        0x00001942U, (fdc_base + 8U) & ~1U, registers.status);
    if ((registers.status & 0x0004U) == 0U) {
        prefetch_word(host, 0x0000194eU);
        prefetch_word(host, 0x00001950U);
        prefetch_word(host, 0x00001952U);
        const auto value = static_cast<std::uint8_t>(registers.data[0]);
        host.write_hardware(2U, 0U, 0xffU, 0x0000194eU,
            (fdc_base + 6U) & ~1U,
            static_cast<std::uint16_t>(value) * 0x0101U, kByteMask);
        set_move_byte_flags(registers, value);
        prefetch_word(host, 0x00001954U);
        return return_from_subroutine(context);
    }

    prefetch_word(host, 0x00001946U);
    prefetch_word(host, 0x00001948U);
    prefetch_word(host, 0x0000194aU);
    const auto waiting = read_hardware_byte(host, 0x00001944U, fdc_base + 8U);
    set_bit_test_flags(registers, (waiting & 0x02U) != 0U);
    if ((waiting & 0x02U) != 0U) {
        prefetch_word(host, 0x0000194cU);
        prefetch_word(host, 0x0000194eU);
        return return_from_subroutine(context);
    }

    prefetch_word(host, 0x0000193cU);
    prefetch_word(host, 0x0000193eU);
    registers.program_counter = 0x0000193cU;
    const auto loop = host.call_function(
        25U, 0U, 0xffU, 1U, 0x0000194aU, 0x0000193cU, context);
    if (loop.status == TranslationStatus::complete && loop.control == 3U)
        return FunctionResult::complete(4U, 0x0000193cU);
    return loop;
}

} // namespace gain_ground::translated
