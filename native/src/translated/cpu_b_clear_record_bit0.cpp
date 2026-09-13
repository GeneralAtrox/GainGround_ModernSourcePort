#include "gain_ground/contract_types.h"
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] constexpr ByteLocation locate(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}
[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto p = locate(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, p.offset, p.mask) >> p.shift);
}
void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto p = locate(address);
    host.write_memory_word(kRegion, p.offset,
        static_cast<std::uint16_t>(value) << p.shift, p.mask);
}
void set_logic_byte(CpuRegisters &r, std::uint8_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}
void set_bit_zero(CpuRegisters &r, bool bit_was_set)
{
    if (bit_was_set) r.status = static_cast<std::uint16_t>(r.status & ~0x0004U);
    else r.status = static_cast<std::uint16_t>(r.status | 0x0004U);
}
} // namespace

FunctionResult cpu_b_clear_record_bit0(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;

    const auto state = read_byte(host, r.address[5]);
    write_byte(host, r.address[5], static_cast<std::uint8_t>(state & ~0x01U));
    set_bit_zero(r, (state & 0x01U) != 0U);

    const auto table = host.read_memory_word(kRegion, r.address[5] + 0x72U, kWordMask);
    r.address[6] = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(table)));
    for (const auto offset : {0U, 0x80U, 0x100U}) {
        write_byte(host, r.address[6] + offset, 0x80U);
        set_logic_byte(r, 0x80U);
    }

    r.program_counter = 0x0000fcaeU;
    return host.call_function(195U, 1U, 0x72U, 0U,
        0x0000fca8U, 0x0000fcaeU, context);
}
} // namespace gain_ground::translated
