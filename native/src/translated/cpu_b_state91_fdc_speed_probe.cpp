#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kPrivateOffsetMask = 0x0003ffffU;
constexpr std::uint32_t kFdcStatusAddress = 0x00b00008U;

void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
                     std::uint32_t negative_mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & negative_mask) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_long_flags(CpuRegisters &registers, std::uint32_t before,
                        std::uint32_t result)
{
    const bool carry = result < before;
    const bool overflow = ((~(before ^ 1U) & (before ^ result))
        & 0x80000000U) != 0U;
    std::uint16_t flags = 0U;
    if (carry) flags |= 0x0011U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_compare_long_flags(CpuRegisters &registers,
                            std::uint32_t destination,
                            std::uint32_t source)
{
    const auto result = destination - source;
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result)
        & 0x80000000U) != 0U) flags |= 0x0002U;
    if (destination < source) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_long(ExecutionHost &host, CpuRegisters &registers,
               std::uint32_t value)
{
    registers.address[7] -= 4U;
    const auto offset = registers.address[7] & kPrivateOffsetMask;
    host.write_memory_word(kPrivateRegion, offset,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, offset + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] FunctionResult invoke_delay(ExecutionHost &host,
                                           FunctionContext &context)
{
    push_long(host, context.registers, 0x0000845cU);
    context.registers.program_counter = 0x00008462U;
    return host.call_function(423U, 1U, 0x91U, 2U,
        0x0000845aU, 0x00008462U, context);
}

[[nodiscard]] std::uint16_t read_status(ExecutionHost &host,
                                        std::uint32_t pc)
{
    return host.read_hardware(1U, 1U, 0x91U, pc,
        kFdcStatusAddress, 0x00ffU);
}
} // namespace

FunctionResult cpu_b_state91_fdc_speed_probe(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.data[1] = (registers.data[1] & 0xffff0000U) | 0x003cU;
    set_logic_flags(registers, 0x003cU, 0x8000U);
    for (;;) {
        const auto child = invoke_delay(host, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
        const auto counter = static_cast<std::uint16_t>(registers.data[1] - 1U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | counter;
        if (counter == 0xffffU)
            break;
    }

    while ((read_status(host, 0x0000846cU) & 0x0020U) != 0U)
        set_logic_flags(registers, 0x0020U, 0x0080U);
    set_logic_flags(registers, 0U, 0x0080U);

    while ((read_status(host, 0x00008472U) & 0x0020U) == 0U)
        set_logic_flags(registers, 0U, 0x0080U);
    set_logic_flags(registers, 0x0020U, 0x0080U);

    registers.data[0] = 0U;
    set_logic_flags(registers, 0U, 0x80000000U);
    do {
        const auto before = registers.data[0];
        ++registers.data[0];
        set_add_long_flags(registers, before, registers.data[0]);
        const auto status = read_status(host, 0x0000847cU);
        set_logic_flags(registers, status & 0x0020U, 0x0080U);
        if ((status & 0x0020U) == 0U)
            break;
    } while (true);

    registers.data[1] = 1U;
    set_logic_flags(registers, 1U, 0x80000000U);
    set_compare_long_flags(registers, registers.data[0], 0x0000dd5bU);

    host.write_hardware(2U, 1U, 0x91U, 0x0000848eU,
        kFdcStatusAddress, 0x0909U, 0x00ffU);
    set_logic_flags(registers, 9U, 0x0080U);

    host.write_memory_word(kSharedRegion, 0x00038002U,
        static_cast<std::uint16_t>(registers.data[1]), kWordMask);
    set_logic_flags(registers,
        static_cast<std::uint16_t>(registers.data[1]), 0x8000U);

    set_compare_long_flags(registers, registers.data[0], 0x0058ffffU);
    registers.program_counter = 0x0000849cU;
    (void)host.call_function(102U, 1U, 0x58U, 6U,
        0x00008496U, 0x0000849cU, context);
    return FunctionResult::complete(5U, 0x0000849cU);
}

} // namespace gain_ground::translated
