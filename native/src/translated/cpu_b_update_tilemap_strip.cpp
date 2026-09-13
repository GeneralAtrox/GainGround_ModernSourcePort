#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kPrivateRegion, location.offset, location.mask) >> location.shift);
}

void set_logic_byte_flags(CpuRegisters &r, std::uint8_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_btst_zero(CpuRegisters &r, bool bit_set)
{
    r.status = static_cast<std::uint16_t>(
        (r.status & ~0x0004U) | (bit_set ? 0U : 0x0004U));
}

void set_compare_word_flags(CpuRegisters &r,
    std::uint16_t destination, std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    std::uint16_t flags = r.status & 0x0010U;
    if (destination < source) flags |= 0x0001U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion, r.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, r.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto high = host.read_memory_word(
        kPrivateRegion, r.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, r.address[7] + 2U, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

[[nodiscard]] FunctionResult finish(FunctionContext &context)
{
    auto &r = context.registers;
    const auto target = pop_return(*context.host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

[[nodiscard]] FunctionResult tail_call(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite, std::uint32_t target)
{
    context.registers.program_counter = target;
    (void)context.host->call_function(function_id, 1U, 0x72U, 1U,
        callsite, target, context);
    return FunctionResult::complete(3U, target);
}

[[nodiscard]] FunctionResult test_bit_two_and_finish(FunctionContext &context)
{
    auto &r = context.registers;
    const auto d4 = static_cast<std::uint8_t>(r.data[4]);
    const bool bit_two = (d4 & 0x04U) != 0U;
    set_btst_zero(r, bit_two);
    if (bit_two)
        return tail_call(context, 292U, 0x0000fa1eU, 0x00015ffaU);
    return finish(context);
}

[[nodiscard]] FunctionResult continue_after_children(FunctionContext &context)
{
    auto &host = *context.host;
    auto &r = context.registers;
    const auto mode = host.read_memory_word(
        kPrivateRegion, 0x00000c00U, kWordMask);
    set_compare_word_flags(r, mode, 3U);
    if (mode >= 3U) return finish(context);
    r.address[1] = 0x000104f6U;
    const auto d4 = static_cast<std::uint8_t>(r.data[4]);
    const bool bit_one = (d4 & 0x02U) != 0U;
    set_btst_zero(r, bit_one);
    if (bit_one)
        return tail_call(context, 290U, 0x0000f9faU, 0x00015fd4U);
    return test_bit_two_and_finish(context);
}
} // namespace

FunctionResult cpu_b_update_tilemap_strip(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    if (r.program_counter == 0x0000f9e8U)
        return continue_after_children(context);
    if (r.program_counter != 0x0000f9c4U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};

    auto d4 = read_byte(host, 0x00000833U);
    r.data[4] = (r.data[4] & 0xffffff00U) | d4;
    set_logic_byte_flags(r, d4);
    r.address[1] = 0x000104daU;
    const auto a6_word = host.read_memory_word(
        kPrivateRegion, r.address[5] + 0x68U, kWordMask);
    r.address[6] = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(a6_word)));
    auto d7 = read_byte(host, r.address[6]);
    r.data[7] = (r.data[7] & 0xffffff00U) | d7;
    set_logic_byte_flags(r, d7);
    d7 = static_cast<std::uint8_t>(d7 | read_byte(host, 0x00000404U));
    r.data[7] = (r.data[7] & 0xffffff00U) | d7;
    set_logic_byte_flags(r, d7);

    if (d7 != 0U) {
        const bool bit_three = (d4 & 0x08U) != 0U;
        set_btst_zero(r, bit_three);
        if (bit_three) {
            push_return(host, r, 0x0000f9e4U);
            r.program_counter = 0x00015ffaU;
            auto child = host.call_function(292U, 1U, 0x72U, 2U,
                0x0000f9deU, 0x00015ffaU, context);
            if (child.status != TranslationStatus::complete || child.control != 1U)
                return child;

            push_return(host, r, 0x0000f9e8U);
            r.program_counter = 0x0000fa30U;
            child = host.call_function(182U, 1U, 0x72U, 2U,
                0x0000f9e4U, 0x0000fa30U, context);
            if (child.status != TranslationStatus::complete || child.control != 1U)
                return child;

            return continue_after_children(context);
        } else {
            r.address[1] = 0x000104f6U;
        }
    }

    const auto mode = host.read_memory_word(
        kPrivateRegion, 0x00000c00U, kWordMask);
    set_compare_word_flags(r, mode, 3U);
    if (mode < 3U) {
        d4 = static_cast<std::uint8_t>(r.data[4]);
        const bool bit_zero = (d4 & 0x01U) != 0U;
        set_btst_zero(r, bit_zero);
        if (bit_zero)
            return tail_call(context, 290U, 0x0000fa12U, 0x00015fd4U);
    }

    return test_bit_two_and_finish(context);
}

} // namespace gain_ground::translated
