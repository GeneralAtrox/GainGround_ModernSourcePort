#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kMainRamRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kMainRamMask = 0x0003ffffU;

[[nodiscard]] std::uint16_t read_main(
    ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(
        kMainRamRegion, address & kMainRamMask, kWordMask);
}

void write_main(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value)
{
    host.write_memory_word(
        kMainRamRegion, address & kMainRamMask, value, kWordMask);
}

void prefetch_program(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, kWordMask);
}

void prefetch_main(ExecutionHost &host, std::uint32_t address)
{
    (void)read_main(host, address);
}
} // namespace

FunctionResult runtime_entry_cpu_a_plain_00002200(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.address[7] -= 4U;
    write_main(host, registers.address[7], 0U);
    write_main(host, registers.address[7] + 2U, 0x2204U);
    prefetch_program(host, 0x00003392U);
    prefetch_program(host, 0x00003394U);
    registers.program_counter = 0x00003392U;
    const auto child = host.call_function(500U, 0U, 0xffU, 2U,
        0x00002200U, 0x00003392U, context);
    if (child.status != TranslationStatus::complete)
        return child;

    const auto stack = registers.address[7];
    const auto restored_status = read_main(host, stack);
    const auto target =
        (static_cast<std::uint32_t>(read_main(host, stack + 2U)) << 16U)
        | read_main(host, stack + 4U);
    registers.address[7] += 6U;
    prefetch_main(host, target);
    prefetch_main(host, target + 2U);
    registers.status = restored_status;
    registers.program_counter = target;
    return FunctionResult::complete(2U, target);
}

} // namespace gain_ground::translated
