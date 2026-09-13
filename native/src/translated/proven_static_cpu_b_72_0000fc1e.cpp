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
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(kRegion, address & ~1U, mask);
    return static_cast<std::uint8_t>(word >> (odd ? 0U : 8U));
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_logic_long(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint16_t asr_word(CpuRegisters &registers,
    std::uint16_t value, unsigned count)
{
    bool carry = false;
    for (unsigned i = 0; i != count; ++i) {
        carry = (value & 1U) != 0U;
        value = static_cast<std::uint16_t>(
            (value >> 1U) | (value & 0x8000U));
    }
    std::uint16_t flags = 0U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    if (carry) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    return value;
}

void set_sub_word(CpuRegisters &registers, std::uint16_t destination,
    std::uint16_t source, std::uint16_t result)
{
    std::uint16_t flags = 0U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (destination < source) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint16_t asl_word(CpuRegisters &registers,
    std::uint16_t value, unsigned count)
{
    bool carry = false;
    bool overflow = false;
    for (unsigned i = 0; i != count; ++i) {
        const bool old_sign = (value & 0x8000U) != 0U;
        carry = old_sign;
        value = static_cast<std::uint16_t>(value << 1U);
        overflow = overflow ||
            (old_sign != ((value & 0x8000U) != 0U));
    }
    std::uint16_t flags = 0U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    if (carry) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    return value;
}
} // namespace

FunctionResult proven_static_cpu_b_72_0000fc1e(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    auto d6 = host.read_memory_word(
        kRegion, registers.address[4] + 0x84U, kWordMask);
    registers.data[6] = (registers.data[6] & 0xffff0000U) | d6;
    set_logic_word(registers, d6);

    d6 = asr_word(registers, d6, 2U);
    registers.data[6] = (registers.data[6] & 0xffff0000U) | d6;

    const auto old_d0 = static_cast<std::uint16_t>(registers.data[0]);
    auto d0 = static_cast<std::uint16_t>(old_d0 - d6);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    set_sub_word(registers, old_d0, d6, d0);

    d0 = static_cast<std::uint16_t>(d0 & 0x007fU);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    set_logic_word(registers, d0);

    registers.address[0] = 0x00202080U;
    registers.address[0] += static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(d0)));

    registers.data[1] = 0U;
    set_logic_long(registers, 0U);

    auto d1 = read_byte(host, registers.address[5] + 0x4aU);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | d1;
    set_logic_byte(registers, d1);

    d1 = read_byte(host, registers.address[4] + 2U
        + static_cast<std::uint16_t>(registers.data[1]));
    registers.data[1] = (registers.data[1] & 0xffffff00U) | d1;
    set_logic_byte(registers, d1);

    auto d1_word = asl_word(registers, d1, 4U);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | d1_word;

    registers.address[1] = 0x00026f1cU;
    d0 = host.read_memory_word(kRegion,
        registers.address[1]
            + static_cast<std::uint32_t>(static_cast<std::int32_t>(
                static_cast<std::int16_t>(d1_word))),
        kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    set_logic_word(registers, d0);

    registers.program_counter = 0x0000fc48U;
    return host.call_function(192U, 1U, 0x72U, 0U,
        0x0000fc44U, 0x0000fc48U, context);
}

} // namespace gain_ground::translated
