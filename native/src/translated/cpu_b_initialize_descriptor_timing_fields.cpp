#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kPrivateRegion, address, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kPrivateRegion, address, value, kWordMask);
}

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word(CpuRegisters &registers, std::uint16_t left,
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

void arithmetic_shift_left_word(CpuRegisters &registers, unsigned count)
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

    std::uint16_t flags{};
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    if (carry) flags |= 0x0011U;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void clear_word(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address)
{
    (void)read_word(host, address);
    write_word(host, address, 0U);
    set_logic_word(registers, 0U);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return target;
}
} // namespace

FunctionResult cpu_b_initialize_descriptor_timing_fields(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.address[0] = read_long(host, registers.address[3] + 0x18U);

    auto d0 = read_word(host, registers.address[0]);
    registers.address[0] += 2U;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    set_logic_word(registers, d0);

    write_word(host, registers.address[6] + 0x16U, d0);
    set_logic_word(registers, d0);
    clear_word(host, registers, registers.address[6] + 0x18U);
    write_word(host, registers.address[6] + 0x2eU, d0);
    set_logic_word(registers, d0);
    write_word(host, registers.address[6] + 0x30U, d0);
    set_logic_word(registers, d0);

    d0 = read_word(host, registers.address[5] + 0x58U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    set_logic_word(registers, d0);
    arithmetic_shift_left_word(registers, 2U);
    registers.address[0] = static_cast<std::uint32_t>(
        registers.address[0] + static_cast<std::int32_t>(
            static_cast<std::int16_t>(registers.data[0])));

    d0 = read_word(host, registers.address[5] + 0x12U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    set_logic_word(registers, d0);
    write_word(host, registers.address[6] + 0x2aU, d0);
    set_logic_word(registers, d0);
    write_word(host, registers.address[6] + 0x2cU, d0);
    set_logic_word(registers, d0);

    auto adjustment = read_word(host, registers.address[0]);
    registers.address[0] += 2U;
    auto sum = static_cast<std::uint16_t>(d0 + adjustment);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | sum;
    set_add_word(registers, d0, adjustment, sum);
    write_word(host, registers.address[6] + 0x12U, sum);
    set_logic_word(registers, sum);
    clear_word(host, registers, registers.address[6] + 0x14U);

    d0 = read_word(host, registers.address[5] + 0x1aU);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    set_logic_word(registers, d0);
    write_word(host, registers.address[6] + 0x32U, d0);
    set_logic_word(registers, d0);
    write_word(host, registers.address[6] + 0x34U, d0);
    set_logic_word(registers, d0);

    adjustment = read_word(host, registers.address[0]);
    registers.address[0] += 2U;
    sum = static_cast<std::uint16_t>(d0 + adjustment);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | sum;
    set_add_word(registers, d0, adjustment, sum);
    write_word(host, registers.address[6] + 0x1aU, sum);
    set_logic_word(registers, sum);
    clear_word(host, registers, registers.address[6] + 0x1cU);

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
