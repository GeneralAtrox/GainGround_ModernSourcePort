#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kCpuBPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags{};
    if ((value & 0x80U) != 0U)
        flags |= 0x0008U;
    if (value == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x000fU) | flags);
}

[[nodiscard]] std::uint8_t read_byte(
    ExecutionHost &host, std::uint32_t byte_offset)
{
    const bool odd=(byte_offset&1U)!=0;
    const auto mask=static_cast<std::uint16_t>(odd ? 0x00ffU:0xff00U);
    return static_cast<std::uint8_t>(
        host.read_memory_word(kCpuBPrivateRegion, byte_offset&~1U, mask) >> (odd ? 0U:8U));
}
} // namespace

FunctionResult cpu_b_prepare_tilemap_strip_flags(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    const auto pointer = host.read_memory_word(
        kCpuBPrivateRegion, registers.address[5] + 0x68U, kWordMask);
    registers.address[6] = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(pointer)));

    auto value = read_byte(host, registers.address[6]);
    registers.data[7] = (registers.data[7] & 0xffffff00U) | value;
    set_logic_byte_flags(registers, value);

    value = static_cast<std::uint8_t>(value | read_byte(host, 0x0404U));
    registers.data[7] = (registers.data[7] & 0xffffff00U) | value;
    set_logic_byte_flags(registers, value);

    registers.program_counter = 0x0000fa30U;
    return host.call_function(
        182U, 1U, 0x72U, 0U, 0x0000fa2cU, 0x0000fa30U, context);
}

} // namespace gain_ground::translated
