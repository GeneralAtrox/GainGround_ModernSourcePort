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

void set_data_word(CpuRegisters &r, std::size_t index, std::uint16_t value)
{
    r.data[index] = (r.data[index] & 0xffff0000U) | value;
    set_logic_flags(r, value, 0x8000U, 0xffffU);
}

void write_word(ExecutionHost &host, CpuRegisters &r,
    std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address, value, kWordMask);
    set_logic_flags(r, value, 0x8000U, 0xffffU);
}

void clear_word(ExecutionHost &host, CpuRegisters &r, std::uint32_t address)
{
    static_cast<void>(host.read_memory_word(kRegion, address, kWordMask));
    host.write_memory_word(kRegion, address, 0U, kWordMask);
    set_logic_flags(r, 0U, 0x8000U, 0xffffU);
}

void add_word(CpuRegisters &r, std::size_t index, std::uint16_t right)
{
    const auto left = static_cast<std::uint16_t>(r.data[index]);
    const auto result = static_cast<std::uint16_t>(left + right);
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x0011U;
    r.data[index] = (r.data[index] & 0xffff0000U) | result;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void and_word(CpuRegisters &r, std::size_t index, std::uint16_t mask)
{
    set_data_word(r, index, static_cast<std::uint16_t>(r.data[index]) & mask);
}

void logical_shift_right_word(CpuRegisters &r, std::size_t index, unsigned count)
{
    const auto original = static_cast<std::uint16_t>(r.data[index]);
    const bool carry = ((original >> (count - 1U)) & 1U) != 0U;
    const auto result = static_cast<std::uint16_t>(original >> count);
    r.data[index] = (r.data[index] & 0xffff0000U) | result;
    std::uint16_t flags{};
    if (result == 0U) flags |= 0x0004U;
    if (carry) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void arithmetic_shift_left_word(CpuRegisters &r, std::size_t index, unsigned count)
{
    const auto original = static_cast<std::uint16_t>(r.data[index]);
    auto value = original;
    bool overflow{};
    for (unsigned step = 0; step < count; ++step) {
        const bool before = (value & 0x8000U) != 0U;
        value = static_cast<std::uint16_t>(value << 1U);
        overflow = overflow || (before != ((value & 0x8000U) != 0U));
    }
    const bool carry = ((original >> (16U - count)) & 1U) != 0U;
    r.data[index] = (r.data[index] & 0xffff0000U) | value;
    std::uint16_t flags{};
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    if (carry) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    write_long(host, r.address[7], value);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto result = read_long(host, r.address[7]);
    r.address[7] += 4U;
    return result;
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
} // namespace

FunctionResult cpu_b_populate_record_slot_geometry(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto source = r.address[0];
    const auto parent = r.address[5];
    const auto slot = r.address[6];

    r.data[0] = 0U;
    set_logic_flags(r, 0U, 0x80000000U, 0xffffffffU);
    const auto source_type = read_byte(host, source + 0x17U);
    r.data[0] = source_type;
    set_logic_flags(r, source_type, 0x80U, 0xffU);
    write_word(host, r, slot + 0x74U, source_type);

    set_data_word(r, 0U, static_cast<std::uint16_t>(r.data[5]));
    write_word(host, r, slot + 0x5cU, static_cast<std::uint16_t>(r.data[0]));
    add_word(r, 0U, 0x0080U);
    and_word(r, 0U, 0x0700U);
    logical_shift_right_word(r, 0U, 6U);

    r.address[0] = read_long(host, slot + 0x48U);
    const auto shape_offset = static_cast<std::int16_t>(r.data[0]);
    const auto shape_base = static_cast<std::uint32_t>(r.address[0] + shape_offset);
    write_word(host, r, slot + 0x06U,
        host.read_memory_word(kRegion, shape_base, kWordMask));
    write_word(host, r, slot,
        host.read_memory_word(kRegion, shape_base + 2U, kWordMask));

    r.address[0] = read_long(host, parent + 0x66U);
    set_data_word(r, 0U,
        host.read_memory_word(kRegion, r.address[0] + 0x18U, kWordMask));
    add_word(r, 0U, static_cast<std::uint16_t>(r.data[0]));
    set_data_word(r, 1U, static_cast<std::uint16_t>(r.data[0]));
    arithmetic_shift_left_word(r, 1U, 4U);
    add_word(r, 1U, static_cast<std::uint16_t>(r.data[0]));

    r.address[0] = static_cast<std::uint32_t>(0x0002a6e0U
        + static_cast<std::int16_t>(r.data[1]));
    set_data_word(r, 0U,
        host.read_memory_word(kRegion, parent + 0x16U, kWordMask));
    add_word(r, 0U, host.read_memory_word(kRegion, r.address[0], kWordMask));
    r.address[0] += 2U;
    auto value = static_cast<std::uint16_t>(r.data[0]);
    write_word(host, r, slot + 0x2eU, value);
    write_word(host, r, slot + 0x30U, value);
    write_word(host, r, slot + 0x16U, value);
    write_word(host, r, slot + 0x18U, 0U);

    set_data_word(r, 0U,
        host.read_memory_word(kRegion, parent + 0x5cU, kWordMask));
    add_word(r, 0U, 0x0080U);
    and_word(r, 0U, 0x0700U);
    logical_shift_right_word(r, 0U, 6U);
    r.address[0] = static_cast<std::uint32_t>(r.address[0]
        + static_cast<std::int16_t>(r.data[0]));

    set_data_word(r, 0U,
        host.read_memory_word(kRegion, parent + 0x12U, kWordMask));
    add_word(r, 0U, host.read_memory_word(kRegion, r.address[0], kWordMask));
    r.address[0] += 2U;
    value = static_cast<std::uint16_t>(r.data[0]);
    write_word(host, r, slot + 0x2aU, value);
    write_word(host, r, slot + 0x2cU, value);
    write_word(host, r, slot + 0x12U, value);
    clear_word(host, r, slot + 0x14U);

    set_data_word(r, 0U,
        host.read_memory_word(kRegion, parent + 0x1aU, kWordMask));
    add_word(r, 0U, host.read_memory_word(kRegion, r.address[0], kWordMask));
    r.address[0] += 2U;
    value = static_cast<std::uint16_t>(r.data[0]);
    write_word(host, r, slot + 0x32U, value);
    write_word(host, r, slot + 0x34U, value);
    write_word(host, r, slot + 0x1aU, value);
    clear_word(host, r, slot + 0x1cU);

    set_data_word(r, 1U,
        host.read_memory_word(kRegion, slot + 0x5cU, kWordMask));
    const auto lookup = call_child(context, 305U,
        0x0001f1deU, 0x0001636cU, 0x0001f1e4U);
    if (lookup.status != TranslationStatus::complete || lookup.control != 1U)
        return lookup;

    r.data[4] = 0U;
    set_logic_flags(r, 0U, 0x80000000U, 0xffffffffU);
    const auto direction = read_byte(host, slot + 0x36U);
    r.data[4] = direction;
    set_logic_flags(r, direction, 0x80U, 0xffU);
    set_data_word(r, 7U, static_cast<std::uint16_t>(r.data[5]));

    const auto transform = call_child(context, 383U,
        0x0001f1ecU, 0x0002043cU, 0x0001f1f0U);
    if (transform.status != TranslationStatus::complete || transform.control != 1U)
        return transform;

    set_data_word(r, 5U, static_cast<std::uint16_t>(r.data[7]));
    write_long(host, slot + 0x1eU, r.data[0]);
    set_logic_flags(r, r.data[0], 0x80000000U, 0xffffffffU);
    write_long(host, slot + 0x26U, r.data[1]);
    set_logic_flags(r, r.data[1], 0x80000000U, 0xffffffffU);

    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
