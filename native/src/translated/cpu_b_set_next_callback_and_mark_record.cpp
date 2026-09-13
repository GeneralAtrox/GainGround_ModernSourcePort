#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host,
    std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto word = host.read_memory_word(
        kRegion, address & ~1U, odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

void write_byte(ExecutionHost &host, std::uint32_t address,
    std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    host.write_memory_word(kRegion, address & ~1U,
        static_cast<std::uint16_t>(odd ? value : value << 8U),
        odd ? 0x00ffU : 0xff00U);
}

void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_b_set_next_callback_and_mark_record(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];

    host.write_memory_word(kRegion, base + 2U, 0x0001U, kWordMask);
    host.write_memory_word(kRegion, base + 4U, 0x2d14U, kWordMask);
    set_logic_flags(registers, 0x00012d14U, 0x80000000U, 0xffffffffU);

    const auto original = read_byte(host, base + 0x3fU);
    write_byte(host, base + 0x3fU,
        static_cast<std::uint8_t>(original | 0x80U));
    set_logic_flags(registers, original, 0x80U, 0xffU);

    registers.program_counter = 0x00012d14U;
    return host.call_function(262U, 1U, 0x72U, 0U,
        0x00012d10U, 0x00012d14U, context);
}

} // namespace gain_ground::translated
