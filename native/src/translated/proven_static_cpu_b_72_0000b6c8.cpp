#include "gain_ground/contract_types.h"

#include <array>
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kPrivateRegion, address, kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kPrivateRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

FunctionResult call_child(FunctionContext &context, std::uint32_t function_id,
    std::uint32_t callsite, std::uint32_t target, std::uint32_t return_pc)
{
    auto &registers = context.registers;
    registers.address[7] -= 4U;
    write_long(*context.host, registers.address[7], return_pc);
    registers.program_counter = target;
    return context.host->call_function(
        function_id, 1U, 0x72U, 2U, callsite, target, context);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0000b6c8(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &registers = context.registers;
    if (registers.program_counter != 0x0000b6c8U
        && registers.program_counter != 0x0000b6ceU)
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};

    const auto calls = std::array{
            std::array<std::uint32_t, 4>{122U, 0x0000b6c8U, 0x00008622U, 0x0000b6ceU},
            std::array<std::uint32_t, 4>{630U, 0x0000b6ceU, 0x0000b6e0U, 0x0000b6d2U},
            std::array<std::uint32_t, 4>{632U, 0x0000b6d2U, 0x0000b762U, 0x0000b6d6U},
            std::array<std::uint32_t, 4>{635U, 0x0000b6d6U, 0x0000b7e2U, 0x0000b6daU},
            std::array<std::uint32_t, 4>{633U, 0x0000b6daU, 0x0000b77aU, 0x0000b6deU}};
    const auto first_call = registers.program_counter == 0x0000b6ceU ? 1U : 0U;
    for (auto index = first_call; index < calls.size(); ++index) {
        const auto &call = calls[index];
        const auto result = call_child(
            context, call[0], call[1], call[2], call[3]);
        if (result.status != TranslationStatus::complete || result.control != 1U)
            return result;
    }

    const auto target = read_long(*context.host, registers.address[7]);
    registers.address[7] += 4U;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
