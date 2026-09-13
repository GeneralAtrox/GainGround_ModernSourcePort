#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic(CpuRegisters &r, std::uint32_t value,
               std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_zero_only(CpuRegisters &r, bool zero)
{
    r.status = static_cast<std::uint16_t>(
        (r.status & ~0x0004U) | (zero ? 0x0004U : 0U));
}

void set_sub_word(CpuRegisters &r, std::uint16_t left,
                  std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (left < right) flags |= 0x0011U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_add_word(CpuRegisters &r, std::uint16_t left,
                  std::uint16_t right, std::uint16_t result)
{
    const auto wide = static_cast<std::uint32_t>(left) + right;
    std::uint16_t flags{};
    if (wide > 0xffffU) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_add_long(CpuRegisters &r, std::uint32_t left,
                  std::uint32_t right, std::uint32_t result)
{
    const auto wide = static_cast<std::uint64_t>(left) + right;
    std::uint16_t flags{};
    if (wide > 0xffffffffULL) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x80000000U) != 0U)
        flags |= 0x0002U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint8_t read_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto word = host.read_memory_word(
        kPrivateRegion, address & ~1U, odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

[[nodiscard]] std::uint32_t read_long(
    ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(
        kPrivateRegion, address, kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address,
                std::uint32_t value)
{
    host.write_memory_word(kPrivateRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    write_long(host, r.address[7], value);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &r)
{
    const auto value = read_long(host, r.address[7]);
    r.address[7] += 4U;
    return value;
}

[[nodiscard]] FunctionResult call_child(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t continuation)
{
    push_return(*context.host, context.registers, continuation);
    context.registers.program_counter = target;
    return context.host->call_function(function_id, 1U, 0x72U, 2U,
        callsite, target, context);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0000aab6(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    if (r.program_counter != 0x0000aab6U)
        return {TranslationStatus::contract_violation, 0U,
            r.program_counter};

    r.data[5] = 0U;
    set_logic(r, r.data[5], 0x80000000U, 0xffffffffU);
    const auto selector = host.read_memory_word(
        kPrivateRegion, 0x00000c02U, kWordMask);
    r.data[5] = (r.data[5] & 0xffff0000U) | selector;
    set_logic(r, selector, 0x8000U, 0xffffU);

    const auto quotient = r.data[5] / 10U;
    const auto remainder = r.data[5] % 10U;
    r.data[5] = (remainder << 16U) | quotient;
    set_logic(r, quotient, 0x8000U, 0xffffU);
    r.data[5] = (r.data[5] << 16U) | (r.data[5] >> 16U);
    set_logic(r, r.data[5], 0x80000000U, 0xffffffffU);
    const auto d5_before = static_cast<std::uint16_t>(r.data[5]);
    const auto d5_after = static_cast<std::uint16_t>(d5_before - 1U);
    r.data[5] = (r.data[5] & 0xffff0000U) | d5_after;
    set_sub_word(r, d5_before, 1U, d5_after);

    auto child = call_child(context, 283U,
        0x0000aac4U, 0x00015e28U, 0x0000aacaU);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    r.data[4] = r.data[0];
    set_logic(r, r.data[4], 0x80000000U, 0xffffffffU);
    const auto table_offset = static_cast<std::uint16_t>(r.data[0]) & 0x003eU;
    r.data[0] = (r.data[0] & 0xffff0000U) | table_offset;
    set_logic(r, table_offset, 0x8000U, 0xffffU);
    r.address[2] = 0x0000b516U;
    r.address[2] = static_cast<std::uint32_t>(
        r.address[2] + static_cast<std::int16_t>(table_offset));
    r.data[3] = (r.data[3] & 0xffff0000U) | 0x0772U;
    set_logic(r, 0x0772U, 0x8000U, 0xffffU);

    const auto mode = read_byte(host, 0x00000c01U);
    set_zero_only(r, (mode & 1U) == 0U);
    if ((mode & 1U) != 0U) {
        r.data[3] = (r.data[3] & 0xffff0000U) | 0x07f2U;
        set_logic(r, 0x07f2U, 0x8000U, 0xffffU);
    }

    for (;;) {
        r.data[0] = 0U;
        set_logic(r, r.data[0], 0x80000000U, 0xffffffffU);
        const auto doubled = r.data[4] + r.data[4];
        set_add_long(r, r.data[4], r.data[4], doubled);
        r.data[4] = doubled;
        if ((r.status & 0x0001U) != 0U) {
            r.data[0] = 0x00000040U;
            set_logic(r, r.data[0], 0x80000000U, 0xffffffffU);
        }
        const auto d0_before = static_cast<std::uint16_t>(r.data[0]);
        const auto d3 = static_cast<std::uint16_t>(r.data[3]);
        const auto d0_after = static_cast<std::uint16_t>(d0_before + d3);
        r.data[0] = (r.data[0] & 0xffff0000U) | d0_after;
        set_add_word(r, d0_before, d3, d0_after);

        child = call_child(context, 626U,
            0x0000aaf0U, 0x0000ab32U, 0x0000aaf4U);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;

        const auto counter = static_cast<std::uint16_t>(r.data[5] - 1U);
        r.data[5] = (r.data[5] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }

    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
