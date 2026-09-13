#include "gain_ground/contract_types.h"
#include "gground_functions.h"

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
} // namespace

FunctionResult cpu_b_record_flag2_gate(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    const auto flags_address = base + 0x40U;
    const auto flags = read_byte(host, flags_address);
    set_zero_only(r, (flags & 0x04U) == 0U);

    if ((flags & 0x04U) != 0U) {
        const auto mode = host.read_memory_word(kRegion, base + 0x5cU, kWordMask);
        r.data[1] = (r.data[1] & 0xffff0000U) | mode;
        set_logic_flags(r, mode, 0x8000U, 0xffffU);

        const auto lookup = call_child(context, 306U,
            0x0001dbd0U, 0x00016372U, 0x0001dbd6U);
        if (lookup.status != TranslationStatus::complete
            || (lookup.control != 1U && lookup.control != 3U))
            return lookup;
        if (lookup.control == 3U) {
            const auto continuation = proven_static_cpu_b_72_00016376(context);
            if (continuation.status != TranslationStatus::complete
                || continuation.control != 1U)
                return continuation;
        }

        r.data[4] = 0U;
        set_logic_flags(r, 0U, 0x80000000U, 0xffffffffU);
        const auto direction = read_byte(host, base + 0x36U);
        r.data[4] = direction;
        set_logic_flags(r, direction, 0x80U, 0xffU);

        const auto transform = call_child(context, 383U,
            0x0001dbdcU, 0x0002043cU, 0x0001dbe0U);
        if (transform.status != TranslationStatus::complete || transform.control != 1U)
            return transform;

        write_long(host, base + 0x1eU, r.data[0]);
        set_logic_flags(r, r.data[0], 0x80000000U, 0xffffffffU);
        write_long(host, base + 0x26U, r.data[1]);
        set_logic_flags(r, r.data[1], 0x80000000U, 0xffffffffU);

        const auto current = read_byte(host, flags_address);
        set_zero_only(r, (current & 0x04U) == 0U);
        write_byte(host, flags_address,
            static_cast<std::uint8_t>(current & static_cast<std::uint8_t>(~0x04U)));
    }

    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
