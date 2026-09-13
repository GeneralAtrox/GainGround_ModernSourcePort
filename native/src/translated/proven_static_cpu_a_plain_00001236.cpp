#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgram = 1U;
constexpr std::uint16_t kShared = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;

void pf(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgram, address, kWordMask);
}

std::uint16_t rs(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kShared, address & 0x0003ffffU, kWordMask);
}

void ws(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kShared, address & 0x0003ffffU, value, kWordMask);
}

void push_long(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    ws(host, r.address[7], static_cast<std::uint16_t>(value >> 16U));
    ws(host, r.address[7] + 2U, static_cast<std::uint16_t>(value));
}

FunctionResult call(FunctionContext &context, std::uint32_t function_id,
    std::uint32_t site, std::uint32_t target, std::uint32_t return_address)
{
    auto &host = *context.host;
    auto &r = context.registers;
    push_long(host, r, return_address);
    pf(host, target);
    pf(host, target + 2U);
    r.program_counter = target;
    return host.call_function(function_id, 0U, 0xffU, 2U,
        site, target, context);
}

FunctionResult branch(FunctionContext &context, std::uint32_t site)
{
    auto &host = *context.host;
    pf(host, 0x0000198cU);
    pf(host, 0x0000198eU);
    context.registers.program_counter = 0x0000198cU;
    return host.call_function(518U, 0U, 0xffU, 1U,
        site, 0x0000198cU, context);
}

bool completed(FunctionResult const &result)
{
    return result.status == TranslationStatus::complete;
}
} // namespace

FunctionResult proven_static_cpu_a_plain_00001236(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;

    r.address[7] -= 2U;
    pf(host, 0x0000123aU);
    pf(host, 0x0000123cU);
    ws(host, r.address[7], 5U);

retry:
    auto child = call(context, 439U, 0x0000123aU,
        0x00001258U, 0x0000123eU);
    if (!completed(child)) return child;

    child = call(context, 17U, 0x0000123eU,
        0x00001710U, 0x00001242U);
    if (!completed(child)) return child;

    r.status = host.apply_controlled_status(
        0U, 0xffU, 0x00001242U, 0x00b00000U, r.status);
    if ((r.status & 0x0001U) != 0U)
        return branch(context, 0x00001242U);

    pf(host, 0x00001246U);
    pf(host, 0x00001248U);
    child = call(context, 16U, 0x00001246U,
        0x000016a0U, 0x0000124aU);
    if (!completed(child)) return child;

    r.status = host.apply_controlled_status(
        0U, 0xffU, 0x0000124aU, 0x00b00000U, r.status);
    if ((r.status & 0x0001U) == 0U) {
        pf(host, 0x00001254U);
        pf(host, 0x00001256U);
    } else {
        pf(host, 0x0000124cU);
        const auto stack_value = rs(host, r.address[7]);
        const auto next = static_cast<std::uint16_t>(stack_value - 1U);
        ws(host, r.address[7], next);
        std::uint16_t flags{};
        if ((next & 0x8000U) != 0U) flags |= 0x0008U;
        if (next == 0U) flags |= 0x0004U;
        if (stack_value == 0x8000U) flags |= 0x0002U;
        if (stack_value == 0U) flags |= 0x0011U;
        r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
        pf(host, 0x0000124eU);
        if ((r.status & 0x0004U) == 0U) {
            pf(host, 0x0000123aU);
            pf(host, 0x0000123cU);
            goto retry;
        }
        pf(host, 0x00001250U);
        pf(host, 0x00001252U);
        return branch(context, 0x00001250U);
    }

    r.address[7] += 2U;
    pf(host, 0x00001258U);
    const auto stack = r.address[7] & 0x0003ffffU;
    const auto target = (static_cast<std::uint32_t>(rs(host, stack)) << 16U)
        | rs(host, stack + 2U);
    r.address[7] += 4U;
    pf(host, target);
    pf(host, target + 2U);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
