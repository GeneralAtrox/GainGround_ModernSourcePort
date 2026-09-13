#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivate = 2U;
constexpr std::uint16_t kMask = 0xffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };
ByteLocation byte_location(std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U, static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}
void write_byte(ExecutionHost &h, std::uint32_t address, std::uint8_t value)
{
    const auto b = byte_location(address);
    h.write_memory_word(kPrivate, b.offset, static_cast<std::uint16_t>(value) << b.shift, b.mask);
}
void set_logic(CpuRegisters &r, std::uint32_t value, std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}
void set_sub_word(CpuRegisters &r, std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((result & 0x8000U) != 0U) flags |= 8U;
    if (result == 0U) flags |= 4U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 2U;
    if (right > left) flags |= 0x0001U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}
void push_return(ExecutionHost &h, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    h.write_memory_word(kPrivate, r.address[7], static_cast<std::uint16_t>(value >> 16U), kMask);
    h.write_memory_word(kPrivate, r.address[7] + 2U, static_cast<std::uint16_t>(value), kMask);
}
std::uint32_t pop_return(ExecutionHost &h, CpuRegisters &r)
{
    const auto hi = h.read_memory_word(kPrivate, r.address[7], kMask);
    const auto lo = h.read_memory_word(kPrivate, r.address[7] + 2U, kMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(hi) << 16U) | lo;
}
} // namespace

FunctionResult cpu_b_populate_object_descriptor(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;
    auto &a0 = r.address[0];
    const auto a6 = r.address[6];

    const auto move_source_word = [&](std::uint32_t destination) {
        const auto value = h.read_memory_word(kPrivate, a0, kMask); a0 += 2U;
        h.write_memory_word(kPrivate, destination, value, kMask);
        set_logic(r, value, 0x8000U, 0xffffU);
        return value;
    };
    move_source_word(a6 + 0x12U);
    h.write_memory_word(kPrivate, a6 + 0x16U, 8U, kMask); set_logic(r, 8U, 0x8000U, 0xffffU);
    move_source_word(a6 + 0x1aU);
    const auto source_d0 = h.read_memory_word(kPrivate, a0, kMask); a0 += 2U;
    r.data[0] = (r.data[0] & 0xffff0000U) | source_d0; set_logic(r, source_d0, 0x8000U, 0xffffU);
    write_byte(h, a6 + 0x4bU, static_cast<std::uint8_t>(source_d0)); set_logic(r, source_d0, 0x80U, 0xffU);
    write_byte(h, a6 + 0x3dU, 1U); set_logic(r, 1U, 0x80U, 0xffU);
    h.write_memory_word(kPrivate, a6 + 0x58U, 2U, kMask); set_logic(r, 2U, 0x8000U, 0xffffU);
    h.write_memory_word(kPrivate, a6 + 0x52U, 9U, kMask); set_logic(r, 9U, 0x8000U, 0xffffU);
    for (const auto offset : {0U, 0x80U, 0x100U, 0x180U}) {
        write_byte(h, a6 + offset, 0x80U); set_logic(r, 0x80U, 0x80U, 0xffU);
    }

    const auto stage = h.read_memory_word(kPrivate, 0x00000c02U, kMask);
    set_sub_word(r, stage, 0x0010U, static_cast<std::uint16_t>(stage - 0x0010U));
    if (stage == 0x0010U) {
        push_return(h, r, 0x0001373eU);
        r.program_counter = 0x00015e28U;
        const auto child = h.call_function(283U, 1U, 0x72U, 2U,
            0x00013738U, 0x00015e28U, context);
        if (child.status != TranslationStatus::complete || child.control != 1U) return child;
        auto d0 = static_cast<std::uint16_t>(r.data[0]) & 0x001cU;
        r.data[0] = (r.data[0] & 0xffff0000U) | d0; set_logic(r, d0, 0x8000U, 0xffffU);
        auto value = h.read_memory_word(kPrivate,
            static_cast<std::uint32_t>(static_cast<std::int64_t>(a0) + static_cast<std::int16_t>(d0)), kMask);
        h.write_memory_word(kPrivate, a6 + 0x12U, value, kMask); set_logic(r, value, 0x8000U, 0xffffU);
        value = h.read_memory_word(kPrivate,
            static_cast<std::uint32_t>(static_cast<std::int64_t>(a0) + static_cast<std::int16_t>(d0) + 2), kMask);
        h.write_memory_word(kPrivate, a6 + 0x1aU, value, kMask); set_logic(r, value, 0x8000U, 0xffffU);
        a0 = static_cast<std::uint32_t>(static_cast<std::int64_t>(a0) + 0x20);
    }

    const auto target = pop_return(h, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
