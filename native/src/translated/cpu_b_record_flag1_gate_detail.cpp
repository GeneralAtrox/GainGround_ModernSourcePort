#include "cpu_b_record_flag1_gate_detail.h"

namespace gain_ground::translated {
namespace cpu_b_record_flag1_gate_detail {
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
    bool preserve_x)
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
} // namespace cpu_b_record_flag1_gate_detail

} // namespace gain_ground::translated
