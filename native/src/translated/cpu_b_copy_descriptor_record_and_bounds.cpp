#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivate = 2U;
constexpr std::uint16_t kMask = 0xffffU;

void set_logic_long(CpuRegisters &r, std::uint32_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 8U;
    if (value == 0U) flags |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}
void push_return(ExecutionHost &h, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    h.write_memory_word(kPrivate, r.address[7], static_cast<std::uint16_t>(value >> 16U), kMask);
    h.write_memory_word(kPrivate, r.address[7] + 2U, static_cast<std::uint16_t>(value), kMask);
}
std::uint32_t pop_return(ExecutionHost &h, CpuRegisters &r)
{
    const auto hi = h.read_memory_word(kPrivate, r.address[7], kMask);
    const auto lo = h.read_memory_word(kPrivate, r.address[7] + 2U, kMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(hi) << 16U) | lo;
}
} // namespace

FunctionResult cpu_b_copy_descriptor_record_and_bounds(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;

    push_return(h, r, 0x00013eb6U);
    r.program_counter = 0x00013eccU;
    const auto child = h.call_function(279U, 1U, 0x72U, 2U,
        0x00013eb2U, 0x00013eccU, context);
    if (child.status != TranslationStatus::complete || child.control != 1U) return child;

    for (const auto destination : {0x2aU, 0x2eU, 0x32U}) {
        const auto hi = h.read_memory_word(kPrivate, r.address[0], kMask);
        const auto lo = h.read_memory_word(kPrivate, r.address[0] + 2U, kMask);
        r.address[0] += 4U;
        h.write_memory_word(kPrivate, r.address[6] + destination, hi, kMask);
        h.write_memory_word(kPrivate, r.address[6] + destination + 2U, lo, kMask);
        set_logic_long(r, (static_cast<std::uint32_t>(hi) << 16U) | lo);
    }
    const auto target = pop_return(h, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
