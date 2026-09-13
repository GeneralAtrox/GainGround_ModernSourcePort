#include "gain_ground/contract_types.h"
#include "cpu_b_step_record_phase_counters_common.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

ByteLocation locate_byte(std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

std::uint8_t read_byte(ExecutionHost &host, ByteLocation location)
{
    return static_cast<std::uint8_t>(host.read_memory_word(
        kPrivateRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, ByteLocation location, std::uint8_t value)
{
    host.write_memory_word(kPrivateRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void set_logic_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    const auto extend = static_cast<std::uint16_t>(registers.status & 0x0010U);
    std::uint16_t flags = extend;
    if (value == 0U) flags |= 0x0004U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_subtract_byte_flags(CpuRegisters &registers,
    std::uint8_t destination, std::uint8_t source, std::uint8_t result)
{
    std::uint16_t flags = 0U;
    if (destination < source) flags |= 0x0011U;
    if (((destination ^ source) & (destination ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_byte_flags(CpuRegisters &registers,
    std::uint8_t destination, std::uint8_t source, std::uint8_t result)
{
    const auto wide = static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(destination) + static_cast<std::uint16_t>(source));
    std::uint16_t flags = 0U;
    if (wide > 0xffU) flags |= 0x0011U;
    if ((~(destination ^ source) & (destination ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(kPrivateRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_step_record_phase_counters_common(
    FunctionContext &context, bool start_at_counter) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto counter = locate_byte(registers.address[5] + 0x3cU);
    const auto phase = locate_byte(registers.address[5] + 0x3dU);

    bool step_counter = start_at_counter;
    if (!start_at_counter) {
        const auto phase_value = read_byte(host, phase);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | phase_value;
        set_logic_byte_flags(registers, phase_value);
        const auto phase_minus_one = static_cast<std::uint8_t>(phase_value - 1U);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | phase_minus_one;
        set_subtract_byte_flags(registers, phase_value, 1U, phase_minus_one);
        step_counter = phase_minus_one != 0U;
    }

    if (step_counter) {
        const auto counter_value = read_byte(host, counter);
        const auto decremented = static_cast<std::uint8_t>(counter_value - 1U);
        write_byte(host, counter, decremented);
        set_subtract_byte_flags(registers, counter_value, 1U, decremented);
        const bool greater_than_zero = (registers.status & 0x0004U) == 0U
            && (((registers.status & 0x0008U) != 0U)
                == ((registers.status & 0x0002U) != 0U));
        if (!greater_than_zero) {
            write_byte(host, counter, 2U);
            set_logic_byte_flags(registers, 2U);

            const auto old_phase = read_byte(host, phase);
            const auto incremented = static_cast<std::uint8_t>(old_phase + 1U);
            write_byte(host, phase, incremented);
            set_add_byte_flags(registers, old_phase, 1U, incremented);

            const auto unmasked = read_byte(host, phase);
            const auto masked = static_cast<std::uint8_t>(unmasked & 0x03U);
            write_byte(host, phase, masked);
            set_logic_byte_flags(registers, masked);
        }
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

FunctionResult cpu_b_step_record_phase_counters(FunctionContext &context) noexcept
{
    return cpu_b_step_record_phase_counters_common(context, false);
}

} // namespace gain_ground::translated
