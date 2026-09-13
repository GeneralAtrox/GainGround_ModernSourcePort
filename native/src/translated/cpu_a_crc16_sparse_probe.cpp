#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 1U;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;
constexpr std::uint16_t kCarry = 0x0001U;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kRegion, address, 0xffffU);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto even_address = address & ~1U;
    if ((address & 1U) == 0U)
        return static_cast<std::uint8_t>(
            host.read_memory_word(kRegion, even_address, 0xff00U) >> 8U);
    return static_cast<std::uint8_t>(
        host.read_memory_word(kRegion, even_address, 0x00ffU));
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(3U, address & 0x0003ffffU, 0xffffU);
    const auto low = host.read_memory_word(
        3U, (address + 2U) & 0x0003ffffU, 0xffffU);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_lsr_flags(CpuRegisters &registers, std::uint16_t result, bool carry)
{
    std::uint16_t flags = carry ? static_cast<std::uint16_t>(kExtend | kCarry) : 0U;
    if ((result & 0x8000U) != 0U) flags |= kNegative;
    if (result == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_eori_flags(CpuRegisters &registers, std::uint16_t result)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((result & 0x8000U) != 0U) flags |= kNegative;
    if (result == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_a_crc16_sparse_probe(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    context.registers.data[1] =
        (context.registers.data[1] & 0xffff0000U) | 0xfffcU;
    context.registers.data[0] = 0U;

    prefetch(host, 0x00001d32U);
    prefetch(host, 0x00001d34U);
    prefetch(host, 0x00001d36U);
    prefetch(host, 0x00001d38U);

    std::uint32_t source = context.registers.address[0];
    std::uint16_t counter = 0xfffcU;
    for (;;) {
        const auto sparse_word = static_cast<std::uint16_t>(
            (static_cast<std::uint16_t>(read_byte(host, source)) << 8U) |
            read_byte(host, source + 2U));
        context.registers.data[2] =
            (context.registers.data[2] & 0xffff0000U) | sparse_word;
        source += 4U;

        prefetch(host, 0x00001d3aU);
        prefetch(host, 0x00001d3cU);

        auto crc = static_cast<std::uint16_t>(context.registers.data[0]) ^ sparse_word;
        prefetch(host, 0x00001d3eU);
        const bool carry = (crc & 1U) != 0U;
        crc = static_cast<std::uint16_t>(crc >> 1U);
        set_lsr_flags(context.registers, crc, carry);
        prefetch(host, 0x00001d40U);
        if (carry)
            prefetch(host, 0x00001d42U);
        if (carry) {
            crc ^= 0x8810U;
            set_eori_flags(context.registers, crc);
        }
        prefetch(host, 0x00001d44U);
        prefetch(host, 0x00001d46U);
        context.registers.data[0] = crc;

        counter = static_cast<std::uint16_t>(counter - 1U);
        prefetch(host, 0x00001d34U);
        if (counter == 0xffffU) {
            prefetch(host, 0x00001d48U);
            prefetch(host, 0x00001d4aU);
            break;
        }
        prefetch(host, 0x00001d36U);
        prefetch(host, 0x00001d38U);
    }

    context.registers.data[1] =
        (context.registers.data[1] & 0xffff0000U) | counter;
    context.registers.address[0] = source;
    const auto return_address = read_long(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    prefetch(host, return_address);
    prefetch(host, return_address + 2U);
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
