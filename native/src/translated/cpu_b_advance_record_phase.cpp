#include "cpu_b_record_deadline_detail.h"
#include "gground_functions.h"

namespace gain_ground::translated {
namespace {
using namespace record_deadline_detail;

void set_word(CpuRegisters &registers, unsigned index, std::uint16_t value)
{
    registers.data[index] = (registers.data[index] & 0xffff0000U) | value;
    logic(registers, value, 0x8000U, 0xffffU);
}

void add_record_word(FunctionContext &context, std::uint32_t displacement,
    std::uint16_t value)
{
    auto &registers = context.registers;
    auto &host = *context.host;
    const auto address = registers.address[5] + displacement;
    const auto previous = read_word(host, address);
    const auto result = static_cast<std::uint16_t>(previous + value);
    write_word(host, address, result);
    add(registers, previous, value, result, 0x8000U, 0xffffU);
}

void add_record_long(FunctionContext &context, std::uint32_t displacement,
    std::uint32_t value)
{
    auto &registers = context.registers;
    auto &host = *context.host;
    const auto address = registers.address[5] + displacement;
    const auto previous = read_long(host, address);
    const auto result = previous + value;
    write_long_read_modify_write(host, address, result);
    add(registers, previous, value, result, 0x80000000U, 0xffffffffU);
}

std::uint32_t arithmetic_shift_right_long(CpuRegisters &registers,
    std::uint32_t value, unsigned count)
{
    bool carry = false;
    for (unsigned index = 0; index < count; ++index) {
        carry = (value & 1U) != 0U;
        value = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(value) >> 1);
    }
    std::uint16_t flags = carry ? kExtend | kCarry : 0U;
    if ((value & 0x80000000U) != 0U) flags |= kNegative;
    if (value == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
    return value;
}

FunctionResult invoke(FunctionContext &context, std::uint32_t id,
    std::uint32_t site, std::uint32_t target, std::uint32_t continuation)
{
    return call_child(context, id, site, target, continuation);
}
} // namespace

FunctionResult cpu_b_advance_record_phase(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x00012346U)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &registers = context.registers;
    auto &host = *context.host;
    const auto record = registers.address[5];
    auto result = invoke(context, 254U,
        0x00012346U, 0x000128eaU, 0x0001234aU);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;

    bool bounds_hit = (registers.status & kCarry) != 0U;
    if (!bounds_hit) {
        result = invoke(context, 257U,
            0x0001234cU, 0x00012b32U, 0x00012350U);
        if (result.status != TranslationStatus::complete)
            return result;
        if (result.control == 8U)
            return FunctionResult::complete(1U, result.exit_program_counter);
        if (result.control != 1U)
            return result;
        bounds_hit = (registers.status & kCarry) != 0U;
    }
    if (bounds_hit)
        add_record_word(context, 0x58U, 0x03d0U);
    add_record_word(context, 0x58U, 0x0030U);

    auto word = read_word(host, record + 0x58U);
    set_word(registers, 1U, word);
    word &= 0x07ffU;
    set_word(registers, 1U, word);
    result = invoke(context, 306U,
        0x00012366U, 0x00016372U, 0x0001236cU);
    if (result.status != TranslationStatus::complete)
        return result;
    if (result.control == 3U
            && result.exit_program_counter == 0x00016376U)
        result = proven_static_cpu_b_72_00016376(context);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;

    add_record_word(context, 0x46U, 4U);
    word = read_word(host, record + 0x46U);
    set_word(registers, 2U, word);

    auto product = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(registers.data[0]))
        * static_cast<std::int32_t>(static_cast<std::int16_t>(registers.data[2])));
    registers.data[0] = product;
    logic(registers, product, 0x80000000U, 0xffffffffU);
    registers.data[0] = arithmetic_shift_right_long(registers, product, 3U);
    add_record_long(context, 0x12U, registers.data[0]);

    auto phase = static_cast<std::uint16_t>(registers.data[1]);
    phase = arithmetic_shift_right_word(registers, phase);
    phase = arithmetic_shift_right_word(registers, phase);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | phase;
    set_word(registers, 0U, phase);

    auto next = static_cast<std::uint16_t>(phase + phase);
    add(registers, phase, phase, next, 0x8000U, 0xffffU);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | next;
    const auto doubled = next;
    next = static_cast<std::uint16_t>(doubled + phase);
    add(registers, doubled, phase, next, 0x8000U, 0xffffU);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | next;

    product = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(next))
        * static_cast<std::int32_t>(static_cast<std::int16_t>(registers.data[2])));
    registers.data[1] = product;
    logic(registers, product, 0x80000000U, 0xffffffffU);
    registers.data[1] = arithmetic_shift_right_long(registers, product, 3U);
    add_record_long(context, 0x1aU, registers.data[1]);

    auto magnitude = static_cast<std::uint16_t>(registers.data[2]);
    const auto extend = static_cast<std::uint16_t>(registers.status & kExtend);
    const auto compared = static_cast<std::uint16_t>(magnitude - 0x001fU);
    subtract_word(registers, magnitude, 0x001fU, compared);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kExtend) | extend);
    if (magnitude < 0x001fU) {
        next = static_cast<std::uint16_t>(magnitude + magnitude);
        add(registers, magnitude, magnitude, next, 0x8000U, 0xffffU);
        magnitude = next;
        registers.data[2] = (registers.data[2] & 0xffff0000U) | magnitude;
    } else {
        next = static_cast<std::uint16_t>(magnitude - 0x01e0U);
        subtract_word(registers, magnitude, 0x01e0U, next);
        magnitude = next;
        registers.data[2] = (registers.data[2] & 0xffff0000U) | magnitude;
        if ((registers.status & kNegative) != 0U) {
            magnitude = 0x003fU;
            set_word(registers, 2U, magnitude);
        } else {
            next = static_cast<std::uint16_t>(magnitude - 0x003fU);
            subtract_word(registers, magnitude, 0x003fU, next);
            magnitude = next;
            registers.data[2] = (registers.data[2] & 0xffff0000U) | magnitude;
            if ((registers.status & kNegative) == 0U) {
                (void)read_byte(host, record);
                write_byte(host, record, 0U);
                logic(registers, 0U, 0x80U, 0xffU);
                return return_from(context);
            }
            next = static_cast<std::uint16_t>(0U - magnitude);
            subtract_word(registers, 0U, magnitude, next);
            magnitude = next;
            registers.data[2] = (registers.data[2] & 0xffff0000U) | magnitude;
        }
    }

    const auto intensity = static_cast<std::uint8_t>(magnitude);
    write_byte(host, record + 0x10U, intensity);
    logic(registers, intensity, 0x80U, 0xffU);
    write_byte(host, record + 0x11U, intensity);
    logic(registers, intensity, 0x80U, 0xffU);

    auto toggle = read_byte(host, record + 0x3cU);
    write_byte(host, record + 0x3cU, static_cast<std::uint8_t>(toggle ^ 1U));
    if ((toggle & 1U) == 0U)
        registers.status |= kZero;
    else
        registers.status &= static_cast<std::uint16_t>(~kZero);
    if ((registers.status & kZero) != 0U) {
        const auto animation = read_byte(host, record + 0x3dU);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | animation;
        logic(registers, animation, 0x80U, 0xffU);
        const auto previous = static_cast<std::uint16_t>(registers.data[0]);
        next = static_cast<std::uint16_t>(previous + 2U);
        add(registers, previous, 2U, next, 0x8000U, 0xffffU);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | next;
        next &= 6U;
        set_word(registers, 0U, next);
        write_byte(host, record + 0x3dU, static_cast<std::uint8_t>(next));
        logic(registers, static_cast<std::uint8_t>(next), 0x80U, 0xffU);
        registers.address[0] = 0x00012ee6U;
        const auto sprite = read_word(host,
            registers.address[0] + static_cast<std::int16_t>(next));
        write_word(host, record + 6U, sprite);
        logic(registers, sprite, 0x8000U, 0xffffU);
    }

    result = invoke(context, 280U,
        0x000123d6U, 0x00015d24U, 0x000123dcU);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    result = invoke(context, 281U,
        0x000123dcU, 0x00015d3cU, 0x000123e2U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    result = invoke(context, 282U,
        0x000123e2U, 0x00015df2U, 0x000123e8U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    return return_from(context);
}
} // namespace gain_ground::translated
