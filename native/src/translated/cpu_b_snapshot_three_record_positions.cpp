#include "gain_ground/contract_types.h"

#include <cstdint>
#include <initializer_list>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
                     std::uint32_t sign_mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign_mask) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers, std::uint16_t destination,
                        std::uint16_t source, std::uint16_t result)
{
    const bool carry = static_cast<std::uint32_t>(destination) + source > 0xffffU;
    const bool overflow =
        ((~(destination ^ source) & (destination ^ result)) & 0x8000U) != 0U;
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

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

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto mask = static_cast<std::uint16_t>(
        (address & 1U) == 0U ? 0xff00U : 0x00ffU);
    const auto data = static_cast<std::uint16_t>(
        (address & 1U) == 0U ? static_cast<std::uint16_t>(value) << 8U : value);
    host.write_memory_word(kRegion, address & ~1U, data, mask);
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

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(
        kRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_snapshot_three_record_positions(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    for (const auto offset : {0x08U, 0x34U}) {
        const auto before = host.read_memory_word(
            kRegion, registers.address[5] + offset, kWordMask);
        const auto after = static_cast<std::uint16_t>(before + 1U);
        host.write_memory_word(
            kRegion, registers.address[5] + offset, after, kWordMask);
        set_add_word_flags(registers, before, 1U, after);
    }

    registers.address[4] = registers.address[5] + 0x10U;
    registers.address[3] = registers.address[5] + 0x0aU;
    registers.address[6] = 0x00001480U;
    registers.data[0] = 2U;
    set_logic_flags(registers, 2U, 0x80000000U);

    for (unsigned slot = 0; slot != 3U; ++slot) {
        registers.data[1] = 1U;
        set_logic_flags(registers, 1U, 0x80000000U);

        const auto state = host.read_memory_word(
            kRegion, registers.address[6] + 0x44U, kWordMask);
        set_compare_word_flags(registers, state, 5U);
        if (state != 5U) {
            registers.data[1] = 0U;
            set_logic_flags(registers, 0U, 0x80000000U);
        }

        const auto active = static_cast<std::uint8_t>(registers.data[1]);
        write_byte(host, registers.address[3], active);
        ++registers.address[3];
        set_logic_flags(registers, active, 0x80U);

        auto x = host.read_memory_word(
            kRegion, registers.address[6] + 0x12U, kWordMask);
        registers.data[2] = (registers.data[2] & 0xffff0000U) | x;
        set_logic_flags(registers, x, 0x8000U);
        auto y = host.read_memory_word(
            kRegion, registers.address[6] + 0x1aU, kWordMask);
        registers.data[3] = (registers.data[3] & 0xffff0000U) | y;
        set_logic_flags(registers, y, 0x8000U);

        set_logic_flags(registers,
            static_cast<std::uint16_t>(registers.data[1]), 0x8000U);
        if (static_cast<std::uint16_t>(registers.data[1]) == 0U) {
            x = 0xffffU;
            y = 0xffffU;
            registers.data[2] = (registers.data[2] & 0xffff0000U) | x;
            set_logic_flags(registers, x, 0x8000U);
            registers.data[3] = (registers.data[3] & 0xffff0000U) | y;
            set_logic_flags(registers, y, 0x8000U);
        }

        host.write_memory_word(kRegion, registers.address[4], x, kWordMask);
        registers.address[4] += 2U;
        set_logic_flags(registers, x, 0x8000U);
        host.write_memory_word(kRegion, registers.address[4], y, kWordMask);
        registers.address[4] += 2U;
        set_logic_flags(registers, y, 0x8000U);

        registers.address[6] += 0x80U;
        registers.data[0] = (registers.data[0] & 0xffff0000U)
            | static_cast<std::uint16_t>(registers.data[0] - 1U);
    }

    push_return(host, registers, 0x0002382aU);
    registers.program_counter = 0x00023836U;
    const auto child = host.call_function(414U, 1U, 0x72U, 2U,
        0x00023826U, 0x00023836U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
