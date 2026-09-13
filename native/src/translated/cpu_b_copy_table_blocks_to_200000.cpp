#include "gain_ground/contract_types.h"
#include "gain_ground/cpu_b_interrupt.h"

#include <cstdint>
#include <optional>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_move_flags(CpuRegisters &registers, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kPrivateRegion, address, kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(kPrivateRegion, address & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? word : (word >> 8U));
}

[[nodiscard]] std::optional<FunctionResult> check_interrupt(
    FunctionContext &context, std::uint32_t completed_pc, std::uint32_t next_pc)
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
    host.write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(next_pc), kWordMask);
    registers.address[7] -= 2U;
    host.write_memory_word(kPrivateRegion, registers.address[7], saved_status, kWordMask);
    host.write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(next_pc >> 16U), kWordMask);
    registers.status = static_cast<std::uint16_t>((saved_status & 0x38ffU) | 0x2000U
        | (static_cast<std::uint16_t>(interrupt.level) << 8U));
    const auto vector = static_cast<std::uint32_t>(24U + interrupt.level) * 4U;
    const auto target = read_long(host, vector);
    registers.program_counter = target;
    context.state = 0x04U;
    (void)host.call_function(static_cast<std::uint32_t>(96U + interrupt.level),
        1U, 0x04U, 6U, completed_pc, target, context);
    return FunctionResult::complete(5U, target);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto result = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return result;
}
} // namespace

FunctionResult cpu_b_copy_table_blocks_to_200000(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    const auto entry_pc = registers.program_counter;
    const bool resume_active = entry_pc == 0x00009d2aU
        || entry_pc == 0x00009d2cU || entry_pc == 0x00009d2eU
        || entry_pc == 0x00009d32U || entry_pc == 0x00009d36U
        || entry_pc == 0x00009d3aU || entry_pc == 0x00009d3eU;
    if (!resume_active) {
        registers.address[4] = 0x00009fb0U;
        registers.data[5] = 0x0000000aU;
        set_move_flags(registers, registers.data[5], 0x80000000U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x0900U;
        set_move_flags(registers, 0x0900U, 0x8000U);
    }

    const auto run_pixels = [&](std::uint32_t first_pc) -> std::optional<FunctionResult> {
        auto pc = first_pc;
        for (;;) {
            if (pc == 0x00009d2aU) {
                const auto value = read_byte(host, registers.address[2]);
                registers.address[2] += 1U;
                registers.data[0] = (registers.data[0] & 0xffffff00U) | value;
                set_move_flags(registers, value, 0x80U);
                if (auto result = check_interrupt(context, 0x00009d2aU, 0x00009d2cU))
                    return result;
                pc = 0x00009d2cU;
            }
            if (pc == 0x00009d2cU) {
                host.write_memory_word(kTileRegion,
                    registers.address[0] - 0x00200000U,
                    static_cast<std::uint16_t>(registers.data[0]), kWordMask);
                registers.address[0] += 2U;
                set_move_flags(registers,
                    static_cast<std::uint16_t>(registers.data[0]), 0x8000U);
                if (auto result = check_interrupt(context, 0x00009d2cU, 0x00009d2eU))
                    return result;
            }
            const auto d1_counter = static_cast<std::uint16_t>(registers.data[1] - 1U);
            registers.data[1] = (registers.data[1] & 0xffff0000U) | d1_counter;
            const bool repeat_d1 = d1_counter != 0xffffU;
            if (auto result = check_interrupt(context, 0x00009d2eU,
                    repeat_d1 ? 0x00009d2aU : 0x00009d32U)) return result;
            if (!repeat_d1) return std::nullopt;
            pc = 0x00009d2aU;
        }
    };

    bool first_d5 = true;
    for (;;) {
        if (!resume_active || !first_d5) {
        const auto d4 = host.read_memory_word(kPrivateRegion, registers.address[4], kWordMask);
        registers.address[4] += 2U;
        registers.data[4] = (registers.data[4] & 0xffff0000U) | d4;
        set_move_flags(registers, d4, 0x8000U);
        registers.address[3] = read_long(host, registers.address[4]);
        registers.address[4] += 4U;
        }
        bool first_d4 = true;
        for (;;) {
            if (!resume_active || !first_d5 || !first_d4) {
            registers.address[1] = 0x00200000U;
            const auto displacement = host.read_memory_word(
                kPrivateRegion, registers.address[4], kWordMask);
            registers.address[4] += 2U;
            registers.address[1] += static_cast<std::int16_t>(displacement);
            registers.address[2] = registers.address[3];
            const auto d2 = host.read_memory_word(kPrivateRegion, registers.address[2], kWordMask);
            registers.address[2] += 2U;
            registers.data[2] = (registers.data[2] & 0xffff0000U) | d2;
            set_move_flags(registers, d2, 0x8000U);
            const auto d3 = host.read_memory_word(kPrivateRegion, registers.address[2], kWordMask);
            registers.address[2] += 2U;
            registers.data[3] = (registers.data[3] & 0xffff0000U) | d3;
            set_move_flags(registers, d3, 0x8000U);
            }
            bool first_row = true;
            const bool skip_rows = resume_active && first_d5 && first_d4
                && entry_pc >= 0x00009d3aU;
            while (!skip_rows) {
                const bool resume_first_row = resume_active && first_d5 && first_d4 && first_row;
                if (!resume_first_row) {
                registers.address[0] = registers.address[1];
                registers.data[1] = (registers.data[1] & 0xffff0000U)
                    | static_cast<std::uint16_t>(registers.data[2]);
                set_move_flags(registers, static_cast<std::uint16_t>(registers.data[1]), 0x8000U);
                }
                if (!resume_first_row || entry_pc <= 0x00009d2eU) {
                    const auto pixel_pc = resume_first_row ? entry_pc : 0x00009d2aU;
                    if (auto result = run_pixels(pixel_pc)) return *result;
                }
                if (!resume_first_row || entry_pc <= 0x00009d32U)
                    registers.address[1] += 0x80U;
                const auto d3_counter = static_cast<std::uint16_t>(registers.data[3] - 1U);
                registers.data[3] = (registers.data[3] & 0xffff0000U) | d3_counter;
                if (d3_counter == 0xffffU) break;
                first_row = false;
            }
            if (resume_active && first_d5 && first_d4 && entry_pc == 0x00009d3eU)
                break;
            {
                const auto d4_counter = static_cast<std::uint16_t>(registers.data[4] - 1U);
                registers.data[4] = (registers.data[4] & 0xffff0000U) | d4_counter;
                if (d4_counter == 0xffffU) break;
            }
            first_d4 = false;
        }
        const auto d5_counter = static_cast<std::uint16_t>(registers.data[5] - 1U);
        registers.data[5] = (registers.data[5] & 0xffff0000U) | d5_counter;
        if (d5_counter == 0xffffU) break;
        first_d5 = false;
    }

    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
