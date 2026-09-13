#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWindowRegion = 6U;
constexpr std::uint16_t kWordMask = 0xffffU;

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

void push_return(ExecutionHost &h, CpuRegisters &r, std::uint32_t pc)
{
    r.address[7] -= 4U;
    h.write_memory_word(kPrivateRegion, r.address[7], static_cast<std::uint16_t>(pc >> 16U), kWordMask);
    h.write_memory_word(kPrivateRegion, r.address[7] + 2U, static_cast<std::uint16_t>(pc), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &h, CpuRegisters &r)
{
    const auto high = h.read_memory_word(kPrivateRegion, r.address[7], kWordMask);
    const auto low = h.read_memory_word(kPrivateRegion, r.address[7] + 2U, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void clear_long(ExecutionHost &h, CpuRegisters &r, std::uint16_t region, std::uint32_t offset)
{
    (void)h.read_memory_word(region, offset, kWordMask);
    (void)h.read_memory_word(region, offset + 2U, kWordMask);
    h.write_memory_word(region, offset + 2U, 0U, kWordMask);
    h.write_memory_word(region, offset, 0U, kWordMask);
    set_logic_long(r, 0U);
}

[[nodiscard]] FunctionResult call_child(FunctionContext &context, std::uint32_t id,
    std::uint32_t callsite, std::uint32_t target, std::uint32_t return_pc)
{
    auto &h = *context.host;
    push_return(h, context.registers, return_pc);
    context.registers.program_counter = target;
    return h.call_function(id, 1U, 0x72U, 2U, callsite, target, context);
}
} // namespace

FunctionResult cpu_b_initialize_window_and_object_tables(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;

    const auto entry_pc = r.program_counter;
    if (entry_pc != 0x00009ce0U && entry_pc != 0x00009ce4U && entry_pc != 0x00009ce8U) {
        const auto copy_result = call_child(context, 132U, 0x00009cdcU, 0x00009d0aU, 0x00009ce0U);
        if (copy_result.status != TranslationStatus::complete || copy_result.control != 1U)
            return copy_result;
    }
    if (entry_pc != 0x00009ce4U && entry_pc != 0x00009ce8U) {
        const auto window_result = call_child(context, 133U, 0x00009ce0U, 0x00009d44U, 0x00009ce4U);
        if (window_result.status != TranslationStatus::complete || window_result.control != 1U)
            return window_result;
    }
    if (entry_pc != 0x00009ce8U) {
        const auto batch_result = call_child(context, 135U, 0x00009ce4U, 0x00009ddcU, 0x00009ce8U);
        if (batch_result.status != TranslationStatus::complete || batch_result.control != 1U)
            return batch_result;
    }

    r.address[0] = 0x0000050eU;
    clear_long(h, r, kPrivateRegion, r.address[0]);
    r.address[0] += 4U;
    h.write_memory_word(kPrivateRegion, r.address[0], 0x8000U, kWordMask);
    r.address[0] += 2U;
    set_logic_word(r, 0x8000U);
    (void)h.read_memory_word(kPrivateRegion, r.address[0], kWordMask);
    h.write_memory_word(kPrivateRegion, r.address[0], 0U, kWordMask);
    r.address[0] += 2U;
    set_logic_word(r, 0U);
    clear_long(h, r, kPrivateRegion, r.address[0]);
    r.address[0] += 4U;
    clear_long(h, r, kPrivateRegion, r.address[0]);
    r.address[0] += 4U;

    r.address[0] = 0x00208800U;
    r.data[0] = (r.data[0] & 0xffff0000U) | 0x00ffU;
    set_logic_word(r, 0x00ffU);
    for (;;) {
        clear_long(h, r, kWindowRegion, r.address[0] - 0x00208000U);
        r.address[0] += 4U;
        const auto counter = static_cast<std::uint16_t>(r.data[0] - 1U);
        r.data[0] = (r.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }

    const auto return_address = pop_return(h, r);
    r.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
