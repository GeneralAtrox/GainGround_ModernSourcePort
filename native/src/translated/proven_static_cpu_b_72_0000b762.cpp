#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kFlagC = 0x0001U;
constexpr std::uint16_t kFlagV = 0x0002U;
constexpr std::uint16_t kFlagZ = 0x0004U;
constexpr std::uint16_t kFlagN = 0x0008U;
constexpr std::uint16_t kFlagX = 0x0010U;

void set_logic_word(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & kFlagX;
    if ((value & 0x8000U) != 0U) flags |= kFlagN;
    if (value == 0U) flags |= kFlagZ;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_logic_long(CpuRegisters &r, std::uint32_t value)
{
    std::uint16_t flags = r.status & kFlagX;
    if ((value & 0x80000000U) != 0U) flags |= kFlagN;
    if (value == 0U) flags |= kFlagZ;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_compare_word(CpuRegisters &r, std::uint16_t left,
                      std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left - right);
    std::uint16_t flags = r.status & kFlagX;
    if (left < right) flags |= kFlagC;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= kFlagV;
    if ((result & 0x8000U) != 0U) flags |= kFlagN;
    if (result == 0U) flags |= kFlagZ;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t read_long(
    ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(
        kPrivateRegion, address, kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address,
                std::uint32_t value)
{
    host.write_memory_word(kPrivateRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] FunctionResult finish_return(
    ExecutionHost &host, CpuRegisters &r)
{
    const auto target = read_long(host, r.address[7]);
    r.address[7] += 4U;
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0000b762(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    if (r.program_counter != 0x0000b762U)
        return {TranslationStatus::contract_violation, 0U,
            r.program_counter};

    const auto selector = host.read_memory_word(
        kPrivateRegion, r.address[5] + 0x0cU, kWordMask);
    set_compare_word(r, selector, 0x000eU);
    if ((r.status & kFlagN) == 0U)
        return finish_return(host, r);

    r.address[0] = 0x00000504U;
    r.address[1] = 0x0000c5b6U;

    const auto first = host.read_memory_word(
        kPrivateRegion, r.address[1], kWordMask);
    r.address[1] += 2U;
    host.write_memory_word(kPrivateRegion, r.address[0], first, kWordMask);
    r.address[0] += 2U;
    set_logic_word(r, first);

    const auto second = read_long(host, r.address[1]);
    r.address[1] += 4U;
    write_long(host, r.address[0], second);
    r.address[0] += 4U;
    set_logic_long(r, second);

    const auto third = read_long(host, r.address[1]);
    r.address[1] += 4U;
    write_long(host, r.address[0], third);
    r.address[0] += 4U;
    set_logic_long(r, third);

    return finish_return(host, r);
}

} // namespace gain_ground::translated
