#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
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

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &h, CpuRegisters &r)
{
    const auto high = h.read_memory_word(kPrivateRegion, r.address[7], kWordMask);
    const auto low = h.read_memory_word(kPrivateRegion, r.address[7] + 2U, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void clear_long(ExecutionHost &h, CpuRegisters &r, std::uint32_t address)
{
    (void)h.read_memory_word(kPrivateRegion, address, kWordMask);
    (void)h.read_memory_word(kPrivateRegion, address + 2U, kWordMask);
    h.write_memory_word(kPrivateRegion, address + 2U, 0U, kWordMask);
    h.write_memory_word(kPrivateRegion, address, 0U, kWordMask);
    set_logic(r, 0U, 0x80000000U);
}

void write_word_post(ExecutionHost &h, CpuRegisters &r, std::uint16_t value)
{
    h.write_memory_word(kPrivateRegion, r.address[0], value, kWordMask);
    r.address[0] += 2U;
    set_logic(r, value, 0x8000U);
}

void write_long_post(ExecutionHost &h, CpuRegisters &r, std::uint32_t value)
{
    h.write_memory_word(kPrivateRegion, r.address[0], static_cast<std::uint16_t>(value >> 16U), kWordMask);
    h.write_memory_word(kPrivateRegion, r.address[0] + 2U, static_cast<std::uint16_t>(value), kWordMask);
    r.address[0] += 4U;
    set_logic(r, value, 0x80000000U);
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

FunctionResult cpu_b_initialize_overlay_and_descriptor_buffers(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;

    h.write_memory_word(kPrivateRegion, 0x00000834U, 2U, kWordMask);
    set_logic(r, 2U, 0x8000U);
    r.address[1] = 0x00024824U;
    const auto task_result = call_child(context, 126U, 0x00009f36U, 0x00008872U, 0x00009f3cU);
    if (task_result.status != TranslationStatus::complete || task_result.control != 1U)
        return task_result;
    const auto overlay_result = call_child(context, 123U, 0x00009f3cU, 0x00008654U, 0x00009f42U);
    if (overlay_result.status != TranslationStatus::complete || overlay_result.control != 1U)
        return overlay_result;

    r.address[0] = 0x00000d00U;
    r.data[0] = (r.data[0] & 0xffff0000U) | 0x003fU;
    set_logic(r, 0x003fU, 0x8000U);
    for (;;) {
        clear_long(h, r, r.address[0]);
        r.address[0] += 4U;
        const auto counter = static_cast<std::uint16_t>(r.data[0] - 1U);
        r.data[0] = (r.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }

    r.address[0] = 0x00000c1aU;
    r.data[0] = (r.data[0] & 0xffff0000U) | 9U;
    set_logic(r, 9U, 0x8000U);
    for (;;) {
        clear_long(h, r, r.address[0]);
        r.address[0] += 4U;
        const auto counter = static_cast<std::uint16_t>(r.data[0] - 1U);
        r.data[0] = (r.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }

    r.address[0] = 0x0000740cU;
    write_word_post(h, r, 0x01c0U);
    write_word_post(h, r, 0x0058U);
    write_word_post(h, r, 0x0810U);
    write_long_post(h, r, 0x00024e0eU);
    r.address[0] = 0x00007402U;
    write_word_post(h, r, 0x0020U);
    write_word_post(h, r, 0x00d8U);
    write_word_post(h, r, 0x0805U);
    write_long_post(h, r, 0x00024deaU);

    const auto return_address = pop_return(h, r);
    r.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
