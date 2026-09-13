#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto value = host.read_memory_word(
        kRegion, address & ~1U, odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? value : value >> 8U);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    host.write_memory_word(kRegion, address & ~1U,
        static_cast<std::uint16_t>(odd ? value : value << 8U),
        odd ? 0x00ffU : 0xff00U);
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto high = host.read_memory_word(kRegion, r.address[7], kWordMask);
    const auto low = host.read_memory_word(kRegion, r.address[7] + 2U, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_logic_word_flags(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_logic_long_flags(CpuRegisters &r, std::uint32_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &r, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub_word_flags(CpuRegisters &r, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (left < right) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void add_word(CpuRegisters &r, std::uint16_t value)
{
    const auto left = static_cast<std::uint16_t>(r.data[1]);
    const auto result = static_cast<std::uint16_t>(left + value);
    r.data[1] = (r.data[1] & 0xffff0000U) | result;
    set_add_word_flags(r, left, value, result);
}

void subtract_word(CpuRegisters &r, std::uint16_t value)
{
    const auto left = static_cast<std::uint16_t>(r.data[1]);
    const auto result = static_cast<std::uint16_t>(left - value);
    r.data[1] = (r.data[1] & 0xffff0000U) | result;
    set_sub_word_flags(r, left, value, result);
}

void and_word(CpuRegisters &r, std::uint16_t value)
{
    const auto result = static_cast<std::uint16_t>(r.data[1]) & value;
    r.data[1] = (r.data[1] & 0xffff0000U) | result;
    set_logic_word_flags(r, result);
}
} // namespace

FunctionResult cpu_b_decode_mode_offset(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    const auto raw_mode = host.enemy_direction_mode(base, read_byte(host, base + 0x5eU));
    const auto mode = static_cast<std::int8_t>(raw_mode) > 9 ? 0U : raw_mode;

    // LSL.W #2 on every contracted selector value (0..10) shifts out zero,
    // clearing X before the selected handler executes.
    r.status = static_cast<std::uint16_t>(r.status & ~0x0010U);
    r.data[1] = host.read_memory_word(kRegion, base + 0x5cU, kWordMask);
    set_logic_word_flags(r, static_cast<std::uint16_t>(r.data[1]));

    switch (mode) {
    case 0U: {
        write_long(host, base + 0x1eU, 0U);
        set_logic_long_flags(r, 0U);
        write_long(host, base + 0x26U, 0U);
        set_logic_long_flags(r, 0U);
        const auto address = base + 0x40U;
        const auto old = read_byte(host, address);
        write_byte(host, address, static_cast<std::uint8_t>(old | 0x02U));
        r.status = static_cast<std::uint16_t>(
            (r.status & ~0x0004U) | ((old & 0x02U) == 0U ? 0x0004U : 0U));
        break;
    }
    case 1U:
        add_word(r, 0x0200U);
        and_word(r, 0x0400U);
        break;
    case 2U:
        and_word(r, 0x0400U);
        add_word(r, 0x0200U);
        break;
    case 3U:
        add_word(r, 0x0100U);
        and_word(r, 0x0400U);
        add_word(r, 0x0100U);
        break;
    case 4U:
        subtract_word(r, 0x0100U);
        and_word(r, 0x0400U);
        add_word(r, 0x0300U);
        break;
    case 5U:
        add_word(r, 0x0100U);
        and_word(r, 0x0600U);
        break;
    case 6U:
        and_word(r, 0x0600U);
        add_word(r, 0x0100U);
        break;
    case 7U:
        add_word(r, 0x0080U);
        and_word(r, 0x0700U);
        break;
    case 8U:
        add_word(r, 0x0040U);
        and_word(r, 0x0780U);
        break;
    case 9U:
        add_word(r, 0x0020U);
        and_word(r, 0x07c0U);
        break;
    default:
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    }

    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
