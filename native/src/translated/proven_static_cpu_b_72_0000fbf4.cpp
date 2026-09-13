#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host,
    std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address,
    std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void set_logic(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags = 0U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU)
        flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_compare_byte(CpuRegisters &registers, std::uint8_t destination,
    std::uint8_t source, std::uint8_t result)
{
    const auto extend = static_cast<std::uint16_t>(registers.status & 0x0010U);
    std::uint16_t flags = extend;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (destination < source) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_sub_byte(CpuRegisters &registers, std::uint8_t destination,
    std::uint8_t source, std::uint8_t result)
{
    std::uint16_t flags = 0U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (destination < source) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0000fbf4(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.data[0] = 0x12U;
    set_logic(registers, registers.data[0], 0x80000000U, 0xffffffffU);

    auto d1 = read_byte(host, registers.address[5] + 0x4aU);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | d1;
    set_logic(registers, d1, 0x80U, 0xffU);

    const auto old_d1_word = static_cast<std::uint16_t>(registers.data[1]);
    const auto d1_word = static_cast<std::uint16_t>(old_d1_word + 1U);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | d1_word;
    set_add_word(registers, old_d1_word, 1U, d1_word);
    d1 = static_cast<std::uint8_t>(d1_word);

    const auto modulus = read_byte(host, registers.address[4]);
    const auto compare_result = static_cast<std::uint8_t>(d1 - modulus);
    set_compare_byte(registers, d1, modulus, compare_result);
    if ((registers.status & 0x0001U) == 0U) {
        const auto subtractor = read_byte(host, registers.address[4]);
        const auto result = static_cast<std::uint8_t>(d1 - subtractor);
        set_sub_byte(registers, d1, subtractor, result);
        d1 = result;
        registers.data[1] = (registers.data[1] & 0xffffff00U) | d1;
    }

    write_byte(host, registers.address[5] + 0x4aU, d1);
    set_logic(registers, d1, 0x80U, 0xffU);

    constexpr std::uint32_t kCallsite = 0x0000fc0aU;
    constexpr std::uint32_t kTarget = 0x0000fc1eU;
    registers.program_counter = kTarget;
    (void)host.call_function(528U, 1U, 0x72U, 1U,
        kCallsite, kTarget, context);
    return FunctionResult::complete(3U, kTarget);
}

} // namespace gain_ground::translated
