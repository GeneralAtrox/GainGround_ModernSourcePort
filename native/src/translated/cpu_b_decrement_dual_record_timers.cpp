#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U),
        odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host,
    std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address,
    std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void set_subtract_byte_flags(CpuRegisters &registers,
    std::uint8_t destination, std::uint8_t source, std::uint8_t result)
{
    std::uint16_t flags{};
    if (destination < source) flags |= 0x0011U;
    if (((destination ^ source) & (destination ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] bool signed_greater_than_zero(const CpuRegisters &registers)
{
    return (registers.status & 0x0004U) == 0U
        && (((registers.status & 0x0008U) != 0U)
            == ((registers.status & 0x0002U) != 0U));
}

void set_clear_byte_flags(CpuRegisters &registers)
{
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x000fU) | 0x0004U);
}

void set_bit_zero(CpuRegisters &registers, bool bit_was_set)
{
    if (bit_was_set)
        registers.status = static_cast<std::uint16_t>(
            registers.status & ~0x0004U);
    else
        registers.status = static_cast<std::uint16_t>(
            registers.status | 0x0004U);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host,
    CpuRegisters &registers)
{
    const auto high = host.read_memory_word(
        kRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_decrement_dual_record_timers(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];

    auto timer = read_byte(host, base + 0x10U);
    auto updated = static_cast<std::uint8_t>(timer - 0x10U);
    write_byte(host, base + 0x10U, updated);
    set_subtract_byte_flags(registers, timer, 0x10U, updated);

    if (signed_greater_than_zero(registers)) {
        timer = read_byte(host, base + 0x11U);
        updated = static_cast<std::uint8_t>(timer - 0x10U);
        write_byte(host, base + 0x11U, updated);
        set_subtract_byte_flags(registers, timer, 0x10U, updated);

        if (signed_greater_than_zero(registers)) {
            const auto state = read_byte(host, base);
            write_byte(host, base, static_cast<std::uint8_t>(state & ~0x01U));
            set_bit_zero(registers, (state & 0x01U) != 0U);

            push_return(host, registers, 0x00012d32U);
            registers.program_counter = 0x00015d24U;
            auto child = host.call_function(280U, 1U, 0x72U, 2U,
                0x00012d2cU, 0x00015d24U, context);
            if (child.status != TranslationStatus::complete
                || child.control != 1U)
                return child;

            push_return(host, registers, 0x00012d38U);
            registers.program_counter = 0x00015df2U;
            child = host.call_function(282U, 1U, 0x72U, 2U,
                0x00012d32U, 0x00015df2U, context);
            if (child.status != TranslationStatus::complete
                || child.control != 1U)
                return child;

            const auto target = pop_return(host, registers);
            registers.program_counter = target;
            return FunctionResult::complete(1U, target);
        }
    }

    (void)read_byte(host, base);
    write_byte(host, base, 0U);
    set_clear_byte_flags(registers);
    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
