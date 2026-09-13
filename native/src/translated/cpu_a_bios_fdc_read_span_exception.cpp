#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kMainRamRegion = 3U;
constexpr std::uint32_t kMainRamOffsetMask = 0x0003ffffU;

void prefetch_program(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, 0xffffU);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
                 std::uint32_t return_address)
{
    registers.address[7] -= 4U;
    const auto stack = registers.address[7] & kMainRamOffsetMask;
    host.write_memory_word(kMainRamRegion, stack,
                           static_cast<std::uint16_t>(return_address >> 16U),
                           0xffffU);
    host.write_memory_word(kMainRamRegion,
                           (stack + 2U) & kMainRamOffsetMask,
                           static_cast<std::uint16_t>(return_address), 0xffffU);
}
} // namespace

FunctionResult cpu_a_bios_fdc_read_span_exception(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr
        || context.registers.program_counter != 0x000021c0U) {
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};
    }

    auto &host = *context.host;
    auto &registers = context.registers;

    push_return(host, registers, 0x000021c4U);
    prefetch_program(host, 0x00000e1eU);
    prefetch_program(host, 0x00000e20U);
    registers.program_counter = 0x00000e1eU;
    const auto child = host.call_function(
        10U, 0U, 0xffU, 2U, 0x000021c0U, 0x00000e1eU, context);
    if (child.status != TranslationStatus::complete || child.control != 1U
        || registers.program_counter != 0x000021c4U) {
        return child;
    }

    auto stack = registers.address[7] & kMainRamOffsetMask;
    const auto restored_status =
        host.read_memory_word(kMainRamRegion, stack, 0xffffU);
    stack = (stack + 2U) & kMainRamOffsetMask;
    const auto pc_high =
        host.read_memory_word(kMainRamRegion, stack, 0xffffU);
    const auto pc_low = host.read_memory_word(
        kMainRamRegion, (stack + 2U) & kMainRamOffsetMask, 0xffffU);
    registers.address[7] += 6U;

    const auto return_address =
        (static_cast<std::uint32_t>(pc_high) << 16U) | pc_low;
    prefetch_program(host, return_address);
    prefetch_program(host, return_address + 2U);
    registers.status = restored_status;
    registers.program_counter = return_address;
    return FunctionResult::complete(2U, return_address);
}

} // namespace gain_ground::translated
