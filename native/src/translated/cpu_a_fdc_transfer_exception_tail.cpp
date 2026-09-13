#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgram = 1U;
constexpr std::uint16_t kShared = 3U;
constexpr std::uint16_t kWord = 0xffffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgram, address, kWord);
}

void prefetch_shared(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kShared, address & kSharedMask, kWord);
}

std::uint16_t read_shared_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kShared, address & kSharedMask, kWord);
}

void write_shared_word(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value)
{
    host.write_memory_word(kShared, address & kSharedMask, value, kWord);
}

std::uint32_t read_shared_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_shared_word(host, address)) << 16U)
        | read_shared_word(host, address + 2U);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_shared_word(host, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U));
    write_shared_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value));
}
} // namespace

FunctionResult cpu_a_fdc_transfer_exception_tail(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    // BSR.W $113e. The target words become the child's entry prefetches.
    push_return(host, registers, 0x00002224U);
    prefetch(host, 0x0000113eU);
    prefetch(host, 0x00001140U);
    registers.program_counter = 0x0000113eU;
    const auto child = host.call_function(13U, 0U, 0xffU, 2U,
        0x00002220U, 0x0000113eU, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    // Function 13's RTS owns the $2224/$2226 fetch pair. RTE restores the
    // exception frame, then fetches its runtime-RAM destination.
    const auto status = read_shared_word(host, registers.address[7]);
    registers.address[7] += 2U;
    const auto target = read_shared_long(host, registers.address[7]);
    registers.address[7] += 4U;
    prefetch_shared(host, target);
    prefetch_shared(host, target + 2U);
    registers.status = status;
    registers.program_counter = target;
    return FunctionResult::complete(2U, target);
}

} // namespace gain_ground::translated
