#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kWordMask = 0xffffU;

void prefetch_word(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_0000196c(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x0004U;
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);

    prefetch_word(host, 0x00001970U);
    prefetch_word(host, 0x00001972U);
    prefetch_word(host, 0x00001990U);
    prefetch_word(host, 0x00001992U);

    registers.program_counter = 0x00001990U;
    return FunctionResult::complete(3U, 0x00001990U);
}

} // namespace gain_ground::translated
