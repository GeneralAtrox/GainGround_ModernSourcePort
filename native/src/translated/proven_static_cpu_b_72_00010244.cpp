#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kStatusMask = 0x001fU;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U),
        odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void set_word_logic_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kStatusMask) | flags);
}

void set_byte_logic_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kStatusMask) | flags);
}

void set_add_word_flags(CpuRegisters &registers, std::uint16_t destination,
    std::uint16_t source, std::uint16_t result)
{
    const auto wide = static_cast<std::uint32_t>(destination) + source;
    const bool carry = wide > 0xffffU;
    const bool overflow = ((~(destination ^ source)) &
        (destination ^ result) & 0x8000U) != 0U;
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if (overflow) flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kStatusMask) | flags);
}

void set_sub_word_flags(CpuRegisters &registers, std::uint16_t destination,
    std::uint16_t source, std::uint16_t result, bool update_extend)
{
    const bool carry = destination < source;
    const bool overflow = ((destination ^ source) &
        (destination ^ result) & 0x8000U) != 0U;
    std::uint16_t flags = update_extend && carry
        ? 0x0010U : static_cast<std::uint16_t>(registers.status & 0x0010U);
    if (carry) flags |= 0x0001U;
    if (overflow) flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kStatusMask) | flags);
}

[[nodiscard]] bool bgt(const CpuRegisters &registers) noexcept
{
    const bool zero = (registers.status & 0x0004U) != 0U;
    const bool negative = (registers.status & 0x0008U) != 0U;
    const bool overflow = (registers.status & 0x0002U) != 0U;
    return !zero && negative == overflow;
}

[[nodiscard]] bool bmi(const CpuRegisters &registers) noexcept
{
    return (registers.status & 0x0008U) != 0U;
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host,
    CpuRegisters &registers)
{
    const auto stack = registers.address[7];
    const auto high = host.read_memory_word(kRegion, stack, kWordMask);
    const auto low = host.read_memory_word(kRegion, stack + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult proven_static_cpu_b_72_00010244(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    auto &d0 = registers.data[0];
    const auto a5 = registers.address[5];

    auto word = host.read_memory_word(kRegion, a5 + 0x12U, kWordMask);
    d0 = (d0 & 0xffff0000U) | word;
    set_word_logic_flags(registers, word);

    auto result = static_cast<std::uint16_t>(word - 7U);
    d0 = (d0 & 0xffff0000U) | result;
    set_sub_word_flags(registers, word, 7U, result, true);

    auto source = static_cast<std::uint16_t>(registers.data[2]);
    set_sub_word_flags(registers, result, source,
        static_cast<std::uint16_t>(result - source), false);
    if (!bgt(registers)) {
        word = result;
        result = static_cast<std::uint16_t>(word + 0x000eU);
        d0 = (d0 & 0xffff0000U) | result;
        set_add_word_flags(registers, word, 0x000eU, result);

        source = static_cast<std::uint16_t>(registers.data[1]);
        set_sub_word_flags(registers, result, source,
            static_cast<std::uint16_t>(result - source), false);
        if (!bmi(registers)) {
            word = host.read_memory_word(kRegion, a5 + 0x1aU, kWordMask);
            d0 = (d0 & 0xffff0000U) | word;
            set_word_logic_flags(registers, word);

            result = static_cast<std::uint16_t>(word - 6U);
            d0 = (d0 & 0xffff0000U) | result;
            set_sub_word_flags(registers, word, 6U, result, true);

            source = static_cast<std::uint16_t>(registers.data[4]);
            set_sub_word_flags(registers, result, source,
                static_cast<std::uint16_t>(result - source), false);
            if (!bgt(registers)) {
                word = result;
                result = static_cast<std::uint16_t>(word + 0x000cU);
                d0 = (d0 & 0xffff0000U) | result;
                set_add_word_flags(registers, word, 0x000cU, result);

                source = static_cast<std::uint16_t>(registers.data[3]);
                set_sub_word_flags(registers, result, source,
                    static_cast<std::uint16_t>(result - source), false);
                if (!bmi(registers)) {
                    const auto address = registers.address[6] + 0x3fU;
                    const auto original = read_byte(host, address);
                    write_byte(host, address,
                        static_cast<std::uint8_t>(original | 0x80U));
                    set_byte_logic_flags(registers, original);
                }
            }
        }
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
