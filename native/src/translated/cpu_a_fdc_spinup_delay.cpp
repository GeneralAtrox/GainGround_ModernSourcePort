#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kSharedOffsetMask = 0x0003ffffU;

void prefetch_word(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

void push_return_address(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t return_address)
{
    registers.address[7] -= 4U;
    const auto offset = registers.address[7] & kSharedOffsetMask;
    host.write_memory_word(kSharedRegion, offset,
        static_cast<std::uint16_t>(return_address >> 16U), kWordMask);
    host.write_memory_word(kSharedRegion, offset + 2U,
        static_cast<std::uint16_t>(return_address), kWordMask);
}
} // namespace

FunctionResult cpu_a_fdc_spinup_delay(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.data[1] = (registers.data[1] & 0xffff0000U) | 0x003cU;
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);

    for (std::uint32_t iteration = 0; iteration != 61U; ++iteration) {
        prefetch_word(host, 0x000017d2U);
        prefetch_word(host, 0x000017d4U);
        push_return_address(host, registers, 0x000017d4U);
        prefetch_word(host, 0x000017daU);
        prefetch_word(host, 0x000017dcU);
        registers.program_counter = 0x000017daU;
        (void)host.call_function(20U, 0U, 0xffU, 2U,
            0x000017d2U, 0x000017daU, context);

        registers.data[1] = (registers.data[1] & 0xffff0000U)
            | static_cast<std::uint16_t>(registers.data[1] - 1U);
    }

    prefetch_word(host, 0x000017d2U);
    prefetch_word(host, 0x000017d8U);
    prefetch_word(host, 0x000017daU);
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
