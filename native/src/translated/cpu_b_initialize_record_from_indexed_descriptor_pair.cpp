#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address, kMask);
}

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto value = host.read_memory_word(kRegion, address & ~1U,
        odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? value : value >> 8U);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address, value, kMask);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    host.write_memory_word(kRegion, address & ~1U,
        static_cast<std::uint16_t>(value) << (odd ? 0U : 8U),
        odd ? 0x00ffU : 0xff00U);
}

void set_logic_word(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_logic_long(CpuRegisters &r, std::uint32_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_add_word(CpuRegisters &r, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU)
        flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void arithmetic_shift_left_word(CpuRegisters &r)
{
    auto value = static_cast<std::uint16_t>(r.data[0]);
    bool overflow = false;
    bool carry = false;
    for (unsigned index = 0; index != 2U; ++index) {
        const bool old_sign = (value & 0x8000U) != 0U;
        carry = old_sign;
        value = static_cast<std::uint16_t>(value << 1U);
        overflow = overflow || (old_sign != ((value & 0x8000U) != 0U));
    }
    std::uint16_t flags{};
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    if (carry) flags |= 0x0011U;
    r.data[0] = (r.data[0] & 0xffff0000U) | value;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto target = read_long(host, r.address[7]);
    r.address[7] += 4U;
    return target;
}
} // namespace

FunctionResult cpu_b_initialize_record_from_indexed_descriptor_pair(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    const auto a5 = r.address[5];
    const auto a6 = r.address[6];

    r.address[0] = read_long(host, r.address[3] + 0x10U);
    auto high = read_word(host, r.address[0]);
    auto low = read_word(host, r.address[0] + 2U);
    write_word(host, a6 + 2U, high);
    write_word(host, a6 + 4U, low);
    r.address[0] += 4U;
    set_logic_long(r, (static_cast<std::uint32_t>(high) << 16U) | low);

    const auto left = read_word(host, a5 + 0x66U);
    const auto right = read_word(host, r.address[0]);
    r.address[0] += 2U;
    auto value = static_cast<std::uint16_t>(left + right);
    r.data[0] = (r.data[0] & 0xffff0000U) | value;
    set_add_word(r, left, right, value);
    write_word(host, a6 + 8U, value);
    set_logic_word(r, value);

    auto byte = read_byte(host, r.address[0]++);
    write_byte(host, a6 + 0x3cU, byte);
    byte = read_byte(host, r.address[0]++);
    write_byte(host, a6 + 0x0bU, byte);
    high = read_word(host, r.address[0]);
    low = read_word(host, r.address[0] + 2U);
    write_word(host, a6 + 0x22U, high);
    write_word(host, a6 + 0x24U, low);
    r.address[0] += 4U;
    set_logic_long(r, (static_cast<std::uint32_t>(high) << 16U) | low);
    value = read_word(host, r.address[0]);
    r.address[0] += 2U;
    write_word(host, a6 + 0x3aU, value);
    set_logic_word(r, value);
    write_word(host, a6 + 0x36U, static_cast<std::uint16_t>(a5));
    set_logic_word(r, static_cast<std::uint16_t>(a5));
    (void)read_byte(host, a6 + 0x3fU);
    write_byte(host, a6 + 0x3fU, 0U);
    set_logic_word(r, 0U);
    (void)read_byte(host, a6 + 0x3dU);
    write_byte(host, a6 + 0x3dU, 0U);
    set_logic_word(r, 0U);

    value = read_word(host, a5 + 0x58U);
    r.data[0] = (r.data[0] & 0xffff0000U) | value;
    set_logic_word(r, value);
    write_word(host, a6 + 0x58U, value);
    set_logic_word(r, value);
    arithmetic_shift_left_word(r);
    const auto index = static_cast<std::int16_t>(r.data[0]);
    value = read_word(host, static_cast<std::uint32_t>(
        static_cast<std::int64_t>(r.address[0]) + index));
    write_word(host, a6 + 6U, value);
    set_logic_word(r, value);
    value = read_word(host, static_cast<std::uint32_t>(
        static_cast<std::int64_t>(r.address[0]) + index + 2));
    write_word(host, a6, value);
    set_logic_word(r, value);

    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
