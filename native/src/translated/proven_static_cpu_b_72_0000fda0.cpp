#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kPrivateMask = 0x0003ffffU;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const auto offset = address & kPrivateMask;
    const bool odd = (offset & 1U) != 0U;
    return {offset & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U),
        odd ? 0U : 8U};
}

[[nodiscard]] std::uint16_t read_word(
    ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(
        kPrivateRegion, address & kPrivateMask, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value)
{
    host.write_memory_word(
        kPrivateRegion, address & kPrivateMask, value, kWordMask);
}

[[nodiscard]] std::uint8_t read_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(kPrivateRegion,
        location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address,
    std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kPrivateRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

[[nodiscard]] std::uint32_t read_long(
    ExecutionHost &host, std::uint32_t address)
{
    const auto high = read_word(host, address);
    const auto low = read_word(host, address + 2U);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}



void set_logic_flags(CpuRegisters &registers,
    std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}



void set_add_word_flags(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source, std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(destination) + source > 0xffffU)
        flags |= 0x0011U;
    if (((~(destination ^ source)) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}



void set_sub_word_flags(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source, std::uint16_t result)
{
    std::uint16_t flags{};
    if (source > destination) flags |= 0x0011U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}



[[nodiscard]] FunctionResult finish(FunctionContext &context)
{
    auto &registers = context.registers;
    const auto target = read_long(*context.host, registers.address[7]);
    registers.address[7] += 4U;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0000fda0(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x0000fda0U)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    r.data[0] = 0U;
    set_logic_flags(r, 0U, 0x80000000U);
    const auto retained = read_byte(host, r.address[4] + 0x40U);
    r.data[0] = retained;
    set_logic_flags(r, retained, 0x80U);
    const auto total = read_word(host, 0x0c10U);
    const auto sum = static_cast<std::uint16_t>(total + retained);
    write_word(host, 0x0c10U, sum);
    set_add_word_flags(r, total, retained, sum);
    (void)read_word(host, 0x0c12U);
    write_word(host, 0x0c12U, 0U);
    set_logic_flags(r, 0U, 0x8000U);
    auto counter = static_cast<std::uint16_t>(retained + 1U);
    r.data[0] = counter;
    set_add_word_flags(r, retained, 1U, counter);
    for (;;) {
        const auto index = static_cast<std::int16_t>(counter);
        const auto value = read_byte(host, r.address[4] + 0x40U + index);
        write_byte(host, r.address[4] + index, value);
        set_logic_flags(r, value, 0x80U);
        counter = static_cast<std::uint16_t>(counter - 1U);
        r.data[0] = (r.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }
    (void)read_word(host, r.address[4] + 0x40U);
    write_word(host, r.address[4] + 0x40U, 0U);
    set_logic_flags(r, 0U, 0x8000U);
    r.data[1] = 0U;
    set_logic_flags(r, 0U, 0x80000000U);
    const auto count = read_byte(host, r.address[4]);
    r.data[1] = count;
    set_logic_flags(r, count, 0x80U);
    auto outer = static_cast<std::uint16_t>(count - 1U);
    r.data[1] = outer;
    set_sub_word_flags(r, count, 1U, outer);
    if (outer == 0U) return finish(context);
    const auto before = outer;
    outer = static_cast<std::uint16_t>(outer - 1U);
    r.data[1] = outer;
    set_sub_word_flags(r, before, 1U, outer);
    for (;;) {
        auto inner = outer;
        r.data[2] = (r.data[2] & 0xffff0000U) | inner;
        set_logic_flags(r, inner, 0x8000U);
        r.address[0] = r.address[4] + 2U;
        for (;;) {
            const auto left = read_byte(host, r.address[0]);
            ++r.address[0];
            r.data[0] = (r.data[0] & 0xffffff00U) | left;
            set_logic_flags(r, left, 0x80U);
            const auto right = read_byte(host, r.address[0]);
            const auto result = static_cast<std::uint8_t>(left - right);
            std::uint16_t flags = r.status & 0x0010U;
            if (left < right) flags |= 0x0001U;
            if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
            if (result == 0U) flags |= 0x0004U;
            if ((result & 0x80U) != 0U) flags |= 0x0008U;
            r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
            if (left >= right) {
                const auto value = read_byte(host, r.address[0]);
                write_byte(host, r.address[0] - 1U, value);
                set_logic_flags(r, value, 0x80U);
                write_byte(host, r.address[0], static_cast<std::uint8_t>(r.data[0]));
                set_logic_flags(r, static_cast<std::uint8_t>(r.data[0]), 0x80U);
            }
            inner = static_cast<std::uint16_t>(inner - 1U);
            r.data[2] = (r.data[2] & 0xffff0000U) | inner;
            if (inner == 0xffffU) break;
        }
        outer = static_cast<std::uint16_t>(outer - 1U);
        r.data[1] = (r.data[1] & 0xffff0000U) | outer;
        if (outer == 0xffffU) break;
    }
    return finish(context);
}

} // namespace gain_ground::translated
