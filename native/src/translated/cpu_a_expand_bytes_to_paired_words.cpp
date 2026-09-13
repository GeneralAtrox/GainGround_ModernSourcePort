#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

std::uint16_t read_shared(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kSharedRegion, address & kSharedMask, kWordMask);
}

std::uint8_t read_source_byte(FunctionContext &context, std::uint32_t pc,
    std::uint32_t address)
{
    const auto mask = static_cast<std::uint16_t>(
        (address & 1U) != 0U ? 0x00ffU : 0xff00U);
    std::uint16_t value{};
    if (address >= 0x00b00000U) {
        value = context.host->read_hardware(
            1U, context.cpu, context.state, pc, address & ~1U, mask);
    } else {
        value = context.host->read_memory_word(
            kProgramRegion, address & ~1U, mask);
    }
    return static_cast<std::uint8_t>(
        (address & 1U) != 0U ? value : (value >> 8U));
}

void set_logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = static_cast<std::uint16_t>(registers.status & 0x0010U);
    if ((value & 0x80U) != 0U)
        flags |= 0x0008U;
    if (value == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = static_cast<std::uint16_t>(registers.status & 0x0010U);
    if ((value & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (value == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_add_word(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result, bool affect_extend)
{
    const bool carry = static_cast<std::uint32_t>(left) + right > 0xffffU;
    const bool overflow = ((~(left ^ right) & (left ^ result)) & 0x8000U) != 0U;
    std::uint16_t flags = affect_extend
        ? 0U
        : static_cast<std::uint16_t>(registers.status & 0x0010U);
    if (carry)
        flags |= static_cast<std::uint16_t>(affect_extend ? 0x0011U : 0x0001U);
    if ((result & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (result == 0U)
        flags |= 0x0004U;
    if (overflow)
        flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

FunctionResult return_from_subroutine(FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto stack = registers.address[7] & kSharedMask;
    const auto target = (static_cast<std::uint32_t>(read_shared(host, stack)) << 16U)
        | read_shared(host, stack + 2U);
    registers.address[7] += 4U;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_00001cd8(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.data[0] = 0U;
    set_logic_word(registers, 0U);

    for (;;) {
        prefetch(host, 0x00001cdcU);
        const auto source = read_source_byte(
            context, 0x00001cdaU, registers.address[0]);
        ++registers.address[0];
        registers.data[0] = (registers.data[0] & 0xffffff00U) | source;
        set_logic_byte(registers, source);
        prefetch(host, 0x00001cdeU);

        const auto doubled = static_cast<std::uint16_t>(
            static_cast<std::uint16_t>(registers.data[0]) * 2U);
        const auto before_double = static_cast<std::uint16_t>(registers.data[0]);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | doubled;
        set_add_word(registers, before_double, before_double, doubled, true);
        prefetch(host, 0x00001ce0U);

        host.write_memory_word(kTileRegion,
            (registers.address[3] - 0x00200000U) & kSharedMask,
            doubled, kWordMask);
        registers.address[3] += 2U;
        set_logic_word(registers, doubled);
        prefetch(host, 0x00001ce2U);
        prefetch(host, 0x00001ce4U);

        const auto incremented = static_cast<std::uint16_t>(doubled + 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | incremented;
        set_add_word(registers, doubled, 1U, incremented, true);
        prefetch(host, 0x00001ce6U);

        host.write_memory_word(kTileRegion,
            (registers.address[3] + 0x7eU - 0x00200000U) & kSharedMask,
            incremented, kWordMask);
        set_logic_word(registers, incremented);
        prefetch(host, 0x00001ce8U);

        const auto counter = static_cast<std::uint16_t>(registers.data[1] - 1U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | counter;
        prefetch(host, 0x00001cdaU);
        if (counter == 0xffffU)
            break;
    }

    prefetch(host, 0x00001ceaU);
    prefetch(host, 0x00001cecU);
    return return_from_subroutine(context);
}

} // namespace gain_ground::translated
