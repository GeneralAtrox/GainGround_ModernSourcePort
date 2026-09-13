#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address, kWordMask);
}

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = read_word(host, address);
    const auto low = read_word(host, address + 2U);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address, value, kWordMask);
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto value = host.read_memory_word(kRegion, address & ~1U,
        odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? value : value >> 8U);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    host.write_memory_word(kRegion, address & ~1U,
        static_cast<std::uint16_t>(value) << (odd ? 0U : 8U),
        odd ? 0x00ffU : 0xff00U);
}

void set_logic_byte(CpuRegisters &r, std::uint8_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_logic_word(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void arithmetic_shift_left_word(CpuRegisters &r, unsigned count)
{
    auto value = static_cast<std::uint16_t>(r.data[0]);
    bool overflow = false;
    bool carry = false;
    for (unsigned index = 0; index != count; ++index) {
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

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    write_word(host, r.address[7], static_cast<std::uint16_t>(value >> 16U));
    write_word(host, r.address[7] + 2U, static_cast<std::uint16_t>(value));
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto value = read_long(host, r.address[7]);
    r.address[7] += 4U;
    return value;
}

FunctionResult call_update(FunctionContext &context, std::uint32_t callsite,
    std::uint32_t continuation)
{
    auto &host = *context.host;
    auto &r = context.registers;
    r.address[1] = read_long(host, r.address[3] + 0x20U);
    push_return(host, r, continuation);
    r.program_counter = 0x00010c9aU;
    return host.call_function(223U, 1U, 0x72U, 2U,
        callsite, 0x00010c9aU, context);
}

FunctionResult finish(FunctionContext &context)
{
    auto &host = *context.host;
    auto &r = context.registers;
    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_set_record_phase_from_parent_and_update(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;

    const auto phase = read_word(host, r.address[5] + 0x58U);
    r.data[0] = (r.data[0] & 0xffff0000U) | phase;
    set_logic_word(r, phase);
    write_byte(host, r.address[6] + 0x58U,
        static_cast<std::uint8_t>(phase));
    set_logic_byte(r, static_cast<std::uint8_t>(phase));
    (void)read_byte(host, r.address[6] + 0x59U);
    write_byte(host, r.address[6] + 0x59U, 0U);
    set_logic_byte(r, 0U);
    arithmetic_shift_left_word(r, 2U);

    const auto child = call_update(context, 0x00011352U, 0x00011356U);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;
    return finish(context);
}

} // namespace gain_ground::translated
