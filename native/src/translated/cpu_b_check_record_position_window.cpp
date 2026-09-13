#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kConditionMask = 0x001fU;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;
constexpr std::uint16_t kOverflow = 0x0002U;
constexpr std::uint16_t kCarry = 0x0001U;

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kPrivateRegion, address & 0x00ffffffU, kWordMask);
}

[[nodiscard]] std::uint8_t read_byte(
    ExecutionHost &host, std::uint16_t region, std::uint32_t address)
{
    const auto offset = region == kSharedRegion ? address & 0x0003ffffU
                                                : address & 0x00ffffffU;
    const bool odd = (offset & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(region, offset & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

void clear_record_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    host.write_memory_word(kPrivateRegion, address & 0x00fffffeU, 0U, mask);
}

void set_data_word(CpuRegisters &registers, unsigned index, std::uint16_t value)
{
    registers.data[index] = (registers.data[index] & 0xffff0000U) | value;
    std::uint16_t flags = registers.status & kExtend;
    if ((value & 0x8000U) != 0U) flags |= kNegative;
    if (value == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

void compare_word(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    std::uint16_t flags = registers.status & kExtend;
    if ((result & 0x8000U) != 0U) flags |= kNegative;
    if (result == 0U) flags |= kZero;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= kOverflow;
    if (destination < source) flags |= kCarry;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

void compare_byte(CpuRegisters &registers,
    std::uint8_t destination, std::uint8_t source)
{
    const auto result = static_cast<std::uint8_t>(destination - source);
    std::uint16_t flags = registers.status & kExtend;
    if ((result & 0x80U) != 0U) flags |= kNegative;
    if (result == 0U) flags |= kZero;
    if (((destination ^ source) & (destination ^ result) & 0x80U) != 0U)
        flags |= kOverflow;
    if (destination < source) flags |= kCarry;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

[[nodiscard]] bool signed_greater(const CpuRegisters &registers)
{
    return (registers.status & kZero) == 0U
        && ((registers.status & kNegative) != 0U)
            == ((registers.status & kOverflow) != 0U);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = read_word(host, registers.address[7]);
    const auto low = read_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

[[nodiscard]] FunctionResult call_probe(FunctionContext &context,
    std::uint32_t callsite, std::uint32_t continuation)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion, registers.address[7],
        static_cast<std::uint16_t>(continuation >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(continuation), kWordMask);
    registers.program_counter = 0x00015edeU;
    return host.call_function(287U, 1U, 0x72U, 2U,
        callsite, 0x00015edeU, context);
}

[[nodiscard]] bool probe_bit_one(ExecutionHost &host, CpuRegisters &registers)
{
    const auto region = static_cast<std::uint16_t>(
        registers.address[0] <= 0x0003ffffU ? kPrivateRegion : kSharedRegion);
    const auto value = read_byte(host, region, registers.address[0]);
    if ((value & 0x02U) == 0U)
        registers.status |= kZero;
    else
        registers.status &= static_cast<std::uint16_t>(~kZero);
    return (value & 0x02U) != 0U;
}

[[nodiscard]] FunctionResult finish(ExecutionHost &host,
    CpuRegisters &registers, std::uint8_t control)
{
    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(control, target);
}
} // namespace

FunctionResult cpu_b_check_record_position_window(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x00012b32U)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto record = registers.address[5];
    const auto reject_ancestor = [&]() -> FunctionResult {
        registers.address[7] += 4U;
        (void)read_byte(host, kPrivateRegion, record);
        clear_record_byte(host, record);
        registers.status = static_cast<std::uint16_t>(
            (registers.status & ~kConditionMask) | (registers.status & kExtend) | kZero);
        return finish(host, registers, 8U);
    };

    auto value = read_word(host, record + 0x12U);
    set_data_word(registers, 0U, value);
    compare_word(registers, value, 0xffe0U);
    if ((registers.status & kNegative) != 0U) return reject_ancestor();
    compare_word(registers, value, 0x01a0U);
    if (signed_greater(registers)) return reject_ancestor();

    value = read_word(host, record + 0x1aU);
    set_data_word(registers, 0U, value);
    compare_word(registers, value, 0xffe0U);
    if ((registers.status & kNegative) != 0U) return reject_ancestor();
    compare_word(registers, value, 0x0210U);
    if ((registers.status & kNegative) == 0U) return reject_ancestor();

    set_data_word(registers, 0U, read_word(host, record + 0x2aU));
    set_data_word(registers, 1U, read_word(host, record + 0x32U));
    auto result = call_probe(context, 0x00012b60U, 0x00012b66U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    if (probe_bit_one(host, registers)) goto collision;

    compare_byte(registers, read_byte(host, kPrivateRegion, record + 0x0bU), 2U);
    if ((registers.status & kZero) != 0U) goto clear;

    set_data_word(registers, 1U, read_word(host, record + 0x34U));
    result = call_probe(context, 0x00012b78U, 0x00012b7eU);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    if (probe_bit_one(host, registers)) goto collision;

    set_data_word(registers, 0U, read_word(host, record + 0x2cU));
    result = call_probe(context, 0x00012b88U, 0x00012b8eU);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    if (probe_bit_one(host, registers)) goto collision;

    set_data_word(registers, 1U, read_word(host, record + 0x32U));
    result = call_probe(context, 0x00012b98U, 0x00012b9eU);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    if (probe_bit_one(host, registers)) goto collision;

clear:
    registers.status &= static_cast<std::uint16_t>(~kConditionMask);
    return finish(host, registers, 1U);

collision:
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | kCarry);
    return finish(host, registers, 1U);
}

} // namespace gain_ground::translated
