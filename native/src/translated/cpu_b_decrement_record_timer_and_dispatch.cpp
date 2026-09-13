#include "gain_ground/contract_types.h"

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

void set_subtract_word_flags(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source, std::uint16_t result)
{
    std::uint16_t flags = 0U;
    if (destination < source) flags |= 0x0011U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(kPrivateRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_decrement_record_timer_and_dispatch(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto timer_address = registers.address[5] + 0x10U;
    const auto timer = host.read_memory_word(kPrivateRegion, timer_address, kWordMask);
    const auto updated = static_cast<std::uint16_t>(timer - 0x1010U);
    host.write_memory_word(kPrivateRegion, timer_address, updated, kWordMask);
    set_subtract_word_flags(registers, timer, 0x1010U, updated);

    const bool greater_than_zero = (registers.status & 0x0004U) == 0U
        && (((registers.status & 0x0008U) != 0U)
            == ((registers.status & 0x0002U) != 0U));
    const auto record_state = locate_byte(registers.address[5]);
    if (!greater_than_zero) {
        (void)read_byte(host, record_state);
        write_byte(host, record_state, 0U);
        const auto extend = static_cast<std::uint16_t>(registers.status & 0x0010U);
        registers.status = static_cast<std::uint16_t>(
            (registers.status & ~0x001fU) | extend | 0x0004U);
    } else {
        const auto state = read_byte(host, record_state);
        write_byte(host, record_state, static_cast<std::uint8_t>(state & ~0x01U));
        if ((state & 0x01U) == 0U)
            registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
        else
            registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);

        push_return(host, registers, 0x00012d00U);
        registers.program_counter = 0x00015d24U;
        auto child = host.call_function(280U, 1U, 0x72U, 2U,
            0x00012cfaU, 0x00015d24U, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;

        push_return(host, registers, 0x00012d06U);
        registers.program_counter = 0x00015df2U;
        child = host.call_function(282U, 1U, 0x72U, 2U,
            0x00012d00U, 0x00015df2U, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
