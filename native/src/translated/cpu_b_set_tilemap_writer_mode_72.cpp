#include "gain_ground/contract_types.h"
#include "cpu_b_or_record_word_62_common.h"
#include "cpu_b_set_tilemap_writer_mode_common.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] ByteLocation locate_byte(std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
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

void set_logic(CpuRegisters &registers, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_sub_byte(CpuRegisters &registers, std::uint8_t left, std::uint8_t result)
{
    std::uint16_t flags{};
    if (left == 0U) flags |= 0x0011U;
    if (left == 0x80U) flags |= 0x0002U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_sub_word(CpuRegisters &registers,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (right > left) flags |= 0x0011U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_asr_two_word(CpuRegisters &registers, std::uint16_t original, std::uint16_t result)
{
    const bool carry = (original & 0x0002U) != 0U;
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_asl_four_word(CpuRegisters &registers, std::uint16_t original, std::uint16_t result)
{
    auto shifted = original;
    bool carry{};
    bool overflow{};
    for (unsigned count = 0; count != 4U; ++count) {
        const bool old_sign = (shifted & 0x8000U) != 0U;
        carry = old_sign;
        shifted = static_cast<std::uint16_t>(shifted << 1U);
        overflow = overflow || (old_sign != ((shifted & 0x8000U) != 0U));
    }
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if (overflow) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_b_set_tilemap_writer_mode_common(
    FunctionContext &context, bool inline_tail) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto mode_address = registers.address[5] + 0x4aU;
    const auto raw_shift = host.read_memory_word(
        kRegion, registers.address[4] + 0x84U, kWordMask);
    registers.data[6] = (registers.data[6] & 0xffff0000U) | raw_shift;
    set_logic(registers, raw_shift, 0x8000U);
    const auto shift = static_cast<std::uint16_t>(
        static_cast<std::int16_t>(raw_shift) >> 2U);
    registers.data[6] = (registers.data[6] & 0xffff0000U) | shift;
    set_asr_two_word(registers, raw_shift, shift);

    const auto d0 = static_cast<std::uint16_t>(registers.data[0]);
    const auto adjusted = static_cast<std::uint16_t>(d0 - shift);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | adjusted;
    set_sub_word(registers, d0, shift, adjusted);
    const auto masked = static_cast<std::uint16_t>(adjusted & 0x007fU);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | masked;
    set_logic(registers, masked, 0x8000U);

    registers.address[0] = static_cast<std::uint32_t>(
        0x00202080U + static_cast<std::int16_t>(masked));
    registers.data[1] = 0U;
    set_logic(registers, 0U, 0x80000000U);
    const auto index = read_byte(host, mode_address);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | index;
    set_logic(registers, index, 0x80U);
    const auto table_byte = read_byte(host,
        registers.address[4] + 2U + static_cast<std::int16_t>(index));
    registers.data[1] = (registers.data[1] & 0xffffff00U) | table_byte;
    set_logic(registers, table_byte, 0x80U);

    const auto expanded = static_cast<std::uint16_t>(table_byte) << 4U;
    registers.data[1] = (registers.data[1] & 0xffff0000U) | expanded;
    set_asl_four_word(registers, table_byte, expanded);
    registers.address[1] = 0x00026f1cU;
    const auto table_word = host.read_memory_word(kRegion,
        registers.address[1] + static_cast<std::int16_t>(expanded), kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | table_word;
    set_logic(registers, table_word, 0x8000U);

    registers.program_counter = 0x0000fc48U;
    if (inline_tail)
        return cpu_b_or_record_word_62_common(context);
    return host.call_function(192U, 1U, 0x72U, 0U,
        0x0000fc44U, 0x0000fc48U, context);
}

FunctionResult cpu_b_set_tilemap_writer_mode_72(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.data[0] = 0x72U;
    set_logic(registers, registers.data[0], 0x80000000U);

    const auto mode_address = registers.address[5] + 0x4aU;
    auto mode = read_byte(host, mode_address);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | mode;
    set_logic(registers, mode, 0x80U);
    if (mode == 0U) {
        mode = read_byte(host, registers.address[4]);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | mode;
        set_logic(registers, mode, 0x80U);
    }

    const auto decremented = static_cast<std::uint8_t>(mode - 1U);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | decremented;
    set_sub_byte(registers, mode, decremented);
    write_byte(host, mode_address, decremented);
    set_logic(registers, decremented, 0x80U);
    return cpu_b_set_tilemap_writer_mode_common(context);
}

} // namespace gain_ground::translated
