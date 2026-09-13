#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

ByteLocation locate_byte(std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U, static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}
std::uint8_t read_byte(ExecutionHost &host, ByteLocation location)
{
    return static_cast<std::uint8_t>(host.read_memory_word(kRegion, location.offset, location.mask) >> location.shift);
}
void write_byte(ExecutionHost &host, ByteLocation location, std::uint8_t value)
{
    host.write_memory_word(kRegion, location.offset, static_cast<std::uint16_t>(value) << location.shift, location.mask);
}
void set_logic_flags(CpuRegisters &r, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t f = static_cast<std::uint16_t>(r.status & 0x10U);
    if (value == 0U) f |= 4U;
    if ((value & sign) != 0U) f |= 8U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void set_add_word_flags(CpuRegisters &r, std::uint16_t a, std::uint16_t b, std::uint16_t v)
{
    std::uint16_t f = 0U;
    if (static_cast<std::uint32_t>(a) + b > 0xffffU) f |= 0x11U;
    if (((~(a ^ b)) & (a ^ v) & 0x8000U) != 0U) f |= 2U;
    if (v == 0U) f |= 4U;
    if ((v & 0x8000U) != 0U) f |= 8U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void set_sub_word_flags(CpuRegisters &r, std::uint16_t a, std::uint16_t b, std::uint16_t v)
{
    std::uint16_t f = 0U;
    if (a < b) f |= 0x11U;
    if (((a ^ b) & (a ^ v) & 0x8000U) != 0U) f |= 2U;
    if (v == 0U) f |= 4U;
    if ((v & 0x8000U) != 0U) f |= 8U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void set_compare_word_flags(CpuRegisters &r, std::uint16_t a, std::uint16_t b)
{
    const auto x = static_cast<std::uint16_t>(r.status & 0x10U);
    const auto v = static_cast<std::uint16_t>(a - b);
    set_sub_word_flags(r, a, b, v);
    r.status = static_cast<std::uint16_t>((r.status & ~0x10U) | x);
}
void set_neg_word_flags(CpuRegisters &r, std::uint16_t source, std::uint16_t result)
{
    std::uint16_t f = 0U;
    if (source != 0U) f |= 0x11U;
    if (source == 0x8000U) f |= 2U;
    if (result == 0U) f |= 4U;
    if ((result & 0x8000U) != 0U) f |= 8U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto hi = host.read_memory_word(kRegion, r.address[7], kWordMask);
    const auto lo = host.read_memory_word(kRegion, r.address[7] + 2U, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(hi) << 16U) | lo;
}
} // namespace

FunctionResult cpu_b_record_block_pointer_setup_variant_2(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    r.address[4] = 0x3400U;
    for (std::uint32_t offset : {0x7aU, 0x7cU, 0x7eU}) {
        host.write_memory_word(kRegion, r.address[4] + offset, 0x7fffU, kWordMask);
        set_logic_flags(r, 0x7fffU, 0x8000U);
    }
    r.data[0] = (r.data[0] & 0xffff0000U) | 0x000aU; set_logic_flags(r, 0x000aU, 0x8000U);
    r.data[1] = 0x10U; set_logic_flags(r, 0x10U, 0x80000000U);
    r.data[2] = (r.data[2] & 0xffff0000U) | 0x007aU; set_logic_flags(r, 0x007aU, 0x8000U);
    r.data[3] = 2U; set_logic_flags(r, 2U, 0x80000000U);

    for (unsigned iteration = 0; iteration != 3U; ++iteration) {
        const auto test = read_byte(host, locate_byte(r.address[4] + static_cast<std::uint16_t>(r.data[0])));
        set_logic_flags(r, test, 0x80U);
        if (test != 0U) {
            auto d4 = host.read_memory_word(kRegion, r.address[4] + static_cast<std::uint16_t>(r.data[1]), kWordMask);
            r.data[4] = (r.data[4] & 0xffff0000U) | d4; set_logic_flags(r, d4, 0x8000U);
            const auto reference = host.read_memory_word(kRegion, r.address[5] + 0x12U, kWordMask);
            auto value = static_cast<std::uint16_t>(d4 - reference); set_sub_word_flags(r, d4, reference, value);
            r.data[4] = (r.data[4] & 0xffff0000U) | value;
            if (static_cast<std::int16_t>(value) < 0) {
                const auto negated = static_cast<std::uint16_t>(0U - value); set_neg_word_flags(r, value, negated);
                value = negated; r.data[4] = (r.data[4] & 0xffff0000U) | value;
            }
            auto d1 = static_cast<std::uint16_t>(r.data[1]); auto next_d1 = static_cast<std::uint16_t>(d1 + 2U);
            set_add_word_flags(r, d1, 2U, next_d1); r.data[1] = (r.data[1] & 0xffff0000U) | next_d1;
            auto d5 = host.read_memory_word(kRegion, r.address[4] + next_d1, kWordMask);
            r.data[5] = (r.data[5] & 0xffff0000U) | d5; set_logic_flags(r, d5, 0x8000U);
            const auto second_reference = host.read_memory_word(
                kRegion, r.address[5] + 0x12U, kWordMask);
            auto difference = static_cast<std::uint16_t>(d5 - second_reference);
            set_sub_word_flags(r, d5, second_reference, difference);
            r.data[5] = (r.data[5] & 0xffff0000U) | difference;
            if (static_cast<std::int16_t>(difference) < 0) {
                const auto negated = static_cast<std::uint16_t>(0U - difference); set_neg_word_flags(r, difference, negated);
                difference = negated; r.data[5] = (r.data[5] & 0xffff0000U) | difference;
            }
            set_compare_word_flags(r, difference, value);
            if (static_cast<std::int16_t>(difference) < static_cast<std::int16_t>(value)) {
                difference = value; r.data[5] = (r.data[5] & 0xffff0000U) | difference; set_logic_flags(r, difference, 0x8000U);
            }
            host.write_memory_word(kRegion, r.address[4] + static_cast<std::uint16_t>(r.data[2]), difference, kWordMask);
            set_logic_flags(r, difference, 0x8000U);
            const auto before = static_cast<std::uint16_t>(r.data[1]); const auto after = static_cast<std::uint16_t>(before - 2U);
            set_sub_word_flags(r, before, 2U, after); r.data[1] = (r.data[1] & 0xffff0000U) | after;
        }
        auto before = static_cast<std::uint16_t>(r.data[0]); auto after = static_cast<std::uint16_t>(before + 1U);
        set_add_word_flags(r, before, 1U, after); r.data[0] = (r.data[0] & 0xffff0000U) | after;
        before = static_cast<std::uint16_t>(r.data[1]); after = static_cast<std::uint16_t>(before + 4U);
        set_add_word_flags(r, before, 4U, after); r.data[1] = (r.data[1] & 0xffff0000U) | after;
        before = static_cast<std::uint16_t>(r.data[2]); after = static_cast<std::uint16_t>(before + 2U);
        set_add_word_flags(r, before, 2U, after); r.data[2] = (r.data[2] & 0xffff0000U) | after;
        r.data[3] = (r.data[3] & 0xffff0000U) | static_cast<std::uint16_t>(static_cast<std::uint16_t>(r.data[3]) - 1U);
    }

    r.data[0] = 1U; set_logic_flags(r, 1U, 0x80000000U);
    auto selected = host.read_memory_word(kRegion, r.address[4] + 0x7aU, kWordMask);
    r.data[1] = (r.data[1] & 0xffff0000U) | selected; set_logic_flags(r, selected, 0x8000U);
    auto candidate = host.read_memory_word(kRegion, r.address[4] + 0x7cU, kWordMask);
    set_compare_word_flags(r, selected, candidate);
    if (static_cast<std::int16_t>(selected) > static_cast<std::int16_t>(candidate)) {
        r.data[0] = 2U; set_logic_flags(r, 2U, 0x80000000U);
        selected = host.read_memory_word(kRegion, r.address[4] + 0x7cU, kWordMask);
        r.data[1] = (r.data[1] & 0xffff0000U) | selected;
        set_logic_flags(r, selected, 0x8000U);
    }
    candidate = host.read_memory_word(kRegion, r.address[4] + 0x7eU, kWordMask);
    set_compare_word_flags(r, selected, candidate);
    if (static_cast<std::int16_t>(selected) > static_cast<std::int16_t>(candidate)) {
        r.data[0] = 3U; set_logic_flags(r, 3U, 0x80000000U);
        selected = host.read_memory_word(kRegion, r.address[4] + 0x7eU, kWordMask);
        r.data[1] = (r.data[1] & 0xffff0000U) | selected;
        set_logic_flags(r, selected, 0x8000U);
    }
    set_compare_word_flags(r, selected, 0x7fffU);
    if (selected == 0x7fffU) { r.data[0] = 0U; set_logic_flags(r, 0U, 0x80000000U); }
    const auto result = static_cast<std::uint8_t>(r.data[0]);
    write_byte(host, locate_byte(r.address[5] + 0x60U), result); set_logic_flags(r, result, 0x80U);
    const auto target = pop_return(host, r); r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
