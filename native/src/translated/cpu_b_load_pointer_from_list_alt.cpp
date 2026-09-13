#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint16_t read_word(
    ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & 0x00ffffffU, kWordMask);
}

[[nodiscard]] std::uint32_t read_long(
    ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

void logic_word(CpuRegisters &registers, std::uint16_t value) noexcept
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return target;
}

[[nodiscard]] std::uint32_t function_for_target(std::uint32_t target) noexcept
{
    switch (target) {
    case 0x00010d46U: return 226U;
    case 0x00010d78U: return 227U;
    case 0x00010dcaU: return 228U;
    case 0x00010e86U: return 231U;
    case 0x00010ea0U: return 232U;
    default: return 0xffffffffU;
    }
}

[[nodiscard]] FunctionResult call_child(FunctionContext &context,
    std::uint32_t callsite, std::uint32_t target, std::uint32_t continuation)
{
    const auto function_id = function_for_target(target);
    if (function_id == 0xffffffffU)
        return {TranslationStatus::contract_violation, 0U, target};
    push_return(*context.host, context.registers, continuation);
    context.registers.program_counter = target;
    return context.host->call_function(function_id, 1U, 0x72U, 2U,
        callsite, target, context);
}

[[nodiscard]] bool complete(const FunctionResult &result) noexcept
{
    return result.status == TranslationStatus::complete
        && result.control == 1U;
}
} // namespace

FunctionResult cpu_b_load_pointer_from_list_alt(FunctionContext &context) noexcept
{
    if (!context.host) return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    // Original F225 rereads live A3 after the first callback.
    for (;;) {
        switch (r.program_counter) {
        case 0x10d34U: r.address[0] = read_long(host, r.address[3]); r.program_counter = 0x10d36U; break;
        case 0x10d36U: case 0x10d42U: {
            const auto site = r.program_counter;
            const auto continuation = site == 0x10d36U ? 0x10d38U : 0x10d44U;
            const auto child = call_child(context, site, r.address[0], continuation);
            if (!complete(child) || r.program_counter != continuation) return child;
            break;
        }
        case 0x10d38U:
            logic_word(r, static_cast<std::uint16_t>(r.data[1]));
            r.program_counter = 0x10d3aU; break;
        case 0x10d3aU: r.program_counter = (r.status & 8U) ? 0x10d3cU : 0x10d3eU; break;
        case 0x10d3eU: r.address[0] = read_long(host, r.address[3] + 8U); r.program_counter = 0x10d42U; break;
        case 0x10d3cU: case 0x10d44U: {
            const auto target = pop_return(host, r); r.program_counter = target;
            return FunctionResult::complete(1U, target);
        }
        default: return {TranslationStatus::contract_violation, 0U, r.program_counter};
        }
    }
}

} // namespace gain_ground::translated
