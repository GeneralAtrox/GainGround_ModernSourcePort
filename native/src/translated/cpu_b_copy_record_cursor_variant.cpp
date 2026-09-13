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

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    host.write_memory_word(kRegion, address & ~1U,
        static_cast<std::uint16_t>(odd ? value : value << 8U),
        odd ? 0x00ffU : 0xff00U);
}

void set_logic_flags(CpuRegisters &r, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub_word_flags(CpuRegisters &r, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (left < right) flags |= 0x0011U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_zero_only(CpuRegisters &r, bool zero)
{
    r.status = static_cast<std::uint16_t>(
        (r.status & ~0x0004U) | (zero ? 0x0004U : 0U));
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    write_long(host, r.address[7], value);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto value = read_long(host, r.address[7]);
    r.address[7] += 4U;
    return value;
}

[[nodiscard]] FunctionResult call_child(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t continuation)
{
    push_return(*context.host, context.registers, continuation);
    context.registers.program_counter = target;
    return context.host->call_function(function_id, 1U, 0x72U, 2U,
        callsite, target, context);
}
} // namespace

FunctionResult cpu_b_copy_record_cursor_variant(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];

    auto value = host.read_memory_word(kRegion, base + 0x5cU, kWordMask);
    host.write_memory_word(kRegion, base + 0x3aU, value, kWordMask);
    set_logic_flags(r, value, 0x8000U, 0xffffU);

    const auto random = call_child(context, 283U,
        0x0001db62U, 0x00015e28U, 0x0001db68U);
    if (random.status != TranslationStatus::complete || random.control != 1U)
        return random;

    value = static_cast<std::uint16_t>(r.data[1]) & 0x07ffU;
    r.data[1] = (r.data[1] & 0xffff0000U) | value;
    set_logic_flags(r, value, 0x8000U, 0xffffU);
    host.write_memory_word(kRegion, base + 0x5cU, value, kWordMask);
    set_logic_flags(r, value, 0x8000U, 0xffffU);

    const auto decode = call_child(context, 364U,
        0x0001db70U, 0x0001ec00U, 0x0001db74U);
    if (decode.status != TranslationStatus::complete || decode.control != 1U)
        return decode;

    value = host.read_memory_word(kRegion, base + 0x3aU, kWordMask);
    host.write_memory_word(kRegion, base + 0x5cU, value, kWordMask);
    set_logic_flags(r, value, 0x8000U, 0xffffU);
    value = static_cast<std::uint16_t>(r.data[1]);
    host.write_memory_word(kRegion, base + 0x3aU, value, kWordMask);
    set_logic_flags(r, value, 0x8000U, 0xffffU);

    const auto cursor = host.read_memory_word(kRegion, base + 0x5cU, kWordMask);
    r.data[0] = (r.data[0] & 0xffff0000U) | cursor;
    set_logic_flags(r, cursor, 0x8000U, 0xffffU);
    write_byte(host, base + 0x39U, 0x40U);
    set_logic_flags(r, 0x40U, 0x80U, 0xffU);

    const auto decoded = static_cast<std::uint16_t>(r.data[1]);
    const auto difference = static_cast<std::uint16_t>(decoded - cursor);
    r.data[1] = (r.data[1] & 0xffff0000U) | difference;
    set_sub_word_flags(r, decoded, cursor, difference);
    const auto phase = static_cast<std::uint16_t>(difference & 0x0400U);
    r.data[1] = (r.data[1] & 0xffff0000U) | phase;
    set_logic_flags(r, phase, 0x8000U, 0xffffU);
    if (phase != 0U) {
        write_byte(host, base + 0x39U, 0xc0U);
        set_logic_flags(r, 0xc0U, 0x80U, 0xffU);
    }

    const auto flags_address = base + 0x41U;
    const auto flags = read_byte(host, flags_address);
    set_zero_only(r, (flags & 0x10U) == 0U);
    write_byte(host, flags_address, static_cast<std::uint8_t>(flags | 0x10U));

    const auto direction = read_byte(host, base + 0x37U);
    r.data[0] = (r.data[0] & 0xffffff00U) | direction;
    set_logic_flags(r, direction, 0x80U, 0xffU);
    write_byte(host, base + 0x36U, direction);
    set_logic_flags(r, direction, 0x80U, 0xffU);

    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
