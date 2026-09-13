#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
FunctionResult cpu_a_crc16_track_buffer(FunctionContext &context) noexcept;

namespace {
constexpr std::uint16_t kProgramRegion = 1U;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, 0xffffU);
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_000012a8(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x000012a8U)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.address[0] = 0x00b80000U;
    prefetch(host, 0x000012acU);

    registers.address[0] = static_cast<std::uint32_t>(
        registers.address[0] + static_cast<std::int16_t>(registers.data[7]));
    prefetch(host, 0x000012aeU);

    registers.data[2] = (registers.data[2] & 0xffff0000U) | 0x0ffeU;
    set_logic_word(registers, 0x0ffeU);
    prefetch(host, 0x000012b0U);
    prefetch(host, 0x000012b2U);
    prefetch(host, 0x000012b4U);
    prefetch(host, 0x000012b6U);

    registers.program_counter = 0x0000128aU;
    return cpu_a_crc16_track_buffer(context);
}

} // namespace gain_ground::translated
