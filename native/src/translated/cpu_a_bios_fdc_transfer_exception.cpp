#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgram = 1U;
constexpr std::uint16_t kShared = 3U;
constexpr std::uint16_t kWord = 0xffffU;
constexpr std::uint16_t kByte = 0x00ffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgram, address, kWord);
}

void prefetch_return_target(ExecutionHost &host, std::uint32_t address)
{
    if (address >= 0x00080000U && address <= 0x000bffffU) {
        (void)host.read_memory_word(kShared, address & kSharedMask, kWord);
        return;
    }
    prefetch(host, address);
}

std::uint16_t read_stack_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kShared, address & kSharedMask, kWord);
}

void write_stack_word(ExecutionHost &host, std::uint32_t address,
                      std::uint16_t value, std::uint16_t mask = kWord)
{
    host.write_memory_word(
        kShared, (address & kSharedMask) & ~1U, value, mask);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
                 std::uint32_t return_address)
{
    registers.address[7] -= 4U;
    write_stack_word(host, registers.address[7],
        static_cast<std::uint16_t>(return_address >> 16U));
    write_stack_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(return_address));
}

void set_logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

FunctionResult return_from_exception(ExecutionHost &host,
                                     CpuRegisters &registers)
{
    const auto restored_status = read_stack_word(host, registers.address[7]);
    registers.address[7] += 2U;
    const auto high = read_stack_word(host, registers.address[7]);
    const auto low = read_stack_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    prefetch_return_target(host, target);
    prefetch_return_target(host, target + 2U);
    registers.status = restored_status;
    registers.program_counter = target;
    return FunctionResult::complete(2U, target);
}
} // namespace

FunctionResult cpu_a_bios_fdc_transfer_exception(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr
        || context.registers.program_counter != 0x0000220cU) {
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};
    }

    auto &host = *context.host;
    auto &registers = context.registers;

    push_return(host, registers, 0x00002210U);
    prefetch(host, 0x00001018U);
    prefetch(host, 0x0000101aU);
    registers.program_counter = 0x00001018U;
    const auto child = host.call_function(
        12U, 0U, 0xffU, 2U, 0x0000220cU, 0x00001018U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U
        || registers.program_counter != 0x00002210U) {
        return child;
    }

    if ((registers.status & 0x0001U) == 0U) {
        prefetch(host, 0x00002214U);
        prefetch(host, 0x00002216U);
        (void)read_stack_word(host, registers.address[7]);
        prefetch(host, 0x00002218U);
        write_stack_word(host, registers.address[7] + 1U, 0U, kByte);
        set_logic_byte(registers, 0U);
        return return_from_exception(host, registers);
    }

    prefetch(host, 0x00002218U);
    prefetch(host, 0x0000221aU);
    prefetch(host, 0x0000221cU);
    prefetch(host, 0x0000221eU);
    (void)read_stack_word(host, registers.address[7]);
    prefetch(host, 0x00002220U);
    write_stack_word(host, registers.address[7] + 1U, 0x0101U, kByte);
    set_logic_byte(registers, 1U);
    return return_from_exception(host, registers);
}

} // namespace gain_ground::translated
