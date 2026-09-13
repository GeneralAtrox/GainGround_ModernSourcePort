#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

std::uint16_t read_shared(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kSharedRegion, address & kSharedMask, kWordMask);
}

void write_shared(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kSharedRegion, address & kSharedMask, value, kWordMask);
}

FunctionResult return_from_subroutine(FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto stack = registers.address[7] & kSharedMask;
    const auto target = (static_cast<std::uint32_t>(read_shared(host, stack)) << 16U)
        | read_shared(host, stack + 2U);
    registers.address[7] += 4U;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_00001cec(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[1] = 0x00019400U;
    prefetch(host, 0x00001cf0U);
    prefetch(host, 0x00001cf2U);
    prefetch(host, 0x00001cf4U);
    prefetch(host, 0x00001cf6U);
    registers.address[2] = 0x00280800U;
    prefetch(host, 0x00001cf8U);
    prefetch(host, 0x00001cfaU);
    registers.data[7] = (registers.data[7] & 0xffff0000U) | 0x00bfU;
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);
    prefetch(host, 0x00001cfcU);

    for (;;) {
        prefetch(host, 0x00001cfeU);
        registers.address[7] -= 4U;
        write_shared(host, registers.address[7], 0U);
        write_shared(host, registers.address[7] + 2U, 0x1d00U);
        prefetch(host, 0x00001c28U);
        prefetch(host, 0x00001c2aU);
        registers.program_counter = 0x00001c28U;
        const auto child = host.call_function(
            29U, 0U, 0xffU, 2U, 0x00001cfcU, 0x00001c28U, context);
        if (child.status != TranslationStatus::complete)
            return child;

        registers.address[1] += 8U;
        prefetch(host, 0x00001d04U);
        const auto counter = static_cast<std::uint16_t>(registers.data[7] - 1U);
        registers.data[7] = (registers.data[7] & 0xffff0000U) | counter;
        prefetch(host, 0x00001cfcU);
        if (counter == 0xffffU)
            break;
    }

    prefetch(host, 0x00001d06U);
    prefetch(host, 0x00001d08U);
    return return_from_subroutine(context);
}

} // namespace gain_ground::translated
