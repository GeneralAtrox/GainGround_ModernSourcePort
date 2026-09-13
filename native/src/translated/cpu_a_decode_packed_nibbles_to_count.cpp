#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kMainRamRegion = 3U;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;
constexpr std::uint16_t kOverflow = 0x0002U;
constexpr std::uint16_t kCarry = 0x0001U;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, 0xffffU);
}

[[nodiscard]] std::uint8_t read_program_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto word = host.read_memory_word(kProgramRegion, address & ~1U,
        (address & 1U) == 0U ? 0xff00U : 0x00ffU);
    return static_cast<std::uint8_t>((address & 1U) == 0U ? word >> 8U : word);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack = registers.address[7] & 0x0003ffffU;
    const auto high = host.read_memory_word(kMainRamRegion, stack, 0xffffU);
    const auto low = host.read_memory_word(kMainRamRegion, (stack + 2U) & 0x0003ffffU, 0xffffU);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & 0x8000U) != 0U) flags |= kNegative;
    if (value == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_logic_long(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & 0x80000000U) != 0U) flags |= kNegative;
    if (value == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_rol_word(CpuRegisters &registers, std::uint16_t value, bool carry)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & 0x8000U) != 0U) flags |= kNegative;
    if (value == 0U) flags |= kZero;
    if (carry) flags |= kCarry;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x000fU) | flags);
}

void set_add_byte(CpuRegisters &registers, std::uint8_t left,
    std::uint8_t right, std::uint8_t result)
{
    std::uint16_t flags{};
    if ((result & 0x80U) != 0U) flags |= kNegative;
    if (result == 0U) flags |= kZero;
    if (((~(left ^ right) & (left ^ result)) & 0x80U) != 0U) flags |= kOverflow;
    if (static_cast<unsigned>(left) + static_cast<unsigned>(right) > 0xffU)
        flags |= static_cast<std::uint16_t>(kExtend | kCarry);
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_a_decode_packed_nibbles_to_count(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.address[1] = registers.address[0];
    registers.data[0] = 0U;
    set_logic_long(registers, 0U);

    for (;;) {
        prefetch(host, 0x00001a68U);
        prefetch(host, 0x00001a6aU);
        const auto source = host.read_memory_word(
            kProgramRegion, registers.address[1], 0xffffU);
        registers.address[1] += 2U;
        registers.data[1] = (registers.data[1] & 0xffff0000U) | source;
        set_logic_word(registers, source);

        prefetch(host, 0x00001a6cU);
        if (source == 0U) {
            prefetch(host, 0x00001a78U);
            prefetch(host, 0x00001a7aU);
            break;
        }

        prefetch(host, 0x00001a6eU);
        auto index = static_cast<std::uint16_t>(source & 0x7000U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | index;
        set_logic_word(registers, index);

        prefetch(host, 0x00001a70U);
        bool carry{};
        for (unsigned shift = 0; shift != 4U; ++shift) {
            carry = (index & 0x8000U) != 0U;
            index = static_cast<std::uint16_t>((index << 1U) | (carry ? 1U : 0U));
        }
        registers.data[1] = (registers.data[1] & 0xffff0000U) | index;
        set_rol_word(registers, index, carry);

        prefetch(host, 0x00001a72U);
        prefetch(host, 0x00001a74U);
        prefetch(host, 0x00001a76U);
        const auto left = static_cast<std::uint8_t>(registers.data[0]);
        const auto right = read_program_byte(host, 0x00001a7aU + index);
        const auto sum = static_cast<std::uint8_t>(left + right);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | sum;
        set_add_byte(registers, left, right, sum);
        prefetch(host, 0x00001a78U);
    }

    const auto return_address = pop_return(host, registers);
    prefetch(host, return_address);
    prefetch(host, return_address + 2U);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
