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

FunctionResult transfer_expired(FunctionContext &context, std::uint32_t site)
{
    context.registers.program_counter = 0x00012d08U;
    return context.host->call_function(261U, 1U, 0x72U, 1U,
        site, 0x00012d08U, context);
}
} // namespace

FunctionResult cpu_b_apply_record_delta(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x00012058U)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    FunctionResult projectile_result;
    if (context.host->run_projectile_update(context, projectile_result)) return projectile_result;

    auto &r = context.registers;
    auto &host = *context.host;
    auto result = call_child(context, 253U, 0x00012058U,
        0x0001283cU, 0x0001205cU);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    if ((r.status & kCarry) != 0U)
        return transfer_expired(context, 0x0001205cU);

    result = call_child(context, 258U, 0x00012060U,
        0x00012bb0U, 0x00012064U);
    if (result.status != TranslationStatus::complete)
        return result;
    if (result.control == 8U)
        return FunctionResult::complete(1U, result.exit_program_counter);
    if (result.control != 1U)
        return result;
    if ((r.status & kCarry) != 0U)
        return transfer_expired(context, 0x00012064U);

    load_record_long(context, 0x1eU);
    add_record_long(context, 0x12U);
    load_record_long(context, 0x26U);
    add_record_long(context, 0x1aU);
    load_record_long(context, 0x22U);

    const auto velocity = read_long(host, r.address[5] + 0x22U);
    const auto decelerated = velocity - 0x4000U;
    write_long_read_modify_write(host, r.address[5] + 0x22U, decelerated);
    subtract_long(r, velocity, 0x4000U, decelerated);

    add_record_long(context, 0x16U);
    auto word = read_word(host, r.address[5] + 0x16U);
    r.data[0] = (r.data[0] & 0xffff0000U) | word;
    logic(r, word, 0x8000U, 0xffffU);
    const auto shifted = arithmetic_shift_right_word(r, word);
    r.data[0] = (r.data[0] & 0xffff0000U) | shifted;
    word = static_cast<std::uint16_t>(shifted + 0x3fU);
    r.data[0] = (r.data[0] & 0xffff0000U) | word;
    add(r, shifted, 0x3fU, word, 0x8000U, 0xffffU);
    const auto byte = static_cast<std::uint8_t>(word);
    write_byte(host, r.address[5] + 0x10U, byte);
    logic(r, byte, 0x80U, 0xffU);
    write_byte(host, r.address[5] + 0x11U, byte);
    logic(r, byte, 0x80U, 0xffU);

    result = call_child(context, 280U, 0x0001209aU,
        0x00015d24U, 0x000120a0U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    result = call_child(context, 281U, 0x000120a0U,
        0x00015d3cU, 0x000120a6U);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    result = call_child(context, 282U, 0x000120a6U,
        0x00015df2U, 0x000120acU);
    if (result.status != TranslationStatus::complete || result.control != 1U)
        return result;
    return return_from(context);
}
} // namespace gain_ground::translated
