#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kScrollRegion = 6U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void prefetch_shared(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kSharedRegion, address & kSharedMask, kWordMask);
}

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_move_long_flags(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack = registers.address[7] & kSharedMask;
    const auto high = host.read_memory_word(kSharedRegion, stack, kWordMask);
    const auto low = host.read_memory_word(
        kSharedRegion, (stack + 2U) & kSharedMask, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_a_update_tile_rowscroll(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    prefetch_shared(host, 0x0008102eU);
    prefetch_shared(host, 0x00081030U);
    prefetch_shared(host, 0x00081032U);
    const auto enable = host.read_memory_word(kPrivateRegion, 0x00000820U, 0xff00U);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x0004U) | ((enable & 0x0800U) == 0U ? 0x0004U : 0U));
    prefetch_shared(host, 0x00081034U);

    if ((enable & 0x0800U) != 0U) {
        prefetch_shared(host, 0x00081036U);
        prefetch_shared(host, 0x00081038U);
        registers.address[0] = 0x00208410U;
        prefetch_shared(host, 0x0008103aU);
        prefetch_shared(host, 0x0008103cU);
        registers.address[1] = 0xfff00e84U;
        prefetch_shared(host, 0x0008103eU);
        prefetch_shared(host, 0x00081040U);
        registers.data[2] = 2U;
        set_move_long_flags(registers, registers.data[2]);
        prefetch_shared(host, 0x00081042U);

        for (;;) {
            prefetch_shared(host, 0x00081044U);
            prefetch_shared(host, 0x00081046U);
            registers.data[1] = 0x37U;
            set_move_long_flags(registers, registers.data[1]);

            const auto value = host.read_memory_word(
                kPrivateRegion, registers.address[1] & kSharedMask, kWordMask);
            registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
            set_move_word_flags(registers, value);

            for (;;) {
                prefetch_shared(host, 0x00081048U);
                host.write_memory_word(
                    kScrollRegion, registers.address[0] - 0x00208000U, value, kWordMask);
                registers.address[0] += 2U;
                set_move_word_flags(registers, value);
                prefetch_shared(host, 0x0008104aU);

                const auto inner = static_cast<std::uint16_t>(registers.data[1] - 1U);
                registers.data[1] = (registers.data[1] & 0xffff0000U) | inner;
                prefetch_shared(host, 0x00081046U);
                if (inner == 0xffffU) break;
            }

            prefetch_shared(host, 0x0008104cU);
            prefetch_shared(host, 0x0008104eU);
            registers.address[0] += 0x90U;
            prefetch_shared(host, 0x00081050U);
            prefetch_shared(host, 0x00081052U);
            registers.address[1] += 0x200U;
            prefetch_shared(host, 0x00081054U);
            prefetch_shared(host, 0x00081056U);

            const auto outer = static_cast<std::uint16_t>(registers.data[2] - 1U);
            registers.data[2] = (registers.data[2] & 0xffff0000U) | outer;
            prefetch_shared(host, 0x00081042U);
            if (outer == 0xffffU) break;
        }
    }

    prefetch_shared(host, 0x00081058U);
    prefetch_shared(host, 0x0008105aU);
    const auto return_address = pop_return(host, registers);
    prefetch_shared(host, return_address);
    prefetch_shared(host, return_address + 2U);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
