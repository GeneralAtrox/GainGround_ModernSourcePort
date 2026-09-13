#include "gain_ground/contract_types.h"
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;
struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] constexpr ByteLocation locate(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U, static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U),
        odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &h, std::uint32_t address)
{
    const auto l = locate(address);
    return static_cast<std::uint8_t>(h.read_memory_word(kRegion, l.offset, l.mask) >> l.shift);
}

void write_byte(ExecutionHost &h, std::uint32_t address, std::uint8_t value)
{
    const auto l = locate(address);
    h.write_memory_word(kRegion, l.offset,
        static_cast<std::uint16_t>(value) << l.shift, l.mask);
}

void logic(CpuRegisters &r, std::uint32_t value, std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void subtract(CpuRegisters &r, std::uint16_t destination,
    std::uint16_t source, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (destination < source) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &h, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    h.write_memory_word(kRegion, r.address[7], static_cast<std::uint16_t>(value >> 16U), kMask);
    h.write_memory_word(kRegion, r.address[7] + 2U, static_cast<std::uint16_t>(value), kMask);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &h, CpuRegisters &r)
{
    const auto high = h.read_memory_word(kRegion, r.address[7], kMask);
    const auto low = h.read_memory_word(kRegion, r.address[7] + 2U, kMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_copy_record_cursor(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;
    const auto record = r.address[5];

    auto word = h.read_memory_word(kRegion, record + 0x5cU, kMask);
    h.write_memory_word(kRegion, record + 0x3aU, word, kMask);
    logic(r, word, 0x8000U, 0xffffU);

    push_return(h, r, 0x0001da22U);
    r.program_counter = 0x0001da54U;
    const auto child = h.call_function(346U, 1U, 0x72U, 2U,
        0x0001da20U, 0x0001da54U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U
        || child.exit_program_counter != 0x0001da22U)
        return child;

    word = h.read_memory_word(kRegion, record + 0x3aU, kMask);
    h.write_memory_word(kRegion, record + 0x5cU, word, kMask);
    logic(r, word, 0x8000U, 0xffffU);
    const auto d1 = static_cast<std::uint16_t>(r.data[1]);
    h.write_memory_word(kRegion, record + 0x3aU, d1, kMask);
    logic(r, d1, 0x8000U, 0xffffU);

    const auto cursor = h.read_memory_word(kRegion, record + 0x5cU, kMask);
    r.data[0] = (r.data[0] & 0xffff0000U) | cursor;
    logic(r, cursor, 0x8000U, 0xffffU);
    write_byte(h, record + 0x39U, 0x10U);
    logic(r, 0x10U, 0x80U, 0xffU);

    const auto difference = static_cast<std::uint16_t>(d1 - cursor);
    r.data[1] = (r.data[1] & 0xffff0000U) | difference;
    subtract(r, d1, cursor, difference);
    const auto selected = static_cast<std::uint16_t>(difference & 0x0400U);
    r.data[1] = (r.data[1] & 0xffff0000U) | selected;
    logic(r, selected, 0x8000U, 0xffffU);
    if (selected != 0U) {
        write_byte(h, record + 0x39U, 0xf0U);
        logic(r, 0xf0U, 0x80U, 0xffU);
    }

    const auto flags = read_byte(h, record + 0x41U);
    write_byte(h, record + 0x41U, static_cast<std::uint8_t>(flags | 0x10U));
    if ((flags & 0x10U) != 0U) r.status = static_cast<std::uint16_t>(r.status & ~0x0004U);
    else r.status = static_cast<std::uint16_t>(r.status | 0x0004U);

    const auto phase = read_byte(h, record + 0x37U);
    r.data[0] = (r.data[0] & 0xffffff00U) | phase;
    logic(r, phase, 0x80U, 0xffU);
    write_byte(h, record + 0x36U, phase);
    logic(r, phase, 0x80U, 0xffU);

    const auto target = pop_return(h, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
