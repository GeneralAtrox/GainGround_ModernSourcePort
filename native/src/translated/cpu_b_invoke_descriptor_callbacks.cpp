#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint32_t read_long(
    ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kPrivateRegion, address, kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void push_long(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_long(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto value = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return value;
}

void set_test_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = static_cast<std::uint16_t>(registers.status & 0x0010U);
    if ((value & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (value == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t function_id_for_target(std::uint32_t target)
{
    switch (target) {
    case 0x00010aa4U: return 211U;
    case 0x00010aaeU: return 212U;
    case 0x00010ad6U: return 213U;
    case 0x00010ae0U: return 214U;
    case 0x00010b3eU: return 216U;
    case 0x00010d46U: return 226U;
    case 0x00010d78U: return 227U;
    case 0x00010e86U: return 231U;
    case 0x00010ea0U: return 232U;
    default: return 0xffffffffU;
    }
}

[[nodiscard]] FunctionResult call_target(FunctionContext &context,
    std::uint32_t callsite, std::uint32_t return_address,
    std::uint32_t target)
{
    const auto function_id = function_id_for_target(target);
    if (function_id == 0xffffffffU)
        return {TranslationStatus::contract_violation, 0U, callsite};
    auto &registers = context.registers;
    push_long(*context.host, registers, return_address);
    registers.program_counter = target;
    return context.host->call_function(function_id, 1U, 0x72U, 2U,
        callsite, target, context);
}

[[nodiscard]] FunctionResult finish(FunctionContext &context)
{
    const auto target = pop_long(*context.host, context.registers);
    context.registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_invoke_descriptor_callbacks(FunctionContext &context) noexcept
{
    if (!context.host) return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    // Original F210 entries, including both child return sites.
    for (;;) {
        switch (r.program_counter) {
        case 0x10a92U: r.address[0] = read_long(host, r.address[3]); r.program_counter = 0x10a94U; break;
        case 0x10a94U: case 0x10aa0U: {
            const auto site = r.program_counter;
            const auto continuation = site == 0x10a94U ? 0x10a96U : 0x10aa2U;
            const auto child = call_target(context, site, continuation, r.address[0]);
            if (child.status != TranslationStatus::complete || child.control != 1U ||
                r.program_counter != continuation) return child;
            break;
        }
        case 0x10a96U:
            set_test_word_flags(r, static_cast<std::uint16_t>(r.data[1]));
            r.program_counter = 0x10a98U; break;
        case 0x10a98U: r.program_counter = (r.status & 8U) ? 0x10a9aU : 0x10a9cU; break;
        case 0x10a9cU: r.address[0] = read_long(host, r.address[3] + 8U); r.program_counter = 0x10aa0U; break;
        case 0x10a9aU: case 0x10aa2U: return finish(context);
        default: return {TranslationStatus::contract_violation, 0U, r.program_counter};
        }
    }
}

} // namespace gain_ground::translated
