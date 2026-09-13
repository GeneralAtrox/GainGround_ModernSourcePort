#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

void set_logic_flags(CpuRegisters &r, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask);

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

void clear_byte(ExecutionHost &host, CpuRegisters &r, std::uint32_t address)
{
    static_cast<void>(read_byte(host, address));
    write_byte(host, address, 0U);
    set_logic_flags(r, 0U, 0x80U, 0xffU);
}

void clear_word(ExecutionHost &host, CpuRegisters &r, std::uint32_t address)
{
    static_cast<void>(host.read_memory_word(kRegion, address, kWordMask));
    host.write_memory_word(kRegion, address, 0U, kWordMask);
    set_logic_flags(r, 0U, 0x8000U, 0xffffU);
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

void set_sub_word_flags(CpuRegisters &r,
    std::uint16_t destination, std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    std::uint16_t flags = 0U;
    if (destination < source) flags |= 0x0011U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_compare_word_flags(CpuRegisters &r,
    std::uint16_t destination, std::uint16_t source)
{
    const auto saved_extend = static_cast<std::uint16_t>(r.status & 0x0010U);
    set_sub_word_flags(r, destination, source);
    r.status = static_cast<std::uint16_t>((r.status & ~0x0010U) | saved_extend);
}

void set_add_word_flags(CpuRegisters &r,
    std::uint16_t destination, std::uint16_t source)
{
    const auto wide = static_cast<std::uint32_t>(destination) + source;
    const auto result = static_cast<std::uint16_t>(wide);
    std::uint16_t flags = 0U;
    if (wide > 0xffffU) flags |= 0x0011U;
    if (((~(destination ^ source)) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] bool greater_than(const CpuRegisters &r) noexcept
{
    const bool negative = (r.status & 0x0008U) != 0U;
    const bool zero = (r.status & 0x0004U) != 0U;
    const bool overflow = (r.status & 0x0002U) != 0U;
    return !zero && negative == overflow;
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto stack = r.address[7];
    const auto high = host.read_memory_word(kRegion, stack, kWordMask);
    const auto low = host.read_memory_word(kRegion, stack + 2U, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

FunctionResult finish(ExecutionHost &host, CpuRegisters &r)
{
    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_update_descriptor_timer(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    const auto record = r.address[5];

    const auto mode = host.read_memory_word(kRegion, 0x00000c02U, kWordMask);
    set_compare_word_flags(r, mode, 2U);
    if ((r.status & 0x0008U) == 0U) return finish(host, r);

    r.address[0] = 0x0000743eU;
    const auto descriptor = r.address[0];
    const auto state = host.read_memory_word(kRegion, record + 0x42U, kWordMask);
    set_logic_flags(r, state, 0x8000U, 0xffffU);
    if (state != 0U) {
        clear_byte(host, r, 0x00000d1aU);
        clear_word(host, r, descriptor);
        clear_word(host, r, record + 0x46U);
        return finish(host, r);
    }

    const auto active = host.read_memory_word(kRegion, record + 0x46U, kWordMask);
    set_logic_flags(r, active, 0x8000U, 0xffffU);
    if (active != 0U) {
        clear_byte(host, r, 0x00000d1aU);
        const auto timer = host.read_memory_word(kRegion, record + 0x48U, kWordMask);
        const auto decremented = static_cast<std::uint16_t>(timer - 1U);
        host.write_memory_word(kRegion, record + 0x48U, decremented, kWordMask);
        set_sub_word_flags(r, timer, 1U);
        if (greater_than(r)) return finish(host, r);
        clear_word(host, r, descriptor);
        clear_word(host, r, record + 0x46U);
        host.write_memory_word(kRegion, record + 0x48U, 0x012cU, kWordMask);
        set_logic_flags(r, 0x012cU, 0x8000U, 0xffffU);
        return finish(host, r);
    }

    const auto trigger = read_byte(host, 0x00000d1aU);
    set_logic_flags(r, trigger, 0x80U, 0xffU);
    if (trigger != 0U) {
        clear_byte(host, r, 0x00000d1aU);
        write_long(host, descriptor + 6U, 0x00013e20U);
        set_logic_flags(r, 0x00013e20U, 0x80000000U, 0xffffffffU);
    } else {
        const auto timer = host.read_memory_word(kRegion, record + 0x48U, kWordMask);
        const auto decremented = static_cast<std::uint16_t>(timer - 1U);
        host.write_memory_word(kRegion, record + 0x48U, decremented, kWordMask);
        set_sub_word_flags(r, timer, 1U);
        if (greater_than(r)) return finish(host, r);
        write_long(host, descriptor + 6U, 0x00013e0cU);
        set_logic_flags(r, 0x00013e0cU, 0x80000000U, 0xffffffffU);
    }

    const auto tas_value = read_byte(host, record + 0x46U);
    write_byte(host, record + 0x46U, static_cast<std::uint8_t>(tas_value | 0x80U));
    set_logic_flags(r, tas_value, 0x80U, 0xffU);
    host.write_memory_word(kRegion, record + 0x48U, 0x003cU, kWordMask);
    set_logic_flags(r, 0x003cU, 0x8000U, 0xffffU);

    auto value = host.read_memory_word(kRegion, record + 0x0cU, kWordMask);
    r.data[0] = (r.data[0] & 0xffff0000U) | value;
    set_logic_flags(r, value, 0x8000U, 0xffffU);
    const auto added = static_cast<std::uint16_t>(value + 0x20U);
    r.data[0] = (r.data[0] & 0xffff0000U) | added;
    set_add_word_flags(r, value, 0x20U);
    host.write_memory_word(kRegion, r.address[0], added, kWordMask);
    set_logic_flags(r, added, 0x8000U, 0xffffU);
    r.address[0] += 2U;

    value = host.read_memory_word(kRegion, record + 0x0eU, kWordMask);
    r.data[0] = (r.data[0] & 0xffff0000U) | value;
    set_logic_flags(r, value, 0x8000U, 0xffffU);
    const auto subtracted = static_cast<std::uint16_t>(value - 0x10U);
    r.data[0] = (r.data[0] & 0xffff0000U) | subtracted;
    set_sub_word_flags(r, value, 0x10U);
    host.write_memory_word(kRegion, r.address[0], subtracted, kWordMask);
    set_logic_flags(r, subtracted, 0x8000U, 0xffffU);
    r.address[0] += 2U;
    host.write_memory_word(kRegion, r.address[0], 0x080dU, kWordMask);
    set_logic_flags(r, 0x080dU, 0x8000U, 0xffffU);
    return finish(host, r);
}

} // namespace gain_ground::translated
