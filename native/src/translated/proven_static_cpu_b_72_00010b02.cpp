#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint32_t read_long(
    ExecutionHost &host, std::uint32_t address) noexcept
{
    const auto high = host.read_memory_word(
        kPrivateRegion, address, kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address,
                std::uint32_t value) noexcept
{
    host.write_memory_word(kPrivateRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value) noexcept
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (value == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_logic_long(CpuRegisters &registers, std::uint32_t value) noexcept
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U)
        flags |= 0x0008U;
    if (value == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_word(ExecutionHost &host, CpuRegisters &registers,
               std::uint16_t value) noexcept
{
    registers.address[7] -= 2U;
    host.write_memory_word(
        kPrivateRegion, registers.address[7], value, kWordMask);
}

void push_long(ExecutionHost &host, CpuRegisters &registers,
               std::uint32_t value) noexcept
{
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], value);
}

void push_movem_long(ExecutionHost &host, CpuRegisters &registers,
                     std::uint32_t value) noexcept
{
    registers.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
    host.write_memory_word(kPrivateRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
}

[[nodiscard]] std::uint32_t function_id_for_first_target(
    std::uint32_t target) noexcept
{
    switch (target) {
    case 0x00010b58U: return 217U;
    case 0x00010b92U: return 218U;
    case 0x00010bd2U: return 219U;
    case 0x00010c0eU: return 220U;
    default: return 0xffffffffU;
    }
}

[[nodiscard]] FunctionResult call_target(
    FunctionContext &context, std::uint32_t function_id,
    std::uint32_t callsite, std::uint32_t return_address,
    std::uint32_t target) noexcept
{
    auto &registers = context.registers;
    push_long(*context.host, registers, return_address);
    registers.program_counter = target;
    return context.host->call_function(function_id, 1U, 0x72U, 2U,
                                       callsite, target, context);
}

[[nodiscard]] FunctionResult finish(FunctionContext &context) noexcept
{
    auto &registers = context.registers;
    const auto target = read_long(*context.host, registers.address[7]);
    registers.address[7] += 4U;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult proven_static_cpu_b_72_00010b02(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.address[0] = read_long(host, registers.address[3] + 0x0cU);
    auto function_id = function_id_for_first_target(registers.address[0]);
    if (function_id == 0xffffffffU)
        return {TranslationStatus::contract_violation, 0U, 0x00010b06U};
    auto child = call_target(context, function_id, 0x00010b06U,
                             0x00010b08U, registers.address[0]);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    registers.program_counter = 0x00010b08U;
    registers.address[0] = read_long(host, registers.address[3] + 0x14U);
    if (registers.address[0] != 0x00010c3eU)
        return {TranslationStatus::contract_violation, 0U, 0x00010b0cU};
    child = call_target(context, 221U, 0x00010b0cU,
                        0x00010b0eU, registers.address[0]);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    registers.program_counter = 0x00010b0eU;
    registers.address[0] = read_long(host, registers.address[3] + 0x1cU);
    if (registers.address[0] != 0x00010c8aU)
        return {TranslationStatus::contract_violation, 0U, 0x00010b12U};
    child = call_target(context, 222U, 0x00010b12U,
                        0x00010b14U, registers.address[0]);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    registers.program_counter = 0x00010b14U;
    const auto d0 = host.read_memory_word(
        kPrivateRegion, registers.address[3] + 0x24U, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    set_logic_word(registers, d0);

    const auto d7 = static_cast<std::uint16_t>(registers.data[7]);
    push_word(host, registers, d7);
    set_logic_word(registers, d7);

    push_movem_long(host, registers, registers.address[4]);
    set_logic_long(registers, registers.address[4]);

    child = call_target(context, 308U, 0x00010b1cU,
                        0x00010b22U, 0x00016ff8U);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    registers.program_counter = 0x00010b22U;
    registers.address[4] = read_long(host, registers.address[7]);
    registers.address[7] += 4U;

    const auto restored_d7 = host.read_memory_word(
        kPrivateRegion, registers.address[7], kWordMask);
    registers.address[7] += 2U;
    registers.data[7] = (registers.data[7] & 0xffff0000U) | restored_d7;
    set_logic_word(registers, restored_d7);

    return finish(context);
}

} // namespace gain_ground::translated
