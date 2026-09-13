#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_logic_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_asl_word_two_flags(CpuRegisters &registers, std::uint16_t original,
                            std::uint16_t result)
{
    const bool carry = (original & 0x4000U) != 0U;
    const bool overflow = ((original ^ (original << 1U)) & 0x8000U) != 0U
        || (((original << 1U) ^ (original << 2U)) & 0x8000U) != 0U;
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers, std::uint16_t destination,
                        std::uint16_t source, std::uint16_t result)
{
    const bool carry = static_cast<std::uint32_t>(destination) + source > 0xffffU;
    const bool overflow =
        ((~(destination ^ source) & (destination ^ result)) & 0x8000U) != 0U;
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_compare_word_flags(CpuRegisters &registers,
                            std::uint16_t destination,
                            std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (destination < source) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void write_byte(ExecutionHost &host, std::uint32_t address,
                std::uint8_t value)
{
    const auto mask = static_cast<std::uint16_t>(
        (address & 1U) == 0U ? 0xff00U : 0x00ffU);
    const auto data = static_cast<std::uint16_t>(
        (address & 1U) == 0U ? static_cast<std::uint16_t>(value) << 8U
                             : value);
    host.write_memory_word(kRegion, address & ~1U, data, mask);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
                 std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(
        kRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

FunctionResult return_from_function(ExecutionHost &host,
                                    CpuRegisters &registers)
{
    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_dispatch_callback_record_22382(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    push_return(host, registers, 0x00022386U);
    registers.program_counter = 0x000237e2U;
    const auto snapshot = host.call_function(412U, 1U, 0x72U, 2U,
        0x00022382U, 0x000237e2U, context);
    if (snapshot.status != TranslationStatus::complete
        || snapshot.control != 1U)
        return snapshot;

    const auto selector = host.read_memory_word(
        kRegion, registers.address[5] + 0x1cU, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | selector;
    set_logic_word_flags(registers, selector);
    const auto table_offset = static_cast<std::uint16_t>(selector << 2U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | table_offset;
    set_asl_word_two_flags(registers, selector, table_offset);

    if (table_offset == 0U) {
        registers.address[2] = 0x00023bf2U;
        push_return(host, registers, 0x000223a2U);
        registers.program_counter = 0x00023936U;
        const auto allocate = host.call_function(420U, 1U, 0x72U, 2U,
            0x0002239eU, 0x00023936U, context);
        if (allocate.status != TranslationStatus::complete
            || allocate.control != 1U)
            return allocate;

        registers.data[0] = 2U;
        set_logic_word_flags(registers, 2U);
        registers.address[6] = registers.address[5] + 0x12U;

        for (;;) {
            const auto value = host.read_memory_word(
                kRegion, registers.address[6], kWordMask);
            set_logic_word_flags(registers, value);
            if ((value & 0x8000U) == 0U) {
                const auto compared_value = host.read_memory_word(
                    kRegion, registers.address[6], kWordMask);
                set_compare_word_flags(registers, compared_value, 0x0150U);
                const bool signed_less =
                    ((registers.status & 0x0008U) != 0U)
                    != ((registers.status & 0x0002U) != 0U);
                if (signed_less) {
                    const auto before = host.read_memory_word(kRegion,
                        registers.address[5] + 0x1cU, kWordMask);
                    const auto after = static_cast<std::uint16_t>(before + 1U);
                    host.write_memory_word(kRegion,
                        registers.address[5] + 0x1cU, after, kWordMask);
                    set_add_word_flags(registers, before, 1U, after);
                    write_byte(host, registers.address[5] + 0x2aU, 2U);
                    set_logic_byte_flags(registers, 2U);
                    return return_from_function(host, registers);
                }
            }

            registers.address[6] += 4U;
            const auto counter = static_cast<std::uint16_t>(registers.data[0]);
            registers.data[0] = (registers.data[0] & 0xffff0000U)
                | static_cast<std::uint16_t>(counter - 1U);
            if (counter == 0U)
                return return_from_function(host, registers);
        }
    }

    if (table_offset == 4U) {
        registers.address[2] = 0x00023bf2U;
        push_return(host, registers, 0x000223d2U);
        registers.program_counter = 0x0002392aU;
        const auto test = host.call_function(419U, 1U, 0x72U, 2U,
            0x000223ceU, 0x0002392aU, context);
        if (test.status != TranslationStatus::complete || test.control != 1U)
            return test;
        return return_from_function(host, registers);
    }

    return {TranslationStatus::contract_violation, 0U, 0x0002238cU};
}

} // namespace gain_ground::translated
