#include "gain_ground/contract_types.h"
#include "gain_ground/sound_caller_timing.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

[[nodiscard]] std::uint16_t read_word(
    SoundCallerTiming &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & kAddressMask, kWordMask);
}

void prefetch(SoundCallerTiming &host, std::uint32_t address)
{
    (void)read_word(host, address);
}

void set_move_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_compare_flags(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (source > destination)
        flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t read_return_address(
    SoundCallerTiming &host, std::uint32_t address)
{
    const auto high = read_word(host, address);
    const auto low = read_word(host, address + 2U);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_a_sound_commit_pending_table_value(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    SoundCallerTiming host(context);
    host.begin(0x83de8U);
    const auto base = context.registers.address[0];
    prefetch(host, 0x00083decU);
    auto value3 = read_word(host, base + 0x20U);
    context.registers.data[3] =
        (context.registers.data[3] & 0xffff0000U) | value3;
    set_compare_flags(context.registers, value3, 0U);
    prefetch(host, 0x00083deeU);
    prefetch(host, 0x00083df0U);
    prefetch(host, 0x00083df2U);

    if (value3 == 0U) {
        host.clocks(2U); // BEQ.w taken.
        prefetch(host, 0x00083df6U);
        prefetch(host, 0x00083df8U);
        prefetch(host, 0x00083dfaU);
        value3 = read_word(host, base + 0x24U);
        context.registers.data[3] =
            (context.registers.data[3] & 0xffff0000U) | value3;
        prefetch(host, 0x00083dfcU);
        prefetch(host, 0x00083dfeU);
        const auto value4 = read_word(host, base + 0x22U);
        context.registers.data[4] =
            (context.registers.data[4] & 0xffff0000U) | value4;
        set_compare_flags(context.registers, value4, value3);
        prefetch(host, 0x00083e00U);
        prefetch(host, 0x00083e02U);
        if (value4 != value3) {
            host.clocks(2U); // BNE.w taken.
            prefetch(host, 0x00083e06U);
            prefetch(host, 0x00083e08U);
            prefetch(host, 0x00083e0aU);
            host.write_memory_word(kRegion,
                (base + 0x24U) & kAddressMask, value4, kWordMask);
            set_move_flags(context.registers, value4);
            prefetch(host, 0x00083e0cU);
        } else {
            host.clocks(4U); // BNE.w not taken.
            prefetch(host, 0x00083e04U);
            prefetch(host, 0x00083e06U);
        }
    } else {
        host.clocks(4U); // BEQ.w not taken.
        prefetch(host, 0x00083df4U);
        prefetch(host, 0x00083df6U);
    }

    const auto return_address = read_return_address(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    prefetch(host, return_address);
    prefetch(host, return_address + 2U);
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
