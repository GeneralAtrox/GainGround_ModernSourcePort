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
} // namespace

FunctionResult cpu_a_busy_wait_10000(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x2710U;
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);

    for (std::uint32_t iteration = 0; iteration != 10001U; ++iteration) {
        prefetch_word(host, 0x000017deU);
        prefetch_word(host, 0x000017e0U);
        registers.data[0] = (registers.data[0] & 0xffff0000U)
            | static_cast<std::uint16_t>(registers.data[0] - 1U);
    }
    prefetch_word(host, 0x000017deU);
    prefetch_word(host, 0x000017e2U);
    prefetch_word(host, 0x000017e4U);

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
