#include "gain_ground/contract_types.h"
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWindowRegion = 7U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic(CpuRegisters &r, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}
void push_return(ExecutionHost &h, CpuRegisters &r, std::uint32_t pc)
{
    r.address[7] -= 4U;
    h.write_memory_word(kPrivateRegion, r.address[7], static_cast<std::uint16_t>(pc >> 16U), kWordMask);
    h.write_memory_word(kPrivateRegion, r.address[7] + 2U, static_cast<std::uint16_t>(pc), kWordMask);
}
std::uint32_t pop_return(ExecutionHost &h, CpuRegisters &r)
{
    const auto high = h.read_memory_word(kPrivateRegion, r.address[7], kWordMask);
    const auto low = h.read_memory_word(kPrivateRegion, r.address[7] + 2U, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
FunctionResult call_child(FunctionContext &context, std::uint32_t id,
    std::uint32_t callsite, std::uint32_t target, std::uint32_t return_pc)
{
    push_return(*context.host, context.registers, return_pc);
    context.registers.program_counter = target;
    return context.host->call_function(id, 1U, 0x72U, 2U, callsite, target, context);
}
void write_private_long(ExecutionHost &h, CpuRegisters &r, std::uint32_t value)
{
    h.write_memory_word(kPrivateRegion, r.address[0], static_cast<std::uint16_t>(value >> 16U), kWordMask);
    h.write_memory_word(kPrivateRegion, r.address[0] + 2U, static_cast<std::uint16_t>(value), kWordMask);
    r.address[0] += 4U;
    set_logic(r, value, 0x80000000U);
}
void clear_private_long(ExecutionHost &h, CpuRegisters &r)
{
    (void)h.read_memory_word(kPrivateRegion, r.address[0], kWordMask);
    (void)h.read_memory_word(kPrivateRegion, r.address[0] + 2U, kWordMask);
    h.write_memory_word(kPrivateRegion, r.address[0] + 2U, 0U, kWordMask);
    h.write_memory_word(kPrivateRegion, r.address[0], 0U, kWordMask);
    r.address[0] += 4U;
    set_logic(r, 0U, 0x80000000U);
}
void write_window_long(ExecutionHost &h, CpuRegisters &r, std::uint32_t value)
{
    const auto offset = r.address[0] & 0x00003fffU;
    h.write_memory_word(kWindowRegion, offset, static_cast<std::uint16_t>(value >> 16U), kWordMask);
    h.write_memory_word(kWindowRegion, offset + 2U, static_cast<std::uint16_t>(value), kWordMask);
    r.address[0] += 4U;
    set_logic(r, value, 0x80000000U);
}
} // namespace

FunctionResult cpu_b_initialize_object_and_video_tables(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;
    const auto entry_pc = r.program_counter;

    if (entry_pc != 0x0000a5d8U) {
        r.address[0] = 0x00202000U;
        r.address[1] = 0x00008960U;
        r.data[1] = 0x0000002fU;
        set_logic(r, r.data[1], 0x80000000U);
        for (;;) {
            const auto child = call_child(context, 157U, 0x0000a5beU, 0x0000dc5eU, 0x0000a5c2U);
            if (child.status != TranslationStatus::complete || child.control != 1U)
                return child;
            const auto counter = static_cast<std::uint16_t>(r.data[1] - 1U);
            r.data[1] = (r.data[1] & 0xffff0000U) | counter;
            if (counter == 0xffffU) break;
        }
        r.address[4] = 0x000252ceU;
        r.address[3] = 0x00024ef6U;
        const auto grid = call_child(context, 159U, 0x0000a5d2U, 0x0000dc94U, 0x0000a5d8U);
        if (grid.status != TranslationStatus::complete || grid.control != 1U)
            return grid;
    }

    r.address[0] = 0x0000050eU;
    write_private_long(h, r, 0x00008000U);
    clear_private_long(h, r);
    clear_private_long(h, r);
    clear_private_long(h, r);
    r.data[0] = 0x03ffffffU;
    set_logic(r, r.data[0], 0x80000000U);
    r.data[1] = 0xffffffffU;
    set_logic(r, r.data[1], 0x80000000U);
    r.data[2] = (r.data[2] & 0xffff0000U) | 0x017fU;
    set_logic(r, 0x017fU, 0x8000U);
    r.address[0] = 0x0020c000U;
    for (;;) {
        write_window_long(h, r, r.data[0]);
        write_window_long(h, r, r.data[1]);
        const auto counter = static_cast<std::uint16_t>(r.data[2] - 1U);
        r.data[2] = (r.data[2] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }
    r.address[0] = 0x0020d000U;
    r.data[0] = (r.data[0] & 0xffff0000U) | 0x02ffU;
    set_logic(r, 0x02ffU, 0x8000U);
    for (;;) {
        write_window_long(h, r, 0xffffffffU);
        const auto counter = static_cast<std::uint16_t>(r.data[0] - 1U);
        r.data[0] = (r.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }
    const auto return_address = pop_return(h, r);
    r.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}
} // namespace gain_ground::translated
