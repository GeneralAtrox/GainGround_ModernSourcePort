#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivate = 2U;
constexpr std::uint16_t kShared = 3U;
constexpr std::uint16_t kWord = 0xffffU;
constexpr std::uint16_t kCondition = 0x001fU;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;
constexpr std::uint16_t kOverflow = 0x0002U;
constexpr std::uint16_t kCarry = 0x0001U;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kPrivate, address & 0x00ffffffU, kWord);
}

std::uint8_t read_byte(ExecutionHost &host, std::uint16_t region,
                       std::uint32_t address)
{
    const auto offset = region == kShared ? address & 0x0003ffffU : address & 0x00ffffffU;
    const bool odd = (offset & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(region, offset & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

void set_word(CpuRegisters &r, unsigned index, std::uint16_t value)
{
    r.data[index] = (r.data[index] & 0xffff0000U) | value;
    r.status = static_cast<std::uint16_t>((r.status & ~kCondition)
        | (r.status & kExtend) | ((value & 0x8000U) != 0U ? kNegative : 0U)
        | (value == 0U ? kZero : 0U));
}

void compare_word(CpuRegisters &r, std::uint16_t destination, std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    r.status = static_cast<std::uint16_t>((r.status & ~kCondition) | (r.status & kExtend)
        | ((result & 0x8000U) != 0U ? kNegative : 0U)
        | (result == 0U ? kZero : 0U)
        | (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U ? kOverflow : 0U)
        | (destination < source ? kCarry : 0U));
}

void compare_byte(CpuRegisters &r, std::uint8_t destination, std::uint8_t source)
{
    const auto result = static_cast<std::uint8_t>(destination - source);
    r.status = static_cast<std::uint16_t>((r.status & ~kCondition) | (r.status & kExtend)
        | ((result & 0x80U) != 0U ? kNegative : 0U)
        | (result == 0U ? kZero : 0U)
        | (((destination ^ source) & (destination ^ result) & 0x80U) != 0U ? kOverflow : 0U)
        | (destination < source ? kCarry : 0U));
}

FunctionResult finish(ExecutionHost &host, CpuRegisters &r, std::uint8_t control)
{
    const auto high = read_word(host, r.address[7]);
    const auto low = read_word(host, r.address[7] + 2U);
    r.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    r.program_counter = target;
    return FunctionResult::complete(control, target);
}

FunctionResult call_probe(FunctionContext &context, std::uint32_t callsite,
                          std::uint32_t continuation)
{
    auto &host = *context.host;
    auto &r = context.registers;
    r.address[7] -= 4U;
    host.write_memory_word(kPrivate, r.address[7],
        static_cast<std::uint16_t>(continuation >> 16U), kWord);
    host.write_memory_word(kPrivate, r.address[7] + 2U,
        static_cast<std::uint16_t>(continuation), kWord);
    r.program_counter = 0x00015edeU;
    return host.call_function(287U, 1U, 0x72U, 2U, callsite,
        0x00015edeU, context);
}

bool probe_collision(ExecutionHost &host, CpuRegisters &r)
{
    const auto region = static_cast<std::uint16_t>(
        r.address[0] <= 0x0003ffffU ? kPrivate : kShared);
    const auto value = read_byte(host, region, r.address[0]);
    r.data[3] = (r.data[3] & 0xffffff00U) | value;
    const auto extended = static_cast<std::uint16_t>(
        static_cast<std::int16_t>(static_cast<std::int8_t>(value)));
    set_word(r, 3U, extended);
    compare_word(r, static_cast<std::uint16_t>(r.data[2]), extended);
    return (r.status & kNegative) != 0U;
}
} // namespace

FunctionResult cpu_b_check_record_position_window_alt(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x00012bb0U)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto record = r.address[5];
    const auto reject = [&]() -> FunctionResult {
        r.address[7] += 4U;
        (void)read_byte(host, kPrivate, record);
        const bool odd = (record & 1U) != 0U;
        host.write_memory_word(kPrivate, record & 0x00fffffeU, 0U,
            static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U));
        r.status = static_cast<std::uint16_t>((r.status & ~kCondition)
            | (r.status & kExtend) | kZero);
        return finish(host, r, 8U);
    };

    auto value = read_word(host, record + 0x12U);
    set_word(r, 0U, value);
    compare_word(r, value, 0xffe0U);
    if ((r.status & kNegative) != 0U) return reject();
    compare_word(r, value, 0x01a0U);
    if ((r.status & kZero) == 0U
        && ((r.status & kNegative) != 0U) == ((r.status & kOverflow) != 0U))
        return reject();

    value = read_word(host, record + 0x1aU);
    set_word(r, 0U, value);
    compare_word(r, value, 0xffe0U);
    if ((r.status & kNegative) != 0U) return reject();
    compare_word(r, value, 0x0210U);
    if ((r.status & kNegative) == 0U) return reject();

    set_word(r, 0U, read_word(host, record + 0x2aU));
    set_word(r, 1U, read_word(host, record + 0x32U));
    set_word(r, 2U, read_word(host, record + 0x2eU));
    const auto addend = static_cast<std::uint16_t>(r.data[2]);
    const auto sum = static_cast<std::uint32_t>(addend) + addend;
    const auto doubled = static_cast<std::uint16_t>(sum);
    r.data[2] = (r.data[2] & 0xffff0000U) | doubled;
    r.status = static_cast<std::uint16_t>((r.status & ~kCondition)
        | ((doubled & 0x8000U) != 0U ? kNegative : 0U)
        | (doubled == 0U ? kZero : 0U)
        | (((~(addend ^ addend) & (addend ^ doubled)) & 0x8000U) != 0U
            ? kOverflow : 0U)
        | (sum > kWord ? kExtend | kCarry : 0U));
    auto result = call_probe(context, 0x00012be4U, 0x00012beaU);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    if (probe_collision(host, r)) goto collision;

    compare_byte(r, read_byte(host, kPrivate, record + 0x0bU), 2U);
    if ((r.status & kZero) != 0U) goto clear;

    set_word(r, 1U, read_word(host, record + 0x34U));
    result = call_probe(context, 0x00012bfeU, 0x00012c04U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    if (probe_collision(host, r)) goto collision;

    set_word(r, 0U, read_word(host, record + 0x2cU));
    result = call_probe(context, 0x00012c10U, 0x00012c16U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    if (probe_collision(host, r)) goto collision;

    set_word(r, 1U, read_word(host, record + 0x32U));
    result = call_probe(context, 0x00012c22U, 0x00012c28U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    if (probe_collision(host, r)) goto collision;

clear:
    r.status &= static_cast<std::uint16_t>(~kCondition);
    return finish(host, r, 1U);

collision:
    r.status = static_cast<std::uint16_t>((r.status & ~kCondition) | kCarry);
    return finish(host, r, 1U);
}
} // namespace gain_ground::translated
