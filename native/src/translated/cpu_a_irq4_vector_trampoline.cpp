#include "gain_ground/contract_types.h"
#include "gain_ground/sound_caller_timing.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kByteMask = 0x00ffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kSharedRegion, address & kSharedMask, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kSharedRegion, address & kSharedMask, value, kWordMask);
}

void set_logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word(CpuRegisters &registers,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_a_irq4_vector_trampoline(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    SoundCallerTiming entry(context);
    if (host.resumes_interrupts_inline()) entry.begin(0x00080048U);
    const auto vector_low = entry.read_memory_word(kSharedRegion, 0x0000004cU, kWordMask);

    if (vector_low == 0x0fc6U) {
        (void)entry.read_memory_word(kSharedRegion, 0x00000fc6U, kWordMask);
        (void)entry.read_memory_word(kSharedRegion, 0x00000fc8U, kWordMask);
        entry.stop(); // JMP complete; the handler owns its instruction timing.
        constexpr std::uint32_t target = 0x00080fc6U;
        registers.program_counter = target;
        const auto child = host.call_function(
            73U, 0U, 0xffU, 1U, 0x00080048U, target, context);
        if (child.status != TranslationStatus::complete)
            return child;
        return FunctionResult::complete(3U, target);
    }
    if (vector_low != 0x219cU)
        return {TranslationStatus::contract_violation, 0U,
                registers.program_counter};

    (void)entry.read_memory_word(kProgramRegion, 0x0000219cU, kWordMask);
    (void)entry.read_memory_word(kProgramRegion, 0x0000219eU, kWordMask);
    entry.stop(); // The legacy BIOS handler body remains uninstrumented.
    prefetch(host, 0x000021a0U);
    registers.address[7] -= 2U;
    write_word(host, registers.address[7], static_cast<std::uint16_t>(registers.data[0]));
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x0080U;
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);

    for (std::uint32_t iteration = 0; iteration != 129U; ++iteration) {
        prefetch(host, 0x000021a2U);
        prefetch(host, 0x000021a4U);
        registers.data[0] = (registers.data[0] & 0xffff0000U)
            | static_cast<std::uint16_t>(registers.data[0] - 1U);
    }
    prefetch(host, 0x000021a2U);

    prefetch(host, 0x000021a6U);
    prefetch(host, 0x000021a8U);
    prefetch(host, 0x000021aaU);
    prefetch(host, 0x000021acU);
    const auto io = static_cast<std::uint8_t>(host.read_hardware(
        1U, 0U, 0xffU, 0x000021a6U, 0x00a00004U, kByteMask));
    set_logic_byte(registers, io);

    prefetch(host, 0x000021aeU);
    prefetch(host, 0x000021b0U);
    const auto counter = read_word(host, 0xfffffc84U);
    prefetch(host, 0x000021b2U);
    const auto incremented = static_cast<std::uint16_t>(counter + 1U);
    write_word(host, 0xfffffc84U, incremented);
    set_add_word(registers, counter, 1U, incremented);

    const auto saved_d0 = read_word(host, registers.address[7]);
    registers.address[7] += 2U;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | saved_d0;
    prefetch(host, 0x000021b4U);

    const auto restored_status = read_word(host, registers.address[7]);
    registers.address[7] += 2U;
    const auto target = (static_cast<std::uint32_t>(
        read_word(host, registers.address[7])) << 16U)
        | read_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.status = restored_status;
    registers.program_counter = target;
    return FunctionResult::complete(2U, target);
}

} // namespace gain_ground::translated
