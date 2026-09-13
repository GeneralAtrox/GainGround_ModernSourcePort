#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    return {address & 0x0003fffeU,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U),
        odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kPrivateRegion, location.offset, location.mask) >> location.shift);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kPrivateRegion,
        address & 0x0003ffffU, kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion,
        (address + 2U) & 0x0003ffffU, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_logic_flags(CpuRegisters &r, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub_word_flags(CpuRegisters &r, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (left < right) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void lsl_word_four(CpuRegisters &r)
{
    const auto before = static_cast<std::uint16_t>(r.data[7]);
    const auto result = static_cast<std::uint16_t>(before << 4U);
    r.data[7] = (r.data[7] & 0xffff0000U) | result;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if ((before & 0x1000U) != 0U) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void lsr_byte_one(CpuRegisters &r)
{
    const auto before = static_cast<std::uint8_t>(r.data[4]);
    const auto result = static_cast<std::uint8_t>(before >> 1U);
    r.data[4] = (r.data[4] & 0xffffff00U) | result;
    std::uint16_t flags{};
    if (result == 0U) flags |= 0x0004U;
    if ((before & 1U) != 0U) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] FunctionResult dispatch(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t kind,
    std::uint32_t callsite, std::uint32_t target)
{
    context.registers.program_counter = target;
    return context.host->call_function(function_id, 1U, 0x72U, kind,
        callsite, target, context);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001f0b8(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    r.address[0] = read_long(host, base + 0x66U);

    const auto initial = host.read_memory_word(kPrivateRegion,
        (base + 0x5cU) & 0x0003ffffU, kWordMask);
    r.data[5] = (r.data[5] & 0xffff0000U) | initial;
    set_logic_flags(r, initial, 0x8000U, 0xffffU);
    host.write_memory_word(kPrivateRegion,
        (base + 0x3aU) & 0x0003ffffU, initial, kWordMask);
    set_logic_flags(r, initial, 0x8000U, 0xffffU);

    r.data[6] = 0U;
    set_logic_flags(r, 0U, 0x80000000U, 0xffffffffU);
    r.data[7] = 0U;
    set_logic_flags(r, 0U, 0x80000000U, 0xffffffffU);

    const auto count = read_byte(host, r.address[0] + 0x14U);
    r.data[6] = (r.data[6] & 0xffffff00U) | count;
    set_logic_flags(r, count, 0x80U, 0xffU);
    const auto step = read_byte(host, r.address[0] + 0x15U);
    r.data[7] = (r.data[7] & 0xffffff00U) | step;
    set_logic_flags(r, step, 0x80U, 0xffU);
    lsl_word_four(r);

    const auto before = static_cast<std::uint16_t>(r.data[6]);
    const auto decremented = static_cast<std::uint16_t>(before - 1U);
    r.data[6] = (r.data[6] & 0xffff0000U) | decremented;
    set_sub_word_flags(r, before, 1U, decremented);
    if (decremented == 0U)
        return dispatch(context, 610U, 1U, 0x0001f0d4U, 0x0001f0e6U);

    const auto moved = static_cast<std::uint8_t>(r.data[6]);
    r.data[4] = (r.data[4] & 0xffffff00U) | moved;
    set_logic_flags(r, moved, 0x80U, 0xffU);
    lsr_byte_one(r);
    return dispatch(context, 609U, 0U, 0x0001f0d8U, 0x0001f0daU);
}

} // namespace gain_ground::translated
