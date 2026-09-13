#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kCharacterRegion = 8U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint32_t read_return_address(ExecutionHost &host, std::uint32_t address)
{
    const auto offset = address & 0x0003ffffU;
    const auto high = host.read_memory_word(kSharedRegion, offset, kWordMask);
    const auto low = host.read_memory_word(kSharedRegion, (offset + 2U) & 0x0003ffffU, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_add_word_flags(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_a_fill_longword_table_and_add_index(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    std::uint32_t destination = context.registers.address[2];
    const std::uint32_t value = context.registers.data[0];
    (void)host.read_memory_word(1U, 0x00003854U, kWordMask);
    for (unsigned count = 0; count != 8U; ++count) {
        const auto offset = destination - 0x00280000U;
        host.write_memory_word(kCharacterRegion, offset,
            static_cast<std::uint16_t>(value >> 16U), kWordMask);
        host.write_memory_word(kCharacterRegion, offset + 2U,
            static_cast<std::uint16_t>(value), kWordMask);
        destination += 4U;
        (void)host.read_memory_word(1U, 0x00003856U, kWordMask);
        (void)host.read_memory_word(1U, 0x00003852U, kWordMask);
        if (count != 7U)
            (void)host.read_memory_word(1U, 0x00003854U, kWordMask);
    }
    (void)host.read_memory_word(1U, 0x00003858U, kWordMask);
    (void)host.read_memory_word(1U, 0x0000385aU, kWordMask);
    (void)host.read_memory_word(1U, 0x0000385cU, kWordMask);
    (void)host.read_memory_word(1U, 0x0000385eU, kWordMask);
    context.registers.address[2] = destination;
    context.registers.data[1] = 0x0000ffffU;
    const std::uint32_t swapped = (value << 16U) | (value >> 16U);
    const auto left = static_cast<std::uint16_t>(swapped);
    const auto right = static_cast<std::uint16_t>(context.registers.data[6]);
    const auto result = static_cast<std::uint16_t>(left + right);
    context.registers.data[0] = (swapped & 0xffff0000U) | result;
    set_add_word_flags(context.registers, left, right, result);
    const auto return_address = read_return_address(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    // The captured CPU-A RTS completes its two-word return-target prefetch
    // before the fixture boundary.
    (void)host.read_memory_word(1U, return_address, kWordMask);
    (void)host.read_memory_word(1U, return_address + 2U, kWordMask);
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}
} // namespace gain_ground::translated
