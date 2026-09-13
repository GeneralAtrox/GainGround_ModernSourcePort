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

FunctionResult cpu_a_fdc_read_data_byte(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto fdc_base = registers.address[5];

    for (;;) {
        prefetch_word(host, 0x00001928U);
        prefetch_word(host, 0x0000192aU);
        const auto status = read_hardware_byte(host, 0x00001924U, fdc_base + 8U);
        set_bit_test_flags(registers, (status & 0x01U) != 0U);
        if ((status & 0x01U) != 0U) {
            prefetch_word(host, 0x0000192cU);
            prefetch_word(host, 0x00001936U);
            prefetch_word(host, 0x00001938U);
            prefetch_word(host, 0x0000193aU);
            const auto value = read_hardware_byte(host, 0x00001936U, fdc_base + 6U);
            registers.data[0] = (registers.data[0] & 0xffffff00U) | value;
            set_move_byte_flags(registers, value);
            prefetch_word(host, 0x0000193cU);
            return return_from_subroutine(context);
        }

        prefetch_word(host, 0x0000192cU);
        prefetch_word(host, 0x0000192eU);
        prefetch_word(host, 0x00001930U);
        prefetch_word(host, 0x00001932U);
        const auto waiting = read_hardware_byte(host, 0x0000192cU, fdc_base + 8U);
        set_bit_test_flags(registers, (waiting & 0x02U) != 0U);
        if ((waiting & 0x02U) != 0U) {
            prefetch_word(host, 0x00001934U);
            prefetch_word(host, 0x00001936U);
            return return_from_subroutine(context);
        }
        prefetch_word(host, 0x00001924U);
        prefetch_word(host, 0x00001926U);
    }
}

} // namespace gain_ground::translated
