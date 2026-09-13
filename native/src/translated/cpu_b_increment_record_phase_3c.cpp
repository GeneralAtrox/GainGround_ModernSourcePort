#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] ByteLocation locate_byte(std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void set_logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_add_byte(CpuRegisters &registers,
    std::uint8_t left, std::uint8_t right, std::uint8_t result)
{
    const auto wide = static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(left) + static_cast<std::uint16_t>(right));
    std::uint16_t flags{};
    if (wide > 0xffU) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_compare_byte(CpuRegisters &registers,
    std::uint8_t left, std::uint8_t right, std::uint8_t result)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if (right > left) flags |= 0x0001U;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_asl_two_word(CpuRegisters &registers, std::uint16_t original, std::uint16_t result)
{
    auto shifted = original;
    bool carry{};
    bool overflow{};
    for (unsigned count = 0; count != 2U; ++count) {
        const bool old_sign = (shifted & 0x8000U) != 0U;
        carry = old_sign;
        shifted = static_cast<std::uint16_t>(shifted << 1U);
        overflow = overflow || (old_sign != ((shifted & 0x8000U) != 0U));
    }
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if (overflow) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(kRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(kRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_increment_record_phase_3c(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto phase_address = registers.address[5] + 0x3cU;
    const auto old_phase = read_byte(host, phase_address);
    const auto phase = static_cast<std::uint8_t>(old_phase + 1U);
    write_byte(host, phase_address, phase);
    set_add_byte(registers, old_phase, 1U, phase);

    const auto compared_phase = read_byte(host, phase_address);
    set_compare_byte(registers, compared_phase, 5U,
        static_cast<std::uint8_t>(compared_phase - 5U));
    if (compared_phase >= 5U) {
        (void)read_byte(host, phase_address);
        write_byte(host, phase_address, 0U);
        set_logic_byte(registers, 0U);

        const auto subphase_address = registers.address[5] + 0x3dU;
        const auto old_subphase = read_byte(host, subphase_address);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | old_subphase;
        set_logic_byte(registers, old_subphase);
        const auto incremented = static_cast<std::uint8_t>(old_subphase + 1U);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | incremented;
        set_add_byte(registers, old_subphase, 1U, incremented);
        const auto wrapped = static_cast<std::uint8_t>(incremented & 3U);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | wrapped;
        set_logic_byte(registers, wrapped);
        write_byte(host, subphase_address, wrapped);
        set_logic_byte(registers, wrapped);
    }

    const auto base = host.read_memory_word(kRegion, registers.address[5] + 0x58U, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | base;
    set_logic_word(registers, base);
    const auto scaled = static_cast<std::uint16_t>(base << 2U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | scaled;
    set_asl_two_word(registers, base, scaled);

    const auto subphase = read_byte(host, registers.address[5] + 0x3dU);
    const auto old_low = static_cast<std::uint8_t>(registers.data[0]);
    const auto combined = static_cast<std::uint8_t>(old_low + subphase);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | combined;
    set_add_byte(registers, old_low, subphase, combined);
    host.write_memory_word(kRegion, registers.address[5] + 0x52U,
        static_cast<std::uint16_t>(registers.data[0]), kWordMask);
    set_logic_word(registers, static_cast<std::uint16_t>(registers.data[0]));

    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
