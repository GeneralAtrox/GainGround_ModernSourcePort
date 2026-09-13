#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kConditionCodeMask = 0x000fU;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_logic_long_flags(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags{};
    if ((value & 0x80000000U) != 0U)
        flags |= kNegative;
    if (value == 0U)
        flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
}

void clear_long(ExecutionHost &host, CpuRegisters &registers, std::uint32_t address)
{
    (void)read_long(host, address);
    host.write_memory_word(kRegion, address + 2U, 0U, kWordMask);
    host.write_memory_word(kRegion, address, 0U, kWordMask);
    set_logic_long_flags(registers, 0U);
}

} // namespace

FunctionResult cpu_b_clear_record_motion_fields(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    const std::uint32_t base = context.registers.address[5];
    clear_long(host, context.registers, base + 0x1eU);
    clear_long(host, context.registers, base + 0x26U);
    const auto value = read_long(host, base + 0x22U);
    set_logic_long_flags(context.registers, value);
    if (static_cast<std::int32_t>(value) > 0)
        clear_long(host, context.registers, base + 0x22U);

    const std::uint32_t return_address = read_long(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
