#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] constexpr ByteLocation locate_byte(
    std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & 0x00fffffeU,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U),
        odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kPrivateRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address,
    std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kPrivateRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void set_logic_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_sub_byte_flags(CpuRegisters &registers,
    std::uint8_t left, std::uint8_t right, std::uint8_t result)
{
    const auto old_x = static_cast<std::uint16_t>(registers.status & 0x0010U);
    std::uint16_t flags{};
    if (left < right) flags |= 0x0001U;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags | old_x);
}

[[nodiscard]] FunctionResult tail_call(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target)
{
    context.registers.program_counter = target;
    return context.host->call_function(function_id, 1U, 0x72U, 1U,
        callsite, target, context);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001f06e(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto timer_address = registers.address[5] + 0x72U;

    const auto tested = read_byte(host, timer_address);
    set_logic_byte_flags(registers, tested);
    if (tested == 0U)
        return tail_call(context, 606U, 0x0001f072U, 0x0001f07aU);

    const auto before = read_byte(host, timer_address);
    const auto after = static_cast<std::uint8_t>(before - 1U);
    write_byte(host, timer_address, after);
    set_sub_byte_flags(registers, before, 1U, after);
    if (after != 0U)
        return tail_call(context, 607U, 0x0001f078U, 0x0001f096U);
    return tail_call(context, 606U, 0x0001f078U, 0x0001f07aU);
}

} // namespace gain_ground::translated
