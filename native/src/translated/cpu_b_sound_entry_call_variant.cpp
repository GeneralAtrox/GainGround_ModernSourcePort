#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivate = 2U;
constexpr std::uint16_t kMask = 0xffffU;

void set_logic_long(CpuRegisters &r, std::uint32_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void push_address(ExecutionHost &h, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    h.write_memory_word(kPrivate, r.address[7] + 2U, static_cast<std::uint16_t>(value), kMask);
    h.write_memory_word(kPrivate, r.address[7], static_cast<std::uint16_t>(value >> 16U), kMask);
    set_logic_long(r, value);
}

std::uint32_t pop_address(ExecutionHost &h, CpuRegisters &r)
{
    const auto low = h.read_memory_word(kPrivate, r.address[7] + 2U, kMask);
    const auto high = h.read_memory_word(kPrivate, r.address[7], kMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void push_return(ExecutionHost &h, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    h.write_memory_word(kPrivate, r.address[7], static_cast<std::uint16_t>(value >> 16U), kMask);
    h.write_memory_word(kPrivate, r.address[7] + 2U, static_cast<std::uint16_t>(value), kMask);
}

std::uint32_t pop_return(ExecutionHost &h, CpuRegisters &r)
{
    const auto high = h.read_memory_word(kPrivate, r.address[7], kMask);
    const auto low = h.read_memory_word(kPrivate, r.address[7] + 2U, kMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_sound_entry_call_variant(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;

    push_address(h, r, r.address[5]);
    push_address(h, r, r.address[6]);
    r.address[6] = 0xffffc000U;
    r.address[5] = 0x00fb0000U;
    push_return(h, r, 0x0001704eU);
    r.program_counter = 0x00017072U;
    const auto child = h.call_function(313U, 1U, 0x72U, 2U,
        0x0001704aU, 0x00017072U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U) return child;

    r.address[6] = pop_address(h, r);
    r.address[5] = pop_address(h, r);
    const auto target = pop_return(h, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
