#include "gain_ground/contract_types.h"
#include "gain_ground/cpu_b_interrupt.h"

#include <cstdint>
#include <optional>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint16_t read_word(
    ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & 0x00ffffffU, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value)
{
    host.write_memory_word(kRegion, address & 0x00ffffffU, value, kWordMask);
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

void add_word(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result) noexcept
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

void sub_word(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result, bool update_extend) noexcept
{
    std::uint16_t flags = update_extend ? 0U : (registers.status & 0x0010U);
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (right > left) flags |= update_extend ? 0x0011U : 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void asl_word(CpuRegisters &registers, unsigned count) noexcept
{
    auto value = static_cast<std::uint16_t>(registers.data[0]);
    bool overflow{};
    bool carry{};
    for (unsigned index = 0; index != count; ++index) {
        const bool old_sign = (value & 0x8000U) != 0U;
        carry = old_sign;
        value = static_cast<std::uint16_t>(value << 1U);
        overflow = overflow || (old_sign != ((value & 0x8000U) != 0U));
    }
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    std::uint16_t flags{};
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    if (carry) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] FunctionResult finish(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

[[nodiscard]] std::optional<FunctionResult> check_interrupt(
    FunctionContext &context, std::uint32_t completed_pc,
    std::uint32_t next_pc)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    if (host.resumes_interrupts_inline())
        return cpu_b_interrupt_boundary(context, completed_pc, next_pc);
    const auto interrupt = host.consume_pending_interrupt(1U, 0x72U, completed_pc);
    const auto mask = static_cast<std::uint8_t>((registers.status >> 8U) & 7U);
    if (!interrupt.asserted || interrupt.level <= mask) return std::nullopt;

    const auto saved_status = registers.status;
    registers.address[7] -= 4U;
    write_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(next_pc));
    registers.address[7] -= 2U;
    write_word(host, registers.address[7], saved_status);
    write_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(next_pc >> 16U));
    registers.status = static_cast<std::uint16_t>(
        (saved_status & 0x38ffU) | 0x2000U
        | (static_cast<std::uint16_t>(interrupt.level) << 8U));
    const auto vector = static_cast<std::uint32_t>(24U + interrupt.level) * 4U;
    const auto target = read_long(host, vector);
    registers.program_counter = target;
    context.state = 0x04U;
    (void)host.call_function(static_cast<std::uint32_t>(96U + interrupt.level),
        1U, 0x04U, 6U, completed_pc, target, context);
    return FunctionResult::complete(5U, target);
}
} // namespace

FunctionResult cpu_b_indirect_record_dispatch(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto record = registers.address[4];
    const auto start_pc = registers.program_counter;
    if (start_pc != 0x0000dd00U && start_pc != 0x0000dd06U
        && start_pc != 0x0000dd4eU)
        return {TranslationStatus::contract_violation, 0U, start_pc};

    const auto run_case_one = [&]() -> FunctionResult {
        const auto old_position = read_word(host, record + 2U);
        const auto position = static_cast<std::uint16_t>(old_position - 8U);
        write_word(host, record + 2U, position);
        sub_word(registers, old_position, 8U, position, true);

        const auto mask_operand = read_word(host, record + 2U);
        const auto masked = static_cast<std::uint16_t>(mask_operand & 0x01ffU);
        write_word(host, record + 2U, masked);
        logic_word(registers, masked);
        return finish(host, registers);
    };

    if (start_pc == 0x0000dd4eU) return run_case_one();

    std::uint16_t selector{};
    if (start_pc == 0x0000dd00U) {
        selector = read_word(host, record);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | selector;
        logic_word(registers, selector);
        asl_word(registers, 2U);
        if (auto interrupt = check_interrupt(
                context, 0x0000dd04U, 0x0000dd06U))
            return *interrupt;
    } else {
        selector = static_cast<std::uint16_t>(registers.data[0]) >> 2U;
    }

    switch (selector) {
    case 0U:
        return finish(host, registers);
    case 1U: {
        if (auto interrupt = check_interrupt(context, 0x0000dd0eU, 0x0000dd4eU))
            return *interrupt;
        return run_case_one();
    }
    case 2U: {
        const auto position = read_word(host, record + 2U);
        sub_word(registers, position, 0x0030U,
            static_cast<std::uint16_t>(position - 0x0030U), false);
        if (position != 0x0030U) {
            const auto sub_operand = read_word(host, record + 2U);
            const auto decremented = static_cast<std::uint16_t>(sub_operand - 8U);
            write_word(host, record + 2U, decremented);
            sub_word(registers, sub_operand, 8U, decremented, true);
            const auto mask_operand = read_word(host, record + 2U);
            const auto masked = static_cast<std::uint16_t>(mask_operand & 0x01ffU);
            write_word(host, record + 2U, masked);
            logic_word(registers, masked);
            return finish(host, registers);
        }
        write_word(host, record, 3U);
        logic_word(registers, 3U);
        break;
    }
    case 3U:
        break;
    default:
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    }

    registers.address[0] = 0x0000e03eU;
    auto table_index = read_word(host, record + 4U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | table_index;
    logic_word(registers, table_index);
    const auto doubled = static_cast<std::uint16_t>(table_index + table_index);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | doubled;
    add_word(registers, table_index, table_index, doubled);
    const auto table_address = static_cast<std::uint32_t>(registers.address[0]
        + static_cast<std::int32_t>(static_cast<std::int16_t>(doubled)));
    const auto delta = read_word(host, table_address);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | delta;
    logic_word(registers, delta);
    if (delta == 0U) {
        (void)read_word(host, record);
        write_word(host, record, 0U);
        logic_word(registers, 0U);
        return finish(host, registers);
    }

    const auto old_position = read_word(host, record + 2U);
    const auto position = static_cast<std::uint16_t>(old_position - delta);
    write_word(host, record + 2U, position);
    sub_word(registers, old_position, delta, position, true);
    const auto mask_operand = read_word(host, record + 2U);
    const auto masked = static_cast<std::uint16_t>(mask_operand & 0x01ffU);
    write_word(host, record + 2U, masked);
    logic_word(registers, masked);
    const auto old_index = read_word(host, record + 4U);
    const auto next_index = static_cast<std::uint16_t>(old_index + 1U);
    write_word(host, record + 4U, next_index);
    add_word(registers, old_index, 1U, next_index);
    return finish(host, registers);
}

} // namespace gain_ground::translated
