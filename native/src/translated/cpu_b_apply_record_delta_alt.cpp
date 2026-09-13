#include "cpu_b_record_deadline_detail.h"

namespace gain_ground::translated {
namespace {
using namespace record_deadline_detail;

void add_record_long(FunctionContext &context, std::uint32_t displacement)
{
    auto &r = context.registers;
    auto &host = *context.host;
    const auto destination = read_long(host, r.address[5] + displacement);
    const auto result = destination + r.data[0];
    write_long_read_modify_write(host, r.address[5] + displacement, result);
    add(r, destination, r.data[0], result, 0x80000000U, 0xffffffffU);
}

void load_record_long(FunctionContext &context, std::uint32_t displacement)
{
    auto &r = context.registers;
    r.data[0] = read_long(*context.host, r.address[5] + displacement);
    logic(r, r.data[0], 0x80000000U, 0xffffffffU);
}

FunctionResult sound_continuation(FunctionContext &context)
{
    auto &r = context.registers;
    r.data[0] = (r.data[0] & 0xffff0000U) | 0x12U;
    logic(r, 0x12U, 0x8000U, 0xffffU);
    auto result = call_child(context, 309U, 0x00012736U,
        0x0001700cU, 0x0001273cU);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    result = call_child(context, 264U, 0x0001273cU,
        0x00012d4eU, 0x00012740U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    return return_from(context);
}
} // namespace

FunctionResult cpu_b_apply_record_delta_alt(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x000126b0U)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &r = context.registers;
    auto &host = *context.host;
    const auto record = r.address[5];
    auto result = call_child(context, 253U, 0x000126b0U,
        0x0001283cU, 0x000126b4U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    if ((r.status & kCarry) != 0U)
        return sound_continuation(context);

    result = call_child(context, 258U, 0x000126b6U,
        0x00012bb0U, 0x000126baU);
    if (result.status != TranslationStatus::complete)
        return result;
    if (result.control == 8U)
        return FunctionResult::complete(1U, result.exit_program_counter);
    if (result.control != 1U)
        return result;
    if ((r.status & kCarry) != 0U)
        return sound_continuation(context);

    load_record_long(context, 0x1eU);
    add_record_long(context, 0x12U);
    load_record_long(context, 0x22U);
    const auto velocity = read_long(host, record + 0x22U);
    const auto decelerated = velocity - 0x4000U;
    write_long_read_modify_write(host, record + 0x22U, decelerated);
    subtract_long(r, velocity, 0x4000U, decelerated);
    add_record_long(context, 0x16U);
    load_record_long(context, 0x26U);
    add_record_long(context, 0x1aU);

    auto word = read_word(host, record + 0x16U);
    r.data[0] = (r.data[0] & 0xffff0000U) | word;
    logic(r, word, 0x8000U, 0xffffU);
    const auto shifted = arithmetic_shift_right_word(r, word);
    r.data[0] = (r.data[0] & 0xffff0000U) | shifted;
    word = static_cast<std::uint16_t>(shifted + 0x3fU);
    r.data[0] = (r.data[0] & 0xffff0000U) | word;
    add(r, shifted, 0x3fU, word, 0x8000U, 0xffffU);
    auto byte = static_cast<std::uint8_t>(word);
    write_byte(host, record + 0x10U, byte);
    logic(r, byte, 0x80U, 0xffU);
    write_byte(host, record + 0x11U, byte);
    logic(r, byte, 0x80U, 0xffU);

    const auto countdown = read_byte(host, record + 0x3cU);
    const auto remaining = static_cast<std::uint8_t>(countdown - 1U);
    write_byte(host, record + 0x3cU, remaining);
    subtract_byte(r, countdown, 1U, remaining);
    if (!((r.status & kZero) == 0U
        && ((r.status & kNegative) != 0U)
            == ((r.status & kOverflow) != 0U))) {
        write_byte(host, record + 0x3cU, 3U);
        logic(r, 3U, 0x80U, 0xffU);
        word = read_word(host, record + 0x58U);
        r.data[0] = (r.data[0] & 0xffff0000U) | word;
        logic(r, word, 0x8000U, 0xffffU);
        word = arithmetic_shift_left_word(r, word, 5U);
        r.data[0] = (r.data[0] & 0xffff0000U) | word;

        const auto previous_phase = read_byte(host, record + 0x3dU);
        auto phase = static_cast<std::uint8_t>(previous_phase + 4U);
        write_byte(host, record + 0x3dU, phase);
        add(r, previous_phase, 4U, phase, 0x80U, 0xffU);
        phase = read_byte(host, record + 0x3dU);
        phase &= 0x1cU;
        write_byte(host, record + 0x3dU, phase);
        logic(r, phase, 0x80U, 0xffU);
        phase = read_byte(host, record + 0x3dU);
        const auto previous_low = static_cast<std::uint8_t>(r.data[0]);
        byte = static_cast<std::uint8_t>(previous_low + phase);
        r.data[0] = (r.data[0] & 0xffffff00U) | byte;
        add(r, previous_low, phase, byte, 0x80U, 0xffU);

        r.address[0] = 0x000134eaU;
        const auto index = static_cast<std::int16_t>(r.data[0] & 0xffffU);
        word = read_word(host, static_cast<std::uint32_t>(r.address[0] + index));
        write_word(host, record + 6U, word);
        logic(r, word, 0x8000U, 0xffffU);
        byte = read_byte(host, static_cast<std::uint32_t>(r.address[0] + index + 2));
        write_byte(host, record + 1U, byte);
        logic(r, byte, 0x80U, 0xffU);
    }

    result = call_child(context, 280U, 0x0001271eU,
        0x00015d24U, 0x00012724U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    result = call_child(context, 281U, 0x00012724U,
        0x00015d3cU, 0x0001272aU);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    result = call_child(context, 282U, 0x0001272aU,
        0x00015df2U, 0x00012730U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    return return_from(context);
}
} // namespace gain_ground::translated
