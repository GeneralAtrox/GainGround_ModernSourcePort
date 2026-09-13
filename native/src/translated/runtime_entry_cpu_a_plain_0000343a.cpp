#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kMainRamRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kMainRamMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

void set_moveq_flags(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    const auto offset = registers.address[7] & kMainRamMask;
    host.write_memory_word(kMainRamRegion, offset,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kMainRamRegion, offset + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}
} // namespace

FunctionResult runtime_entry_cpu_a_plain_0000343a(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    registers.data[2] = 2U;
    set_moveq_flags(registers, 2U);
    prefetch(host, 0x0000343eU);
    push_return(host, registers, 0x00003440U);
    prefetch(host, 0x000034bcU);
    prefetch(host, 0x000034beU);
    registers.program_counter = 0x000034bcU;
    (void)host.call_function(513U, 0U, 0xffU, 2U,
        0x0000343cU, 0x000034bcU, context);

    registers.data[2] = 0U;
    set_moveq_flags(registers, 0U);
    prefetch(host, 0x00003444U);
    prefetch(host, 0x000034caU);
    prefetch(host, 0x000034ccU);
    registers.program_counter = 0x000034caU;
    return host.call_function(514U, 0U, 0xffU, 1U,
        0x00003442U, 0x000034caU, context);
}

} // namespace gain_ground::translated
