#include "gain_ground/contract_types.h"
#include "gground_functions.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kCallsite = 0x0001da54U;
constexpr std::uint32_t kContinuation = 0x0001da58U;
constexpr std::uint32_t kTarget = 0x0001daa6U;

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}
} // namespace

FunctionResult cpu_b_record_helper_tail(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    push_return(host, registers, kContinuation);
    registers.program_counter = kTarget;
    const auto setup = host.call_function(349U, 1U, 0x72U, 2U,
        kCallsite, kTarget, context);
    if (setup.status != TranslationStatus::complete || setup.control != 1U
        || setup.exit_program_counter != kContinuation)
        return setup;

    // The BSR return crosses directly into function 347's owned body. This is
    // a captured continuation, not another call, and therefore pushes no
    // additional return address and emits no host call record.
    return cpu_b_record_zero_accumulator(context);
}

} // namespace gain_ground::translated
