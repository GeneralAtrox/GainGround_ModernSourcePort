#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;

void set_move_word_flags(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void push_long(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    host.write_memory_word(kRegion, r.address[7], static_cast<std::uint16_t>(value >> 16U), kMask);
    host.write_memory_word(kRegion, r.address[7] + 2U, static_cast<std::uint16_t>(value), kMask);
}

[[nodiscard]] std::uint32_t pop_long(ExecutionHost &host, CpuRegisters &r)
{
    const auto high = host.read_memory_word(kRegion, r.address[7], kMask);
    const auto low = host.read_memory_word(kRegion, r.address[7] + 2U, kMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_dispatch_record_helper_a86c(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    const auto value = host.read_memory_word(kRegion, 0x0c14U, kMask);
    r.data[2] = (r.data[2] & 0xffff0000U) | value;
    set_move_word_flags(r, value);
    r.address[0] = 0x000074e2U;
    push_long(host, r, 0x0000a87aU);
    r.program_counter = 0x00016154U;
    (void)host.call_function(297U, 1U, 0x72U, 2U,
        0x0000a874U, 0x00016154U, context);
    const auto return_address = pop_long(host, r);
    r.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}
} // namespace gain_ground::translated
