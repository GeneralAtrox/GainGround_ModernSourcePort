#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void set_logic_flags(CpuRegisters &r,
    std::uint32_t value, std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_bit_zero(CpuRegisters &r, bool bit_was_set)
{
    if (bit_was_set) r.status = static_cast<std::uint16_t>(r.status & ~0x0004U);
    else r.status = static_cast<std::uint16_t>(r.status | 0x0004U);
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
    const auto high = host.read_memory_word(kRegion, r.address[7], kWordMask);
    const auto low = host.read_memory_word(kRegion, r.address[7] + 2U, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_begin_record_transition(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;

    const auto mode_address = r.address[5] + 1U;
    const auto old_mode = read_byte(host, mode_address);
    write_byte(host, mode_address, static_cast<std::uint8_t>(old_mode | 0x04U));
    set_bit_zero(r, (old_mode & 0x04U) != 0U);

    host.write_memory_word(kRegion, r.address[5] + 2U, 0x0002U, kWordMask);
    host.write_memory_word(kRegion, r.address[5] + 4U, 0x08b6U, kWordMask);
    set_logic_flags(r, 0x000208b6U, 0x80000000U, 0xffffffffU);

    const auto tas_address = r.address[5] + 0x3fU;
    const auto old_tas = read_byte(host, tas_address);
    write_byte(host, tas_address, static_cast<std::uint8_t>(old_tas | 0x80U));
    set_logic_flags(r, old_tas, 0x80U, 0xffU);

    host.write_memory_word(kRegion, r.address[5] + 0x10U, 0x3737U, kWordMask);
    set_logic_flags(r, 0x3737U, 0x8000U, 0xffffU);

    const auto old_state = read_byte(host, r.address[5]);
    write_byte(host, r.address[5], static_cast<std::uint8_t>(old_state & ~0x01U));
    set_bit_zero(r, (old_state & 0x01U) != 0U);

    push_return(host, r, 0x000208ceU);
    r.program_counter = 0x00015d24U;
    auto child = host.call_function(280U, 1U, 0x72U, 2U,
        0x000208c8U, 0x00015d24U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    push_return(host, r, 0x000208d4U);
    r.program_counter = 0x00015df2U;
    child = host.call_function(282U, 1U, 0x72U, 2U,
        0x000208ceU, 0x00015df2U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
