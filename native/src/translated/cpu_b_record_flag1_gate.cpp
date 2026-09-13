#include "gain_ground/contract_types.h"

#include <cstddef>
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto word = host.read_memory_word(
        kRegion, address & ~1U, odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    host.write_memory_word(kRegion, address & ~1U,
        static_cast<std::uint16_t>(odd ? value : value << 8U),
        odd ? 0x00ffU : 0xff00U);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
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
    r.status = static_cast<std::uint16_t>(
        (r.status & ~0x0004U) | (zero ? 0x0004U : 0U));
}

void set_add_word_flags(CpuRegisters &r,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub_word_flags(CpuRegisters &r,
    std::uint16_t left, std::uint16_t right, std::uint16_t result,
    bool preserve_x = false)
{
    const auto old_x = static_cast<std::uint16_t>(r.status & 0x0010U);
    std::uint16_t flags{};
    if (left < right) flags |= 0x0011U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (preserve_x) flags = static_cast<std::uint16_t>((flags & ~0x0010U) | old_x);
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub_byte_flags(CpuRegisters &r,
    std::uint8_t left, std::uint8_t right, std::uint8_t result)
{
    const auto old_x = static_cast<std::uint16_t>(r.status & 0x0010U);
    std::uint16_t flags{};
    if (left < right) flags |= 0x0001U;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    r.status = static_cast<std::uint16_t>(
        (r.status & ~0x001fU) | flags | old_x);
}

void set_asl_word_flags(CpuRegisters &r, std::uint16_t input,
    unsigned count, std::uint16_t result)
{
    bool carry = false;
    bool overflow = false;
    auto value = input;
    bool prior_sign = (value & 0x8000U) != 0U;
    for (unsigned index = 0; index < count; ++index) {
        carry = (value & 0x8000U) != 0U;
        value = static_cast<std::uint16_t>(value << 1U);
        const bool sign = (value & 0x8000U) != 0U;
        overflow = overflow || sign != prior_sign;
        prior_sign = sign;
    }
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if (overflow) flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_data_word(CpuRegisters &r, std::size_t index, std::uint16_t value)
{
    r.data[index] = (r.data[index] & 0xffff0000U) | value;
}

void bit_clear(ExecutionHost &host, CpuRegisters &r,
    std::uint32_t address, std::uint8_t mask)
{
    const auto old = read_byte(host, address);
    set_zero_only(r, (old & mask) == 0U);
    write_byte(host, address, static_cast<std::uint8_t>(old & ~mask));
}

void bit_set(ExecutionHost &host, CpuRegisters &r,
    std::uint32_t address, std::uint8_t mask)
{
    const auto old = read_byte(host, address);
    set_zero_only(r, (old & mask) == 0U);
    write_byte(host, address, static_cast<std::uint8_t>(old | mask));
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    write_long(host, r.address[7], value);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto value = read_long(host, r.address[7]);
    r.address[7] += 4U;
    return value;
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

[[nodiscard]] bool signed_less(const CpuRegisters &r)
{
    return ((r.status & 0x0008U) != 0U) != ((r.status & 0x0002U) != 0U);
}

[[nodiscard]] bool signed_less_equal(const CpuRegisters &r)
{
    return (r.status & 0x0004U) != 0U || signed_less(r);
}

[[nodiscard]] FunctionResult return_from_function(ExecutionHost &host, CpuRegisters &r)
{
    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_record_flag1_gate(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    const auto base_flags_address = base + 0x40U;

    auto base_flags = read_byte(host, base_flags_address);
    set_zero_only(r, (base_flags & 0x02U) == 0U);
    if ((base_flags & 0x02U) != 0U)
        return return_from_function(host, r);

    const auto gate = read_byte(host, base + 0x3eU);
    set_logic_flags(r, gate, 0x80U, 0xffU);
    if (gate != 0U)
        return return_from_function(host, r);

    r.address[3] = 0x00006c00U;
    auto table_marker = host.read_memory_word(kRegion, r.address[3], kWordMask);
    r.address[3] += 2U;
    set_logic_flags(r, table_marker, 0x8000U, 0xffffU);
    if ((table_marker & 0x8000U) == 0U)
        r.address[3] = 0x00007002U;

    auto d0 = host.read_memory_word(kRegion, base + 0x1aU, kWordMask);
    set_data_word(r, 0U, d0);
    set_logic_flags(r, d0, 0x8000U, 0xffffU);
    const auto compare_0200 = static_cast<std::uint16_t>(d0 - 0x0200U);
    set_sub_word_flags(r, d0, 0x0200U, compare_0200, true);
    if ((r.status & 0x0001U) == 0U) {
        r.program_counter = 0x0001dcd6U;
        return host.call_function(552U, 1U, 0x72U, 1U,
            0x0001dc16U, 0x0001dcd6U, context);
    }

    set_data_word(r, 7U, d0);
    set_logic_flags(r, d0, 0x8000U, 0xffffU);
    auto d7 = static_cast<std::uint16_t>(d0 + 0x0020U);
    set_add_word_flags(r, d0, 0x0020U, d7);
    set_data_word(r, 7U, d7);

    auto scan_offset = static_cast<std::uint16_t>(d0 - 0x0020U);
    set_sub_word_flags(r, d0, 0x0020U, scan_offset);
    set_data_word(r, 0U, scan_offset);

    if ((scan_offset & 0x8000U) == 0U) {
        const auto clamp_compare = static_cast<std::uint16_t>(d7 - 0x01ffU);
        set_sub_word_flags(r, d7, 0x01ffU, clamp_compare, true);
        if (!signed_less_equal(r)) {
            d7 = 0x01ffU;
            set_data_word(r, 7U, d7);
            set_logic_flags(r, d7, 0x8000U, 0xffffU);
        }

        const auto reduced = static_cast<std::uint16_t>(d7 - scan_offset);
        set_sub_word_flags(r, d7, scan_offset, reduced);
        d7 = reduced;
        set_data_word(r, 7U, d7);

        const auto doubled = static_cast<std::uint16_t>(scan_offset + scan_offset);
        set_add_word_flags(r, scan_offset, scan_offset, doubled);
        scan_offset = doubled;
        set_data_word(r, 0U, scan_offset);
        r.address[3] = static_cast<std::uint32_t>(
            r.address[3] + static_cast<std::int16_t>(scan_offset));

        bit_clear(host, r, base_flags_address, 0x02U);
        bit_clear(host, r, base_flags_address, 0x40U);
        bit_clear(host, r, base_flags_address, 0x01U);
        bit_clear(host, r, base_flags_address, 0x80U);
    }

    for (;;) {
        d0 = host.read_memory_word(kRegion, r.address[3], kWordMask);
        r.address[3] += 2U;
        set_data_word(r, 0U, d0);
        set_logic_flags(r, d0, 0x8000U, 0xffffU);

        if (d0 != 0U) {
            r.address[6] = static_cast<std::uint32_t>(
                static_cast<std::int32_t>(static_cast<std::int16_t>(d0)));
            const auto current_kind = read_byte(host, base + 0x0aU);
            r.data[0] = (r.data[0] & 0xffffff00U) | current_kind;
            set_logic_flags(r, current_kind, 0x80U, 0xffU);
            const auto candidate_kind = read_byte(host, r.address[6] + 0x0aU);
            const auto kind_difference = static_cast<std::uint8_t>(
                current_kind - candidate_kind);
            set_sub_byte_flags(r, current_kind, candidate_kind, kind_difference);

            if (kind_difference != 0U) {
                r.data[0] = 0U;
                set_logic_flags(r, 0U, 0x80000000U, 0xffffffffU);
                const auto selector = read_byte(host, r.address[6] + 0x0bU);
                r.data[0] = selector;
                set_logic_flags(r, selector, 0x80U, 0xffU);
                const auto shifted = static_cast<std::uint16_t>(selector << 2U);
                set_asl_word_flags(r, selector, 2U, shifted);
                set_data_word(r, 0U, shifted);

                bool test_common_bounds = false;
                bool test_small_bounds = false;
                bool test_record_bounds = false;
                switch (selector) {
                case 1U: test_small_bounds = true; break;
                case 4U: test_record_bounds = true; break;
                case 8U:
                case 9U: test_common_bounds = true; break;
                default: break;
                }

                if (test_common_bounds) {
                    const auto active = read_byte(host, r.address[6] + 0x3fU);
                    set_logic_flags(r, active, 0x80U, 0xffU);
                    if (active == 0U) {
                        set_data_word(r, 1U, 0xfff6U);
                        set_logic_flags(r, 0xfff6U, 0x8000U, 0xffffU);
                        set_data_word(r, 2U, 0x000aU);
                        set_logic_flags(r, 0x000aU, 0x8000U, 0xffffU);
                        set_data_word(r, 3U, 0xfff7U);
                        set_logic_flags(r, 0xfff7U, 0x8000U, 0xffffU);
                        set_data_word(r, 4U, 0x0009U);
                        set_logic_flags(r, 0x0009U, 0x8000U, 0xffffU);
                        const auto x = host.read_memory_word(
                            kRegion, r.address[6] + 0x12U, kWordMask);
                        set_data_word(r, 0U, x);
                        set_logic_flags(r, x, 0x8000U, 0xffffU);
                        for (const auto index : {1U, 2U}) {
                            const auto old = static_cast<std::uint16_t>(r.data[index]);
                            const auto value = static_cast<std::uint16_t>(old + x);
                            set_add_word_flags(r, old, x, value);
                            set_data_word(r, index, value);
                        }
                        const auto y = host.read_memory_word(
                            kRegion, r.address[6] + 0x1aU, kWordMask);
                        set_data_word(r, 0U, y);
                        set_logic_flags(r, y, 0x8000U, 0xffffU);
                        for (const auto index : {3U, 4U}) {
                            const auto old = static_cast<std::uint16_t>(r.data[index]);
                            const auto value = static_cast<std::uint16_t>(old + y);
                            set_add_word_flags(r, old, y, value);
                            set_data_word(r, index, value);
                        }

                        const auto child = call_child(context, 354U,
                            0x0001dcc6U, 0x0001ddf4U, 0x0001dccaU);
                        if (child.status != TranslationStatus::complete
                            || child.control != 1U)
                            return child;

                        base_flags = read_byte(host, base_flags_address);
                        set_zero_only(r, (base_flags & 0x02U) == 0U);
                        if ((base_flags & 0x02U) != 0U) {
                            const auto candidate_gate = read_byte(
                                host, r.address[6] + 0x3eU);
                            set_logic_flags(r, candidate_gate, 0x80U, 0xffU);
                            if (candidate_gate == 0U) {
                                base_flags = read_byte(host, base_flags_address);
                                set_zero_only(r, (base_flags & 0x20U) == 0U);
                                auto phase = host.read_memory_word(
                                    kRegion, base + 0x5cU, kWordMask);
                                if ((base_flags & 0x20U) != 0U) {
                                    set_data_word(r, 0U, phase);
                                    set_logic_flags(r, phase, 0x8000U, 0xffffU);
                                    const auto old = phase;
                                    phase = static_cast<std::uint16_t>(phase + 0x0400U);
                                    set_data_word(r, 0U, phase);
                                    set_add_word_flags(r, old, 0x0400U, phase);
                                    phase = static_cast<std::uint16_t>(phase & 0x07ffU);
                                    set_data_word(r, 0U, phase);
                                    set_logic_flags(r, phase, 0x8000U, 0xffffU);
                                } else {
                                    const auto old = phase;
                                    phase = static_cast<std::uint16_t>(phase + 0x0100U);
                                    set_add_word_flags(r, old, 0x0100U, phase);
                                }
                                host.write_memory_word(
                                    kRegion, base + 0x5cU, phase, kWordMask);
                            } else {
                                auto phase = host.read_memory_word(
                                    kRegion, base + 0x5cU, kWordMask);
                                const auto old = phase;
                                phase = static_cast<std::uint16_t>(phase + 0x0100U);
                                set_add_word_flags(r, old, 0x0100U, phase);
                                host.write_memory_word(
                                    kRegion, base + 0x5cU, phase, kWordMask);
                            }

                            const auto current_direction = read_byte(host, base + 0x36U);
                            r.data[0] = (r.data[0] & 0xffffff00U) | current_direction;
                            set_logic_flags(r, current_direction, 0x80U, 0xffU);
                            const auto candidate_direction = read_byte(
                                host, r.address[6] + 0x36U);
                            const auto direction_difference = static_cast<std::uint8_t>(
                                current_direction - candidate_direction);
                            set_sub_byte_flags(r, current_direction,
                                candidate_direction, direction_difference);
                            if (!signed_less(r))
                                write_byte(host, r.address[6] + 0x36U,
                                    read_byte(host, base + 0x36U));
                            write_byte(host, base + 0x36U,
                                read_byte(host, r.address[6] + 0x36U));
                            bit_set(host, r, r.address[6] + 0x40U, 0x40U);
                            auto masked_phase = host.read_memory_word(
                                kRegion, base + 0x5cU, kWordMask);
                            masked_phase = static_cast<std::uint16_t>(masked_phase & 0x0700U);
                            host.write_memory_word(
                                kRegion, base + 0x5cU, masked_phase, kWordMask);
                            set_logic_flags(r, masked_phase, 0x8000U, 0xffffU);
                            write_byte(host, base + 0x5eU, 7U);
                            set_logic_flags(r, 7U, 0x80U, 0xffU);
                            bit_set(host, r, base_flags_address, 0x01U);
                            bit_set(host, r, base_flags_address, 0x02U);
                            return return_from_function(host, r);
                        }
                    }
                } else if (test_small_bounds) {
                    const auto active = read_byte(host, r.address[6] + 0x3fU);
                    set_logic_flags(r, active, 0x80U, 0xffU);
                    if (active == 0U) {
                        set_data_word(r, 1U, 0xfff9U);
                        set_logic_flags(r, 0xfff9U, 0x8000U, 0xffffU);
                        set_data_word(r, 2U, 0x0007U);
                        set_logic_flags(r, 0x0007U, 0x8000U, 0xffffU);
                        set_data_word(r, 3U, 0xfffaU);
                        set_logic_flags(r, 0xfffaU, 0x8000U, 0xffffU);
                        set_data_word(r, 4U, 0x0006U);
                        set_logic_flags(r, 0x0006U, 0x8000U, 0xffffU);
                        const auto x = host.read_memory_word(
                            kRegion, r.address[6] + 0x12U, kWordMask);
                        set_data_word(r, 0U, x);
                        set_logic_flags(r, x, 0x8000U, 0xffffU);
                        for (const auto index : {1U, 2U}) {
                            const auto old = static_cast<std::uint16_t>(r.data[index]);
                            const auto value = static_cast<std::uint16_t>(old + x);
                            set_add_word_flags(r, old, x, value);
                            set_data_word(r, index, value);
                        }
                        const auto y = host.read_memory_word(
                            kRegion, r.address[6] + 0x1aU, kWordMask);
                        set_data_word(r, 0U, y);
                        set_logic_flags(r, y, 0x8000U, 0xffffU);
                        for (const auto index : {3U, 4U}) {
                            const auto old = static_cast<std::uint16_t>(r.data[index]);
                            const auto value = static_cast<std::uint16_t>(old + y);
                            set_add_word_flags(r, old, y, value);
                            set_data_word(r, index, value);
                        }
                        const auto child = call_child(context, 354U,
                            0x0001dd82U, 0x0001ddf4U, 0x0001dd86U);
                        if (child.status != TranslationStatus::complete
                            || child.control != 1U)
                            return child;
                        base_flags = read_byte(host, base_flags_address);
                        set_zero_only(r, (base_flags & 0x02U) == 0U);
                        if ((base_flags & 0x02U) != 0U) {
                            bit_set(host, r, base_flags_address, 0x80U);
                            bit_clear(host, r, base_flags_address, 0x02U);
                            const auto source_index =
                                read_byte(host, r.address[6] + 0x0aU);
                            write_byte(host, base + 0x60U, source_index);
                            set_logic_flags(r, source_index, 0x80U, 0xffU);
                            write_byte(host, base + 0x59U, 0x1eU);
                            set_logic_flags(r, 0x1eU, 0x80U, 0xffU);
                            const auto reset = call_child(context, 347U,
                                0x0001dda8U, 0x0001da58U, 0x0001ddacU);
                            if (reset.status != TranslationStatus::complete
                                || reset.control != 1U)
                                return reset;
                            r.program_counter = 0x0001dcd6U;
                            return host.call_function(552U, 1U, 0x72U, 1U,
                                0x0001ddacU, 0x0001dcd6U, context);
                        }
                    }
                } else if (test_record_bounds) {
                    set_data_word(r, 1U, host.read_memory_word(
                        kRegion, r.address[6] + 0x2aU, kWordMask));
                    set_logic_flags(r, r.data[1], 0x8000U, 0xffffU);
                    set_data_word(r, 2U, host.read_memory_word(
                        kRegion, r.address[6] + 0x2cU, kWordMask));
                    set_logic_flags(r, r.data[2], 0x8000U, 0xffffU);
                    set_data_word(r, 3U, host.read_memory_word(
                        kRegion, r.address[6] + 0x32U, kWordMask));
                    set_logic_flags(r, r.data[3], 0x8000U, 0xffffU);
                    set_data_word(r, 4U, host.read_memory_word(
                        kRegion, r.address[6] + 0x34U, kWordMask));
                    set_logic_flags(r, r.data[4], 0x8000U, 0xffffU);
                    const auto child = call_child(context, 354U,
                        0x0001ddc0U, 0x0001ddf4U, 0x0001ddc4U);
                    if (child.status != TranslationStatus::complete
                        || child.control != 1U)
                        return child;
                    base_flags = read_byte(host, base_flags_address);
                    set_zero_only(r, (base_flags & 0x02U) == 0U);
                    if ((base_flags & 0x02U) != 0U) {
                        bit_set(host, r, base_flags_address, 0x40U);
                        auto phase = host.read_memory_word(
                            kRegion, base + 0x5cU, kWordMask);
                        const auto added = static_cast<std::uint16_t>(phase + 0x0100U);
                        set_add_word_flags(r, phase, 0x0100U, added);
                        host.write_memory_word(
                            kRegion, base + 0x5cU, added, kWordMask);
                        phase = host.read_memory_word(
                            kRegion, base + 0x5cU, kWordMask);
                        phase = static_cast<std::uint16_t>(phase & 0x0700U);
                        set_logic_flags(r, phase, 0x8000U, 0xffffU);
                        host.write_memory_word(
                            kRegion, base + 0x5cU, phase, kWordMask);
                        write_byte(host, base + 0x5eU, 7U);
                        set_logic_flags(r, 7U, 0x80U, 0xffU);
                        bit_set(host, r, base_flags_address, 0x01U);
                        bit_set(host, r, base_flags_address, 0x02U);
                        return return_from_function(host, r);
                    }
                }
            }
        }

        d7 = static_cast<std::uint16_t>(d7 - 1U);
        set_data_word(r, 7U, d7);
        if (d7 == 0xffffU) {
            r.program_counter = 0x0001dcd6U;
            return host.call_function(552U, 1U, 0x72U, 1U,
                0x0001dcd2U, 0x0001dcd6U, context);
        }
    }
}

} // namespace gain_ground::translated
