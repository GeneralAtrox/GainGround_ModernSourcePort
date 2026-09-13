#include "gain_ground/contract_types.h"
#include "gain_ground/sound_caller_timing.h"

#include <array>
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void prefetch(SoundCallerTiming &host, std::uint32_t address)
{
    (void)host.read_memory_word(kSharedRegion, address & kSharedMask, kWordMask);
}

std::uint16_t read_word(SoundCallerTiming &host, std::uint32_t address)
{
    return host.read_memory_word(kSharedRegion, address & kSharedMask, kWordMask);
}

std::uint32_t read_long(SoundCallerTiming &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

void write_long_predecrement(SoundCallerTiming &host, CpuRegisters &registers,
                             std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kSharedRegion,
        (registers.address[7] + 2U) & kSharedMask,
        static_cast<std::uint16_t>(value), kWordMask);
    host.write_memory_word(kSharedRegion,
        registers.address[7] & kSharedMask,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
}

void push_return(SoundCallerTiming &host, CpuRegisters &registers,
                 std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kSharedRegion, registers.address[7] & kSharedMask,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kSharedRegion,
        (registers.address[7] + 2U) & kSharedMask,
        static_cast<std::uint16_t>(value), kWordMask);
}

void save_all(SoundCallerTiming &host, CpuRegisters &registers)
{
    const std::array<std::uint32_t, 15> values{
        registers.address[6], registers.address[5], registers.address[4],
        registers.address[3], registers.address[2], registers.address[1],
        registers.address[0], registers.data[7], registers.data[6],
        registers.data[5], registers.data[4], registers.data[3],
        registers.data[2], registers.data[1], registers.data[0]};
    for (const auto value : values)
        write_long_predecrement(host, registers, value);
}

void restore_all(SoundCallerTiming &host, CpuRegisters &registers)
{
    for (unsigned index = 0; index != 8U; ++index) {
        registers.data[index] = read_long(host, registers.address[7]);
        registers.address[7] += 4U;
    }
    for (unsigned index = 0; index != 7U; ++index) {
        registers.address[index] = read_long(host, registers.address[7]);
        registers.address[7] += 4U;
    }
}

FunctionResult rte(SoundCallerTiming &host, CpuRegisters &registers,
                   std::uint32_t next_word, bool next_prefetched = false,
                   bool frame_word_prefetched = false)
{
    if (!frame_word_prefetched)
        (void)read_word(host, registers.address[7]);
    if (!next_prefetched)
        prefetch(host, next_word);
    const auto status = read_word(host, registers.address[7]);
    registers.address[7] += 2U;
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.status = status;
    registers.program_counter = target;
    return FunctionResult::complete(2U, target);
}
} // namespace

FunctionResult cpu_a_irq3_sound_service(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};
    SoundCallerTiming host(context);
    auto &registers = context.registers;
    host.begin(0x00080f96U);

    prefetch(host, 0x00080f9aU);
    prefetch(host, 0x00080f9cU);
    prefetch(host, 0x00080f9eU);
    context.host->write_hardware(2U, 0U, 0xffU, 0x00080f96U,
        0x00a00004U, 0x0018U, kWordMask);
    host.clocks(4U);
    prefetch(host, 0x00080fa0U);
    prefetch(host, 0x00080fa2U);
    const auto gate = static_cast<std::uint8_t>(host.read_memory_word(
        kSharedRegion, 0x0003800aU, 0xff00U) >> 8U);
    host.clocks(2U); // TAS internal interval; ordinary bus accesses charge four.
    host.write_memory_word(kSharedRegion, 0x0003800aU,
        static_cast<std::uint16_t>(gate | 0x80U) << 8U, 0xff00U);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x000fU) | (gate == 0U ? 0x0004U : 0U));
    prefetch(host, 0x00080fa4U);

    if (gate != 0U) {
        host.clocks(2U); // BNE.w taken.
        prefetch(host, 0x00080f86U);
        prefetch(host, 0x00080f88U);
        prefetch(host, 0x00080f8aU);
        write_long_predecrement(host, registers, registers.data[0]);
        registers.data[0] = 0x0000007fU;
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);
        prefetch(host, 0x00080f8cU);
        for (;;) {
            prefetch(host, 0x00080f8eU);
            const auto counter = static_cast<std::uint16_t>(registers.data[0] - 1U);
            registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
            host.clocks(2U); // DBF internal clocks before the target probe.
            prefetch(host, 0x00080f8cU);
            if (counter == 0xffffU)
                break;
        }
        prefetch(host, 0x00080f90U);
        prefetch(host, 0x00080f92U);
        prefetch(host, 0x00080f94U);
        registers.data[0] = read_long(host, registers.address[7]);
        registers.address[7] += 4U;
        return rte(host, registers, 0x00080f96U);
    }

    host.clocks(4U); // BNE.w not taken.
    prefetch(host, 0x00080fa6U);
    prefetch(host, 0x00080fa8U);
    prefetch(host, 0x00080faaU);
    save_all(host, registers);
    prefetch(host, 0x00080facU);
    prefetch(host, 0x00080faeU);
    prefetch(host, 0x00080fb0U);
    registers.address[5] = 0x00fb0000U;
    prefetch(host, 0x00080fb2U);
    prefetch(host, 0x00080fb4U);
    prefetch(host, 0x00080fb6U);
    registers.address[6] = 0xffffc000U;
    prefetch(host, 0x00080fb8U);
    prefetch(host, 0x00080fbaU);
    prefetch(host, 0x00083208U);
    push_return(host, registers, 0x00080fbcU);
    prefetch(host, 0x0008320aU);
    registers.program_counter = 0x00083208U;
    const auto child = host.call_function(76U, 0U, 0xffU, 2U,
        0x00080fb6U, 0x00083208U, context);
    if (child.status != TranslationStatus::complete
            || registers.program_counter != 0x00080fbcU)
        return child;

    host.begin(0x00080fbcU); // The child owns its elapsed time; resume after RTS.
    prefetch(host, 0x00080fc0U);
    restore_all(host, registers);
    (void)read_word(host, registers.address[7]);
    prefetch(host, 0x00080fc2U);
    prefetch(host, 0x00080fc4U);
    (void)host.read_memory_word(kSharedRegion, 0x0003800aU, 0xff00U);
    prefetch(host, 0x00080fc6U);
    host.write_memory_word(kSharedRegion, 0x0003800aU, 0U, 0xff00U);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x000fU) | 0x0004U);
    return rte(host, registers, 0x00080fc6U, true, true);
}

} // namespace gain_ground::translated
