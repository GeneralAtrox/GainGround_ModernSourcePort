#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

[[nodiscard]] std::uint16_t read_shared(
    ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(
        kSharedRegion, address & kSharedMask, kWordMask);
}

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kSharedRegion,
        address & kSharedMask, kWordMask);
}

void write_shared(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value)
{
    host.write_memory_word(
        kSharedRegion, address & kSharedMask, value, kWordMask);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_00000400(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto stack = registers.address[7];
    const auto restored_status = read_shared(host, stack);
    const auto target =
        (static_cast<std::uint32_t>(read_shared(host, stack + 2U)) << 16U)
        | read_shared(host, stack + 4U);
    registers.address[7] += 6U;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.status = restored_status;
    registers.program_counter = target;

    const auto pending = host.consume_pending_interrupt(0U, 0xffU, 0x00000400U);
    const auto mask = static_cast<std::uint8_t>((restored_status >> 8U) & 7U);
    if (!pending.asserted || pending.level <= mask)
        return FunctionResult::complete(2U, target);
    if (pending.level < 3U || pending.level > 5U)
        return {TranslationStatus::contract_violation, 0U, target};

    registers.address[7] -= 4U;
    write_shared(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(target));
    registers.address[7] -= 2U;
    write_shared(host, registers.address[7], restored_status);
    write_shared(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(target >> 16U));

    const auto vector_address = static_cast<std::uint32_t>(
        0x00000060U + static_cast<std::uint32_t>(pending.level) * 4U);
    const auto vector_target =
        (static_cast<std::uint32_t>(host.read_memory_word(
            kProgramRegion, vector_address, kWordMask)) << 16U)
        | host.read_memory_word(kProgramRegion, vector_address + 2U, kWordMask);
    prefetch(host, vector_target);
    prefetch(host, vector_target + 2U);
    registers.status = static_cast<std::uint16_t>(
        (restored_status & 0x38ffU) | 0x2000U
        | (static_cast<std::uint16_t>(pending.level) << 8U));
    registers.program_counter = vector_target;
    const auto child_id = static_cast<std::uint32_t>(43U + pending.level);
    (void)host.call_function(child_id, 0U, 0xffU, 3U,
        0x00000400U, vector_target, context);
    return FunctionResult::complete(2U, vector_target);
}

} // namespace gain_ground::translated
