#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto word = host.read_memory_word(
        kRegion, address & ~1U, odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_logic_word_flags(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_logic_byte_flags(CpuRegisters &r, std::uint8_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_compare_byte_flags(CpuRegisters &r,
    std::uint8_t destination, std::uint8_t source, std::uint8_t result)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (destination < source) flags |= 0x0001U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] FunctionResult finish_return(FunctionContext &context)
{
    auto &host = *context.host;
    auto &r = context.registers;
    const auto target = read_long(host, r.address[7]);
    r.address[7] += 4U;
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_find_free_slot_32_entries(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto gate = read_byte(host, 0x00000502U);
    const auto comparison = static_cast<std::uint8_t>(gate - 0xfeU);
    set_compare_byte_flags(r, gate, 0xfeU, comparison);
    if ((r.status & 0x0008U) != 0U) {
        r.data[0] = (r.data[0] & 0xffff0000U) | 0xffffU;
        set_logic_word_flags(r, 0xffffU);
        return finish_return(context);
    }

    r.data[0] = (r.data[0] & 0xffff0000U) | 0x001fU;
    set_logic_word_flags(r, 0x001fU);
    r.address[6] = 0x00005400U;

    for (;;) {
        const auto state = read_byte(host, r.address[6]);
        set_logic_byte_flags(r, state);
        if ((state & 0x80U) == 0U)
            return finish_return(context);

        r.address[6] += 0x80U;
        const auto counter = static_cast<std::uint16_t>(r.data[0] - 1U);
        r.data[0] = (r.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU)
            return finish_return(context);
    }
}

} // namespace gain_ground::translated
