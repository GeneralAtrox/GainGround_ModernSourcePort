#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;

void set_move_word_flags(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_move_long_flags(CpuRegisters &r, std::uint32_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kRegion, address, static_cast<std::uint16_t>(value >> 16U), kMask);
    host.write_memory_word(kRegion, address + 2U, static_cast<std::uint16_t>(value), kMask);
}

void move_long(ExecutionHost &host, CpuRegisters &r)
{
    const auto value = read_long(host, r.address[1]);
    r.address[1] += 4U;
    write_long(host, r.address[0], value);
    r.address[0] += 4U;
    set_move_long_flags(r, value);
}

void push_long(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    write_long(host, r.address[7], value);
}

[[nodiscard]] std::uint32_t pop_long(ExecutionHost &host, CpuRegisters &r)
{
    const auto value = read_long(host, r.address[7]);
    r.address[7] += 4U;
    return value;
}

void call(ExecutionHost &host, FunctionContext &context, std::uint32_t id,
    std::uint32_t callsite, std::uint32_t target, std::uint32_t return_pc)
{
    push_long(host, context.registers, return_pc);
    context.registers.program_counter = target;
    (void)host.call_function(id, 1U, 0x72U, 2U, callsite, target, context);
}
} // namespace

FunctionResult cpu_b_update_record_state_fields(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;

    r.address[1] = 0x0000acd6U;
    r.address[0] = 0x00007416U;
    r.data[0] = 9U;
    set_move_long_flags(r, 9U);
    for (unsigned count = 0; count != 10U; ++count)
        move_long(host, r);
    r.data[0] = (r.data[0] & 0xffff0000U) | 0xffffU;

    write_long(host, 0x000074c6U, 3U);
    set_move_long_flags(r, 3U);
    write_long(host, 0x000074deU, 3U);
    set_move_long_flags(r, 3U);
    call(host, context, 145U, 0x0000a7f4U, 0x0000a85cU, 0x0000a7f8U);
    call(host, context, 146U, 0x0000a7f8U, 0x0000a86cU, 0x0000a7fcU);

    const auto flags = static_cast<std::uint8_t>(
        host.read_memory_word(kRegion, 0x00000820U, 0xff00U) >> 8U);
    if ((flags & 0x08U) == 0U) {
        r.status = static_cast<std::uint16_t>(r.status | 0x0004U);
    } else {
        r.status = static_cast<std::uint16_t>(r.status & ~0x0004U);
        r.address[0] = 0x0000747aU;
        r.address[1] = 0x0000abe4U;
        for (unsigned count = 0; count != 4U; ++count)
            move_long(host, r);

        auto index = host.read_memory_word(kRegion, 0x00000c00U, kMask);
        r.data[0] = (r.data[0] & 0xffff0000U) | index;
        set_move_word_flags(r, index);
        index = static_cast<std::uint16_t>(index << 2U);
        r.data[0] = (r.data[0] & 0xffff0000U) | index;
        r.address[1] = 0x0000ac6aU;
        const auto selected = read_long(host, r.address[1] + static_cast<std::int16_t>(index));
        write_long(host, r.address[0], selected);
        r.address[0] += 4U;
        set_move_long_flags(r, selected);

        r.data[0] = 0U;
        set_move_long_flags(r, 0U);
        auto decimal = host.read_memory_word(kRegion, 0x00000c02U, kMask);
        r.data[0] = decimal;
        set_move_word_flags(r, decimal);
        decimal = static_cast<std::uint16_t>(decimal + 1U);
        r.data[0] = decimal;
        const auto quotient = static_cast<std::uint16_t>(r.data[0] / 10U);
        const auto remainder = static_cast<std::uint16_t>(r.data[0] % 10U);
        r.data[0] = (static_cast<std::uint32_t>(remainder) << 16U) | quotient;
        r.data[0] = (static_cast<std::uint32_t>(quotient) << 16U) | remainder;
        r.address[1] = 0x0000ac04U;
        set_move_word_flags(r, remainder);
        if (remainder == 0U) {
            for (unsigned count = 0; count != 3U; ++count)
                move_long(host, r);
        } else {
            r.address[1] = 0x0000abf4U;
            for (unsigned count = 0; count != 4U; ++count)
                move_long(host, r);
            const auto table_index = static_cast<std::uint16_t>((remainder - 1U) << 2U);
            r.data[0] = (r.data[0] & 0xffff0000U) | table_index;
            r.address[1] = 0x0000ac6aU;
            const auto value = read_long(host, r.address[1] + static_cast<std::int16_t>(table_index));
            write_long(host, r.address[0], value);
            r.address[0] += 4U;
            set_move_long_flags(r, value);
        }
    }

    const auto return_address = pop_long(host, r);
    r.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}
} // namespace gain_ground::translated
