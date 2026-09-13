#include "gain_ground/contract_types.h"

#include <cstddef>
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

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_logic_flags(CpuRegisters &r, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub_word_flags(CpuRegisters &r,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (left < right) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void subtract_word(CpuRegisters &r, std::size_t data_index, std::uint16_t right)
{
    const auto left = static_cast<std::uint16_t>(r.data[data_index]);
    const auto result = static_cast<std::uint16_t>(left - right);
    r.data[data_index] = (r.data[data_index] & 0xffff0000U) | result;
    set_sub_word_flags(r, left, right, result);
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    host.write_memory_word(kRegion, r.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, r.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
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
    auto &host = *context.host;
    push_return(host, context.registers, continuation);
    context.registers.program_counter = target;
    return host.call_function(function_id, 1U, 0x72U, 2U,
        callsite, target, context);
}
} // namespace

FunctionResult cpu_b_record_zero_accumulator(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];

    r.data[0] = 0U;
    set_logic_flags(r, 0U, 0x80000000U, 0xffffffffU);
    const auto block_index = read_byte(host, base + 0x60U);
    r.data[0] = block_index;
    set_logic_flags(r, block_index, 0x80U, 0xffU);

    if (block_index == 0U) {
        const auto x = host.read_memory_word(kRegion, base + 0x62U, kWordMask);
        r.data[0] = (r.data[0] & 0xffff0000U) | x;
        set_logic_flags(r, x, 0x8000U, 0xffffU);
        subtract_word(r, 0U,
            host.read_memory_word(kRegion, base + 0x12U, kWordMask));

        const auto y = host.read_memory_word(kRegion, base + 0x64U, kWordMask);
        r.data[1] = (r.data[1] & 0xffff0000U) | y;
        set_logic_flags(r, y, 0x8000U, 0xffffU);
        subtract_word(r, 1U,
            host.read_memory_word(kRegion, base + 0x1aU, kWordMask));
    } else {
        const auto setup = call_child(context, 348U,
            0x0001da72U, 0x0001da92U, 0x0001da76U);
        if (setup.status != TranslationStatus::complete || setup.control != 1U)
            return setup;
        subtract_word(r, 0U,
            host.read_memory_word(kRegion, base + 0x12U, kWordMask));
        subtract_word(r, 1U,
            host.read_memory_word(kRegion, base + 0x1aU, kWordMask));
    }

    const auto lookup = call_child(context, 304U,
        0x0001da7eU, 0x0001631aU, 0x0001da84U);
    if (lookup.status != TranslationStatus::complete
        || (lookup.control != 1U && lookup.control != 3U))
        return lookup;

    auto value = static_cast<std::uint16_t>(r.data[1]);
    host.write_memory_word(kRegion, base + 0x5cU, value, kWordMask);
    set_logic_flags(r, value, 0x8000U, 0xffffU);

    const auto decode = call_child(context, 364U,
        0x0001da88U, 0x0001ec00U, 0x0001da8cU);
    if (decode.status != TranslationStatus::complete || decode.control != 1U)
        return decode;

    value = static_cast<std::uint16_t>(r.data[1]);
    host.write_memory_word(kRegion, base + 0x5cU, value, kWordMask);
    set_logic_flags(r, value, 0x8000U, 0xffffU);

    const auto return_address = pop_return(host, r);
    r.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
