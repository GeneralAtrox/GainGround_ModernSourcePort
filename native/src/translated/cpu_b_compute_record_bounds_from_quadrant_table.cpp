#include "gain_ground/contract_types.h"
#include "gain_ground/cpu_b_interrupt.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & 0x00ffffffU, kWordMask);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto value = host.read_memory_word(
        kRegion, address & 0x00fffffeU, mask);
    return static_cast<std::uint8_t>(odd ? value : value >> 8U);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(
        kRegion, address & 0x00ffffffU, value, kWordMask);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = read_word(host, address);
    const auto low = read_word(host, address + 2U);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_word(CpuRegisters &registers, std::size_t index, std::uint16_t value)
{
    registers.data[index] = (registers.data[index] & 0xffff0000U) | value;
}

void set_byte(CpuRegisters &registers, std::size_t index, std::uint8_t value)
{
    registers.data[index] = (registers.data[index] & 0xffffff00U) | value;
}

void set_logic_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU)
        flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_sub_word_flags(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (left < right) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint16_t add_word(CpuRegisters &registers,
    std::uint16_t left, std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left + right);
    set_add_word_flags(registers, left, right, result);
    return result;
}

[[nodiscard]] std::uint16_t subtract_word(CpuRegisters &registers,
    std::uint16_t left, std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left - right);
    set_sub_word_flags(registers, left, right, result);
    return result;
}

[[nodiscard]] std::uint16_t signed_table_byte(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto raw = read_byte(host, registers.address[0]++);
    set_byte(registers, 1U, raw);
    const auto value = static_cast<std::uint16_t>(
        static_cast<std::int16_t>(static_cast<std::int8_t>(raw)));
    set_word(registers, 1U, value);
    set_logic_word_flags(registers, value);
    return value;
}

void store_result(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address, std::uint16_t value)
{
    write_word(host, address, value);
    set_logic_word_flags(registers, value);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = read_word(host, registers.address[7]);
    const auto low = read_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

[[nodiscard]] std::optional<FunctionResult> interrupt(ExecutionHost &host, FunctionContext &context,
    std::uint32_t completed_pc, std::uint32_t resume_pc)
{
    if (host.resumes_interrupts_inline())
        return cpu_b_interrupt_boundary(context, completed_pc, resume_pc);
    auto &registers = context.registers;
    const auto pending = host.consume_pending_interrupt(1U, 0x72U, completed_pc);
    if (!pending.asserted
        || pending.level <= ((registers.status >> 8U) & 7U))
        return std::nullopt;

    const auto saved_status = registers.status;
    registers.address[7] -= 4U;
    write_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(resume_pc));
    registers.address[7] -= 2U;
    write_word(host, registers.address[7], saved_status);
    write_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(resume_pc >> 16U));
    registers.status = static_cast<std::uint16_t>((saved_status & 0x38ffU)
        | 0x2000U | (static_cast<std::uint16_t>(pending.level) << 8U));
    const auto target = read_long(host,
        static_cast<std::uint32_t>(24U + pending.level) * 4U);
    registers.program_counter = target;
    context.state = 0x04U;
    (void)host.call_function(static_cast<std::uint32_t>(96U + pending.level),
        1U, 0x04U, 6U, completed_pc, target, context);
    return FunctionResult::complete(5U, target);
}
} // namespace

FunctionResult cpu_b_compute_record_bounds_from_quadrant_table(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];
    const auto entry_pc = registers.program_counter;
    if (entry_pc != 0x000207c2U && entry_pc != 0x000207f8U
        && entry_pc != 0x000207feU)
        return {TranslationStatus::contract_violation, 0U, entry_pc};
    std::uint16_t d0 = static_cast<std::uint16_t>(registers.data[0]);

    if (entry_pc == 0x000207c2U) {
        registers.address[0] = 0x0003ae48U;
        registers.data[0] = 0U;
        set_logic_word_flags(registers, 0U);
        const auto mode = read_byte(host, base + 0x3cU);
        set_byte(registers, 0U, mode);
        set_logic_word_flags(registers, mode);
        d0 = add_word(registers, mode, mode);
        set_word(registers, 0U, d0);
        set_word(registers, 1U, d0);
        set_logic_word_flags(registers, d0);
        d0 = static_cast<std::uint16_t>(d0 << 3U);
        set_word(registers, 0U, d0);
        set_logic_word_flags(registers, d0);
        d0 = add_word(registers, d0,
            static_cast<std::uint16_t>(registers.data[1]));
        set_word(registers, 0U, d0);
        registers.address[0] = static_cast<std::uint32_t>(registers.address[0]
            + static_cast<std::int16_t>(d0));

        d0 = read_word(host, base + 0x5cU);
        set_word(registers, 0U, d0);
        set_logic_word_flags(registers, d0);
        d0 = add_word(registers, d0, 0x0080U);
        set_word(registers, 0U, d0);
        d0 = static_cast<std::uint16_t>(d0 & 0x0300U);
        set_word(registers, 0U, d0);
        set_logic_word_flags(registers, d0);
        set_word(registers, 1U, d0);
        set_logic_word_flags(registers, d0);
        registers.address[0] += 6U;
        d0 = static_cast<std::uint16_t>(d0 & 0x0100U);
        set_word(registers, 0U, d0);
        set_logic_word_flags(registers, d0);
        if (d0 != 0U) {
            // The first quadrant bit selects the second six-byte table row.
        } else {
            registers.address[0] += 6U;
            d0 = static_cast<std::uint16_t>(d0 & 0x0200U);
            set_word(registers, 0U, d0);
            set_logic_word_flags(registers, d0);
            if (const auto interrupted = interrupt(host, context, 0x000207f4U, 0x000207f8U))
                return *interrupted;
            registers.address[0] -= 12U;
            if (const auto interrupted = interrupt(host, context, 0x000207faU, 0x000207feU))
                return *interrupted;
        }
    } else if (entry_pc == 0x000207f8U) {
        registers.address[0] -= 12U;
        if (const auto interrupted = interrupt(host, context, 0x000207faU, 0x000207feU))
            return *interrupted;
    }

    d0 = read_word(host, base + 0x12U);
    set_word(registers, 0U, d0);
    set_logic_word_flags(registers, d0);
    const bool reverse_x = (read_byte(host, base + 1U) & 0x02U) != 0U;
    if (!reverse_x) {
        auto offset = signed_table_byte(host, registers);
        d0 = subtract_word(registers, d0, offset);
        set_word(registers, 0U, d0);
        store_result(host, registers, base + 0x2aU, d0);
        offset = signed_table_byte(host, registers);
        d0 = add_word(registers, d0, offset);
        set_word(registers, 0U, d0);
        store_result(host, registers, base + 0x2cU, d0);
    } else {
        auto offset = signed_table_byte(host, registers);
        d0 = add_word(registers, d0, offset);
        set_word(registers, 0U, d0);
        store_result(host, registers, base + 0x2cU, d0);
        offset = signed_table_byte(host, registers);
        d0 = subtract_word(registers, d0, offset);
        set_word(registers, 0U, d0);
        store_result(host, registers, base + 0x2aU, d0);
    }
    d0 = read_word(host, base + 0x16U);
    set_word(registers, 0U, d0);
    set_logic_word_flags(registers, d0);
    const bool reverse_y = (read_byte(host, base + 1U) & 0x01U) != 0U;
    if (!reverse_y) {
        auto offset = signed_table_byte(host, registers);
        d0 = subtract_word(registers, d0, offset);
        set_word(registers, 0U, d0);
        store_result(host, registers, base + 0x2eU, d0);
        offset = signed_table_byte(host, registers);
        d0 = add_word(registers, d0, offset);
        set_word(registers, 0U, d0);
        store_result(host, registers, base + 0x30U, d0);
        d0 = read_word(host, base + 0x1aU);
        set_word(registers, 0U, d0);
        set_logic_word_flags(registers, d0);
        offset = signed_table_byte(host, registers);
        d0 = subtract_word(registers, d0, offset);
        set_word(registers, 0U, d0);
        store_result(host, registers, base + 0x32U, d0);
        offset = signed_table_byte(host, registers);
        d0 = add_word(registers, d0, offset);
        set_word(registers, 0U, d0);
        store_result(host, registers, base + 0x34U, d0);
    } else {
        auto offset = signed_table_byte(host, registers);
        d0 = add_word(registers, d0, offset);
        set_word(registers, 0U, d0);
        store_result(host, registers, base + 0x30U, d0);
        offset = signed_table_byte(host, registers);
        d0 = subtract_word(registers, d0, offset);
        set_word(registers, 0U, d0);
        store_result(host, registers, base + 0x2eU, d0);
        d0 = read_word(host, base + 0x1aU);
        set_word(registers, 0U, d0);
        set_logic_word_flags(registers, d0);
        offset = signed_table_byte(host, registers);
        d0 = add_word(registers, d0, offset);
        set_word(registers, 0U, d0);
        store_result(host, registers, base + 0x34U, d0);
        offset = signed_table_byte(host, registers);
        d0 = subtract_word(registers, d0, offset);
        set_word(registers, 0U, d0);
        store_result(host, registers, base + 0x32U, d0);
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
