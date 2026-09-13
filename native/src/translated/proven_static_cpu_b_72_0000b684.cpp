#include "gain_ground/contract_types.h"

#include <array>
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kMixerRegion = 10U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign_bit) noexcept
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign_bit) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

std::uint8_t read_private_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kPrivateRegion, address & ~1U, mask) >> (odd ? 0U : 8U));
}

void write_private_long(ExecutionHost &host, std::uint32_t address,
    std::uint32_t value)
{
    host.write_memory_word(kPrivateRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

std::uint32_t read_private_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(host.read_memory_word(
                kPrivateRegion, address, kWordMask)) << 16U)
        | host.read_memory_word(kPrivateRegion, address + 2U, kWordMask);
}

FunctionResult call_child(FunctionContext &context, std::uint32_t function_id,
    std::uint32_t callsite, std::uint32_t target, std::uint32_t return_pc)
{
    auto &registers = context.registers;
    registers.address[7] -= 4U;
    write_private_long(*context.host, registers.address[7], return_pc);
    registers.program_counter = target;
    return context.host->call_function(
        function_id, 1U, 0x72U, 2U, callsite, target, context);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0000b684(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    std::size_t first_call = 0U;
    switch (registers.program_counter) {
    case 0x0000b684U:
        host.write_memory_word(kMixerRegion, 0x1aU, 1U, kWordMask);
        set_logic_flags(registers, 1U, 0x8000U);
        break;
    case 0x0000b692U: first_call = 1U; break;
    case 0x0000b698U: first_call = 2U; break;
    case 0x0000b69eU: first_call = 3U; break;
    default:
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};
    }

    const auto initial_calls = std::array{
        std::array<std::uint32_t, 4>{117U, 0x0000b68cU, 0x000085acU, 0x0000b692U},
        std::array<std::uint32_t, 4>{121U, 0x0000b692U, 0x000085f2U, 0x0000b698U},
        std::array<std::uint32_t, 4>{122U, 0x0000b698U, 0x00008622U, 0x0000b69eU},
        std::array<std::uint32_t, 4>{630U, 0x0000b69eU, 0x0000b6e0U, 0x0000b6a2U},
        std::array<std::uint32_t, 4>{632U, 0x0000b6a2U, 0x0000b762U, 0x0000b6a6U}};
    for (auto index = first_call; index < initial_calls.size(); ++index) {
        const auto &call = initial_calls[index];
        const auto result = call_child(
            context, call[0], call[1], call[2], call[3]);
        if (result.status != TranslationStatus::complete || result.control != 1U)
            return result;
    }

    host.write_memory_word(kPrivateRegion, 0x0c02U, 0x0029U, kWordMask);
    set_logic_flags(registers, 0x0029U, 0x8000U);

    for (const auto call : {
            std::array<std::uint32_t, 4>{322U, 0x0000b6acU, 0x0001826eU, 0x0000b6b2U},
            std::array<std::uint32_t, 4>{633U, 0x0000b6b2U, 0x0000b77aU, 0x0000b6b6U},
            std::array<std::uint32_t, 4>{634U, 0x0000b6b6U, 0x0000b7d8U, 0x0000b6baU}}) {
        const auto result = call_child(
            context, call[0], call[1], call[2], call[3]);
        if (result.status != TranslationStatus::complete || result.control != 1U)
            return result;
    }

    registers.data[0] = 0U;
    set_logic_flags(registers, 0U, 0x80000000U);
    const auto byte = read_private_byte(host, 0x0419U);
    const auto result_byte = static_cast<std::uint8_t>(registers.data[0] | byte);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | result_byte;
    set_logic_flags(registers, result_byte, 0x80U);
    host.write_memory_word(kMixerRegion, 0x18U,
        static_cast<std::uint16_t>(registers.data[0] >> 16U), kWordMask);
    host.write_memory_word(kMixerRegion, 0x1aU,
        static_cast<std::uint16_t>(registers.data[0]), kWordMask);
    set_logic_flags(registers, registers.data[0], 0x80000000U);

    const auto target = read_private_long(host, registers.address[7]);
    registers.address[7] += 4U;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
