#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kTimerAddress = 0x00a00000U;

void set_compare_word_flags(CpuRegisters &registers,
                            std::uint16_t destination,
                            std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (destination < source) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers,
                        std::uint16_t destination,
                        std::uint16_t source,
                        std::uint16_t result)
{
    const bool carry = static_cast<std::uint32_t>(destination) + source > 0xffffU;
    const bool overflow = ((~(destination ^ source) & (destination ^ result)) & 0x8000U) != 0U;
    std::uint16_t flags = 0U;
    if (carry) flags |= 0x0011U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
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
    if (((destination ^ source) & (destination ^ result) & 0x80000000U) != 0U)
        flags |= 0x0002U;
    if (destination < source) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

std::uint16_t read_timer(ExecutionHost &host, std::uint32_t pc)
{
    return host.read_hardware(1U, 1U, 0x58U, pc, kTimerAddress, kWordMask);
}
} // namespace

FunctionResult cpu_b_state58_timer_calibration(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.data[1] = 0xffffffffU;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x000fU) | 0x0008U);

    const auto initial = read_timer(host, 0x0000849eU);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | initial;

    for (;;) {
        const auto current = read_timer(host, 0x000084a4U);
        set_compare_word_flags(registers,
            static_cast<std::uint16_t>(registers.data[0]), current);
        if (current != static_cast<std::uint16_t>(registers.data[0]))
            break;
    }

    const auto before_add = static_cast<std::uint16_t>(registers.data[0]);
    const auto after_add = static_cast<std::uint16_t>(before_add + 0x0200U);
    set_add_word_flags(registers, before_add, 0x0200U, after_add);
    const auto target = static_cast<std::uint16_t>(after_add & 0x0fffU);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | target;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x000fU)
        | (target == 0U ? 0x0004U : 0U)
        | ((target & 0x8000U) != 0U ? 0x0008U : 0U));

    for (;;) {
        const auto current = read_timer(host, 0x000084b4U);
        set_compare_word_flags(registers, target, current);
        if (current == target)
            break;
        const auto counter = static_cast<std::uint16_t>(registers.data[1] - 1U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | counter;
        if (counter == 0xffffU)
            break;
    }

    const auto counter = static_cast<std::uint16_t>(registers.data[1]);
    const auto calibrated = static_cast<std::uint16_t>(counter + 0x1b5fU);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | calibrated;
    set_add_word_flags(registers, counter, 0x1b5fU, calibrated);

    if ((calibrated & 0x8000U) != 0U)
        host.write_memory_word(kPrivateRegion, 0x00000404U, 0xff00U, 0xff00U);

    host.write_hardware(2U, 1U, 0x58U, 0x000084c8U,
        0x00a00004U, 0x0018U, kWordMask);
    registers.status = 0x2000U;
    set_compare_long_flags(registers, registers.data[0], 0x00ddffffU);
    registers.program_counter = 0x000084daU;
    (void)host.call_function(424U, 1U, 0xddU, 6U,
        0x000084d4U, 0x000084daU, context);
    return FunctionResult::complete(5U, 0x000084daU);
}

} // namespace gain_ground::translated
