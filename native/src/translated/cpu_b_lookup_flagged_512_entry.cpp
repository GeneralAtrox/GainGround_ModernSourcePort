#include "gain_ground/contract_types.h"
#include "cpu_b_lookup_flagged_512_entry_common.h"
#include "cpu_b_normalize_wrapped_index_common.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & kAddressMask, kMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address & kAddressMask, value, kMask);
}

void logic(CpuRegisters &r, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = static_cast<std::uint16_t>(r.status & 0x10U);
    if ((value & sign) != 0U) flags |= 8U;
    if (value == 0U) flags |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}

void sub_word(CpuRegisters &r, std::uint16_t left, std::uint16_t right,
    std::uint16_t result, bool compare)
{
    std::uint16_t flags = compare ? static_cast<std::uint16_t>(r.status & 0x10U) : 0U;
    if ((result & 0x8000U) != 0U) flags |= 8U;
    if (result == 0U) flags |= 4U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 2U;
    if (left < right) flags |= compare ? 1U : 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}

void negate_word(CpuRegisters &r, unsigned index)
{
    const auto operand = static_cast<std::uint16_t>(r.data[index]);
    const auto result = static_cast<std::uint16_t>(0U - operand);
    r.data[index] = (r.data[index] & 0xffff0000U) | result;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 8U;
    if (result == 0U) flags |= 4U;
    if (operand == 0x8000U) flags |= 2U;
    if (operand != 0U) flags |= 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}

void btst(CpuRegisters &r, bool set)
{
    r.status = static_cast<std::uint16_t>((r.status & ~4U) | (set ? 0U : 4U));
}

void push(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    write_word(host, r.address[7], static_cast<std::uint16_t>(value >> 16U));
    write_word(host, r.address[7] + 2U, static_cast<std::uint16_t>(value));
}

std::uint32_t pop(ExecutionHost &host, CpuRegisters &r)
{
    const auto value = (static_cast<std::uint32_t>(read_word(host, r.address[7])) << 16U)
        | read_word(host, r.address[7] + 2U);
    r.address[7] += 4U;
    return value;
}

FunctionResult call_normalize(FunctionContext &context, std::uint32_t callsite)
{
    context.registers.program_counter = 0x162f2U;
    return context.host->call_function(302U, 1U, 0x72U, 2U,
        callsite, 0x162f2U, context);
}
} // namespace

FunctionResult cpu_b_lookup_flagged_512_entry_common(
    FunctionContext &context, std::uint32_t table_base,
    bool inline_normalize_tail) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    r.address[0] = table_base;
    const auto original = static_cast<std::uint16_t>(r.data[1]);
    r.data[7] = (r.data[7] & 0xffff0000U) | original;
    logic(r, original, 0x8000U);
    const auto index = static_cast<std::uint16_t>(original & 0x1ffU);
    r.data[1] = (r.data[1] & 0xffff0000U) | index;
    logic(r, index, 0x8000U);

    const bool bit10 = (r.data[7] & 0x400U) != 0U;
    btst(r, bit10);
    if (!bit10) {
        const bool bit9 = (r.data[7] & 0x200U) != 0U;
        btst(r, bit9);
        if (!bit9) {
            r.program_counter = 0x162f2U;
            if (inline_normalize_tail)
                return cpu_b_normalize_wrapped_index_common(context);
            (void)host.call_function(302U, 1U, 0x72U, 1U,
                0x16386U, 0x162f2U, context);
            return FunctionResult::complete(3U, 0x162f2U);
        }
        r.data[0] = (r.data[0] & 0xffff0000U) | 0x200U;
        logic(r, 0x200U, 0x8000U);
        const auto value = static_cast<std::uint16_t>(0x200U - index);
        r.data[0] = (r.data[0] & 0xffff0000U) | value;
        sub_word(r, 0x200U, index, value, false);
        r.data[1] = (r.data[1] & 0xffff0000U) | value;
        logic(r, value, 0x8000U);
        push(host, r, 0x16396U);
        const auto child = call_normalize(context, 0x16392U);
        if (child.status != TranslationStatus::complete || child.control != 1U) return child;
        negate_word(r, 0U);
    } else {
        const bool bit9 = (r.data[7] & 0x200U) != 0U;
        btst(r, bit9);
        if (!bit9) {
            push(host, r, 0x163a4U);
            const auto child = call_normalize(context, 0x163a0U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            negate_word(r, 0U);
            negate_word(r, 1U);
        } else {
            r.data[0] = (r.data[0] & 0xffff0000U) | 0x200U;
            logic(r, 0x200U, 0x8000U);
            const auto value = static_cast<std::uint16_t>(0x200U - index);
            r.data[0] = (r.data[0] & 0xffff0000U) | value;
            sub_word(r, 0x200U, index, value, false);
            r.data[1] = (r.data[1] & 0xffff0000U) | value;
            logic(r, value, 0x8000U);
            push(host, r, 0x163b6U);
            const auto child = call_normalize(context, 0x163b2U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            negate_word(r, 1U);
        }
    }
    const auto target = pop(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

FunctionResult cpu_b_lookup_flagged_512_entry(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &registers = context.registers;
    if (registers.program_counter != 0x00016372U)
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};

    registers.address[0] = 0x0001680eU;
    registers.program_counter = 0x00016376U;
    return context.host->call_function(542U, 1U, 0x72U, 0U,
        0x00016372U, 0x00016376U, context);
}

} // namespace gain_ground::translated
