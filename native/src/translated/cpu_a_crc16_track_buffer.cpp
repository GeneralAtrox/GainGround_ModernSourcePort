#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kMainRamRegion = 3U;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgramRegion, address, 0xffffU);
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_lsr_word(CpuRegisters &registers, std::uint16_t value, bool carry)
{
    std::uint16_t flags = carry ? 0x0011U : 0U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_compare_word(CpuRegisters &registers, std::uint16_t destination,
                      std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    const bool borrow = destination < source;
    const bool overflow = (((destination ^ source) & (destination ^ result)) & 0x8000U) != 0U;
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    if (borrow) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x000fU) | flags);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack = registers.address[7] & 0x0003ffffU;
    const auto high = host.read_memory_word(kMainRamRegion, stack, 0xffffU);
    const auto low = host.read_memory_word(kMainRamRegion, (stack + 2U) & 0x0003ffffU, 0xffffU);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t pc,
                                      std::uint32_t address)
{
    if (address >= 0x00b80000U && address <= 0x00b83fffU)
        return host.read_hardware(1U, 0U, 0xffU, pc, address, 0xffffU);
    return host.read_memory_word(kMainRamRegion, address & 0x0003ffffU, 0xffffU);
}

void write_word(ExecutionHost &host, std::uint32_t pc, std::uint32_t address,
                std::uint16_t value)
{
    if (address >= 0x00b80000U && address <= 0x00b83fffU) {
        host.write_hardware(2U, 0U, 0xffU, pc, address, value, 0xffffU);
        return;
    }
    host.write_memory_word(kMainRamRegion, address & 0x0003ffffU, value, 0xffffU);
}
} // namespace

FunctionResult cpu_a_crc16_track_buffer(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const bool continuation_entry = registers.program_counter == 0x0000128aU;
    if (!continuation_entry) {
        registers.address[0] = registers.address[2];
        registers.data[2] = (registers.data[2] & 0xffff0000U) | 0x167eU;
        set_logic_word(registers, 0x167eU);
        prefetch(host, 0x00001288U);
    }
    registers.data[0] = 0U;
    set_logic_word(registers, 0U);
    prefetch(host, 0x0000128aU);

    std::uint16_t counter = static_cast<std::uint16_t>(registers.data[2]);
    for (;;) {
        prefetch(host, 0x0000128cU);
        prefetch(host, 0x0000128eU);
        const auto word = read_word(host, 0x0000128cU, registers.address[0]);
        registers.address[0] += 2U;
        registers.data[1] = (registers.data[1] & 0xffff0000U) | word;
        set_logic_word(registers, word);

        auto crc = static_cast<std::uint16_t>(registers.data[0]) ^ word;
        registers.data[0] = (registers.data[0] & 0xffff0000U) | crc;
        set_logic_word(registers, crc);
        prefetch(host, 0x00001290U);

        const bool carry = (crc & 1U) != 0U;
        crc = static_cast<std::uint16_t>(crc >> 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | crc;
        set_lsr_word(registers, crc, carry);
        prefetch(host, 0x00001292U);
        prefetch(host, 0x00001294U);
        if (carry) {
            crc ^= 0x8810U;
            registers.data[0] = (registers.data[0] & 0xffff0000U) | crc;
            set_logic_word(registers, crc);
            prefetch(host, 0x00001296U);
        }

        prefetch(host, 0x00001298U);
        prefetch(host, 0x0000129aU);
        counter = static_cast<std::uint16_t>(counter - 1U);
        registers.data[2] = (registers.data[2] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }

    prefetch(host, 0x0000128cU);
    prefetch(host, 0x0000129cU);
    prefetch(host, 0x0000129eU);
    const auto stored = read_word(host, 0x0000129cU, registers.address[0]);
    const auto crc = static_cast<std::uint16_t>(registers.data[0]);
    set_compare_word(registers, crc, stored);
    prefetch(host, 0x000012a0U);
    if (crc != stored) {
        prefetch(host, 0x000012a2U);
        write_word(host, 0x000012a0U, registers.address[0], crc);
        prefetch(host, 0x000012a4U);
        prefetch(host, 0x000012a6U);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | 0x0001U);
    }
    prefetch(host, 0x000012a6U);
    prefetch(host, 0x000012a8U);

    const auto return_address = pop_return(host, registers);
    prefetch(host, return_address);
    prefetch(host, return_address + 2U);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
