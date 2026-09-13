#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kByteMask = 0x00ffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

std::uint8_t read_program_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto word = host.read_memory_word(kProgramRegion, address & ~1U,
        (address & 1U) == 0U ? 0xff00U : kByteMask);
    return static_cast<std::uint8_t>((address & 1U) == 0U ? word >> 8U : word);
}

std::uint8_t read_fdc(ExecutionHost &host, std::uint32_t pc,
    std::uint32_t address)
{
    return static_cast<std::uint8_t>(host.read_hardware(
        1U, 0U, 0xffU, pc, address & ~1U, kByteMask));
}

void write_fdc(ExecutionHost &host, std::uint32_t pc,
    std::uint32_t address, std::uint8_t value)
{
    host.write_hardware(2U, 0U, 0xffU, pc, address & ~1U,
        static_cast<std::uint16_t>(value) * 0x0101U, kByteMask);
}

void set_logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_logic_long(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_bit_test(CpuRegisters &registers, bool bit_set)
{
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x0004U) | (bit_set ? 0U : 0x0004U));
}

FunctionResult return_from_subroutine(FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto stack = registers.address[7] & 0x0003ffffU;
    const auto target =
        (static_cast<std::uint32_t>(host.read_memory_word(
            kSharedRegion, stack, kWordMask)) << 16U)
        | host.read_memory_word(kSharedRegion,
            (stack + 2U) & 0x0003ffffU, kWordMask);
    registers.address[7] += 4U;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_00000df6(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.data[1] = 0U;
    set_logic_long(registers, 0U);

    prefetch(host, 0x00000dfaU);
    const auto count = read_program_byte(host, registers.address[0]++);
    registers.data[1] = count;
    set_logic_byte(registers, count);

    prefetch(host, 0x00000dfcU);
    const auto value = read_program_byte(host, registers.address[0]++);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | value;
    set_logic_byte(registers, value);

    bool first_iteration = true;
    for (;;) {
        if (first_iteration) {
            prefetch(host, 0x00000dfeU);
        } else {
            prefetch(host, 0x00000dfcU);
            prefetch(host, 0x00000dfeU);
        }
        first_iteration = false;
        prefetch(host, 0x00000e00U);
        prefetch(host, 0x00000e02U);
        const auto bit0_status = read_fdc(
            host, 0x00000dfcU, registers.address[5] + 8U);
        set_bit_test(registers, (bit0_status & 0x01U) != 0U);
        prefetch(host, 0x00000e04U);
        registers.status = host.apply_controlled_status(0U, 0xffU,
            0x00000e02U, (registers.address[5] + 8U) & ~1U,
            registers.status);

        if ((registers.status & 0x0004U) != 0U) {
            prefetch(host, 0x00000e06U);
            prefetch(host, 0x00000e08U);
            prefetch(host, 0x00000e0aU);
            const auto bit1_status = read_fdc(
                host, 0x00000e04U, registers.address[5] + 8U);
            set_bit_test(registers, (bit1_status & 0x02U) != 0U);
            prefetch(host, 0x00000e0cU);
            registers.status = host.apply_controlled_status(0U, 0xffU,
                0x00000e0aU, (registers.address[5] + 8U) & ~1U,
                registers.status);
            if ((registers.status & 0x0004U) != 0U) {
                continue;
            }
            prefetch(host, 0x00000e0eU);
            prefetch(host, 0x0000196cU);
            prefetch(host, 0x0000196eU);
            registers.program_counter = 0x0000196cU;
            return host.call_function(613U, 0U, 0xffU, 1U,
                0x00000e0cU, 0x0000196cU, context);
        }

        prefetch(host, 0x00000e10U);
        prefetch(host, 0x00000e12U);
        prefetch(host, 0x00000e14U);
        write_fdc(host, 0x00000e10U, registers.address[5] + 6U,
            static_cast<std::uint8_t>(registers.data[0]));
        set_logic_byte(registers, static_cast<std::uint8_t>(registers.data[0]));

        prefetch(host, 0x00000e16U);
        const auto d1 = static_cast<std::uint16_t>(registers.data[1] - 1U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | d1;
        if (d1 != 0xffffU) {
            continue;
        }

        prefetch(host, 0x00000dfcU);
        prefetch(host, 0x00000e18U);
        prefetch(host, 0x00000e1aU);
        const auto d4 = static_cast<std::uint16_t>(registers.data[4] - 1U);
        registers.data[4] = (registers.data[4] & 0xffff0000U) | d4;
        if (d4 != 0xffffU) {
            prefetch(host, 0x00000df6U);
            prefetch(host, 0x00000df8U);
            registers.program_counter = 0x00000df6U;
            if (host.consume_self_continuation_boundary(437U, 0U, 0xffU,
                    1U, 0x00000e18U, 0x00000df6U, context))
                return FunctionResult::complete(4U, 0x00000df6U);
            return host.call_function(437U, 0U, 0xffU, 1U,
                0x00000e18U, 0x00000df6U, context);
        }

        prefetch(host, 0x00000df6U);
        prefetch(host, 0x00000e1cU);
        prefetch(host, 0x00000e1eU);
        return return_from_subroutine(context);
    }
}

} // namespace gain_ground::translated
