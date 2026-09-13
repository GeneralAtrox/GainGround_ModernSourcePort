#include "cpu_b_step_record_phase_counters_common.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;
constexpr std::int32_t kLimit = 0x00028000;

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address, kMask);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = read_word(host, address);
    const auto low = read_word(host, address + 2U);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address, value, kMask);
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
}

void set_logic_long(CpuRegisters &r, std::uint32_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub_long(CpuRegisters &r, std::uint32_t left,
    std::uint32_t right, std::uint32_t result, bool compare)
{
    std::uint16_t flags = compare ? static_cast<std::uint16_t>(r.status & 0x0010U) : 0U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x80000000U) != 0U) flags |= 0x0002U;
    if (left < right) flags |= compare ? 0x0001U : 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_logic_word(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] std::int32_t clamp_delta(
    CpuRegisters &r, std::uint32_t destination, std::uint32_t source,
    unsigned data_index)
{
    const auto raw = destination - source;
    r.data[data_index] = raw;
    set_sub_long(r, destination, source, raw, false);
    const auto signed_raw = static_cast<std::int32_t>(raw);
    if (signed_raw > kLimit) {
        r.data[data_index] = static_cast<std::uint32_t>(kLimit);
        set_logic_long(r, r.data[data_index]);
        return kLimit;
    }
    if (signed_raw < -kLimit) {
        r.data[data_index] = static_cast<std::uint32_t>(-kLimit);
        set_logic_long(r, r.data[data_index]);
        return -kLimit;
    }
    return signed_raw;
}
} // namespace

FunctionResult cpu_b_clamp_record_delta(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    r.address[0] = 0x00013dbaU;

    const auto destination_x = read_long(host, base + 0x62U);
    const auto current_x = read_long(host, base + 0x12U);
    const auto delta_x = clamp_delta(r, destination_x, current_x, 0U);
    if (delta_x <= 0) {
        r.address[0] += 6U;
        if (delta_x < 0) r.address[0] += 6U;
    }

    const auto destination_y = read_long(host, base + 0x66U);
    const auto current_y = read_long(host, base + 0x1aU);
    const auto delta_y = clamp_delta(r, destination_y, current_y, 1U);
    if (delta_y <= 0) {
        r.address[0] += 2U;
        if (delta_y < 0) r.address[0] += 2U;
    }

    const auto selector = read_word(host, r.address[0]);
    write_word(host, base + 0x58U, selector);
    set_logic_word(r, selector);

    if ((selector & 0x8000U) == 0U) {
        const auto accumulated_x = read_long(host, base + 0x12U)
            + static_cast<std::uint32_t>(delta_x);
        write_long(host, base + 0x12U, accumulated_x);
        set_logic_long(r, accumulated_x);

        const auto accumulated_y = read_long(host, base + 0x1aU)
            + static_cast<std::uint32_t>(delta_y);
        write_long(host, base + 0x1aU, accumulated_y);
        set_logic_long(r, accumulated_y);
        return cpu_b_step_record_phase_counters_common(context, true);
    }

    r.program_counter = 0x00013962U;
    (void)host.call_function(272U, 1U, 0x72U, 1U,
        0x00013956U, 0x00013962U, context);
    return FunctionResult::complete(3U, 0x00013962U);
}

} // namespace gain_ground::translated
