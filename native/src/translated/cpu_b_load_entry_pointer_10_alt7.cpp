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

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address, value, kMask);
}

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto value = host.read_memory_word(kRegion, address & ~1U, mask);
    return static_cast<std::uint8_t>(value >> (odd ? 0U : 8U));
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    host.write_memory_word(kRegion, address & ~1U,
        static_cast<std::uint16_t>(value) << (odd ? 0U : 8U), mask);
}

void set_logic(CpuRegisters &registers, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_add_word(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void shift_left_two(CpuRegisters &registers)
{
    auto value = static_cast<std::uint16_t>(registers.data[0]);
    bool overflow{};
    bool carry{};
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
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_b_load_entry_pointer_10_alt7(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;

    r.address[0] = read_long(host, r.address[3] + 0x10U);
    const auto descriptor = read_long(host, r.address[0]);
    r.address[0] += 4U;
    write_long(host, r.address[6] + 2U, descriptor);
    set_logic(r, descriptor, 0x80000000U);

    auto d0 = read_word(host, r.address[5] + 0x66U);
    r.data[0] = (r.data[0] & 0xffff0000U) | d0;
    set_logic(r, d0, 0x8000U);
    const auto adjustment = read_word(host, r.address[0]);
    r.address[0] += 2U;
    const auto sum = static_cast<std::uint16_t>(d0 + adjustment);
    r.data[0] = (r.data[0] & 0xffff0000U) | sum;
    set_add_word(r, d0, adjustment, sum);
    write_word(host, r.address[6] + 8U, sum);
    set_logic(r, sum, 0x8000U);

    const auto byte_3c = read_byte(host, r.address[0]++);
    write_byte(host, r.address[6] + 0x3cU, byte_3c);
    set_logic(r, byte_3c, 0x80U);
    const auto byte_0b = read_byte(host, r.address[0]++);
    write_byte(host, r.address[6] + 0x0bU, byte_0b);
    set_logic(r, byte_0b, 0x80U);
    const auto field_3a = read_word(host, r.address[0]);
    r.address[0] += 2U;
    write_word(host, r.address[6] + 0x3aU, field_3a);
    set_logic(r, field_3a, 0x8000U);

    const auto a5_word = static_cast<std::uint16_t>(r.address[5]);
    write_word(host, r.address[6] + 0x36U, a5_word);
    set_logic(r, a5_word, 0x8000U);
    (void)read_byte(host, r.address[6] + 0x3fU);
    write_byte(host, r.address[6] + 0x3fU, 0U);
    set_logic(r, 0U, 0x80U);
    (void)read_byte(host, r.address[6] + 0x3dU);
    write_byte(host, r.address[6] + 0x3dU, 0U);
    set_logic(r, 0U, 0x80U);

    d0 = read_word(host, r.address[5] + 0x58U);
    r.data[0] = (r.data[0] & 0xffff0000U) | d0;
    set_logic(r, d0, 0x8000U);
    write_word(host, r.address[6] + 0x58U, d0);
    set_logic(r, d0, 0x8000U);
    shift_left_two(r);
    const auto displacement = static_cast<std::int16_t>(static_cast<std::uint16_t>(r.data[0]));
    const auto table = static_cast<std::uint32_t>(r.address[0] + static_cast<std::int32_t>(displacement));
    const auto field_06 = read_word(host, table);
    write_word(host, r.address[6] + 6U, field_06);
    set_logic(r, field_06, 0x8000U);
    const auto field_00 = read_word(host, table + 2U);
    write_word(host, r.address[6], field_00);
    set_logic(r, field_00, 0x8000U);

    const auto target = read_long(host, r.address[7]);
    r.address[7] += 4U;
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
