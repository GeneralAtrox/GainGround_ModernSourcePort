#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

[[nodiscard]] std::uint16_t read_shared(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & kAddressMask, kWordMask);
}

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)read_shared(host, address);
}

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_a_sound_resolve_stream_pointer(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    context.registers.data[0] &= 0x0000ffffU;

    prefetch(host, 0x00084142U);
    prefetch(host, 0x00084144U);
    prefetch(host, 0x00084146U);
    prefetch(host, 0x00084148U);
    const std::uint32_t first_address = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(context.registers.address[5]) +
        static_cast<std::int16_t>(context.registers.data[1]));
    const auto table_offset = read_shared(host, first_address);
    context.registers.data[1] =
        (context.registers.data[1] & 0xffff0000U) | table_offset;
    set_move_word_flags(context.registers, table_offset);

    prefetch(host, 0x0008414aU);
    prefetch(host, 0x0008414cU);
    prefetch(host, 0x0008414eU);
    std::uint32_t pointer = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(context.registers.address[5]) +
        static_cast<std::int16_t>(table_offset));
    context.registers.address[0] = pointer;
    prefetch(host, 0x00084150U);
    const std::uint32_t second_address = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(pointer) +
        static_cast<std::int16_t>(context.registers.data[0]));
    const auto resolved = read_shared(host, second_address);
    context.registers.data[0] =
        (context.registers.data[0] & 0xffff0000U) | resolved;
    set_move_word_flags(context.registers, resolved);

    prefetch(host, 0x00084152U);
    prefetch(host, 0x00084154U);
    pointer += context.registers.data[0];
    context.registers.address[0] = pointer;
    const auto stack = context.registers.address[7] & kAddressMask;
    const auto return_address = (static_cast<std::uint32_t>(
        host.read_memory_word(kRegion, stack, kWordMask)) << 16U) |
        host.read_memory_word(kRegion, (stack + 2U) & kAddressMask, kWordMask);
    context.registers.address[7] += 4U;
    prefetch(host, return_address);
    prefetch(host, return_address + 2U);
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}
} // namespace gain_ground::translated
