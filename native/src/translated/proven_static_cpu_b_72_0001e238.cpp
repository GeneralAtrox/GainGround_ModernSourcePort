#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U),
        odd ? 0U : 8U};
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
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

void set_sub_byte_flags(CpuRegisters &registers, std::uint8_t left,
    std::uint8_t right, std::uint8_t result)
{
    std::uint16_t flags{};
    if (right > left) flags |= 0x0011U;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_bit_zero(CpuRegisters &registers, bool bit_was_set)
{
    if (bit_was_set)
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);
    else
        registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001e238(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto timer_address = registers.address[5] + 0x59U;
    const auto prior_timer = read_byte(host, timer_address);
    const auto timer = static_cast<std::uint8_t>(prior_timer - 1U);
    write_byte(host, timer_address, timer);
    set_sub_byte_flags(registers, prior_timer, 1U, timer);

    if (timer != 0U) {
        registers.program_counter = 0x0001e244U;
        return host.call_function(599U, 1U, 0x72U, 1U,
            0x0001e23cU, 0x0001e244U, context);
    }

    const auto flag_address = registers.address[5] + 0x40U;
    const auto prior_flag = read_byte(host, flag_address);
    write_byte(host, flag_address, static_cast<std::uint8_t>(prior_flag & ~0x80U));
    set_bit_zero(registers, (prior_flag & 0x80U) != 0U);
    registers.program_counter = 0x0001e244U;
    return host.call_function(599U, 1U, 0x72U, 0U,
        0x0001e23eU, 0x0001e244U, context);
}

} // namespace gain_ground::translated
