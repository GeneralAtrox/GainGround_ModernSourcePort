#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kCpuBMainMemoryRegion = 2U;
constexpr std::uint16_t kFullWordMask = 0xffffU;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;
constexpr std::uint16_t kOverflow = 0x0002U;
constexpr std::uint16_t kCarry = 0x0001U;
constexpr std::uint16_t kConditionCodeMask = 0x001fU;

void set_test_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & 0x8000U) != 0U)
        flags |= kNegative;
    if (value == 0U)
        flags |= kZero;
    registers.status = static_cast<std::uint16_t>((registers.status & ~kConditionCodeMask) | flags);
}

void set_compare_word_flags(CpuRegisters &registers, std::uint16_t destination, std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    std::uint16_t flags = registers.status & kExtend;
    if ((result & 0x8000U) != 0U)
        flags |= kNegative;
    if (result == 0U)
        flags |= kZero;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= kOverflow;
    if (destination < source)
        flags |= kCarry;
    registers.status = static_cast<std::uint16_t>((registers.status & ~kConditionCodeMask) | flags);
}

void set_add_word_flags(
    CpuRegisters &registers, std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    const std::uint32_t wide = static_cast<std::uint32_t>(left) + right;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U)
        flags |= kNegative;
    if (result == 0U)
        flags |= kZero;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= kOverflow;
    if (wide > 0xffffU)
        flags |= kExtend | kCarry;
    registers.status = static_cast<std::uint16_t>((registers.status & ~kConditionCodeMask) | flags);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kCpuBMainMemoryRegion, address, kFullWordMask);
    const auto low = host.read_memory_word(kCpuBMainMemoryRegion, address + 2U, kFullWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

} // namespace

FunctionResult cpu_b_compute_attribute_workspace_offset(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    const std::uint16_t saved_d0 = static_cast<std::uint16_t>(context.registers.data[0]);
    const std::uint16_t saved_d1 = static_cast<std::uint16_t>(context.registers.data[1]);
    context.registers.address[7] -= 2U;
    host.write_memory_word(
        kCpuBMainMemoryRegion, context.registers.address[7], saved_d1, kFullWordMask);
    context.registers.address[7] -= 2U;
    host.write_memory_word(
        kCpuBMainMemoryRegion, context.registers.address[7], saved_d0, kFullWordMask);

    context.registers.address[0] = 0x00000200U;
    const auto x = saved_d0;
    const auto y = saved_d1;
    bool valid = true;

    set_test_word_flags(context.registers, x);
    if (static_cast<std::int16_t>(x) < 0) {
        valid = false;
    } else {
        set_compare_word_flags(context.registers, x, 0x017fU);
        if (static_cast<std::int16_t>(x) > 0x017f) {
            valid = false;
        } else {
            set_test_word_flags(context.registers, y);
            if (static_cast<std::int16_t>(y) < 0) {
                valid = false;
            } else {
                set_compare_word_flags(context.registers, y, 0x01efU);
                if (static_cast<std::int16_t>(y) > 0x01ef)
                    valid = false;
            }
        }
    }

    if (valid) {
        const auto y_offset = static_cast<std::uint16_t>((0x01efU - y) >> 3U);
        const auto x_offset = static_cast<std::uint16_t>((x & 0xfff8U) << 3U);
        const auto offset = static_cast<std::uint16_t>(x_offset + y_offset);
        set_add_word_flags(context.registers, x_offset, y_offset, offset);
        context.registers.address[0] = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(static_cast<std::int16_t>(0xaf22U)) +
            static_cast<std::int16_t>(offset));
    }

    const auto restored_d0 = host.read_memory_word(
        kCpuBMainMemoryRegion, context.registers.address[7], kFullWordMask);
    context.registers.address[7] += 2U;
    const auto restored_d1 = host.read_memory_word(
        kCpuBMainMemoryRegion, context.registers.address[7], kFullWordMask);
    context.registers.address[7] += 2U;
    context.registers.data[0] = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(restored_d0)));
    context.registers.data[1] = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(restored_d1)));

    // The captured 68000 MOVEM.W postincrement path performs one bus read at
    // the restored stack pointer before RTS consumes the return address.
    (void)host.read_memory_word(
        kCpuBMainMemoryRegion, context.registers.address[7], kFullWordMask);
    const std::uint32_t return_address = read_long(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
