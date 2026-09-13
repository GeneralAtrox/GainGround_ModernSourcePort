#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & 0x00ffffffU, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(
        kRegion, address & 0x00ffffffU, value, kWordMask);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto value = host.read_memory_word(
        kRegion, address & 0x00fffffeU, mask);
    return static_cast<std::uint8_t>(odd ? value : value >> 8U);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = read_word(host, address);
    const auto low = read_word(host, address + 2U);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    // 68000 longword read-modify-write operations commit the low word first.
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
}

void set_logic_flags(CpuRegisters &registers,
    std::uint32_t value, std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_sub_word_flags(CpuRegisters &registers,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (left < right) flags |= 0x0011U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_long_flags(CpuRegisters &registers,
    std::uint32_t left, std::uint32_t right, std::uint32_t result)
{
    std::uint16_t flags{};
    if (result < left) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x80000000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_word(host, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U));
    write_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value));
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return target;
}

[[nodiscard]] FunctionResult call_child(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t continuation)
{
    auto &registers = context.registers;
    auto &host = *context.host;
    push_return(host, registers, continuation);
    registers.program_counter = target;
    return host.call_function(
        function_id, 1U, 0x72U, 2U, callsite, target, context);
}

[[nodiscard]] FunctionResult finish(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_advance_record_position(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];
    const auto entry_pc = registers.program_counter;
    if (entry_pc == 0x0001fe94U) {
        const auto child = call_child(context, 282U,
            0x0001fe94U, 0x00015df2U, 0x0001fe9aU);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
        return finish(host, registers);
    }
    if (entry_pc != 0x0001fe60U)
        return {TranslationStatus::contract_violation, 0U,
            entry_pc};

    const auto busy = read_byte(host, base + 0x3fU);
    set_logic_flags(registers, busy, 0x80U, 0xffU);
    if (busy != 0U) goto reject;

    {
        const auto timer = read_word(host, base + 0x74U);
        const auto updated = static_cast<std::uint16_t>(timer - 1U);
        write_word(host, base + 0x74U, updated);
        set_sub_word_flags(registers, timer, 1U, updated);
        const bool less_or_equal = (registers.status & 0x0004U) != 0U
            || (((registers.status & 0x0008U) != 0U)
                != ((registers.status & 0x0002U) != 0U));
        if (less_or_equal) goto reject;
    }

    {
        const auto child = call_child(context, 386U,
            0x0001fe6cU, 0x000206a4U, 0x0001fe70U);
        // The rejection path already discarded our frame and returned.
        if (child.status == TranslationStatus::complete && child.control == 8U)
            return FunctionResult::complete(1U, child.exit_program_counter);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
        if ((registers.status & 0x0001U) != 0U) goto reject;
    }
    {
        const auto child = call_child(context, 385U,
            0x0001fe72U, 0x0002060cU, 0x0001fe76U);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
        if ((registers.status & 0x0001U) != 0U) goto reject;
    }

    {
        const auto delta = read_long(host, base + 0x1eU);
        registers.data[0] = delta;
        set_logic_flags(registers, delta, 0x80000000U, 0xffffffffU);
        const auto position = read_long(host, base + 0x12U);
        const auto result = position + delta;
        write_long(host, base + 0x12U, result);
        set_add_long_flags(registers, position, delta, result);
    }
    {
        const auto delta = read_long(host, base + 0x26U);
        registers.data[0] = delta;
        set_logic_flags(registers, delta, 0x80000000U, 0xffffffffU);
        const auto position = read_long(host, base + 0x1aU);
        const auto result = position + delta;
        write_long(host, base + 0x1aU, result);
        set_add_long_flags(registers, position, delta, result);
    }
    {
        const auto child = call_child(context, 280U,
            0x0001fe88U, 0x00015d24U, 0x0001fe8eU);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }
    {
        const auto child = call_child(context, 388U,
            0x0001fe8eU, 0x000207c2U, 0x0001fe94U);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }
    {
        const auto child = call_child(context, 282U,
            0x0001fe94U, 0x00015df2U, 0x0001fe9aU);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }
    return finish(host, registers);

reject:
    {
        const auto child = call_child(context, 389U,
            0x0001fe9cU, 0x0002089cU, 0x0001fea0U);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }
    return finish(host, registers);
}

} // namespace gain_ground::translated
