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
[[nodiscard]] std::uint8_t read_byte(ExecutionHost &h, std::uint16_t region, std::uint32_t address)
{
    const auto p = locate(address);
    return static_cast<std::uint8_t>(h.read_memory_word(region, p.offset, p.mask) >> p.shift);
}
[[nodiscard]] std::uint8_t read_stream(ExecutionHost &h, std::uint32_t address)
{
    return read_byte(h, address <= kMask ? kPrivate : kShared, address);
}
void write_byte(ExecutionHost &h, std::uint32_t address, std::uint8_t value)
{
    const auto p = locate(address);
    h.write_memory_word(kPrivate, p.offset,
        static_cast<std::uint16_t>(value) << p.shift, p.mask);
}
void logic(CpuRegisters &r, std::uint32_t value, std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}
void add_byte(CpuRegisters &r, std::uint8_t left, std::uint8_t right, std::uint8_t result)
{
    std::uint16_t flags{};
    if (static_cast<unsigned>(left) + right > 0xffU) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}
void bit_zero(CpuRegisters &r, bool set)
{
    if (set) r.status = static_cast<std::uint16_t>(r.status & ~0x0004U);
    else r.status = static_cast<std::uint16_t>(r.status | 0x0004U);
}
void push(ExecutionHost &h, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    h.write_memory_word(kPrivate, r.address[7], static_cast<std::uint16_t>(value >> 16U), kWord);
    h.write_memory_word(kPrivate, r.address[7] + 2U, static_cast<std::uint16_t>(value), kWord);
}
[[nodiscard]] std::uint32_t pop(ExecutionHost &h, CpuRegisters &r)
{
    const auto hi = h.read_memory_word(kPrivate, r.address[7], kWord);
    const auto lo = h.read_memory_word(kPrivate, r.address[7] + 2U, kWord);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(hi) << 16U) | lo;
}
} // namespace

FunctionResult cpu_b_load_record_header(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;

    h.write_memory_word(kPrivate, r.address[5] + 0x54U,
        static_cast<std::uint16_t>(r.address[0] >> 16U), kWord);
    h.write_memory_word(kPrivate, r.address[5] + 0x56U,
        static_cast<std::uint16_t>(r.address[0]), kWord);
    logic(r, r.address[0], 0x80000000U, 0xffffffffU);

    const auto stream_region = r.address[0] <= kMask ? kPrivate : kShared;
    const auto marker = h.read_memory_word(stream_region, r.address[0] & kMask, kWord);
    r.address[0] += 2U;
    h.write_memory_word(kPrivate, r.address[5] + 6U, marker, kWord);
    logic(r, marker, 0x8000U, 0xffffU);
    if (static_cast<std::int16_t>(marker) < 0) {
        const auto state = read_byte(h, kPrivate, r.address[5]);
        write_byte(h, r.address[5], static_cast<std::uint8_t>(state & ~0x01U));
        bit_zero(r, (state & 0x01U) != 0U);
        const auto target = pop(h, r);
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    }

    auto value = read_stream(h, r.address[0]++);
    write_byte(h, r.address[5] + 0x10U, value); logic(r, value, 0x80U, 0xffU);
    value = read_stream(h, r.address[0]++);
    write_byte(h, r.address[5] + 0x11U, value); logic(r, value, 0x80U, 0xffU);

    const auto attribute = h.read_memory_word(kPrivate, r.address[5] + 0x66U, kWord);
    r.data[0] = (r.data[0] & 0xffff0000U) | attribute;
    logic(r, attribute, 0x8000U, 0xffffU);
    auto low = static_cast<std::uint8_t>(r.data[0]);
    value = read_byte(h, kPrivate, r.address[5] + 0x5bU);
    auto sum = static_cast<std::uint8_t>(low + value);
    r.data[0] = (r.data[0] & 0xffffff00U) | sum; add_byte(r, low, value, sum);
    low = sum; value = read_stream(h, r.address[0]++);
    sum = static_cast<std::uint8_t>(low + value);
    r.data[0] = (r.data[0] & 0xffffff00U) | sum; add_byte(r, low, value, sum);
    h.write_memory_word(kPrivate, r.address[5] + 8U,
        static_cast<std::uint16_t>(r.data[0]), kWord);
    logic(r, static_cast<std::uint16_t>(r.data[0]), 0x8000U, 0xffffU);

    value = read_stream(h, r.address[0]++);
    r.data[0] = (r.data[0] & 0xffffff00U) | value; logic(r, value, 0x80U, 0xffU);
    value = static_cast<std::uint8_t>(value | 4U);
    r.data[0] = (r.data[0] & 0xffffff00U) | value; logic(r, value, 0x80U, 0xffU);
    write_byte(h, r.address[5] + 1U, value); logic(r, value, 0x80U, 0xffU);

    push(h, r, 0x0000fd02U);
    r.program_counter = 0x00015d24U;
    auto child = h.call_function(280U, 1U, 0x72U, 2U,
        0x0000fcfcU, 0x00015d24U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U) return child;
    push(h, r, 0x0000fd08U);
    r.program_counter = 0x00015df2U;
    child = h.call_function(282U, 1U, 0x72U, 2U,
        0x0000fd02U, 0x00015df2U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U) return child;
    const auto target = pop(h, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace gain_ground::translated
