#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
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
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void set_logic_flags(CpuRegisters &r, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_zero_only(CpuRegisters &r, bool zero)
{
    if (zero) r.status = static_cast<std::uint16_t>(r.status | 0x0004U);
    else r.status = static_cast<std::uint16_t>(r.status & ~0x0004U);
}

[[nodiscard]] std::uint8_t add_byte(CpuRegisters &r,
    std::uint8_t left, std::uint8_t right)
{
    const auto wide = static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(left) + static_cast<std::uint16_t>(right));
    const auto result = static_cast<std::uint8_t>(wide);
    std::uint16_t flags{};
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if (wide > 0xff) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    return result;
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(
        host.read_memory_word(kRegion, address, kWordMask)) << 16U)
        | host.read_memory_word(kRegion, address + 2U, kWordMask);
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    host.write_memory_word(kRegion, r.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, r.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] FunctionResult call_child(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t continuation)
{
    auto &host = *context.host;
    push_return(host, context.registers, continuation);
    context.registers.program_counter = target;
    return host.call_function(function_id, 1U, 0x72U, 2U,
        callsite, target, context);
}

[[nodiscard]] FunctionResult finish(FunctionContext &context)
{
    auto &host = *context.host;
    auto &r = context.registers;
    const auto target = read_long(host, r.address[7]);
    r.address[7] += 4U;
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

[[nodiscard]] bool child_complete(const FunctionResult &result)
{
    return result.status == TranslationStatus::complete
        && result.control == 1U;
}

[[nodiscard]] FunctionResult run_tail(FunctionContext &context,
    std::uint32_t accumulator_callsite, std::uint32_t accumulator_continuation,
    std::uint32_t decode_callsite, std::uint32_t decode_continuation)
{
    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    auto child = call_child(context, 347U, accumulator_callsite,
        0x0001da58U, accumulator_continuation);
    if (!child_complete(child)) return child;

    r.address[0] = read_long(host, base + 0x6eU);
    const auto adjustment = read_byte(host, r.address[0] + 3U);
    r.data[0] = (r.data[0] & 0xffffff00U) | adjustment;
    set_logic_flags(r, adjustment, 0x80U, 0xffU);
    const auto source = read_byte(host, base + 0x37U);
    write_byte(host, base + 0x36U, source);
    set_logic_flags(r, source, 0x80U, 0xffU);
    const auto result = add_byte(r, read_byte(host, base + 0x36U), adjustment);
    write_byte(host, base + 0x36U, result);

    child = call_child(context, 364U, decode_callsite,
        0x0001ec00U, decode_continuation);
    if (!child_complete(child)) return child;
    const auto decoded = static_cast<std::uint16_t>(r.data[1]);
    host.write_memory_word(kRegion, base + 0x5cU, decoded, kWordMask);
    set_logic_flags(r, decoded, 0x8000U, 0xffffU);
    return finish(context);
}
} // namespace

FunctionResult cpu_b_record_flag5_dispatch_common(
    FunctionContext &context,
    std::uint32_t scan_function_id,
    std::uint32_t scan_target,
    std::uint32_t first_scan_callsite,
    std::uint32_t first_scan_continuation,
    std::uint32_t first_accumulator_callsite,
    std::uint32_t first_accumulator_continuation,
    std::uint32_t first_decode_callsite,
    std::uint32_t first_decode_continuation,
    std::uint32_t second_scan_callsite,
    std::uint32_t second_scan_continuation,
    std::uint32_t second_accumulator_callsite,
    std::uint32_t second_accumulator_continuation,
    std::uint32_t second_decode_callsite,
    std::uint32_t second_decode_continuation) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    const auto flag5 = [&]() {
        const bool set = (read_byte(host, base + 0x41U) & 0x20U) != 0U;
        set_zero_only(r, !set);
        return set;
    };

    if (!flag5()) {
        const auto scan = call_child(context, scan_function_id,
            first_scan_callsite, scan_target, first_scan_continuation);
        if (!child_complete(scan)) return scan;
        if (!flag5()) return finish(context);

        auto phase = host.read_memory_word(kRegion, base + 0x54U, kWordMask);
        r.data[0] = (r.data[0] & 0xffff0000U) | phase;
        set_logic_flags(r, phase, 0x8000U, 0xffffU);
        phase = static_cast<std::uint16_t>(phase & 0x000fU);
        r.data[0] = (r.data[0] & 0xffff0000U) | phase;
        set_logic_flags(r, phase, 0x8000U, 0xffffU);
        if (phase == 0U) return finish(context);
        return run_tail(context, first_accumulator_callsite,
            first_accumulator_continuation,
            first_decode_callsite, first_decode_continuation);
    }

    const auto scan = call_child(context, scan_function_id,
        second_scan_callsite, scan_target, second_scan_continuation);
    if (!child_complete(scan)) return scan;
    if (!flag5()) return finish(context);
    const bool secondary_flag5 = (read_byte(host, base + 0x40U) & 0x20U) != 0U;
    set_zero_only(r, !secondary_flag5);
    if (!secondary_flag5) return finish(context);
    return run_tail(context, second_accumulator_callsite,
        second_accumulator_continuation,
        second_decode_callsite, second_decode_continuation);
}

FunctionResult cpu_b_record_flag5_dispatch_variant_3(FunctionContext &context) noexcept
{
    return cpu_b_record_flag5_dispatch_common(context,
        343U, 0x0001d86aU,
        0x0001d542U, 0x0001d546U,
        0x0001d55cU, 0x0001d560U,
        0x0001d572U, 0x0001d576U,
        0x0001d57cU, 0x0001d580U,
        0x0001d594U, 0x0001d598U,
        0x0001d5aaU, 0x0001d5aeU);
}

} // namespace gain_ground::translated
