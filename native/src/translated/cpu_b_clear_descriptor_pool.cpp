#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_move_long_zero_flags(CpuRegisters &registers)
{
    constexpr std::uint16_t kConditionCodeMask = 0x000fU;
    constexpr std::uint16_t kZero = 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | kZero);
}

} // namespace

FunctionResult cpu_b_clear_descriptor_pool(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    std::uint32_t destination = 0x00003400U;
    const std::uint32_t d0_high = context.registers.data[0] & 0xffff0000U;
    const std::uint32_t d2_high = context.registers.data[2] & 0xffff0000U;
    context.registers.data[1] = 0U;

    for (std::uint32_t record = 0U; record < 128U; ++record) {
        destination += 0x0cU;
        for (std::uint32_t field = 0U; field < 29U; ++field) {
            host.write_memory_word(kRegion, destination, 0U, kWordMask);
            host.write_memory_word(kRegion, destination + 2U, 0U, kWordMask);
            destination += 4U;
            set_move_long_zero_flags(context.registers);
        }
    }

    context.registers.data[0] = d0_high | 0xffffU;
    context.registers.data[2] = d2_high | 0xffffU;
    context.registers.address[6] = destination;

    const std::uint32_t return_address = read_long(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
