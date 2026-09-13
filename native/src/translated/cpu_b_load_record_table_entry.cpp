#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {(address & kAddressMask) & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(address <= kAddressMask ? kPrivateRegion : kSharedRegion,
        address & kAddressMask, kWordMask);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        address <= kAddressMask ? kPrivateRegion : kSharedRegion,
        location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kPrivateRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kPrivateRegion, address & kAddressMask,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, (address + 2U) & kAddressMask,
        static_cast<std::uint16_t>(value), kWordMask);
}

void set_logic_flags(CpuRegisters &r, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_add_flags(CpuRegisters &r, std::uint32_t left,
    std::uint32_t right, std::uint32_t result, std::uint32_t sign,
    std::uint32_t mask)
{
    std::uint16_t flags{};
    if (left + right > mask) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & sign) != 0U) flags |= 0x0002U;
    if ((result & sign) != 0U) flags |= 0x0008U;
    if ((result & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint16_t asl_word(CpuRegisters &r,
    std::uint16_t value, unsigned count)
{
    bool carry{};
    bool overflow{};
    auto result = value;
    for (unsigned i = 0; i != count; ++i) {
        const bool old_sign = (result & 0x8000U) != 0U;
        carry = old_sign;
        result = static_cast<std::uint16_t>(result << 1U);
        overflow |= old_sign != ((result & 0x8000U) != 0U);
    }
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if (overflow) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    return result;
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    write_long(host, r.address[7], value);
}

[[nodiscard]] FunctionResult finish(FunctionContext &context)
{
    auto &r = context.registers;
    const auto target = read_long(*context.host, r.address[7]);
    r.address[7] += 4U;
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_load_record_table_entry(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    if (r.program_counter != 0x00013998U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};

    r.data[0] = 0U;
    set_logic_flags(r, 0U, 0x80000000U, 0xffffffffU);
    const auto type = read_byte(host, r.address[5] + 0x4bU);
    r.data[0] = type;
    set_logic_flags(r, type, 0x80U, 0xffU);
    auto d0w = asl_word(r, static_cast<std::uint16_t>(r.data[0]), 4U);
    r.data[0] = d0w;
    r.address[0] = static_cast<std::uint32_t>(0x00026f1cU
        + static_cast<std::int16_t>(d0w));
    r.address[0] = read_long(host, r.address[0] + 2U);

    d0w = read_word(host, r.address[5] + 0x52U);
    r.data[0] = (r.data[0] & 0xffff0000U) | d0w;
    set_logic_flags(r, d0w, 0x8000U, 0xffffU);
    if (static_cast<std::int16_t>(d0w) < 0) return finish(context);

    d0w = asl_word(r, d0w, 3U);
    r.data[0] = (r.data[0] & 0xffff0000U) | d0w;
    r.data[1] = (r.data[1] & 0xffff0000U) | d0w;
    set_logic_flags(r, d0w, 0x8000U, 0xffffU);
    const auto doubled = static_cast<std::uint16_t>(d0w + d0w);
    set_add_flags(r, d0w, d0w, doubled, 0x8000U, 0xffffU);
    const auto offset = static_cast<std::uint16_t>(doubled + d0w);
    set_add_flags(r, doubled, d0w, offset, 0x8000U, 0xffffU);
    r.data[0] = (r.data[0] & 0xffff0000U) | offset;
    r.address[0] = static_cast<std::uint32_t>(r.address[0]
        + static_cast<std::int16_t>(offset));
    write_long(host, r.address[5] + 0x54U, r.address[0]);
    set_logic_flags(r, r.address[0], 0x80000000U, 0xffffffffU);

    const auto marker = read_word(host, r.address[0]);
    r.address[0] += 2U;
    host.write_memory_word(kPrivateRegion, r.address[5] + 6U, marker, kWordMask);
    set_logic_flags(r, marker, 0x8000U, 0xffffU);
    if (static_cast<std::int16_t>(marker) < 0) return finish(context);

    r.address[0] += 2U;
    d0w = read_word(host, r.address[5] + 0x6aU);
    r.data[0] = (r.data[0] & 0xffff0000U) | d0w;
    set_logic_flags(r, d0w, 0x8000U, 0xffffU);
    const auto delta = read_byte(host, r.address[0]++);
    const auto sum = static_cast<std::uint8_t>(d0w + delta);
    r.data[0] = (r.data[0] & 0xffffff00U) | sum;
    set_add_flags(r, static_cast<std::uint8_t>(d0w), delta, sum, 0x80U, 0xffU);
    host.write_memory_word(kPrivateRegion, r.address[5] + 8U,
        static_cast<std::uint16_t>(r.data[0]), kWordMask);
    set_logic_flags(r, static_cast<std::uint16_t>(r.data[0]), 0x8000U, 0xffffU);

    auto attribute = read_byte(host, r.address[0]++);
    r.data[0] = (r.data[0] & 0xffffff00U) | attribute;
    set_logic_flags(r, attribute, 0x80U, 0xffU);
    attribute = static_cast<std::uint8_t>(attribute | 0x04U);
    r.data[0] = (r.data[0] & 0xffffff00U) | attribute;
    set_logic_flags(r, attribute, 0x80U, 0xffU);
    write_byte(host, r.address[5] + 1U, attribute);
    set_logic_flags(r, attribute, 0x80U, 0xffU);

    push_return(host, r, 0x000139e2U);
    r.program_counter = 0x00015d24U;
    auto child = host.call_function(280U, 1U, 0x72U, 2U,
        0x000139dcU, 0x00015d24U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;
    push_return(host, r, 0x000139e8U);
    r.program_counter = 0x00015df2U;
    child = host.call_function(282U, 1U, 0x72U, 2U,
        0x000139e2U, 0x00015df2U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;
    return finish(context);
}

} // namespace gain_ground::translated
