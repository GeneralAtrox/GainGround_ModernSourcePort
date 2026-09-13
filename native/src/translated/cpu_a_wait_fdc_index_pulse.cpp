#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kByteMask = 0x00ffU;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kSharedOffsetMask = 0x0003ffffU;
constexpr std::uint32_t kFdcStatusAddress = 0x00b00008U;

void prefetch_word(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

[[nodiscard]] std::uint16_t read_fdc_status(
    ExecutionHost &host, std::uint32_t instruction_pc)
{
    return host.read_hardware(
        1U, 0U, 0xffU, instruction_pc, kFdcStatusAddress, kByteMask);
}

void set_compare_long_flags(CpuRegisters &registers,
    std::uint32_t destination, std::uint32_t source)
{
    const auto result = destination - source;
    const bool negative = (result & 0x80000000U) != 0U;
    const bool zero = result == 0U;
    const bool overflow = ((destination ^ source) & (destination ^ result)
        & 0x80000000U) != 0U;
    const bool carry = destination < source;
    std::uint16_t flags = registers.status & 0x0010U;
    if (negative) flags |= 0x0008U;
    if (zero) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    if (carry) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_a_wait_fdc_index_pulse(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    prefetch_word(host, 0x00000856U);
    registers.address[0] = 0x00b00009U;

    for (;;) {
        prefetch_word(host, 0x00000858U);
        prefetch_word(host, 0x0000085aU);
        prefetch_word(host, 0x0000085cU);
        const auto status = read_fdc_status(host, 0x00000858U);
        prefetch_word(host, 0x0000085eU);
        if ((status & 0x0020U) == 0U)
            break;
    }

    for (;;) {
        prefetch_word(host, 0x00000860U);
        prefetch_word(host, 0x00000862U);
        const auto status = read_fdc_status(host, 0x0000085eU);
        prefetch_word(host, 0x00000864U);
        if ((status & 0x0020U) != 0U)
            break;
        prefetch_word(host, 0x0000085eU);
    }

    registers.data[0] = 0U;
    prefetch_word(host, 0x00000866U);
    for (;;) {
        ++registers.data[0];
        prefetch_word(host, 0x00000868U);
        prefetch_word(host, 0x0000086aU);
        prefetch_word(host, 0x0000086cU);
        const auto status = read_fdc_status(host, 0x00000868U);
        prefetch_word(host, 0x0000086eU);
        if ((status & 0x0020U) == 0U)
            break;
        prefetch_word(host, 0x00000866U);
    }

    prefetch_word(host, 0x00000870U);
    prefetch_word(host, 0x00000872U);
    set_compare_long_flags(registers, registers.data[0], 0x0000f700U);
    prefetch_word(host, 0x00000874U);
    prefetch_word(host, 0x00000876U);
    if ((registers.status & 0x0001U) != 0U)
        registers.address[6] = (registers.address[6] & 0xffff0000U)
            | static_cast<std::uint16_t>(registers.address[6] + 2U);

    prefetch_word(host, 0x00000878U);
    prefetch_word(host, 0x0000087aU);
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
