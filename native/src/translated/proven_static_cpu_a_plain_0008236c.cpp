#include "gain_ground/contract_types.h"
#include "gground_functions.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kPaletteRegion = 9U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void pf(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(
        kSharedRegion, address & kSharedMask, kWordMask);
}

std::uint16_t read_shared(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(
        kSharedRegion, address & kSharedMask, kWordMask);
}

void write_shared(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value)
{
    host.write_memory_word(
        kSharedRegion, address & kSharedMask, value, kWordMask);
}

void write_palette_long(ExecutionHost &host, std::uint32_t address,
    std::uint32_t value)
{
    const auto offset = address - 0x00400000U;
    host.write_memory_word(kPaletteRegion, offset,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPaletteRegion, offset + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

void move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void move_long_flags(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

FunctionResult call_child(FunctionContext &context, std::uint32_t function_id,
    std::uint32_t callsite, std::uint32_t target, std::uint32_t return_pc)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[7] -= 4U;
    write_shared(host, registers.address[7],
        static_cast<std::uint16_t>(return_pc >> 16U));
    write_shared(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(return_pc));
    pf(host, target);
    pf(host, target + 2U);
    registers.program_counter = target;
    return host.call_function(function_id, 0U, 0xffU, 2U,
        callsite, target, context);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_0008236c(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    pf(host, 0x00082370U); pf(host, 0x00082372U);
    registers.address[0] = 0x00400000U;

    constexpr std::uint16_t colours[] = {
        0x7fffU, 0x100fU, 0x20f0U, 0x4f00U, 0x30efU,
    };
    constexpr std::uint32_t callsites[] = {
        0x00082376U, 0x0008237eU, 0x00082386U,
        0x0008238eU, 0x00082396U,
    };
    constexpr std::uint32_t returns[] = {
        0x0008237aU, 0x00082382U, 0x0008238aU,
        0x00082392U, 0x0008239aU,
    };
    for (unsigned index = 0; index < 5U; ++index) {
        const auto move_pc = callsites[index] - 4U;
        if (index == 0U) pf(host, move_pc + 2U);
        pf(host, callsites[index]);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | colours[index];
        move_word_flags(registers, colours[index]);
        pf(host, callsites[index] + 2U);
        const auto child = call_child(context, 463U, callsites[index],
            0x00082410U, returns[index]);
        if (child.status != TranslationStatus::complete) return child;
    }

    pf(host, 0x0008239eU); pf(host, 0x000823a0U);
    registers.address[0] = 0x00400000U;
    pf(host, 0x000823a2U); pf(host, 0x000823a4U);
    registers.address[0] += 0x00000100U;
    pf(host, 0x000823a6U); pf(host, 0x000823a8U);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | 0x0017U;
    move_word_flags(registers, 0x0017U);

    for (;;) {
        pf(host, 0x000823aaU); pf(host, 0x000823acU); pf(host, 0x000823aeU);
        registers.address[1] = 0x00082d62U;
        for (unsigned index = 0; index < 8U; ++index) {
            pf(host, 0x000823b0U + index * 2U);
            const auto high = read_shared(host, registers.address[1]);
            const auto low = read_shared(host, registers.address[1] + 2U);
            registers.address[1] += 4U;
            const auto value = (static_cast<std::uint32_t>(high) << 16U) | low;
            write_palette_long(host, registers.address[0], value);
            registers.address[0] += 4U;
            move_long_flags(registers, value);
        }
        pf(host, 0x000823c0U);
        const auto counter = static_cast<std::uint16_t>(registers.data[1] - 1U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | counter;
        pf(host, 0x000823a8U);
        if (counter != 0xffffU) continue;
        pf(host, 0x000823c2U);
        break;
    }

    pf(host, 0x000823c4U); pf(host, 0x000823c6U); pf(host, 0x000823c8U);
    registers.address[0] = 0x00400000U;
    pf(host, 0x000823caU); pf(host, 0x000823ccU);
    registers.address[0] += 0x000000e0U;
    registers.address[1] = registers.address[0];
    pf(host, 0x000823ceU);

    constexpr std::uint32_t starts[] = {
        0x7fff0fffU, 0x100f000fU, 0x20f000f0U, 0x4f000f00U,
    };
    constexpr std::uint32_t steps[] = {
        0x01110111U, 0x00010001U, 0x00100010U, 0x01000100U,
    };
    constexpr std::uint32_t gradient_calls[] = {
        0x000823daU, 0x000823eaU, 0x000823faU, 0x0008240aU,
    };
    constexpr std::uint32_t gradient_returns[] = {
        0x000823deU, 0x000823eeU, 0x000823feU, 0x0008240eU,
    };
    for (unsigned index = 0; index < 4U; ++index) {
        const auto d0_pc = gradient_calls[index] - 12U;
        if (index == 0U) pf(host, d0_pc + 2U);
        pf(host, d0_pc + 4U); pf(host, d0_pc + 6U);
        registers.data[0] = starts[index];
        move_long_flags(registers, starts[index]);
        pf(host, d0_pc + 8U); pf(host, d0_pc + 10U); pf(host, gradient_calls[index]);
        registers.data[1] = steps[index];
        move_long_flags(registers, steps[index]);
        pf(host, gradient_calls[index] + 2U);
        const auto child = call_child(context, 464U, gradient_calls[index],
            0x00082432U, gradient_returns[index]);
        if (child.status != TranslationStatus::complete) return child;
    }

    const auto high = read_shared(host, registers.address[7]);
    const auto low = read_shared(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    pf(host, target); pf(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
