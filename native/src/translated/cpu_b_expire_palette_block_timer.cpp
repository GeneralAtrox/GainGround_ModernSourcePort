#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    const std::uint16_t flags = static_cast<std::uint16_t>(
        (registers.status & 0x0010U) | ((value & 0x8000U) ? 0x0008U : 0U) | (value == 0U ? 0x0004U : 0U));
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_sub_word_flags(CpuRegisters &registers, std::uint16_t left, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ 1U) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (left < 1U) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

} // namespace

FunctionResult cpu_b_expire_palette_block_timer(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    const std::uint32_t timer_address = context.registers.address[5] + 0x24U;
    const auto timer = host.read_memory_word(kRegion, timer_address, kWordMask);
    set_logic_word_flags(context.registers, timer);
    if (timer != 0U) {
        const auto current = host.read_memory_word(kRegion, timer_address, kWordMask);
        const auto decremented = static_cast<std::uint16_t>(current - 1U);
        host.write_memory_word(kRegion, timer_address, decremented, kWordMask);
        set_sub_word_flags(context.registers, current, decremented);
        if (static_cast<std::int16_t>(decremented) <= 0) {
            (void)host.read_memory_word(kRegion, 0x0000745cU, kWordMask);
            host.write_memory_word(kRegion, 0x0000745cU, 0U, kWordMask);
            set_logic_word_flags(context.registers, 0U);
        }
    }
    const auto return_address = read_long(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
