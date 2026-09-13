#include "gain_ground/contract_types.h"
#include "gain_ground/cpu_b_interrupt.h"

#include <cstdint>
#include <optional>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic(CpuRegisters &registers, std::uint32_t value,
               std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(
        kPrivateRegion, address, kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(
        odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(
        kPrivateRegion, address & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? word : (word >> 8U));
}

std::optional<FunctionResult> check_interrupt(
    FunctionContext &context, std::uint32_t completed_pc,
    std::uint32_t next_pc)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    if (host.resumes_interrupts_inline())
        return cpu_b_interrupt_boundary(context, completed_pc, next_pc);
    const auto interrupt = host.consume_pending_interrupt(
        1U, 0x72U, completed_pc);
    const auto mask = static_cast<std::uint8_t>(
        (registers.status >> 8U) & 7U);
    if (!interrupt.asserted || interrupt.level <= mask)
        return std::nullopt;

    const auto saved_status = registers.status;
    registers.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(next_pc), kWordMask);
    registers.address[7] -= 2U;
    host.write_memory_word(kPrivateRegion, registers.address[7],
        saved_status, kWordMask);
    host.write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(next_pc >> 16U), kWordMask);
    registers.status = static_cast<std::uint16_t>(
        (saved_status & 0x38ffU) | 0x2000U
        | (static_cast<std::uint16_t>(interrupt.level) << 8U));
    const auto vector = static_cast<std::uint32_t>(
        24U + interrupt.level) * 4U;
    const auto target = read_long(host, vector);
    registers.program_counter = target;
    context.state = 0x04U;
    (void)host.call_function(
        static_cast<std::uint32_t>(96U + interrupt.level),
        1U, 0x04U, 6U, completed_pc, target, context);
    return FunctionResult::complete(5U, target);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return target;
}
} // namespace

FunctionResult proven_static_cpu_b_72_0000aa84(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto entry_pc = registers.program_counter;
    bool resume_at_aa9e = entry_pc == 0x0000aa9eU;
    bool resume_at_aaa8 = entry_pc == 0x0000aaa8U;

    if (!resume_at_aa9e && !resume_at_aaa8) {
        registers.address[0] = 0x00204000U;
        registers.address[1] = 0x0000b216U;
        registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x0700U;
        set_logic(registers, 0x0700U, 0x8000U);
        registers.data[2] = 0x00000017U;
        set_logic(registers, registers.data[2], 0x80000000U);
        registers.data[1] = 0x0000001fU;
        set_logic(registers, registers.data[1], 0x80000000U);
    }

    for (;;) {
        if (!resume_at_aaa8) {
            if (!resume_at_aa9e) {
                const auto value = read_byte(host, registers.address[1]);
                registers.address[1] += 1U;
                registers.data[0] =
                    (registers.data[0] & 0xffffff00U) | value;
                set_logic(registers, value, 0x80U);
                host.write_memory_word(kTileRegion,
                    registers.address[0] - 0x00200000U + 0x40U,
                    static_cast<std::uint16_t>(registers.data[0]),
                    kWordMask);
                set_logic(registers,
                    static_cast<std::uint16_t>(registers.data[0]),
                    0x8000U);
                if (auto interrupted = check_interrupt(
                        context, 0x0000aa9aU, 0x0000aa9eU))
                    return *interrupted;
            }
            const auto value = static_cast<std::uint16_t>(registers.data[0]);

            host.write_memory_word(kTileRegion,
                registers.address[0] - 0x00200000U + 0x0c00U,
                value, kWordMask);
            set_logic(registers, value, 0x8000U);
            host.write_memory_word(kTileRegion,
                registers.address[0] - 0x00200000U + 0x0c40U,
                value, kWordMask);
            set_logic(registers, value, 0x8000U);
            host.write_memory_word(kTileRegion,
                registers.address[0] - 0x00200000U,
                value, kWordMask);
            registers.address[0] += 2U;
            set_logic(registers, value, 0x8000U);
            if (auto interrupted = check_interrupt(
                    context, 0x0000aaa6U, 0x0000aaa8U))
                return *interrupted;
        }

        const auto d1 = static_cast<std::uint16_t>(registers.data[1] - 1U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | d1;
        resume_at_aa9e = false;
        resume_at_aaa8 = false;
        if (d1 != 0xffffU) {
            if (auto interrupted = check_interrupt(
                    context, 0x0000aaa8U, 0x0000aa98U))
                return *interrupted;
            continue;
        }
        registers.address[0] += 0x40U;
        const auto d2 = static_cast<std::uint16_t>(registers.data[2] - 1U);
        registers.data[2] = (registers.data[2] & 0xffff0000U) | d2;
        if (d2 == 0xffffU) break;
        registers.data[1] = 0x0000001fU;
        set_logic(registers, registers.data[1], 0x80000000U);
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
