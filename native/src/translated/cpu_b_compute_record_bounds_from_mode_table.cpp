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

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address, value, kWordMask);
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

[[nodiscard]] std::uint16_t add_word(
    CpuRegisters &r, std::uint16_t left, std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left + right);
    set_add_word_flags(r, left, right, result);
    return result;
}

[[nodiscard]] std::uint16_t subtract_word(
    CpuRegisters &r, std::uint16_t left, std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left - right);
    set_sub_word_flags(r, left, right, result);
    return result;
}

void store_result(ExecutionHost &host, CpuRegisters &r,
    std::uint32_t address, std::uint16_t value)
{
    write_word(host, address, value);
    set_logic_word_flags(r, value);
}
} // namespace

FunctionResult cpu_b_compute_record_bounds_from_mode_table(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    const auto mode = read_byte(host, base + 0x3cU);
    r.address[0] = 0x0002a7f0U + static_cast<std::uint32_t>(mode) * 6U;
    r.data[0] = 0U;

    auto d0 = host.read_memory_word(kRegion, base + 0x12U, kWordMask);
    const auto first_axis_flags = read_byte(host, base + 1U);
    r.data[1] = 0U;
    const auto first = static_cast<std::uint16_t>(read_byte(host, r.address[0]++));
    r.data[1] = first;
    if ((first_axis_flags & 0x02U) == 0U) {
        d0 = subtract_word(r, d0, first);
        r.data[0] = d0;
        store_result(host, r, base + 0x2aU, d0);
        r.data[1] = 0U;
        const auto second = static_cast<std::uint16_t>(read_byte(host, r.address[0]++));
        r.data[1] = second;
        d0 = add_word(r, d0, second);
        r.data[0] = d0;
        store_result(host, r, base + 0x2cU, d0);
    } else {
        d0 = add_word(r, d0, first);
        r.data[0] = d0;
        store_result(host, r, base + 0x2cU, d0);
        r.data[1] = 0U;
        const auto second = static_cast<std::uint16_t>(read_byte(host, r.address[0]++));
        r.data[1] = second;
        d0 = subtract_word(r, d0, second);
        r.data[0] = d0;
        store_result(host, r, base + 0x2aU, d0);
    }

    d0 = host.read_memory_word(kRegion, base + 0x16U, kWordMask);
    r.data[0] = d0;
    const auto second_axis_flags = read_byte(host, base + 1U);
    const bool reversed = (second_axis_flags & 0x01U) != 0U;
    for (unsigned pair = 0; pair != 2U; ++pair) {
        const bool signed_offsets = pair != 0U || reversed;
        if (pair != 0U) {
            d0 = host.read_memory_word(kRegion, base + 0x1aU, kWordMask);
            r.data[0] = d0;
        }

        auto decode = [&](std::uint8_t raw) {
            return static_cast<std::uint16_t>(signed_offsets
                ? static_cast<std::int16_t>(static_cast<std::int8_t>(raw))
                : raw);
        };
        auto offset = decode(read_byte(host, r.address[0]++));
        r.data[1] = offset;
        if (!reversed) {
            d0 = subtract_word(r, d0, offset);
            r.data[0] = d0;
            store_result(host, r, base + (pair == 0U ? 0x2eU : 0x32U), d0);
        } else {
            d0 = add_word(r, d0, offset);
            r.data[0] = d0;
            store_result(host, r, base + (pair == 0U ? 0x30U : 0x34U), d0);
        }

        offset = decode(read_byte(host, r.address[0]++));
        r.data[1] = offset;
        if (!reversed) {
            d0 = add_word(r, d0, offset);
            r.data[0] = d0;
            store_result(host, r, base + (pair == 0U ? 0x30U : 0x34U), d0);
        } else {
            d0 = subtract_word(r, d0, offset);
            r.data[0] = d0;
            store_result(host, r, base + (pair == 0U ? 0x2eU : 0x32U), d0);
        }
    }

    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
