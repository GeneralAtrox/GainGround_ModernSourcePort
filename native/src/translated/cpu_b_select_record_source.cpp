#include "gain_ground/contract_types.h"
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivate = 2U, kShared = 3U, kWord = 0xffffU;
constexpr std::uint32_t kMask = 0x0003ffffU;
struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] constexpr ByteLocation locate(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {(address & kMask) & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}
[[nodiscard]] std::uint8_t read_byte(ExecutionHost &h, std::uint32_t address)
{
    const auto p = locate(address);
    return static_cast<std::uint8_t>(h.read_memory_word(kPrivate, p.offset, p.mask) >> p.shift);
}
void write_byte(ExecutionHost &h, std::uint32_t address, std::uint8_t value)
{
    const auto p = locate(address);
    h.write_memory_word(kPrivate, p.offset,
        static_cast<std::uint16_t>(value) << p.shift, p.mask);
}
[[nodiscard]] std::uint32_t read_long(ExecutionHost &h, std::uint32_t address)
{
    const auto region = address <= kMask ? kPrivate : kShared;
    const auto hi = h.read_memory_word(region, address & kMask, kWord);
    const auto lo = h.read_memory_word(region, (address + 2U) & kMask, kWord);
    return (static_cast<std::uint32_t>(hi) << 16U) | lo;
}
void logic(CpuRegisters &r, std::uint32_t value, std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}
void add_word(CpuRegisters &r, std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}
[[nodiscard]] std::uint16_t shift_left_three(CpuRegisters &r, std::uint16_t value)
{
    auto result = value;
    bool carry{};
    bool overflow{};
    for (unsigned i = 0; i != 3U; ++i) {
        const bool old_sign = (result & 0x8000U) != 0U;
        carry = old_sign;
        result = static_cast<std::uint16_t>(result << 1U);
        overflow = overflow || (old_sign != ((result & 0x8000U) != 0U));
    }
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if (overflow) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    return result;
}
void bit_zero(CpuRegisters &r, bool set)
{
    if (set) r.status = static_cast<std::uint16_t>(r.status & ~0x0004U);
    else r.status = static_cast<std::uint16_t>(r.status | 0x0004U);
}
[[nodiscard]] std::uint32_t pop(ExecutionHost &h, CpuRegisters &r)
{
    const auto hi = h.read_memory_word(kPrivate, r.address[7], kWord);
    const auto lo = h.read_memory_word(kPrivate, r.address[7] + 2U, kWord);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(hi) << 16U) | lo;
}
} // namespace

FunctionResult cpu_b_select_record_source(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;

    const auto selector = read_byte(h, r.address[5] + 0x5aU);
    logic(r, selector, 0x80U, 0xffU);
    if (static_cast<std::int8_t>(selector) < 0) {
        r.address[0] = 0x00010652U;
        r.program_counter = 0x0000fcd2U;
        return h.call_function(196U, 1U, 0x72U, 1U,
            0x0000fcb8U, 0x0000fcd2U, context);
    }

    r.address[0] = read_long(h, r.address[5] + 0x4cU);
    r.address[0] = read_long(h, r.address[0] + 2U);
    const auto index = h.read_memory_word(kPrivate, r.address[5] + 0x52U, kWord);
    r.data[0] = (r.data[0] & 0xffff0000U) | index;
    logic(r, index, 0x8000U, 0xffffU);
    if (static_cast<std::int16_t>(index) < 0) {
        const auto state = read_byte(h, r.address[5]);
        write_byte(h, r.address[5], static_cast<std::uint8_t>(state & ~0x01U));
        bit_zero(r, (state & 0x01U) != 0U);
        const auto target = pop(h, r);
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    }

    auto d0 = shift_left_three(r, index);
    r.data[0] = (r.data[0] & 0xffff0000U) | d0;
    r.data[1] = (r.data[1] & 0xffff0000U) | d0;
    logic(r, d0, 0x8000U, 0xffffU);
    const auto doubled = static_cast<std::uint16_t>(d0 + d0);
    add_word(r, d0, d0, doubled);
    r.data[0] = (r.data[0] & 0xffff0000U) | doubled;
    const auto tripled = static_cast<std::uint16_t>(doubled + d0);
    add_word(r, doubled, d0, tripled);
    r.data[0] = (r.data[0] & 0xffff0000U) | tripled;
    r.address[0] += static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(tripled)));

    r.program_counter = 0x0000fcd2U;
    return h.call_function(196U, 1U, 0x72U, 0U,
        0x0000fcd0U, 0x0000fcd2U, context);
}
} // namespace gain_ground::translated
