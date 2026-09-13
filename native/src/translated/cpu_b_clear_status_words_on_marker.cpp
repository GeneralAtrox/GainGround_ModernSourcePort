#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_compare_word_flags(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    const bool destination_negative = (destination & 0x8000U) != 0U;
    const bool source_negative = (source & 0x8000U) != 0U;
    const bool result_negative = (result & 0x8000U) != 0U;
    std::uint16_t flags = static_cast<std::uint16_t>(registers.status & 0x0010U);
    if (result_negative) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (destination_negative != source_negative && result_negative != destination_negative)
        flags |= 0x0002U;
    if (destination < source) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void clear_word(ExecutionHost &host, CpuRegisters &registers, std::uint32_t address)
{
    (void)host.read_memory_word(kRegion, address, kWordMask);
    host.write_memory_word(kRegion, address, 0U, kWordMask);
    const auto extend = static_cast<std::uint16_t>(registers.status & 0x0010U);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | extend | 0x0004U);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(kRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(kRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_clear_status_words_on_marker(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x4940U;
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);
    const auto marker = host.read_memory_word(kRegion, 0x00000d04U, kWordMask);
    set_compare_word_flags(registers, 0x4940U, marker);

    if (marker == 0x4940U) {
        registers.address[0] = 0x0000747aU;
        clear_word(host, registers, registers.address[0]);
        clear_word(host, registers, registers.address[0] + 0x0aU);
        clear_word(host, registers, registers.address[0] + 0x14U);
        clear_word(host, registers, registers.address[0] + 0x1eU);
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
