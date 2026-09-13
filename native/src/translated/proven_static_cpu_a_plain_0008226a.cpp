#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kMemoryMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(
        kSharedRegion, address & kMemoryMask, kWordMask);
}

[[nodiscard]] std::uint16_t read_shared_word(
    ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(
        kSharedRegion, address & kMemoryMask, kWordMask);
}

[[nodiscard]] std::uint8_t read_private_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(
        kPrivateRegion, address & 0x0003fffeU, mask);
    return static_cast<std::uint8_t>(odd ? word : (word >> 8U));
}

void write_shared_word(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value)
{
    host.write_memory_word(
        kSharedRegion, address & kMemoryMask, value, kWordMask);
}

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_move_long_flags(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_bit_test_flags(CpuRegisters &registers, bool bit_was_set)
{
    if (bit_was_set)
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);
    else
        registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
}

void set_add_word_flags(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(left) + right > 0xffffU)
        flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_compare_word_flags(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    const auto old_x = static_cast<std::uint16_t>(registers.status & 0x0010U);
    std::uint16_t flags{};
    if (left < right) flags |= 0x0001U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags | old_x);
}

[[nodiscard]] FunctionResult return_from_subroutine(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = read_shared_word(host, registers.address[7]);
    const auto low = read_shared_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_0008226a(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    prefetch(host, 0x0008226eU);
    const auto mode = read_shared_word(host, registers.address[6] + 4U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | mode;
    set_move_word_flags(registers, mode);

    prefetch(host, 0x00082270U);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | 4U;
    set_move_word_flags(registers, 4U);

    prefetch(host, 0x00082272U);
    prefetch(host, 0x00082274U);
    prefetch(host, 0x00082276U);
    registers.address[0] = 0xfff00810U;
    prefetch(host, 0x00082278U);
    prefetch(host, 0x0008227aU);
    registers.address[0] += 2U;
    prefetch(host, 0x0008227cU);
    prefetch(host, 0x0008227eU);
    prefetch(host, 0x00082280U);

    for (;;) {
        const auto input = read_private_byte(host, registers.address[0]);
        const bool active = (input & 0x08U) != 0U;
        set_bit_test_flags(registers, active);
        prefetch(host, 0x00082282U);
        if (active) {
            prefetch(host, 0x0008228cU);
            break;
        }

        prefetch(host, 0x00082284U);
        registers.address[0] += 4U;
        prefetch(host, 0x00082286U);
        prefetch(host, 0x00082288U);
        const auto counter = static_cast<std::uint16_t>(registers.data[1] - 1U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | counter;
        prefetch(host, 0x0008227cU);
        if (counter != 0xffffU) {
            prefetch(host, 0x0008227eU);
            prefetch(host, 0x00082280U);
            continue;
        }

        prefetch(host, 0x0008228aU);
        prefetch(host, 0x0008228cU);
        return return_from_subroutine(host, registers);
    }

    const auto before = static_cast<std::uint16_t>(registers.data[0]);
    const auto incremented = static_cast<std::uint16_t>(before + 1U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | incremented;
    set_add_word_flags(registers, before, 1U, incremented);

    prefetch(host, 0x0008228eU);
    prefetch(host, 0x00082290U);
    const auto compared = static_cast<std::uint16_t>(incremented - 5U);
    set_compare_word_flags(registers, incremented, 5U, compared);
    prefetch(host, 0x00082292U);
    prefetch(host, 0x00082294U);

    const bool less_or_equal =
        (registers.status & 0x0004U) != 0U ||
        (((registers.status & 0x0008U) != 0U) !=
         ((registers.status & 0x0002U) != 0U));
    if (!less_or_equal) {
        registers.data[0] = 0U;
        set_move_long_flags(registers, 0U);
    }

    prefetch(host, 0x00082296U);
    prefetch(host, 0x00082298U);
    prefetch(host, 0x0008229aU);
    const auto result = static_cast<std::uint16_t>(registers.data[0]);
    write_shared_word(host, registers.address[6] + 4U, result);
    set_move_word_flags(registers, result);
    prefetch(host, 0x0008229cU);
    return return_from_subroutine(host, registers);
}

} // namespace gain_ground::translated
