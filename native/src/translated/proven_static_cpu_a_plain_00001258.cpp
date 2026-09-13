#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgram = 1U;
constexpr std::uint16_t kShared = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kByteMask = 0x00ffU;

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

std::uint8_t rh(ExecutionHost &host, std::uint32_t pc, std::uint32_t address)
{
    return static_cast<std::uint8_t>(host.read_hardware(
        1U, 0U, 0xffU, pc, address & ~1U, kByteMask));
}

void bit_test(CpuRegisters &r, bool set)
{
    r.status = static_cast<std::uint16_t>(
        (r.status & ~0x0004U) | (set ? 0U : 0x0004U));
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

FunctionResult branch(FunctionContext &context, std::uint32_t function_id,
    std::uint32_t site, std::uint32_t target)
{
    auto &host = *context.host;
    pf(host, target);
    pf(host, target + 2U);
    context.registers.program_counter = target;
    return host.call_function(function_id, 0U, 0xffU, 1U,
        site, target, context);
}

bool completed(FunctionResult const &result)
{
    return result.status == TranslationStatus::complete;
}
} // namespace

FunctionResult proven_static_cpu_a_plain_00001258(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;

    pf(host, 0x0000125cU);
    pf(host, 0x0000125eU);
    const auto initial_status = rh(host, 0x00001258U, r.address[5] + 8U);
    bit_test(r, (initial_status & 0x10U) != 0U);
    pf(host, 0x00001260U);
    r.status = host.apply_controlled_status(0U, 0xffU,
        0x0000125eU, (r.address[5] + 8U) & ~1U, r.status);
    if ((r.status & 0x0004U) != 0U) {
        pf(host, 0x0000195cU);
        pf(host, 0x0000195eU);
        r.program_counter = 0x0000195cU;
        return FunctionResult::complete(3U, 0x0000195cU);
    }

    pf(host, 0x00001262U);
    pf(host, 0x00001264U);
    auto child = call(context, 21U, 0x00001262U,
        0x000017e4U, 0x00001266U);
    if (!completed(child)) return child;

    pf(host, 0x0000126aU);
    const auto saved_a0 = r.address[0];
    r.address[7] -= 4U;
    ws(host, r.address[7] + 2U, static_cast<std::uint16_t>(saved_a0));
    ws(host, r.address[7], static_cast<std::uint16_t>(saved_a0 >> 16U));
    pf(host, 0x0000126cU);
    pf(host, 0x0000126eU);
    r.address[0] = 0x000065ccU;

    pf(host, 0x00001270U);
    child = call(context, 444U, 0x0000126eU,
        0x00001862U, 0x00001272U);
    if (!completed(child)) return child;

    child = call(context, 436U, 0x00001272U,
        0x00000d46U, 0x00001276U);
    if (!completed(child)) return child;

    pf(host, 0x0000127aU);
    pf(host, 0x0000127cU);
    const auto final_status = rh(host, 0x00001276U, r.address[5]);
    bit_test(r, (final_status & 0x04U) != 0U);
    pf(host, 0x0000127eU);
    r.status = host.apply_controlled_status(0U, 0xffU,
        0x0000127cU, r.address[5] & ~1U, r.status);
    if ((r.status & 0x0004U) == 0U)
        return branch(context, 612U, 0x0000127cU, 0x00001974U);

    pf(host, 0x00001280U);
    pf(host, 0x00001282U);
    const auto stack = r.address[7] & 0x0003ffffU;
    r.address[0] = (static_cast<std::uint32_t>(rs(host, stack)) << 16U)
        | rs(host, stack + 2U);
    r.address[7] += 4U;

    pf(host, 0x00001284U);
    const auto return_stack = r.address[7] & 0x0003ffffU;
    const auto target = (static_cast<std::uint32_t>(rs(host, return_stack)) << 16U)
        | rs(host, return_stack + 2U);
    r.address[7] += 4U;
    pf(host, target);
    pf(host, target + 2U);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
